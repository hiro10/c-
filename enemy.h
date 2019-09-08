//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ENEMY				(80)						// エネミー最大数
#define	VALUE_ROTATE_ENEMY		(D3DX_PI * 0.025f)			// 回転速度
#define VALUE_MOVE_ENEMY		(0.5f)						// 弾の移動速度
#define	ENEMY_RADIUS				(100.0f)				// 半径
#define ENEMY_TEXTURE_FILENAME  "data/TEXTURE/UFO.bmp"	    // 読み込むテクスチャファイル名
#define ENEMY_DAMAGE			(25)

//**************************************
// 種類
//**************************************
enum
{
	ENEMYTYPE_ENEMY = 0,	// エネミー
	ENEMYTYPE_ENEMY2,
	ENEMYTYPE_ENEMY3,
	ENEMYTYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Type = ENEMYTYPE_ENEMY);
void DeleteEnemy(int IdxEnemy);
Enemy *GetEnemy(int IdxEnemy);
D3DXVECTOR3 GetPositionEnemy(int IdxEnemy);
D3DXVECTOR3 GetSclEnemy(int IdxEnemy);

#endif
