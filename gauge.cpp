//=============================================================================
//
// �w�i���� [gauge.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "gauge.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGauge(void);
void SetVertexGauge(float val);
void SetTextureGauge(float val);
void SetVertexGaugeFrame(void);
void SetTextureGaugeFrame(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureGauge = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTextureGaugeFrame = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTexturehp = NULL;			// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				VertexWkhp[NUM_VERTEX];			// ���_���i�[���[�N
VERTEX_2D				VertexWkGauge[NUM_VERTEX];		// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_GAUGE00,						// �t�@�C���̖��O
		&D3DTextureGauge);							// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_GAUGEFRAME00,					// �t�@�C���̖��O
		&D3DTextureGaugeFrame);						// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,				// �f�o�C�X�̃|�C���^
		TEXTURE_HP,									// �t�@�C���̖��O
		&D3DTexturehp);								// �ǂݍ��ރ������̃|�C���^

	// ���_���̍쐬
	MakeVertexGauge();

	SetVertexGauge(100.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGauge(void)
{
	if (D3DTextureGauge != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureGauge->Release();
		D3DTextureGauge = NULL;
	}

	if (D3DTexturehp != NULL)
	{// �e�N�X�`���̊J��
		D3DTexturehp->Release();
		D3DTexturehp = NULL;
	}

	if (D3DTextureGaugeFrame != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureGaugeFrame->Release();
		D3DTextureGaugeFrame = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGauge(void)
{
	// ���t���[�����s���鏈��
	Player *player = GetPlayer();
		
	// �v���C���[�̌��݂�HP/�v���C���[��HP�̍ő�l
	float val  = player->hp / (float)(PLAYER_LIFEMAX);

	SetVertexGauge(val);
	SetTextureGauge(val);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Player *player = GetPlayer();

	//���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureGauge);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkGauge, sizeof(VERTEX_2D));

	SetVertexGaugeFrame();
	SetTextureGaugeFrame();

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureGaugeFrame);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkGauge, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�(TEXTURE_TITLE)
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�(TEXTURE_TITLE)
	Device->SetTexture(0, D3DTexturehp);

	// �|���S���̕`��(TEXTURE_TITLE)
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkhp, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGauge(void)
{
	// ���_���W�̐ݒ�
	VertexWkGauge[0].vtx = D3DXVECTOR3(GAUGE00_POS_X, GAUGE00_POS_Y, 0.0f);
	VertexWkGauge[1].vtx = D3DXVECTOR3(GAUGE00_POS_X + GAUGE00_SIZE_X, GAUGE00_POS_Y, 0.0f);
	VertexWkGauge[2].vtx = D3DXVECTOR3(GAUGE00_POS_X, GAUGE00_POS_Y + GAUGE00_SIZE_Y, 0.0f);
	VertexWkGauge[3].vtx = D3DXVECTOR3(GAUGE00_POS_X + GAUGE00_SIZE_X, GAUGE00_POS_Y + GAUGE00_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	VertexWkGauge[0].rhw =
	VertexWkGauge[1].rhw =
	VertexWkGauge[2].rhw =
	VertexWkGauge[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	VertexWkGauge[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkGauge[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkGauge[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkGauge[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	VertexWkGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	VertexWkhp[0].vtx = D3DXVECTOR3(HP_POS_X, HP_POS_Y, 0.0f);
	VertexWkhp[1].vtx = D3DXVECTOR3(HP_POS_X + HP_SIZE_X, HP_POS_Y, 0.0f);
	VertexWkhp[2].vtx = D3DXVECTOR3(HP_POS_X, HP_POS_Y + HP_SIZE_Y, 0.0f);
	VertexWkhp[3].vtx = D3DXVECTOR3(HP_POS_X + HP_SIZE_X, HP_POS_Y + HP_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	VertexWkhp[0].rhw =
	VertexWkhp[1].rhw =
	VertexWkhp[2].rhw =
	VertexWkhp[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	VertexWkhp[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkhp[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkhp[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkhp[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	VertexWkhp[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkhp[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkhp[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkhp[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexGauge(float val)
{
	// ���_���W�̐ݒ�
	VertexWkGauge[0].vtx = D3DXVECTOR3(GAUGE00_POS_X, GAUGE00_POS_Y, 0.0f); 
	VertexWkGauge[1].vtx = D3DXVECTOR3(GAUGE00_POS_X + (GAUGE00_SIZE_X * val), GAUGE00_POS_Y, 0.0f);
	VertexWkGauge[2].vtx = D3DXVECTOR3(GAUGE00_POS_X, GAUGE00_POS_Y + GAUGE00_SIZE_Y, 0.0f);
	VertexWkGauge[3].vtx = D3DXVECTOR3(GAUGE00_POS_X + (GAUGE00_SIZE_X * val), GAUGE00_POS_Y + GAUGE00_SIZE_Y, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureGauge(float val)
{
	// �e�N�X�`�����W�̐ݒ�
	VertexWkGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkGauge[1].tex = D3DXVECTOR2(1.0f * val, 0.0f);
	VertexWkGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkGauge[3].tex = D3DXVECTOR2(1.0f * val, 1.0f);
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexGaugeFrame(void)
{
	// ���_���W�̐ݒ�
	VertexWkGauge[0].vtx = D3DXVECTOR3(GAUGEFRAME00_POS_X, GAUGEFRAME00_POS_Y, 0.0f);
	VertexWkGauge[1].vtx = D3DXVECTOR3(GAUGEFRAME00_POS_X + GAUGEFRAME00_SIZE_X, GAUGEFRAME00_POS_Y, 0.0f);
	VertexWkGauge[2].vtx = D3DXVECTOR3(GAUGEFRAME00_POS_X, GAUGEFRAME00_POS_Y + GAUGEFRAME00_SIZE_Y, 0.0f);
	VertexWkGauge[3].vtx = D3DXVECTOR3(GAUGEFRAME00_POS_X + GAUGEFRAME00_SIZE_X, GAUGEFRAME00_POS_Y + GAUGEFRAME00_SIZE_Y, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureGaugeFrame(void)
{
	// �e�N�X�`�����W�̐ݒ�
	VertexWkGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

