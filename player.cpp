//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "wallleft.h"
#include "wallright.h"
#include "item.h"
#include "homingbullet.h"
#include "bossbullet.h"
#include "fade.h"
#include "score.h"
#include "timer.h"
#include "enemy.h"
#include "boss.h"
#include "spacebox.h"
#include "explosion.h"
#include "sound.h"
#include "collision.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTexturePlayer;		// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			MeshPlayer;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXMatBuffPlayer;		// ���b�V���̃}�e���A�������i�[
DWORD				NumMatPlayer;			// �������̑���

D3DXMATRIX			WorldPlayer;			// ���[���h�}�g���b�N�X
Player				player;					// �v���C���[���[�N

//*****************************************************************************
// 1st�X�e�[�W�p�̏������i�����o�[�֐��j
//*****************************************************************************
void Player::FirstBattlePlayerInitStatus()
{
	pos = D3DXVECTOR3(0.0f, 380.0f, -1000.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	RotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	radius = PLAYER_RADIUS;
	use = true;
	PlayerMove = 0;
	hp = PLAYER_LIFEMAX;
	mp = 0;
	Flame = 0;
}

//*****************************************************************************
// �{�X�X�e�[�W�p�̏������i�����o�[�֐��j
//*****************************************************************************
void Player::BossBattlePlayerInitStatus()
{
	pos = D3DXVECTOR3(0.0f, 1280.0f, -1000.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	RotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	radius = PLAYER_RADIUS;
	use = true;
	PlayerMove = 0;
	mp = 0;
	Flame = 0;
}

//*****************************************************************************
// �v���C���[�̎��S���o�i�����o�[�֐��j
//*****************************************************************************
void Player::PlayerDeadMove(float a,float b)
{
	// �����̐ݒ�
	SetExplosion(pos, a, b);

	//�@�v���C���[�̓����̃J�E���g��2��
	PlayerMove = 2;

	if (PlayerMove == 2)
	{
		// �ė����o�p�̉�]
		rot.y -= 0.2f;

		// �ė����o�p�̉��~
		move.y -= VALUE_MOVE_PLAYER;

		// �v���C���[��y���ʒu��10�ȉ��ɂȂ�����
		if (pos.y <= 10)
		{
			// �����̐ݒ�
			SetExplosion(pos, a, b);

			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_ENEMYEXPLOSION);

			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_FALSE);

			// ���g�p��
			use = false;

			// �t�F�[�h�A�E�g
			SetFade(FADE_OUT);
		}
	}
}

//*****************************************************************************
// �v���C���[�̏������o�i�����o�[�֐��j
//*****************************************************************************
void Player::PlayerWinMove(int z)
{
	// �v���C���[�̓����̃J�E���g��3��
	PlayerMove = 3;

	if (PlayerMove == 3)
	{
		// �����������l�ɖ߂�
		rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

		// z�������ֈړ�
		move.z += VALUE_MOVE_PLAYER;

		if (pos.z >= z)
		{
			// �t�F�[�h�A�E�g
			SetFade(FADE_OUT);
		}
	}
}

//*****************************************************************************
// �v���C���[�̈ړ������i�����o�[�֐��j
//*****************************************************************************
void Player::PlayerMoveLimit()
{
	//MODE_GAME�̎��̈ړ��͈�
	if (GetMode() == MODE_GAME)
	{
		if (PlayerMove == 1)
		{
			// �����̈ړ��͈�
			if (pos.y > 590)
			{
				pos.y = 590;
			}
			if (pos.y <= 30)
			{
				pos.y = 30;
			}

			// x���̈ړ��͈�
			if (pos.x > WALLRIGNT_VTX_X - 30)
			{
				pos.x = WALLRIGNT_VTX_X - 30;
			}
			if (pos.x <= -WALLLEFE_VTX_X + 30)
			{
				pos.x = -WALLLEFE_VTX_X + 30;
			}

			// z���̈ړ��͈�
			if (pos.z > 3000)
			{
				pos.z = 3000;
			}
			if (pos.z <= 80)
			{
				pos.z = 80;
			}
		}
	}

	// �{�X��ł̈ړ��͈�
	else if (GetMode() == MODE_BOSS)
	{
		if (PlayerMove == 1)
		{
			// �����̈ړ��͈�
			if (pos.y > 2600)
			{
				pos.y = 2600;
			}
			if (pos.y <= 230)
			{
				pos.y = 230;
			}

			// x���̈ړ��͈�
			if (pos.x > 1200)
			{
				pos.x = 1200;
			}
			if (pos.x <= -1200)
			{
				pos.x = -1200;
			}

			// z���̈ړ��͈�
			if (pos.z > 0)
			{
				pos.z = 0;
			}
			if (pos.z <= -0)
			{
				pos.z = -0;
			}
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DTexturePlayer = NULL;
	MeshPlayer = NULL;
	D3DXMatBuffPlayer = NULL;

	// �m�[�}�����[�h�̎�
	if (GetMode() == MODE_GAME)
	{
		// 1st�X�e�[�W�p�̏������p�����o�֐��Ăяo��
		player.FirstBattlePlayerInitStatus();
	}
	// �{�X���[�h�̎�
	else if (GetMode() == MODE_BOSS)
	{
		// �{�X�X�e�[�W�p�̏������p�����o�֐��Ăяo��
		player.BossBattlePlayerInitStatus();
	}

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER,
		D3DXMESH_SYSTEMMEM,
		Device,
		NULL,
		&D3DXMatBuffPlayer,
		NULL,
		&NumMatPlayer,
		&MeshPlayer)))
	{
		return E_FAIL;
	}

#if 1
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,		// �f�o�C�X�ւ̃|�C���^
		PLAER_TEXTURE_FILENAME,				// �t�@�C���̖��O
		&D3DTexturePlayer);				// �ǂݍ��ރ������[
#endif

	// �e�̐ݒ�
	player.IdxShadow = SetShadow(player.pos, player.radius * 2.0f, player.radius * 2.0f, player.radius * 2.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if (D3DTexturePlayer != NULL)
	{// �e�N�X�`���̊J��
		D3DTexturePlayer->Release();
		D3DTexturePlayer = NULL;
	}

	if (MeshPlayer != NULL)
	{// ���b�V���̊J��
		MeshPlayer->Release();
		MeshPlayer = NULL;
	}

	if (D3DXMatBuffPlayer != NULL)
	{// �}�e���A���̊J��
		D3DXMatBuffPlayer->Release();
		D3DXMatBuffPlayer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	//�J�����̌����̎擾
	D3DXVECTOR3 RotCamera;

	Camera *Cwk;
	Cwk = GetCamerawk();

	RotCamera = GetRotCamera();


	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();

	// �A�C�e���̎擾
	Item *item;
	item = GetItem();
	
	if (player.use == true)
	{
		if (player.PlayerMove == 0)
		{
			player.move.z = 5.0f;

			if (player.pos.z > 0.0f)
			{
				player.PlayerMove = 1;
			}

			else if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_TOUCH))
			{
				player.pos.z = 0.0f;

				player.PlayerMove = 1;
			}
		}

		// �v���C���[�̓�����͈͂̐���
		player.PlayerMoveLimit();
		
		// �v���C���[�̈ړ�����
		if (player.PlayerMove == 1)
		{
			player.RotDest.y = D3DX_PI + RotCamera.y;

			if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
			{
				if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
				{// ���O�ړ�
					player.move.x -= sinf(RotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

				}
				else
				{// ���ړ�
					player.move.x -= sinf(RotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

					player.rot.z -= 0.01f;

					if (player.rot.z <= -0.3f)
					{
						player.rot.z = -0.3f;
					}
				}
			}

			else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
			{
				{// �E�ړ�
					player.move.x -= sinf(RotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
					player.move.z -= cosf(RotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

					player.rot.z += 0.01f;

					if (player.rot.z >= 0.3f)
					{
						player.rot.z = 0.3f;
					}
				}
			}
			else
			{
				// �Ⴄ�ꍇ�A�����̉�]��0�ɖ߂�
				player.rot.z = 0.0f;
			}

			if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
			{// �㏸
				player.rot.x += 0.009f;
				player.move.y += VALUE_MOVE_PLAYER;
				if (player.rot.x >= 0.3f)
				{
					player.rot.x = +0.3f;
				}
			}

			else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
			{// ���~
				player.rot.x -= 0.002f;
				player.move.y -= VALUE_MOVE_PLAYER;
				if (player.rot.x <= -0.3f)
				{
					player.rot.x = -0.3f;
				}
			}
			else
			{// �Ⴄ�ꍇ�Ax���̉�]��0�ɖ߂�
				player.rot.x = 0.0f;
			}

			// �e����
			if (GetKeyboardPress(DIK_SPACE) || IsButtonPressed(0, BUTTON_R))
			{
				// ���ˊ��o�̒���
				player.Flame++;
				player.Flame %= PLAYER_BULLET_FLAME;

				if (player.Flame == 0 && (GetKeyboardPress(DIK_SPACE) || IsButtonPressed(0, BUTTON_R)))
				{
					D3DXVECTOR3 pos;
					D3DXVECTOR3 move;

					pos.x = player.pos.x - sinf(player.rot.y) * 10.0f;
					pos.y = player.pos.y;
					pos.z = player.pos.z - cosf(player.rot.y) * 20.0f;

					move.x = -sinf(player.rot.y) * VALUE_MOVE_PLAYER_BULLET;
					move.y = 0.0f;
					move.z = -cosf(player.rot.y) * VALUE_MOVE_PLAYER_BULLET;

					// �o���b�g�̃Z�b�g
					SetBullet(pos, move, 14.0f, 14.0f, 100);

					// SE�Đ�
					PlaySound(SOUND_LABEL_SE_SHOT);
				}

			}
			// ���b�N�I�������Ƃ��̂݃z�[�~���O�����Ă�
			if (Cwk->LockOnTrigger == true)
			{
				if (GetPlayer()->mp >= 100)
				{
					// �L�[�{�[�h�Ȃ�H�L�[�A�Q�[���p�b�h�Ȃ�R1����������
					if (GetKeyboardPress(DIK_H) || IsButtonPressed(0, BUTTON_R1))
					{
						// �z�[�~���O�e�̃Z�b�g
						SetHomingBullet(player.pos, 0.01f, 0.01f);
					}
				}
			}
		}
		// �ʒu�ړ�
		player.pos.x += player.move.x;
		player.pos.y += player.move.y;
		player.pos.z += player.move.z;

		// �ړ��ʂɊ�����������
		player.move.x += (0.0f - player.move.x) * RATE_MOVE_PLAYER;
		player.move.y += (0.0f - player.move.y) * RATE_MOVE_PLAYER;
		player.move.z += (0.0f - player.move.z) * RATE_MOVE_PLAYER;

		float colA = (200.0f - (player.pos.y - 10.0f)) / 400.0f;
		if (colA < 0.0f)
		{
			colA = 0.0f;
		}
		SetColorShadow(player.IdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));

		// �e�̈ʒu�ݒ�
		if (player.pos.x >= WALLRIGNT_VTX_X / 2)
		{
			SetPositionShadow(player.IdxShadow, D3DXVECTOR3(WALLRIGNT_VTX_X - 1, player.pos.y, player.pos.z));
			SetVertexShadow(player.IdxShadow, 0, 30.0f, 30.0f, 30.0f);
		}
		else if (player.pos.x <= -WALLLEFE_VTX_X / 2)
		{
			SetPositionShadow(player.IdxShadow, D3DXVECTOR3(-WALLLEFE_VTX_X + 1, player.pos.y, player.pos.z));
			SetVertexShadow(player.IdxShadow, 1, 30.0f, 30.0f, 30.0f);
		}


		// �G�t�F�N�g�̐ݒ�
		SetEffect(D3DXVECTOR3(player.pos.x, player.pos.y, player.pos.z - 14), D3DXVECTOR3(0.0f, 0.0f, -5.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 1.2f), 20.0f, 20.0f, 5);
		SetEffect(D3DXVECTOR3(player.pos.x, player.pos.y, player.pos.z - 14), D3DXVECTOR3(0.0f, 0.0f, -6.0f),
			D3DXCOLOR(0.65f, 0.85f, 0.05f, 1.30f), 10.0f, 10.0f, 5);

		// �s�k���o
		if (player.hp <= PLAYER_DEAD)
		{
			player.PlayerDeadMove(50,50);
		}

		// �������o
		else if (GetMode() == MODE_BOSS && boss->use == false && boss->hp <= PLAYER_DEAD)
		{
			player.PlayerWinMove(2000);
		}

		// ���ۂ̈ړ�
		player.pos += player.move;

		// �A�C�e���i�@���j�Ƃ̓����蔻��
		for (int CntItem = 0; CntItem < MAX_ITEM; CntItem++, item++)
		{
			if (item->use == true)
			{
				// �v���C���[�Ƃ̓����蔻��
				if (AttackCheak(player.pos, item->pos,
					player.radius, item->radius))
				{
					SetExplosion(item->pos, 40.0f, 40.0f);

					// �A�C�e������
					DeleteItem(CntItem);

					// HP�̌���
					player.hp -= ITEM_DAMAGE;

					// SE�Đ�
					PlaySound(SOUND_LABEL_SE_ENEMYEXPLOSION);

					break;
				}
			}
		}

		// �G�l�~�[�Ƃ̓����蔻��
		Enemy *enemy;

		for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++, enemy++)
		{
			enemy = GetEnemy(CntEnemy);

			if (enemy->use == true)
			{
				// �v���C���[�Ƃ̓����蔻��
				if (AttackCheak(player.pos, enemy->pos,
					player.radius, enemy->radius))
				{
					SetExplosion(enemy->pos, 40.0f, 40.0f);

					player.hp -= ENEMY_DAMAGE;
					// �A�C�e������
					DeleteEnemy(CntEnemy);

					// SE�Đ�
					PlaySound(SOUND_LABEL_SE_ENEMYEXPLOSION);

					break;
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	if (player.use == true)
	{
		LPDIRECT3DDEVICE9 Device = GetDevice();
		D3DXMATRIX MtxRot, MtxTranslate;
		D3DXMATERIAL *D3DXMat;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&WorldPlayer);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&MtxRot, player.rot.y, player.rot.x, player.rot.z);
		D3DXMatrixMultiply(&WorldPlayer, &WorldPlayer, &MtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&MtxTranslate, player.pos.x, player.pos.y, player.pos.z);
		D3DXMatrixMultiply(&WorldPlayer, &WorldPlayer, &MtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		Device->SetTransform(D3DTS_WORLD, &WorldPlayer);

		// �}�e���A�����ɑ΂���|�C���^���擾
		D3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer->GetBufferPointer();

		for (int CntMat = 0; CntMat < (int)NumMatPlayer; CntMat++)
		{
			// �}�e���A���̐ݒ�
			Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTexturePlayer);

			// �`��
			MeshPlayer->DrawSubset(CntMat);
		}

		{// �}�e���A�����f�t�H���g�ɖ߂�
			D3DXMATERIAL Mat;

			Mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			Mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			Mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			Device->SetMaterial(&Mat.MatD3D);
		}
	}
}

//=============================================================================
// �v���C���[���擾
//=============================================================================
Player *GetPlayer(void)
{
	return &player;
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return player.pos;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return player.rot;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return player.RotDest;
}

//=============================================================================
// �ړ��ʎ擾
//=============================================================================
D3DXVECTOR3 GetMovePayer(void)
{
	return player.move;
}