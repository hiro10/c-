//=============================================================================
//
// 右壁処理 [wallright.cpp]
// Author :  西村　任弘
//
//=============================================================================
#include "wallright.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexWallright(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureWallright = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffWallright = NULL;		// 頂点バッファへのポインタ
D3DXMATRIX				MtxWorldWallright;				// ワールドマトリックス
D3DXVECTOR3				PosWallright;					// 現在の位置
D3DXVECTOR3				RotWallright;					// 現在の向き
static float offset = 0.0f;								// スクロール用
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWallright(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	HRESULT hr;

	// 位置、向きの初期設定
	PosWallright = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotWallright = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
		TEXTURE_WALLRIGHT,								// ファイルの名前
		&D3DTextureWallright);							// 読み込むメモリー

	// 頂点情報の作成
	hr = MakeVertexWallright(Device);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWallright(void)
{
	if (D3DTextureWallright != NULL)
	{// テクスチャの開放
		D3DTextureWallright->Release();
		D3DTextureWallright = NULL;
	}

	if (D3DVtxBuffWallright != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffWallright->Release();
		D3DVtxBuffWallright = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWallright(void)
{
	// 頂点バッファの中身を埋める
	VERTEX_3D *Vtxr;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffWallright->Lock(0, 0, (void**)&Vtxr, 0);

	//テクスチャ用のスクロールスピード
	offset -= SCROLL_SPEED_RIGHT;

	if (offset < 1.0f)
	{
		// テクスチャ座標の設定
		Vtxr[0].tex = D3DXVECTOR2(offset, 0.0f);
		Vtxr[1].tex = D3DXVECTOR2(offset, 1.0f);
		Vtxr[2].tex = D3DXVECTOR2(offset + 1.0f, 0.0f);
		Vtxr[3].tex = D3DXVECTOR2(offset + 1.0f, 1.0f);
	}

	if (offset <= -1.0f)
	{
		// offsetのリセット
		offset = 0.0f;
	}

	// 頂点データをアンロックする
	D3DVtxBuffWallright->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWallright(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxRot, MtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&MtxWorldWallright);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&MtxRot, RotWallright.y, RotWallright.x, RotWallright.z);
	D3DXMatrixMultiply(&MtxWorldWallright, &MtxWorldWallright, &MtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&MtxTranslate, PosWallright.x, PosWallright.y, PosWallright.z);
	D3DXMatrixMultiply(&MtxWorldWallright, &MtxWorldWallright, &MtxTranslate);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &MtxWorldWallright);

	// 頂点バッファをデバイスのデータストリームにバインド
	Device->SetStreamSource(0, D3DVtxBuffWallright, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureWallright);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexWallright(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffWallright,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	// 頂点バッファの中身を埋める
	VERTEX_3D *Vtxr;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffWallright->Lock(0, 0, (void**)&Vtxr, 0);

	//hidari
	Vtxr[0].vtx = D3DXVECTOR3(WALLRIGNT_VTX_X, -WALLRIGNT_VTX_Y, WALLRIGNT_VTX_Z);
	Vtxr[1].vtx = D3DXVECTOR3(WALLRIGNT_VTX_X, WALLRIGNT_VTX_Y, WALLRIGNT_VTX_Z);
	Vtxr[2].vtx = D3DXVECTOR3(WALLRIGNT_VTX_X, -WALLRIGNT_VTX_Y, -WALLRIGNT_VTX_Z);
	Vtxr[3].vtx = D3DXVECTOR3(WALLRIGNT_VTX_X, WALLRIGNT_VTX_Y, -WALLRIGNT_VTX_Z);

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
	D3DVtxBuffWallright->Unlock();


	return S_OK;
}
