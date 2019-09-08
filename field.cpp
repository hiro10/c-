//=============================================================================
//
// 地面処理 [field.cpp]
// Author : 西村任弘
//
//=============================================================================
#include "field.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 Device);
HRESULT MakeVertexSpace(LPDIRECT3DDEVICE9 Device);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureLoad = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		D3DTextureSpace = NULL;		// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffLoad = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffSpace = NULL;	// 頂点バッファへのポインタ

D3DXMATRIX				mtxWorldLoad;				// ワールドマトリックス
D3DXVECTOR3				posLoad;					// 現在の位置
D3DXVECTOR3				rotLoad;					// 現在の向き

D3DXMATRIX				g_mtxWorldSpace;			// ワールドマトリックス
D3DXVECTOR3				g_posSpace;					// 現在の位置
D3DXVECTOR3				g_rotSpace;					// 現在の向き

static float offset = 0.0f;							// スクロール用
float s_offset = 0.0f;								// スクロール用				

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice(); 
	HRESULT hr;
	HRESULT sr;

	// 位置、向きの初期設定
	posLoad = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotLoad = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
		TEXTURE_FIELD,									// ファイルの名前
		&D3DTextureLoad);								// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
		TEXTURE_SPACE,									// ファイルの名前
		&D3DTextureSpace);								// 読み込むメモリー

	// 頂点情報の作成
	hr = MakeVertexField(Device);
	sr = MakeVertexSpace(Device);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	if(D3DTextureLoad != NULL)
	{// テクスチャの開放
		D3DTextureLoad->Release();
		D3DTextureLoad = NULL;
	}

	if (D3DTextureSpace != NULL)
	{// テクスチャの開放
		D3DTextureSpace->Release();
		D3DTextureSpace = NULL;
	}

	if(D3DVtxBuffLoad != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffLoad->Release();
		D3DVtxBuffLoad = NULL;
	}

	if (g_pD3DVtxBuffSpace != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffSpace->Release();
		g_pD3DVtxBuffSpace = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
	{
	//頂点バッファの中身を埋める
	VERTEX_3D *Vtx;
	VERTEX_3D *Vtxs;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffLoad->Lock(0, 0, (void**)&Vtx, 0);
	g_pD3DVtxBuffSpace->Lock(0, 0, (void**)&Vtxs, 0);

	//　スクロールスピードの設定
	offset -= SCROLL_SPEED;				// 要塞
	s_offset -= SPACE_SCROLL_SPEED;		// 宇宙

	// 要塞
	if (offset < 1.0f)
	{
		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(offset, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(offset, 1.0f);
		Vtx[2].tex = D3DXVECTOR2(offset + 1.0f, 0.0f);
		Vtx[3].tex = D3DXVECTOR2(offset + 1.0f, 1.0f);

	}
	if (offset <= -1.0f)
	{
		// offsetを0に
		offset = 0.0f;
	}

	// 宇宙
	if (s_offset < 1.0f)
	{

		// テクスチャ座標の設定
		Vtxs[0].tex = D3DXVECTOR2(s_offset, 0.0f);
		Vtxs[1].tex = D3DXVECTOR2(s_offset, 1.0f);
		Vtxs[2].tex = D3DXVECTOR2(s_offset + 1.0f, 0.0f);
		Vtxs[3].tex = D3DXVECTOR2(s_offset + 1.0f, 1.0f);

	}
	if (s_offset <= -1.0f)
	{
		// s_offsetを0に
		s_offset = 0.0f;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice(); 
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXVECTOR3 eb_move_left;

	eb_move_left.x = 0.0;
	eb_move_left.y = 0.0f;
	eb_move_left.z -= VALUE_MOVE_EFFECTBULLET;


	// 頂点データをアンロックする
	D3DVtxBuffLoad->Unlock();
	g_pD3DVtxBuffSpace->Unlock();


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldSpace);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotLoad.y, rotLoad.x, rotLoad.z);
	D3DXMatrixMultiply(&g_mtxWorldSpace, &g_mtxWorldSpace, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, posLoad.x, posLoad.y, posLoad.z);
	D3DXMatrixMultiply(&g_mtxWorldSpace, &g_mtxWorldSpace, &mtxTranslate);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &mtxWorldLoad);

	// 頂点バッファをデバイスのデータストリームにバインド
	Device->SetStreamSource(0, g_pD3DVtxBuffSpace, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureSpace);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorldLoad);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotLoad.y, rotLoad.x, rotLoad.z);
	D3DXMatrixMultiply(&mtxWorldLoad, &mtxWorldLoad, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, posLoad.x, posLoad.y, posLoad.z);
	D3DXMatrixMultiply(&mtxWorldLoad, &mtxWorldLoad, &mtxTranslate);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &mtxWorldLoad);

	// 頂点バッファをデバイスのデータストリームにバインド
	Device->SetStreamSource(0, D3DVtxBuffLoad, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureLoad);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffLoad,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	

	//頂点バッファの中身を埋める
	VERTEX_3D *Vtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffLoad->Lock(0, 0, (void**)&Vtx, 0);

	// 頂点座標の設定
	Vtx[0].vtx = D3DXVECTOR3(-900.0f, -130.0f, 11800.0f);
	Vtx[1].vtx = D3DXVECTOR3(900.0f, -130.0f, 11800.0f);
	Vtx[2].vtx = D3DXVECTOR3(-900.0f, -130.0f, -13400.0f);
	Vtx[3].vtx = D3DXVECTOR3(900.0f, -130.0f, -13400.0f);

	// 法線ベクトルの設定
	Vtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 反射光の設定
	Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuffLoad->Unlock();


	return S_OK;
}

//=============================================================================
// 頂点の作成(二枚目の宇宙）
//=============================================================================
HRESULT MakeVertexSpace(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffSpace,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_3D *Vtxs;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffSpace->Lock(0, 0, (void**)&Vtxs, 0);

	// 頂点座標の設定
	Vtxs[0].vtx = D3DXVECTOR3(-900.0f, -140.0f, 14100.0f);
	Vtxs[1].vtx = D3DXVECTOR3(900, -140.0f, 14100.0f);
	Vtxs[2].vtx = D3DXVECTOR3(-900.0f, -140.0f, -14100.0f);
	Vtxs[3].vtx = D3DXVECTOR3(900.0f, -140.0f, -14100.0f);

	// 法線ベクトルの設定
	Vtxs[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxs[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxs[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vtxs[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 反射光の設定
	Vtxs[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxs[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxs[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Vtxs[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	// 頂点データをアンロックする
	g_pD3DVtxBuffSpace->Unlock();

	return S_OK;
}
