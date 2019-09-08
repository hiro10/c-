//=============================================================================
//
// タイマー処理 [timer.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TIMER		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_TIMER	"data/TEXTURE/frame_timer.png"	// 読み込むテクスチャファイル名
#define	TIMER_SIZE_X		(35.0f)							// タイマーの数字の幅
#define	TIMER_SIZE_Y		(50.0f)							// タイマーの数字の高さ
#define	TIMER_INTERVAL_X	(0.0f)							// タイマーの数字の表示間隔

#define	NUM_TIMER (3)										// タイマーの桁数

#define	TIMER_POS_X			(1150)							// タイマーの表示基準位置X座標
#define	TIMER_POS_Y			(145)						    // タイマーの表示基準位置Y座標																// タイマーの表示基準位置Ｙ座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
void GetEnableTimer(bool Enable);
void ResetTimer(int Time = 999);


#endif
