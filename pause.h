//=============================================================================
//
// pause画面処理 [pause.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	TEXTURE_PAUSE_LOGO1	("data/TEXTURE/coollogo_com-26727588.png")	// 読み込むテクスチャファイル名
#define	PAUSELOGO1_POS_X	(330)										// ポーズロゴの表示位置
#define	PAUSELOGO1_POS_Y	(350)										// ポーズロゴの表示位置
#define	PAUSELOGO1_POS_Z	(0)											// ポーズロゴの表示位置
#define	PAUSELOGO1_SIZE_X	(650)										// ポーズロゴの幅
#define	PAUSELOGO1_SIZE_Y	(200)										// ポーズロゴの高さ

#define	TEXTURE_RETURNTITLE_LOGO1	("data/TEXTURE/タイトルに戻る  (1).png")	// 読み込むテクスチャファイル名
#define	RETURNTITLELOGO1_POS_X	(830)										// ロゴの表示位置
#define	RETURNTITLELOGO1_POS_Y	(850)										// ロゴの表示位置
#define	RETURNTITLELOGO1_POS_Z	(0)											// ロゴの表示位置
#define	RETURNTITLELOGO1_SIZE_X	(250)										// ロゴの幅
#define	RETURNTITLELOGO1_SIZE_Y	(100)										// ロゴの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif

