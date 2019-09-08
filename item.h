//=============================================================================
//
// アイテム処理 [item.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "gameobject.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ITEM				(60)						// アイテム最大数
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.025f)			// 回転速度
#define	ITEM_RADIUS				(10.0f)						// 半径
#define ITEM_FIRST_POS			(30000)
#define ITEM_DAMAGE				(50)

//**************************************
// 種類
//**************************************
enum
{
	ITEMTYPE_COIN = 0,	
	ITEMTYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType = ITEMTYPE_COIN);
void DeleteItem(int IdxItem);
Item *GetItem(void);

#endif
