//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "timer.h"
#include "fade.h"
#include "ace.h"
#include "boss.h"
#include "player.h"
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTimer(LPDIRECT3DDEVICE9 Device);
void SetTextureTimer(int idx, int number);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureTimer[2] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffTimer = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

int						Timer;						// �^�C�}�[
bool					EnableTimer;					// �^�C�}�[����ON/OFF

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTimer(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �^�C�}�[�̏�����
	Timer = 999 * 60;
	EnableTimer = true;

	// ���_���̍쐬
	MakeVertexTimer(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_TIMER,			// �t�@�C���̖��O
								&D3DTextureTimer[0]);// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FRAME_TIMER,	// �t�@�C���̖��O
								&D3DTextureTimer[1]);// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimer(void)
{
	if(D3DTextureTimer[0] != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureTimer[0]->Release();
		D3DTextureTimer[0] = NULL;
	}

	if(D3DTextureTimer[1] != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureTimer[1]->Release();
		D3DTextureTimer[1] = NULL;
	}

	if(D3DVtxBuffTimer != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffTimer->Release();
		D3DVtxBuffTimer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimer(void)
{
	// �G�[�X�̎擾
	Ace*ace;
	ace = GetAce();

	// �{�X�̎擾
	Boss*boss;
	boss = GetBoss();

	// �v���C���[�̎擾
	Player*player;
	player = GetPlayer();

	if (EnableTimer)
	{
		if (ace->hp > ACE_DEAD||boss->hp>0)
		{
			//�^�C�����f�N�������g
			Timer--;
		}
		if (Timer < 0)
		{//�^�C����0��艺�Ȃ�
			Timer = 0;

			if (GetMode() == MODE_BOSS)
			{
				// �v���C���[������
				player->hp = PLAYER_DEAD;
			}


			//�t�F�[�h�A�E�g
			SetFade(FADE_OUT);
		}
		else if ((Timer / 60) >= (int)(powf(10.0f, (float)(NUM_TIMER + 1))))
		{
			Timer = (int)(powf(10.0f, (float)(NUM_TIMER + 1))) - 1;
		}
	}

	for(int CntPlace = 0; CntPlace < NUM_TIMER; CntPlace++)
	{
		int number;
		int timer;
		
		timer = Timer + 59;
		number = ((timer / 60) % (int)(powf(10.0f, (float)(NUM_TIMER - CntPlace)))) / (int)(powf(10.0f, (float)(NUM_TIMER - CntPlace - 1)));
		SetTextureTimer(CntPlace, number);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    Device->SetStreamSource(0, D3DVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureTimer[0]);

	// �|���S���̕`��
	for(int CntPlace = 0; CntPlace < NUM_TIMER; CntPlace++)
	{
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntPlace * 4), NUM_POLYGON);
	}

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureTimer[1]);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_TIMER * 4), NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTimer(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_TIMER + 4),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,								// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,									// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,								// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffTimer,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))											// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffTimer->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntPlace = 0; CntPlace < NUM_TIMER; CntPlace++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(TIMER_POS_X + CntPlace * TIMER_SIZE_X + TIMER_INTERVAL_X, TIMER_POS_Y, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(TIMER_POS_X + CntPlace * (TIMER_INTERVAL_X + TIMER_SIZE_X) + TIMER_SIZE_X, TIMER_POS_Y, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(TIMER_POS_X + CntPlace * TIMER_SIZE_X + TIMER_INTERVAL_X, TIMER_POS_Y + TIMER_SIZE_Y, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(TIMER_POS_X + CntPlace * (TIMER_INTERVAL_X + TIMER_SIZE_X) + TIMER_SIZE_X, TIMER_POS_Y + TIMER_SIZE_Y, 0.0f);

			// rhw�̐ݒ�
			Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			Vtx[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			Vtx[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			Vtx[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			Vtx[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

			// �e�N�X�`�����W�̐ݒ�
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(TIMER_POS_X - 17.5f, TIMER_POS_Y - 25, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(TIMER_POS_X + (TIMER_INTERVAL_X + TIMER_SIZE_X) * NUM_TIMER + 17.5f, TIMER_POS_Y - 25, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(TIMER_POS_X - 17.5f, TIMER_POS_Y + 55, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(TIMER_POS_X + (TIMER_INTERVAL_X + TIMER_SIZE_X) * NUM_TIMER + 17.5f, TIMER_POS_Y + 55, 0.0f);

		// rhw�̐ݒ�
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffTimer->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTimer(int idx, int number)
{
	VERTEX_2D *Vtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffTimer->Lock(0, 0, (void**)&Vtx, 0);

	Vtx += (idx * 4);

	// ���_���W�̐ݒ�
	Vtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	Vtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	Vtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	Vtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffTimer->Unlock();
}

//=============================================================================
// �^�C�}�[�̊J�n
//=============================================================================
void GetEnableTimer(bool Enable)
{
	EnableTimer = Enable;
}

//=============================================================================
// �^�C�}�[�̃��Z�b�g
//=============================================================================
void ResetTimer(int Time)
{
	Timer = Time * 60;
}

