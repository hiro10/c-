//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "fade.h"
#include "game.h"
#include "timer.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 Device);
void SetColor(D3DCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureFade = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffFade = NULL;	// 頂点バッファインターフェースへのポインタ
D3DXCOLOR				Color;
FADE					Fade = FADE_IN;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFade(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の設定
	MakeVertexFade(Device);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFade(void)
{
	if(D3DTextureFade != NULL)
	{// テクスチャの開放
		D3DTextureFade->Release();
	}

	if(D3DVtxBuffFade != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffFade->Release();
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	if(Fade != FADE_NONE)
	{
		// フェード処理中
		if(Fade == FADE_OUT)
		{
			// フェードアウト処理
			Color.a += FADE_RATE;		// α値を加算して画面を消していく

			if(Color.a >= 1.0f)
			{
				Player *player;
				MODE mode;

				// フェードイン処理に切り替え
				Color.a = 1.0f;
				SetFade(FADE_IN);

				// 現在のモードを取得
				mode = GetMode();

				// プレイヤーの取得
				player = GetPlayer();

				// エネミー戦でHPが0以下になったら
				if (GetMode() == MODE_GAME &&(player->hp <= PLAYER_DEAD))
				{
					// 終了処理
					UninitGame();
					UninitBossGame();

					// モードを2つ進める
					mode = (MODE)((mode + 2) % MODE_MAX);	
				}
				else
				{
					// モードを１つ進める
					mode = (MODE)((mode + 1) % MODE_MAX);
				}

				// モードを設定
				SetMode(mode);
			}

			// 色を設定
			SetColor(Color);
		}

		else if(Fade == FADE_IN)
		{// フェードイン処理
			Color.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせる
			if(Color.a <= 0.0f)
			{
				// フェード処理終了
				Color.a = 0.0f;
				SetFade(FADE_NONE);
			}

			// 色を設定
			SetColor(Color);
		}
	}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    Device->SetStreamSource(0, D3DVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, NULL);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,				// 頂点バッファの使用法　
												FVF_VERTEX_2D,					// 使用する頂点フォーマット
												D3DPOOL_MANAGED,				// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffFade,				// 頂点バッファインターフェースへのポインタ
												NULL)))							// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffFade->Lock(0, 0, (void**)&Vtx, 0);

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
		Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[0].diffuse = Color;
		Vtx[1].diffuse = Color;
		Vtx[2].diffuse = Color;
		Vtx[3].diffuse = Color;

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

		// 頂点データをアンロックする
		D3DVtxBuffFade->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 色を設定
//=============================================================================
void SetColor(D3DCOLOR col)
{
	VERTEX_2D *Vtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffFade->Lock(0, 0, (void**)&Vtx, 0);

	// 反射光の設定
	Vtx[0].diffuse = col;
	Vtx[1].diffuse = col;
	Vtx[2].diffuse = col;
	Vtx[3].diffuse = col;

	// 頂点データをアンロックする
	D3DVtxBuffFade->Unlock();
}

//=============================================================================
// フェードの状態設定
//=============================================================================
void SetFade(FADE fade)
{
	Fade = fade;
}

//=============================================================================
// フェードの状態取得
//=============================================================================
FADE GetFade(void)
{
	return Fade;
}

