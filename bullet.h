//=============================================================================
//
// �e���� [bullet.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLET			"data/TEXTURE/bullet000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLET_SIZE_X			(150.0f)						// �r���{�[�h�̕�
#define	BULLET_SIZE_Y			(150.0f)						// �r���{�[�h�̍���
#define	MAX_BULLET				(1512)							// �o���b�g�ő吔
#define BULLET_RADIUS			(82)							// �o���b�g�̔��a
#define TUIBI_BULLET_SPEED		(30.0f)							// �ǔ����̃X�s�[�h
#define PLAYER_BULLET_FLAME		(13)							// �o���b�g�̔��ˊԊu�p
#define BULLET_DAMAGE			(100)							// �o���b�g�̃_���[�W

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer);
void DeleteBullet(int nIdxBullet);
void SetTuibi(D3DXVECTOR3 pos1, float Speed);

#endif
