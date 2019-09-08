//=============================================================================
//
// �{�X�̃z�[�~���O�e���� [bosshomingbullet .cpp]
// Author :  �����@�C�O
//
//=============================================================================
#include "bosshomingbullet.h"
#include "input.h"
#include "camera.h"
#include "boss.h"
#include "shadow.h"
#include "effect.h"
#include "explosion.h"
#include "player.h"
#include "score.h"
#include "boss.h"
#include "sound.h"
#include "collision.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBossHomingBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexBossHomingBullet(int IdxBossHomingBullet, float SizeX, float SizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBossHomingBullet = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBossHomingBullet = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				MtxWorldBossHomingBullet;				// ���[���h�}�g���b�N�X
Homingbullet		BossHomingBullet[MAX_BOSSHOMINGBULLET];	// �e���[�N

//=============================================================================
// �{�X�z�[�~���O�̃p�����[�^�̏����l
//=============================================================================
void Homingbullet::BossHomingbulletInit()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	SizeX = BOSSHOMINGBULLET_SIZE_X;
	SizeY = BOSSHOMINGBULLET_SIZE_Y;
	radius = BOSSHOMINGBULLET_RADIUS;
	Timer = 0;
	use = false;
}

//=============================================================================
// �{�X�z�[�~���O�̍X�V����
//=============================================================================
void Homingbullet::BossHoming()
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
HRESULT InitBossHomingBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexBossHomingBullet(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						    // �f�o�C�X�ւ̃|�C���^
								TEXTURE_BOSSHOMINGBULLET,		   // �t�@�C���̖��O
								&D3DTextureBossHomingBullet );	// �ǂݍ��ރ������[


	for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++)
	{
		BossHomingBullet[CntBossHomingBullet].BossHomingbulletInit();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossHomingBullet(void)
{
	if (D3DTextureBossHomingBullet != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureBossHomingBullet->Release();
		D3DTextureBossHomingBullet = NULL;
	}

	if (D3DVtxBuffBossHomingBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffBossHomingBullet->Release();
		D3DVtxBuffBossHomingBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossHomingBullet(void)
{
	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();

	// �v���C���[���擾
	Player *player;
	player = GetPlayer();

	if (boss->use == true)
	{// �{�X���g�p���Ă���΍X�V
		D3DXVECTOR3 rotCamera;

		// �J�����̉�]���擾
		rotCamera = GetRotCamera();

		for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++)
		{
			if (BossHomingBullet[CntBossHomingBullet].use)
			{
				BossHomingBullet[CntBossHomingBullet].BossHoming();

				// �v���C���[�Ƃ̓����蔻��
				if (AttackCheak(BossHomingBullet[CntBossHomingBullet].pos, player->pos,
					BossHomingBullet[CntBossHomingBullet].radius, player->radius))
				{
					// �{�X�̃z�[�~���O�e�̏���
					DeleteBossHomingBullet(CntBossHomingBullet);

					// �����̔���
					SetExplosion(player->pos, 40.0f, 40.0f);

					// �v���C���[��HP�̌���
					player->hp -= BOSSHOMINGBULLET_DAMAGE;

					break;
				}
			}
			// �{�X�����񂾂�
			if (boss->use == false)
			{
				// �{�X�̃z�[�~���O�����g�p��
				BossHomingBullet->use = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossHomingBullet(void)
{
	Boss *boss;
	boss = GetBoss();

	if (boss->use == true)
	{// �{�X���g�p���Ă���΍X�V
		LPDIRECT3DDEVICE9 Device = GetDevice();
		D3DXMATRIX MtxView, MtxScale, MtxTranslate;

		// ���C�e�B���O�𖳌���
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ���e�X�g��L����
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHAREF, 0);
		Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++)
		{
			if (BossHomingBullet[CntBossHomingBullet].use)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&MtxWorldBossHomingBullet);

				// �r���[�}�g���b�N�X���擾
				MtxView = GetMtxView();

				MtxWorldBossHomingBullet._11 = MtxView._11;
				MtxWorldBossHomingBullet._12 = MtxView._21;
				MtxWorldBossHomingBullet._13 = MtxView._31;
				MtxWorldBossHomingBullet._21 = MtxView._12;
				MtxWorldBossHomingBullet._22 = MtxView._22;
				MtxWorldBossHomingBullet._23 = MtxView._32;
				MtxWorldBossHomingBullet._31 = MtxView._13;
				MtxWorldBossHomingBullet._32 = MtxView._23;
				MtxWorldBossHomingBullet._33 = MtxView._33;

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&MtxScale, BossHomingBullet[CntBossHomingBullet].scl.x, BossHomingBullet[CntBossHomingBullet].scl.y, BossHomingBullet[CntBossHomingBullet].scl.z);
				D3DXMatrixMultiply(&MtxWorldBossHomingBullet, &MtxWorldBossHomingBullet, &MtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&MtxTranslate, BossHomingBullet[CntBossHomingBullet].pos.x, BossHomingBullet[CntBossHomingBullet].pos.y, BossHomingBullet[CntBossHomingBullet].pos.z);
				D3DXMatrixMultiply(&MtxWorldBossHomingBullet, &MtxWorldBossHomingBullet, &MtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				Device->SetTransform(D3DTS_WORLD, &MtxWorldBossHomingBullet);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				Device->SetStreamSource(0, D3DVtxBuffBossHomingBullet, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				Device->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				Device->SetTexture(0, D3DTextureBossHomingBullet);

				// �|���S���̕`��
				Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntBossHomingBullet * 4), NUM_POLYGON);
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
HRESULT MakeVertexBossHomingBullet(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BOSSHOMINGBULLET,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffBossHomingBullet,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBossHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(-BOSSHOMINGBULLET_SIZE_X / 2, -BOSSHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(BOSSHOMINGBULLET_SIZE_X / 2, -BOSSHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-BOSSHOMINGBULLET_SIZE_X / 2, BOSSHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(BOSSHOMINGBULLET_SIZE_X / 2, BOSSHOMINGBULLET_SIZE_Y / 2, 0.0f);

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
		D3DVtxBuffBossHomingBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBossHomingBullet(int IdxBossHomingBullet, float SizeX, float SizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBossHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxBossHomingBullet * 4);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(-SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, SizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(SizeX / 2, SizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBossHomingBullet->Unlock();
	}
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
int SetBossHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY)
{
	int IdxBossHomingBullet = -1;

	for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++)
	{
		if (!BossHomingBullet[CntBossHomingBullet].use)
		{
			BossHomingBullet[CntBossHomingBullet].start = pos;
			BossHomingBullet[CntBossHomingBullet].bc1 = D3DXVECTOR3((float)(rand() % 400), (float)(rand() % 650 - 500), (float)(rand() % 550)) + pos;
			BossHomingBullet[CntBossHomingBullet].bc2 = D3DXVECTOR3((float)(rand() % 400), (float)(rand() % 650 - 300), (float)(rand() % 90)) + pos;
			BossHomingBullet[CntBossHomingBullet].Frame = 0;
			BossHomingBullet[CntBossHomingBullet].SizeX = SizeX;
			BossHomingBullet[CntBossHomingBullet].SizeY = SizeY;
			BossHomingBullet[CntBossHomingBullet].use = true;
			BossHomingBullet[CntBossHomingBullet].Timer = 2000;
			BossHomingBullet[CntBossHomingBullet].targetpos = GetPositionPlayer();

			// ���_���W�̐ݒ�
			SetVertexBossHomingBullet(CntBossHomingBullet, SizeX, SizeY);

			IdxBossHomingBullet = CntBossHomingBullet;

			break;
		}
	}
	return IdxBossHomingBullet;
}

//=============================================================================
// �e�̍폜
//=============================================================================
void DeleteBossHomingBullet(int IdxBossHomingBullet)
{
	if (IdxBossHomingBullet >= 0 && IdxBossHomingBullet < MAX_BOSSHOMINGBULLET)
	{
		BossHomingBullet[IdxBossHomingBullet].use = false;
	}
}