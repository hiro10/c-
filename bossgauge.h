//=============================================================================
//
// ゲージ処理 [bossgauge.h]
// Author :西村任弘
//
//=============================================================================
#ifndef _BOSSGAUGE_H_
#define _BOSSGAUGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_BOSSGAUGE00		("data/TEXTURE/HP素材ボス用.png")		// サンプル用画像
#define TEXTURE_GAME_BOSSGAUGEFRAME00	("data/TEXTURE/ゲージカバーBOSS.png")	// サンプル用画像
#define BOSSGAUGE00_SIZE_X			(350)	// テクスチャサイズ
#define BOSSGAUGE00_SIZE_Y			(30)	// 同上
#define BOSSGAUGE00_POS_X			(40)	// ポリゴンの初期位置X(左上)
#define BOSSGAUGE00_POS_Y			(30)	// 同上

#define BOSSGAUGEFRAME00_SIZE_X			(1000)	// テクスチャサイズ
#define BOSSGAUGEFRAME00_SIZE_Y			(BOSSGAUGE00_SIZE_Y + BOSSGAUGE00_SIZE_Y / 3)	// 同上
#define BOSSGAUGEFRAME00_POS_X			(0)		// ポリゴンの初期位置X(左上)
#define BOSSGAUGEFRAME00_POS_Y			(BOSSGAUGE00_POS_Y - BOSSGAUGE00_SIZE_Y / 3)	// 同上

//HP表記用
#define	TEXTURE_BOSSHP1	("data/TEXTURE/boss_hp素材.png")	// 読み込むテクスチャファイル名
#define	BOSSHP1_POS_X	(200)							    // タイトルロゴの表示位置
#define	BOSSHP1_POS_Y	(-10)								// タイトルロゴの表示位置                         
#define	BOSSHP1_POS_Z	(0)									// タイトルロゴの表示位置
#define	BOSSHP1_SIZE_X	(100)								// タイトルロゴの幅
#define	BOSSHP1_SIZE_Y	(50)								// タイトルロゴの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBossGauge(void);
void UninitBossGauge(void);
void UpdateBossGauge(void);
void DrawBossGauge(void);

#endif





