//=============================================================================
//
// skybox�̏��� [space.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _SPACE_H_
#define _SPACE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SPACE		(6)			
#define	SPACE_TEXTURE_FILENAME		"data/TEXTURE/space-3d-screenshot.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SPACE_NUM_VERTEX			(4)
#define	SPACE_NUM_INDEX				(4)
#define	SPACE_NUM_POLYGIN			(2)
#define	SPACE_SIZE					(VIEW_FAR_Z/2.8f)						// skybox�̑傫��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSpace();
void UninitSpace(void);
void UpdateSpace(void);
void DrawSpace(void);

#endif
