//=============================================================================
//
// �E�Ǐ��� [wallright.cpp]
// Author :  �����@�C�O
//
//=============================================================================
#include "wallright.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexWallright(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureWallright = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffWallright = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				MtxWorldWallright;				// ���[���h�}�g���b�N�X
D3DXVECTOR3				PosWallright;					// ���݂̈ʒu
D3DXVECTOR3				RotWallright;					// ���݂̌���
static float offset = 0.0f;								// �X�N���[���p
//=============================================================================
// ����������
//=============================================================================
HRESULT InitWallright(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	HRESULT hr;

	// �ʒu�A�����̏����ݒ�
	PosWallright = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotWallright = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_WALLRIGHT,								// �t�@�C���̖��O
		&D3DTextureWallright);							// �ǂݍ��ރ������[

	// ���_���̍쐬
	hr = MakeVertexWallright(Device);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWallright(void)
{
	if (D3DTextureWallright != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureWallright->Release();
		D3DTextureWallright = NULL;
	}

	if (D3DVtxBuffWallright != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffWallright->Release();
		D3DVtxBuffWallright = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWallright(void)
{
	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtxr;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffWallright->Lock(0, 0, (void**)&Vtxr, 0);

	//�e�N�X�`���p�̃X�N���[���X�s�[�h
	offset -= SCROLL_SPEED_RIGHT;

	if (offset < 1.0f)
	{
		// �e�N�X�`�����W�̐ݒ�
		Vtxr[0].tex = D3DXVECTOR2(offset, 0.0f);
		Vtxr[1].tex = D3DXVECTOR2(offset, 1.0f);
		Vtxr[2].tex = D3DXVECTOR2(offset + 1.0f, 0.0f);
		Vtxr[3].tex = D3DXVECTOR2(offset + 1.0f, 1.0f);
	}

	if (offset <= -1.0f)
	{
		// offset�̃��Z�b�g
		offset = 0.0f;
	}

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffWallright->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWallright(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxRot, MtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MtxWorldWallright);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&MtxRot, RotWallright.y, RotWallright.x, RotWallright.z);
	D3DXMatrixMultiply(&MtxWorldWallright, &MtxWorldWallright, &MtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&MtxTranslate, PosWallright.x, PosWallright.y, PosWallright.z);
	D3DXMatrixMultiply(&MtxWorldWallright, &MtxWorldWallright, &MtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &MtxWorldWallright);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	Device->SetStreamSource(0, D3DVtxBuffWallright, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureWallright);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexWallright(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffWallright,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtxr;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffWallright->Lock(0, 0, (void**)&Vtxr, 0);

	//hidari
	Vtxr[0].vtx = D3DXVECTOR3(WALLRIGNT_VTX_X, -WALLRIGNT_VTX_Y, WALLRIGNT_VTX_Z);
	Vtxr[1].vtx = D3DXVECTOR3(WALLRIGNT_VTX_X, WALLRIGNT_VTX_Y, WALLRIGNT_VTX_Z);
	Vtxr[2].vtx = D3DXVECTOR3(WALLRIGNT_VTX_X, -WALLRIGNT_VTX_Y, -WALLRIGNT_VTX_Z);
	Vtxr[3].vtx = D3DXVECTOR3(WALLRIGNT_VTX_X, WALLRIGNT_VTX_Y, -WALLRIGNT_VTX_Z);

	// �@���x�N�g���̐ݒ�
	Vtxr[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxr[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxr[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxr[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���ˌ��̐ݒ�
	Vtxr[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxr[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxr[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxr[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffWallright->Unlock();


	return S_OK;
}
