#pragma once

class SceneBase
{
public:
	SceneBase();

	// 受け取ったアクターに所属シーンとして、最新のシーンを教える
	// （そのままポインタを返すと、外部から最新シーンをいじることが可能となるため、カプセル化の観点から却下）
	static void GetLatestScene(class Actor * actor);

protected:

private:
	// 最も新しく作られたSceneBaseクラスへのポインタ
	static SceneBase * mLatestScene;
};
