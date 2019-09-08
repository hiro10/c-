//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT				"data/TEXTURE/Gameover_back.jpg"	// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_CLEAR		"data/TEXTURE/背景素材.jpg"			// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO			"data/TEXTURE/coollogo_com-229421742.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO_CLEAR	"data/TEXTURE/coollogo_com-303064.png"	// 読み込むテクスチャファイル名

#define	TEXTURE_RESULT_LOGO_RANK	"data/TEXTURE/rank.png"	// 読み込むテクスチャファイル名

#define	TEXTURE_RESULT_RANKS	"data/TEXTURE/S.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_RANKA	"data/TEXTURE/A.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_RANKB	"data/TEXTURE/B .png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_RANKC	"data/TEXTURE/C.png"	// 読み込むテクスチャファイル名

#define	RESULT_LOGO_POS_X	(240)		// リザルトロゴの位置(X座標)
#define	RESULT_LOGO_POS_Y	(320)		// リザルトロゴの位置(Y座標)
#define	RESULT_LOGO_WIDTH	(700)		// リザルトロゴの幅
#define	RESULT_LOGO_HEIGHT	(240)		// リザルトロゴの高さ

// ランク（文字）の位置
#define RANK_POS_X (100)
#define RANK_POS_Y (600)

// ランク、S,A,B,Cの位置とサイズ
#define RANK_LOGO_POS_X (750)
#define RANK_LOGO_POS_Y (550)
#define	RANK_LOGO_WIDTH	(400)		
#define	RANK_LOGO_HEIGHT (340)		

#define	COUNT_APPERA_RESULT	(60)		// リザルトロゴ出現までの待ち時間	
#define	APPERA_RESULT_TIME	(0.005f)
#define	LIMIT_COUNT_WAIT	(60 * 5)	// 待ち時間


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
