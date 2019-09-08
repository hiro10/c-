//=============================================================================
//
// �e���� [bullet.h]
// Author : �����C�O
//
//=============================================================================
#ifndef _BOSSBULLET_H_
#define _BOSSBULLET_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BOSSBULLET			"data/TEXTURE/bullet000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BOSSBULLET_SIZE_X			(50.0f)							// �r���{�[�h�̕�
#define	BOSSBULLET_SIZE_Y			(50.0f)							// �r���{�[�h�̍���
#define	MAX_BOSSBULLET				(3000)							// �r���{�[�h�ő吔
#define BOSSBULLET_DAMADE			(50)
#define BOSSBULLET_RADIUS			(35)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBossBullet(void);
void UninitBossBullet(void);
void UpdateBossBullet(void);
void DrawBossBullet(void);

int SetBossBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float SizeXX, float SizeXY, int Timer);
void DeleteBossBullet(int nIdxBossBullet);
#endif
