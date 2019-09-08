//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "gameobject.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EFFECT			"data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EFFECT_SIZE_X			(50.0f)							// �r���{�[�h�̕�
#define	EFFECT_SIZE_Y			(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_BULLET		(2.0f)							// �ړ����x
#define	MAX_EFFECT				(20000)							// �G�t�F�N�g�ő吔


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);

#endif
