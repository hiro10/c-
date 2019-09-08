//=============================================================================
//
// �A�C�e������ [item.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "gameobject.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ITEM				(60)						// �A�C�e���ő吔
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.025f)			// ��]���x
#define	ITEM_RADIUS				(10.0f)						// ���a
#define ITEM_FIRST_POS			(30000)
#define ITEM_DAMAGE				(50)

//**************************************
// ���
//**************************************
enum
{
	ITEMTYPE_COIN = 0,	
	ITEMTYPE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType = ITEMTYPE_COIN);
void DeleteItem(int IdxItem);
Item *GetItem(void);

#endif
