#pragma once
#include <list>

class SceneBase
{
public:
	SceneBase();

	// 受け取ったアクターに所属シーンとして、最新のシーンを教える
	// （そのままポインタを返すと、外部から最新シーンをいじることが可能となるため、カプセル化の観点から却下）
	static void GetLatestScene(class Actor * actor);

	// シーンクラス内のコンテナに、受け取ったアクターを加える
	void ResisterActor(class Actor * actor);

	// アクターのソートを要請。アクターの更新順が変わったときに実行する。
	void RequestSortActor() { mSortActorFlag = true; }

	// シーン全体の一連の更新
	void Update();

protected:
	// アクターへのポインタのコンテナ
	std::list<class Actor *> mActors;

	// 最も新しく作られたSceneBaseクラスへのポインタ
	static SceneBase * mLatestScene;

	virtual void UpdateScene();

private:
	// アクターをソートするフラグ
	bool mSortActorFlag;

	// 所属アクターの更新
	void UpdateActors();
};
