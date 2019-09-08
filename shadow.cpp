//=============================================================================
//
// 影処理 [shadow.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "shadow.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureShadow = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffShadow = NULL;		// 頂点バッファインターフェースへのポインタ

static D3DXMATRIX				MtxWorldShadow;			// ワールドマトリックス

SHADOW					shadow[MAX_SHADOW];			// 影ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexShadow(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
								TEXTURE_SHADOW,				// ファイルの名前
								&D3DTextureShadow);		// 読み込むメモリー

	for(int CntShadow = 0; CntShadow < MAX_SHADOW; CntShadow++)
	{
		shadow[CntShadow].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		shadow[CntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		shadow[CntShadow].use = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	if(D3DTextureShadow != NULL)
	{// テクスチャの開放
		D3DTextureShadow->Release();
		D3DTextureShadow = NULL;
	}

	if(D3DVtxBuffShadow != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffShadow->Release();
		D3DVtxBuffShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// 描画処理
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

	// 減算合成
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);		// 結果 = 転送先(DEST) - 転送元(SRC)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	for (int CntShadow = 0; CntShadow < MAX_SHADOW; CntShadow++)
	{
		if (shadow[CntShadow].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&MtxWorldShadow);

#if 0
			// 回転を反映
			D3DXQuaternionIdentity(&quat);

			vecUpObj = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// オブジェクトの上方向
			vecUpField = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 地面の法線

			// オブジェクトの上方向と地面の法線の外積から回転軸を求める
			D3DXVec3Cross(&outVec, &vecUpObj, &vecUpField);

			// オブジェクトの上方向と地面の法線のなす角を求める
			fDotProduct = D3DXVec3Dot(&vecUpObj, &vecUpField);
			fRot = acosf(fDotProduct / (sqrtf(vecUpObj.x * vecUpObj.x + vecUpObj.y * vecUpObj.y + vecUpObj.z * vecUpObj.z)
				* sqrtf(vecUpField.x * vecUpField.x + vecUpField.y * vecUpField.y + vecUpField.z * vecUpField.z)));

			// 回転軸となす角からクォータニオンを求め、回転マトリックスを算出
			D3DXQuaternionRotationAxis(&quat, &outVec, fRot);
			D3DXMatrixRotationQuaternion(&mtxRot, &quat);
			D3DXMatrixMultiply(&MtxWorldShadow, &MtxWorldShadow, &mtxRot);
#endif

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, shadow[CntShadow].pos.x, shadow[CntShadow].pos.y, shadow[CntShadow].pos.z);
			D3DXMatrixMultiply(&MtxWorldShadow, &MtxWorldShadow, &mtxTranslate);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &MtxWorldShadow);

			// 頂点バッファをレンダリングパイプラインに設定
			Device->SetStreamSource(0, D3DVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureShadow);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntShadow * 4), NUM_POLYGON);
		}
	}

	// 通常ブレンド
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);				// 結果 = 転送元(SRC) + 転送先(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_SHADOW,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_3D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffShadow,						// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffShadow->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntShadow = 0; CntShadow < MAX_SHADOW; CntShadow++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(-SHADOW_SIZE_X / 2,0.0f, SHADOW_SIZE_Z / 2);
			Vtx[1].vtx = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
			Vtx[2].vtx = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);
			Vtx[3].vtx = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);

			// 法線の設定
			Vtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			Vtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			Vtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			Vtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 反射光の設定
			Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			// テクスチャ座標の設定
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		D3DVtxBuffShadow->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexShadow(int IdxShadow,int s, float SizeX, float SizeY, float SizeZ)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;
		
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffShadow->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxShadow * 4);

		if (s == 0)
		{
			// 頂点座標の設定
			Vtx[1].vtx = D3DXVECTOR3(0, SizeY / 2, SizeZ / 2);
			Vtx[0].vtx = D3DXVECTOR3(0, -SizeY / 2, SizeZ / 2);
			Vtx[3].vtx = D3DXVECTOR3(0, SizeY / 2, -SizeZ / 2);
			Vtx[2].vtx = D3DXVECTOR3(0, -SizeY / 2, -SizeZ / 2);
		}

		 if (s == 1)
		{
			// 頂点座標の設定
			Vtx[1].vtx = D3DXVECTOR3(0, -SizeY / 2, SizeZ / 2);
			Vtx[0].vtx = D3DXVECTOR3(0, SizeY / 2, SizeZ / 2);
			Vtx[3].vtx = D3DXVECTOR3(0, -SizeY / 2, -SizeZ / 2);
			Vtx[2].vtx = D3DXVECTOR3(0, SizeY / 2, -SizeZ / 2);
		}


		 if (s == 2)
		 {
			 // 頂点座標の設定
			 Vtx[0].vtx = D3DXVECTOR3(SizeX / 2, 0.0f, SizeZ / 2);
			 Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, 0.0f, SizeZ / 2);
			 Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, 0.0f, -SizeZ / 2);
			 Vtx[3].vtx = D3DXVECTOR3(-SizeX / 2, 0.0f, -SizeZ / 2);

		 }

		// 頂点データをアンロックする
		D3DVtxBuffShadow->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorShadow(int IdxShadow, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffShadow->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxShadow * 4);

		// 頂点座標の設定
		Vtx[0].diffuse =
		Vtx[1].diffuse =
		Vtx[2].diffuse =
		Vtx[3].diffuse = col;

		// 頂点データをアンロックする
		D3DVtxBuffShadow->Unlock();
	}
}

//=============================================================================
// 影の設定
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
// 位置の設定
//=============================================================================
void SetPositionShadow(int IdxShadow, D3DXVECTOR3 pos)
{
	for (int nIdxShadow = 0; nIdxShadow < MAX_SHADOW; nIdxShadow++)
	{
		shadow[nIdxShadow].pos = pos;
	}
}

//=============================================================================
// 影の削除
//=============================================================================
void DeleteShadow(int IdxShadow)
{
	if(IdxShadow >= 0 && IdxShadow < MAX_SHADOW)
	{
		shadow[IdxShadow].use = false;
	}
}
