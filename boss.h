//=============================================================================
//
// ボス処理 [boss.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_CAR			  "data/MODEL/UFO(big).x"	// 読み込むモデル名
#define BOSS_TEXTURE_FILENAME "data/TEXTURE/メカ壁.png" // 読み込むテクスチャファイル名
#define	VALUE_MOVE				(3.0f)					// 移動量	
#define	VALUE_ROTATE			(D3DX_PI * 0.02f)		// 回転量
#define	VALUE_ROTATE_BOSS		(D3DX_PI * 0.025f)		// 回転速度
#define	RATE_ROTATE_BOSS		(0.10f)					// 回転慣性係数
#define	VALUE_MOVE_BOSS_BULLET		(20.0f)				// 弾の移動速度
#define BOSS_FIRST_POS_Z		(2500)					// ボスの最初のｚ軸の位置
#define BOSS_HP_MAX				(50000)					// ボスのHP
#define BOSS_RADIUS				(500.0f)				// ボスの半径
#define BOSS_BULLET_SIZE_X		(110)					// ボスの弾のXサイズ
#define BOSS_BULLET_SIZE_Y		(110)					// ボスの弾のYサイズ
#define BOSS_DEAD				(0)						// ボスの死亡時
#define BOSS_LIFEMAX			(20000)   
#define BOSS_ROT_Y				(0.07f)
#define BOSS_BULLET_QUANTITY_0	(12)					// ボスバレットデータ０の量
#define BOSS_BULLET_QUANTITY_1	(12)					// ボスバレットデータ１の量
#define BOSS_HOMING_QUANTITY	(3)						// ボスホーミングバレットの量
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
Boss *GetBoss(void);
D3DXVECTOR3	GetRotBoss(void);
D3DXVECTOR3	*GetPosBoss(void);
D3DXVECTOR3	GetBossPos(void);
D3DXMATRIX *GetBossWorldMatrix(void);


#endif
