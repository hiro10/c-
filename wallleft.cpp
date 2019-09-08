//=============================================================================
//
// 左かべ処理 [wallleft.cpp]
// Author :  西村　任弘
//
//=============================================================================
#include "wallleft.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexWallleft(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureWallleft = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffWallleft = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX				MtxWorldWallleft;			// ワールドマトリックス
D3DXVECTOR3				PosWallleft;				// 現在の位置
D3DXVECTOR3				RotWallleft;				// 現在の向き
static float offset = 0.0f;							// スクロール用
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWallleft(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	HRESULT hr;

	// 位置、向きの初期設定
	PosWallleft = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotWallleft = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,				// デバイスへのポインタ
		TEXTURE_WALLLEFT,							// ファイルの名前
		&D3DTextureWallleft);						// 読み込むメモリー

	// 頂点情報の作成
	hr = MakeVertexWallleft(Device);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWallleft(void)
{
	if (D3DTextureWallleft != NULL)
	{// テクスチャの開放
		D3DTextureWallleft->Release();
		D3DTextureWallleft = NULL;
	}

	if (D3DVtxBuffWallleft != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffWallleft->Release();
		D3DVtxBuffWallleft = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWallleft(void)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *Vtxl;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffWallleft->Lock(0, 0, (void**)&Vtxl, 0);

	//テクスチャスクロ―ルのスピード
	offset -= SCROLL_SPEED_LEFT;

	//1.0より下になったら
	if (offset < 1.0f)
	{
		// テクスチャ座標の設定
		Vtxl[0].tex = D3DXVECTOR2(offset, 0.0f);
		Vtxl[1].tex = D3DXVECTOR2(offset, 1.0f);
		Vtxl[2].tex = D3DXVECTOR2(offset + 1.0f, 0.0f);
		Vtxl[3].tex = D3DXVECTOR2(offset + 1.0f, 1.0f);
	}

	if (offset <= -1.0f)
	{//-1.0以下になったら0に戻す
		offset = 0.0f;
	}

	// 頂点データをアンロックする
	D3DVtxBuffWallleft->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWallleft(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxRot, MtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&MtxWorldWallleft);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&MtxRot, RotWallleft.y, RotWallleft.x, RotWallleft.z);
	D3DXMatrixMultiply(&MtxWorldWallleft, &MtxWorldWallleft, &MtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&MtxTranslate, PosWallleft.x, PosWallleft.y, PosWallleft.z);
	D3DXMatrixMultiply(&MtxWorldWallleft, &MtxWorldWallleft, &MtxTranslate);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &MtxWorldWallleft);

	// 頂点バッファをデバイスのデータストリームにバインド
	Device->SetStreamSource(0, D3DVtxBuffWallleft, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureWallleft);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexWallleft(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffWallleft,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	//{//頂点バッファの中身を埋める
	VERTEX_3D *Vtxl;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffWallleft->Lock(0, 0, (void**)&Vtxl, 0);

	// 頂点座標の設定
	Vtxl[1].vtx = D3DXVECTOR3(-WALLLEFE_VTX_X, -WALLLEFE_VTX_Y, WALLLEFE_VTX_Z);
	Vtxl[0].vtx = D3DXVECTOR3(-WALLLEFE_VTX_X, WALLLEFE_VTX_Y, WALLLEFE_VTX_Z);
	Vtxl[3].vtx = D3DXVECTOR3(-WALLLEFE_VTX_X, -WALLLEFE_VTX_Y, -WALLLEFE_VTX_Z);
	Vtxl[2].vtx = D3DXVECTOR3(-WALLLEFE_VTX_X, WALLLEFE_VTX_Y, -WALLLEFE_VTX_Z);

	// 法線ベクトルの設定
	Vtxl[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxl[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxl[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxl[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 反射光の設定
	Vtxl[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxl[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxl[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxl[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	Vtxl[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Vtxl[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Vtxl[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Vtxl[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点データをアンロックする
	D3DVtxBuffWallleft->Unlock();
	//}

	return S_OK;
}

