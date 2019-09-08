//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "game.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "meshfield.h"
#include "player.h"
#include "boss.h"
#include "bossbullet.h"
#include "field.h"
#include "bossgauge.h"
#include "fieldup.h"
#include "enemy.h"
#include "wallleft.h"
#include "wallright.h"
#include "bosshomingbullet.h"
#include "bossgauge.h"
#include "gauge.h"
#include "homingbullet.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "billboard.h"
#include "explosion.h"
#include "ace.h"
#include "timer.h"
#include "pause.h"
#include "score.h"
#include "item.h"
#include "centerlock.h" 
#include "spacebox.h"
#include "sound.h"
#include "acehomingbullet.h"
#include "ui.h"
#include "mpgauge.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
bool StopTrigger ;		// �|�[�Y�p�̃g���K�[

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	// �|�[�Y�g���K�[�̐ݒ�
	StopTrigger = false;

	// ���C�g�̏�����
	InitLight();

	// �J�����̏�����
	InitCamera();

	// �t�B�[���h�̏�����
	InitField();

	// ���̕ǂ̏�����
	InitWallleft();

	// �E�̕ǂ̏�����
	InitWallright();

	// �V��̕ǂ̏�����
	InitFieldup();

	// �����̃J�[�\���̏�����
	InitCenterLock();

	// �|�[�Y��ʂ̏�����
	InitPause();

	// �e�̏�����
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	// ���b�N�I���̏�����
	InitBillboard();

	// �{�X�̏�����
	InitAce();

	// �e�̏�����
	InitBullet();

	// �z�[�~���O�e�̏�����
	InitHomingBullet();

	// �z�[�~���O�e�̏�����
	InitAceHomingBullet();

	// �����̏�����
	InitExplosion();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// ���C�t�̏�����
	InitGauge();

	// MP�Q�[�W�̏���������
	InitMpGauge();

	// �^�C�}�[�̏�����
	InitTimer();
	ResetTimer(60);

	// �X�R�A�̏�����
	InitScore();

	// skybox�̏�����
	InitSpace();

	// UI�̏�����
	InitUi();

	// �A�C�e���̏�����
	InitItem();

	for(int CntCoin = 0; CntCoin < 600000; CntCoin++)
	{
		float fPosX, fPosY, fPosZ;

		fPosX = (float)(rand() % 12000) / 10.0f - 600.0f;
		fPosY = (float)(rand()%600)/1.0f;
		fPosZ = (float)(rand() % 20000)+ ITEM_FIRST_POS;
		SetItem(D3DXVECTOR3(fPosX, fPosY, fPosZ),  D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_COIN); 
	}

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM000);

	return S_OK;
}

//=============================================================================
// �{�X��p�̏���������
//=============================================================================
HRESULT InitBossGame(void)
{
	// �J�����̏�����
	InitCamera();

	// �n�ʂ̏�����
	InitMeshField(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 16, 2800.0f, 2800.0f);

	// �v���C���[�̏�����
	InitPlayer();

	// �{�X�̏�����
	InitBoss();

	// �z�[�~���O�o���b�g�̏�����
	InitHomingBullet();

	// �{�X�̃Q�[�W�̏�����
	InitBossGauge();

	// �{�X�o���b�g�̏�����
	InitBossBullet();

	// �{�X�̃z�[�~���O�̏�����
	InitBossHomingBullet();

	// �o���b�g�̏�����
	InitBullet();

	// �����̏�����
	InitExplosion();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �^�C�}�[�̏�����
	InitTimer();
	ResetTimer(90);

	// UI�̏�����
	InitUi();

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM001);

	return S_OK;
}

//=============================================================================
// �G�l�~�[��̏I������
//=============================================================================
void UninitGame(void)
{
	// �t�B�[���h�̏I������
	UninitField();

	// �����̏I������
	UninitExplosion();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// ���̕ǂ̏I������
	UninitWallleft();

	// �E�̕ǂ̏I������
	UninitWallright();

	// �V��̏I������
	UninitFieldup();

	// �v���C���[�̏I������
	UninitPlayer();

	// �^�C���̏I������
	UninitTimer();

	// �G�t�F�N�g�̏I������
	UninitBullet();

	// �A�C�e���̏I������
	UninitHomingBullet();

	// �A�C�e���̏I������
	UninitAceHomingBullet();

	// �A�C�e���̏I������
	UninitItem();

	// �G�[�X�̏I������
	UninitAce();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// ACE�̏I������
	UninitAce();

	// UI�̏I������
	UninitUi();

	// BGM��~
	StopSound(SOUND_LABEL_BGM000);
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossGame(void)
{
	// ���C�g�̏I������
	UninitLight();

	// �J�����̏I������
	UninitCamera();

	// �n�ʂ̏I������
	UninitMeshField();

	// �e�̏I������
	UninitShadow();

	// �v���C���[�̏I������
	UninitPlayer();

	// �{�X�̏I������
	UninitBoss();

	// �r���{�[�h�̏I������
	UninitBillboard();

	// �e�̏I������
	UninitBullet();

	// �z�[�~���O�e�̏�����
	UninitHomingBullet();

	// �{�X�o���b�g�̏�����
	UninitBossBullet();

	// �{�X�̃Q�[�W�̏I������
	UninitBossGauge();

	// �{�X�̃z�[�~���O�̏I������
	UninitBossHomingBullet();

	// �����̏I������
	UninitExplosion();

	// �G�t�F�g�̏I������
	UninitEffect();

	// ���C�t�̏I������
	UninitGauge();

	// Mp�Q�[�W�̏I������
	UninitMpGauge();

	// �^�C�}�[�̏I������
	UninitTimer();

	// �X�R�A�̏I������
	UninitScore();

	// spacebox�̏I������
	UninitSpace();

	// �|�[�Y��ʂ̏I������
	UninitPause();

	// �����̃��b�N�I���̏I������
	UninitCenterLock();

	// UI�̏I������
	UninitUi();

	// BGM��~
	StopSound(SOUND_LABEL_BGM001);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	// �|�[�Y�̍X�V
	UpdatePause();

	if (StopTrigger == true)
	{
		if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_OPTIONS))
		{
			StopTrigger = false;

			// �L�����Z�����̍Đ�
			PlaySound(SOUND_LABEL_SE_CANCEL);
		}

		else if (GetKeyboardTrigger(DIK_L))
		{
			// �I������
			UninitGame();
			UninitBossGame();

			// �^�C�g����
			SetMode(MODE_TITLE);
			
		}

	}
	else if ((GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_OPTIONS)) && StopTrigger == false)
	{
		// ��~�̃g���K�[���g�p��
		StopTrigger = true;
	}
	
	// ��~�̃g���K�[�����g�p�Ȃ�
	if (StopTrigger == false)
	{
		// �J�����X�V
		UpdateCamera(GetAcePos(), GetPositionPlayer(), CAMERA_LOCK);

		// �t�B�[���h�̍X�V
		UpdateField();

		// ���̕ǂ̍X�V
		UpdateWallleft();

		// �E�̕ǂ̍X�V
		UpdateWallright();

		// �V��̍X�V
		UpdateFieldup();

		// �e�����̍X�V
		UpdateShadow();

		// �G�t�F�N�g�����̍X�V
		UpdateEffect();

		// �v���C���[�����̍X�V
		UpdatePlayer();

		// �G�l�~�[�̍X�V
		UpdateEnemy();

		// �r���{�[�h�̍X�V
		UpdateBillboard();

		// �{�X�����̍X�V
		UpdateAce();

		// �e�����̍X�V
		UpdateBullet();

		// �z�[�~���O�e�̍X�V
		UpdateHomingBullet();

		// �z�[�~���O�e�̍X�V
		UpdateAceHomingBullet();

		// ���������̍X�V
		UpdateExplosion();

		if (GetPositionPlayer().z > 0.0f)
		{
			// ���C�t�����̍X�V
			UpdateGauge();

			// Mp�Q�[�W�̍X�V����
			UpdateMpGauge();

			// �^�C�}�[�����̍X�V
			UpdateTimer();

			// �X�R�A�����̍X�V
			UpdateScore();
		}
		// �A�C�e�������̍X�V
		UpdateItem();

		// �����̃��b�N�I���J�[�\���̍X�V
		UpdateCenterLock();

		// UI�̍X�V
		UpdateUi();
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossGame(void)
{
	// �|�[�Y�̍X�V
	UpdatePause();

	if (StopTrigger == true)
	{
		if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_OPTIONS))
		{
			StopTrigger = false;
			
			// �L�����Z�����̍Đ�
			PlaySound(SOUND_LABEL_SE_CANCEL);
		}

		else if (GetKeyboardTrigger(DIK_L))
		{
			// �I������
			UninitGame();

			// Boss�X�e�[�W�̏I������
			UninitBossGame();

			// �^�C�g����
			SetMode(MODE_TITLE);

		}
	}
	else if ((GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_OPTIONS)) && StopTrigger == false)
	{
		// �X�g�b�v�g���K�[���g�p��
		StopTrigger = true;
	}
	
	// �X�g�b�v�g���K�[�����g�p�Ȃ�
	if (StopTrigger == false)
	{
		// �J�����X�V
		UpdateCamera( GetBossPos(),GetPositionPlayer(),  CAMERA_LOCK);

		// Skybox_Sace�̍X�V
		UpdateSpace();

		// �n�ʏ����̍X�V
		UpdateMeshField();

		// �e�����̍X�V
		UpdateShadow();

		// �v���C���[�����̍X�V
		UpdatePlayer();

		// �������b�N�I���J�[�\���̍X�V
		UpdateCenterLock();

		// boss�����̍X�V
		UpdateBoss();

		// �G�t�F�N�g�����̍X�V
		UpdateEffect();

		// �{�X�o���b�g�̍X�V
		UpdateBossBullet();

		// �{�X�̃z�[�~���O�̍X�V
		UpdateBossHomingBullet();

		// ���b�N�I���̍X�V
		UpdateBillboard();

		// �e�����̍X�V
		UpdateBullet();

		// �z�[�~���O�e�̍X�V
		UpdateHomingBullet();

		// ���������̍X�V
		UpdateExplosion();

		// ���C�t�����̍X�V
		UpdateGauge();

		// �^�C�}�[�����̍X�V
		UpdateTimer();

		// �X�R�A�����̍X�V
		UpdateScore();

		// �A�C�e�������̍X�V
		UpdateItem();

		// Mp�Q�[�W�̍X�V����
		UpdateMpGauge();

		// �{�X�̃Q�[�W�̍X�V
		UpdateBossGauge();

		// UI�̍X�V
		UpdateUi();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	// �J�����̐ݒ�
	DrawCamera();

	// spacebox
	DrawSpace();

	// �{�X�����̕`��
	DrawAce();

	// �v���C���[�����̕`��
	DrawPlayer();

	// �G�l�~�[�̕`��
	DrawEnemy();

	// �A�C�e�������̕`��
	DrawItem();

	// �e�����̕`��
	DrawBullet();

	// �z�[�~���O�e�̕`��
	DrawHomingBullet();

	// �z�[�~���O�e�̕`��
	DrawAceHomingBullet();

	// �E�̕ǂ̕`�揈��
	DrawWallright();

	// ���̕ǂ̕`�揈��
	DrawWallleft();

	// �V��̕`�揈��
	DrawFieldup();

	// �t�B�[���h�̕`�揈��
	DrawField();

	// ���������̕`��
	DrawExplosion();

	// �G�t�F�N()+�g�����̕`��
	DrawEffect();

	if (GetPositionPlayer().z >= 100.0f)
	{
		// �r���{�[�h�̕`��
		DrawBillboard();
	}

	if (GetPositionPlayer().z > 0.0f)
	{
		if (StopTrigger == false)
		{
			// �����̃��b�N�I���̕`��
			DrawCenterLock();

			// ���C�t�����̕`��
			DrawGauge();

			// Mp�Q�[�W�̕`�揈��
			DrawMpGauge();

			// �^�C�}�[�����̕`��
			DrawTimer();

			// �X�R�A�����̕`��
			DrawScore();

			// UI�̕`��
			DrawUi();
		}
	}

	// �e�����̕`��
	DrawShadow();


	if (StopTrigger == true)
	{
		// �|�[�Y��ʂ̕`��
		DrawPause();
	}
}

void DrawBossGame(void)
{
	// �J�����̐ݒ�
	DrawCamera();

	// spacebox�̕`��
	DrawSpace();

	// �n�ʏ����̕`��
	DrawMeshField();

	// boss�����̕`��
	DrawBoss();

	// �v���C���[�����̕`��
	DrawPlayer();

	// ���������̕`��
	DrawExplosion();

	if (GetPositionPlayer().z >= 100.0f)
	{
		// �r���{�[�h�̕`��
		DrawBillboard();
	}

	// �{�X�̒e�̕`��
	DrawBossBullet();

	// �{�X�̃z�[�~���O�̕`��
	DrawBossHomingBullet();

	// �G�t�F�N()+�g�����̕`��
	DrawEffect();

	if (StopTrigger == false)
	{
		// ���C�t�����̕`��
		DrawGauge();

		// �^�C�}�[�����̕`��
		DrawTimer();

		// �X�R�A�����̕`��
		DrawScore();

		// �{�X�̃Q�[�W�̕`��
		DrawBossGauge();

		// Mp�Q�[�W�̕`�揈��
		DrawMpGauge();

		if (GetPositionPlayer().z > 0.0f)
		{
			// �����̃J�[�\���̕`��
			DrawCenterLock();
		}
	}

	// UI�̕`��
	DrawUi();

	if (StopTrigger == true)
	{
		// �|�[�Y�̕`��
		DrawPause();
	}
}
