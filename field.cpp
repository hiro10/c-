//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author : �����C�O
//
//=============================================================================
#include "field.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 Device);
HRESULT MakeVertexSpace(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureLoad = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTextureSpace = NULL;		// �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffLoad = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffSpace = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXMATRIX				mtxWorldLoad;				// ���[���h�}�g���b�N�X
D3DXVECTOR3				posLoad;					// ���݂̈ʒu
D3DXVECTOR3				rotLoad;					// ���݂̌���

D3DXMATRIX				g_mtxWorldSpace;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				g_posSpace;					// ���݂̈ʒu
D3DXVECTOR3				g_rotSpace;					// ���݂̌���

static float offset = 0.0f;							// �X�N���[���p
float s_offset = 0.0f;								// �X�N���[���p				

//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice(); 
	HRESULT hr;
	HRESULT sr;

	// �ʒu�A�����̏����ݒ�
	posLoad = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotLoad = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FIELD,									// �t�@�C���̖��O
		&D3DTextureLoad);								// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_SPACE,									// �t�@�C���̖��O
		&D3DTextureSpace);								// �ǂݍ��ރ������[

	// ���_���̍쐬
	hr = MakeVertexField(Device);
	sr = MakeVertexSpace(Device);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	if(D3DTextureLoad != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureLoad->Release();
		D3DTextureLoad = NULL;
	}

	if (D3DTextureSpace != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureSpace->Release();
		D3DTextureSpace = NULL;
	}

	if(D3DVtxBuffLoad != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffLoad->Release();
		D3DVtxBuffLoad = NULL;
	}

	if (g_pD3DVtxBuffSpace != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffSpace->Release();
		g_pD3DVtxBuffSpace = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
	{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtx;
	VERTEX_3D *Vtxs;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffLoad->Lock(0, 0, (void**)&Vtx, 0);
	g_pD3DVtxBuffSpace->Lock(0, 0, (void**)&Vtxs, 0);

	//�@�X�N���[���X�s�[�h�̐ݒ�
	offset -= SCROLL_SPEED;				// �v��
	s_offset -= SPACE_SCROLL_SPEED;		// �F��

	// �v��
	if (offset < 1.0f)
	{
		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(offset, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(offset, 1.0f);
		Vtx[2].tex = D3DXVECTOR2(offset + 1.0f, 0.0f);
		Vtx[3].tex = D3DXVECTOR2(offset + 1.0f, 1.0f);

	}
	if (offset <= -1.0f)
	{
		// offset��0��
		offset = 0.0f;
	}

	// �F��
	if (s_offset < 1.0f)
	{

		// �e�N�X�`�����W�̐ݒ�
		Vtxs[0].tex = D3DXVECTOR2(s_offset, 0.0f);
		Vtxs[1].tex = D3DXVECTOR2(s_offset, 1.0f);
		Vtxs[2].tex = D3DXVECTOR2(s_offset + 1.0f, 0.0f);
		Vtxs[3].tex = D3DXVECTOR2(s_offset + 1.0f, 1.0f);

	}
	if (s_offset <= -1.0f)
	{
		// s_offset��0��
		s_offset = 0.0f;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice(); 
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXVECTOR3 eb_move_left;

	eb_move_left.x = 0.0;
	eb_move_left.y = 0.0f;
	eb_move_left.z -= VALUE_MOVE_EFFECTBULLET;


	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffLoad->Unlock();
	g_pD3DVtxBuffSpace->Unlock();


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldSpace);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotLoad.y, rotLoad.x, rotLoad.z);
	D3DXMatrixMultiply(&g_mtxWorldSpace, &g_mtxWorldSpace, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, posLoad.x, posLoad.y, posLoad.z);
	D3DXMatrixMultiply(&g_mtxWorldSpace, &g_mtxWorldSpace, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &mtxWorldLoad);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	Device->SetStreamSource(0, g_pD3DVtxBuffSpace, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureSpace);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorldLoad);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotLoad.y, rotLoad.x, rotLoad.z);
	D3DXMatrixMultiply(&mtxWorldLoad, &mtxWorldLoad, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, posLoad.x, posLoad.y, posLoad.z);
	D3DXMatrixMultiply(&mtxWorldLoad, &mtxWorldLoad, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &mtxWorldLoad);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	Device->SetStreamSource(0, D3DVtxBuffLoad, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureLoad);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffLoad,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffLoad->Lock(0, 0, (void**)&Vtx, 0);

	// ���_���W�̐ݒ�
	Vtx[0].vtx = D3DXVECTOR3(-900.0f, -130.0f, 11800.0f);
	Vtx[1].vtx = D3DXVECTOR3(900.0f, -130.0f, 11800.0f);
	Vtx[2].vtx = D3DXVECTOR3(-900.0f, -130.0f, -13400.0f);
	Vtx[3].vtx = D3DXVECTOR3(900.0f, -130.0f, -13400.0f);

	// �@���x�N�g���̐ݒ�
	Vtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���ˌ��̐ݒ�
	Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffLoad->Unlock();


	return S_OK;
}

//=============================================================================
// ���_�̍쐬(�񖇖ڂ̉F���j
//=============================================================================
HRESULT MakeVertexSpace(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffSpace,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *Vtxs;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffSpace->Lock(0, 0, (void**)&Vtxs, 0);

	// ���_���W�̐ݒ�
	Vtxs[0].vtx = D3DXVECTOR3(-900.0f, -140.0f, 14100.0f);
	Vtxs[1].vtx = D3DXVECTOR3(900, -140.0f, 14100.0f);
	Vtxs[2].vtx = D3DXVECTOR3(-900.0f, -140.0f, -14100.0f);
	Vtxs[3].vtx = D3DXVECTOR3(900.0f, -140.0f, -14100.0f);

	// �@���x�N�g���̐ݒ�
	Vtxs[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxs[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxs[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxs[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���ˌ��̐ݒ�
	Vtxs[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxs[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxs[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxs[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffSpace->Unlock();

	return S_OK;
}
