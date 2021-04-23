#pragma once
#include "glad/glad.h"
#include "Texture.h"
#include "Shader.h"
#include "Collision.h"
#include "VertexArray.h"
#include <vector>
#include <list>
#include <string>
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

private:
	VertexArray * mVertexArray;

	// 頂点配列
	std::vector<float> mVertices;

	// モデルをいくつかのオブジェクト・ポリゴングループに分けることで、
	// テクスチャの使い分け等を可能にする
	// データ内容：名前、当たり判定用直方体、描画フラグ、ポリゴングループのコンテナ
	class ObjectData;
	std::vector<ObjectData *> mObjects;

	// 読み込んだmtlデータ
	// データ内容：名前、ディゾルブ値、テクスチャ
	class MtlData;
	std::list<MtlData *> mMtlDatas;

	// 頂点の情報をインデックスで記録している
	// データ内容：頂点座標インデックス、UV座標インデックス、法線インデックス
	struct ObjPolyVertData;

	// ObjPolyVertDataを3つ持ち、ポリゴンを示す
	// データ内容：ObjPolyVertData[3]、所属するオブジェクトのインデックス、所属するポリゴングループのインデックス、使用するMtlの名前
	struct ObjPolyData;

	// 読み込んだ１行を単語に分解する
	void GetWordsInLine(const char * line, std::vector<std::string> & wordsInLine);

	// オブジェクトを名前で検索
	class ObjectData * FindObjectWithName(const std::string & objectName) const;

	// 条件が真となるものを描画する
	void DrawUnderCondition(Shader * shader, std::function<bool(ObjectData * obj, size_t polyGroupIndex)> condition) const;

	static VertexArray::ArgumentPreset mVAPreset;
};
