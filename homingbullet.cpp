//=============================================================================
//
// �e���� [homingbullet .cpp]
// Author :���� �C�O
//
//=============================================================================
#include "homingbullet.h"
#include "input.h"
#include "camera.h"
#include "boss.h"
#include "shadow.h"
#include "effect.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "fade.h"
#include "Ace.h"
#include "score.h"
#include "boss.h"
#include "sound.h"
#include "collision.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexHomingBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexHomingBullet(int IdxHomingBullet, float SizeX, float SizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureHomingBullet = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffHomingBullet = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				WorldHomingBullet;					// ���[���h�}�g���b�N�X
Homingbullet			HomingBullet[MAX_HOMINGBULLET];		// �e���[�N

//=============================================================================
// �v���C���[�̃z�[�~���O�e�̏���
//=============================================================================
void Homingbullet::PlayerHoming(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	// �^�C�}�[���f�N�������g
	Timer--;

	// �t���[�����C���N�������g
	Frame++;

	float t = Frame / 60.0f;
	// �n�_
	D3DXVECTOR3 p0 = (1 - t)*(1 - t)*(1 - t)*start;
	// �ʉߓ_�P
	D3DXVECTOR3 p1 = 3 * (1 - t)*(1 - t)*t*bc1;
	// �ʉߓ_�Q
	D3DXVECTOR3 p2 = 3 * (1 - t)*t*t*bc2;

	// �G�l�~�[��̎�
	if (GetMode() == MODE_GAME)
	{
		// �I�_(Mode=1�̎�)
		D3DXVECTOR3 p3 = t * t*t*pos1;//GetAcePos();
		pos = p0 + p1 + p2 + p3;
	}
	// �{�X��̎�
	else if (GetMode() == MODE_BOSS)
	{
		// �I�_(����ȊO�̎�)
		D3DXVECTOR3 p3 = t * t*t*pos2;//GetBossPos();
		pos = p0 + p1 + p2 + p3;
	}
	if (pos == pos1 || pos == pos2)
	{// ���g�p
		use = false;
	}
	else
	{	// �G�t�F�N�g�̐ݒ�
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 1.0f),
			D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f), 16.0f, 16.0f, 10);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 1.0f),
			D3DXCOLOR(0.5f, 0.5f, 1.0f, 0.30f), 22.0f, 22.0f, 150);
	}
}

//=============================================================================
// �v���C���[�̃z�[�~���O�e�̃p�����[�^�����l
//=============================================================================
void Homingbullet::PlayerHomingbulletInit()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �傫��
	SizeX = HOMINGBULLET_SIZE_X;			// X�T�C�Y
	SizeY = HOMINGBULLET_SIZE_Y;			// Y�T�C�Y
	radius = 42.0f;							// ���a
	Timer = 100;							// ���C�t
	use = false;							// use�̔���
	HomingGage = 0;
}
//=============================================================================
// ����������
//=============================================================================
HRESULT InitHomingBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexHomingBullet(Device);

	for (int CntHomingBullet = 0; CntHomingBullet < MAX_HOMINGBULLET ; CntHomingBullet++)
	{
		// �������p�̃����o�֐����Ă�
		HomingBullet[CntHomingBullet].PlayerHomingbulletInit();
	}
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHomingBullet(void)
{
	if (D3DTextureHomingBullet != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureHomingBullet->Release();
		D3DTextureHomingBullet = NULL;
	}

	if (D3DVtxBuffHomingBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffHomingBullet->Release();
		D3DVtxBuffHomingBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHomingBullet(void)
{
	// �{�X���擾
	Boss *boss;
	boss = GetBoss();

	// �G�[�X���擾
	Ace *ace;
	ace = GetAce();

	for (int CntHomingBullet = 0; CntHomingBullet < MAX_HOMINGBULLET; CntHomingBullet++)
	{
		//�x�W�F�Ȑ�
		if (HomingBullet[CntHomingBullet].use)
		{//�o���b�g�̈ʒu�Ɉړ��ʂ����Ă���

			// �z�[�~���O����(1st�X�e�[�W�ABoss�X�e�[�W)
			HomingBullet[CntHomingBullet].PlayerHoming(GetAcePos(),GetBossPos());
			
			// �{�X��̎�
			if (GetMode() ==MODE_BOSS)
			{
				// �{�X�Ƃ̓����蔻��
				if(AttackCheak(HomingBullet[CntHomingBullet].pos, boss->pos,
					HomingBullet[CntHomingBullet].radius, boss->radius))
				{
					if (boss->use == true)
					{
						// �v���C���[�̃����o�ϐ���mp�����炷
						GetPlayer()->mp -= 20;

						// SE�Đ�
						PlaySound(SOUND_LABEL_SE_EXPLOSION2);

						// �z�[�~���O�̏���
						DeleteHomingBullet(CntHomingBullet);

						// �G�t�F�N�g�̔���
						SetExplosion(boss->pos, 240.0f, 240.0f);

						// Hp�̌���
						boss->hp -= HOMING_DAMAGE;

						break;
					}
				}
			}
			else
			{
				// �G�[�X�Ƃ̓����蔻��
				if (AttackCheak(HomingBullet[CntHomingBullet].pos, ace->pos,
					HomingBullet[CntHomingBullet].radius, ace->radius))
				{
					// �v���C���[�̃����o�ϐ���mp�����炷
					GetPlayer()->mp -= 20;

					// SE�Đ�
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					// �G�[�X�̏ꏊ�ŁA��������
					SetExplosion(ace->pos, 240.0f, 240.0f);

					// Hp�̌���
					ace->hp -= HOMING_DAMAGE;

					break;
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHomingBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxView,MtxScale,MtxTranslate;

	// ���C�e�B���O�𖳌���
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L����
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, 0);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for(int CntHomingBullet  = 0; CntHomingBullet  < MAX_HOMINGBULLET; CntHomingBullet ++)
	{
		// �z�[�~���O�o���b�g���g�p���Ȃ�
		if(HomingBullet [CntHomingBullet].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&WorldHomingBullet);

			// �r���[�}�g���b�N�X���擾
			MtxView = GetMtxView();

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&MtxScale, HomingBullet [CntHomingBullet ].scl.x, HomingBullet [CntHomingBullet ].scl.y, HomingBullet [CntHomingBullet ].scl.z);
			D3DXMatrixMultiply(&WorldHomingBullet , &WorldHomingBullet , &MtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&MtxTranslate, HomingBullet [CntHomingBullet ].pos.x, HomingBullet [CntHomingBullet ].pos.y, HomingBullet [CntHomingBullet ].pos.z);
			D3DXMatrixMultiply(&WorldHomingBullet , &WorldHomingBullet , &MtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			Device->SetTransform(D3DTS_WORLD, &WorldHomingBullet );

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			Device->SetStreamSource(0, D3DVtxBuffHomingBullet , 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_3D);
		}
	}

	// ���C�e�B���O��L����
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexHomingBullet(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_HOMINGBULLET,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffHomingBullet,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntHomingBullet = 0; CntHomingBullet < MAX_HOMINGBULLET; CntHomingBullet++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(-HOMINGBULLET_SIZE_X / 2, -HOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(HOMINGBULLET_SIZE_X / 2, -HOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-HOMINGBULLET_SIZE_X / 2, HOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(HOMINGBULLET_SIZE_X / 2, HOMINGBULLET_SIZE_Y / 2, 0.0f);

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
		D3DVtxBuffHomingBullet->Unlock();
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexHomingBullet(int IdxHomingBullet, float SizeX, float SizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxHomingBullet * 4);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(-SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, SizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(SizeX / 2, SizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffHomingBullet->Unlock();
	}
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
int SetHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY)
{
	int IdxHomingBullet = -1;

	// �ʉߓ_���΂炯�����邽�߂̕ϐ�
	int Angle;

	for (int CntHomingBullet = 0; CntHomingBullet < MAX_HOMINGBULLET; CntHomingBullet++)
	{
		//�z�[�~���O�o���b�g���g�p���Ȃ�
		if (!HomingBullet[CntHomingBullet].use)
		{
			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_SHOT);

			// �ʉߓ_�������_���ɂ��邽�߂̃����_��
			Angle =rand() % 2;
			HomingBullet[CntHomingBullet].start = pos;

			// �ʉߓ_�̐ݒ�
			if (Angle == 1)
			{
				HomingBullet[CntHomingBullet].bc1 = D3DXVECTOR3((float)(rand() % 400 - 350 / 1.0f), (float)(rand() % 650 ) / 1.0f, (float)(rand() % 250) / 1.0f) + pos;
				HomingBullet[CntHomingBullet].bc2 = D3DXVECTOR3((float)(rand() % 400 - 350 / 1.0f), (float)(rand() % 650 -500) / 1.0f, (float)(rand() % 50) / 1.0f) + pos;
			}
			if (Angle ==0)
			{
				HomingBullet[CntHomingBullet].bc1 = D3DXVECTOR3((float)(rand() % 350 / 1.0f), (float)(rand() %300) / 1.0f, (float)(rand() % 250) / 1.0f) + pos;
				HomingBullet[CntHomingBullet].bc2 = D3DXVECTOR3((float)(rand() % 350 / 1.0f), (float)(rand() % 100) / 1.0f, (float)(rand() % 50) / 1.0f) + pos;
			}

			HomingBullet[CntHomingBullet].Frame = 0;
			HomingBullet[CntHomingBullet].SizeX = SizeX;
			HomingBullet[CntHomingBullet].SizeY = SizeY;
			HomingBullet[CntHomingBullet].use = true;
			HomingBullet[CntHomingBullet].Timer = 2600;

			// ���_���W�̐ݒ�
			SetVertexHomingBullet(CntHomingBullet, SizeX, SizeY);

			IdxHomingBullet = CntHomingBullet;

			break;
		}
	}

	return IdxHomingBullet;
}

//=============================================================================
// �e�̍폜
//=============================================================================
void DeleteHomingBullet(int IdxHomingBullet)
{
	if (IdxHomingBullet >= 0 && IdxHomingBullet < MAX_HOMINGBULLET)
	{
		HomingBullet[IdxHomingBullet].use = false;
	}
}

