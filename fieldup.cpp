//=============================================================================
//
// �V�䏈�� [fieldup.cpp]
// Author :  �����@�C�O
//
//=============================================================================
#include "fieldup.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexFieldup(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureFieldup = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffFieldup = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				MtxWorldFieldup;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				PosFieldup;					// ���݂̈ʒu
D3DXVECTOR3				RotFieldup;					// ���݂̌���
static float offset = 0.0f;							// �X�N���[���p
//=============================================================================
// ����������
//=============================================================================
HRESULT InitFieldup(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	HRESULT hr;

	// �ʒu�A�����̏����ݒ�
	PosFieldup = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotFieldup = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_WALLRIGHT,								// �t�@�C���̖��O
		&D3DTextureFieldup);							// �ǂݍ��ރ������[

	// ���_���̍쐬
	hr = MakeVertexFieldup(Device);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFieldup(void)
{
	if (D3DTextureFieldup != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureFieldup->Release();
		D3DTextureFieldup = NULL;
	}

	if (D3DVtxBuffFieldup != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffFieldup->Release();
		D3DVtxBuffFieldup = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFieldup(void)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtxr;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffFieldup->Lock(0, 0, (void**)&Vtxr, 0);

	offset -= FIELDUP_SCROLL_SPEED;

	if (offset < 1.0f)
	{
		// �e�N�X�`�����W�̐ݒ�
		Vtxr[0].tex = D3DXVECTOR2(offset, 0.0f);
		Vtxr[1].tex = D3DXVECTOR2(offset, 1.0f);
		Vtxr[2].tex = D3DXVECTOR2(offset + 1.0f, 0.0f);
		Vtxr[3].tex = D3DXVECTOR2(offset + 1.0f, 1.0f);
	}

	if (offset <= -1.0f)
	{// offset��-1�ȉ��Ȃ�0�ɖ߂�
		offset = 0.0f;
	}

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffFieldup->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFieldup(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MtxWorldFieldup);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, RotFieldup.y, RotFieldup.x, RotFieldup.z);
	D3DXMatrixMultiply(&MtxWorldFieldup, &MtxWorldFieldup, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, PosFieldup.x, PosFieldup.y, PosFieldup.z);
	D3DXMatrixMultiply(&MtxWorldFieldup, &MtxWorldFieldup, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &MtxWorldFieldup);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	Device->SetStreamSource(0, D3DVtxBuffFieldup, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureFieldup);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexFieldup(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffFieldup,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	//{//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtxr;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffFieldup->Lock(0, 0, (void**)&Vtxr, 0);

	//hidari
	Vtxr[1].vtx = D3DXVECTOR3(-860.0f, 620.0f, 24100.0f);
	Vtxr[0].vtx = D3DXVECTOR3(860.0f, 620.0f, 24100.0f);
	Vtxr[3].vtx = D3DXVECTOR3(-860.0f, 620.0f, -14100.0f);
	Vtxr[2].vtx = D3DXVECTOR3(860.0f, 620.0f, -14100.0f);

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
	D3DVtxBuffFieldup->Unlock();


	return S_OK;
}