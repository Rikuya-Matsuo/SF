#include "Collision.h"
#include "CommonMath.h"

// 判定を行う軸の順番
// 外部のクラスから閲覧できないようにするため、cppに定義する
// このゲームでは自機は基本的にz方向に進むため、z軸から判定を行った方が工程を減らせるのではないかと考え、この順番にしている
const char chkOrder[3] = { 'z', 'x', 'y' };

bool CheckHit(const AABB & box1, const AABB & box2)
{
	// 各要素が接触しているかを検証するラムダ式
	auto chkEachElem = [&box1, &box2](char elem)
	{
		return
			(box1.mMin[elem] <= box2.mMin[elem] && box1.mMax[elem] >= box2.mMin[elem]) ||
			(box2.mMin[elem] <= box1.mMin[elem] && box2.mMax[elem] >= box1.mMin[elem]);
	};

	// 全ての軸でボックスの範囲が重なっていれば、接触している
	bool hitFlag = chkEachElem(chkOrder[0]) && chkEachElem(chkOrder[1]) && chkEachElem(chkOrder[2]);

	return hitFlag;
}

bool CheckHit(const AABB & box, const Sphere & s)
{
	// 球の中心からボックスまでの最短距離を計算
	float distSq = box.MinDistanceSq(s.mCenter);

	// 最短距離が球の半径以下なら接触している
	bool hitFlag = (distSq <= s.mRadius * s.mRadius);

	return hitFlag;
}

bool CheckHit(const Sphere & s1, const Sphere & s2)
{
	// 半径の合計
	const float radiusSum = s1.mRadius + s2.mRadius;

	// 中心から中心までのベクトル
	const Vector3D c2c = (s1.mCenter - s2.mCenter);

	// 中心と中心の距離が半径の合計以下なら接触している
	// 負荷を軽減するため、両者2乗した状態で比較
	bool hitFlag = (c2c.LengthSq() <= radiusSum * radiusSum);

	return hitFlag;
}

bool AABB::IsPointInside(const Vector3D & point) const
{
	// 指定の軸において、pointがボックスの内側にあるかを検証するラムダ式
	auto chkInsideEachElem = [&point, this](char elemKey)
	{
		return (mMin[elemKey] <= point[elemKey] && mMax[elemKey] >= point[elemKey]);
	};

	// 全ての軸で検証が真を返せば内側にある
	bool inside = chkInsideEachElem(chkOrder[0]) && chkInsideEachElem(chkOrder[1]) && chkInsideEachElem(chkOrder[2]);

	return inside;
}

float AABB::MinDistanceSq(const Vector3D & point) const
{
	// 指定された軸のpointに最も近い座標を返す
	auto chkNearest = [&point, this](char elemKey)
	{
		float near;
		near = LARGER(mMin[elemKey], point[elemKey]);
		near = SMALLER(mMax[elemKey], near);

		return near;
	};

	// pointに最も近い、ボックス表面上の点
	Vector3D nearestPoint;

	nearestPoint.x = chkNearest('x');
	nearestPoint.y = chkNearest('y');
	nearestPoint.z = chkNearest('z');

	// pointからボックスまでの最短ベクトル
	Vector3D nearestVec = nearestPoint - point;

	return nearestPoint.LengthSq();
}

bool Sphere::IsPointInside(const Vector3D & point) const
{
	// 中心から引数の点までのベクトル
	Vector3D c2p = point - mCenter;

	// c2pの長さが半径以下なら内側にある
	bool inside = (c2p.LengthSq() <= mRadius * mRadius);

	return inside;
}
