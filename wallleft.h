//=============================================================================
//
// ���Ǐ��� [wallleft.h]
// Author : 
//
//=============================================================================
#ifndef _WALLLEFT_H_
#define _WALLLEFT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_WALLLEFT	"data/TEXTURE/���J��.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define SCROLL_SPEED_LEFT	(0.00305f)
#define WALLLEFE_VTX_X		(860.0f)
#define WALLLEFE_VTX_Y		(800.0f)
#define WALLLEFE_VTX_Z		(16100.0f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitWallleft(void);
void UninitWallleft(void);
void UpdateWallleft(void);
void DrawWallleft(void);

#endif

