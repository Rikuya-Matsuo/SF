#pragma once
#include <SDL/SDL.h>

// 属性
enum ColliderAttribute : Uint8
{
	Player_ColAtt = 0,
	Enemy_ColAtt,
	PlayerBullet_ColAtt,
	EnemyBullet_ColAtt,
	Invalid_ColAtt
};

// 接触することができる属性の組み合わせ
static const ColliderAttribute TouchableAttributeCombinations[][2] =
{
	{Player_ColAtt, Enemy_ColAtt},
	{Player_ColAtt, EnemyBullet_ColAtt},
	{Enemy_ColAtt, PlayerBullet_ColAtt}
};

// ２つの属性のコリジョンが接触可能かを調べる
static bool CheckTouchable(ColliderAttribute att1, ColliderAttribute att2)
{
	bool matchFlag = false;

	const size_t iMax = sizeof(TouchableAttributeCombinations) / 2 / sizeof(ColliderAttribute);
	for (size_t i = 0; i < iMax; ++i)
	{
		const ColliderAttribute * tacIPtr = TouchableAttributeCombinations[i];

		matchFlag =
			(att1 == tacIPtr[0] && att2 == tacIPtr[1]) ||
			(att1 == tacIPtr[1] && att2 == tacIPtr[0]);

		if (matchFlag)
		{
			break;
		}
	}

	return matchFlag;
}
