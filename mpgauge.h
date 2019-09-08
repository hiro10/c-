#//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author :西村任弘
//
//=============================================================================
#ifndef _MPGAUGE_H_
#define _MPGAUGE_H_

// マクロ定義
#define TEXTURE_GAME_MPGAUGE00		("data/TEXTURE/mp.png")				// サンプル用画像
#define TEXTURE_GAME_MPGAUGEFRAME00	("data/TEXTURE/ゲージカバー2 (1).png")	// サンプル用画像


#define MPGAUGE00_SIZE_X			(200)	// テクスチャサイズ
#define MPGAUGE00_SIZE_Y			(20)	// 同上
#define MPGAUGE00_POS_X			(10)	// ポリゴンの初期位置X(左上)
#define MPGAUGE00_POS_Y			(900)	// 同上

#define MPGAUGEFRAME00_SIZE_X			(220)	// テクスチャサイズ
#define MPGAUGEFRAME00_SIZE_Y			(35)	// 同上
#define MPGAUGEFRAME00_POS_X			(7)	// ポリゴンの初期位置X(左上)
#define MPGAUGEFRAME00_POS_Y			(890)	// 同上


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMpGauge(void);
void UninitMpGauge(void);
void UpdateMpGauge(void);
void DrawMpGauge(void);

#endif

