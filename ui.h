//=============================================================================
//
// pause画面処理 [pause.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	TEXTURE_UI_LOGO1	("data/TEXTURE/敵機接近.png")				// 読み込むテクスチャファイル名
#define	UILOGO1_POS_X		(470)										// ロゴの表示位置
#define	UILOGO1_POS_Y		(5)											// ロゴの表示位置
#define	UILOGO1_POS_Z		(0)											// ロゴの表示位置
#define	UILOGO1_SIZE_X		(400)										// ロゴの幅
#define	UILOGO1_SIZE_Y		(100)										// ロゴの高さ

#define	TEXTURE_UIACE_LOGO	("data/TEXTURE/エース出現.png")				// 読み込むテクスチャファイル名
#define	UIACELOGO1_POS_X	(470)										// ロゴの表示位置
#define	UIACELOGO1_POS_Y	(5)											// ロゴの表示位置
#define	UIACELOGO1_POS_Z	(0)											// ロゴの表示位置
#define	UIACELOGO1_SIZE_X	(500)										// ロゴの幅
#define	UIACELOGO1_SIZE_Y	(150)										// ロゴの高さ

#define	TEXTURE_UIITEM_LOGO	("data/TEXTURE/ターゲット.png")				// 読み込むテクスチャファイル名
#define	UIITEMLOGO1_POS_X	(240)										// ロゴの表示位置
#define	UIITEMLOGO1_POS_Y	(150)										// ロゴの表示位置
#define	UIITEMLOGO1_POS_Z	(0)											// ロゴの表示位置
#define	UIITEMLOGO1_SIZE_X	(900)										// ロゴの幅
#define	UIITEMLOGO1_SIZE_Y	(300)										// ロゴの高さ

#define	TEXTURE_UI2_LOGO	("data/TEXTURE/ボス演出用警告画面用.png")	// 読み込むテクスチャファイル名
#define	UILOGO2_POS_X	(0)												// ロゴの表示位置
#define	UILOGO2_POS_Y	(0)												// ロゴの表示位置
#define UILOGO2_POS_Z	(0)												// ロゴの表示位置
#define	UILOGO2_SIZE_X	(SCREEN_WIDTH)									// ロゴの幅
#define	UILOGO2_SIZE_Y	(SCREEN_HEIGHT)									// ロゴの高さ


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

