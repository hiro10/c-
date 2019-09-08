//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TITLE		"data/TEXTURE/128428312238816125082.jpg"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_LOGO	"data/TEXTURE/タイトル.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_START	"data/TEXTURE/coollogo_com-1171249.png"	// 読み込むテクスチャファイル名

#define	TITLE_LOGO_POS_X		(120)		// タイトルロゴの位置(X座標)
#define	TITLE_LOGO_POS_Y		(40)		// タイトルロゴの位置(Y座標)
#define	TITLE_LOGO_WIDTH		(1080)		// タイトルロゴの幅
#define	TITLE_LOGO_HEIGHT		(640)		// タイトルロゴの高さ

#define	START_POS_X				(400)		// スタートボタンの位置(X座標)
#define	START_POS_Y				(720)		// スタートボタンの位置(Y座標)
#define	START_WIDTH				(480)		// スタートボタンの幅
#define	START_HEIGHT			(120)		// スタートボタンの高さ

#define	COUNT_APPERA_START		(60)		// スタートボタン出現までの時間
#define	INTERVAL_DISP_START		(60)		// スタートボタン点滅の時間

#define	COUNT_WAIT_DEMO			(60 * 5)	// デモまでの待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif
