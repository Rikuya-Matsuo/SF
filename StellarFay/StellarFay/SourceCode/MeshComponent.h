#pragma once
#include "ComponentBase.h"

class MeshComponent : public ComponentBase
{
public:
	MeshComponent(class Actor * owner, class Mesh * msh, int drawPriority);
	~MeshComponent();

private:
	// メッシュ
	class Mesh * mMesh;

	// 描画順
	// 半透明な部分に関してはこの値に関係なく、カメラからの距離で描画順を決定する予定
	int mDrawPriority;
};
