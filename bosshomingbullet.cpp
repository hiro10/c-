//=============================================================================
//
// ボスのホーミング弾処理 [bosshomingbullet .cpp]
// Author :  西村　任弘
//
//=============================================================================
#include "bosshomingbullet.h"
#include "input.h"
#include "camera.h"
#include "boss.h"
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
HRESULT MakeVertexBossHomingBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexBossHomingBullet(int IdxBossHomingBullet, float SizeX, float SizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBossHomingBullet = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBossHomingBullet = NULL;	// 頂点バッファインターフェースへのポインタ
D3DXMATRIX				MtxWorldBossHomingBullet;				// ワールドマトリックス
Homingbullet		BossHomingBullet[MAX_BOSSHOMINGBULLET];	// 弾ワーク

//=============================================================================
// ボスホーミングのパラメータの初期値
//=============================================================================
void Homingbullet::BossHomingbulletInit()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	SizeX = BOSSHOMINGBULLET_SIZE_X;
	SizeY = BOSSHOMINGBULLET_SIZE_Y;
	radius = BOSSHOMINGBULLET_RADIUS;
	Timer = 0;
	use = false;
}

//=============================================================================
// ボスホーミングの更新処理
//=============================================================================
void Homingbullet::BossHoming()
{
	Frame++;

	float t = (float)Frame / 60.0f;
	//始点
	D3DXVECTOR3 p0 = (1 - t)*(1 - t)*(1 - t)*start;
	//通過点１
	D3DXVECTOR3 p1 = 3 * (1 - t)*(1 - t)*t*bc1;
	//通過点２
	D3DXVECTOR3 p2 = 3 * (1 - t)*t*t*bc2;
	// 終点
	D3DXVECTOR3 p3 = t * t*t*targetpos;

	pos = p0 + p1 + p2 + p3;

	if (pos.z <= -100)
	{
		// 未使用
		use = false;
	}
	else
	{
		// エフェクトの設定
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.05f, 0.05f, 0.05f, 1.50f), 46.0f, 46.0f, 30);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.05f, 0.05f, 0.45f, 1.20f), 6.0f, 6.0f, 50);
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBossHomingBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexBossHomingBullet(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,						    // デバイスへのポインタ
								TEXTURE_BOSSHOMINGBULLET,		   // ファイルの名前
								&D3DTextureBossHomingBullet );	// 読み込むメモリー


	for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++)
	{
		BossHomingBullet[CntBossHomingBullet].BossHomingbulletInit();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBossHomingBullet(void)
{
	if (D3DTextureBossHomingBullet != NULL)
	{// テクスチャの開放
		D3DTextureBossHomingBullet->Release();
		D3DTextureBossHomingBullet = NULL;
	}

	if (D3DVtxBuffBossHomingBullet != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffBossHomingBullet->Release();
		D3DVtxBuffBossHomingBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBossHomingBullet(void)
{
	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	// プレイヤーを取得
	Player *player;
	player = GetPlayer();

	if (boss->use == true)
	{// ボスが使用していれば更新
		D3DXVECTOR3 rotCamera;

		// カメラの回転を取得
		rotCamera = GetRotCamera();

		for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++)
		{
			if (BossHomingBullet[CntBossHomingBullet].use)
			{
				BossHomingBullet[CntBossHomingBullet].BossHoming();

				// プレイヤーとの当たり判定
				if (AttackCheak(BossHomingBullet[CntBossHomingBullet].pos, player->pos,
					BossHomingBullet[CntBossHomingBullet].radius, player->radius))
				{
					// ボスのホーミング弾の消去
					DeleteBossHomingBullet(CntBossHomingBullet);

					// 爆発の発生
					SetExplosion(player->pos, 40.0f, 40.0f);

					// プレイヤーのHPの減少
					player->hp -= BOSSHOMINGBULLET_DAMAGE;

					break;
				}
			}
			// ボスが死んだら
			if (boss->use == false)
			{
				// ボスのホーミングも未使用に
				BossHomingBullet->use = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBossHomingBullet(void)
{
	Boss *boss;
	boss = GetBoss();

	if (boss->use == true)
	{// ボスが使用していれば更新
		LPDIRECT3DDEVICE9 Device = GetDevice();
		D3DXMATRIX MtxView, MtxScale, MtxTranslate;

		// ライティングを無効に
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// αテストを有効に
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHAREF, 0);
		Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++)
		{
			if (BossHomingBullet[CntBossHomingBullet].use)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&MtxWorldBossHomingBullet);

				// ビューマトリックスを取得
				MtxView = GetMtxView();

				MtxWorldBossHomingBullet._11 = MtxView._11;
				MtxWorldBossHomingBullet._12 = MtxView._21;
				MtxWorldBossHomingBullet._13 = MtxView._31;
				MtxWorldBossHomingBullet._21 = MtxView._12;
				MtxWorldBossHomingBullet._22 = MtxView._22;
				MtxWorldBossHomingBullet._23 = MtxView._32;
				MtxWorldBossHomingBullet._31 = MtxView._13;
				MtxWorldBossHomingBullet._32 = MtxView._23;
				MtxWorldBossHomingBullet._33 = MtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&MtxScale, BossHomingBullet[CntBossHomingBullet].scl.x, BossHomingBullet[CntBossHomingBullet].scl.y, BossHomingBullet[CntBossHomingBullet].scl.z);
				D3DXMatrixMultiply(&MtxWorldBossHomingBullet, &MtxWorldBossHomingBullet, &MtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&MtxTranslate, BossHomingBullet[CntBossHomingBullet].pos.x, BossHomingBullet[CntBossHomingBullet].pos.y, BossHomingBullet[CntBossHomingBullet].pos.z);
				D3DXMatrixMultiply(&MtxWorldBossHomingBullet, &MtxWorldBossHomingBullet, &MtxTranslate);

				// ワールドマトリックスの設定
				Device->SetTransform(D3DTS_WORLD, &MtxWorldBossHomingBullet);

				// 頂点バッファをデバイスのデータストリームにバインド
				Device->SetStreamSource(0, D3DVtxBuffBossHomingBullet, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				Device->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				Device->SetTexture(0, D3DTextureBossHomingBullet);

				// ポリゴンの描画
				Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntBossHomingBullet * 4), NUM_POLYGON);
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
HRESULT MakeVertexBossHomingBullet(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BOSSHOMINGBULLET,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffBossHomingBullet,						// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBossHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(-BOSSHOMINGBULLET_SIZE_X / 2, -BOSSHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(BOSSHOMINGBULLET_SIZE_X / 2, -BOSSHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-BOSSHOMINGBULLET_SIZE_X / 2, BOSSHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(BOSSHOMINGBULLET_SIZE_X / 2, BOSSHOMINGBULLET_SIZE_Y / 2, 0.0f);

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
		D3DVtxBuffBossHomingBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBossHomingBullet(int IdxBossHomingBullet, float SizeX, float SizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBossHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxBossHomingBullet * 4);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(-SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, SizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(SizeX / 2, SizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffBossHomingBullet->Unlock();
	}
}

//=============================================================================
// 弾の設定
//=============================================================================
int SetBossHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY)
{
	int IdxBossHomingBullet = -1;

	for (int CntBossHomingBullet = 0; CntBossHomingBullet < MAX_BOSSHOMINGBULLET; CntBossHomingBullet++)
	{
		if (!BossHomingBullet[CntBossHomingBullet].use)
		{
			BossHomingBullet[CntBossHomingBullet].start = pos;
			BossHomingBullet[CntBossHomingBullet].bc1 = D3DXVECTOR3((float)(rand() % 400), (float)(rand() % 650 - 500), (float)(rand() % 550)) + pos;
			BossHomingBullet[CntBossHomingBullet].bc2 = D3DXVECTOR3((float)(rand() % 400), (float)(rand() % 650 - 300), (float)(rand() % 90)) + pos;
			BossHomingBullet[CntBossHomingBullet].Frame = 0;
			BossHomingBullet[CntBossHomingBullet].SizeX = SizeX;
			BossHomingBullet[CntBossHomingBullet].SizeY = SizeY;
			BossHomingBullet[CntBossHomingBullet].use = true;
			BossHomingBullet[CntBossHomingBullet].Timer = 2000;
			BossHomingBullet[CntBossHomingBullet].targetpos = GetPositionPlayer();

			// 頂点座標の設定
			SetVertexBossHomingBullet(CntBossHomingBullet, SizeX, SizeY);

			IdxBossHomingBullet = CntBossHomingBullet;

			break;
		}
	}
	return IdxBossHomingBullet;
}

//=============================================================================
// 弾の削除
//=============================================================================
void DeleteBossHomingBullet(int IdxBossHomingBullet)
{
	if (IdxBossHomingBullet >= 0 && IdxBossHomingBullet < MAX_BOSSHOMINGBULLET)
	{
		BossHomingBullet[IdxBossHomingBullet].use = false;
	}
}