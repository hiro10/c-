//=============================================================================
//
// �E�Ǐ��� [wallright.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _WALLRIGHT_H_
#define _WALLRIGHT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_WALLRIGHT	"data/TEXTURE/���J��.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define SCROLL_SPEED_RIGHT	(0.00305f)
#define WALLRIGNT_VTX_X		(860.0f)
#define WALLRIGNT_VTX_Y		(800.0f)
#define WALLRIGNT_VTX_Z		(16100.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitWallright(void);
void UninitWallright(void);
void UpdateWallright(void);
void DrawWallright(void);
#endif
