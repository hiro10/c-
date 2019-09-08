//=============================================================================
//
// ビルボード処理 [billboard.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "billboard.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "enemy.h"
#include "boss.h"
#include "main.h"
#include "ace.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 Device);
void SetVertexBillboard(float fSizeX, float fSizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBillboard = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBillboard = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				MtxWorldBillboard;			// ワールドマトリックス
D3DXVECTOR3				PosBillboard;				// 位置
D3DXVECTOR3				SclBillboard;				// スケール
D3DXVECTOR3				RotBillboard;				// 回転
D3DXVECTOR3				MoveBillboard;				// 移動量
int						IdxShadowBillboard;			// 影ID
bool					BillboardUse;				// 使用、未使用判定
static bool				AlpaTest;					// アルファテストON/OFF
static int				Alpha;						// アルファテストの閾値


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBillboard(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Player*player;
	player = GetPlayer();

	// 頂点情報の作成
	MakeVertexBillboard(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
								TEXTURE_BILLBOARD,			// ファイルの名前
								&D3DTextureBillboard);		// 読み込むメモリー

	PosBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SclBillboard = D3DXVECTOR3(16.0f, 13.0f, 13.0f);
	RotBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MoveBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 影の設定
	SetColorShadow(IdxShadowBillboard, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	// ビルボードを使用に
	BillboardUse =true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBillboard(void)
{
	if(D3DTextureBillboard != NULL)
	{// テクスチャの開放
		D3DTextureBillboard->Release();
		D3DTextureBillboard = NULL;
	}

	if(D3DVtxBuffBillboard != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffBillboard->Release();
		D3DVtxBuffBillboard = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBillboard(void)
{
	// エースの取得
	Ace*ace;
	ace = GetAce();

	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	if (BillboardUse == true)
	{
		if (GetMode() == MODE_GAME)
		{//モードが1の時
			PosBillboard = GetAcePos();
		}

		if (GetMode() == MODE_BOSS)
		{//ボス戦の時
			PosBillboard = GetBossPos();
			if (boss->use == false)
			{// ボスがやられたら未使用に
				BillboardUse = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxView, MtxScale, MtxTranslate;
	if (BillboardUse == true)
	{
		// ラインティングを無効にする
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ビューマトリックスを取得
		MtxView = GetMtxView();

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&MtxWorldBillboard);

		// ポリゴンを正面に向ける
		MtxWorldBillboard._11 = MtxView._11;
		MtxWorldBillboard._21 = MtxView._12;
		MtxWorldBillboard._31 = MtxView._13;
		MtxWorldBillboard._12 = MtxView._21;
		MtxWorldBillboard._22 = MtxView._22;
		MtxWorldBillboard._32 = MtxView._23;
		MtxWorldBillboard._13 = MtxView._31;
		MtxWorldBillboard._23 = MtxView._32;
		MtxWorldBillboard._33 = MtxView._33;

		// スケールを反映
		D3DXMatrixScaling(&MtxScale, SclBillboard.x,
			SclBillboard.y,
			SclBillboard.z);
		D3DXMatrixMultiply(&MtxWorldBillboard,
			&MtxWorldBillboard, &MtxScale);

		// 移動を反映
		D3DXMatrixTranslation(&MtxTranslate, PosBillboard.x,
			PosBillboard.y,
			PosBillboard.z);
		D3DXMatrixMultiply(&MtxWorldBillboard,
			&MtxWorldBillboard, &MtxTranslate);

		// ワールドマトリックスの設定
		Device->SetTransform(D3DTS_WORLD, &MtxWorldBillboard);

		// 頂点バッファをデバイスのデータストリームにバインド
		Device->SetStreamSource(0, D3DVtxBuffBillboard, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		Device->SetTexture(0, D3DTextureBillboard);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ラインティングを有効にする
		Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}
//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffBillboard,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBillboard->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);

		// 法線の設定
		Vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		Vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		Vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		Vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		Vtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffBillboard->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBillboard(float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBillboard->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffBillboard->Unlock();
	}
}
