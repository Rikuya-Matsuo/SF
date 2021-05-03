#include "Camera.h"
#include "CommonMath.h"
#include "Renderer.h"
#include "GameSystem.h"

Camera::Camera(const Vector3D & pos, const Vector3D & front, const Vector3D & up) :
	mPosition(pos),
	mFront(front),
	mUp(up),
	mWidth(static_cast<float>(RENDERER_INSTANCE.GetWidth())),
	mHeight(static_cast<float>(RENDERER_INSTANCE.GetHeight())),
	mZoom(DEG_TO_RAD(45.0f))
{
}

Camera::~Camera()
{
}

void Camera::Activate() const
{
	RENDERER_INSTANCE.SetActiveCamera(this);
}

void Camera::Update()
{
	// 継承先依存の更新
	UpdateCamera();

	// ビュー行列の計算
	CalculateMatrices();
}

void Camera::UpdateCamera()
{
	// 内容は継承先に依存
}

void Camera::CalculateMatrices()
{
	// ビュー行列の計算
	mViewMat = Matrix4::CreateLookAt(mPosition, mFront, mUp);

	// プロジェクション行列の計算
	mProjectionMat = Matrix4::CreatePerspectiveFOV(mZoom, mWidth, mHeight, 0.1f, 1000.f);
}
