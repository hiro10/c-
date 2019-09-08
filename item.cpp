//=============================================================================
//
// アイテム処理 [item.cpp](機雷）
// Author : 西村　任弘
//
//=============================================================================
#include "item.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureItem[ITEMTYPE_MAX];	// テクスチャ読み込み場所
LPD3DXMESH			MeshItem[ITEMTYPE_MAX];			// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXMatBuffItem[ITEMTYPE_MAX];	// メッシュのマテリアル情報を格納
DWORD				NumMatItem[ITEMTYPE_MAX];		// 属性情報の総数

D3DXMATRIX			MtxWorldItem;				// ワールドマトリックス

Item				item[MAX_ITEM];			// アイテムワーク

const char *FileNameItem[ITEMTYPE_MAX] =
{
	"data/MODEL/item000.x",		// 機雷
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	for(int CntItemType = 0; CntItemType < ITEMTYPE_MAX; CntItemType++)
	{
		D3DTextureItem[CntItemType] = NULL;
		MeshItem[CntItemType] = NULL;
		D3DXMatBuffItem[CntItemType] = NULL;

		// Xファイルの読み込み
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
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(Device,									// デバイスへのポインタ
									TEXTURE_FILENAME,						// ファイルの名前
									&D3DTextureModelItem[CntItemType]);	// 読み込むメモリー
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
// 終了処理
//=============================================================================
void UninitItem(void)
{
	for(int CntItemType = 0; CntItemType < ITEMTYPE_MAX; CntItemType++)
	{
		if(D3DTextureItem[CntItemType] != NULL)
		{// テクスチャの開放
			D3DTextureItem[CntItemType]->Release();
			D3DTextureItem[CntItemType] = NULL;
		}

		if(MeshItem[CntItemType] != NULL)
		{// メッシュの開放
			MeshItem[CntItemType]->Release();
			MeshItem[CntItemType] = NULL;
		}

		if(D3DXMatBuffItem[CntItemType] != NULL)
		{// マテリアルの開放
			D3DXMatBuffItem[CntItemType]->Release();
			D3DXMatBuffItem[CntItemType] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
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
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	for(int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
	{
		if(item[CntItem].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&MtxWorldItem);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, item[CntItem].rot.y, item[CntItem].rot.x, item[CntItem].rot.z);
			D3DXMatrixMultiply(&MtxWorldItem, &MtxWorldItem, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, item[CntItem].pos.x, item[CntItem].pos.y, item[CntItem].pos.z);
			D3DXMatrixMultiply(&MtxWorldItem, &MtxWorldItem, &mtxTranslate);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &MtxWorldItem);

			D3DXMATERIAL *pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffItem[item[CntItem].Type]->GetBufferPointer();

			for(int CntMat = 0; CntMat < (int)NumMatItem[item[CntItem].Type]; CntMat++)
			{
				// マテリアルの設定
				Device->SetMaterial(&pD3DXMat[CntMat].MatD3D);

				// テクスチャの設定
				Device->SetTexture(0, D3DTextureItem[item[CntItem].Type]);

				// 描画
				MeshItem[item[CntItem].Type]->DrawSubset(CntMat);
			}
		}
	}

	{// マテリアルをデフォルトに戻す
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		Device->SetMaterial(&mat.MatD3D);
	}
}

//=============================================================================
// アイテムの設定
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
// アイテムの削除
//=============================================================================
void DeleteItem(int IdxItem)
{
	if(IdxItem >= 0 && IdxItem < MAX_ITEM)
	{
		
		item[IdxItem].use = false;
	}
}

//=============================================================================
// アイテムの取得
//=============================================================================
Item *GetItem(void)
{
	return &item[0];
}
