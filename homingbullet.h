//=============================================================================
//
// �e���� [bullet.h]
// Author : �����C�O
//
//=============================================================================
#ifndef _HOMINGBULLET_H_
#define _HOMINGBULLET_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_ROCKON		�@	"data/TEXTURE/bullet000.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	HOMINGBULLET_SIZE_X		(50.0f)								// �r���{�[�h�̕�
#define	HOMINGBULLET_SIZE_Y		(50.0f)								// �r���{�[�h�̍���
#define	MAX_HOMINGBULLET		(5)									// �r���{�[�h�ő吔
#define HOMING_DAMAGE			(500)
#define	H_BULLET_SOUND_MAX		(140)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitHomingBullet(void);
void UninitHomingBullet(void);
void UpdateHomingBullet(void);
void DrawHomingBullet(void);

int SetHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY);
void DeleteHomingBullet(int nIdxHomingBullet);

#endif
