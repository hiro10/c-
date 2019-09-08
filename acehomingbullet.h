//=============================================================================
//
// �e���� [acehomingbullet.h]
// Author : �����C�O
//
//=============================================================================
#ifndef _ACEHOMINGBULLET_H_
#define _ACEHOMINGBULLET_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_ACEHOMINGBULLET	"data/TEXTURE/bullet000.png"				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	ACEHOMINGBULLET_SIZE_X			(50.0f)									// �r���{�[�h�̕�
#define	ACEHOMINGBULLET_SIZE_Y			(50.0f)									// �r���{�[�h�̍���
#define	MAX_ACEHOMINGBULLET				(10)									// �r���{�[�h�ő吔
#define ACEHOMINGBULLET_RADIUS			(50.0f)									// �r���{�[�h���a
#define ACEHOMINGBULLET_DAMAGE			(50)									// �_���[�W


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitAceHomingBullet(void);
void UninitAceHomingBullet(void);
void UpdateAceHomingBullet(void);
void DrawAceHomingBullet(void);

int SetAceHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY);
void DeleteAceHomingBullet(int nIdxAceHomingBullet);

#endif


