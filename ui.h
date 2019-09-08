//=============================================================================
//
// pause画面処理 [pause.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	TEXTURE_UI_LOGO1	("data/TEXTURE/敵機接近.png")	// 読み込むテクスチャファイル名
#define	UILOGO1_POS_X	(470)										// ポーズロゴの表示位置
#define	UILOGO1_POS_Y	(5)										// ポーズロゴの表示位置
#define	UILOGO1_POS_Z	(0)											// ポーズロゴの表示位置
#define	UILOGO1_SIZE_X	(400)										// ポーズロゴの幅
#define	UILOGO1_SIZE_Y	(100)										// ポーズロゴの高さ

#define	TEXTURE_UIACE_LOGO	("data/TEXTURE/エース出現.png")	// 読み込むテクスチャファイル名
#define	UIACELOGO1_POS_X	(470)										// ポーズロゴの表示位置
#define	UIACELOGO1_POS_Y	(5)										// ポーズロゴの表示位置
#define	UIACELOGO1_POS_Z	(0)											// ポーズロゴの表示位置
#define	UIACELOGO1_SIZE_X	(500)										// ポーズロゴの幅
#define	UIACELOGO1_SIZE_Y	(150)										// ポーズロゴの高さ

#define	TEXTURE_UIITEM_LOGO	("data/TEXTURE/ターゲット.png")	// 読み込むテクスチャファイル名
#define	UIITEMLOGO1_POS_X	(240)										// ポーズロゴの表示位置
#define	UIITEMLOGO1_POS_Y	(150)										// ポーズロゴの表示位置
#define	UIITEMLOGO1_POS_Z	(0)											// ポーズロゴの表示位置
#define	UIITEMLOGO1_SIZE_X	(900)										// ポーズロゴの幅
#define	UIITEMLOGO1_SIZE_Y	(300)										// ポーズロゴの高さ

#define	TEXTURE_UI2_LOGO	("data/TEXTURE/sign-1513493_960_720 (2).png")	// 読み込むテクスチャファイル名
#define	UILOGO2_POS_X	(0)										// ポーズロゴの表示位置
#define	UILOGO2_POS_Y	(0)										// ポーズロゴの表示位置
#define UILOGO2_POS_Z	(0)											// ポーズロゴの表示位置
#define	UILOGO2_SIZE_X	(SCREEN_WIDTH)										// ポーズロゴの幅
#define	UILOGO2_SIZE_Y	(SCREEN_HEIGHT)										// ポーズロゴの高さ


#define MODE_GAME_COUNT		(30)
#define MODE_BOSS_COUNT		(10)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);

#endif

