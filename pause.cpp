//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "sound.h"
#include "game.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPause(void);


//*****************************************************************************
// グローバル変数
//**********************************************************************
LPDIRECT3DTEXTURE9		D3DTexturePauseLogo = NULL;				// テクスチャへのポインタ
VERTEX_2D				VertexWkPauseLogo[NUM_VERTEX];			// 頂点情報格納ワーク

LPDIRECT3DTEXTURE9		D3DTextureReturnTitleLogo = NULL;		// テクスチャへのポインタ
VERTEX_2D				VertexWkReturnTitleLogo[NUM_VERTEX];	// 頂点情報格納ワーク


bool FlgFlash = true;								// 点滅用flg
static int count = 0;								// 点滅用カウント
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DXCreateTextureFromFile(Device,	// デバイスへのポインタ
		TEXTURE_PAUSE_LOGO1,			// ファイルの名前
		&D3DTexturePauseLogo);			// 読み込むメモリー

	D3DXCreateTextureFromFile(Device,	// デバイスへのポインタ
		TEXTURE_RETURNTITLE_LOGO1,			// ファイルの名前
		&D3DTextureReturnTitleLogo);			// 読み込むメモリー
								
	MakeVertexPause();					// 頂点情報の作成

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	if (D3DTexturePauseLogo != NULL)
	{// テクスチャの開放
		D3DTexturePauseLogo->Release();
		D3DTexturePauseLogo = NULL;
	}

	if (D3DTextureReturnTitleLogo != NULL)
	{// テクスチャの開放
		D3DTextureReturnTitleLogo->Release();
		D3DTextureReturnTitleLogo = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
	count++;					// 点滅用のカウンタをインクリメント

	if (count > 30)
	{
		FlgFlash = !FlgFlash;	// flgをfalseに

		count = 0;				// countの初期化
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (FlgFlash == true )
	{
		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTexturePauseLogo);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkPauseLogo, sizeof(VERTEX_2D));
	}

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureReturnTitleLogo);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkReturnTitleLogo, sizeof(VERTEX_2D));
	
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPause(void)
{

	// 頂点座標の設定
	VertexWkPauseLogo[0].vtx = D3DXVECTOR3(PAUSELOGO1_POS_X, PAUSELOGO1_POS_Y, 0.0f);
	VertexWkPauseLogo[1].vtx = D3DXVECTOR3(PAUSELOGO1_POS_X + PAUSELOGO1_SIZE_X, PAUSELOGO1_POS_Y, 0.0f);
	VertexWkPauseLogo[2].vtx = D3DXVECTOR3(PAUSELOGO1_POS_X, PAUSELOGO1_POS_Y + PAUSELOGO1_SIZE_Y, 0.0f);
	VertexWkPauseLogo[3].vtx = D3DXVECTOR3(PAUSELOGO1_POS_X + PAUSELOGO1_SIZE_X, PAUSELOGO1_POS_Y + PAUSELOGO1_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	VertexWkPauseLogo[0].rhw =
		VertexWkPauseLogo[1].rhw =
		VertexWkPauseLogo[2].rhw =
		VertexWkPauseLogo[3].rhw = 1.0f;

	//// 反射光の設定
	VertexWkPauseLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkPauseLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkPauseLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkPauseLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	VertexWkPauseLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkPauseLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkPauseLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkPauseLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	VertexWkReturnTitleLogo[0].vtx = D3DXVECTOR3(RETURNTITLELOGO1_POS_X, RETURNTITLELOGO1_POS_Y, 0.0f);
	VertexWkReturnTitleLogo[1].vtx = D3DXVECTOR3(RETURNTITLELOGO1_POS_X + RETURNTITLELOGO1_SIZE_X, RETURNTITLELOGO1_POS_Y, 0.0f);
	VertexWkReturnTitleLogo[2].vtx = D3DXVECTOR3(RETURNTITLELOGO1_POS_X, RETURNTITLELOGO1_POS_Y + RETURNTITLELOGO1_SIZE_Y, 0.0f);
	VertexWkReturnTitleLogo[3].vtx = D3DXVECTOR3(RETURNTITLELOGO1_POS_X + RETURNTITLELOGO1_SIZE_X, RETURNTITLELOGO1_POS_Y + RETURNTITLELOGO1_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用p
	VertexWkReturnTitleLogo[0].rhw =
		VertexWkReturnTitleLogo[1].rhw =
		VertexWkReturnTitleLogo[2].rhw =
		VertexWkReturnTitleLogo[3].rhw = 1.0f;

	//// 反射光の設定
	VertexWkReturnTitleLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkReturnTitleLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkReturnTitleLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkReturnTitleLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	VertexWkReturnTitleLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkReturnTitleLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkReturnTitleLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkReturnTitleLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}