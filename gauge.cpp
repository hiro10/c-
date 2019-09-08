//=============================================================================
//
// 背景処理 [gauge.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "gauge.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGauge(void);
void SetVertexGauge(float val);
void SetTextureGauge(float val);
void SetVertexGaugeFrame(void);
void SetTextureGaugeFrame(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureGauge = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		D3DTextureGaugeFrame = NULL;	// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		D3DTexturehp = NULL;			// テクスチャへのポインタ

VERTEX_2D				VertexWkhp[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D				VertexWkGauge[NUM_VERTEX];		// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,				// デバイスへのポインタ
		TEXTURE_GAME_GAUGE00,						// ファイルの名前
		&D3DTextureGauge);							// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,				// デバイスへのポインタ
		TEXTURE_GAME_GAUGEFRAME00,					// ファイルの名前
		&D3DTextureGaugeFrame);						// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,				// デバイスのポインタ
		TEXTURE_HP,									// ファイルの名前
		&D3DTexturehp);								// 読み込むメモリのポインタ

	// 頂点情報の作成
	MakeVertexGauge();

	SetVertexGauge(100.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGauge(void)
{
	if (D3DTextureGauge != NULL)
	{// テクスチャの開放
		D3DTextureGauge->Release();
		D3DTextureGauge = NULL;
	}

	if (D3DTexturehp != NULL)
	{// テクスチャの開放
		D3DTexturehp->Release();
		D3DTexturehp = NULL;
	}

	if (D3DTextureGaugeFrame != NULL)
	{// テクスチャの開放
		D3DTextureGaugeFrame->Release();
		D3DTextureGaugeFrame = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGauge(void)
{
	// 毎フレーム実行する処理
	Player *player = GetPlayer();
		
	// プレイヤーの現在のHP/プレイヤーのHPの最大値
	float val  = player->hp / (float)(PLAYER_LIFEMAX);

	SetVertexGauge(val);
	SetTextureGauge(val);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Player *player = GetPlayer();

	//頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureGauge);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkGauge, sizeof(VERTEX_2D));

	SetVertexGaugeFrame();
	SetTextureGaugeFrame();

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureGaugeFrame);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkGauge, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定(TEXTURE_TITLE)
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(TEXTURE_TITLE)
	Device->SetTexture(0, D3DTexturehp);

	// ポリゴンの描画(TEXTURE_TITLE)
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkhp, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGauge(void)
{
	// 頂点座標の設定
	VertexWkGauge[0].vtx = D3DXVECTOR3(GAUGE00_POS_X, GAUGE00_POS_Y, 0.0f);
	VertexWkGauge[1].vtx = D3DXVECTOR3(GAUGE00_POS_X + GAUGE00_SIZE_X, GAUGE00_POS_Y, 0.0f);
	VertexWkGauge[2].vtx = D3DXVECTOR3(GAUGE00_POS_X, GAUGE00_POS_Y + GAUGE00_SIZE_Y, 0.0f);
	VertexWkGauge[3].vtx = D3DXVECTOR3(GAUGE00_POS_X + GAUGE00_SIZE_X, GAUGE00_POS_Y + GAUGE00_SIZE_Y, 0.0f);

	// rhwの設定
	VertexWkGauge[0].rhw =
	VertexWkGauge[1].rhw =
	VertexWkGauge[2].rhw =
	VertexWkGauge[3].rhw = 1.0f;

	// 反射光の設定
	VertexWkGauge[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkGauge[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkGauge[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkGauge[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	VertexWkGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	VertexWkhp[0].vtx = D3DXVECTOR3(HP_POS_X, HP_POS_Y, 0.0f);
	VertexWkhp[1].vtx = D3DXVECTOR3(HP_POS_X + HP_SIZE_X, HP_POS_Y, 0.0f);
	VertexWkhp[2].vtx = D3DXVECTOR3(HP_POS_X, HP_POS_Y + HP_SIZE_Y, 0.0f);
	VertexWkhp[3].vtx = D3DXVECTOR3(HP_POS_X + HP_SIZE_X, HP_POS_Y + HP_SIZE_Y, 0.0f);

	// rhwの設定
	VertexWkhp[0].rhw =
	VertexWkhp[1].rhw =
	VertexWkhp[2].rhw =
	VertexWkhp[3].rhw = 1.0f;

	// 反射光の設定
	VertexWkhp[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkhp[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkhp[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkhp[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	VertexWkhp[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkhp[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkhp[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkhp[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexGauge(float val)
{
	// 頂点座標の設定
	VertexWkGauge[0].vtx = D3DXVECTOR3(GAUGE00_POS_X, GAUGE00_POS_Y, 0.0f); 
	VertexWkGauge[1].vtx = D3DXVECTOR3(GAUGE00_POS_X + (GAUGE00_SIZE_X * val), GAUGE00_POS_Y, 0.0f);
	VertexWkGauge[2].vtx = D3DXVECTOR3(GAUGE00_POS_X, GAUGE00_POS_Y + GAUGE00_SIZE_Y, 0.0f);
	VertexWkGauge[3].vtx = D3DXVECTOR3(GAUGE00_POS_X + (GAUGE00_SIZE_X * val), GAUGE00_POS_Y + GAUGE00_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureGauge(float val)
{
	// テクスチャ座標の設定
	VertexWkGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkGauge[1].tex = D3DXVECTOR2(1.0f * val, 0.0f);
	VertexWkGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkGauge[3].tex = D3DXVECTOR2(1.0f * val, 1.0f);
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexGaugeFrame(void)
{
	// 頂点座標の設定
	VertexWkGauge[0].vtx = D3DXVECTOR3(GAUGEFRAME00_POS_X, GAUGEFRAME00_POS_Y, 0.0f);
	VertexWkGauge[1].vtx = D3DXVECTOR3(GAUGEFRAME00_POS_X + GAUGEFRAME00_SIZE_X, GAUGEFRAME00_POS_Y, 0.0f);
	VertexWkGauge[2].vtx = D3DXVECTOR3(GAUGEFRAME00_POS_X, GAUGEFRAME00_POS_Y + GAUGEFRAME00_SIZE_Y, 0.0f);
	VertexWkGauge[3].vtx = D3DXVECTOR3(GAUGEFRAME00_POS_X + GAUGEFRAME00_SIZE_X, GAUGEFRAME00_POS_Y + GAUGEFRAME00_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureGaugeFrame(void)
{
	// テクスチャ座標の設定
	VertexWkGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

