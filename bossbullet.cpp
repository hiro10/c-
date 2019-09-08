//=============================================================================
//
// �{�X�e���� [bossbullet.cpp]
// Author :����  �C�O
//
//=============================================================================
#include "bossbullet.h"
#include "input.h"
#include "camera.h"
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
HRESULT MakeVertexBossBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexBossBullet(int nIdxBossBullet, float SizeX, float SizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBossBullet = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBossBullet = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				WorldBossBullet;				    // ���[���h�}�g���b�N�X
Bullet					BossBullet[MAX_BOSSBULLET];			// �e���[�N



//=============================================================================
// ����������
//=============================================================================
HRESULT InitBossBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexBossBullet(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_BOSSBULLET,		// �t�@�C���̖��O
								&D3DTextureBossBullet);	// �ǂݍ��ރ������[

	
	for (int CntBossBullet = 0; CntBossBullet < MAX_BOSSBULLET; CntBossBullet++)
	{
		BossBullet[CntBossBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		BossBullet[CntBossBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		BossBullet[CntBossBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		BossBullet[CntBossBullet].move = D3DXVECTOR3(1.0f, 1.0f, 10.0f);
		BossBullet[CntBossBullet].SizeX = BOSSBULLET_SIZE_X;
		BossBullet[CntBossBullet].SizeY = BOSSBULLET_SIZE_Y;
		BossBullet[CntBossBullet].radius = BOSSBULLET_RADIUS;
		BossBullet[CntBossBullet].Timer = 0;
		BossBullet[CntBossBullet].use = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossBullet(void)
{
	if (D3DTextureBossBullet != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureBossBullet->Release();
		D3DTextureBossBullet = NULL;
	}

	if (D3DVtxBuffBossBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffBossBullet->Release();
		D3DVtxBuffBossBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossBullet(void)
{
	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();

	// �v���C���[�̎擾
	Player *player;
	player = GetPlayer();

	if (boss->use == true)
	{// �{�X���g�p���Ă���΍X�V
		
		for (int CntBossBullet = 0; CntBossBullet < MAX_BOSSBULLET; CntBossBullet++)
		{
			// ba
			if (BossBullet[CntBossBullet].use)
			{//�o���b�g�̈ʒu�Ɉړ��ʂ����Ă���
				BossBullet[CntBossBullet].pos.x += BossBullet[CntBossBullet].move.x;
				BossBullet[CntBossBullet].pos.y += BossBullet[CntBossBullet].move.y;
				BossBullet[CntBossBullet].pos.z += BossBullet[CntBossBullet].move.z;

				// �{�X��Timer���f�N�������g
				BossBullet[CntBossBullet].Timer--;
				
				// �{�X��Timer��0�ȉ��Ȃ�
				if (BossBullet[CntBossBullet].Timer <= 0)
				{
					// �{�X�o���b�g�𖢎g�p��
					BossBullet[CntBossBullet].use = false;
				}

				// �v���C���[�Ƃ̓����蔻��
				if (AttackCheak(BossBullet[CntBossBullet].pos, player->pos,
					BossBullet[CntBossBullet].radius, player->radius))
				{
					// �{�X�̃o���b�g�𖢎g�p��
					BossBullet[CntBossBullet].use = false;

					// SE�Đ�
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					// �����̔���
					SetExplosion(player->pos, 140.0f, 140.0f);

					// Hp�̌���
					player->hp -= BOSSBULLET_DAMADE;

					break;
				}
			}
		}
	}
	// �{�X�����g�p�Ȃ�
	else if (boss->use == false)
	{
		// �{�X�o���b�g�𖢎g�p��
		BossBullet->use = false;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossBullet(void)
{
	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();
	
	if (boss->use == true)
	{// �{�X���g�p���Ă���Ε`��
		LPDIRECT3DDEVICE9 Device = GetDevice();
		D3DXMATRIX View, Scale, mtxTranslate;

		// ���C�e�B���O�𖳌���
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ���e�X�g��L����
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHAREF, 0);
		Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		for (int nCntBullet = 0; nCntBullet < MAX_BOSSBULLET; nCntBullet++)
		{
			if (BossBullet[nCntBullet].pos.z - GetPositionPlayer().z <= 2500)
			{
				if (BossBullet[nCntBullet].use)
				{
					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&WorldBossBullet);

					// �r���[�}�g���b�N�X���擾
					View = GetMtxView();

					WorldBossBullet._11 = View._11;
					WorldBossBullet._12 = View._21;
					WorldBossBullet._13 = View._31;
					WorldBossBullet._21 = View._12;
					WorldBossBullet._22 = View._22;
					WorldBossBullet._23 = View._32;
					WorldBossBullet._31 = View._13;
					WorldBossBullet._32 = View._23;
					WorldBossBullet._33 = View._33;

					// �X�P�[���𔽉f
					D3DXMatrixScaling(&Scale, BossBullet[nCntBullet].scl.x, BossBullet[nCntBullet].scl.y, BossBullet[nCntBullet].scl.z);
					D3DXMatrixMultiply(&WorldBossBullet, &WorldBossBullet, &Scale);

					// �ړ��𔽉f
					D3DXMatrixTranslation(&mtxTranslate, BossBullet[nCntBullet].pos.x, BossBullet[nCntBullet].pos.y, BossBullet[nCntBullet].pos.z);
					D3DXMatrixMultiply(&WorldBossBullet, &WorldBossBullet, &mtxTranslate);

					// ���[���h�}�g���b�N�X�̐ݒ�
					Device->SetTransform(D3DTS_WORLD, &WorldBossBullet);

					// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
					Device->SetStreamSource(0, D3DVtxBuffBossBullet, 0, sizeof(VERTEX_3D));

					// ���_�t�H�[�}�b�g�̐ݒ�
					Device->SetFVF(FVF_VERTEX_3D);

					// �e�N�X�`���̐ݒ�
					Device->SetTexture(0, D3DTextureBossBullet);

					// �|���S���̕`��
					Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBullet * 4), NUM_POLYGON);
				}
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
HRESULT MakeVertexBossBullet(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BOSSBULLET,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffBossBullet,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBossBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntBossBullet = 0; CntBossBullet < MAX_BOSSBULLET; CntBossBullet++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(-BOSSBULLET_SIZE_X / 2, -BOSSBULLET_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(BOSSBULLET_SIZE_X / 2, -BOSSBULLET_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-BOSSBULLET_SIZE_X / 2, BOSSBULLET_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(BOSSBULLET_SIZE_X / 2, BOSSBULLET_SIZE_Y / 2, 0.0f);

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
		D3DVtxBuffBossBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBossBullet(int IdxBossBullet, float SizeX, float SizeY)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffBossBullet->Lock(0, 0, (void**)&Vtx, 0);

	Vtx += (IdxBossBullet * 4);

	// ���_���W�̐ݒ�
	Vtx[0].vtx = D3DXVECTOR3(-SizeX / 2, -SizeY / 2, 0.0f);
	Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, SizeY / 2, 0.0f);
	Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, -SizeY / 2, 0.0f);
	Vtx[3].vtx = D3DXVECTOR3(SizeX / 2, SizeY / 2, 0.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffBossBullet->Unlock();
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
int SetBossBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float SizeX, float SizeY, int Timer)
{
	int IdxBossBullet = -1;

	for (int CntBossBullet = 0; CntBossBullet < MAX_BOSSBULLET; CntBossBullet++)
	{
		if (!BossBullet[CntBossBullet].use)
		{
			BossBullet[CntBossBullet].pos = pos;
			BossBullet[CntBossBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			BossBullet[CntBossBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			BossBullet[CntBossBullet].move = move;
			BossBullet[CntBossBullet].SizeX = SizeX;
			BossBullet[CntBossBullet].SizeY = SizeY;
			BossBullet[CntBossBullet].Timer = Timer;
			BossBullet[CntBossBullet].use = true;

			// ���_���W�̐ݒ�
			SetVertexBossBullet(CntBossBullet, SizeX, SizeY);

			IdxBossBullet = CntBossBullet;

			break;
		}
	} 

	return IdxBossBullet;
}

//=============================================================================
// �e�̍폜
//=============================================================================
void DeleteBossBullet(int IdxBossBullet)
{
	if (IdxBossBullet >= 0 && IdxBossBullet < MAX_BOSSBULLET)
	{
		BossBullet[IdxBossBullet].use = false;
	}
}
