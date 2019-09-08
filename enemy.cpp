//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : �����C�O
//
//=============================================================================
#include "enemy.h"
#include "shadow.h"
#include "effect.h"
#include "boss.h"
#include "timer.h"
#include "player.h"
#include "ace.h"
#include "fade.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureEnemy[ENEMYTYPE_MAX];	// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			MeshEnemy[ENEMYTYPE_MAX];		// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXMatBuffEnemy[ENEMYTYPE_MAX];// ���b�V���̃}�e���A�������i�[
DWORD				NumMatEnemy[ENEMYTYPE_MAX];		// �������̑���

static D3DXMATRIX	MtxWorldEnemy;					// ���[���h�}�g���b�N�X

Enemy				enemy[MAX_ENEMY];				

const char *FileNameEnemy[ENEMYTYPE_MAX] =
{
	"data/MODEL/UFO(big).x",		// UFO
	"data/MODEL/�~�T�C��.x",		// �~�T�C��
	"data/MODEL/UFO(big).x",
};

//*****************************************************************************
// �G�l�~�[�̏������p�i�����o�֐��j
//*****************************************************************************
void Enemy::EnemyInit()
{
	use = true;								// use�̔��� 
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]
	scl = D3DXVECTOR3(2.0f, 2.0f, 2.0f);	// �T�C�Y
	radius = 2.0f;							// ���a
	IdxShadow = -1;							// �eID
	Type = ENEMYTYPE_ENEMY;					// �G�l�~�[�̃��f��

	// �G�l�~�[�̏����z�u
	float PosX, PosY, PosZ;

	PosX = (float)(rand() % 500 - 200);
	PosY = (float)(rand() % 600);
	PosZ = (float)(rand() % 300000) + 7300;
	pos = D3DXVECTOR3(PosX, PosY, PosZ);	// �ʒu
}


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	for (int CntEnemyType = 0; CntEnemyType < ENEMYTYPE_MAX; CntEnemyType++)
	{
		// �e�N�X�`���̏�����
		D3DTextureEnemy[CntEnemyType] = NULL;

		// ���b�V���̏�����
		MeshEnemy[CntEnemyType] = NULL;

		// �}�e���A���̏�����
		D3DXMatBuffEnemy[CntEnemyType] = NULL;


		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(FileNameEnemy[CntEnemyType],
			D3DXMESH_SYSTEMMEM,
			Device,
			NULL,
			&D3DXMatBuffEnemy[CntEnemyType],
			NULL,
			&NumMatEnemy[CntEnemyType],
			&MeshEnemy[CntEnemyType])))
		{
			return E_FAIL;
		}

#if 1
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(Device,		// �f�o�C�X�ւ̃|�C���^
			ENEMY_TEXTURE_FILENAME,				// �t�@�C���̖��O
			&D3DTextureEnemy[CntEnemyType]);	// �ǂݍ��ރ������[
#endif
	}

	//�G�l�~�[�̐ݒ�
	for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++)
	{
		// �G�l�~�[�̏���������
		enemy[CntEnemy].EnemyInit();
	}
	
	return S_OK;
}

//=============================================================================
// �G�l�~�[�̏I������
//=============================================================================
void UninitEnemy(void)
{
	for (int CntEnemyType = 0; CntEnemyType < ENEMYTYPE_MAX;CntEnemyType++)
	{
		if (D3DTextureEnemy[CntEnemyType] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureEnemy[CntEnemyType]->Release();
			D3DTextureEnemy[CntEnemyType] = NULL;
		}

		if (MeshEnemy[CntEnemyType] != NULL)
		{// ���b�V���̊J��
			MeshEnemy[CntEnemyType]->Release();
			MeshEnemy[CntEnemyType] = NULL;
		}

		if (D3DXMatBuffEnemy[CntEnemyType] != NULL)
		{// �}�e���A���̊J��
			D3DXMatBuffEnemy[CntEnemyType]->Release();
			D3DXMatBuffEnemy[CntEnemyType] = NULL;
		}
	}
}

//=============================================================================
// �G�l�~�[�̍X�V����
//=============================================================================
void UpdateEnemy(void)
{
	// ace�o���܂ł̃J�E���g
	int acecount=20;

	// �X�s�[�h�̐ݒ�
	float Speed = 2.0f;

	D3DXVECTOR3 Sabun;

	// �v���C���[�̎擾
	Player *player;
	player = GetPlayer();

	// �G�[�X�̎擾
	Ace*ace;
	ace = GetAce();
	
	for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++)
	{
		// 4�Ŋ���؂��z��̎��z�[�~���O
		if (CntEnemy % 4 == 0)
		{
			if (ace->use == false)
			{
				enemy[CntEnemy].rot.y = 3.17f;
				enemy[CntEnemy].Type = ENEMYTYPE_ENEMY2;
				enemy[CntEnemy].scl = D3DXVECTOR3(4.0f, 4.0f, 2.0f);
				//�z�[�~���O�p�̏���
				Sabun = (player->pos - enemy[CntEnemy].pos);
				D3DXVec3Normalize(&Sabun, &Sabun);

				// ����
				FLOAT Kakudo = D3DXVec3Dot(&Sabun, &enemy[CntEnemy].pos);
				Kakudo = acosf(Kakudo);

			
				enemy[CntEnemy].pos.z -= 8.0f;
				
				Speed++;
				enemy[CntEnemy].pos += (Sabun)*(Speed);
			}

			else
			{
				enemy[CntEnemy].use = false;
			}
		}

		// 3�Ŋ���؂��Ȃ�
		else if (CntEnemy % 3 == 0)
		{
			enemy[CntEnemy].rot.y += 2.0f;
			enemy[CntEnemy].pos.z -= 5.0f;
		}

		// ���ׂĂ̏����ɓ��Ă͂܂�Ȃ��̂Ȃ�
		else
		{
			enemy[CntEnemy].rot.y += 2.0f;
			enemy[CntEnemy].pos.z -= 20.0f;
		}

		if (enemy[CntEnemy].pos.z < -1 || ace->hp <= 0)
		{
			enemy[CntEnemy].use=false;

			acecount--;
			// �J�E���g��0�ɂȂ�����
			if (acecount == 0)
			{
				// ACE���o��������
				ace->use = true;
			}
		}
	}
	
}
//=============================================================================
// �G�l�~�[�̕`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxScl, MtxRot, MtxTranslate;

	// �G�[�X�̎擾
	Ace*ace;
	ace = GetAce();

	for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++)
	{
		if (enemy[CntEnemy].use == true && (GetPositionPlayer().z - enemy[CntEnemy].pos.z > -4000))
		{//�G�l�~�[��true�Ȃ�

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&MtxWorldEnemy);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&MtxScl, enemy[CntEnemy].scl.x, enemy[CntEnemy].scl.y, enemy[CntEnemy].scl.z);
			D3DXMatrixMultiply(&MtxWorldEnemy, &MtxWorldEnemy, &MtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&MtxRot, enemy[CntEnemy].rot.y, enemy[CntEnemy].rot.x, enemy[CntEnemy].rot.z);
			D3DXMatrixMultiply(&MtxWorldEnemy, &MtxWorldEnemy, &MtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&MtxTranslate, enemy[CntEnemy].pos.x, enemy[CntEnemy].pos.y, enemy[CntEnemy].pos.z);
			D3DXMatrixMultiply(&MtxWorldEnemy, &MtxWorldEnemy, &MtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			Device->SetTransform(D3DTS_WORLD, &MtxWorldEnemy);

			D3DXMATERIAL *D3DXMat = (D3DXMATERIAL*)D3DXMatBuffEnemy[enemy[CntEnemy].Type]->GetBufferPointer();

			for (int CntMat = 0; CntMat < (int)NumMatEnemy[enemy[CntEnemy].Type]; CntMat++)
			{
				// �}�e���A���̐ݒ�
				Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				Device->SetTexture(0, D3DTextureEnemy[enemy[CntEnemy].Type]);

				// �`��
				MeshEnemy[enemy[CntEnemy].Type]->DrawSubset(CntMat);
			}
		}

	}

	{// �}�e���A�����f�t�H���g�ɖ߂�
		D3DXMATERIAL Mat;

		Mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
		Mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		Mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		Device->SetMaterial(&Mat.MatD3D);
	}

}

//=============================================================================
// �G�l�~�[�̐ݒ�
//=============================================================================
void SetEnemy(D3DXVECTOR3 disp, D3DXVECTOR3 Rot, int Type)
{
	for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++)
	{
		if (!enemy[CntEnemy].use)
		{
			enemy[CntEnemy].start = disp;
			enemy[CntEnemy].c1 = D3DXVECTOR3((float)(rand() % 400 - 350 / 1.0f), (float)(rand() % 750 - 600) / 1.0f, (float)(rand() % 20250) / 1.0f) + disp;
			enemy[CntEnemy].c2 = D3DXVECTOR3((float)(rand() % 200 - 150 / 1.0f), (float)(rand() % 450 - 100) / 1.0f, (float)(rand() % 10250) / 1.0f) + disp;
			enemy[CntEnemy].rot = Rot;
			enemy[CntEnemy].radius = ENEMY_RADIUS;
			enemy[CntEnemy].Type = Type;
			enemy[CntEnemy].use = true;
	
			break;
		}
	}
}

//=============================================================================
// �G�l�~�[�̍폜
//=============================================================================
void DeleteEnemy(int IdxEnemy)
{
	if (IdxEnemy >= 0 && IdxEnemy < MAX_ENEMY)
	{
		enemy[IdxEnemy].use = false;
	}
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
Enemy *GetEnemy(int IdxEnemy)
{
	return &enemy[IdxEnemy];
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionEnemy(int IdxEnemy)
{
	return enemy[IdxEnemy].pos;
}

//=============================================================================
// �傫���擾
//=============================================================================
D3DXVECTOR3 GetSclEnemy(int IdxEnemy)
{
	return enemy[IdxEnemy].scl;
}