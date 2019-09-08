//=============================================================================
//
// �e���� [shadow.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "shadow.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureShadow = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffShadow = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

static D3DXMATRIX				MtxWorldShadow;			// ���[���h�}�g���b�N�X

SHADOW					shadow[MAX_SHADOW];			// �e���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexShadow(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_SHADOW,				// �t�@�C���̖��O
								&D3DTextureShadow);		// �ǂݍ��ރ������[

	for(int CntShadow = 0; CntShadow < MAX_SHADOW; CntShadow++)
	{
		shadow[CntShadow].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		shadow[CntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		shadow[CntShadow].use = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	if(D3DTextureShadow != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureShadow->Release();
		D3DTextureShadow = NULL;
	}

	if(D3DVtxBuffShadow != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffShadow->Release();
		D3DVtxBuffShadow = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxTranslate;
#if 0
	D3DXMATRIX mtxRot;
	D3DXQUATERNION quat;
	D3DXVECTOR3 vecUpObj, vecUpField, outVec;
	float fDotProduct, fRot;
#endif

	// ���Z����
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);		// ���� = �]����(DEST) - �]����(SRC)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	for (int CntShadow = 0; CntShadow < MAX_SHADOW; CntShadow++)
	{
		if (shadow[CntShadow].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&MtxWorldShadow);

#if 0
			// ��]�𔽉f
			D3DXQuaternionIdentity(&quat);

			vecUpObj = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// �I�u�W�F�N�g�̏����
			vecUpField = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// �n�ʂ̖@��

			// �I�u�W�F�N�g�̏�����ƒn�ʂ̖@���̊O�ς����]�������߂�
			D3DXVec3Cross(&outVec, &vecUpObj, &vecUpField);

			// �I�u�W�F�N�g�̏�����ƒn�ʂ̖@���̂Ȃ��p�����߂�
			fDotProduct = D3DXVec3Dot(&vecUpObj, &vecUpField);
			fRot = acosf(fDotProduct / (sqrtf(vecUpObj.x * vecUpObj.x + vecUpObj.y * vecUpObj.y + vecUpObj.z * vecUpObj.z)
				* sqrtf(vecUpField.x * vecUpField.x + vecUpField.y * vecUpField.y + vecUpField.z * vecUpField.z)));

			// ��]���ƂȂ��p����N�H�[�^�j�I�������߁A��]�}�g���b�N�X���Z�o
			D3DXQuaternionRotationAxis(&quat, &outVec, fRot);
			D3DXMatrixRotationQuaternion(&mtxRot, &quat);
			D3DXMatrixMultiply(&MtxWorldShadow, &MtxWorldShadow, &mtxRot);
#endif

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, shadow[CntShadow].pos.x, shadow[CntShadow].pos.y, shadow[CntShadow].pos.z);
			D3DXMatrixMultiply(&MtxWorldShadow, &MtxWorldShadow, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			Device->SetTransform(D3DTS_WORLD, &MtxWorldShadow);

			// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
			Device->SetStreamSource(0, D3DVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureShadow);

			// �|���S���̕`��
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntShadow * 4), NUM_POLYGON);
		}
	}

	// �ʏ�u�����h
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);				// ���� = �]����(SRC) + �]����(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_SHADOW,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffShadow,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffShadow->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntShadow = 0; CntShadow < MAX_SHADOW; CntShadow++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(-SHADOW_SIZE_X / 2,0.0f, SHADOW_SIZE_Z / 2);
			Vtx[1].vtx = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
			Vtx[2].vtx = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);
			Vtx[3].vtx = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);

			// �@���̐ݒ�
			Vtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			Vtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			Vtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			Vtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���ˌ��̐ݒ�
			Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			// �e�N�X�`�����W�̐ݒ�
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffShadow->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexShadow(int IdxShadow,int s, float SizeX, float SizeY, float SizeZ)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;
		
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffShadow->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxShadow * 4);

		if (s == 0)
		{
			// ���_���W�̐ݒ�
			Vtx[1].vtx = D3DXVECTOR3(0, SizeY / 2, SizeZ / 2);
			Vtx[0].vtx = D3DXVECTOR3(0, -SizeY / 2, SizeZ / 2);
			Vtx[3].vtx = D3DXVECTOR3(0, SizeY / 2, -SizeZ / 2);
			Vtx[2].vtx = D3DXVECTOR3(0, -SizeY / 2, -SizeZ / 2);
		}

		 if (s == 1)
		{
			// ���_���W�̐ݒ�
			Vtx[1].vtx = D3DXVECTOR3(0, -SizeY / 2, SizeZ / 2);
			Vtx[0].vtx = D3DXVECTOR3(0, SizeY / 2, SizeZ / 2);
			Vtx[3].vtx = D3DXVECTOR3(0, -SizeY / 2, -SizeZ / 2);
			Vtx[2].vtx = D3DXVECTOR3(0, SizeY / 2, -SizeZ / 2);
		}


		 if (s == 2)
		 {
			 // ���_���W�̐ݒ�
			 Vtx[0].vtx = D3DXVECTOR3(SizeX / 2, 0.0f, SizeZ / 2);
			 Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, 0.0f, SizeZ / 2);
			 Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, 0.0f, -SizeZ / 2);
			 Vtx[3].vtx = D3DXVECTOR3(-SizeX / 2, 0.0f, -SizeZ / 2);

		 }

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffShadow->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorShadow(int IdxShadow, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffShadow->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxShadow * 4);

		// ���_���W�̐ݒ�
		Vtx[0].diffuse =
		Vtx[1].diffuse =
		Vtx[2].diffuse =
		Vtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffShadow->Unlock();
	}
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, float SizeX,float SizeY, float SizeZ)
{
	int nIdxShadow = -1;
	
	for(int CntShadow = 0; CntShadow < MAX_SHADOW; CntShadow++)
	{
		if(!shadow[CntShadow].use)
		{
			shadow[CntShadow].pos = pos;
			shadow[CntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			shadow[CntShadow].use = true;

			//SetVertexShadow(CntShadow,s, SizeX, SizeY, SizeZ);

			nIdxShadow = CntShadow;
			break;
		}
	}

	return nIdxShadow;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void SetPositionShadow(int IdxShadow, D3DXVECTOR3 pos)
{
	for (int nIdxShadow = 0; nIdxShadow < MAX_SHADOW; nIdxShadow++)
	{
		shadow[nIdxShadow].pos = pos;
	}
}

//=============================================================================
// �e�̍폜
//=============================================================================
void DeleteShadow(int IdxShadow)
{
	if(IdxShadow >= 0 && IdxShadow < MAX_SHADOW)
	{
		shadow[IdxShadow].use = false;
	}
}
