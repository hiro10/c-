//=============================================================================
//
// �{�X�̂g�o�Q�[�W���� [bossgauge.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "main.h"
#include "boss.h"
#include "bossgauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBossGauge(void);
void SetVertexBossGauge(float val);
void SetTextureBossGauge(float val);
void SetVertexBossGaugeFrame(void);
void SetTextureBossGaugeFrame(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBossGauge = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTextureBossGaugeFrame = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTexturebosshp = NULL;			// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				VertexWkbosshp[NUM_VERTEX];		// ���_���i�[���[�N
VERTEX_2D				VertexWkBossGauge[NUM_VERTEX];	// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBossGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_BOSSGAUGE00,		// �t�@�C���̖��O
		&D3DTextureBossGauge);			// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_BOSSGAUGEFRAME00,	// �t�@�C���̖��O
		&D3DTextureBossGaugeFrame);		// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,	// �f�o�C�X�̃|�C���^
		TEXTURE_BOSSHP1,				// �t�@�C���̖��O
		&D3DTexturebosshp);				// �ǂݍ��ރ������̃|�C���^

	// ���_���̍쐬
	MakeVertexBossGauge();

	SetVertexBossGauge(100.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossGauge(void)
{
	if (D3DTextureBossGauge != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureBossGauge->Release();
		D3DTextureBossGauge = NULL;
	}

	if (D3DTexturebosshp != NULL)
	{// �e�N�X�`���̊J��
		D3DTexturebosshp->Release();
		D3DTexturebosshp = NULL;
	}

	if (D3DTextureBossGaugeFrame != NULL)
	{//	�e�N�X�`���̊J��
		D3DTextureBossGaugeFrame->Release();
		D3DTextureBossGaugeFrame = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossGauge(void)
{
	// ���t���[�����s���鏈��
	Boss *boss = GetBoss();
	;
	float val = boss->hp / (float)(BOSS_LIFEMAX);

	SetVertexBossGauge(val);
	SetTextureBossGauge(val);

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Boss *boss = GetBoss();


	//���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureBossGauge);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkBossGauge, sizeof(VERTEX_2D));

	//�{�X�Q�[�W��SET
	SetVertexBossGaugeFrame();
	SetTextureBossGaugeFrame();

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureBossGaugeFrame);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkBossGauge, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�(TEXTURE_TITLE)
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�(TEXTURE_TITLE)
	Device->SetTexture(0, D3DTexturebosshp);

	// �|���S���̕`��(TEXTURE_TITLE)
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkbosshp, sizeof(VERTEX_2D));

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBossGauge(void)
{
	// ���_���W�̐ݒ�
	VertexWkBossGauge[0].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X, BOSSGAUGE00_POS_Y, 0.0f);
	VertexWkBossGauge[1].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X + BOSSGAUGE00_SIZE_X, BOSSGAUGE00_POS_Y, 0.0f);
	VertexWkBossGauge[2].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X, BOSSGAUGE00_POS_Y + BOSSGAUGE00_SIZE_Y, 0.0f);
	VertexWkBossGauge[3].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X + BOSSGAUGE00_SIZE_X, BOSSGAUGE00_POS_Y + BOSSGAUGE00_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	VertexWkBossGauge[0].rhw =
		VertexWkBossGauge[1].rhw =
		VertexWkBossGauge[2].rhw =
		VertexWkBossGauge[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	VertexWkBossGauge[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkBossGauge[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkBossGauge[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkBossGauge[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	VertexWkBossGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkBossGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkBossGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkBossGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	VertexWkbosshp[0].vtx = D3DXVECTOR3(BOSSHP1_POS_X, BOSSHP1_POS_Y, 0.0f);
	VertexWkbosshp[1].vtx = D3DXVECTOR3(BOSSHP1_POS_X + BOSSHP1_SIZE_X, BOSSHP1_POS_Y, 0.0f);
	VertexWkbosshp[2].vtx = D3DXVECTOR3(BOSSHP1_POS_X, BOSSHP1_POS_Y + BOSSHP1_SIZE_Y, 0.0f);
	VertexWkbosshp[3].vtx = D3DXVECTOR3(BOSSHP1_POS_X + BOSSHP1_SIZE_X, BOSSHP1_POS_Y + BOSSHP1_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	VertexWkbosshp[0].rhw =
		VertexWkbosshp[1].rhw =
		VertexWkbosshp[2].rhw =
		VertexWkbosshp[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	VertexWkbosshp[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkbosshp[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkbosshp[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkbosshp[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	VertexWkbosshp[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkbosshp[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkbosshp[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkbosshp[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBossGauge(float val)
{
	// ���_���W�̐ݒ�
	VertexWkBossGauge[0].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X, BOSSGAUGE00_POS_Y, 0.0f);
	VertexWkBossGauge[1].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X + (BOSSGAUGE00_SIZE_X * val), BOSSGAUGE00_POS_Y, 0.0f);
	VertexWkBossGauge[2].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X, BOSSGAUGE00_POS_Y + BOSSGAUGE00_SIZE_Y, 0.0f);
	VertexWkBossGauge[3].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X + (BOSSGAUGE00_SIZE_X * val), BOSSGAUGE00_POS_Y + BOSSGAUGE00_SIZE_Y, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureBossGauge(float val)
{
	// �e�N�X�`�����W�̐ݒ�
	VertexWkBossGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkBossGauge[1].tex = D3DXVECTOR2(1.0f * val, 0.0f);
	VertexWkBossGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkBossGauge[3].tex = D3DXVECTOR2(1.0f * val, 1.0f);
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBossGaugeFrame(void)
{
	// ���_���W�̐ݒ�
	VertexWkBossGauge[0].vtx = D3DXVECTOR3(BOSSGAUGEFRAME00_POS_X, BOSSGAUGEFRAME00_POS_Y, 0.0f);
	VertexWkBossGauge[1].vtx = D3DXVECTOR3(BOSSGAUGEFRAME00_POS_X + BOSSGAUGEFRAME00_SIZE_X, BOSSGAUGEFRAME00_POS_Y, 0.0f);
	VertexWkBossGauge[2].vtx = D3DXVECTOR3(BOSSGAUGEFRAME00_POS_X, BOSSGAUGEFRAME00_POS_Y + BOSSGAUGEFRAME00_SIZE_Y, 0.0f);
	VertexWkBossGauge[3].vtx = D3DXVECTOR3(BOSSGAUGEFRAME00_POS_X + BOSSGAUGEFRAME00_SIZE_X, BOSSGAUGEFRAME00_POS_Y + BOSSGAUGEFRAME00_SIZE_Y, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureBossGaugeFrame(void)
{
	// �e�N�X�`�����W�̐ݒ�
	VertexWkBossGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkBossGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkBossGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkBossGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

