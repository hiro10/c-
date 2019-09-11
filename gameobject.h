//=============================================================================
//
// Class���� [gameobject.h]
// Author : �����C�O
//
//=============================================================================
#include "main.h"

//******************************************************************************
// �Q�[���I�u�W�F�N�g�N���X�i���N���X�j
//******************************************************************************
class GameObject
{
public:

	D3DXVECTOR3			pos;				// �v���C���[�̈ʒu
	D3DXVECTOR3			rot;				// �v���C���[�̌���(��])
	D3DXVECTOR3			scl;				// �v���C���[�̑傫��(�X�P�[��)
	D3DXVECTOR3			move;				// �ړ���
	D3DXVECTOR3			RotDest;			// �ړI�̌���
	bool				use;				// �g�p�A���g�p�̔���
	int					hp;					// ���ʂ�HP����
	float               radius;				// ���a
};


//*******************************************************************************
//�@�q�N���X
//*******************************************************************************

//=============================================================================
// �v���C���[�N���X�i�e�FGameObject)
//=============================================================================
class Player :public GameObject
{
public:

	int                PlayerMove;			// �v���C���[�̓���(0,���ꎞ�A1,�Q�[���v���C���A2,�s�k���A3,������)
	int                mp;					// �o���b�g���˂܂ł̂����̐ݒ�
	int                IdxShadow;			// �eID
	int                Flame;
	void               FirstBattlePlayerInitStatus();	// 1st�X�e�[�W�p�̃v���C���[�̏���������
	void               BossBattlePlayerInitStatus();	// �{�X�X�e�[�W�p�̃v���C���[�̏���������
	void			   PlayerDeadMove(float a, float b);
	void			   PlayerWinMove(int z);
	void			   PlayerMoveLimit();
};

//=============================================================================
// �G�[�X�N���X�i�e�FGameObject)
//=============================================================================
class Ace :public GameObject
{
public:
	
	int                ShotCount;		// �o���b�g�̔��ˊԊu
	int				   AceMoveCount;	// �����̔���p�ϐ�
	void			   AceInitStatus(); // �G�[�X�̃N���X�p�����[�^�������֐�
	void			   AceDeadMove();	// �G�[�X�̎��S���̓����̊֐�
	
};

//=============================================================================
// �{�X�N���X�i�e�FGameObject)
//=============================================================================
class Boss:public GameObject
{
public:
	void BossInitStatus();					// �{�X�̃N���X�p�����[�^�������̊֐�

};

//=============================================================================
// �G�l�~�[�N���X�i�e�FGameObject)
//=============================================================================
class Enemy :public GameObject
{
public:
	
	
	int		          IdxShadow;	// �eID
	int		          Type;			// ���

	//�x�W�F
	D3DXVECTOR3       start;		//�z�[�~���O�J�n�ʒu
	D3DXVECTOR3       c1;			//�ʉ߃|�C���g�P
	D3DXVECTOR3       c2;			//�ʉ߃|�C���g�Q

	void EnemyInit();				// �G�l�~�[�̃N���X�p�����[�^�������̊֐�

};

//=============================================================================
// �o���b�g�N���X�i�e�FGameObject)
//=============================================================================
class Bullet :public GameObject
{
public:
	float SizeX;			// ��
	float SizeY;			// ����
	int Timer;				// �^�C�}�[
	int IdxShadow;			// �eID

	void BulletInit();		// �o���b�g�̃N���X�p�����[�^�������̊֐�
};

//=============================================================================
// �z�[�~���O�N���X�i�e�FBullet)
//=============================================================================

class  Homingbullet :public Bullet
{
public:
	int  Frame;
	int HomingGage;

	//�x�W�F
	D3DXVECTOR3 start;		// �z�[�~���O�J�n�ʒu
	D3DXVECTOR3 bc1;		// �ʉ݃|�C���g�P
	D3DXVECTOR3 bc2;		// �ʉ݃|�C���g�Q
	D3DXVECTOR3 targetpos;  // �z�[�~���O�̏I�_

	void BossHomingbulletInit();	//�{�X�p�̏������֐�
	void AceHomingbulletInit();		//�G�[�X�p�̏������֐�
	void PlayerHomingbulletInit();	//�v���C���[�p�̏������֐�

	void PlayerHoming(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);	//�v���C���[�̃z�[�~���O
	void AceHoming();										// �G�[�X�̃z�[�~���O
	void BossHoming();										// �{�X�̃z�[�~���O
};

//=============================================================================
// �G�t�F�N�g�N���X�i�e�FGameObject)
//=============================================================================
class Effect1 :public GameObject
{
public:
	D3DXCOLOR col;			// �F
	float SizeX;			// ��
	float SizeY;			// ����
	int   Count;			// �J�E���g
	int   Pattern;			// �p�^�[��
	int	  Timer;			// �^�C�}�[
	float DecAlpha;			// �����l
};

//=============================================================================
// �A�C�e���N���X�i�e�FGameObject)
//=============================================================================
class Item :public GameObject
{
public:

	int IdxShadow;			// �eID
	int Type;				// ���
};


#pragma once