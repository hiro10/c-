//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author :西村任弘
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_GAUGE00		("data/TEXTURE/HP素材.png")				// サンプル用画像
#define TEXTURE_GAME_GAUGEFRAME00	("data/TEXTURE/ゲージカバー2 (1).png")	// サンプル用画像
#define GAUGE00_SIZE_X			(350)	// テクスチャサイズ
#define GAUGE00_SIZE_Y			(35)	// 同上
#define GAUGE00_POS_X			(10)	// ポリゴンの初期位置X(左上)
#define GAUGE00_POS_Y			(850)	// 同上
#define GAUGEFRAME00_SIZE_X			(GAUGE00_SIZE_X + GAUGE00_SIZE_X / 5)	// テクスチャサイズ
#define GAUGEFRAME00_SIZE_Y			(GAUGE00_SIZE_Y + GAUGE00_SIZE_Y / 3)	// 同上
#define GAUGEFRAME00_POS_X			(GAUGE00_POS_X - GAUGE00_SIZE_X / 25)	// ポリゴンの初期位置X(左上)
#define GAUGEFRAME00_POS_Y			(GAUGE00_POS_Y - GAUGE00_SIZE_Y / 3)	// 同上

//HP表記用
#define	TEXTURE_HP	("data/TEXTURE/coollogo_com-63881504.png")			// 読み込むテクスチャファイル名
#define	HP_POS_X	(20)												// タイトルロゴの表示位置
#define	HP_POS_Y	(800)												// タイトルロゴの表示位置                         
#define	HP_POS_Z	(0)													// タイトルロゴの表示位置
#define	HP_SIZE_X	(60)												// タイトルロゴの幅
#define	HP_SIZE_Y	(50)												// タイトルロゴの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);

#endif

