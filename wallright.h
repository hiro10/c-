//=============================================================================
//
// 右壁処理 [wallright.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _WALLRIGHT_H_
#define _WALLRIGHT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_WALLRIGHT	"data/TEXTURE/メカ壁.png"	// 読み込むテクスチャファイル名
#define SCROLL_SPEED_RIGHT	(0.00305f)
#define WALLRIGNT_VTX_X		(860.0f)
#define WALLRIGNT_VTX_Y		(800.0f)
#define WALLRIGNT_VTX_Z		(16100.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitWallright(void);
void UninitWallright(void);
void UpdateWallright(void);
void DrawWallright(void);
#endif
