//=============================================================================
//
// メッシュ地面の処理 [meshfield.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "meshfield.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	LOOP_MAX		(500)		// フラクタルの演算数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 D3DTextureField;			// テクスチャ読み込み場所
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffField;	// 頂点バッファインターフェースへのポインタ
LPDIRECT3DINDEXBUFFER9 D3DIdxBuffField;		// インデックスバッファインターフェースへのポインタ

D3DXMATRIX WorldField;					// ワールドマトリックス
D3DXVECTOR3 PosField;						// ポリゴン表示位置の中心座標
D3DXVECTOR3 RotField;						// ポリゴンの回転角

int NumBlockXField, NumBlockZField;			// ブロック数
int NumVertexField;							// 総頂点数	
int NumVertexIndexField;					// 総インデックス数
int NumPolygonField;						// 総ポリゴン数
float BlockSizeXField, BlockSizeZField;		// ブロックサイズ
float Xrand, Zrand, Rrand;
float ValField;
float xMax, zMax, rMax;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	xMax = (fBlockSizeX * nNumBlockX) / 2;
	zMax = (fBlockSizeX * nNumBlockX) / 2;
	rMax = 100;

	ValField = (fBlockSizeX * nNumBlockX);

	// ポリゴン表示位置の中心座標を設定
	PosField = pos;

	RotField = rot;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,				// デバイスへのポインタ
								TEXTURE_BOSSFIELD,		// ファイルの名前
								&D3DTextureField);	// 読み込むメモリー

	// ブロック数の設定
	NumBlockXField = nNumBlockX;
	NumBlockZField = nNumBlockZ;

	// 頂点数の設定
	NumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	NumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	NumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	BlockSizeXField = fBlockSizeX;
	BlockSizeZField = fBlockSizeZ;

	// オブジェクトの頂点バッファを生成
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NumVertexField,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_3D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffField,				// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
    if(FAILED(Device->CreateIndexBuffer(sizeof(WORD) * NumVertexIndexField,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												D3DFMT_INDEX16,						// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&D3DIdxBuffField,				// インデックスバッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;
#if 0
		const float TexSizeX = 1.0f / NumBlockX;
		const float TexSizeZ = 1.0f / NumBlockZ;
#else
		const float TexSizeX = 1.0f;
		const float TexSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffField->Lock( 0, 0, (void**)&Vtx, 0 );

		for(int CntVtxZ = 0; CntVtxZ < (NumBlockZField + 1); CntVtxZ++)
		{
			for(int CntVtxX = 0; CntVtxX < (NumBlockXField + 1); CntVtxX++)
			{
				// 頂点座標の設定
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.x = -(NumBlockXField / 2.0f) * BlockSizeXField + CntVtxX * BlockSizeXField;
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.y = 0.0f;
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.z = (NumBlockZField / 2.0f) * BlockSizeZField - CntVtxZ * BlockSizeZField;

				// 法線の設定
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// 反射光の設定
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].tex.x = TexSizeX * CntVtxX;
				Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].tex.y = TexSizeZ * CntVtxZ;
			}
		}

		// 頂点データをアンロックする
		D3DVtxBuffField->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *Idx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		D3DIdxBuffField->Lock( 0, 0, (void**)&Idx, 0 );

		int CntIdx = 0;
		for(int CntVtxZ = 0; CntVtxZ < NumBlockZField; CntVtxZ++)
		{
			if(CntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
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
			{// 縮退ポリゴンのためのダブりの設定
				Idx[CntIdx] = CntVtxZ * (NumBlockXField + 1) + NumBlockXField;
				CntIdx++;
			}
		}

		// インデックスデータをアンロックする
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
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	if(D3DVtxBuffField!=NULL)
	{// 頂点バッファの開放
		D3DVtxBuffField->Release();
		D3DVtxBuffField = NULL;
	}

	if(D3DIdxBuffField != NULL)
	{// インデックスバッファの開放
		D3DIdxBuffField->Release();
		D3DIdxBuffField = NULL;
	}

	if(D3DTextureField != NULL)
	{// テクスチャの開放
		D3DTextureField->Release();
		D3DTextureField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DXMATRIX MtxRot, MtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldField);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&MtxRot, RotField.y, RotField.x, RotField.z);
	D3DXMatrixMultiply(&WorldField, &WorldField, &MtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&MtxTranslate, PosField.x, PosField.y, PosField.z);
	D3DXMatrixMultiply(&WorldField, &WorldField, &MtxTranslate);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &WorldField);

	// 頂点バッファをレンダリングパイプラインに設定
	Device->SetStreamSource(0, D3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	Device->SetIndices(D3DIdxBuffField);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureField);

	// ポリゴンの描画
	Device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertexField, 0, NumPolygonField);
}

