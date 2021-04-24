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

	// 所属アクターの更新
	void UpdateActor();

protected:
	// アクターへのポインタのコンテナ
	std::list<class Actor *> mActors;

private:
	// 最も新しく作られたSceneBaseクラスへのポインタ
	static SceneBase * mLatestScene;
};
