#pragma once
#include "glad/glad.h"
#include "Texture.h"
#include "Shader.h"
#include "Collision.h"
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Hash.h"
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <functional>
#include <cstring>

// objファイルからモデルを読み込む
class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Load(const std::string & path);

	void DrawFullDissolve(Shader * shader) const;
	void DrawNotFullDissolve(Shader * shader) const;

	void SetObjectDrawFlag(const std::string & objectName, bool value);

	// 特定オブジェクトのAABBを返す
	AABB GetObjectAABB(const std::string & objectName) const;

	// 体積が最も大きいAABBを返す
	AABB GetBiggestAABB() const;

	friend class MeshComponent;

	// モデルをいくつかのオブジェクト・ポリゴングループに分けることで、
	// テクスチャの使い分け等を可能にする
	// データ内容：名前、当たり判定用直方体、描画フラグ、ポリゴングループのコンテナ
	class ObjectData;

	// 読み込んだmtlデータ
	// データ内容：名前、ディゾルブ値、テクスチャ
	class MtlData;

private:
	static VertexArray::ArgumentPreset mVAPreset;

	VertexArray * mVertexArray;

	// 頂点配列
	std::vector<float> mVertices;

	// オブジェクトのコンテナ
	std::vector<ObjectData *> mObjects;

	// Mtlのリスト
	std::list<MtlData *> mMtlDatas;

	// 頂点の情報をインデックスで記録している
	// データ内容：頂点座標インデックス、UV座標インデックス、法線インデックス
	struct ObjPolyVertData;

	// ObjPolyVertDataを3つ持ち、ポリゴンを示す
	// データ内容：ObjPolyVertData[3]、所属するオブジェクトのインデックス、所属するポリゴングループのインデックス、使用するMtlの名前
	struct ObjPolyData;

	// オブジェクトを名前で検索
	class ObjectData * FindObjectWithName(const std::string & objectName) const;

	// 条件が真となるものを描画する
	void DrawUnderCondition(Shader * shader, std::function<bool(ObjectData * obj, size_t polyGroupIndex)> condition) const;
	
	//////////////////////////
	// ロード関連
	//////////////////////////

	// ロードのフローを通して使うローカル変数群
	struct LoadLocalVariable;

	// 読み込んだ１行を単語に分解する
	void GetWordsInLine(const char * line, std::vector<std::string> & wordsInLine);

	// Objファイルのロード
	bool LoadObj(LoadLocalVariable & local, const std::string & path);

	// Mtlファイルのロード
	bool LoadMtl(LoadLocalVariable & local, const std::string & path);

};

//////////////////////////
// 内部構造体群
//////////////////////////
class Mesh::ObjectData
{
public:
	friend Mesh;

	struct PolyGroup
	{
		std::string mName;
		
		ElementBuffer * mElementBuffer;

		std::string mUsemtlName;
		const MtlData * mUsemtl;

		~PolyGroup()
		{
			delete mElementBuffer;
			mElementBuffer = nullptr;
		}
	};

	const std::string & GetName() const { return mName; }
	const AABB & GetBox() const { return mBox; }
	bool GetDrawFlag() const { return mDrawFlag; }
	const std::vector<PolyGroup*> & GetPolyGroups() const { return mPolyGroups; }

	~ObjectData()
	{
		for (auto itr = mPolyGroups.rbegin(); itr != mPolyGroups.rend(); ++itr)
		{
			delete *itr;
		}
		std::vector<PolyGroup *>().swap(mPolyGroups);
	}

private:
	std::string mName;

	// 内包する直方体（AABB）
	AABB mBox;

	bool mDrawFlag;

	std::vector<PolyGroup *> mPolyGroups;

	ObjectData() :
		mDrawFlag(true)
	{
		PolyGroup * group = new PolyGroup;
		mPolyGroups.emplace_back(group);
	}
};

struct Mesh::ObjPolyVertData
{
	int mVertIndex = -1;
	int mTexCoordIndex = -1;
	int mNormIndex = -1;

	void Load(const std::string & str);

	friend bool operator==(const ObjPolyVertData & lhs, const ObjPolyVertData & rhs)
	{
		return lhs.mVertIndex == rhs.mVertIndex &&
			lhs.mTexCoordIndex == rhs.mTexCoordIndex &&
			lhs.mNormIndex == rhs.mNormIndex;
	}

	struct HashFunc final
	{
		size_t operator()(const ObjPolyVertData & v) const
		{
			static const size_t arraySize = 3;
			int array[arraySize] = { v.mVertIndex, v.mTexCoordIndex, v.mNormIndex };
			return Hash::Fnv1(reinterpret_cast<uint32_t*>(array), arraySize);
		}
	};
};

struct Mesh::ObjPolyData
{
	ObjPolyVertData mVerts[3];
	unsigned int mObjectIndex = 0;
	unsigned int mPolyGroupIndex = 0;

	std::string mUsemtl;
};

class Mesh::MtlData
{
public:
	friend Mesh;

	const std::string & GetName() const { return mName; }
	float GetDissolve() const { return mDissolve; }
	float GetShininess() const { return mShininess; }
	const Texture * GetDiffuseTexture() const { return mDiffuseTexture; }
	const Texture * GetSpecularTexture() const { return mSpecularTexture; }

private:
	std::string mName;

	float mDissolve;

	float mShininess;

	Texture * mDiffuseTexture;

	Texture * mSpecularTexture;
};

struct Mesh::LoadLocalVariable
{
	std::vector<Vector3D> vertexPos;
	std::vector<Vector2D> texCoord;
	std::vector<Vector3D> norm;
	std::vector<ObjPolyData> polyDatas;
	std::vector<std::string> mtllibStrings;
	std::string lastUsemtlWord;

	// ファイル内の１行の中の、半角スペースで区切られた単語を格納する
	std::vector<std::string> wordsInLine;

	// バッファ
	static const unsigned int bufSize = 256;
	char buf[bufSize];

	// モデル全体における頂点のインデックス値を記録するマップ
	std::unordered_map<ObjPolyVertData, GLuint, ObjPolyVertData::HashFunc> vertIndices;

	// ポリゴンのインデックスデータのバッファ
	typedef std::vector<GLuint> UintVec;
	std::unordered_map<ObjectData::PolyGroup *, UintVec> indexBuffer;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
