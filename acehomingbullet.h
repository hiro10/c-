//=============================================================================
//
// 弾処理 [acehomingbullet.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _ACEHOMINGBULLET_H_
#define _ACEHOMINGBULLET_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_ACEHOMINGBULLET	"data/TEXTURE/bullet000.png"				// 読み込むテクスチャファイル名
#define	ACEHOMINGBULLET_SIZE_X			(50.0f)									// ビルボードの幅
#define	ACEHOMINGBULLET_SIZE_Y			(50.0f)									// ビルボードの高さ
#define	MAX_ACEHOMINGBULLET				(10)									// ビルボード最大数
#define ACEHOMINGBULLET_RADIUS			(50.0f)									// ビルボード半径
#define ACEHOMINGBULLET_DAMAGE			(50)									// ダメージ


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitAceHomingBullet(void);
void UninitAceHomingBullet(void);
void UpdateAceHomingBullet(void);
void DrawAceHomingBullet(void);

int SetAceHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY);
void DeleteAceHomingBullet(int nIdxAceHomingBullet);

#endif


