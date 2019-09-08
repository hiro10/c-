//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "timer.h"
#include "fade.h"
#include "ace.h"
#include "boss.h"
#include "player.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTimer(LPDIRECT3DDEVICE9 Device);
void SetTextureTimer(int idx, int number);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureTimer[2] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffTimer = NULL;		// 頂点バッファインターフェースへのポインタ

int						Timer;						// タイマー
bool					EnableTimer;					// タイマー動作ON/OFF

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimer(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// タイマーの初期化
	Timer = 999 * 60;
	EnableTimer = true;

	// 頂点情報の作成
	MakeVertexTimer(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
								TEXTURE_TIMER,			// ファイルの名前
								&D3DTextureTimer[0]);// 読み込むメモリー

	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
								TEXTURE_FRAME_TIMER,	// ファイルの名前
								&D3DTextureTimer[1]);// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{
	if(D3DTextureTimer[0] != NULL)
	{// テクスチャの開放
		D3DTextureTimer[0]->Release();
		D3DTextureTimer[0] = NULL;
	}

	if(D3DTextureTimer[1] != NULL)
	{// テクスチャの開放
		D3DTextureTimer[1]->Release();
		D3DTextureTimer[1] = NULL;
	}

	if(D3DVtxBuffTimer != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffTimer->Release();
		D3DVtxBuffTimer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer(void)
{
	// エースの取得
	Ace*ace;
	ace = GetAce();

	// ボスの取得
	Boss*boss;
	boss = GetBoss();

	// プレイヤーの取得
	Player*player;
	player = GetPlayer();

	if (EnableTimer)
	{
		if (ace->hp > ACE_DEAD||boss->hp>0)
		{
			//タイムをデクリメント
			Timer--;
		}
		if (Timer < 0)
		{//タイムが0より下なら
			Timer = 0;

			if (GetMode() == MODE_BOSS)
			{
				// プレイヤーを死に
				player->hp = PLAYER_DEAD;
			}


			//フェードアウト
			SetFade(FADE_OUT);
		}
		else if ((Timer / 60) >= (int)(powf(10.0f, (float)(NUM_TIMER + 1))))
		{
			Timer = (int)(powf(10.0f, (float)(NUM_TIMER + 1))) - 1;
		}
	}

	for(int CntPlace = 0; CntPlace < NUM_TIMER; CntPlace++)
	{
		int number;
		int timer;
		
		timer = Timer + 59;
		number = ((timer / 60) % (int)(powf(10.0f, (float)(NUM_TIMER - CntPlace)))) / (int)(powf(10.0f, (float)(NUM_TIMER - CntPlace - 1)));
		SetTextureTimer(CntPlace, number);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    Device->SetStreamSource(0, D3DVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureTimer[0]);

	// ポリゴンの描画
	for(int CntPlace = 0; CntPlace < NUM_TIMER; CntPlace++)
	{
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntPlace * 4), NUM_POLYGON);
	}

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureTimer[1]);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_TIMER * 4), NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTimer(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_TIMER + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
												FVF_VERTEX_2D,									// 使用する頂点フォーマット
												D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffTimer,							// 頂点バッファインターフェースへのポインタ
												NULL)))											// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffTimer->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntPlace = 0; CntPlace < NUM_TIMER; CntPlace++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(TIMER_POS_X + CntPlace * TIMER_SIZE_X + TIMER_INTERVAL_X, TIMER_POS_Y, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(TIMER_POS_X + CntPlace * (TIMER_INTERVAL_X + TIMER_SIZE_X) + TIMER_SIZE_X, TIMER_POS_Y, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(TIMER_POS_X + CntPlace * TIMER_SIZE_X + TIMER_INTERVAL_X, TIMER_POS_Y + TIMER_SIZE_Y, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(TIMER_POS_X + CntPlace * (TIMER_INTERVAL_X + TIMER_SIZE_X) + TIMER_SIZE_X, TIMER_POS_Y + TIMER_SIZE_Y, 0.0f);

			// rhwの設定
			Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

			// 反射光の設定
			Vtx[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			Vtx[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			Vtx[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			Vtx[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

			// テクスチャ座標の設定
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(TIMER_POS_X - 17.5f, TIMER_POS_Y - 25, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(TIMER_POS_X + (TIMER_INTERVAL_X + TIMER_SIZE_X) * NUM_TIMER + 17.5f, TIMER_POS_Y - 25, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(TIMER_POS_X - 17.5f, TIMER_POS_Y + 55, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(TIMER_POS_X + (TIMER_INTERVAL_X + TIMER_SIZE_X) * NUM_TIMER + 17.5f, TIMER_POS_Y + 55, 0.0f);

		// rhwの設定
		Vtx[0].rhw =
		Vtx[1].rhw =
		Vtx[2].rhw =
		Vtx[3].rhw = 1.0f;

		// 反射光の設定
		Vtx[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Vtx[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Vtx[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
		Vtx[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffTimer->Unlock();
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTimer(int idx, int number)
{
	VERTEX_2D *Vtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffTimer->Lock(0, 0, (void**)&Vtx, 0);

	Vtx += (idx * 4);

	// 頂点座標の設定
	Vtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	Vtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	Vtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	Vtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuffTimer->Unlock();
}

//=============================================================================
// タイマーの開始
//=============================================================================
void GetEnableTimer(bool Enable)
{
	EnableTimer = Enable;
}

//=============================================================================
// タイマーのリセット
//=============================================================================
void ResetTimer(int Time)
{
	Timer = Time * 60;
}

