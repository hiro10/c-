//=============================================================================
//
// エース処理 [ace.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _ACE_H_
#define _ACE_H_

//*****************************************************************************
//インクルード
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ACE				"data/MODEL/ACE.x"						// 読み込むモデル名
#define ACE_TEXTURE_FILENAME	"data/TEXTURE/Sukhoi__S37_Berkut.jpg"	// 読み込むテクスチャファイル名
#define	ACE_VALUE_MOVE			(8.0f)								// 移動量
#define	VALUE_ROTATE			(D3DX_PI * 0.02f)					// 回転量
#define ACE_MOVE_PLACE_LEFT		(-840.0f)							// 左側のプレイヤーの移動限界
#define ACE_MOVE_PLACE_RIGHT	(840.0f)							// 右側のプレイヤーの移動限界
#define ACE_FIRST_POS_Z			(1500)								// ACEの最初のｚ軸の位置
#define ACE_HP_MAX				(20000)								// ACEのHP
#define ACE_RADIUS				(50.0f)								// ACEの半径
#define ACE_DEAD				(0)									// ACEの死亡判定


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitAce(void);
void UninitAce(void);
void UpdateAce(void);
void DrawAce(void);
Ace *GetAce(void);
D3DXVECTOR3	GetRotAce(void);
D3DXVECTOR3	*GetPosAce(void);
D3DXVECTOR3	GetAcePos(void);
D3DXMATRIX *GetAceWorldMatrix(void);


#endif
