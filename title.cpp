//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 Device);
void SetColorTitleLogo(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureTitle = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffTitle = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTextureTitleLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffTitleLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTextureStart = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffStart = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
int						CountAppearStart = 0;		//
float					AlphaLogo = 0.0f;			//
int						CountDisp = 0;				//
bool					DispStart = false;			//
int						ConutDemo = 0;				//

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	CountAppearStart = 0;
	AlphaLogo = 0.0f;
	CountDisp = 0;
	DispStart = false;
	ConutDemo = 0;

	// ���_���̍쐬
	MakeVertexTitle(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_TITLE,				// �t�@�C���̖��O
								&D3DTextureTitle);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_TITLE_LOGO,			// �t�@�C���̖��O
								&D3DTextureTitleLogo);	// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_LOGO_START,			// �t�@�C���̖��O
								&D3DTextureStart);		// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if(D3DTextureTitle != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureTitle->Release();
		D3DTextureTitle = NULL;
	}

	if(D3DVtxBuffTitle != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffTitle->Release();
		D3DVtxBuffTitle = NULL;
	}

	if(D3DTextureTitleLogo != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureTitleLogo->Release();
		D3DTextureTitleLogo = NULL;
	}

	if(D3DVtxBuffTitleLogo != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffTitleLogo->Release();
		D3DVtxBuffTitleLogo = NULL;
	}

	if(D3DTextureStart != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureStart->Release();
		D3DTextureStart = NULL;
	}

	if(D3DVtxBuffStart != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffStart->Release();
		D3DVtxBuffStart = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	// SE��~
	StopSound(SOUND_LABEL_SE_TRUE);

	// SE��~
	StopSound(SOUND_LABEL_SE_FALSE);

	if(AlphaLogo < 1.0f)
	{
		AlphaLogo += 0.005f;
		if(AlphaLogo >= 1.0f)
		{
			AlphaLogo = 1.0f;
		}
		SetColorTitleLogo();
	}
	else
	{
		CountAppearStart++;
		if(CountAppearStart > COUNT_APPERA_START)
		{
			CountDisp = (CountDisp + 1) % 80;
			if(CountDisp > INTERVAL_DISP_START)
			{
				DispStart = false;
			}
			else
			{
				DispStart = true;
			}
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN)|| IsButtonTriggered(0, BUTTON_TOUCH))
	{
		if(CountAppearStart == 0)
		{// �^�C�g���o��X�L�b�v
			AlphaLogo = 1.0f;

			SetColorTitleLogo();

			CountAppearStart = COUNT_APPERA_START;
		}
		else
		{// �Q�[����

			// SE�̍Đ�
			PlaySound(SOUND_LABEL_SE_ENTER);

			// �t�F�[�h�A�E�g
			SetFade(FADE_OUT);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    Device->SetStreamSource(0, D3DVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureTitle);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    Device->SetStreamSource(0, D3DVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureTitleLogo);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	if(DispStart == true)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		Device->SetStreamSource(0, D3DVtxBuffStart, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTextureStart);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffTitle,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffTitle->Lock(0, 0, (void**)&Vtx, 0);

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
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffTitle->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffTitleLogo,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffTitleLogo->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffTitleLogo->Unlock();
	}


	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffStart,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffStart->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y + START_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffStart->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void SetColorTitleLogo(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffTitleLogo->Lock(0, 0, (void**)&Vtx, 0);

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffTitleLogo->Unlock();
	}

}

