//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "sound.h"
#include "game.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPause(void);


//*****************************************************************************
// �O���[�o���ϐ�
//**********************************************************************
LPDIRECT3DTEXTURE9		D3DTexturePauseLogo = NULL;				// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				VertexWkPauseLogo[NUM_VERTEX];			// ���_���i�[���[�N

LPDIRECT3DTEXTURE9		D3DTextureReturnTitleLogo = NULL;		// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				VertexWkReturnTitleLogo[NUM_VERTEX];	// ���_���i�[���[�N


bool FlgFlash = true;								// �_�ŗpflg
static int count = 0;								// �_�ŗp�J�E���g
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DXCreateTextureFromFile(Device,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PAUSE_LOGO1,			// �t�@�C���̖��O
		&D3DTexturePauseLogo);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(Device,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RETURNTITLE_LOGO1,			// �t�@�C���̖��O
		&D3DTextureReturnTitleLogo);			// �ǂݍ��ރ������[
								
	MakeVertexPause();					// ���_���̍쐬

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
	if (D3DTexturePauseLogo != NULL)
	{// �e�N�X�`���̊J��
		D3DTexturePauseLogo->Release();
		D3DTexturePauseLogo = NULL;
	}

	if (D3DTextureReturnTitleLogo != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureReturnTitleLogo->Release();
		D3DTextureReturnTitleLogo = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{
	count++;					// �_�ŗp�̃J�E���^���C���N�������g

	if (count > 30)
	{
		FlgFlash = !FlgFlash;	// flg��false��

		count = 0;				// count�̏�����
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (FlgFlash == true )
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTexturePauseLogo);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkPauseLogo, sizeof(VERTEX_2D));
	}

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureReturnTitleLogo);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkReturnTitleLogo, sizeof(VERTEX_2D));
	
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPause(void)
{

	// ���_���W�̐ݒ�
	VertexWkPauseLogo[0].vtx = D3DXVECTOR3(PAUSELOGO1_POS_X, PAUSELOGO1_POS_Y, 0.0f);
	VertexWkPauseLogo[1].vtx = D3DXVECTOR3(PAUSELOGO1_POS_X + PAUSELOGO1_SIZE_X, PAUSELOGO1_POS_Y, 0.0f);
	VertexWkPauseLogo[2].vtx = D3DXVECTOR3(PAUSELOGO1_POS_X, PAUSELOGO1_POS_Y + PAUSELOGO1_SIZE_Y, 0.0f);
	VertexWkPauseLogo[3].vtx = D3DXVECTOR3(PAUSELOGO1_POS_X + PAUSELOGO1_SIZE_X, PAUSELOGO1_POS_Y + PAUSELOGO1_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	VertexWkPauseLogo[0].rhw =
		VertexWkPauseLogo[1].rhw =
		VertexWkPauseLogo[2].rhw =
		VertexWkPauseLogo[3].rhw = 1.0f;

	//// ���ˌ��̐ݒ�
	VertexWkPauseLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkPauseLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkPauseLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkPauseLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	VertexWkPauseLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkPauseLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkPauseLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkPauseLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	VertexWkReturnTitleLogo[0].vtx = D3DXVECTOR3(RETURNTITLELOGO1_POS_X, RETURNTITLELOGO1_POS_Y, 0.0f);
	VertexWkReturnTitleLogo[1].vtx = D3DXVECTOR3(RETURNTITLELOGO1_POS_X + RETURNTITLELOGO1_SIZE_X, RETURNTITLELOGO1_POS_Y, 0.0f);
	VertexWkReturnTitleLogo[2].vtx = D3DXVECTOR3(RETURNTITLELOGO1_POS_X, RETURNTITLELOGO1_POS_Y + RETURNTITLELOGO1_SIZE_Y, 0.0f);
	VertexWkReturnTitleLogo[3].vtx = D3DXVECTOR3(RETURNTITLELOGO1_POS_X + RETURNTITLELOGO1_SIZE_X, RETURNTITLELOGO1_POS_Y + RETURNTITLELOGO1_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�pp
	VertexWkReturnTitleLogo[0].rhw =
		VertexWkReturnTitleLogo[1].rhw =
		VertexWkReturnTitleLogo[2].rhw =
		VertexWkReturnTitleLogo[3].rhw = 1.0f;

	//// ���ˌ��̐ݒ�
	VertexWkReturnTitleLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkReturnTitleLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkReturnTitleLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkReturnTitleLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	VertexWkReturnTitleLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkReturnTitleLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkReturnTitleLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkReturnTitleLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}