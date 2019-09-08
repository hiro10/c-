//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "score.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 Device);
void SetTextureScore(int idx, int number);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureScore[2] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffScore = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXVECTOR3				PosScore;						// �ʒu
D3DXVECTOR3				RotScore;						// ����

int						Score;						// �X�R�A

//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	PosScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �X�R�A�̏�����
	Score = 0;

	// ���_���̍쐬
	MakeVertexScore(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_SCORE,			// �t�@�C���̖��O
								&D3DTextureScore[0]);	// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FRAME_SCORE,		// �t�@�C���̖��O
								&D3DTextureScore[1]);	// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	if(D3DTextureScore[0] != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureScore[0]->Release();
		D3DTextureScore[0] = NULL;
	}

	if(D3DTextureScore[1] != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureScore[1]->Release();
		D3DTextureScore[1] = NULL;
	}

	if(D3DVtxBuffScore != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffScore->Release();
		D3DVtxBuffScore = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	for(int CntPlace = 0; CntPlace < NUM_SCORE; CntPlace++)
	{
		int number;
		
		number = (Score % (int)(powf(10.0f, (float)(NUM_SCORE - CntPlace)))) / (int)(powf(10.0f, (float)(NUM_SCORE - CntPlace - 1)));
		SetTextureScore(CntPlace, number);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    Device->SetStreamSource(0, D3DVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureScore[0]);

	// �|���S���̕`��
	for(int CntPlace = 0; CntPlace < NUM_SCORE; CntPlace++)
	{
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntPlace * 4), NUM_POLYGON);
	}

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureScore[1]);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_SCORE * 4), NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_SCORE + 4),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,								// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,									// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,								// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffScore,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))											// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffScore->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntPlace = 0; CntPlace < NUM_SCORE; CntPlace++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(SCORE_POS_X + CntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(SCORE_POS_X + CntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(SCORE_POS_X + CntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y + SCORE_SIZE_Y, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + CntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y + SCORE_SIZE_Y, 0.0f);

			// rhw�̐ݒ�
			Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			Vtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(SCORE_POS_X - 15, SCORE_POS_Y-25, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(SCORE_POS_X + (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_SCORE + 15, SCORE_POS_Y-25, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(SCORE_POS_X - 15, SCORE_POS_Y + 55, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_SCORE + 15, SCORE_POS_Y + 55, 0.0f);

			// rhw�̐ݒ�
			Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			Vtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffScore->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureScore(int idx, int number)
{
	VERTEX_2D *Vtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffScore->Lock(0, 0, (void**)&Vtx, 0);

	Vtx += (idx * 4);

	// ���_���W�̐ݒ�
	Vtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	Vtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	Vtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	Vtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffScore->Unlock();
}

//=============================================================================
// �X�R�A�̕ύX
//=============================================================================
void ChangeScore(int value)
{
	// �X�R�A��0��艺�ɂȂ�Ȃ��悤��
	if(Score < 0)
	{
		Score = 0;
	}
	else if(Score >= (int)(powf(10.0f, (float)(NUM_SCORE + 1))))
	{
		
		Score = (int)(powf(10.0f, (float)(NUM_SCORE + 1))) - 1;
	}
	Score += value;
}

