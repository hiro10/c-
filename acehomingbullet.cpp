//=============================================================================
//
// �{�X�̃z�[�~���O�e���� [acehomingbullet .cpp]
// Author :  �����@�C�O
//
//=============================================================================
#include "acehomingbullet.h"
#include "input.h"
#include "camera.h"
#include "ace.h"
#include "shadow.h"
#include "effect.h"
#include "explosion.h"
#include "player.h"
#include "score.h"
#include "ace.h"
#include "sound.h"
#include "collision.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexAceHomingBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexAceHomingBullet(int IdxAceHomingBullet, float SizeX, float SizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureAceHomingBullet = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffAceHomingBullet = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				MtxWorldAceHomingBullet;				// ���[���h�}�g���b�N�X
Homingbullet			AceHomingBullet[MAX_ACEHOMINGBULLET];	// �e���[�N

//=============================================================================
// �G�[�X�̃z�[�~���O�e�̃p�����[�^�����l
//=============================================================================
void Homingbullet::AceHomingbulletInit()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	SizeX = ACEHOMINGBULLET_SIZE_X;
	SizeY = ACEHOMINGBULLET_SIZE_Y;
	radius = ACEHOMINGBULLET_RADIUS;
	Timer = 0;
	use = false;
}

//=============================================================================
// �G���ׂẴz�[�~���O�̏���
//=============================================================================
void Homingbullet::AceHoming()
{
	Frame++;

	float t = (float)Frame / 60.0f;
	//�n�_
	D3DXVECTOR3 p0 = (1 - t)*(1 - t)*(1 - t)*start;
	//�ʉߓ_�P
	D3DXVECTOR3 p1 = 3 * (1 - t)*(1 - t)*t*bc1;
	//�ʉߓ_�Q
	D3DXVECTOR3 p2 = 3 * (1 - t)*t*t*bc2;
	// �I�_
	D3DXVECTOR3 p3 = t * t*t*targetpos;

	pos = p0 + p1 + p2 + p3;

	if (pos.z <= -100)
	{
		// ���g�p
		use = false;
	}
	else
	{
		// �G�t�F�N�g�̐ݒ�
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.05f, 0.05f, 0.05f, 1.50f), 46.0f, 46.0f, 30);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.05f, 0.05f, 0.45f, 1.20f), 6.0f, 6.0f, 50);
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT InitAceHomingBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexAceHomingBullet(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,	    // �f�o�C�X�ւ̃|�C���^
		TEXTURE_ACEHOMINGBULLET,			// �t�@�C���̖��O
		&D3DTextureAceHomingBullet);		// �ǂݍ��ރ������[


	for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++)
	{
		AceHomingBullet[CntAceHomingBullet].AceHomingbulletInit();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitAceHomingBullet(void)
{
	if (D3DTextureAceHomingBullet != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureAceHomingBullet->Release();
		D3DTextureAceHomingBullet = NULL;
	}

	if (D3DVtxBuffAceHomingBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffAceHomingBullet->Release();
		D3DVtxBuffAceHomingBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateAceHomingBullet(void)
{
	Ace *ace;
	ace = GetAce();

	// �v���C���[���擾
	Player *Player;
	Player = GetPlayer();

	if (ace->use == true)
	{// �{�X���g�p���Ă���΍X�V
		
		// �J�����̉�]���擾
		D3DXVECTOR3 rotCamera;
		rotCamera = GetRotCamera();

		// �z�[�~���O�o���b�g��MAX_ACEHOMINGBULLET���J��Ԃ�
		for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++)
		{
			// �z�[�~���O�o���b�g���g�p���Ă����
			if (AceHomingBullet[CntAceHomingBullet].use)
			{
				AceHomingBullet[CntAceHomingBullet].AceHoming();

				// �v���C���[�Ƃ̓����蔻��
				if (AttackCheak(AceHomingBullet[CntAceHomingBullet].pos, Player->pos,
					AceHomingBullet[CntAceHomingBullet].radius, Player->radius))
				{
					// �{�X�̃z�[�~���O�e�̏���
					DeleteAceHomingBullet(CntAceHomingBullet);

					// �����̔���
					SetExplosion(Player->pos, 40.0f, 40.0f);

					// SE�Đ�
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					// �v���C���[��HP�̌���
					Player->hp -= ACEHOMINGBULLET_DAMAGE;

					break;
				}
			}
			if (ace->use == false)
			{
				AceHomingBullet->use = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawAceHomingBullet(void)
{
	// ACE�̎擾
	Ace *ace;
	ace = GetAce();

	if (ace->use == true)
	{// �{�X���g�p���Ă���΍X�V
		LPDIRECT3DDEVICE9 Device = GetDevice();
		D3DXMATRIX MtxView, MtxScale, MtxTranslate;

		// ���C�e�B���O�𖳌���
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ���e�X�g��L����
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHAREF, 0);
		Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++)
		{
			if (AceHomingBullet[CntAceHomingBullet].use)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&MtxWorldAceHomingBullet);

				//// �r���[�}�g���b�N�X���擾
				MtxView = GetMtxView();

				MtxWorldAceHomingBullet._11 = MtxView._11;
				MtxWorldAceHomingBullet._12 = MtxView._21;
				MtxWorldAceHomingBullet._13 = MtxView._31;
				MtxWorldAceHomingBullet._21 = MtxView._12;
				MtxWorldAceHomingBullet._22 = MtxView._22;
				MtxWorldAceHomingBullet._23 = MtxView._32;
				MtxWorldAceHomingBullet._31 = MtxView._13;
				MtxWorldAceHomingBullet._32 = MtxView._23;
				MtxWorldAceHomingBullet._33 = MtxView._33;

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&MtxScale, AceHomingBullet[CntAceHomingBullet].scl.x, AceHomingBullet[CntAceHomingBullet].scl.y, AceHomingBullet[CntAceHomingBullet].scl.z);
				D3DXMatrixMultiply(&MtxWorldAceHomingBullet, &MtxWorldAceHomingBullet, &MtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&MtxTranslate, AceHomingBullet[CntAceHomingBullet].pos.x, AceHomingBullet[CntAceHomingBullet].pos.y, AceHomingBullet[CntAceHomingBullet].pos.z);
				D3DXMatrixMultiply(&MtxWorldAceHomingBullet, &MtxWorldAceHomingBullet, &MtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				Device->SetTransform(D3DTS_WORLD, &MtxWorldAceHomingBullet);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				Device->SetStreamSource(0, D3DVtxBuffAceHomingBullet, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				Device->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				Device->SetTexture(0, D3DTextureAceHomingBullet);

				// �|���S���̕`��
				Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAceHomingBullet * 4), NUM_POLYGON);
			}
		}

		// ���C�e�B���O��L����
		Device->SetRenderState(D3DRS_LIGHTING, TRUE);

		// ���e�X�g�𖳌���
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexAceHomingBullet(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_ACEHOMINGBULLET,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffAceHomingBullet,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffAceHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(-ACEHOMINGBULLET_SIZE_X / 2, -ACEHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(ACEHOMINGBULLET_SIZE_X / 2, -ACEHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-ACEHOMINGBULLET_SIZE_X / 2, ACEHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(ACEHOMINGBULLET_SIZE_X / 2, ACEHOMINGBULLET_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			Vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffAceHomingBullet->Unlock();
	}

	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexAceHomingBullet(int IdxAceHomingBullet, float SizeX, float SizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffAceHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxAceHomingBullet * 4);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(-SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, SizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(SizeX / 2, SizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffAceHomingBullet->Unlock();
	}
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
int SetAceHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY)
{
	int IdxAceHomingBullet = -1;

	for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++)
	{
		if (!AceHomingBullet[CntAceHomingBullet].use)
		{
			AceHomingBullet[CntAceHomingBullet].start = pos;
			AceHomingBullet[CntAceHomingBullet].bc1 = D3DXVECTOR3((float)(rand() % 400), (float)(rand() % 650 - 500), (float)(rand() % 550)) + pos;
			AceHomingBullet[CntAceHomingBullet].bc2 = D3DXVECTOR3((float)(rand() % 400), (float)(rand() % 650 - 300), (float)(rand() % 90)) + pos;
			AceHomingBullet[CntAceHomingBullet].Frame = 0;
			AceHomingBullet[CntAceHomingBullet].SizeX = SizeX;
			AceHomingBullet[CntAceHomingBullet].SizeY = SizeY;
			AceHomingBullet[CntAceHomingBullet].use = true;
			AceHomingBullet[CntAceHomingBullet].Timer = 2000;
			AceHomingBullet[CntAceHomingBullet].targetpos = GetPositionPlayer();

			// ���_���W�̐ݒ�
			SetVertexAceHomingBullet(CntAceHomingBullet, SizeX, SizeY);

			IdxAceHomingBullet = CntAceHomingBullet;

			break;
		}
	}
	return IdxAceHomingBullet;
}

//=============================================================================
// �e�̍폜
//=============================================================================
void DeleteAceHomingBullet(int IdxAceHomingBullet)
{
	if (IdxAceHomingBullet >= 0 && IdxAceHomingBullet < MAX_ACEHOMINGBULLET)
	{
		AceHomingBullet[IdxAceHomingBullet].use = false;
	}
}
