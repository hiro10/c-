//=============================================================================
//
// �w�i���� [mpgauge.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "mpgauge.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexMpGauge(void);
void SetVertexMpGauge(float val);
void SetTextureMpGauge(float val);
void SetVertexMpGaugeFrame(void);
void SetTextureMpGaugeFrame(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureMpGauge = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTextureMpGaugeFrame = NULL;	// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				VertexWkMpGauge[NUM_VERTEX];	// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMpGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_MPGAUGE00,						// �t�@�C���̖��O
		&D3DTextureMpGauge);							// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_MPGAUGEFRAME00,					// �t�@�C���̖��O
		&D3DTextureMpGaugeFrame);						// �ǂݍ��ރ������[

	

	// ���_���̍쐬
	MakeVertexMpGauge();

	SetVertexMpGauge(100.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMpGauge(void)
{
	if (D3DTextureMpGauge != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureMpGauge->Release();
		D3DTextureMpGauge = NULL;
	}

	if (D3DTextureMpGaugeFrame != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureMpGaugeFrame->Release();
		D3DTextureMpGaugeFrame = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMpGauge(void)
{
	// ���t���[�����s���鏈��
	Player *player = GetPlayer();
	
	// �v���C���[�̌��݂�HP/�v���C���[��HP�̍ő�l
	float val  = player->mp / (float)(100);

	SetVertexMpGauge(val);
	SetTextureMpGauge(val);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMpGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	

	//���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureMpGauge);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkMpGauge, sizeof(VERTEX_2D));

	SetVertexMpGaugeFrame();
	SetTextureMpGaugeFrame();

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureMpGaugeFrame);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkMpGauge, sizeof(VERTEX_2D));

	}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexMpGauge(void)
{
	// ���_���W�̐ݒ�
	VertexWkMpGauge[0].vtx = D3DXVECTOR3(MPGAUGE00_POS_X, MPGAUGE00_POS_Y, 0.0f);
	VertexWkMpGauge[1].vtx = D3DXVECTOR3(MPGAUGE00_POS_X + MPGAUGE00_SIZE_X, MPGAUGE00_POS_Y, 0.0f);
	VertexWkMpGauge[2].vtx = D3DXVECTOR3(MPGAUGE00_POS_X, MPGAUGE00_POS_Y + MPGAUGE00_SIZE_Y, 0.0f);
	VertexWkMpGauge[3].vtx = D3DXVECTOR3(MPGAUGE00_POS_X + MPGAUGE00_SIZE_X, MPGAUGE00_POS_Y + MPGAUGE00_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	VertexWkMpGauge[0].rhw =
	VertexWkMpGauge[1].rhw =
	VertexWkMpGauge[2].rhw =
	VertexWkMpGauge[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	VertexWkMpGauge[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkMpGauge[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkMpGauge[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkMpGauge[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	VertexWkMpGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkMpGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkMpGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkMpGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexMpGauge(float val)
{
	// ���_���W�̐ݒ�
	VertexWkMpGauge[0].vtx = D3DXVECTOR3(MPGAUGE00_POS_X, MPGAUGE00_POS_Y, 0.0f);
	VertexWkMpGauge[1].vtx = D3DXVECTOR3(MPGAUGE00_POS_X + (MPGAUGE00_SIZE_X * val), MPGAUGE00_POS_Y, 0.0f);
	VertexWkMpGauge[2].vtx = D3DXVECTOR3(MPGAUGE00_POS_X, MPGAUGE00_POS_Y + MPGAUGE00_SIZE_Y, 0.0f);
	VertexWkMpGauge[3].vtx = D3DXVECTOR3(MPGAUGE00_POS_X + (MPGAUGE00_SIZE_X * val), MPGAUGE00_POS_Y + MPGAUGE00_SIZE_Y, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureMpGauge(float val)
{
	// �e�N�X�`�����W�̐ݒ�
	VertexWkMpGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkMpGauge[1].tex = D3DXVECTOR2(1.0f * val, 0.0f);
	VertexWkMpGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkMpGauge[3].tex = D3DXVECTOR2(1.0f * val, 1.0f);
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexMpGaugeFrame(void)
{
	// ���_���W�̐ݒ�
	VertexWkMpGauge[0].vtx = D3DXVECTOR3(MPGAUGEFRAME00_POS_X, MPGAUGEFRAME00_POS_Y, 0.0f);
	VertexWkMpGauge[1].vtx = D3DXVECTOR3(MPGAUGEFRAME00_POS_X + MPGAUGEFRAME00_SIZE_X, MPGAUGEFRAME00_POS_Y, 0.0f);
	VertexWkMpGauge[2].vtx = D3DXVECTOR3(MPGAUGEFRAME00_POS_X, MPGAUGEFRAME00_POS_Y + MPGAUGEFRAME00_SIZE_Y, 0.0f);
	VertexWkMpGauge[3].vtx = D3DXVECTOR3(MPGAUGEFRAME00_POS_X + MPGAUGEFRAME00_SIZE_X, MPGAUGEFRAME00_POS_Y + MPGAUGEFRAME00_SIZE_Y, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureMpGaugeFrame(void)
{
	// �e�N�X�`�����W�̐ݒ�
	VertexWkMpGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkMpGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkMpGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkMpGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

