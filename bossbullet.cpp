//=============================================================================
//
// ボス弾処理 [bossbullet.cpp]
// Author :西村  任弘
//
//=============================================================================
#include "bossbullet.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "effect.h"
#include "explosion.h"
#include "player.h"
#include "score.h"
#include "boss.h"
#include "sound.h"
#include "collision.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBossBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexBossBullet(int nIdxBossBullet, float SizeX, float SizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBossBullet = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBossBullet = NULL;		// 頂点バッファインターフェースへのポインタ
D3DXMATRIX				WorldBossBullet;				    // ワールドマトリックス
Bullet					BossBullet[MAX_BOSSBULLET];			// 弾ワーク



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBossBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexBossBullet(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
								TEXTURE_BOSSBULLET,		// ファイルの名前
								&D3DTextureBossBullet);	// 読み込むメモリー

	
	for (int CntBossBullet = 0; CntBossBullet < MAX_BOSSBULLET; CntBossBullet++)
	{
		BossBullet[CntBossBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		BossBullet[CntBossBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		BossBullet[CntBossBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		BossBullet[CntBossBullet].move = D3DXVECTOR3(1.0f, 1.0f, 10.0f);
		BossBullet[CntBossBullet].SizeX = BOSSBULLET_SIZE_X;
		BossBullet[CntBossBullet].SizeY = BOSSBULLET_SIZE_Y;
		BossBullet[CntBossBullet].radius = BOSSBULLET_RADIUS;
		BossBullet[CntBossBullet].Timer = 0;
		BossBullet[CntBossBullet].use = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBossBullet(void)
{
	if (D3DTextureBossBullet != NULL)
	{// テクスチャの開放
		D3DTextureBossBullet->Release();
		D3DTextureBossBullet = NULL;
	}

	if (D3DVtxBuffBossBullet != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffBossBullet->Release();
		D3DVtxBuffBossBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBossBullet(void)
{
	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	// プレイヤーの取得
	Player *player;
	player = GetPlayer();

	if (boss->use == true)
	{// ボスが使用していれば更新
		
		for (int CntBossBullet = 0; CntBossBullet < MAX_BOSSBULLET; CntBossBullet++)
		{
			// ba
			if (BossBullet[CntBossBullet].use)
			{//バレットの位置に移動量を入れている
				BossBullet[CntBossBullet].pos.x += BossBullet[CntBossBullet].move.x;
				BossBullet[CntBossBullet].pos.y += BossBullet[CntBossBullet].move.y;
				BossBullet[CntBossBullet].pos.z += BossBullet[CntBossBullet].move.z;

				// ボスのTimerをデクリメント
				BossBullet[CntBossBullet].Timer--;
				
				// ボスのTimerが0以下なら
				if (BossBullet[CntBossBullet].Timer <= 0)
				{
					// ボスバレットを未使用に
					BossBullet[CntBossBullet].use = false;
				}

				// プレイヤーとの当たり判定
				if (AttackCheak(BossBullet[CntBossBullet].pos, player->pos,
					BossBullet[CntBossBullet].radius, player->radius))
				{
					// ボスのバレットを未使用に
					BossBullet[CntBossBullet].use = false;

					// SE再生
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					// 爆発の発生
					SetExplosion(player->pos, 140.0f, 140.0f);

					// Hpの減少
					player->hp -= BOSSBULLET_DAMADE;

					break;
				}
			}
		}
	}
	// ボスが未使用なら
	else if (boss->use == false)
	{
		// ボスバレットを未使用に
		BossBullet->use = false;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBossBullet(void)
{
	// ボスの取得
	Boss *boss;
	boss = GetBoss();
	
	if (boss->use == true)
	{// ボスが使用していれば描画
		LPDIRECT3DDEVICE9 Device = GetDevice();
		D3DXMATRIX View, Scale, mtxTranslate;

		// ライティングを無効に
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// αテストを有効に
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHAREF, 0);
		Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		for (int nCntBullet = 0; nCntBullet < MAX_BOSSBULLET; nCntBullet++)
		{
			if (BossBullet[nCntBullet].pos.z - GetPositionPlayer().z <= 2500)
			{
				if (BossBullet[nCntBullet].use)
				{
					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&WorldBossBullet);

					// ビューマトリックスを取得
					View = GetMtxView();

					WorldBossBullet._11 = View._11;
					WorldBossBullet._12 = View._21;
					WorldBossBullet._13 = View._31;
					WorldBossBullet._21 = View._12;
					WorldBossBullet._22 = View._22;
					WorldBossBullet._23 = View._32;
					WorldBossBullet._31 = View._13;
					WorldBossBullet._32 = View._23;
					WorldBossBullet._33 = View._33;

					// スケールを反映
					D3DXMatrixScaling(&Scale, BossBullet[nCntBullet].scl.x, BossBullet[nCntBullet].scl.y, BossBullet[nCntBullet].scl.z);
					D3DXMatrixMultiply(&WorldBossBullet, &WorldBossBullet, &Scale);

					// 移動を反映
					D3DXMatrixTranslation(&mtxTranslate, BossBullet[nCntBullet].pos.x, BossBullet[nCntBullet].pos.y, BossBullet[nCntBullet].pos.z);
					D3DXMatrixMultiply(&WorldBossBullet, &WorldBossBullet, &mtxTranslate);

					// ワールドマトリックスの設定
					Device->SetTransform(D3DTS_WORLD, &WorldBossBullet);

					// 頂点バッファをデバイスのデータストリームにバインド
					Device->SetStreamSource(0, D3DVtxBuffBossBullet, 0, sizeof(VERTEX_3D));

					// 頂点フォーマットの設定
					Device->SetFVF(FVF_VERTEX_3D);

					// テクスチャの設定
					Device->SetTexture(0, D3DTextureBossBullet);

					// ポリゴンの描画
					Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBullet * 4), NUM_POLYGON);
				}
			}
		}
		// ライティングを有効に
		Device->SetRenderState(D3DRS_LIGHTING, TRUE);

		// αテストを無効に
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBossBullet(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BOSSBULLET,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffBossBullet,						// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBossBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntBossBullet = 0; CntBossBullet < MAX_BOSSBULLET; CntBossBullet++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(-BOSSBULLET_SIZE_X / 2, -BOSSBULLET_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(BOSSBULLET_SIZE_X / 2, -BOSSBULLET_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-BOSSBULLET_SIZE_X / 2, BOSSBULLET_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(BOSSBULLET_SIZE_X / 2, BOSSBULLET_SIZE_Y / 2, 0.0f);

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
		D3DVtxBuffBossBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBossBullet(int IdxBossBullet, float SizeX, float SizeY)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *Vtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffBossBullet->Lock(0, 0, (void**)&Vtx, 0);

	Vtx += (IdxBossBullet * 4);

	// 頂点座標の設定
	Vtx[0].vtx = D3DXVECTOR3(-SizeX / 2, -SizeY / 2, 0.0f);
	Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, SizeY / 2, 0.0f);
	Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, -SizeY / 2, 0.0f);
	Vtx[3].vtx = D3DXVECTOR3(SizeX / 2, SizeY / 2, 0.0f);

	// 頂点データをアンロックする
	D3DVtxBuffBossBullet->Unlock();
}

//=============================================================================
// 弾の設定
//=============================================================================
int SetBossBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float SizeX, float SizeY, int Timer)
{
	int IdxBossBullet = -1;

	for (int CntBossBullet = 0; CntBossBullet < MAX_BOSSBULLET; CntBossBullet++)
	{
		if (!BossBullet[CntBossBullet].use)
		{
			BossBullet[CntBossBullet].pos = pos;
			BossBullet[CntBossBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			BossBullet[CntBossBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			BossBullet[CntBossBullet].move = move;
			BossBullet[CntBossBullet].SizeX = SizeX;
			BossBullet[CntBossBullet].SizeY = SizeY;
			BossBullet[CntBossBullet].Timer = Timer;
			BossBullet[CntBossBullet].use = true;

			// 頂点座標の設定
			SetVertexBossBullet(CntBossBullet, SizeX, SizeY);

			IdxBossBullet = CntBossBullet;

			break;
		}
	} 

	return IdxBossBullet;
}

//=============================================================================
// 弾の削除
//=============================================================================
void DeleteBossBullet(int IdxBossBullet)
{
	if (IdxBossBullet >= 0 && IdxBossBullet < MAX_BOSSBULLET)
	{
		BossBullet[IdxBossBullet].use = false;
	}
}
