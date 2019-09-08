//=============================================================================
//
// �r���{�[�h���� [billboard.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "billboard.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "enemy.h"
#include "boss.h"
#include "main.h"
#include "ace.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 Device);
void SetVertexBillboard(float fSizeX, float fSizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBillboard = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBillboard = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX				MtxWorldBillboard;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				PosBillboard;				// �ʒu
D3DXVECTOR3				SclBillboard;				// �X�P�[��
D3DXVECTOR3				RotBillboard;				// ��]
D3DXVECTOR3				MoveBillboard;				// �ړ���
int						IdxShadowBillboard;			// �eID
bool					BillboardUse;				// �g�p�A���g�p����
static bool				AlpaTest;					// �A���t�@�e�X�gON/OFF
static int				Alpha;						// �A���t�@�e�X�g��臒l


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBillboard(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Player*player;
	player = GetPlayer();

	// ���_���̍쐬
	MakeVertexBillboard(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_BILLBOARD,			// �t�@�C���̖��O
								&D3DTextureBillboard);		// �ǂݍ��ރ������[

	PosBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SclBillboard = D3DXVECTOR3(16.0f, 13.0f, 13.0f);
	RotBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MoveBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�̐ݒ�
	SetColorShadow(IdxShadowBillboard, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	// �r���{�[�h���g�p��
	BillboardUse =true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBillboard(void)
{
	if(D3DTextureBillboard != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureBillboard->Release();
		D3DTextureBillboard = NULL;
	}

	if(D3DVtxBuffBillboard != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffBillboard->Release();
		D3DVtxBuffBillboard = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBillboard(void)
{
	// �G�[�X�̎擾
	Ace*ace;
	ace = GetAce();

	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();

	if (BillboardUse == true)
	{
		if (GetMode() == MODE_GAME)
		{//���[�h��1�̎�
			PosBillboard = GetAcePos();
		}

		if (GetMode() == MODE_BOSS)
		{//�{�X��̎�
			PosBillboard = GetBossPos();
			if (boss->use == false)
			{// �{�X�����ꂽ�疢�g�p��
				BillboardUse = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxView, MtxScale, MtxTranslate;
	if (BillboardUse == true)
	{
		// ���C���e�B���O�𖳌��ɂ���
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �r���[�}�g���b�N�X���擾
		MtxView = GetMtxView();

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&MtxWorldBillboard);

		// �|���S���𐳖ʂɌ�����
		MtxWorldBillboard._11 = MtxView._11;
		MtxWorldBillboard._21 = MtxView._12;
		MtxWorldBillboard._31 = MtxView._13;
		MtxWorldBillboard._12 = MtxView._21;
		MtxWorldBillboard._22 = MtxView._22;
		MtxWorldBillboard._32 = MtxView._23;
		MtxWorldBillboard._13 = MtxView._31;
		MtxWorldBillboard._23 = MtxView._32;
		MtxWorldBillboard._33 = MtxView._33;

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&MtxScale, SclBillboard.x,
			SclBillboard.y,
			SclBillboard.z);
		D3DXMatrixMultiply(&MtxWorldBillboard,
			&MtxWorldBillboard, &MtxScale);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&MtxTranslate, PosBillboard.x,
			PosBillboard.y,
			PosBillboard.z);
		D3DXMatrixMultiply(&MtxWorldBillboard,
			&MtxWorldBillboard, &MtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		Device->SetTransform(D3DTS_WORLD, &MtxWorldBillboard);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		Device->SetStreamSource(0, D3DVtxBuffBillboard, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, D3DTextureBillboard);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ���C���e�B���O��L���ɂ���
		Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}
//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffBillboard,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBillboard->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		Vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		Vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		Vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		Vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBillboard->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBillboard(float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBillboard->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBillboard->Unlock();
	}
}
