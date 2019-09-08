//=============================================================================
//
// �{�X���� [boss.h]
// Author : �����C�O
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_CAR			  "data/MODEL/UFO(big).x"	// �ǂݍ��ރ��f����
#define BOSS_TEXTURE_FILENAME "data/TEXTURE/���J��.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE				(3.0f)					// �ړ���	
#define	VALUE_ROTATE			(D3DX_PI * 0.02f)		// ��]��
#define	VALUE_ROTATE_BOSS		(D3DX_PI * 0.025f)		// ��]���x
#define	RATE_ROTATE_BOSS		(0.10f)					// ��]�����W��
#define	VALUE_MOVE_BOSS_BULLET		(20.0f)				// �e�̈ړ����x
#define BOSS_FIRST_POS_Z		(2500)					// �{�X�̍ŏ��̂����̈ʒu
#define BOSS_HP_MAX				(50000)					// �{�X��HP
#define BOSS_RADIUS				(500.0f)				// �{�X�̔��a
#define BOSS_BULLET_SIZE_X		(110)					// �{�X�̒e��X�T�C�Y
#define BOSS_BULLET_SIZE_Y		(110)					// �{�X�̒e��Y�T�C�Y
#define BOSS_DEAD				(0)						// �{�X�̎��S��
#define BOSS_LIFEMAX			(20000)   
#define BOSS_ROT_Y				(0.07f)
#define BOSS_BULLET_QUANTITY_0	(12)					// �{�X�o���b�g�f�[�^�O�̗�
#define BOSS_BULLET_QUANTITY_1	(12)					// �{�X�o���b�g�f�[�^�P�̗�
#define BOSS_HOMING_QUANTITY	(3)						// �{�X�z�[�~���O�o���b�g�̗�
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
Boss *GetBoss(void);
D3DXVECTOR3	GetRotBoss(void);
D3DXVECTOR3	*GetPosBoss(void);
D3DXVECTOR3	GetBossPos(void);
D3DXMATRIX *GetBossWorldMatrix(void);


#endif
