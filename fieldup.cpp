//=============================================================================
//
// 天井処理 [fieldup.cpp]
// Author :  西村　任弘
//
//=============================================================================
#include "fieldup.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexFieldup(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureFieldup = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffFieldup = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX				MtxWorldFieldup;			// ワールドマトリックス
D3DXVECTOR3				PosFieldup;					// 現在の位置
D3DXVECTOR3				RotFieldup;					// 現在の向き
static float offset = 0.0f;							// スクロール用
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFieldup(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	HRESULT hr;

	// 位置、向きの初期設定
	PosFieldup = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotFieldup = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
		TEXTURE_WALLRIGHT,								// ファイルの名前
		&D3DTextureFieldup);							// 読み込むメモリー

	// 頂点情報の作成
	hr = MakeVertexFieldup(Device);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFieldup(void)
{
	if (D3DTextureFieldup != NULL)
	{// テクスチャの開放
		D3DTextureFieldup->Release();
		D3DTextureFieldup = NULL;
	}

	if (D3DVtxBuffFieldup != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffFieldup->Release();
		D3DVtxBuffFieldup = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFieldup(void)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *Vtxr;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffFieldup->Lock(0, 0, (void**)&Vtxr, 0);

	offset -= FIELDUP_SCROLL_SPEED;

	if (offset < 1.0f)
	{
		// テクスチャ座標の設定
		Vtxr[0].tex = D3DXVECTOR2(offset, 0.0f);
		Vtxr[1].tex = D3DXVECTOR2(offset, 1.0f);
		Vtxr[2].tex = D3DXVECTOR2(offset + 1.0f, 0.0f);
		Vtxr[3].tex = D3DXVECTOR2(offset + 1.0f, 1.0f);
	}

	if (offset <= -1.0f)
	{// offsetが-1以下なら0に戻す
		offset = 0.0f;
	}

	// 頂点データをアンロックする
	D3DVtxBuffFieldup->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFieldup(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&MtxWorldFieldup);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, RotFieldup.y, RotFieldup.x, RotFieldup.z);
	D3DXMatrixMultiply(&MtxWorldFieldup, &MtxWorldFieldup, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, PosFieldup.x, PosFieldup.y, PosFieldup.z);
	D3DXMatrixMultiply(&MtxWorldFieldup, &MtxWorldFieldup, &mtxTranslate);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &MtxWorldFieldup);

	// 頂点バッファをデバイスのデータストリームにバインド
	Device->SetStreamSource(0, D3DVtxBuffFieldup, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureFieldup);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexFieldup(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffFieldup,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	//{//頂点バッファの中身を埋める
	VERTEX_3D *Vtxr;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffFieldup->Lock(0, 0, (void**)&Vtxr, 0);

	//hidari
	Vtxr[1].vtx = D3DXVECTOR3(-860.0f, 620.0f, 24100.0f);
	Vtxr[0].vtx = D3DXVECTOR3(860.0f, 620.0f, 24100.0f);
	Vtxr[3].vtx = D3DXVECTOR3(-860.0f, 620.0f, -14100.0f);
	Vtxr[2].vtx = D3DXVECTOR3(860.0f, 620.0f, -14100.0f);

	// 法線ベクトルの設定
	Vtxr[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxr[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxr[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxr[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 反射光の設定
	Vtxr[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxr[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxr[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxr[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuffFieldup->Unlock();


	return S_OK;
}