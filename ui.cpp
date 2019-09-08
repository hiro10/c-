//=============================================================================
//
// UI���� [ui.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "main.h"
#include "ui.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "enemy.h"
#include "ace.h"
#include "boss.h"
#include "item.h"
#include "sound.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexUi(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureUiLogo = NULL;		// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				vertexWkUiLogo[NUM_VERTEX];		// ���_���i�[���[�N

LPDIRECT3DTEXTURE9		D3DTextureUiAceLogo = NULL;		// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				vertexWkUiAceLogo[NUM_VERTEX];	// ���_���i�[���[�N

LPDIRECT3DTEXTURE9		D3DTextureUibossLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				vertexWkUibossLogo[NUM_VERTEX];	// ���_���i�[���[�N

LPDIRECT3DTEXTURE9		D3DTextureUiLogo2 = NULL;		// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				vertexWkUiLogo2[NUM_VERTEX];	// ���_���i�[���[�N

// �_�ŏ����p�ϐ�
static bool flgFlash1 = true;							// �_�ŗpflg

int uicount = 0;									// �_�ŗp�J�E���g
//=============================================================================
// ����������
//=============================================================================
HRESULT InitUi(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DXCreateTextureFromFile(Device,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_UI_LOGO1,				// �t�@�C���̖��O
		&D3DTextureUiLogo);				// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(Device,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_UIACE_LOGO,				// �t�@�C���̖��O
		&D3DTextureUiAceLogo);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(Device,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_UIITEM_LOGO,			// �t�@�C���̖��O
		&D3DTextureUibossLogo);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(Device,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_UI2_LOGO,				// �t�@�C���̖��O
		&D3DTextureUiLogo2);			// �ǂݍ��ރ������[

	MakeVertexUi();						// ���_���̍쐬

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUi(void)
{
	if (D3DTextureUiLogo != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureUiLogo->Release();
		D3DTextureUiLogo = NULL;
	}

	if (D3DTextureUiLogo2 != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureUiLogo2->Release();
		D3DTextureUiLogo2 = NULL;
	}

	if (D3DTextureUiAceLogo != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureUiAceLogo->Release();
		D3DTextureUiAceLogo = NULL;
	}

	if (D3DTextureUibossLogo != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureUibossLogo->Release();
		D3DTextureUibossLogo = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateUi(void)
{
	// �_�ŗp�̃J�E���^���C���N�������g
	uicount++;

	if (GetMode() == MODE_GAME)
	{
		// �J�E���^��30��艺�Ȃ�
		if (uicount > MODE_GAME_COUNT)
		{
			// flg��false��
			flgFlash1 = !flgFlash1;

			// count�̏�����
			uicount = 0;
		}
	}
	else if ((GetMode() == MODE_BOSS))
	{
		// �J�E���^��30��艺�Ȃ�
		if (uicount > MODE_BOSS_COUNT)
		{
			// flg��false��
			flgFlash1 = !flgFlash1;

			// count�̏�����
			uicount = 0;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUi(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �G�[�X�̎擾
	Ace*ace;
	ace = GetAce();

	// �{�X�̎擾
	Boss*boss;
	boss = GetBoss();

	// �G�[�X�����g�p��HP��0����Ȃ�
	if ((GetMode() == MODE_GAME)&&ace->use == false && ace->hp > 0)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTextureUiLogo);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkUiLogo, sizeof(VERTEX_2D));
	}
	
	// �G�[�X���g�p���Ȃ�
	else if((GetMode() == MODE_GAME) && ace->use == true)
	{
		// �_�ŗp�̏���������
		if (flgFlash1 == true)
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureUiAceLogo);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkUiAceLogo, sizeof(VERTEX_2D));
		}
	}

	// �{�X���g�p���Ȃ�
	if (boss->use == true)
	{
		if (boss->pos.y > 1200)
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureUiLogo2);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkUiLogo2, sizeof(VERTEX_2D));

			// �_�ŗp�̏���������
			if (flgFlash1 == true)
			{
				// ���_�t�H�[�}�b�g�̐ݒ�
				Device->SetFVF(FVF_VERTEX_2D);

				// �e�N�X�`���̐ݒ�
				Device->SetTexture(0, D3DTextureUibossLogo);

				// �|���S���̕`��
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkUibossLogo, sizeof(VERTEX_2D));
			}
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexUi(void)
{

	// ���_���W�̐ݒ�
	vertexWkUiLogo[0].vtx = D3DXVECTOR3(UILOGO1_POS_X, UILOGO1_POS_Y, 0.0f);
	vertexWkUiLogo[1].vtx = D3DXVECTOR3(UILOGO1_POS_X + UILOGO1_SIZE_X, UILOGO1_POS_Y, 0.0f);
	vertexWkUiLogo[2].vtx = D3DXVECTOR3(UILOGO1_POS_X, UILOGO1_POS_Y + UILOGO1_SIZE_Y, 0.0f);
	vertexWkUiLogo[3].vtx = D3DXVECTOR3(UILOGO1_POS_X + UILOGO1_SIZE_X, UILOGO1_POS_Y + UILOGO1_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	vertexWkUiLogo[0].rhw =
		vertexWkUiLogo[1].rhw =
		vertexWkUiLogo[2].rhw =
		vertexWkUiLogo[3].rhw = 1.0f;

	//// ���ˌ��̐ݒ�
	vertexWkUiLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWkUiLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkUiLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkUiLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkUiLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	vertexWkUiAceLogo[0].vtx = D3DXVECTOR3(UIACELOGO1_POS_X, UIACELOGO1_POS_Y, 0.0f);
	vertexWkUiAceLogo[1].vtx = D3DXVECTOR3(UIACELOGO1_POS_X + UIACELOGO1_SIZE_X, UIACELOGO1_POS_Y, 0.0f);
	vertexWkUiAceLogo[2].vtx = D3DXVECTOR3(UIACELOGO1_POS_X, UIACELOGO1_POS_Y + UIACELOGO1_SIZE_Y, 0.0f);
	vertexWkUiAceLogo[3].vtx = D3DXVECTOR3(UIACELOGO1_POS_X + UIACELOGO1_SIZE_X, UIACELOGO1_POS_Y + UIACELOGO1_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�pp
	vertexWkUiAceLogo[0].rhw =
		vertexWkUiAceLogo[1].rhw =
		vertexWkUiAceLogo[2].rhw =
		vertexWkUiAceLogo[3].rhw = 1.0f;

	//// ���ˌ��̐ݒ�
	vertexWkUiAceLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiAceLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiAceLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiAceLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWkUiAceLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkUiAceLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkUiAceLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkUiAceLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	vertexWkUibossLogo[0].vtx = D3DXVECTOR3(UIITEMLOGO1_POS_X, UIITEMLOGO1_POS_Y, 0.0f);
	vertexWkUibossLogo[1].vtx = D3DXVECTOR3(UIITEMLOGO1_POS_X + UIITEMLOGO1_SIZE_X, UIITEMLOGO1_POS_Y, 0.0f);
	vertexWkUibossLogo[2].vtx = D3DXVECTOR3(UIITEMLOGO1_POS_X, UIITEMLOGO1_POS_Y + UIITEMLOGO1_SIZE_Y, 0.0f);
	vertexWkUibossLogo[3].vtx = D3DXVECTOR3(UIITEMLOGO1_POS_X + UIITEMLOGO1_SIZE_X, UIITEMLOGO1_POS_Y + UIITEMLOGO1_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�pp
	vertexWkUibossLogo[0].rhw =
		vertexWkUibossLogo[1].rhw =
		vertexWkUibossLogo[2].rhw =
		vertexWkUibossLogo[3].rhw = 1.0f;

	//// ���ˌ��̐ݒ�
	vertexWkUibossLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUibossLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUibossLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUibossLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWkUibossLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkUibossLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkUibossLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkUibossLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	vertexWkUiLogo2[0].vtx = D3DXVECTOR3(UILOGO2_POS_X, UILOGO2_POS_Y, 0.0f);
	vertexWkUiLogo2[1].vtx = D3DXVECTOR3(UILOGO2_POS_X + UILOGO2_SIZE_X, UILOGO2_POS_Y, 0.0f);
	vertexWkUiLogo2[2].vtx = D3DXVECTOR3(UILOGO2_POS_X, UILOGO2_POS_Y + UILOGO2_SIZE_Y, 0.0f);
	vertexWkUiLogo2[3].vtx = D3DXVECTOR3(UILOGO2_POS_X + UILOGO2_SIZE_X, UILOGO2_POS_Y + UILOGO2_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	vertexWkUiLogo2[0].rhw =
		vertexWkUiLogo2[1].rhw =
		vertexWkUiLogo2[2].rhw =
		vertexWkUiLogo2[3].rhw = 1.0f;

	//// ���ˌ��̐ݒ�
	vertexWkUiLogo2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWkUiLogo2[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkUiLogo2[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkUiLogo2[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkUiLogo2[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	return S_OK;
}