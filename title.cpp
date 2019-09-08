//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 Device);
void SetColorTitleLogo(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureTitle = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffTitle = NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		D3DTextureTitleLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffTitleLogo = NULL;	// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		D3DTextureStart = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffStart = NULL;		// 頂点バッファインターフェースへのポインタ
int						CountAppearStart = 0;		//
float					AlphaLogo = 0.0f;			//
int						CountDisp = 0;				//
bool					DispStart = false;			//
int						ConutDemo = 0;				//

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	CountAppearStart = 0;
	AlphaLogo = 0.0f;
	CountDisp = 0;
	DispStart = false;
	ConutDemo = 0;

	// 頂点情報の作成
	MakeVertexTitle(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
								TEXTURE_TITLE,				// ファイルの名前
								&D3DTextureTitle);		// 読み込むメモリー

	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
								TEXTURE_TITLE_LOGO,			// ファイルの名前
								&D3DTextureTitleLogo);	// 読み込むメモリー

	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
								TEXTURE_LOGO_START,			// ファイルの名前
								&D3DTextureStart);		// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if(D3DTextureTitle != NULL)
	{// テクスチャの開放
		D3DTextureTitle->Release();
		D3DTextureTitle = NULL;
	}

	if(D3DVtxBuffTitle != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffTitle->Release();
		D3DVtxBuffTitle = NULL;
	}

	if(D3DTextureTitleLogo != NULL)
	{// テクスチャの開放
		D3DTextureTitleLogo->Release();
		D3DTextureTitleLogo = NULL;
	}

	if(D3DVtxBuffTitleLogo != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffTitleLogo->Release();
		D3DVtxBuffTitleLogo = NULL;
	}

	if(D3DTextureStart != NULL)
	{// テクスチャの開放
		D3DTextureStart->Release();
		D3DTextureStart = NULL;
	}

	if(D3DVtxBuffStart != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffStart->Release();
		D3DVtxBuffStart = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	// SE停止
	StopSound(SOUND_LABEL_SE_TRUE);

	// SE停止
	StopSound(SOUND_LABEL_SE_FALSE);

	if(AlphaLogo < 1.0f)
	{
		AlphaLogo += 0.005f;
		if(AlphaLogo >= 1.0f)
		{
			AlphaLogo = 1.0f;
		}
		SetColorTitleLogo();
	}
	else
	{
		CountAppearStart++;
		if(CountAppearStart > COUNT_APPERA_START)
		{
			CountDisp = (CountDisp + 1) % 80;
			if(CountDisp > INTERVAL_DISP_START)
			{
				DispStart = false;
			}
			else
			{
				DispStart = true;
			}
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN)|| IsButtonTriggered(0, BUTTON_TOUCH))
	{
		if(CountAppearStart == 0)
		{// タイトル登場スキップ
			AlphaLogo = 1.0f;

			SetColorTitleLogo();

			CountAppearStart = COUNT_APPERA_START;
		}
		else
		{// ゲームへ

			// SEの再生
			PlaySound(SOUND_LABEL_SE_ENTER);

			// フェードアウト
			SetFade(FADE_OUT);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    Device->SetStreamSource(0, D3DVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureTitle);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// 頂点バッファをデバイスのデータストリームにバインド
    Device->SetStreamSource(0, D3DVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureTitleLogo);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	if(DispStart == true)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		Device->SetStreamSource(0, D3DVtxBuffStart, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTextureStart);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffTitle,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffTitle->Lock(0, 0, (void**)&Vtx, 0);

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
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffTitle->Unlock();
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffTitleLogo,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffTitleLogo->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffTitleLogo->Unlock();
	}


	// オブジェクトの頂点バッファを生成
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffStart,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffStart->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y + START_HEIGHT, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffStart->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点の作成
//=============================================================================
void SetColorTitleLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffTitleLogo->Lock(0, 0, (void**)&Vtx, 0);

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, AlphaLogo);

		// 頂点データをアンロックする
		D3DVtxBuffTitleLogo->Unlock();
	}

}

