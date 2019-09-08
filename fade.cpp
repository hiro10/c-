//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "fade.h"
#include "game.h"
#include "timer.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 Device);
void SetColor(D3DCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureFade = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffFade = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXCOLOR				Color;
FADE					Fade = FADE_IN;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFade(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̐ݒ�
	MakeVertexFade(Device);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFade(void)
{
	if(D3DTextureFade != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureFade->Release();
	}

	if(D3DVtxBuffFade != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffFade->Release();
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFade(void)
{
	if(Fade != FADE_NONE)
	{
		// �t�F�[�h������
		if(Fade == FADE_OUT)
		{
			// �t�F�[�h�A�E�g����
			Color.a += FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���

			if(Color.a >= 1.0f)
			{
				Player *player;
				MODE mode;

				// �t�F�[�h�C�������ɐ؂�ւ�
				Color.a = 1.0f;
				SetFade(FADE_IN);

				// ���݂̃��[�h���擾
				mode = GetMode();

				// �v���C���[�̎擾
				player = GetPlayer();

				// �G�l�~�[���HP��0�ȉ��ɂȂ�����
				if (GetMode() == MODE_GAME &&(player->hp <= PLAYER_DEAD))
				{
					// �I������
					UninitGame();
					UninitBossGame();

					// ���[�h��2�i�߂�
					mode = (MODE)((mode + 2) % MODE_MAX);	
				}
				else
				{
					// ���[�h���P�i�߂�
					mode = (MODE)((mode + 1) % MODE_MAX);
				}

				// ���[�h��ݒ�
				SetMode(mode);
			}

			// �F��ݒ�
			SetColor(Color);
		}

		else if(Fade == FADE_IN)
		{// �t�F�[�h�C������
			Color.a -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
			if(Color.a <= 0.0f)
			{
				// �t�F�[�h�����I��
				Color.a = 0.0f;
				SetFade(FADE_NONE);
			}

			// �F��ݒ�
			SetColor(Color);
		}
	}
}

//=============================================================================
// �^�C�g�����
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    Device->SetStreamSource(0, D3DVtxBuffFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, NULL);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,					// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,				// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffFade,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))							// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffFade->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[0].diffuse = Color;
		Vtx[1].diffuse = Color;
		Vtx[2].diffuse = Color;
		Vtx[3].diffuse = Color;

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffFade->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �F��ݒ�
//=============================================================================
void SetColor(D3DCOLOR col)
{
	VERTEX_2D *Vtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffFade->Lock(0, 0, (void**)&Vtx, 0);

	// ���ˌ��̐ݒ�
	Vtx[0].diffuse = col;
	Vtx[1].diffuse = col;
	Vtx[2].diffuse = col;
	Vtx[3].diffuse = col;

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffFade->Unlock();
}

//=============================================================================
// �t�F�[�h�̏�Ԑݒ�
//=============================================================================
void SetFade(FADE fade)
{
	Fade = fade;
}

//=============================================================================
// �t�F�[�h�̏�Ԏ擾
//=============================================================================
FADE GetFade(void)
{
	return Fade;
}

