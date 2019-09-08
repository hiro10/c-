//=============================================================================
//
// 弾処理 [bullet.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _BOSSBULLET_H_
#define _BOSSBULLET_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BOSSBULLET			"data/TEXTURE/bullet000.png"	// 読み込むテクスチャファイル名
#define	BOSSBULLET_SIZE_X			(50.0f)							// ビルボードの幅
#define	BOSSBULLET_SIZE_Y			(50.0f)							// ビルボードの高さ
#define	MAX_BOSSBULLET				(3000)							// ビルボード最大数
#define BOSSBULLET_DAMADE			(50)
#define BOSSBULLET_RADIUS			(35)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBossBullet(void);
void UninitBossBullet(void);
void UpdateBossBullet(void);
void DrawBossBullet(void);

int SetBossBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float SizeXX, float SizeXY, int Timer);
void DeleteBossBullet(int nIdxBossBullet);
#endif
