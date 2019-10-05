
//=============================================================================
//
// �{�X���� [boss.cpp]
// Author : �����C�O
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "bullet.h"
#include "effect.h"
#include "boss.h"
#include "camera.h"
#include "bossbullet.h"
#include "bosshomingbullet.h"
#include "input.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
#include "effekseer.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureBoss;		// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			D3DXMeshBoss;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatBoss;	// �}�e���A�����ւ̃|�C���^
DWORD				NumMatBoss;			// �}�e���A�����̐�
D3DXMATRIX			BossWorld;			// ���[���h�}�g���b�N�X
Boss				boss;				// �{�X���[�N

//*****************************************************************************
// �{�X�p�o���b�g�f�[�^(ShotCount=0)
//*****************************************************************************
static D3DXVECTOR3 BossBulletData0[BOSS_BULLET_QUANTITY_0]
{// �{�X�̃o���b�g�O�̔��˒n�_ D3DXVECTOR3(x,y,z)
	
	{BossBulletData0[0] = D3DXVECTOR3(0,1400,0)},
	{BossBulletData0[1] = D3DXVECTOR3(0,1100,-30)},
	{BossBulletData0[2] = D3DXVECTOR3(0,1000,30)},
	{BossBulletData0[3] = D3DXVECTOR3(0,930,30)},
	{BossBulletData0[4] = D3DXVECTOR3(0,740,30)},
	{BossBulletData0[5] = D3DXVECTOR3(0,640,30)},
	{BossBulletData0[6] = D3DXVECTOR3(300,640,100)},
	{BossBulletData0[7] = D3DXVECTOR3(-100,330,0)},
	{BossBulletData0[8] = D3DXVECTOR3(100,240,0)},
	{BossBulletData0[9] = D3DXVECTOR3(0,-100,0)},
	{BossBulletData0[10] = D3DXVECTOR3(100,-200,0)},
	{BossBulletData0[11] = D3DXVECTOR3(100,-500,0)},
};
//*****************************************************************************
// �{�X�p�o���b�g�f�[�^(ShotCount=1)
//*****************************************************************************
static D3DXVECTOR3 BossBulletData1[BOSS_BULLET_QUANTITY_1]
{// �{�X�̃o���b�g�P�̔��˒n�_ D3DXVECTOR3(x,y,z)

	{BossBulletData1[0] = D3DXVECTOR3(0,1300,0)},
	{BossBulletData1[1] = D3DXVECTOR3(0,1000,0)},
	{BossBulletData1[2] = D3DXVECTOR3(0,800,30)},
	{BossBulletData1[3] = D3DXVECTOR3(0,700,30)},
	{BossBulletData1[4] = D3DXVECTOR3(0,600,300)},
	{BossBulletData1[5] = D3DXVECTOR3(0,500,300)},
	{BossBulletData1[6] = D3DXVECTOR3(300,400,100)},
	{BossBulletData1[7] = D3DXVECTOR3(200,130,10)},
	{BossBulletData1[8] = D3DXVECTOR3(-200,40,10)},
	{BossBulletData1[9] = D3DXVECTOR3(0,-100,10)},
	{BossBulletData1[10] = D3DXVECTOR3(100,-400,0)},
	{BossBulletData1[11] = D3DXVECTOR3(100,-600,10)},
};

//*****************************************************************************
// �{�X�p�z�[�~���O�o���b�g�f�[�^(
//*****************************************************************************
static D3DXVECTOR3 BossHomingBulletData[BOSS_HOMING_QUANTITY]
{// �{�X�̃z�[�~���O�o���b�g�̔��˒n�_ D3DXVECTOR3(x,y,z)

	{BossHomingBulletData[0] = D3DXVECTOR3(0, 550,0)},
	{BossHomingBulletData[1] = D3DXVECTOR3(450,0,0)},
	{BossHomingBulletData[2] = D3DXVECTOR3(-450,0,0)},
};

//*****************************************************************************
// �{�X�N���X�ɏ����l����
//*****************************************************************************
void Boss::BossInitStatus()
{
	//�{�X�͖��g�p
	use = false;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	pos = D3DXVECTOR3(0.0f, 3240.0f, 2600.0f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	scl = D3DXVECTOR3(40.0f, 40.0f, 40.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���a�̐ݒ�
	radius = BOSS_RADIUS;

	//HP(�{�X)�̏����l
	hp = BOSS_HP_MAX;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBoss(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �{�X�̃p�����[�^�ɒl��ݒ�
	boss.BossInitStatus();

	// �e�N�X�`���̏�����
	D3DTextureBoss = NULL;

	// ���b�V���̏�����
	D3DXMeshBoss = NULL;

	// �}�e���A���̏�����
	D3DXBuffMatBoss = NULL;
	NumMatBoss = 0;

	Device->SetRenderState(D3DRS_BLENDOPALPHA, TRUE);

	//X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,								// ���b�V���̍쐬�I�v�V�������w��
		Device,											// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,											// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatBoss,								// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,											// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatBoss,									// D3DXMATERIAL�\���̂̐�
		&D3DXMeshBoss)))								// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

#if 1
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
		BOSS_TEXTURE_FILENAME,							// �t�@�C���̖��O
		&D3DTextureBoss);								// �ǂݍ��ރ������[
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBoss(void)
{
	if (boss.use == true)
	{// �����{�X���g�p���Ȃ疢�g�p��
		boss.use = false;
	}

	if (D3DXMeshBoss != NULL)
	{// ���b�V���̊J��
		D3DXMeshBoss->Release();
		D3DXMeshBoss = NULL;
	}

	if (D3DXBuffMatBoss != NULL)
	{// �}�e���A���̊J��
		D3DXBuffMatBoss->Release();
		D3DXBuffMatBoss = NULL;
	}

	if (D3DTextureBoss != NULL)
	{//�e�N�X�`���̊J��
		D3DTextureBoss->Release();
		D3DTextureBoss = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoss(void)
{
	
	

	// �o���b�g�̔��ˊԊu
	int ShotCount = rand() % 6;						

	// �{�X��true��
	boss.use = true;								

	// �{�X�̉�]
	boss.rot.y -= BOSS_ROT_Y;

	boss.move.x = -sinf(boss.rot.y) * VALUE_MOVE_BOSS_BULLET;
	boss.move.y = 0;
	boss.move.z = -cosf(boss.rot.y) * VALUE_MOVE_BOSS_BULLET;

	// �{�X���g�p���Ă����
	if (boss.use == true)
	{
		if (boss.pos.y >= 1200)
		{
			// �{�X�퓱�����̉��~���o
			boss.pos.y -= 20.0f;
		}

		// y�������l�ɂȂ�����
		if (boss.pos.y <= 1200)
		{

			// �{�X�o���b�g�̃Z�b�g
			if (ShotCount == 0)
			{//ShotCount���O�̂Ƃ��̃{�X�̃o���b�g

				for (int BulletDataCount0 = 0; BulletDataCount0 < BOSS_BULLET_QUANTITY_0; BulletDataCount0++)
				{// BOSS_BULLET_QUANTITY_0��J��Ԃ�
					SetBossBullet(D3DXVECTOR3(boss.pos.x+BossBulletData0[BulletDataCount0].x,
						boss.pos.y + BossBulletData0[BulletDataCount0].y,
						boss.pos.z + BossBulletData0[BulletDataCount0].z),
						boss.move, BOSS_BULLET_SIZE_X, BOSS_BULLET_SIZE_Y, 100 * 3);
				}
			}
			else if (ShotCount == 1)
			{//ShotCount���P�̂Ƃ��̃{�X�̃o���b�g
				for (int BulletDataCount1 = 0; BulletDataCount1 < BOSS_BULLET_QUANTITY_1; BulletDataCount1++)
				{// BOSS_BULLET_QUANTITY_1��J��Ԃ�
					SetBossBullet(D3DXVECTOR3(boss.pos.x + BossBulletData1[BulletDataCount1].x,
						boss.pos.y + BossBulletData1[BulletDataCount1].y,
						boss.pos.z + BossBulletData1[BulletDataCount1].z),
						boss.move, BOSS_BULLET_SIZE_X, BOSS_BULLET_SIZE_Y, 100 * 3);
				}
			}
		}

		// HP��������艺�ɂȂ�����
		if (boss.hp <= BOSS_HP_MAX / 2)
		{
			for (int BulletHomingDataCount = 0; BulletHomingDataCount < 3; BulletHomingDataCount++)
			{
				// �{�X�p�̃z�[�~���O�e���Z�b�g
				SetBossHomingBullet(D3DXVECTOR3(boss.pos.x + BossHomingBulletData[BulletHomingDataCount].x,
					boss.pos.y + BossHomingBulletData[BulletHomingDataCount].y,
					boss.pos.z + BossHomingBulletData[BulletHomingDataCount].z), 10.0f, 10.0f);
			}
		}
		
		// HP��0�ȉ��ɂȂ�����
		if (boss.hp <= BOSS_DEAD)
		{
			
			// �����̔���
			SetExplosion(boss.pos, 2200, 2200, 0);
			
			// SE�̍Đ�
			PlaySound(SOUND_LABEL_SE_TRUE);
		
			// �{�X�𖢎g�p��
			boss.use = false;	
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX Scl, Rot, Translate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 Def;

	//�{�X��true�Ȃ�`�悷��
	if (boss.use == true)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&BossWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&Scl, boss.scl.x, boss.scl.y, boss.scl.z);
		D3DXMatrixMultiply(&BossWorld, &BossWorld, &Scl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&Rot, boss.rot.y, boss.rot.x, boss.rot.z);
		D3DXMatrixMultiply(&BossWorld, &BossWorld, &Rot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&Translate, boss.pos.x, boss.pos.y, boss.pos.z);
		D3DXMatrixMultiply(&BossWorld, &BossWorld, &Translate);


		
		// ���[���h�}�g���b�N�X�̐ݒ�
		Device->SetTransform(D3DTS_WORLD, &BossWorld);

		// ���݂̃}�e���A�����擾
		Device->GetMaterial(&Def);

		// �}�e���A�����ɑ΂���|�C���^���擾
		D3DXMat = (D3DXMATERIAL*)D3DXBuffMatBoss->GetBufferPointer();

		for (int CntMat = 0; CntMat < (int)NumMatBoss; CntMat++)
		{
			// �}�e���A���̐ݒ�
			Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureBoss);

			// �`��
			D3DXMeshBoss->DrawSubset(CntMat);
		}
		// �}�e���A�����f�t�H���g�ɖ߂�
		Device->SetMaterial(&Def);
	}
}

//=============================================================================
// �{�X���擾(GET)
//=============================================================================
Boss *GetBoss(void)
{
	return &boss;
}

//=============================================================================
// �{�X�̈ʒu���擾(GET)
//=============================================================================
D3DXVECTOR3	GetBossPos(void)
{
	return boss.pos;
}
//=============================================================================
// �{�X�̈ʒu���擾(GET)
//=============================================================================
D3DXVECTOR3	*GetPosBoss(void)
{
	return (&boss.pos);
}
//=============================================================================
// �{�X�̉�]���擾(GET)
//=============================================================================
D3DXVECTOR3	GetRotBoss(void)
{
	return(boss.rot);
}

//=============================================================================
// �{�X�̃��[���h�}�g���N�X���擾(GET)
//=============================================================================
D3DXMATRIX *GetBossWorldMatrix(void)
{
	return (&BossWorld);
}