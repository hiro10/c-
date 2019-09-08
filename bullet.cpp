//=============================================================================
//
// �e���� [bullet.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "enemy.h"
#include "effect.h"
#include "boss.h"
#include "ace.h"
#include "explosion.h"
#include "player.h"
#include "sound.h"
#include "collision.h"
#include "score.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexBullet(int nIdxBullet, float fSizeX, float fSizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBullet = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				MtxWorldBullet;				// ���[���h�}�g���b�N�X
Bullet					bullet[MAX_BULLET];			// �e���[�N

//*****************************************************************************
// �o���b�g�N���X�̕ϐ��������p�̃����o�[�֐�
//*****************************************************************************
void Bullet::BulletInit()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �X�P�[��
	SizeX = BULLET_SIZE_X;					// �T�C�YX
	SizeY = BULLET_SIZE_Y;					// �T�C�YY
	Timer = 0;								// ����
	radius = BULLET_RADIUS;					// ���a
	use = false;							// �g�p���Ă��邩
}
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexBullet(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BULLET,			// �t�@�C���̖��O
		&D3DTextureBullet);	// �ǂݍ��ރ������[

	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		bullet[CntBullet].BulletInit();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	if (D3DTextureBullet != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureBullet->Release();
		D3DTextureBullet = NULL;
	}

	if (D3DVtxBuffBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffBullet->Release();
		D3DVtxBuffBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	// �v���C���[���擾
	Player *player;
	player = GetPlayer();

	// �J�����̎擾
	Camera	*Cwk;
	Cwk = GetCamerawk();

	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();

	// �G�[�X�̎擾
	Ace*ace;
	ace = GetAce();

	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		// �o���b�g���g�p���Ȃ�
		if (bullet[CntBullet].use)
		{
			if (Cwk->LockOnTrigger == false)
			{
				// �ʏ펞
				bullet[CntBullet].pos.x += bullet[CntBullet].move.x;
				bullet[CntBullet].pos.z += bullet[CntBullet].move.z;

			}
			else if (Cwk->LockOnTrigger == true && GetMode() == MODE_GAME)
			{
				// �ǔ��e�i�G�[�X�p)�̃Z�b�g
				SetTuibi(ace->pos, TUIBI_BULLET_SPEED);
			}
			else if (Cwk->LockOnTrigger == true && GetMode() == MODE_BOSS)
			{
				// �ǔ��e�i�{�X�p)�̃Z�b�g
				SetTuibi(boss->pos, TUIBI_BULLET_SPEED);
			}

			// ���������炷
			bullet[CntBullet].Timer--;

			// �o���b�g�̎�����0�ȉ��̎��A�v���C���[�̏����A�s�k���o�̎�
			if (bullet[CntBullet].Timer <= 0 || player->PlayerMove== 4 || player->PlayerMove == 3)
			{
				// �o���b�g�𖢎g�p��
				bullet[CntBullet].use = false;
			}
			// �����łȂ��Ȃ�
			else
			{
				// �G�t�F�N�g�̐ݒ�
				SetEffect(bullet[CntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.65f, 1.0f, 0.85f, 1.0f), 16.0f, 16.0f, 40);
			}

			// MODE_GAME�̎�
			if (GetMode() == MODE_GAME)
			{
				Enemy *enemy;

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, enemy++)
				{
					// �G�l�~�[���擾
					enemy = GetEnemy(nCntEnemy);

					// �G�l�~�[�ƃo���b�g�̓����蔻��
					if (enemy->use == true && bullet->use == true)
					{
						if (AttackCheak(bullet[CntBullet].pos, enemy->pos,
							bullet[CntBullet].radius, enemy->radius) == true)
						{
							if (player->mp < 100)
							{
								player->mp += 10;
							}
							// SE�̍Đ�
							PlaySound(SOUND_LABEL_SE_EXPLOSION2);

							// �����̔���
							SetExplosion(enemy->pos, 170.0f, 170.0f);

							// �G�l�~�[����
							DeleteEnemy(nCntEnemy);

							// �o���b�g�̏���
							DeleteBullet(CntBullet);

							// �X�R�A���Z
							ChangeScore(100);

							break;
						}
					}
				}

				// �G�[�X�ƃo���b�g�̓����蔻��
				if (AttackCheak(bullet[CntBullet].pos, ace->pos,
					bullet[CntBullet].radius, ace->radius) == true)
				{
					if (player->mp < 100)
					{
						player->mp += 10;
					}
					// SE�̍Đ�
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					// �����̔���
					SetExplosion(ace->pos, 40.0f, 40.0f);

					// HP�̌���
					ace->hp -= BULLET_DAMAGE;

					// �o���b�g�̏���
					DeleteBullet(CntBullet);

					// �X�R�A���Z
					ChangeScore(100);

					break;
				}
			}

			// �{�X��̎�
			if (GetMode() == MODE_BOSS)
			{
				if (boss->use == true&&boss->hp>0)
				{
					if (AttackCheak(bullet[CntBullet].pos, boss->pos,
						bullet[CntBullet].radius, boss->radius) == true)
					{
						if (player->mp < 100)
						{
							player->mp += 10;
						}
						// �o���b�g�̏���
						DeleteBullet(CntBullet);

						// �X�R�A�̑���
						ChangeScore(10);

						// HP�̌���
						boss->hp -= BULLET_DAMAGE;

						// �����̔���
						SetExplosion(bullet[CntBullet].pos, 240.0f, 240.0f);

						// SE�̍Đ�
						PlaySound(SOUND_LABEL_SE_EXPLOSION2);

						break;
					}
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	// ���C�e�B���O�𖳌���
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L����
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, 0);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		if (bullet[CntBullet].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&MtxWorldBullet);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			MtxWorldBullet._11 = mtxView._11;
			MtxWorldBullet._12 = mtxView._21;
			MtxWorldBullet._13 = mtxView._31;
			MtxWorldBullet._21 = mtxView._12;
			MtxWorldBullet._22 = mtxView._22;
			MtxWorldBullet._23 = mtxView._32;
			MtxWorldBullet._31 = mtxView._13;
			MtxWorldBullet._32 = mtxView._23;
			MtxWorldBullet._33 = mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, bullet[CntBullet].scl.x, bullet[CntBullet].scl.y, bullet[CntBullet].scl.z);
			D3DXMatrixMultiply(&MtxWorldBullet, &MtxWorldBullet, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, bullet[CntBullet].pos.x, bullet[CntBullet].pos.y, bullet[CntBullet].pos.z);
			D3DXMatrixMultiply(&MtxWorldBullet, &MtxWorldBullet, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			Device->SetTransform(D3DTS_WORLD, &MtxWorldBullet);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			Device->SetStreamSource(0, D3DVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureBullet);

			// �|���S���̕`��
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntBullet * 4), NUM_POLYGON);
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
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLET,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffBullet,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);

			// �@���̐ݒ�
			Vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 201.0f);
			Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 201.0f);
			Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 201.0f);
			Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 201.0f);

			// �e�N�X�`�����W�̐ݒ�
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBullet(int nIdxBullet, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBullet->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxBullet * 4);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer)
{
	int IdxBullet = -1;

	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		if (!bullet[CntBullet].use)
		{
			bullet[CntBullet].pos = pos;
			bullet[CntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			bullet[CntBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			bullet[CntBullet].move = move;
			bullet[CntBullet].SizeX = fSizeX;
			bullet[CntBullet].SizeY = fSizeY;
			bullet[CntBullet].Timer = nTimer;
			bullet[CntBullet].use = true;

			// ���_���W�̐ݒ�
			SetVertexBullet(CntBullet, fSizeX, fSizeY);

			IdxBullet = CntBullet;

			break;
		}
	}
	return IdxBullet;
}

//=============================================================================
// �e�̍폜
//=============================================================================
void DeleteBullet(int nIdxBullet)
{
	if (nIdxBullet >= 0 && nIdxBullet < MAX_BULLET)
	{
		bullet[nIdxBullet].use = false;
	}
}

//=============================================================================
//�ǔ��e�̐ݒ�(���b�N�I�����̒ʏ�o���b�g�j
//=============================================================================
void SetTuibi(D3DXVECTOR3 pos1,float Speed)
{
	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		//�z�[�~���O�p�̏���
		D3DXVECTOR3 Sabun = (pos1 - bullet[CntBullet].pos);
		D3DXVec3Normalize(&Sabun, &Sabun);

		// ����
		FLOAT Kakudo = D3DXVec3Dot(&Sabun, &bullet[CntBullet].pos);
		Kakudo = acosf(Kakudo);

		if (bullet[CntBullet].pos.z == pos1.z)
		{
			bullet[CntBullet].pos.z -= Speed;
		}

		bullet[CntBullet].pos += (Sabun)*(Speed);
	}
}
