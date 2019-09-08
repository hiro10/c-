//=============================================================================
//
// UI処理 [ui.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "main.h"
#include "ui.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "enemy.h"
#include "ace.h"
#include "boss.h"
#include "item.h"
#include "sound.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexUi(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureUiLogo = NULL;		// テクスチャへのポインタ
VERTEX_2D				vertexWkUiLogo[NUM_VERTEX];		// 頂点情報格納ワーク

LPDIRECT3DTEXTURE9		D3DTextureUiAceLogo = NULL;		// テクスチャへのポインタ
VERTEX_2D				vertexWkUiAceLogo[NUM_VERTEX];	// 頂点情報格納ワーク

LPDIRECT3DTEXTURE9		D3DTextureUibossLogo = NULL;	// テクスチャへのポインタ
VERTEX_2D				vertexWkUibossLogo[NUM_VERTEX];	// 頂点情報格納ワーク

LPDIRECT3DTEXTURE9		D3DTextureUiLogo2 = NULL;		// テクスチャへのポインタ
VERTEX_2D				vertexWkUiLogo2[NUM_VERTEX];	// 頂点情報格納ワーク

// 点滅処理用変数
static bool flgFlash1 = true;							// 点滅用flg

int uicount = 0;									// 点滅用カウント
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUi(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DXCreateTextureFromFile(Device,	// デバイスへのポインタ
		TEXTURE_UI_LOGO1,				// ファイルの名前
		&D3DTextureUiLogo);				// 読み込むメモリー

	D3DXCreateTextureFromFile(Device,	// デバイスへのポインタ
		TEXTURE_UIACE_LOGO,				// ファイルの名前
		&D3DTextureUiAceLogo);			// 読み込むメモリー

	D3DXCreateTextureFromFile(Device,	// デバイスへのポインタ
		TEXTURE_UIITEM_LOGO,			// ファイルの名前
		&D3DTextureUibossLogo);			// 読み込むメモリー

	D3DXCreateTextureFromFile(Device,	// デバイスへのポインタ
		TEXTURE_UI2_LOGO,				// ファイルの名前
		&D3DTextureUiLogo2);			// 読み込むメモリー

	MakeVertexUi();						// 頂点情報の作成

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUi(void)
{
	if (D3DTextureUiLogo != NULL)
	{// テクスチャの開放
		D3DTextureUiLogo->Release();
		D3DTextureUiLogo = NULL;
	}

	if (D3DTextureUiLogo2 != NULL)
	{// テクスチャの開放
		D3DTextureUiLogo2->Release();
		D3DTextureUiLogo2 = NULL;
	}

	if (D3DTextureUiAceLogo != NULL)
	{// テクスチャの開放
		D3DTextureUiAceLogo->Release();
		D3DTextureUiAceLogo = NULL;
	}

	if (D3DTextureUibossLogo != NULL)
	{// テクスチャの開放
		D3DTextureUibossLogo->Release();
		D3DTextureUibossLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateUi(void)
{
	// 点滅用のカウンタをインクリメント
	uicount++;

	if (GetMode() == MODE_GAME)
	{
		// カウンタが30より下なら
		if (uicount > MODE_GAME_COUNT)
		{
			// flgをfalseに
			flgFlash1 = !flgFlash1;

			// countの初期化
			uicount = 0;
		}
	}
	else if ((GetMode() == MODE_BOSS))
	{
		// カウンタが30より下なら
		if (uicount > MODE_BOSS_COUNT)
		{
			// flgをfalseに
			flgFlash1 = !flgFlash1;

			// countの初期化
			uicount = 0;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUi(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// エースの取得
	Ace*ace;
	ace = GetAce();

	// ボスの取得
	Boss*boss;
	boss = GetBoss();

	// エースが未使用でHPが0より上なら
	if ((GetMode() == MODE_GAME)&&ace->use == false && ace->hp > 0)
	{
		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTextureUiLogo);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkUiLogo, sizeof(VERTEX_2D));
	}
	
	// エースが使用中なら
	else if((GetMode() == MODE_GAME) && ace->use == true)
	{
		// 点滅用の処理を入れる
		if (flgFlash1 == true)
		{
			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureUiAceLogo);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkUiAceLogo, sizeof(VERTEX_2D));
		}
	}

	// ボスが使用中なら
	if (boss->use == true)
	{
		if (boss->pos.y > 1200)
		{
			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureUiLogo2);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkUiLogo2, sizeof(VERTEX_2D));

			// 点滅用の処理を入れる
			if (flgFlash1 == true)
			{
				// 頂点フォーマットの設定
				Device->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				Device->SetTexture(0, D3DTextureUibossLogo);

				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkUibossLogo, sizeof(VERTEX_2D));
			}
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexUi(void)
{

	// 頂点座標の設定
	vertexWkUiLogo[0].vtx = D3DXVECTOR3(UILOGO1_POS_X, UILOGO1_POS_Y, 0.0f);
	vertexWkUiLogo[1].vtx = D3DXVECTOR3(UILOGO1_POS_X + UILOGO1_SIZE_X, UILOGO1_POS_Y, 0.0f);
	vertexWkUiLogo[2].vtx = D3DXVECTOR3(UILOGO1_POS_X, UILOGO1_POS_Y + UILOGO1_SIZE_Y, 0.0f);
	vertexWkUiLogo[3].vtx = D3DXVECTOR3(UILOGO1_POS_X + UILOGO1_SIZE_X, UILOGO1_POS_Y + UILOGO1_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	vertexWkUiLogo[0].rhw =
		vertexWkUiLogo[1].rhw =
		vertexWkUiLogo[2].rhw =
		vertexWkUiLogo[3].rhw = 1.0f;

	//// 反射光の設定
	vertexWkUiLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWkUiLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkUiLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkUiLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkUiLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	vertexWkUiAceLogo[0].vtx = D3DXVECTOR3(UIACELOGO1_POS_X, UIACELOGO1_POS_Y, 0.0f);
	vertexWkUiAceLogo[1].vtx = D3DXVECTOR3(UIACELOGO1_POS_X + UIACELOGO1_SIZE_X, UIACELOGO1_POS_Y, 0.0f);
	vertexWkUiAceLogo[2].vtx = D3DXVECTOR3(UIACELOGO1_POS_X, UIACELOGO1_POS_Y + UIACELOGO1_SIZE_Y, 0.0f);
	vertexWkUiAceLogo[3].vtx = D3DXVECTOR3(UIACELOGO1_POS_X + UIACELOGO1_SIZE_X, UIACELOGO1_POS_Y + UIACELOGO1_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用p
	vertexWkUiAceLogo[0].rhw =
		vertexWkUiAceLogo[1].rhw =
		vertexWkUiAceLogo[2].rhw =
		vertexWkUiAceLogo[3].rhw = 1.0f;

	//// 反射光の設定
	vertexWkUiAceLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiAceLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiAceLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiAceLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWkUiAceLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkUiAceLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkUiAceLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkUiAceLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	vertexWkUibossLogo[0].vtx = D3DXVECTOR3(UIITEMLOGO1_POS_X, UIITEMLOGO1_POS_Y, 0.0f);
	vertexWkUibossLogo[1].vtx = D3DXVECTOR3(UIITEMLOGO1_POS_X + UIITEMLOGO1_SIZE_X, UIITEMLOGO1_POS_Y, 0.0f);
	vertexWkUibossLogo[2].vtx = D3DXVECTOR3(UIITEMLOGO1_POS_X, UIITEMLOGO1_POS_Y + UIITEMLOGO1_SIZE_Y, 0.0f);
	vertexWkUibossLogo[3].vtx = D3DXVECTOR3(UIITEMLOGO1_POS_X + UIITEMLOGO1_SIZE_X, UIITEMLOGO1_POS_Y + UIITEMLOGO1_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用p
	vertexWkUibossLogo[0].rhw =
		vertexWkUibossLogo[1].rhw =
		vertexWkUibossLogo[2].rhw =
		vertexWkUibossLogo[3].rhw = 1.0f;

	//// 反射光の設定
	vertexWkUibossLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUibossLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUibossLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUibossLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWkUibossLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkUibossLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkUibossLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkUibossLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	vertexWkUiLogo2[0].vtx = D3DXVECTOR3(UILOGO2_POS_X, UILOGO2_POS_Y, 0.0f);
	vertexWkUiLogo2[1].vtx = D3DXVECTOR3(UILOGO2_POS_X + UILOGO2_SIZE_X, UILOGO2_POS_Y, 0.0f);
	vertexWkUiLogo2[2].vtx = D3DXVECTOR3(UILOGO2_POS_X, UILOGO2_POS_Y + UILOGO2_SIZE_Y, 0.0f);
	vertexWkUiLogo2[3].vtx = D3DXVECTOR3(UILOGO2_POS_X + UILOGO2_SIZE_X, UILOGO2_POS_Y + UILOGO2_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	vertexWkUiLogo2[0].rhw =
		vertexWkUiLogo2[1].rhw =
		vertexWkUiLogo2[2].rhw =
		vertexWkUiLogo2[3].rhw = 1.0f;

	//// 反射光の設定
	vertexWkUiLogo2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkUiLogo2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWkUiLogo2[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkUiLogo2[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkUiLogo2[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkUiLogo2[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	return S_OK;
}