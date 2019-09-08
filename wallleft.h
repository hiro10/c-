//=============================================================================
//
// 左壁処理 [wallleft.h]
// Author : 
//
//=============================================================================
#ifndef _WALLLEFT_H_
#define _WALLLEFT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_WALLLEFT	"data/TEXTURE/メカ壁.png"	// 読み込むテクスチャファイル名
#define SCROLL_SPEED_LEFT	(0.00305f)
#define WALLLEFE_VTX_X		(860.0f)
#define WALLLEFE_VTX_Y		(800.0f)
#define WALLLEFE_VTX_Z		(16100.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitWallleft(void);
void UninitWallleft(void);
void UpdateWallleft(void);
void DrawWallleft(void);

#endif

