//=============================================================================
//
// 弾処理 [bullet.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _HOMINGBULLET_H_
#define _HOMINGBULLET_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_ROCKON		　	"data/TEXTURE/bullet000.png"		// 読み込むテクスチャファイル名
#define	HOMINGBULLET_SIZE_X		(50.0f)								// ビルボードの幅
#define	HOMINGBULLET_SIZE_Y		(50.0f)								// ビルボードの高さ
#define	MAX_HOMINGBULLET		(5)									// ビルボード最大数
#define HOMING_DAMAGE			(500)
#define	H_BULLET_SOUND_MAX		(140)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitHomingBullet(void);
void UninitHomingBullet(void);
void UpdateHomingBullet(void);
void DrawHomingBullet(void);

int SetHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY);
void DeleteHomingBullet(int nIdxHomingBullet);

#endif
