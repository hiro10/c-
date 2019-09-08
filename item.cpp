//=============================================================================
//
// �A�C�e������ [item.cpp](�@���j
// Author : �����@�C�O
//
//=============================================================================
#include "item.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureItem[ITEMTYPE_MAX];	// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			MeshItem[ITEMTYPE_MAX];			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXMatBuffItem[ITEMTYPE_MAX];	// ���b�V���̃}�e���A�������i�[
DWORD				NumMatItem[ITEMTYPE_MAX];		// �������̑���

D3DXMATRIX			MtxWorldItem;				// ���[���h�}�g���b�N�X

Item				item[MAX_ITEM];			// �A�C�e�����[�N

const char *FileNameItem[ITEMTYPE_MAX] =
{
	"data/MODEL/item000.x",		// �@��
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	for(int CntItemType = 0; CntItemType < ITEMTYPE_MAX; CntItemType++)
	{
		D3DTextureItem[CntItemType] = NULL;
		MeshItem[CntItemType] = NULL;
		D3DXMatBuffItem[CntItemType] = NULL;

		// X�t�@�C���̓ǂݍ���
		if(FAILED(D3DXLoadMeshFromX(FileNameItem[CntItemType],
									D3DXMESH_SYSTEMMEM,
									Device,
									NULL,
									&D3DXMatBuffItem[CntItemType],
									NULL,
									&NumMatItem[CntItemType],
									&MeshItem[CntItemType])))
		{
			return E_FAIL;
		}

#if 0
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(Device,									// �f�o�C�X�ւ̃|�C���^
									TEXTURE_FILENAME,						// �t�@�C���̖��O
									&D3DTextureModelItem[CntItemType]);	// �ǂݍ��ރ������[
#endif
	}

	for(int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
	{
		item[CntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		item[CntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		item[CntItem].radius = 0.0f;
		item[CntItem].IdxShadow = -1;
		item[CntItem].Type = ITEMTYPE_COIN;
		item[CntItem].use = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	for(int CntItemType = 0; CntItemType < ITEMTYPE_MAX; CntItemType++)
	{
		if(D3DTextureItem[CntItemType] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureItem[CntItemType]->Release();
			D3DTextureItem[CntItemType] = NULL;
		}

		if(MeshItem[CntItemType] != NULL)
		{// ���b�V���̊J��
			MeshItem[CntItemType]->Release();
			MeshItem[CntItemType] = NULL;
		}

		if(D3DXMatBuffItem[CntItemType] != NULL)
		{// �}�e���A���̊J��
			D3DXMatBuffItem[CntItemType]->Release();
			D3DXMatBuffItem[CntItemType] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{

	for(int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
	{
		if(item[CntItem].use)
		{
			item[CntItem].pos.z -= 50.0f;
		}

		if (item[CntItem].pos.z - GetPositionPlayer().z <= -5.0f)
		{
			item[CntItem].use = false;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	for(int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
	{
		if(item[CntItem].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&MtxWorldItem);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, item[CntItem].rot.y, item[CntItem].rot.x, item[CntItem].rot.z);
			D3DXMatrixMultiply(&MtxWorldItem, &MtxWorldItem, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, item[CntItem].pos.x, item[CntItem].pos.y, item[CntItem].pos.z);
			D3DXMatrixMultiply(&MtxWorldItem, &MtxWorldItem, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			Device->SetTransform(D3DTS_WORLD, &MtxWorldItem);

			D3DXMATERIAL *pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffItem[item[CntItem].Type]->GetBufferPointer();

			for(int CntMat = 0; CntMat < (int)NumMatItem[item[CntItem].Type]; CntMat++)
			{
				// �}�e���A���̐ݒ�
				Device->SetMaterial(&pD3DXMat[CntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				Device->SetTexture(0, D3DTextureItem[item[CntItem].Type]);

				// �`��
				MeshItem[item[CntItem].Type]->DrawSubset(CntMat);
			}
		}
	}

	{// �}�e���A�����f�t�H���g�ɖ߂�
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		Device->SetMaterial(&mat.MatD3D);
	}
}

//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for(int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
	{
		if(!item[CntItem].use)
		{
			item[CntItem].pos = pos;
			item[CntItem].rot = rot;
			item[CntItem].radius = ITEM_RADIUS;
			item[CntItem].Type = nType;
			item[CntItem].use = true;
			break;
		}
	}
}

//=============================================================================
// �A�C�e���̍폜
//=============================================================================
void DeleteItem(int IdxItem)
{
	if(IdxItem >= 0 && IdxItem < MAX_ITEM)
	{
		
		item[IdxItem].use = false;
	}
}

//=============================================================================
// �A�C�e���̎擾
//=============================================================================
Item *GetItem(void)
{
	return &item[0];
}
