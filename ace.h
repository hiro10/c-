//=============================================================================
//
// �G�[�X���� [ace.h]
// Author : �����C�O
//
//=============================================================================
#ifndef _ACE_H_
#define _ACE_H_

//*****************************************************************************
//�C���N���[�h
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ACE				"data/MODEL/ACE.x"						// �ǂݍ��ރ��f����
#define ACE_TEXTURE_FILENAME	"data/TEXTURE/Sukhoi__S37_Berkut.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	ACE_VALUE_MOVE			(8.0f)								// �ړ���
#define	VALUE_ROTATE			(D3DX_PI * 0.02f)					// ��]��
#define ACE_MOVE_PLACE_LEFT		(-840.0f)							// �����̃v���C���[�̈ړ����E
#define ACE_MOVE_PLACE_RIGHT	(840.0f)							// �E���̃v���C���[�̈ړ����E
#define ACE_FIRST_POS_Z			(1500)								// ACE�̍ŏ��̂����̈ʒu
#define ACE_HP_MAX				(20000)								// ACE��HP
#define ACE_RADIUS				(50.0f)								// ACE�̔��a
#define ACE_DEAD				(0)									// ACE�̎��S����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitAce(void);
void UninitAce(void);
void UpdateAce(void);
void DrawAce(void);
Ace *GetAce(void);
D3DXVECTOR3	GetRotAce(void);
D3DXVECTOR3	*GetPosAce(void);
D3DXVECTOR3	GetAcePos(void);
D3DXMATRIX *GetAceWorldMatrix(void);


#endif
