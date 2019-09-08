//=============================================================================
//
// ���C������ [camera.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "ace.h"
#include "boss.h"
#include "sound.h"
#include "enemy.h"
#include "main.h"

//=============================================================================
//�O���[�o���ϐ�
//=============================================================================
Camera		Cwk[2];

//=============================================================================
// �J�������_�ړ���z�������i�����o�[�֐�
//=============================================================================
void Camera::CameraAriaLimit(float z1,float z2)
{
	// �J�����̈ړ�����
	if (GetMode() == MODE_GAME)
	{
		// �P�����X�e�[�W��z���͈̔�
		if (PosCameraP.z <= z1)
		{
			PosCameraP.z = z1;
		}
	}
	else if (GetPlayer()->PlayerMove== 3)
	{
		// BOSS�X�e�[�W��z���͈̔�
		if (Cwk->PosCameraP.z >= z2)
		{
			Cwk->PosCameraP.z = z2;
		}
	}
}

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Cwk->PosCameraP = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Cwk->PosCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Cwk->PosCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �J�������_�̉�]�p�x������
	Cwk->RotCameraP = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���b�N�I���g���K�[�̐ݒ�
	Cwk->LockOnTrigger=false;

}
//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}
//=============================================================================
// �J�������_�ړ�����
//=============================================================================

// �J�����̈ʒu = �J�����̒����_ + �����_����̑��ΓI�ʒu
// �J�����̈ʒu = �J�����̒����_ + (�����_����̊p�x * ���_�܂ł̋���)

void CalcPosCameraP(D3DXVECTOR3 *at)
{
	D3DXVECTOR3 Vec(0.0f, 0.25f, -1.0f);		// ���K�����ꂽ�����_����̌���
	D3DXMATRIX	Mtx;							// �}�g���N�X		

	Cwk->PosCameraR = *at;

	// �J�����̑��ΓI�ȉ�]�p�x���s��ɕϊ�����
	D3DXMatrixRotationYawPitchRoll(&Mtx, Cwk->RotCameraP.y, Cwk->RotCameraP.x, Cwk->RotCameraP.z);

	// ���K�����ꂽ�_�ɉ�]�s����|����
	D3DXVec3TransformCoord(&Vec, &Vec, &Mtx);

	// �J�����̈ʒu = �J�����̒����_ + (�����_����̊p�x * ���_�܂ł̋���)
	Cwk->PosCameraP = Cwk->PosCameraR + (Vec * Cwk->Cam_len);

}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float distance)
{
	// �����_�Ǝ��_�̋���
	Cwk->Cam_len = CAMERA_CMK;

	// ACE�̎擾
	Ace *ace;
	ace = GetAce();

	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();

	// �v���C���[�̎擾
	Player *player;
	player = GetPlayer();

	// �ʏ펞�̒����_�擾
	CalcPosCameraP(&D3DXVECTOR3(GetPositionPlayer().x*0.8f, GetPositionPlayer().y+20, GetPositionPlayer().z-70));
	
	// ����,�s�k���o���̃J����
	if (player->PlayerMove == 3|| player->PlayerMove == 2)
	{
		// �ʏ펞�̒����_�擾
		CalcPosCameraP(&D3DXVECTOR3(GetPositionPlayer().x, GetPositionPlayer().y + 20, GetPositionPlayer().z - 70));
	}

	// ���b�N�I���̎g�p���A���g�p���̐ݒ�
	if ((GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_RR)) && player->PlayerMove == 1)
	{
		if (boss->use == true || ace->use == true)
		{
			// ���b�N�I���g���K�[�����g�p�Ȃ�
			if (Cwk->LockOnTrigger == false)
			{
				// ���b�N���̍Đ�
				PlaySound(SOUND_LABEL_SE_ROCK);

				// ���b�N�I���g���K�[���g�p��
				Cwk->LockOnTrigger = true;
			}
			else
			{
				// �L�����Z�����̍Đ�
				PlaySound(SOUND_LABEL_SE_CANCEL);

				// ���b�N�I���g���K�[�𖢎g�p��
				Cwk->LockOnTrigger = false;
			}
		}
	}

	// ���b�N�I���̏���
	// �G�ƃv���C���[�����񂾒�����ɃJ������ݒu
	//(���v���C���[�ƃ^�[�Q�b�g�̍��W�����S�Ɉ�v����ƃJ�������o�O��)
	if (player->PlayerMove == 1 && Cwk->LockOnTrigger == true)
	{
		// �v���C���[����^�[�Q�b�g�w�̃x�N�g��
		D3DXVECTOR3 PlayerToTarget = D3DXVECTOR3(pos1.x - pos2.x, (pos1.y - pos2.y) + 100, pos1.z - pos2.z);

		// �v���C���[����^�[�Q�b�g�w�̃x�N�g���̒P�ʃx�N�g��
		float Length = sqrtf(powf(PlayerToTarget.x, 2.0f) +
			powf(PlayerToTarget.y + 40, 2.0f) + powf(PlayerToTarget.z, 2.0f));

		// ��̋t�x�N�g���̎擾
		D3DXVECTOR3 UnitvecPlayerToTarget =
			D3DXVECTOR3(PlayerToTarget.x / Length, PlayerToTarget.y / Length, PlayerToTarget.z / Length);

		// �J�����ʒu�擾
		Cwk->PosCameraP = D3DXVECTOR3(pos2.x - UnitvecPlayerToTarget.x * distance, GetPositionPlayer().y, pos2.z - UnitvecPlayerToTarget.z * distance);

		// �����_�擾
		Cwk->PosCameraR = D3DXVECTOR3(pos1.x + UnitvecPlayerToTarget.x, pos1.y + UnitvecPlayerToTarget.y, pos1.z + UnitvecPlayerToTarget.z);
	}

	//�J�����̂����̈ړ�����
	Cwk->CameraAriaLimit(FIRST_CAMERA_POS_LIMIT, BOSS_CAMERA_POS_LIMIT);

}
//=============================================================================
// �J�����̕`�揈��
//=============================================================================
void DrawCamera(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&Cwk->MtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&Cwk->MtxView,
		&Cwk->PosCameraP,		// �J�����̎��_
		&Cwk->PosCameraR,		// �J�����̒����_
		&Cwk->PosCameraU);		// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_VIEW, &Cwk->MtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&Cwk->MtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&Cwk->MtxProjection,
		VIEW_ANGLE,										// ����p
		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),	// �A�X�y�N�g��
		VIEW_NEAR_Z,									// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);									// �r���[���ʂ�FarZ�l

// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_PROJECTION, &Cwk->MtxProjection);
}

//=============================================================================
// �J�������[�N�̃A�h���X���擾
//=============================================================================
Camera *GetCamerawk(void)
{
	return(Cwk);
}

//=============================================================================
// �J�����̌����̎擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return Cwk->RotCameraP;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return Cwk->MtxView;
}


