//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

#define	TEXTURE_BOSSFIELD	"data/TEXTURE/-1755048.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int NumBlockX, int NumBlockZ, float BlockSizeX, float BlockSizeZ);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif
