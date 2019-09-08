//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "boss.h"
#include "sound.h"
#include "ace.h"
#include "timer.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 Device);
void SetColorResultLogo(void);
void SetColorResultLogoClear(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureResult = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResult = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

LPDIRECT3DTEXTURE9		D3DTextureResultClear = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultClear = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

LPDIRECT3DTEXTURE9		D3DTextureResultLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

LPDIRECT3DTEXTURE9		D3DTextureResultLogoClear = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultLogoClear = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

LPDIRECT3DTEXTURE9		D3DTextureResultRank = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultRank = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

// RANK
LPDIRECT3DTEXTURE9		D3DTextureResultLogoRank = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultLogoRank = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

// S
LPDIRECT3DTEXTURE9		D3DTextureRankS = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffRankS = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

// A
LPDIRECT3DTEXTURE9		D3DTextureRankA = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffRankA = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

// B
LPDIRECT3DTEXTURE9		D3DTextureRankB = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffRankB = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

// C
LPDIRECT3DTEXTURE9		D3DTextureRankC = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffRankC = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^


int						CountAppearResult = 0;		// �o���܂ł̑҂�����
float					AlphaResult = 1.0f;			// ���U���g���S�̃��l
int						CountWaitResult = 0;			// �҂�����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	CountAppearResult = 0;
	AlphaResult = 0.0f;
	CountWaitResult = 0;

	// ���_���̍쐬
	MakeVertexResult(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_RESULT,				// �t�@�C���̖��O
								&D3DTextureResult);		// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
						TEXTURE_RESULT_CLEAR,				// �t�@�C���̖��O
							&D3DTextureResultClear);		// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_RESULT_LOGO,		// �t�@�C���̖��O
								&D3DTextureResultLogo);	// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
							TEXTURE_RESULT_LOGO_CLEAR,		// �t�@�C���̖��O
							&D3DTextureResultLogoClear);	// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
							TEXTURE_RESULT_LOGO_RANK,		// �t�@�C���̖��O
							&D3DTextureResultLogoRank);	// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
							TEXTURE_RESULT_RANKS,			// �t�@�C���̖��O
							&D3DTextureRankS);			// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
							TEXTURE_RESULT_RANKA,			// �t�@�C���̖��O
							&D3DTextureRankA);			// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
							TEXTURE_RESULT_RANKB,			// �t�@�C���̖��O
							&D3DTextureRankB);			// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
							TEXTURE_RESULT_RANKC,			// �t�@�C���̖��O
							&D3DTextureRankC);			// �ǂݍ��ރ������[


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	if(D3DTextureResult != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureResult->Release();
		D3DTextureResult = NULL;
	}

	if(D3DVtxBuffResult != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffResult->Release();
		D3DVtxBuffResult = NULL;
	}

	if (D3DTextureResultClear != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureResultClear->Release();
		D3DTextureResultClear = NULL;
	}

	if (D3DVtxBuffResultClear != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffResultClear->Release();
		D3DVtxBuffResultClear = NULL;
	}

	if(D3DTextureResultLogo != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureResultLogo->Release();
		D3DTextureResultLogo = NULL;
	}

	if(D3DVtxBuffResultLogo != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffResultLogo->Release();
		D3DVtxBuffResultLogo = NULL;
	}

	if (D3DTextureResultLogoClear != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureResultLogoClear->Release();
		D3DTextureResultLogoClear = NULL;
	}

	if (D3DVtxBuffResultLogoClear != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffResultLogoClear->Release();
		D3DVtxBuffResultLogoClear = NULL;
	}

	if (D3DTextureResultLogoRank != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureResultLogoRank->Release();
		D3DTextureResultLogoRank = NULL;
	}

	if (D3DVtxBuffResultLogoRank != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffResultLogoRank->Release();
		D3DVtxBuffResultLogoRank = NULL;
	}

	if (D3DTextureRankS != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureRankS->Release();
		D3DTextureRankS = NULL;
	}

	if (D3DVtxBuffRankS != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffRankS->Release();
		D3DVtxBuffRankS = NULL;
	}

	if (D3DTextureRankA != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureRankA->Release();
		D3DTextureRankA = NULL;
	}

	if (D3DVtxBuffRankA != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffRankA->Release();
		D3DVtxBuffRankA = NULL;
	}

	if (D3DTextureRankB != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureRankB->Release();
		D3DTextureRankB = NULL;
	}

	if (D3DVtxBuffRankB != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffRankB->Release();
		D3DVtxBuffRankB = NULL;
	}

	if (D3DTextureRankC != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureRankC->Release();
		D3DTextureRankC = NULL;
	}

	if (D3DVtxBuffRankC != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffRankC->Release();
		D3DVtxBuffRankC = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();

	// �v���C���[�̎擾
	Player *player;
	player = GetPlayer();

	if (AlphaResult < 1.0f)
	{
		AlphaResult += APPERA_RESULT_TIME;
		if (AlphaResult >= 1.0f)
		{
			AlphaResult = 1.0f;
		}

		SetColorResultLogo();
		SetColorResultLogoClear();
	}
	

	if(GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_TOUCH))
	{// Enter��������A�t�F�[�h�A�E�g���ă��[�h��؂�ւ�����

		SetFade(FADE_OUT);
	}

	CountWaitResult++;
	if(CountWaitResult > LIMIT_COUNT_WAIT)
	{
		SetFade(FADE_OUT);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();

	// �v���C���[�̎擾
	Player *player;
	player = GetPlayer();

	// �G�[�X�̎擾
	Ace *ace;
	ace = GetAce();

	if (boss->use == true ||( player->hp <= 0))
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		Device->SetStreamSource(0, D3DVtxBuffResult, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTextureResult);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		Device->SetStreamSource(0, D3DVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTextureResultLogo);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// RANK(�����j
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		Device->SetStreamSource(0, D3DVtxBuffResultLogoRank, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTextureResultLogoRank);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// �G�[�X�������Ă�����C
		if (ace->hp > 0)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			Device->SetStreamSource(0, D3DVtxBuffRankC, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureRankC);

			// �|���S���̕`��
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
		else
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			Device->SetStreamSource(0, D3DVtxBuffRankB, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureRankB);

			// �|���S���̕`��
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
	else if (boss->use == false&&boss->hp<=0)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		Device->SetStreamSource(0, D3DVtxBuffResultClear, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTextureResultClear);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		Device->SetStreamSource(0, D3DVtxBuffResultLogoClear, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTextureResultLogoClear);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// RANK(�����j
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		Device->SetStreamSource(0, D3DVtxBuffResultLogoRank, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTextureResultLogoRank);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// �G�[�X�����񂾂�
		if (ace->hp <= 0)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			Device->SetStreamSource(0, D3DVtxBuffRankS, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureRankS);

			// �|���S���̕`��
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}

		// �G�[�X�������Ă�����
		else if (ace->hp > 0)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			Device->SetStreamSource(0, D3DVtxBuffRankA, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureRankA);

			// �|���S���̕`��
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffResult,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffResult->Lock(0, 0, (void**)&Vtx, 0);

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
		Vtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResult->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffResultClear,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffResultClear->Lock(0, 0, (void**)&Vtx, 0);

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
		Vtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultClear->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffResultLogo,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffResultLogo->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultLogo->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffResultLogoClear,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffResultLogoClear->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffResultLogoRank,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffResultLogoRank->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_POS_X + RESULT_LOGO_WIDTH, RANK_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y+ RESULT_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_POS_X + RESULT_LOGO_WIDTH, RANK_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffRankS,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffRankS->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffRankA,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffRankA->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffRankB,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffRankB->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffRankC,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffRankC->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultLogoClear->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorResultLogo(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffResultLogo->Lock(0, 0, (void**)&Vtx, 0);

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultLogo->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorResultLogoClear(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffResultLogoClear->Lock(0, 0, (void**)&Vtx, 0);

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffResultLogoClear->Unlock();
	}
}