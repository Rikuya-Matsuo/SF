#pragma once
#include "Vector.h"
#include "Matrix.h"

class Camera
{
public:
	Camera(const Vector3D & pos = Vector3D::Zero, const Vector3D & target = Vector3D::UnitZ, const Vector3D & up = Vector3D::UnitY);

	virtual ~Camera();

	void SetPosition(const Vector3D & pos) { mPosition = pos; }

	void SetTarget(const Vector3D & front) { mFront = front; }

	void SetUpVector(const Vector3D & up) { mUp = up; }

	// 更新
	void Update();

	// 位置
	const Vector3D & GetPosition() const { return mPosition; }

	// ビュー行列
	const Matrix4 & GetViewMat() const { return mViewMat; }

	// プロジェクション行列
	const Matrix4 & GetProjectionMat() const { return mProjectionMat; }

	// アクティブ化
	void Activate() const;

protected:
	// 位置
	Vector3D mPosition;
	
	// 前方向
	Vector3D mFront;

	// 上方向
	Vector3D mUp;

	// ズーム
	float mZoom;

	// 広さ
	float mWidth;
	float mHeight;

	// ビュー行列
	Matrix4 mViewMat;

	// プロジェクション行列
	Matrix4 mProjectionMat;

	// 継承先固有の更新
	virtual void UpdateCamera();

	// 行列の計算
	void CalculateMatrices();
};
