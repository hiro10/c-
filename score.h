//=============================================================================
//
// スコア処理 [score.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_SCORE	"data/TEXTURE/frame_score.png"	// 読み込むテクスチャファイル名
#define	SCORE_SIZE_X		(35.0f)							// スコアの数字の幅
#define	SCORE_SIZE_Y		(50.0f)							// スコアの数字の高さ
#define	SCORE_INTERVAL_X	(0.0f)							// スコアの数字の表示間隔

#define	NUM_SCORE			(8)								// スコアの桁数

#define	SCORE_POS_X			(SCREEN_WIDTH - (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_SCORE - 20.0f)	// スコアの表示基準位置Ｘ座標
#define	SCORE_POS_Y			(60.0f)																	// スコアの表示基準位置Ｙ座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void ChangeScore(int value);

#endif
