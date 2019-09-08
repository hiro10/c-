//=============================================================================
//
// 当たり判定処理 [collision.cpp]
// Author : 西村任弘
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "collision.h"

//=============================================================================
// 当たり判定の設定
//=============================================================================
bool AttackCheak(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2,float Radius1, float Radius2)
{
	float Length;

	Length = (pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (Length < (Radius1 + Radius2) * (Radius1 + Radius2))
	{
		return true;
	}
	else
	{
		return false;
	}
}