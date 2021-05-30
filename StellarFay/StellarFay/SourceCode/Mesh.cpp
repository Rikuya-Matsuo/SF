#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Vector.h"
#include "Renderer.h"
#include "GameSystem.h"

VertexArray::ArgumentPreset Mesh::mVAPreset;

Mesh::Mesh()
{
	static bool firstLoad = true;
	if (firstLoad)
	{
		mVAPreset.mType = GL_FLOAT;
		mVAPreset.mNormalize = GL_FALSE;
		mVAPreset.SetAttributeSizes(3, 3, 2, 3);

		firstLoad = false;
	}
}

Mesh::~Mesh()
{
	// リソース割り当ての解除
	if (mVertexArray)
	{
		delete mVertexArray;
	}

	for (auto itr = mObjects.begin(); itr != mObjects.end(); ++itr)
	{
		delete *itr;
	}

	for (auto itr = mMtlDatas.begin(); itr != mMtlDatas.end(); ++itr)
	{
		delete *itr;
	}
}

bool Mesh::Load(const std::string & path)
{
	/////////////////////////
	// 変数・コンテナの用意
	/////////////////////////
	LoadLocalVariable local;

	bool success;

	/////////////////////////
	// objファイルのロード
	/////////////////////////
	success = LoadObj(local, path);

	if (!success)
	{
		return false;
	}

	/////////////////////////////
	// mtlファイルのロード
	/////////////////////////////
	success = LoadMtl(local, path);

	if (!success)
	{
		return false;
	}

	/////////////////////////////
	// OpenGLに組み込む
	/////////////////////////////
	mVertexArray = new VertexArray(mVertices.data(), mVertices.size(), mVAPreset);

	// ポリゴングループごとにインデックスバッファを作成
	for (auto objItr : mObjects)
	{
		for (auto polygItr : objItr->mPolyGroups)
		{
			// エイリアス取得
			LoadLocalVariable::UintVec & vecAlies = local.indexBuffer[polygItr];

			polygItr->mElementBuffer = new ElementBuffer(vecAlies.data(), vecAlies.size(), sizeof(vecAlies[0]));
		}
	}

	return true;
}

void Mesh::DrawFullDissolve(Shader * shader) const
{
	// ディゾルブが１であるか否かを調べるラムダ式
	auto isFullDissolve = [this](ObjectData * obj, size_t polygIndex) -> bool
	{
		const Mesh::MtlData * mtl = obj->mPolyGroups[polygIndex]->mUsemtl;
		return (mtl->mDissolve == 1.0f);
	};

	DrawUnderCondition(shader, isFullDissolve);
}

void Mesh::DrawNotFullDissolve(Shader * shader) const
{
	// ディゾルブが１未満であるか否かを調べるラムダ式
	auto isNotFullDissolve = [this](ObjectData * obj, size_t polygIndex) -> bool
	{
		const Mesh::MtlData * mtl = obj->mPolyGroups[polygIndex]->mUsemtl;
		return (mtl->mDissolve < 1.0f);
	};

	DrawUnderCondition(shader, isNotFullDissolve);
}

void Mesh::SetObjectDrawFlag(const std::string & objectName, bool value)
{
	// 指定された名前でオブジェクトを検索
	ObjectData * obj = FindObjectWithName(objectName);
	if (!obj)
	{
		return;
	}

	// 描画フラグをいじる
	obj->mDrawFlag = value;
}

AABB Mesh::GetObjectAABB(const std::string & objectName) const
{
	ObjectData * obj = FindObjectWithName(objectName);
	return obj->mBox;
}

AABB Mesh::GetBiggestAABB() const
{
	// AABBの体積が最も大きいオブジェクトを探す
	ObjectData * biggestObj = nullptr;
	float maxVolume = 0.0f;

	for (auto itr : mObjects)
	{
		// 体積を計算
		float volume = itr->mBox.CalculateVolume();

		if (volume > maxVolume)
		{
			biggestObj = itr;
			volume = maxVolume;
		}
	}

	return biggestObj->mBox;
}

void Mesh::GetWordsInLine(const char * line, std::vector<std::string> & wordsInLine)
{
	// バッファ
	std::string buf;

	for (const char * ptr = line; *ptr != '\0'; ++ptr)
	{
		// 半角スペース以外の文字はバッファに格納
		if (*ptr != ' ')
		{
			buf += *ptr;
		}
		// ptrが半角スペースを指した場合、wordsInLineにバッファを挿入
		// バッファが空文字列（ファイル内容にて半角スペースが2つ連続で記述されていた場合）のときは処理を行わない
		else if (buf != "")
		{
			wordsInLine.emplace_back(buf);

			// バッファをクリアする
			buf.clear();
		}
	}

	// バッファが空文字列でないフラグ
	bool bufIsNotEmpty = !buf.empty();

	// バッファは空文字列だが、wordsInLineも空文字列であるフラグ
	bool bufAndWILisEmpty = (buf.empty() && wordsInLine.empty());

	// ループ終了後にバッファに残された文字列も格納する
	// 空文字列は格納しないが、wordsInLineが空の場合は空文字でも格納する
	if (bufIsNotEmpty || bufAndWILisEmpty)
	{
		wordsInLine.emplace_back(buf);
	}
}

// バッファから特定の文字を全て取り除くラムダ式
// をキャプチャして使う予定だったが、char*型変数はキャプチャされたとき自動的にconst char *に変換されるらしく、
// 素直に引数として使うことにした
// LoadObj, LoadMtl内で使う
auto eliminateLetter = [](char * str, char letter)
{
	char * writerPtr;
	char * checkerPtr;
	writerPtr = checkerPtr = str;

	while (*checkerPtr != '\0')
	{
		if (*checkerPtr == letter)
		{
			checkerPtr++;
			continue;
		}
		else
		{
			*writerPtr = *checkerPtr;
		}

		writerPtr++;
		checkerPtr++;
	}

	*writerPtr = '\0';
};

bool Mesh::LoadObj(LoadLocalVariable & local, const std::string & path)
{
	// 読み込み
	std::ifstream objFile(path);
	if (objFile.fail())
	{
		printf("メッシュデータのロードに失敗 : %s\n", path.c_str());
		return false;
	}

	// 文字列ストリームに流し込む
	std::stringstream content;
	content << objFile.rdbuf();

	// ファイルを閉じる
	objFile.close();

	// オブジェクトデータの初期値を生成
	ObjectData * newObjData = new ObjectData;
	mObjects.emplace_back(newObjData);

	// 内容の解析
	while (content.getline(local.buf, local.bufSize))
	{
		// タブ文字を削除
		eliminateLetter(local.buf, '\t');

		// １行を半角スペースで分解する
		GetWordsInLine(local.buf, local.wordsInLine);

		if (local.wordsInLine[0] == "v")
		{
			Vector3D pos;
			pos.x = std::stof(local.wordsInLine[1]);
			pos.y = std::stof(local.wordsInLine[2]);
			pos.z = std::stof(local.wordsInLine[3]);

			local.vertexPos.emplace_back(pos);

			// AABBの頂点情報を更新
			mObjects.back()->mBox.UpdateVertex(pos);
		}
		else if (local.wordsInLine[0] == "vt")
		{
			Vector2D uv;
			uv.x = std::stof(local.wordsInLine[1]);
			uv.y = std::stof(local.wordsInLine[2]);

			local.texCoord.emplace_back(uv);
		}
		else if (local.wordsInLine[0] == "vn")
		{
			Vector3D n;
			n.x = std::stof(local.wordsInLine[1]);
			n.y = std::stof(local.wordsInLine[2]);
			n.z = std::stof(local.wordsInLine[3]);

			local.norm.emplace_back(n);
		}
		else if (local.wordsInLine[0] == "f")
		{
			std::vector<ObjPolyVertData> verts;

			// 頂点の数は、wordsInLineから"f"の分を引いた数
			const unsigned int vertsNum = local.wordsInLine.size() - 1;
			for (unsigned int i = 0; i < vertsNum; ++i)
			{
				ObjPolyVertData v;
				v.Load(local.wordsInLine[i + 1]);

				verts.emplace_back(v);
			}

			// 頂点をポリゴンデータに格納
			ObjPolyData poly;
			poly.mVerts[0] = verts[0];

			// 多角形の中に作れる三角形の数 = 頂点の数 - 2 より
			const int triangleNum = vertsNum - 2;

			// 0番の頂点を基点に、多角形を三角形で刻んでポリゴンとして記録
			for (int i = 0; i < triangleNum; ++i)
			{
				poly.mVerts[1] = verts[i + 1];
				poly.mVerts[2] = verts[i + 2];

				// 所属するオブジェクトとポリゴングループのインデックスを記録
				// 常に最新のものに所属させる
				poly.mObjectIndex = mObjects.size() - 1;
				poly.mPolyGroupIndex = mObjects.back()->mPolyGroups.size() - 1;

				// 直前のusemtlオプションを保存
				poly.mUsemtl = local.lastUsemtlWord;

				local.polyDatas.emplace_back(poly);
			}
		}
		else if (local.wordsInLine[0] == "o")
		{
			// 一番最初のオブジェクトに名称が設定されている場合、この行は二回目以降に読み込まれた"o"オプションである
			if (!mObjects[0]->mName.empty())
			{
				newObjData = new ObjectData;
				newObjData->mName = local.wordsInLine[1];

				mObjects.emplace_back(newObjData);
			}
			// 一番最初のオブジェクトが名称未設定であった場合、この行は初めて読み込まれた"o"オプションである
			else
			{
				mObjects[0]->mName = local.wordsInLine[1];
			}
		}
		else if (local.wordsInLine[0] == "g")
		{
			// オブジェクトのエイリアス取得
			ObjectData * lastObj = mObjects.back();

			// 一番最初のポリゴングループに名称が設定されている場合、この行は二回目以降に読み込まれた"g"オプションである
			if (!lastObj->mPolyGroups[0]->mName.empty())
			{
				ObjectData::PolyGroup * polyGroup = new ObjectData::PolyGroup;
				polyGroup->mName = local.wordsInLine[1];

				lastObj->mPolyGroups.emplace_back(polyGroup);
			}
			// この行が初めて読み込まれた"g"オプションである場合
			else
			{
				lastObj->mPolyGroups[0]->mName = local.wordsInLine[1];
			}
		}
		else if (local.wordsInLine[0] == "mtllib")
		{
			// 使うmtlデータの名前を保存し、後でロードする
			local.mtllibStrings.emplace_back(local.wordsInLine[1]);
		}
		else if (local.wordsInLine[0] == "usemtl")
		{
			local.lastUsemtlWord = local.wordsInLine[1];
		}

		local.wordsInLine.clear();
	}
	mObjects.shrink_to_fit();
	for (auto itr = mObjects.begin(); itr != mObjects.end(); ++itr)
	{
		(*itr)->mPolyGroups.shrink_to_fit();
	}

	// 頂点データを作成していく

	// 頂点配列のメモリ領域を確保する
	mVertices.reserve(local.polyDatas.size() * 3 * 8);

	// ポリゴンループ
	for (auto polyItr : local.polyDatas)
	{
		// エイリアス取得
		ObjectData * objAlias = mObjects[polyItr.mObjectIndex];
		ObjectData::PolyGroup * polyGroupAlias = objAlias->mPolyGroups[polyItr.mPolyGroupIndex];

		// ポリゴンの頂点ループ
		for (size_t i = 0; i < 3; ++i)
		{
			// エイリアス取得
			ObjPolyVertData & vert = polyItr.mVerts[i];

			// vertIndicesから、同じ情報を持った頂点のインデックスを検索
			auto itr = local.vertIndices.find(vert);

			// 検索にかからなかった場合、新たにインデックスと頂点を作成
			if (itr == local.vertIndices.end())
			{
				// 位置
				GLuint newIndex = local.vertIndices.size();
				local.vertIndices[vert] = newIndex;

				int posIndex = 0;
				if (vert.mVertIndex > 0)
				{
					posIndex = vert.mVertIndex - 1;
				}
				else if (vert.mVertIndex < 0)
				{
					posIndex = local.vertexPos.size() + vert.mVertIndex;
				}
				else
				{
					std::cout << "Mesh : 面情報＞頂点座標にて、無効な値が検出されました。正常なモデル表示が行われない恐れがあります。\n";
				}

				const Vector3D & pos = local.vertexPos[posIndex];
				mVertices.emplace_back(pos.x);
				mVertices.emplace_back(pos.y);
				mVertices.emplace_back(pos.z);

				// テクスチャ座標
				int texIndex = 0;
				if (vert.mTexCoordIndex > 0)
				{
					texIndex = vert.mTexCoordIndex - 1;
				}
				else if (vert.mTexCoordIndex < 0)
				{
					texIndex = local.texCoord.size() + vert.mTexCoordIndex;
				}
				else
				{
					std::cout << "Mesh : 面情報＞テクスチャ座標にて、無効な値が検出されました。正常なモデル表示が行われない恐れがあります。\n";
				}

				Vector2D uv = local.texCoord[texIndex];
				uv.y = 1 - uv.y;
				mVertices.emplace_back(uv.x);
				mVertices.emplace_back(uv.y);

				// 法線
				int normIndex = 0;
				if (vert.mNormIndex > 0)
				{
					normIndex = vert.mNormIndex - 1;
				}
				else if (vert.mNormIndex < 0)
				{
					normIndex = local.norm.size() + vert.mNormIndex;
				}
				else
				{
					std::cout << "Mesh : 面情報＞法線にて、無効な値が検出されました。正常なモデル表示が行われない恐れがあります。\n";
				}

				const Vector3D & n = local.norm[normIndex];
				mVertices.emplace_back(n.x);
				mVertices.emplace_back(n.y);
				mVertices.emplace_back(n.z);
			}

			// 使用マテリアルデータ保存
			polyGroupAlias->mUsemtlName = polyItr.mUsemtl;

			// インデックスバッファに記録
			local.indexBuffer[polyGroupAlias].emplace_back(local.vertIndices[vert]);
		}
	}
	mVertices.shrink_to_fit();

	return true;
}

bool Mesh::LoadMtl(LoadLocalVariable & local, const std::string & path)
{
	for (auto mtllibItr : local.mtllibStrings)
	{
		// パスの最後の'/'を捜索
		size_t lastSlashIndex = path.find_last_of('/');

		// objデータから見た相対パスを、プログラムファイルから見た相対パスに変換
		std::string relativePathTmp = path;
		relativePathTmp.erase(lastSlashIndex + 1);
		std::string mtlPath = relativePathTmp + mtllibItr;

		// ファイル読み込み
		std::ifstream mtlFile(mtlPath);
		if (mtlFile.fail())
		{
			printf("マテリアルデータのロードに失敗 : %s\n", mtlPath.c_str());
			return false;
		}

		// 文字列ストリームに流し込む
		std::stringstream mtlContent;
		mtlContent << mtlFile.rdbuf();

		// ファイルを閉じる
		mtlFile.close();

		// 解析開始
		while (mtlContent.getline(local.buf, local.bufSize))
		{
			// タブ文字を削除
			eliminateLetter(local.buf, '\t');

			// １行を半角スペースで分解する
			GetWordsInLine(local.buf, local.wordsInLine);

			if (local.wordsInLine[0] == "newmtl")
			{
				MtlData * newmtl = new MtlData;

				newmtl->mName = local.wordsInLine[1];

				mMtlDatas.emplace_back(newmtl);
			}
			else if (local.wordsInLine[0] == "d")
			{
				mMtlDatas.back()->mDissolve = std::stof(local.wordsInLine[1]);
			}
			else if (local.wordsInLine[0] == "Ns")
			{
				mMtlDatas.back()->mShininess = std::stof(local.wordsInLine[1]);
			}
			else if (local.wordsInLine[0] == "map_Kd")
			{
				std::string texPath = relativePathTmp + local.wordsInLine[1];
				Texture * tex = RENDERER_INSTANCE.GetTexture(texPath);
				if (tex->Fail())
				{
					printf("map_Kd テクスチャの読み込みに失敗 : %s", texPath.c_str());
					return false;
				}

				mMtlDatas.back()->mDiffuseTexture = tex;
			}
			else if (local.wordsInLine[0] == "map_Ks")
			{
				std::string texPath = relativePathTmp + local.wordsInLine[1];
				Texture * tex = RENDERER_INSTANCE.GetTexture(texPath);
				if (tex->Fail())
				{
					printf("map_Ks テクスチャの読み込みに失敗 : %s", texPath.c_str());
					return false;
				}

				mMtlDatas.back()->mSpecularTexture = tex;
			}

			local.wordsInLine.clear();
		}
	}

	// mMtlDatasから名前を使って特定のマテリアルを検索するラムダ式
	auto FindMtlWithName = [this](const std::string & name)
	{
		MtlData * ret = nullptr;

		for (auto itr : this->mMtlDatas)
		{
			if (itr->mName == name)
			{
				ret = itr;
				break;
			}
		}

		return ret;
	};

	// 各ポリゴングループにMtlDataを結びつける
	for (auto objItr : mObjects)
	{
		for (auto polygItr : objItr->mPolyGroups)
		{
			polygItr->mUsemtl = FindMtlWithName(polygItr->mUsemtlName);
		}
	}

	return true;
}

Mesh::ObjectData * Mesh::FindObjectWithName(const std::string & objectName) const
{
	ObjectData * ret = nullptr;

	// 名称が一致するものをretに代入し、ループ終了
	for (auto itr : mObjects)
	{
		if (itr->mName == objectName)
		{
			ret = itr;
			break;
		}
	}

	return ret;
}

void Mesh::DrawUnderCondition(Shader * shader, std::function<bool(ObjectData * obj, size_t polyGroupIndex)> condition) const
{
	mVertexArray->Activate();
	for (auto objItr : mObjects)
	{
		// オブジェクトデータの描画フラグが偽のものはスキップ
		if (!objItr->mDrawFlag)
		{
			continue;
		}

		for (size_t polygIndex = 0; polygIndex < objItr->mPolyGroups.size(); ++polygIndex)
		{
			// 条件に合わないものは描画しない
			if (!condition(objItr, polygIndex))
			{
				continue;
			}

			// エイリアス取得
			ObjectData::PolyGroup * polygPtr = objItr->mPolyGroups[polygIndex];

			// テクスチャをアクティブ化
			const MtlData * mtl = polygPtr->mUsemtl;
			const Texture * tex = mtl->mDiffuseTexture;
			if (tex)
			{
				tex->Activate();
			}

			// ディゾルブをシェーダに通知
			shader->SetUniform1f("dissolve", mtl->mDissolve);

			// 描画
			polygPtr->mElementBuffer->DrawElements(GL_TRIANGLES);
		}
	}
}

void Mesh::ObjPolyVertData::Load(const std::string & str)
{
	std::string buf;

	unsigned char index = 0;

	// '/'で区切られた各数値を取得していく
	const char * ptr = str.c_str();
	bool readStrEnd = false;
	while (!readStrEnd)
	{
		if (*ptr != '/' && *ptr != '\0')
		{
			buf += *ptr;
		}
		else
		{
			// 終端文字を読んだフラグの真偽を設定
			readStrEnd = (*ptr == '\0');

			// '/'が読み込めたがbufが空文字列の場合、次の要素の設定に移る
			if (buf.empty())
			{
				index++;
				ptr++;
				continue;
			}

			// 得られた数値
			int num = std::stoi(buf);

			switch (index)
			{
			case 0:
				mVertIndex = num;
				break;
			case 1:
				mTexCoordIndex = num;
				break;
			case 2:
				mNormIndex = num;
				break;
			default:
				break;
			}
			buf.clear();

			index++;
		}

		ptr++;
	};
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
