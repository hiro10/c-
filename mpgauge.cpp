//=============================================================================
//
// 背景処理 [mpgauge.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "mpgauge.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMpGauge(void);
void SetVertexMpGauge(float val);
void SetTextureMpGauge(float val);
void SetVertexMpGaugeFrame(void);
void SetTextureMpGaugeFrame(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureMpGauge = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		D3DTextureMpGaugeFrame = NULL;	// テクスチャへのポインタ

VERTEX_2D				VertexWkMpGauge[NUM_VERTEX];	// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMpGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,				// デバイスへのポインタ
		TEXTURE_GAME_MPGAUGE00,						// ファイルの名前
		&D3DTextureMpGauge);							// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,				// デバイスへのポインタ
		TEXTURE_GAME_MPGAUGEFRAME00,					// ファイルの名前
		&D3DTextureMpGaugeFrame);						// 読み込むメモリー

	

	// 頂点情報の作成
	MakeVertexMpGauge();

	SetVertexMpGauge(100.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMpGauge(void)
{
	if (D3DTextureMpGauge != NULL)
	{// テクスチャの開放
		D3DTextureMpGauge->Release();
		D3DTextureMpGauge = NULL;
	}

	if (D3DTextureMpGaugeFrame != NULL)
	{// テクスチャの開放
		D3DTextureMpGaugeFrame->Release();
		D3DTextureMpGaugeFrame = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMpGauge(void)
{
	// 毎フレーム実行する処理
	Player *player = GetPlayer();
	
	// プレイヤーの現在のHP/プレイヤーのHPの最大値
	float val  = player->mp / (float)(100);

	SetVertexMpGauge(val);
	SetTextureMpGauge(val);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMpGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	

	//頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureMpGauge);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkMpGauge, sizeof(VERTEX_2D));

	SetVertexMpGaugeFrame();
	SetTextureMpGaugeFrame();

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureMpGaugeFrame);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkMpGauge, sizeof(VERTEX_2D));

	}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexMpGauge(void)
{
	// 頂点座標の設定
	VertexWkMpGauge[0].vtx = D3DXVECTOR3(MPGAUGE00_POS_X, MPGAUGE00_POS_Y, 0.0f);
	VertexWkMpGauge[1].vtx = D3DXVECTOR3(MPGAUGE00_POS_X + MPGAUGE00_SIZE_X, MPGAUGE00_POS_Y, 0.0f);
	VertexWkMpGauge[2].vtx = D3DXVECTOR3(MPGAUGE00_POS_X, MPGAUGE00_POS_Y + MPGAUGE00_SIZE_Y, 0.0f);
	VertexWkMpGauge[3].vtx = D3DXVECTOR3(MPGAUGE00_POS_X + MPGAUGE00_SIZE_X, MPGAUGE00_POS_Y + MPGAUGE00_SIZE_Y, 0.0f);

	// rhwの設定
	VertexWkMpGauge[0].rhw =
	VertexWkMpGauge[1].rhw =
	VertexWkMpGauge[2].rhw =
	VertexWkMpGauge[3].rhw = 1.0f;

	// 反射光の設定
	VertexWkMpGauge[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkMpGauge[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkMpGauge[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkMpGauge[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	VertexWkMpGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkMpGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkMpGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkMpGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexMpGauge(float val)
{
	// 頂点座標の設定
	VertexWkMpGauge[0].vtx = D3DXVECTOR3(MPGAUGE00_POS_X, MPGAUGE00_POS_Y, 0.0f);
	VertexWkMpGauge[1].vtx = D3DXVECTOR3(MPGAUGE00_POS_X + (MPGAUGE00_SIZE_X * val), MPGAUGE00_POS_Y, 0.0f);
	VertexWkMpGauge[2].vtx = D3DXVECTOR3(MPGAUGE00_POS_X, MPGAUGE00_POS_Y + MPGAUGE00_SIZE_Y, 0.0f);
	VertexWkMpGauge[3].vtx = D3DXVECTOR3(MPGAUGE00_POS_X + (MPGAUGE00_SIZE_X * val), MPGAUGE00_POS_Y + MPGAUGE00_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureMpGauge(float val)
{
	// テクスチャ座標の設定
	VertexWkMpGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkMpGauge[1].tex = D3DXVECTOR2(1.0f * val, 0.0f);
	VertexWkMpGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkMpGauge[3].tex = D3DXVECTOR2(1.0f * val, 1.0f);
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexMpGaugeFrame(void)
{
	// 頂点座標の設定
	VertexWkMpGauge[0].vtx = D3DXVECTOR3(MPGAUGEFRAME00_POS_X, MPGAUGEFRAME00_POS_Y, 0.0f);
	VertexWkMpGauge[1].vtx = D3DXVECTOR3(MPGAUGEFRAME00_POS_X + MPGAUGEFRAME00_SIZE_X, MPGAUGEFRAME00_POS_Y, 0.0f);
	VertexWkMpGauge[2].vtx = D3DXVECTOR3(MPGAUGEFRAME00_POS_X, MPGAUGEFRAME00_POS_Y + MPGAUGEFRAME00_SIZE_Y, 0.0f);
	VertexWkMpGauge[3].vtx = D3DXVECTOR3(MPGAUGEFRAME00_POS_X + MPGAUGEFRAME00_SIZE_X, MPGAUGEFRAME00_POS_Y + MPGAUGEFRAME00_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureMpGaugeFrame(void)
{
	// テクスチャ座標の設定
	VertexWkMpGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkMpGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkMpGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkMpGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

