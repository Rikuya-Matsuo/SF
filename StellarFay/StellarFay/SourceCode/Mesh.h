#pragma once
#include "glad/glad.h"
#include "Texture.h"
#include "Shader.h"
#include "Collision.h"
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

	AABB GetObjectAABB(const std::string & objectName) const;

	// 体積が最も大きいAABBを返す
	AABB GetBiggestAABB() const;

private:
	GLuint mVAO;

	GLuint mVBO;

	// 頂点配列
	std::vector<float> mVertices;

	// モデルをいくつかのオブジェクト・ポリゴングループに分けることで、
	// テクスチャの使い分け等を可能にする
	class ObjectData;
	std::vector<ObjectData *> mObjects;

	// 読み込んだmtlデータ
	class MtlData;
	std::list<MtlData *> mMtlDatas;

	struct ObjPolyVertData;

	struct ObjPolyData;

	// 読み込んだ１行を単語に分解する
	void GetWordsInLine(const char * line, std::vector<std::string> & wordsInLine);

	// オブジェクトを名前で検索
	class ObjectData * FindObjectWithName(const std::string & objectName) const;

	// 条件が真となるものを描画する
	void DrawUnderCondition(Shader * shader, std::function<bool(ObjectData * obj, size_t polyGroupIndex)> condition) const;
};
