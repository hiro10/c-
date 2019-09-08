//=============================================================================
//
// ボスのＨＰゲージ処理 [bossgauge.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "main.h"
#include "boss.h"
#include "bossgauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBossGauge(void);
void SetVertexBossGauge(float val);
void SetTextureBossGauge(float val);
void SetVertexBossGaugeFrame(void);
void SetTextureBossGaugeFrame(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBossGauge = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		D3DTextureBossGaugeFrame = NULL;	// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		D3DTexturebosshp = NULL;			// テクスチャへのポインタ

VERTEX_2D				VertexWkbosshp[NUM_VERTEX];		// 頂点情報格納ワーク
VERTEX_2D				VertexWkBossGauge[NUM_VERTEX];	// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBossGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,	// デバイスへのポインタ
		TEXTURE_GAME_BOSSGAUGE00,		// ファイルの名前
		&D3DTextureBossGauge);			// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,	// デバイスへのポインタ
		TEXTURE_GAME_BOSSGAUGEFRAME00,	// ファイルの名前
		&D3DTextureBossGaugeFrame);		// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,	// デバイスのポインタ
		TEXTURE_BOSSHP1,				// ファイルの名前
		&D3DTexturebosshp);				// 読み込むメモリのポインタ

	// 頂点情報の作成
	MakeVertexBossGauge();

	SetVertexBossGauge(100.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBossGauge(void)
{
	if (D3DTextureBossGauge != NULL)
	{// テクスチャの開放
		D3DTextureBossGauge->Release();
		D3DTextureBossGauge = NULL;
	}

	if (D3DTexturebosshp != NULL)
	{// テクスチャの開放
		D3DTexturebosshp->Release();
		D3DTexturebosshp = NULL;
	}

	if (D3DTextureBossGaugeFrame != NULL)
	{//	テクスチャの開放
		D3DTextureBossGaugeFrame->Release();
		D3DTextureBossGaugeFrame = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBossGauge(void)
{
	// 毎フレーム実行する処理
	Boss *boss = GetBoss();
	;
	float val = boss->hp / (float)(BOSS_LIFEMAX);

	SetVertexBossGauge(val);
	SetTextureBossGauge(val);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBossGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Boss *boss = GetBoss();


	//頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureBossGauge);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkBossGauge, sizeof(VERTEX_2D));

	//ボスゲージのSET
	SetVertexBossGaugeFrame();
	SetTextureBossGaugeFrame();

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureBossGaugeFrame);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkBossGauge, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定(TEXTURE_TITLE)
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(TEXTURE_TITLE)
	Device->SetTexture(0, D3DTexturebosshp);

	// ポリゴンの描画(TEXTURE_TITLE)
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkbosshp, sizeof(VERTEX_2D));

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBossGauge(void)
{
	// 頂点座標の設定
	VertexWkBossGauge[0].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X, BOSSGAUGE00_POS_Y, 0.0f);
	VertexWkBossGauge[1].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X + BOSSGAUGE00_SIZE_X, BOSSGAUGE00_POS_Y, 0.0f);
	VertexWkBossGauge[2].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X, BOSSGAUGE00_POS_Y + BOSSGAUGE00_SIZE_Y, 0.0f);
	VertexWkBossGauge[3].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X + BOSSGAUGE00_SIZE_X, BOSSGAUGE00_POS_Y + BOSSGAUGE00_SIZE_Y, 0.0f);

	// rhwの設定
	VertexWkBossGauge[0].rhw =
		VertexWkBossGauge[1].rhw =
		VertexWkBossGauge[2].rhw =
		VertexWkBossGauge[3].rhw = 1.0f;

	// 反射光の設定
	VertexWkBossGauge[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkBossGauge[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkBossGauge[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkBossGauge[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	VertexWkBossGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkBossGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkBossGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkBossGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	VertexWkbosshp[0].vtx = D3DXVECTOR3(BOSSHP1_POS_X, BOSSHP1_POS_Y, 0.0f);
	VertexWkbosshp[1].vtx = D3DXVECTOR3(BOSSHP1_POS_X + BOSSHP1_SIZE_X, BOSSHP1_POS_Y, 0.0f);
	VertexWkbosshp[2].vtx = D3DXVECTOR3(BOSSHP1_POS_X, BOSSHP1_POS_Y + BOSSHP1_SIZE_Y, 0.0f);
	VertexWkbosshp[3].vtx = D3DXVECTOR3(BOSSHP1_POS_X + BOSSHP1_SIZE_X, BOSSHP1_POS_Y + BOSSHP1_SIZE_Y, 0.0f);

	// rhwの設定
	VertexWkbosshp[0].rhw =
		VertexWkbosshp[1].rhw =
		VertexWkbosshp[2].rhw =
		VertexWkbosshp[3].rhw = 1.0f;

	// 反射光の設定
	VertexWkbosshp[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkbosshp[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkbosshp[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkbosshp[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	VertexWkbosshp[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkbosshp[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkbosshp[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkbosshp[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBossGauge(float val)
{
	// 頂点座標の設定
	VertexWkBossGauge[0].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X, BOSSGAUGE00_POS_Y, 0.0f);
	VertexWkBossGauge[1].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X + (BOSSGAUGE00_SIZE_X * val), BOSSGAUGE00_POS_Y, 0.0f);
	VertexWkBossGauge[2].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X, BOSSGAUGE00_POS_Y + BOSSGAUGE00_SIZE_Y, 0.0f);
	VertexWkBossGauge[3].vtx = D3DXVECTOR3(BOSSGAUGE00_POS_X + (BOSSGAUGE00_SIZE_X * val), BOSSGAUGE00_POS_Y + BOSSGAUGE00_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBossGauge(float val)
{
	// テクスチャ座標の設定
	VertexWkBossGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkBossGauge[1].tex = D3DXVECTOR2(1.0f * val, 0.0f);
	VertexWkBossGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkBossGauge[3].tex = D3DXVECTOR2(1.0f * val, 1.0f);
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBossGaugeFrame(void)
{
	// 頂点座標の設定
	VertexWkBossGauge[0].vtx = D3DXVECTOR3(BOSSGAUGEFRAME00_POS_X, BOSSGAUGEFRAME00_POS_Y, 0.0f);
	VertexWkBossGauge[1].vtx = D3DXVECTOR3(BOSSGAUGEFRAME00_POS_X + BOSSGAUGEFRAME00_SIZE_X, BOSSGAUGEFRAME00_POS_Y, 0.0f);
	VertexWkBossGauge[2].vtx = D3DXVECTOR3(BOSSGAUGEFRAME00_POS_X, BOSSGAUGEFRAME00_POS_Y + BOSSGAUGEFRAME00_SIZE_Y, 0.0f);
	VertexWkBossGauge[3].vtx = D3DXVECTOR3(BOSSGAUGEFRAME00_POS_X + BOSSGAUGEFRAME00_SIZE_X, BOSSGAUGEFRAME00_POS_Y + BOSSGAUGEFRAME00_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBossGaugeFrame(void)
{
	// テクスチャ座標の設定
	VertexWkBossGauge[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkBossGauge[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkBossGauge[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkBossGauge[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

