//=============================================================================
//
// �����׏��� [wallleft.cpp]
// Author :  �����@�C�O
//
//=============================================================================
#include "wallleft.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexWallleft(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureWallleft = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffWallleft = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				MtxWorldWallleft;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				PosWallleft;				// ���݂̈ʒu
D3DXVECTOR3				RotWallleft;				// ���݂̌���
static float offset = 0.0f;							// �X�N���[���p
//=============================================================================
// ����������
//=============================================================================
HRESULT InitWallleft(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	HRESULT hr;

	// �ʒu�A�����̏����ݒ�
	PosWallleft = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotWallleft = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_WALLLEFT,							// �t�@�C���̖��O
		&D3DTextureWallleft);						// �ǂݍ��ރ������[

	// ���_���̍쐬
	hr = MakeVertexWallleft(Device);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWallleft(void)
{
	if (D3DTextureWallleft != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureWallleft->Release();
		D3DTextureWallleft = NULL;
	}

	if (D3DVtxBuffWallleft != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffWallleft->Release();
		D3DVtxBuffWallleft = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWallleft(void)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtxl;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffWallleft->Lock(0, 0, (void**)&Vtxl, 0);

	//�e�N�X�`���X�N���\���̃X�s�[�h
	offset -= SCROLL_SPEED_LEFT;

	//1.0��艺�ɂȂ�����
	if (offset < 1.0f)
	{
		// �e�N�X�`�����W�̐ݒ�
		Vtxl[0].tex = D3DXVECTOR2(offset, 0.0f);
		Vtxl[1].tex = D3DXVECTOR2(offset, 1.0f);
		Vtxl[2].tex = D3DXVECTOR2(offset + 1.0f, 0.0f);
		Vtxl[3].tex = D3DXVECTOR2(offset + 1.0f, 1.0f);
	}

	if (offset <= -1.0f)
	{//-1.0�ȉ��ɂȂ�����0�ɖ߂�
		offset = 0.0f;
	}

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffWallleft->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWallleft(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxRot, MtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MtxWorldWallleft);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&MtxRot, RotWallleft.y, RotWallleft.x, RotWallleft.z);
	D3DXMatrixMultiply(&MtxWorldWallleft, &MtxWorldWallleft, &MtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&MtxTranslate, PosWallleft.x, PosWallleft.y, PosWallleft.z);
	D3DXMatrixMultiply(&MtxWorldWallleft, &MtxWorldWallleft, &MtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &MtxWorldWallleft);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	Device->SetStreamSource(0, D3DVtxBuffWallleft, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureWallleft);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexWallleft(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffWallleft,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	//{//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtxl;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffWallleft->Lock(0, 0, (void**)&Vtxl, 0);

	// ���_���W�̐ݒ�
	Vtxl[1].vtx = D3DXVECTOR3(-WALLLEFE_VTX_X, -WALLLEFE_VTX_Y, WALLLEFE_VTX_Z);
	Vtxl[0].vtx = D3DXVECTOR3(-WALLLEFE_VTX_X, WALLLEFE_VTX_Y, WALLLEFE_VTX_Z);
	Vtxl[3].vtx = D3DXVECTOR3(-WALLLEFE_VTX_X, -WALLLEFE_VTX_Y, -WALLLEFE_VTX_Z);
	Vtxl[2].vtx = D3DXVECTOR3(-WALLLEFE_VTX_X, WALLLEFE_VTX_Y, -WALLLEFE_VTX_Z);

	// �@���x�N�g���̐ݒ�
	Vtxl[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxl[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxl[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxl[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���ˌ��̐ݒ�
	Vtxl[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxl[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxl[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxl[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	Vtxl[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Vtxl[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Vtxl[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Vtxl[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffWallleft->Unlock();
	//}

	return S_OK;
}

