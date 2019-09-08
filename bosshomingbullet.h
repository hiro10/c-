//=============================================================================
//
// ボスのホーミング弾処理 [bosshomingbullet.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _BOSSHOMINGBULLET_H_
#define _BOSSHOMINGBULLET_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BOSSHOMINGBULLET	"data/TEXTURE/bullet000.png"				// 読み込むテクスチャファイル名
#define	BOSSHOMINGBULLET_SIZE_X			(50.0f)									// ビルボードの幅
#define	BOSSHOMINGBULLET_SIZE_Y			(50.0f)									// ビルボードの高さ
#define	MAX_BOSSHOMINGBULLET			(18)									// ビルボード最大数
#define BOSSHOMINGBULLET_RADIUS			(50.0f)									// ビルボード半径
#define BOSSHOMINGBULLET_DAMAGE			(10)									// ダメージ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBossHomingBullet(void);
void UninitBossHomingBullet(void);
void UpdateBossHomingBullet(void);
void DrawBossHomingBullet(void);
int SetBossHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY);
void DeleteBossHomingBullet(int nIdxBossHomingBullet);

#endif

