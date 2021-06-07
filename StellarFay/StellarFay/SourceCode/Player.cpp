#include "Player.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "Input.h"
#include "PhongShaderWrapper.h"

const Vector2D Player::mMaxNose = Vector2D(DEG_TO_RAD(45.0f), DEG_TO_RAD(30.0f));

Player::Player() :
	DestroyableActor(0, 100)
{
	Mesh * msh = RENDERER_INSTANCE.GetMesh("Assets/SciFi_Fighter/MK6_OBJ.obj");

	MeshComponent * mshComp = new MeshComponent(this, msh, 0);

	mShaderWrapper = new PhongShaderWrapper();
	mShaderWrapper->SetMatricesReference(&mModelMat, RENDERER_INSTANCE);

	mshComp->SetShader(mShaderWrapper);

	mTransform.mScales.SetAll(0.003f);
}

Player::~Player()
{
	delete mShaderWrapper;
}

void Player::UpdateActor()
{
	// 操作受け付け
	const GamePad & pad = INPUT_INSTANCE.GetGamePad(0);

	if (pad.IsValid())
	{
		const Vector2D & lstick = pad.GetLeftStick();

		// 移動方向に機首を向ける
		// 縦
		Quaternion pitch(Vector3D::UnitX, mMaxNose.y * lstick.y);

		// 横
		Quaternion yaw(Vector3D::UnitY, mMaxNose.x * lstick.x);

		// 合成
		mTransform.mRotation = Quaternion::Lerp(pitch, yaw, 0.5f);

		// 移動
		mTransform.mPosition.x += lstick.x;
		mTransform.mPosition.y -= lstick.y;
	}
}
