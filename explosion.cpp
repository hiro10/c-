//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "explosion.h"
#include "input.h"
#include "camera.h"
#include "boss.h"
#include "sound.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 Device);
void SetVertexExplosion(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorExplosion(int nIdxExplosion, D3DXCOLOR col);
void SetTextureExplosion(int nIdxExplosion, int nPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureExplosion = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffExplosion = NULL;	// 頂点バッファインターフェースへのポインタ
D3DXMATRIX				MtxWorldExplosion;			// ワールドマトリックス
EXPLOSION				Explosion[MAX_EXPLOSION];	// 爆発ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexExplosion(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						// デバイスへのポインタ
								TEXTURE_EXPLOSION,			// ファイルの名前
								&D3DTextureExplosion);	// 読み込むメモリー

	for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++)
	{
		Explosion[CntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Explosion[CntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Explosion[CntExplosion].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Explosion[CntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Explosion[CntExplosion].SizeX = EXPLOSION_SIZE_X;
		Explosion[CntExplosion].SizeY = EXPLOSION_SIZE_Y;
		Explosion[CntExplosion].Counter = 0;
		Explosion[CntExplosion].Pattern = 0;
		Explosion[CntExplosion].Type = EXPLOSIONTYPE_BULLET_Player;
		Explosion[CntExplosion].use = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitExplosion(void)
{
	if(D3DTextureExplosion != NULL)
	{// テクスチャの開放
		D3DTextureExplosion->Release();
		D3DTextureExplosion = NULL;
	}

	if(D3DVtxBuffExplosion != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffExplosion->Release();
		D3DVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateExplosion(void)
{
	
	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++)
	{
		if(Explosion[CntExplosion].use)
		{
			Explosion[CntExplosion].Counter--;
			if((Explosion[CntExplosion].Counter % 4) == 0)
			{
				//パターンカウントを進める
				Explosion[CntExplosion].Pattern++;

				if(Explosion[CntExplosion].Pattern >= 8)
				{
					//未使用に
					Explosion[CntExplosion].use = false;
				}
				else
				{
					// テクスチャ座標の設定
					SetTextureExplosion(CntExplosion, Explosion[CntExplosion].Pattern);
				}
			}

			// 頂点座標の設定
			Explosion[CntExplosion].SizeX += 0.50f;
			Explosion[CntExplosion].SizeY += 0.50f;
			SetVertexExplosion(CntExplosion, Explosion[CntExplosion].SizeX, Explosion[CntExplosion].SizeY);

			if(Explosion[CntExplosion].Pattern >= 2)
			{
				// 頂点カラーの設定
				Explosion[CntExplosion].col.a -= 0.02f;
				if(Explosion[CntExplosion].col.a < 0.0f)
				{
					Explosion[CntExplosion].col.a = 0.0f;
				}
				SetColorExplosion(CntExplosion, Explosion[CntExplosion].col);
			}
		}
		// エンディング演出用に未使用に
		if (GetMode()==MODE_BOSS&&boss->use == false)
		{
			// 未使用に
			Explosion->use = false;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxView,mtxScale,mtxTranslate;

	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Z比較なし

	for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++)
	{
		if(Explosion[CntExplosion].use)
		{

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&MtxWorldExplosion);

			// ビューマトリックスを取得
			mtxView = GetMtxView();

			MtxWorldExplosion._11 = mtxView._11;
			MtxWorldExplosion._12 = mtxView._21;
			MtxWorldExplosion._13 = mtxView._31;
			MtxWorldExplosion._21 = mtxView._12;
			MtxWorldExplosion._22 = mtxView._22;
			MtxWorldExplosion._23 = mtxView._32;
			MtxWorldExplosion._31 = mtxView._13;
			MtxWorldExplosion._32 = mtxView._23;
			MtxWorldExplosion._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, Explosion[CntExplosion].scale.x, Explosion[CntExplosion].scale.y, Explosion[CntExplosion].scale.z);
			D3DXMatrixMultiply(&MtxWorldExplosion, &MtxWorldExplosion, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, Explosion[CntExplosion].pos.x, Explosion[CntExplosion].pos.y, Explosion[CntExplosion].pos.z);
			D3DXMatrixMultiply(&MtxWorldExplosion, &MtxWorldExplosion, &mtxTranslate);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &MtxWorldExplosion);

			Device->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureExplosion);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntExplosion * 4), NUM_POLYGON);

			Device->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EXPLOSION,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_3D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffExplosion,					// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffExplosion->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(-EXPLOSION_SIZE_X / 2, -EXPLOSION_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(EXPLOSION_SIZE_X / 2, -EXPLOSION_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-EXPLOSION_SIZE_X / 2, EXPLOSION_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(EXPLOSION_SIZE_X / 2, EXPLOSION_SIZE_Y / 2, 0.0f);

			// 法線の設定
			Vtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[1].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[2].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[3].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
		D3DVtxBuffExplosion->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexExplosion(int nIdxExplosion, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffExplosion->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxExplosion * 4);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorExplosion(int nIdxExplosion, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffExplosion->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxExplosion * 4);

		// 頂点座標の設定
		Vtx[0].diffuse =
		Vtx[1].diffuse =
		Vtx[2].diffuse =
		Vtx[3].diffuse = col;

		// 頂点データをアンロックする
		D3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureExplosion(int IdxExplosion, int Pattern)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffExplosion->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxExplosion * 4);

		// 頂点座標の設定
		Vtx[0].tex = D3DXVECTOR2(Pattern * 0.125f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2((Pattern + 1) * 0.125f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(Pattern * 0.125f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2((Pattern + 1) * 0.125f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetExplosion(D3DXVECTOR3 pos, float SizeX, float SizeY, int Type)
{
	int IdxExplosion = -1;

	for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++)
	{
		if(!Explosion[CntExplosion].use)
		{
			Explosion[CntExplosion].pos = pos;
			Explosion[CntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Explosion[CntExplosion].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			Explosion[CntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			Explosion[CntExplosion].SizeX = SizeX;
			Explosion[CntExplosion].SizeY = SizeY;
			Explosion[CntExplosion].Counter = 0;
			Explosion[CntExplosion].Pattern = 0;
			Explosion[CntExplosion].Type = Type;
			Explosion[CntExplosion].use = true;

			// 頂点座標の設定
			SetVertexExplosion(CntExplosion, SizeX, SizeY);

			if(Explosion[CntExplosion].Type == EXPLOSIONTYPE_BULLET_Player)
			{
				Explosion[CntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
			}
			else if(Explosion[CntExplosion].Type == EXPLOSIONTYPE_BULLET_ENEMY)
			{
				Explosion[CntExplosion].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f); 
			}
			SetColorExplosion(CntExplosion, Explosion[CntExplosion].col);

			// テクスチャ座標の設定
			SetTextureExplosion(CntExplosion, 0);

			IdxExplosion = CntExplosion;

			break;
		}
	}

	return IdxExplosion;
}
