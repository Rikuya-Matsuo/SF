#include "Mesh.h"
#include <unordered_map>
#include <fstream>
#include <sstream>
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
	std::vector<Vector3D> vertexPos;
	std::vector<Vector2D> texCoord;
	std::vector<Vector3D> norm;
	std::vector<ObjPolyData> polyDatas;
	std::vector<std::string> mtllibStrings;
	std::string lastUsemtlWord;

	// ファイル内の１行の中の、半角スペースで区切られた単語を格納する
	std::vector<std::string> wordsInLine;

	/////////////////////////
	// objファイルのロード
	/////////////////////////

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

	// バッファ
	static const unsigned int bufSize = 256;
	char buf[bufSize];

	// バッファから特定の文字を全て取り除くラムダ式
	// bufをキャプチャして使う予定だったが、char*型変数はキャプチャされたとき自動的にconst char *に変換されるらしく、
	// 素直に引数として使うことにした
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

	// 内容の解析
	while (content.getline(buf, bufSize))
	{
		// タブ文字を削除
		eliminateLetter(buf, '\t');

		// １行を半角スペースで分解する
		GetWordsInLine(buf, wordsInLine);

		if (wordsInLine[0] == "v")
		{
			Vector3D pos;
			pos.x = std::stof(wordsInLine[1]);
			pos.y = std::stof(wordsInLine[2]);
			pos.z = std::stof(wordsInLine[3]);

			vertexPos.emplace_back(pos);

			// AABBの頂点情報を更新
			mObjects.back()->mBox.UpdateVertex(pos);
		}
		else if (wordsInLine[0] == "vt")
		{
			Vector2D uv;
			uv.x = std::stof(wordsInLine[1]);
			uv.y = std::stof(wordsInLine[2]);

			texCoord.emplace_back(uv);
		}
		else if (wordsInLine[0] == "vn")
		{
			Vector3D n;
			n.x = std::stof(wordsInLine[1]);
			n.y = std::stof(wordsInLine[2]);
			n.z = std::stof(wordsInLine[3]);

			norm.emplace_back(n);
		}
		else if (wordsInLine[0] == "f")
		{
			std::vector<ObjPolyVertData> verts;

			// 頂点の数は、wordsInLineから"f"の分を引いた数
			const unsigned int vertsNum = wordsInLine.size() - 1;
			for (unsigned int i = 0; i < vertsNum; ++i)
			{
				ObjPolyVertData v;
				v.Load(wordsInLine[i + 1]);

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
				poly.mUsemtl = lastUsemtlWord;

				polyDatas.emplace_back(poly);
			}
		}
		else if (wordsInLine[0] == "o")
		{
			// 一番最初のオブジェクトに名称が設定されている場合、この行は二回目以降に読み込まれた"o"オプションである
			if (!mObjects[0]->mName.empty())
			{
				newObjData = new ObjectData;
				newObjData->mName = wordsInLine[1];
				
				mObjects.emplace_back(newObjData);
			}
			// 一番最初のオブジェクトが名称未設定であった場合、この行は初めて読み込まれた"o"オプションである
			else
			{
				mObjects[0]->mName = wordsInLine[1];
			}
		}
		else if (wordsInLine[0] == "g")
		{
			// オブジェクトのエイリアス取得
			ObjectData * lastObj = mObjects.back();

			// 一番最初のポリゴングループに名称が設定されている場合、この行は二回目以降に読み込まれた"g"オプションである
			if (!lastObj->mPolyGroups[0]->mName.empty())
			{
				ObjectData::PolyGroup * polyGroup = new ObjectData::PolyGroup;
				polyGroup->mName = wordsInLine[1];

				lastObj->mPolyGroups.emplace_back(polyGroup);
			}
			// この行が初めて読み込まれた"g"オプションである場合
			else
			{
				lastObj->mPolyGroups[0]->mName = wordsInLine[1];
			}
		}
		else if (wordsInLine[0] == "mtllib")
		{
			// 使うmtlデータの名前を保存し、後でロードする
			mtllibStrings.emplace_back(wordsInLine[1]);
		}
		else if (wordsInLine[0] == "usemtl")
		{
			lastUsemtlWord = wordsInLine[1];
		}

		wordsInLine.clear();
	}
	mObjects.shrink_to_fit();
	for (auto itr = mObjects.begin(); itr != mObjects.end(); ++itr)
	{
		(*itr)->mPolyGroups.shrink_to_fit();
		for (auto itr2 = (*itr)->mPolyGroups.begin(); itr2 != (*itr)->mPolyGroups.end(); ++itr2)
		{
			(*itr2)->mIndices.shrink_to_fit();
		}
	}

	// 頂点データを作成していく
	std::unordered_map<ObjPolyVertData, GLuint, ObjPolyVertData::HashFunc> vertIndices;
	mVertices.reserve(polyDatas.size() * 3 * 8);
	for (auto polyItr : polyDatas)
	{
		// エイリアス取得
		ObjectData * objAlias = mObjects[polyItr.mObjectIndex];
		ObjectData::PolyGroup * polyGroupAlias = objAlias->mPolyGroups[polyItr.mPolyGroupIndex];

		for (size_t i = 0; i < 3; ++i)
		{
			ObjPolyVertData & vert = polyItr.mVerts[i];

			// vertIndicesから、同じ情報を持った頂点のインデックスを検索
			auto itr = vertIndices.find(vert);

			// 検索にかからなかった場合、新たにインデックスと頂点を作成
			if (itr == vertIndices.end())
			{
				// 位置
				GLuint newIndex = vertIndices.size();
				vertIndices[vert] = newIndex;

				const Vector3D & pos = vertexPos[vert.mVertIndex - 1];
				mVertices.emplace_back(pos.x);
				mVertices.emplace_back(pos.y);
				mVertices.emplace_back(pos.z);

				// テクスチャ座標
				int texIndex = vert.mTexCoordIndex - 1;
				Vector2D uv;
				if (texIndex < 0)
				{
					uv.x = uv.y = 0.0f;
				}
				else
				{
					uv = texCoord[texIndex];
				}
				uv.y = 1 - uv.y;
				mVertices.emplace_back(uv.x);
				mVertices.emplace_back(uv.y);

				// 法線
				const Vector3D & n = norm[vert.mNormIndex - 1];
				mVertices.emplace_back(n.x);
				mVertices.emplace_back(n.y);
				mVertices.emplace_back(n.z);
			}

			// 使用マテリアルデータ保存
			polyGroupAlias->mUsemtlName = polyItr.mUsemtl;

			// インデックスバッファに記録
			polyGroupAlias->mIndices.emplace_back(vertIndices[vert]);
		}
	}
	mVertices.shrink_to_fit();

	/////////////////////////////
	// mtlファイルのロード
	/////////////////////////////
	for (auto mtllibItr : mtllibStrings)
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
		while (mtlContent.getline(buf, bufSize))
		{
			// タブ文字を削除
			eliminateLetter(buf, '\t');

			// １行を半角スペースで分解する
			GetWordsInLine(buf, wordsInLine);

			if (wordsInLine[0] == "newmtl")
			{
				MtlData * newmtl = new MtlData;

				newmtl->mName = wordsInLine[1];

				mMtlDatas.emplace_back(newmtl);
			}
			else if (wordsInLine[0] == "d")
			{
				mMtlDatas.back()->mDissolve = std::stof(wordsInLine[1]);
			}
			else if (wordsInLine[0] == "map_Kd")
			{
				std::string texPath = relativePathTmp + wordsInLine[1];
				Texture * tex = RENDERER_INSTANCE.GetTexture(texPath);
				if (tex->Fail())
				{
					printf("map_Kd テクスチャの読み込みに失敗 : %s", texPath.c_str());
					return false;
				}

				mMtlDatas.back()->mDiffuseTexture = tex;
			}
			else if (wordsInLine[1] == "map_Ks")
			{
				std::string texPath = relativePathTmp + wordsInLine[1];
				Texture * tex = RENDERER_INSTANCE.GetTexture(texPath);
				if (tex->Fail())
				{
					printf("map_Ks テクスチャの読み込みに失敗 : %s", texPath.c_str());
					return false;
				}

				mMtlDatas.back()->mSpecularTexture = tex;
			}

			wordsInLine.clear();
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

	/////////////////////////////
	// OpenGLに組み込む
	/////////////////////////////
	mVertexArray = new VertexArray(mVertices.data(), mVertices.size(), mVAPreset);

	// ポリゴングループごとにインデックスバッファを作成
	for (auto objItr : mObjects)
	{
		for (auto polygItr : objItr->mPolyGroups)
		{
			glGenBuffers(1, &polygItr->mEBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygItr->mEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * polygItr->mIndices.size(), polygItr->mIndices.data(), GL_STATIC_DRAW);
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

	// ループ終了後にバッファに残された文字列も格納する
	wordsInLine.emplace_back(buf);
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
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygPtr->mEBO);
			glDrawElements(GL_TRIANGLES, polygPtr->mIndices.size(), GL_UNSIGNED_INT, 0);
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
