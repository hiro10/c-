//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "meshfield.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	LOOP_MAX		(500)		// �t���N�^���̉��Z��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 D3DTextureField;			// �e�N�X�`���ǂݍ��ݏꏊ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffField;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 D3DIdxBuffField;		// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX WorldField;					// ���[���h�}�g���b�N�X
D3DXVECTOR3 PosField;						// �|���S���\���ʒu�̒��S���W
D3DXVECTOR3 RotField;						// �|���S���̉�]�p

int NumBlockXField, NumBlockZField;			// �u���b�N��
int NumVertexField;							// �����_��	
int NumVertexIndexField;					// ���C���f�b�N�X��
int NumPolygonField;						// ���|���S����
float BlockSizeXField, BlockSizeZField;		// �u���b�N�T�C�Y
float Xrand, Zrand, Rrand;
float ValField;
float xMax, zMax, rMax;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	xMax = (fBlockSizeX * nNumBlockX) / 2;
	zMax = (fBlockSizeX * nNumBlockX) / 2;
	rMax = 100;

	ValField = (fBlockSizeX * nNumBlockX);

	// �|���S���\���ʒu�̒��S���W��ݒ�
	PosField = pos;

	RotField = rot;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_BOSSFIELD,		// �t�@�C���̖��O
								&D3DTextureField);	// �ǂݍ��ރ������[

	// �u���b�N���̐ݒ�
	NumBlockXField = nNumBlockX;
	NumBlockZField = nNumBlockZ;

	// ���_���̐ݒ�
	NumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	NumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	NumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	BlockSizeXField = fBlockSizeX;
	BlockSizeZField = fBlockSizeZ;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NumVertexField,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffField,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
    if(FAILED(Device->CreateIndexBuffer(sizeof(WORD) * NumVertexIndexField,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DIdxBuffField,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;
#if 0
		const float TexSizeX = 1.0f / NumBlockX;
		const float TexSizeZ = 1.0f / NumBlockZ;
#else
		const float TexSizeX = 1.0f;
		const float TexSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffField->Lock( 0, 0, (void**)&Vtx, 0 );

		for(int CntVtxZ = 0; CntVtxZ < (NumBlockZField + 1); CntVtxZ++)
		{
			for(int CntVtxX = 0; CntVtxX < (NumBlockXField + 1); CntVtxX++)
			{
				// ���_���W�̐ݒ�
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.x = -(NumBlockXField / 2.0f) * BlockSizeXField + CntVtxX * BlockSizeXField;
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.y = 0.0f;
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.z = (NumBlockZField / 2.0f) * BlockSizeZField - CntVtxZ * BlockSizeZField;

				// �@���̐ݒ�
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// ���ˌ��̐ݒ�
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].tex.x = TexSizeX * CntVtxX;
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].tex.y = TexSizeZ * CntVtxZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffField->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *Idx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DIdxBuffField->Lock( 0, 0, (void**)&Idx, 0 );

		int CntIdx = 0;
		for(int CntVtxZ = 0; CntVtxZ < NumBlockZField; CntVtxZ++)
		{
			if(CntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				Idx[CntIdx] = (CntVtxZ + 1) * (NumBlockXField + 1);
				CntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (NumBlockXField + 1); nCntVtxX++)
			{
				Idx[CntIdx] = (CntVtxZ + 1) * (NumBlockXField + 1) + nCntVtxX;
				CntIdx++;
				Idx[CntIdx] = CntVtxZ * (NumBlockXField + 1) + nCntVtxX;
				CntIdx++;
			}

			if(CntVtxZ < (NumBlockZField - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				Idx[CntIdx] = CntVtxZ * (NumBlockXField + 1) + NumBlockXField;
				CntIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		D3DIdxBuffField->Unlock();
	}

	for (int i = 0; i < LOOP_MAX; i++)
	{
		Xrand = (float)(rand() % (int)(ValField)) - xMax;
		Zrand = (float)(rand() % (int)(ValField)) - zMax;
		Rrand = (float)(rand() % 200 * (D3DX_PI)) / 100.0f;

		D3DXVECTOR3 VecRand, VecMesh, Cross;
		D3DXVECTOR3 Tmp;
		VERTEX_3D	*Vtx;
		D3DVtxBuffField->Lock(0, 0, (void**)&Vtx, 0);

		VecRand.x = cosf(Rrand);
		VecRand.z = sinf(Rrand);
		VecRand.y = 0.0f;

		for (int z = 0; z < (NumBlockZField + 1); z++)
		{
			for (int x = 0; x < (NumBlockXField + 1); x++)
			{
				Tmp.x = Vtx[z*(NumBlockXField + 1) + x].vtx.x;
				Tmp.z = Vtx[z*(NumBlockXField + 1) + x].vtx.z;

				VecMesh.x = Tmp.x - Xrand;
				VecMesh.z = Tmp.z - Zrand;
				VecMesh.y = 0.0f;

				D3DXVec3Cross(&Cross, &VecRand, &VecMesh);

				if (Cross.y > 0.0f)
				{
					Vtx[z*(NumBlockXField + 1) + x].vtx.y += 30.0f;
				}
				else
				{
					Vtx[z*(NumBlockXField + 1) + x].vtx.y -= 50.0f;
				}
			}
		}
		D3DIdxBuffField->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	if(D3DVtxBuffField!=NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffField->Release();
		D3DVtxBuffField = NULL;
	}

	if(D3DIdxBuffField != NULL)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		D3DIdxBuffField->Release();
		D3DIdxBuffField = NULL;
	}

	if(D3DTextureField != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureField->Release();
		D3DTextureField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DXMATRIX MtxRot, MtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&WorldField);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&MtxRot, RotField.y, RotField.x, RotField.z);
	D3DXMatrixMultiply(&WorldField, &WorldField, &MtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&MtxTranslate, PosField.x, PosField.y, PosField.z);
	D3DXMatrixMultiply(&WorldField, &WorldField, &MtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &WorldField);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	Device->SetStreamSource(0, D3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	Device->SetIndices(D3DIdxBuffField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureField);

	// �|���S���̕`��
	Device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertexField, 0, NumPolygonField);
}

