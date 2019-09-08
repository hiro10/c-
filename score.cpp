//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "score.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 Device);
void SetTextureScore(int idx, int number);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureScore[2] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffScore = NULL;		// 頂点バッファインターフェースへのポインタ

D3DXVECTOR3				PosScore;						// 位置
D3DXVECTOR3				RotScore;						// 向き

int						Score;						// スコア

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	PosScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// スコアの初期化
	Score = 0;

	// 頂点情報の作成
	MakeVertexScore(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
								TEXTURE_SCORE,			// ファイルの名前
								&D3DTextureScore[0]);	// 読み込むメモリー

	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
								TEXTURE_FRAME_SCORE,		// ファイルの名前
								&D3DTextureScore[1]);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	if(D3DTextureScore[0] != NULL)
	{// テクスチャの開放
		D3DTextureScore[0]->Release();
		D3DTextureScore[0] = NULL;
	}

	if(D3DTextureScore[1] != NULL)
	{// テクスチャの開放
		D3DTextureScore[1]->Release();
		D3DTextureScore[1] = NULL;
	}

	if(D3DVtxBuffScore != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffScore->Release();
		D3DVtxBuffScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	for(int CntPlace = 0; CntPlace < NUM_SCORE; CntPlace++)
	{
		int number;
		
		number = (Score % (int)(powf(10.0f, (float)(NUM_SCORE - CntPlace)))) / (int)(powf(10.0f, (float)(NUM_SCORE - CntPlace - 1)));
		SetTextureScore(CntPlace, number);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    Device->SetStreamSource(0, D3DVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureScore[0]);

	// ポリゴンの描画
	for(int CntPlace = 0; CntPlace < NUM_SCORE; CntPlace++)
	{
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntPlace * 4), NUM_POLYGON);
	}

	// テクスチャの設定
	Device->SetTexture(0, D3DTextureScore[1]);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_SCORE * 4), NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_SCORE + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
												FVF_VERTEX_2D,									// 使用する頂点フォーマット
												D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffScore,							// 頂点バッファインターフェースへのポインタ
												NULL)))											// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffScore->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntPlace = 0; CntPlace < NUM_SCORE; CntPlace++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(SCORE_POS_X + CntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(SCORE_POS_X + CntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(SCORE_POS_X + CntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y + SCORE_SIZE_Y, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + CntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y + SCORE_SIZE_Y, 0.0f);

			// rhwの設定
			Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

			// 反射光の設定
			Vtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			// テクスチャ座標の設定
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(SCORE_POS_X - 15, SCORE_POS_Y-25, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(SCORE_POS_X + (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_SCORE + 15, SCORE_POS_Y-25, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(SCORE_POS_X - 15, SCORE_POS_Y + 55, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_SCORE + 15, SCORE_POS_Y + 55, 0.0f);

			// rhwの設定
			Vtx[0].rhw =
			Vtx[1].rhw =
			Vtx[2].rhw =
			Vtx[3].rhw = 1.0f;

			// 反射光の設定
			Vtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			Vtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			// テクスチャ座標の設定
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		D3DVtxBuffScore->Unlock();
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureScore(int idx, int number)
{
	VERTEX_2D *Vtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffScore->Lock(0, 0, (void**)&Vtx, 0);

	Vtx += (idx * 4);

	// 頂点座標の設定
	Vtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	Vtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	Vtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	Vtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuffScore->Unlock();
}

//=============================================================================
// スコアの変更
//=============================================================================
void ChangeScore(int value)
{
	// スコアが0より下にならないように
	if(Score < 0)
	{
		Score = 0;
	}
	else if(Score >= (int)(powf(10.0f, (float)(NUM_SCORE + 1))))
	{
		
		Score = (int)(powf(10.0f, (float)(NUM_SCORE + 1))) - 1;
	}
	Score += value;
}

