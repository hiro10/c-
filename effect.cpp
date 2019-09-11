//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "effect.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 Device);
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY);
void SetColorEffect(int nIdxEffect, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureEffect = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffEffect = NULL;		// 頂点バッファインターフェースへのポインタ
D3DXMATRIX				MtxWorldEffect;				// ワールドマトリックス
Effect1					effect[MAX_EFFECT];			// エフェクトワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexEffect(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
								TEXTURE_EFFECT,			// ファイルの名前
								&D3DTextureEffect);		// 読み込むメモリー

	for(int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++)
	{
		effect[CntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect[CntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect[CntEffect].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		effect[CntEffect].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		effect[CntEffect].SizeX = EFFECT_SIZE_X;
		effect[CntEffect].SizeY = EFFECT_SIZE_Y;
		effect[CntEffect].Count = 0;
		effect[CntEffect].Pattern = 0;
		effect[CntEffect].Timer = 0;
		effect[CntEffect].use = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	if(D3DTextureEffect != NULL)
	{// テクスチャの開放
		D3DTextureEffect->Release();
		D3DTextureEffect = NULL;
	}

	if(D3DVtxBuffEffect != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffEffect->Release();
		D3DVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	for (int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++)
	{
		if (effect[CntEffect].use)
		{
			effect[CntEffect].pos.x += effect[CntEffect].move.x;
			effect[CntEffect].pos.z += effect[CntEffect].move.z;

			effect[CntEffect].col.a -= effect[CntEffect].DecAlpha;
			if (effect[CntEffect].col.a <= 0.0f)
			{
				effect[CntEffect].col.a = 0.0f;
			}
			SetColorEffect(CntEffect,
				D3DXCOLOR(effect[CntEffect].col.r, effect[CntEffect].col.b,
					effect[CntEffect].col.b, effect[CntEffect].col.a));

			effect[CntEffect].Timer--;
			if (effect[CntEffect].Timer <= 0)
			{
				effect[CntEffect].use = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxView,MtxScale,MtxTranslate;

	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし

	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	for(int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++)
	{
		if(effect[CntEffect].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&MtxWorldEffect);

			// ビューマトリックスを取得
			MtxView = GetMtxView();

			MtxWorldEffect._11 = MtxView._11;
			MtxWorldEffect._12 = MtxView._21;
			MtxWorldEffect._13 = MtxView._31;
			MtxWorldEffect._21 = MtxView._12;
			MtxWorldEffect._22 = MtxView._22;
			MtxWorldEffect._23 = MtxView._32;
			MtxWorldEffect._31 = MtxView._13;
			MtxWorldEffect._32 = MtxView._23;
			MtxWorldEffect._33 = MtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&MtxScale, effect[CntEffect].scl.x, effect[CntEffect].scl.y, effect[CntEffect].scl.z);
			D3DXMatrixMultiply(&MtxWorldEffect, &MtxWorldEffect, &MtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&MtxTranslate, effect[CntEffect].pos.x, effect[CntEffect].pos.y, effect[CntEffect].pos.z);
			D3DXMatrixMultiply(&MtxWorldEffect, &MtxWorldEffect, &MtxTranslate);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &MtxWorldEffect);

			Device->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureEffect);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntEffect * 4), NUM_POLYGON);

			Device->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z比較あり

	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_3D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffEffect,						// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffEffect->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);

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
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		D3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffEffect->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxEffect * 4);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorEffect(int nIdxEffect, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffEffect->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxEffect * 4);

		// 頂点座標の設定
		Vtx[0].diffuse =
		Vtx[1].diffuse =
		Vtx[2].diffuse =
		Vtx[3].diffuse = col;

		// 頂点データをアンロックする
		D3DVtxBuffEffect->Unlock();
	}
}
//=============================================================================
// エフェクトの設定
//=============================================================================
int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxEffect = -1;

	for(int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++)
	{
		if(!effect[CntEffect].use)
		{
			effect[CntEffect].pos = pos;
			effect[CntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			effect[CntEffect].scl= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			effect[CntEffect].move = move;
			effect[CntEffect].col = col;
			effect[CntEffect].SizeX = fSizeX;
			effect[CntEffect].SizeY = fSizeY;
			effect[CntEffect].Timer = nTimer;
			effect[CntEffect].DecAlpha = col.a / nTimer;
			effect[CntEffect].use = true;

			// 頂点座標の設定
			SetVertexEffect(CntEffect, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorEffect(CntEffect,
							D3DXCOLOR(effect[CntEffect].col.r,effect[CntEffect].col.b,
										effect[CntEffect].col.b, effect[CntEffect].col.a));

			nIdxEffect = CntEffect;

			break;
		}
	}
	return nIdxEffect;
}
