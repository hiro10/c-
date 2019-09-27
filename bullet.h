//=============================================================================
//
// 弾処理 [bullet.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLET			"data/TEXTURE/bullet000.png"	// 読み込むテクスチャファイル名
#define	BULLET_SIZE_X			(150.0f)						// ビルボードの幅
#define	BULLET_SIZE_Y			(150.0f)						// ビルボードの高さ
#define	MAX_BULLET				(1512)							// バレット最大数
#define BULLET_RADIUS			(82)							// バレットの半径
#define TUIBI_BULLET_SPEED		(30.0f)							// 追尾時のスピード
#define PLAYER_BULLET_FLAME		(13)							// バレットの発射間隔用
#define BULLET_DAMAGE			(100)							// バレットのダメージ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer);
void DeleteBullet(int nIdxBullet);
void SetTuibi(D3DXVECTOR3 pos1, float Speed);

#endif
