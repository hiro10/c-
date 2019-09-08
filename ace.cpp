//=============================================================================
//
// ACE���� [ace.cpp]
// Author : ���� �C�O
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "bullet.h"
#include "effect.h"
#include "ace.h"
#include "camera.h"
#include "input.h"
#include "explosion.h"
#include "player.h"
#include "score.h"
#include "fade.h"
#include "enemy.h"
#include "acehomingbullet.h"
#include "sound.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureAce = NULL;;	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			D3DXMeshAce = NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatAce = NULL;	// �}�e���A�����ւ̃|�C���^
DWORD				NumMatAce;				// �}�e���A�����̐�
D3DXMATRIX			MtxWorld;				// ���[���h�}�g���b�N�X
Ace					ace;					// ���[�N

//=============================================================================
// �G�[�X�̃����o�ϐ��ɏ����l��������
//=============================================================================
void Ace::AceInitStatus()
{
	// �G�[�X�͖��g�p
	use = false;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	pos = D3DXVECTOR3(0.0f, 40.0f, -1000.0f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	scl = D3DXVECTOR3(3.0f, 3.0f, 3.0f);

	// �G�[�X�̓����p�̕ϐ��̏�����
	ace.AceMoveCount = 0;

	// ���a�̐ݒ�
	radius = ACE_RADIUS;

	// HP(�G�[�X)�̏����l
	hp = ACE_HP_MAX;

}

//=============================================================================
// �G�[�X�̎��S���̓���
//=============================================================================
void Ace::AceDeadMove()
{
	// �ė����o�̂��߂�y���ړ�
	move.y -= 5.0f;

	// �����̔���
	SetExplosion(ace.pos, (float)(rand() % 200), (float)(rand() % 200), 0);

	// �ė��n�_�ɂȂ�����
	if (pos.y < -700)
	{
		// �G�[�X�𖢎g�p
		use = false;

		if (use == false && hp <= 0)
		{
			// �����̔���
			SetExplosion(ace.pos, 1200, 1200, 0);

			// �t�F�[�h�A�E�g
			SetFade(FADE_OUT);

			// �X�R�A�𑝂₷
			ChangeScore(2000);
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT InitAce(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �G�[�X�̃����o�ϐ��ɒl����
	ace.AceInitStatus();

	// D3DXMATERIAL�\���̂̐�
	NumMatAce = 0;

	// ���̂�X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_ACE,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
		Device,										// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatAce,							// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatAce,									// D3DXMATERIAL�\���̂̐�
		&D3DXMeshAce)))								// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

#if 1
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,				// �f�o�C�X�ւ̃|�C���^
		ACE_TEXTURE_FILENAME,						// �t�@�C���̖��O
		&D3DTextureAce);							// �ǂݍ��ރ������[
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitAce(void)
{
	if (D3DXMeshAce != NULL)
	{// ���b�V���̊J��
		D3DXMeshAce->Release();
		D3DXMeshAce = NULL;
	}

	if (D3DXBuffMatAce != NULL)
	{// �}�e���A���̊J��
		D3DXBuffMatAce->Release();
		D3DXBuffMatAce = NULL;
	}

	if (D3DTextureAce != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureAce->Release();
		D3DTextureAce = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateAce(void)
{
	// �v���C���[�̎擾
	Player *player;
	player = GetPlayer();

	// �J�����̎擾
	Camera		*Cwk = GetCamerawk();

	// �o���b�g�̔��ˊԊu
	int ShotCount = 3;

	ace.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �G�[�X���g�p�Ȃ�
	if (ace.use == true)
	{
		if (ace.hp > ACE_DEAD)
		{
			// �{�X�̓����i�����_���Ń{�X�𓮂���)
			if (ace.AceMoveCount == 0)
			{
				//�ŏ��̓���
				ace.move.z += 20;
				ace.pos.z += ace.move.z;
				if (ace.pos.z >= ACE_FIRST_POS_Z)
				{
					ace.pos.z = ACE_FIRST_POS_Z;
					ace.AceMoveCount = 1;
				}
			}
			// �����̕ϐ����P�Ȃ�
			else if (ace.AceMoveCount == 1)
			{
				ace.move.y -= ACE_VALUE_MOVE;
				ace.move.z += ACE_VALUE_MOVE;

				if (ShotCount == 2)
				{
					ace.pos.x += ace.move.x;
					ace.pos.y += ace.move.y;
					ace.pos.z += ace.move.z;
				}
				if (ace.AceMoveCount == 1 && ace.pos.y <= 50)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}

			// �����̕ϐ����Q�Ȃ�
			else if (ace.AceMoveCount == 2)
			{
				ace.move.x -= ACE_VALUE_MOVE;
				ace.move.y += ACE_VALUE_MOVE * 3;
				if (ShotCount == 2)
				{

					ace.pos.x += ace.move.x;
					ace.pos.y += ace.move.y;
					ace.pos.z += ACE_VALUE_MOVE * 2;
				}
				if (ace.AceMoveCount == 2 && ace.pos.x <= -300)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}

			// �����̕ϐ����R�Ȃ�
			else if (ace.AceMoveCount == 3)
			{
				SetAceHomingBullet(GetAcePos(), 10.0f, 10.0f);
				ace.move.x += ACE_VALUE_MOVE;
				ace.pos.x += ace.move.x;

				if (ace.AceMoveCount == 3 && ace.pos.x >= 400)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}

			// �����̕ϐ����S�Ȃ�
			else if (ace.AceMoveCount == 4)
			{
				SetAceHomingBullet(GetAcePos(), 10.0f, 10.0f);
				ace.move.x -= ACE_VALUE_MOVE;
				ace.move.y -= ACE_VALUE_MOVE;
				ace.move.z -= ACE_VALUE_MOVE;
				ace.pos.x -= ace.move.x;
				ace.pos.y -= ace.move.y;
				ace.pos.z += ace.move.z;
				if (ace.pos.x <= -400 && ace.AceMoveCount == 4)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}
			// �����̕ϐ����T�Ȃ�
			else if (ace.AceMoveCount == 5)
			{
				SetAceHomingBullet(GetAcePos(), 10.0f, 10.0f);
				ace.move.x -= ACE_VALUE_MOVE;
				ace.move.y -= ACE_VALUE_MOVE;
				ace.pos.x += ace.move.x;
				ace.pos.y *= ace.move.y;
				if (ace.AceMoveCount == 5 && ace.pos.y <= 40)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}

			// �G�[�X�̈ړ��\�̈�
			if (ace.pos.y > 530.0f)
			{
				ace.pos.y = 530.0f;
			}
			if (ace.pos.y < 15.0f)
			{
				ace.pos.y = 15.0f;
			}

			//�ǂƂ̔���
			if (ace.pos.x <= ACE_MOVE_PLACE_LEFT)
			{//�E���̕�
				ace.pos.x = ACE_MOVE_PLACE_LEFT;
			}

			if (ace.pos.x >= ACE_MOVE_PLACE_RIGHT)
			{//�����̕�
				ace.pos.x = ACE_MOVE_PLACE_RIGHT;
			}

		}

		else if (ace.hp > ACE_DEAD&&ace.hp <= ACE_DEAD + 5500)
		{
			ace.move.z -= 0.0005f;
			ace.pos.z -= ace.move.z;

			if (ace.pos.z = -100)
			{
				SetAceHomingBullet(GetAcePos(), 10.0f, 10.0f);
			}

			if (FLT_EPSILON < D3DXVec3Dot(&ace.pos, &ace.pos))
			{
				ace.rot.y = Cwk->RotCameraP.y + D3DX_PI;
			}
		}
	}
	// HP���O�ȉ��ɂȂ�����
	if (ace.hp <= ACE_DEAD)
	{
		ace.AceDeadMove();
	}

	// ���ۂ̈ړ�
	ace.pos += ace.move;

	// ���f���̌����ڂɃR���W���������킹�Ă݂�
	ace.move = ace.pos + ace.move * 5;


	if (FLT_EPSILON < D3DXVec3Dot(&ace.pos, &ace.pos))
	{
		ace.rot.y = Cwk->RotCameraP.y + D3DX_PI;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawAce(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxScl, MtxRot, MtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;

	//�{�X��true�Ȃ�`�悷��
	if (ace.use == true)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&MtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&MtxScl, ace.scl.x, ace.scl.y, ace.scl.z);
		D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &MtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&MtxRot, ace.rot.y, ace.rot.x, ace.rot.z);
		D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &MtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&MtxTranslate, ace.pos.x, ace.pos.y, ace.pos.z);
		D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &MtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		Device->SetTransform(D3DTS_WORLD, &MtxWorld);

		// ���݂̃}�e���A�����擾
		Device->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		D3DXMat = (D3DXMATERIAL*)D3DXBuffMatAce->GetBufferPointer();

		for (int CntMat = 0; CntMat < (int)NumMatAce; CntMat++)
		{
			// �}�e���A���̐ݒ�
			Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureAce);

			// �`��
			D3DXMeshAce->DrawSubset(CntMat);
		}
		// �}�e���A�����f�t�H���g�ɖ߂�
		Device->SetMaterial(&matDef);



		// �G���W���G�t�F�N�g

			// �E��
		D3DXVECTOR3 Pos;

		Pos.x = ace.pos.x + sinf(ace.rot.y) * 10.0f + 10;
		Pos.y = ace.pos.y;
		Pos.z = ace.pos.z + cosf(ace.rot.y) * 12.0f - 40;
		ace.rot.z = 0.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect(Pos, D3DXVECTOR3(0.0f, 0.0f, -10.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.850f), 24.0f, 24.0f, 20);

		// ����
		D3DXVECTOR3 Posl;

		Posl.x = ace.pos.x + sinf(ace.rot.y) * 10.0f - 10;
		Posl.y = ace.pos.y;
		Posl.z = ace.pos.z + cosf(ace.rot.y) * 12.0f - 40;
		ace.rot.z = 0.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect(Posl, D3DXVECTOR3(0.0f, 0.0f, -10.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.850f), 24.0f, 24.0f, 20);

	}
}

//=============================================================================
// �G�[�X���擾(GET)
//=============================================================================
Ace *GetAce(void)
{
	return &ace;
}

//=============================================================================
// �G�[�X�̈ʒu���擾(GET)
//=============================================================================
D3DXVECTOR3	GetAcePos(void)
{
	return ace.pos;
}
//=============================================================================
// �G�[�X�̈ʒu���擾(GET)
//=============================================================================
D3DXVECTOR3	*GetPosAce(void)
{
	return (&ace.pos);
}
//=============================================================================
// �G�[�X�̉�]���擾(GET)
//=============================================================================
D3DXVECTOR3	GetRotAce(void)
{
	return(ace.rot);
}

//=============================================================================
// �G�[�X�̃��[���h�}�g���N�X���擾(GET)
//=============================================================================
D3DXMATRIX *GetAceWorldMatrix(void)
{
	return (&MtxWorld);
}
