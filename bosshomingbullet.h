//=============================================================================
//
// �{�X�̃z�[�~���O�e���� [bosshomingbullet.h]
// Author : �����C�O
//
//=============================================================================
#ifndef _BOSSHOMINGBULLET_H_
#define _BOSSHOMINGBULLET_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BOSSHOMINGBULLET	"data/TEXTURE/bullet000.png"				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BOSSHOMINGBULLET_SIZE_X			(50.0f)									// �r���{�[�h�̕�
#define	BOSSHOMINGBULLET_SIZE_Y			(50.0f)									// �r���{�[�h�̍���
#define	MAX_BOSSHOMINGBULLET			(18)									// �r���{�[�h�ő吔
#define BOSSHOMINGBULLET_RADIUS			(50.0f)									// �r���{�[�h���a
#define BOSSHOMINGBULLET_DAMAGE			(10)									// �_���[�W

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBossHomingBullet(void);
void UninitBossHomingBullet(void);
void UpdateBossHomingBullet(void);
void DrawBossHomingBullet(void);
int SetBossHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY);
void DeleteBossHomingBullet(int nIdxBossHomingBullet);

#endif

