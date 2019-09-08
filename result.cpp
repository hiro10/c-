//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "boss.h"
#include "sound.h"
#include "ace.h"
#include "timer.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 Device);
void SetColorResultLogo(void);
void SetColorResultLogoClear(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureResult = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResult = NULL;		// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		D3DTextureResultClear = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultClear = NULL;		// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		D3DTextureResultLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultLogo = NULL;	// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		D3DTextureResultLogoClear = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultLogoClear = NULL;	// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		D3DTextureResultRank = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultRank = NULL;	// 頂点バッファインターフェースへのポインタ

// RANK
LPDIRECT3DTEXTURE9		D3DTextureResultLogoRank = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResultLogoRank = NULL;	// 頂点バッファインターフェースへのポインタ

// S
LPDIRECT3DTEXTURE9		D3DTextureRankS = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffRankS = NULL;	// 頂点バッファインターフェースへのポインタ

// A
LPDIRECT3DTEXTURE9		D3DTextureRankA = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffRankA = NULL;	// 頂点バッファインターフェースへのポインタ

// B
LPDIRECT3DTEXTURE9		D3DTextureRankB = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffRankB = NULL;	// 頂点バッファインターフェースへのポインタ

// C
LPDIRECT3DTEXTURE9		D3DTextureRankC = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffRankC = NULL;	// 頂点バッファインターフェースへのポインタ


int						CountAppearResult = 0;		// 出現までの待ち時間
float					AlphaResult = 1.0f;			// リザルトロゴのα値
int						CountWaitResult = 0;			// 待ち時間

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	CountAppearResult = 0;
	AlphaResult = 0.0f;
	CountWaitResult = 0;

	// 頂点情報の作成
	MakeVertexResult(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
								TEXTURE_RESULT,				// ファイルの名前
								&D3DTextureResult);		// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
						TEXTURE_RESULT_CLEAR,				// ファイルの名前
							&D3DTextureResultClear);		// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
								TEXTURE_RESULT_LOGO,		// ファイルの名前
								&D3DTextureResultLogo);	// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
							TEXTURE_RESULT_LOGO_CLEAR,		// ファイルの名前
							&D3DTextureResultLogoClear);	// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
							TEXTURE_RESULT_LOGO_RANK,		// ファイルの名前
							&D3DTextureResultLogoRank);	// 読み込むメモリー

		// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
							TEXTURE_RESULT_RANKS,			// ファイルの名前
							&D3DTextureRankS);			// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
							TEXTURE_RESULT_RANKA,			// ファイルの名前
							&D3DTextureRankA);			// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
							TEXTURE_RESULT_RANKB,			// ファイルの名前
							&D3DTextureRankB);			// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
							TEXTURE_RESULT_RANKC,			// ファイルの名前
							&D3DTextureRankC);			// 読み込むメモリー


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if(D3DTextureResult != NULL)
	{// テクスチャの開放
		D3DTextureResult->Release();
		D3DTextureResult = NULL;
	}

	if(D3DVtxBuffResult != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffResult->Release();
		D3DVtxBuffResult = NULL;
	}

	if (D3DTextureResultClear != NULL)
	{// テクスチャの開放
		D3DTextureResultClear->Release();
		D3DTextureResultClear = NULL;
	}

	if (D3DVtxBuffResultClear != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffResultClear->Release();
		D3DVtxBuffResultClear = NULL;
	}

	if(D3DTextureResultLogo != NULL)
	{// テクスチャの開放
		D3DTextureResultLogo->Release();
		D3DTextureResultLogo = NULL;
	}

	if(D3DVtxBuffResultLogo != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffResultLogo->Release();
		D3DVtxBuffResultLogo = NULL;
	}

	if (D3DTextureResultLogoClear != NULL)
	{// テクスチャの開放
		D3DTextureResultLogoClear->Release();
		D3DTextureResultLogoClear = NULL;
	}

	if (D3DVtxBuffResultLogoClear != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffResultLogoClear->Release();
		D3DVtxBuffResultLogoClear = NULL;
	}

	if (D3DTextureResultLogoRank != NULL)
	{// テクスチャの開放
		D3DTextureResultLogoRank->Release();
		D3DTextureResultLogoRank = NULL;
	}

	if (D3DVtxBuffResultLogoRank != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffResultLogoRank->Release();
		D3DVtxBuffResultLogoRank = NULL;
	}

	if (D3DTextureRankS != NULL)
	{// テクスチャの開放
		D3DTextureRankS->Release();
		D3DTextureRankS = NULL;
	}

	if (D3DVtxBuffRankS != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffRankS->Release();
		D3DVtxBuffRankS = NULL;
	}

	if (D3DTextureRankA != NULL)
	{// テクスチャの開放
		D3DTextureRankA->Release();
		D3DTextureRankA = NULL;
	}

	if (D3DVtxBuffRankA != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffRankA->Release();
		D3DVtxBuffRankA = NULL;
	}

	if (D3DTextureRankB != NULL)
	{// テクスチャの開放
		D3DTextureRankB->Release();
		D3DTextureRankB = NULL;
	}

	if (D3DVtxBuffRankB != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffRankB->Release();
		D3DVtxBuffRankB = NULL;
	}

	if (D3DTextureRankC != NULL)
	{// テクスチャの開放
		D3DTextureRankC->Release();
		D3DTextureRankC = NULL;
	}

	if (D3DVtxBuffRankC != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffRankC->Release();
		D3DVtxBuffRankC = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	// プレイヤーの取得
	Player *player;
	player = GetPlayer();

	if (AlphaResult < 1.0f)
	{
		AlphaResult += APPERA_RESULT_TIME;
		if (AlphaResult >= 1.0f)
		{
			AlphaResult = 1.0f;
		}

		SetColorResultLogo();
		SetColorResultLogoClear();
	}
	

	if(GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_TOUCH))
	{// Enter押したら、フェードアウトしてモードを切り替えいく

		SetFade(FADE_OUT);
	}

	CountWaitResult++;
	if(CountWaitResult > LIMIT_COUNT_WAIT)
	{
		SetFade(FADE_OUT);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	// プレイヤーの取得
	Player *player;
	player = GetPlayer();

	// エースの取得
	Ace *ace;
	ace = GetAce();

	if (boss->use == true ||( player->hp <= 0))
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		Device->SetStreamSource(0, D3DVtxBuffResult, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTextureResult);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


		// 頂点バッファをデバイスのデータストリームにバインド
		Device->SetStreamSource(0, D3DVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTextureResultLogo);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// RANK(文字）
		// 頂点バッファをデバイスのデータストリームにバインド
		Device->SetStreamSource(0, D3DVtxBuffResultLogoRank, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTextureResultLogoRank);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// エースが生きていたらC
		if (ace->hp > 0)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffRankC, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureRankC);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
		else
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffRankB, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureRankB);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
	else if (boss->use == false&&boss->hp<=0)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		Device->SetStreamSource(0, D3DVtxBuffResultClear, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTextureResultClear);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// 頂点バッファをデバイスのデータストリームにバインド
		Device->SetStreamSource(0, D3DVtxBuffResultLogoClear, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTextureResultLogoClear);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// RANK(文字）
		// 頂点バッファをデバイスのデータストリームにバインド
		Device->SetStreamSource(0, D3DVtxBuffResultLogoRank, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTextureResultLogoRank);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// エースが死んだら
		if (ace->hp <= 0)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffRankS, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureRankS);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}

		// エースが生きていたら
		else if (ace->hp > 0)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffRankA, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureRankA);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffResult,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffResult->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffResult->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffResultClear,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffResultClear->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffResultClear->Unlock();
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffResultLogo,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffResultLogo->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffResultLogo->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffResultLogoClear,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffResultLogoClear->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffResultLogoRank,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffResultLogoRank->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_POS_X + RESULT_LOGO_WIDTH, RANK_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y+ RESULT_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_POS_X + RESULT_LOGO_WIDTH, RANK_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffRankS,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffRankS->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffRankA,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffRankA->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffRankB,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffRankB->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffResultLogoClear->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffRankC,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffRankC->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(RANK_LOGO_POS_X, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(RANK_LOGO_POS_X + RANK_LOGO_WIDTH, RANK_LOGO_POS_Y + RANK_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffResultLogoClear->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorResultLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffResultLogo->Lock(0, 0, (void**)&Vtx, 0);

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// 頂点データをアンロックする
		D3DVtxBuffResultLogo->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorResultLogoClear(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffResultLogoClear->Lock(0, 0, (void**)&Vtx, 0);

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaResult);

		// 頂点データをアンロックする
		D3DVtxBuffResultLogoClear->Unlock();
	}
}