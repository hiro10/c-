//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EXPLOSION		"data/TEXTURE/pipo-btleffect037.png"	// 読み込むテクスチャファイル名
#define	EXPLOSION_SIZE_X			(50.0f)									// ビルボードの幅
#define	EXPLOSION_SIZE_Y			(50.0f)									// ビルボードの高さ
#define	VALUE_MOVE_EXPLOSION	(2.0f)									// 移動速度

#define	MAX_EXPLOSION			(128)									// 最大数

//*****************************************************************************
// 構造体(クラス)定義
//*****************************************************************************
class EXPLOSION
{
public:
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXCOLOR col;			// 色
	float SizeX;			// 幅
	float SizeY;			// 高さ
	int Counter;			// カウンター
	int Pattern;			// パターンNo.
	int Type;				// 種類
	bool use;				// 使用しているかどうか
};

//**************************************
// 種類
//**************************************
enum
{
	EXPLOSIONTYPE_BULLET_Player = 0,	// プレイヤーの弾の爆発
	EXPLOSIONTYPE_BULLET_ENEMY,			// 敵の弾の爆発
	EXPLOSIONTYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);

int SetExplosion(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType = EXPLOSIONTYPE_BULLET_Player);

#endif
