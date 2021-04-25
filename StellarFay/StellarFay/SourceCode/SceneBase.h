#pragma once
#include <SDL/SDL.h>
#include <list>

enum SceneEnum : Uint8; 

class SceneBase
{
public:
	SceneBase();

	// シーンのファクトリ関数
	static SceneBase * GenerateScene(SceneEnum sceneEnum);

	// 受け取ったアクターに所属シーンとして、最新のシーンを教える
	// （そのままポインタを返すと、外部から最新シーンをいじることが可能となるため、カプセル化の観点から却下）
	static void GetLatestScene(class Actor * actor);

	// 次シーンへの移動
	bool GetGoNextSceneFlag() const { return mGoNextSceneFlag; }

	// 次シーンの種類を取得
	SceneEnum GetNextScene() const { return mNextScene; }

	// シーンクラス内のコンテナに、受け取ったアクターを加える
	void RegisterActor(class Actor * actor);

	// アクターのソートを要請。アクターの更新順が変わったときに実行する。
	void RequestSortActor() { mSortActorFlag = true; }

	// シーン全体の一連の更新
	void Update();

protected:
	// アクターへのポインタのコンテナ
	std::list<class Actor *> mActors;

	// 最も新しく作られたSceneBaseクラスへのポインタ
	static SceneBase * mLatestScene;

	// 継承先の更新
	virtual void UpdateScene();

	// 次シーンへの移動を希望する
	void GoNextScene() { mGoNextSceneFlag = true; }

	// 次シーンへの移動希望をキャンセルする
	void CancelGoNextScene() { mGoNextSceneFlag = false; }

	void SetNextScene(SceneEnum sceneEnum) { mNextScene = sceneEnum; }

private:
	// アクターをソートするフラグ
	bool mSortActorFlag : 4;

	// 次シーンへ移動することを希望するフラグ
	// シーンの移動はGameSystemが管理するので、あくまで「希望」である
	bool mGoNextSceneFlag : 4;

	// 次シーンの種類を示す列挙型
	SceneEnum mNextScene;

	// 所属アクターの更新
	void UpdateActors();
};

// シーンの種類の列挙型
enum SceneEnum : Uint8
{
	Base_SceneEnum = 0,
	Test_SceneEnum,
	Title_SceneEnum,
	Game_SceneEnum,
	Invalid_SceneEnum
};
