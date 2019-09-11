//=============================================================================
//
// プレイヤー処理 [player.h]
// Author :　西村　任弘
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER			"data/MODEL/F16_Thuderbirds1.x"	// 読み込むモデル名
#define PLAER_TEXTURE_FILENAME	"data/TEXTURE/F16_改.jpg"		// 読み込むテクスチャファイル名
#define	PLAYER_RADIUS		(40.0f)						// 半径
#define	VALUE_MOVE_PLAYER	(0.25f)						// 移動速度
#define	RATE_MOVE_PLAYER	(0.025f)					// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.10f)						// 回転慣性係数
#define	VALUE_MOVE_PLAYER_BULLET	(50.0f)				// 弾の移動速度
#define PLAYER_LIFEMAX		(1000)
#define PLAYER_DEAD			(0)							// プレイヤーの死亡

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

D3DXVECTOR3 GetPositionPlayer(void);
D3DXVECTOR3 GetRotationPlayer(void);
D3DXVECTOR3 GetRotationDestPlayer(void);


#endif