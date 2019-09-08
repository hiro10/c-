//=============================================================================
//
// �������� [explosion.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EXPLOSION		"data/TEXTURE/pipo-btleffect037.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EXPLOSION_SIZE_X			(50.0f)									// �r���{�[�h�̕�
#define	EXPLOSION_SIZE_Y			(50.0f)									// �r���{�[�h�̍���
#define	VALUE_MOVE_EXPLOSION	(2.0f)									// �ړ����x

#define	MAX_EXPLOSION			(128)									// �ő吔

//*****************************************************************************
// �\����(�N���X)��`
//*****************************************************************************
class EXPLOSION
{
public:
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXCOLOR col;			// �F
	float SizeX;			// ��
	float SizeY;			// ����
	int Counter;			// �J�E���^�[
	int Pattern;			// �p�^�[��No.
	int Type;				// ���
	bool use;				// �g�p���Ă��邩�ǂ���
};

//**************************************
// ���
//**************************************
enum
{
	EXPLOSIONTYPE_BULLET_Player = 0,	// �v���C���[�̒e�̔���
	EXPLOSIONTYPE_BULLET_ENEMY,			// �G�̒e�̔���
	EXPLOSIONTYPE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);

int SetExplosion(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType = EXPLOSIONTYPE_BULLET_Player);

#endif
