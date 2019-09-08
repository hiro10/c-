//=============================================================================
//
// ボスのホーミング弾処理 [acehomingbullet .cpp]
// Author :  西村　任弘
//
//=============================================================================
#include "acehomingbullet.h"
#include "input.h"
#include "camera.h"
#include "ace.h"
#include "shadow.h"
#include "effect.h"
#include "explosion.h"
#include "player.h"
#include "score.h"
#include "ace.h"
#include "sound.h"
#include "collision.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexAceHomingBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexAceHomingBullet(int IdxAceHomingBullet, float SizeX, float SizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureAceHomingBullet = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffAceHomingBullet = NULL;	// 頂点バッファインターフェースへのポインタ
D3DXMATRIX				MtxWorldAceHomingBullet;				// ワールドマトリックス
Homingbullet			AceHomingBullet[MAX_ACEHOMINGBULLET];	// 弾ワーク

//=============================================================================
// エースのホーミング弾のパラメータ初期値
//=============================================================================
void Homingbullet::AceHomingbulletInit()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	SizeX = ACEHOMINGBULLET_SIZE_X;
	SizeY = ACEHOMINGBULLET_SIZE_Y;
	radius = ACEHOMINGBULLET_RADIUS;
	Timer = 0;
	use = false;
}

//=============================================================================
// 敵すべてのホーミングの処理
//=============================================================================
void Homingbullet::AceHoming()
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
HRESULT InitAceHomingBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexAceHomingBullet(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,	    // デバイスへのポインタ
		TEXTURE_ACEHOMINGBULLET,			// ファイルの名前
		&D3DTextureAceHomingBullet);		// 読み込むメモリー


	for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++)
	{
		AceHomingBullet[CntAceHomingBullet].AceHomingbulletInit();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAceHomingBullet(void)
{
	if (D3DTextureAceHomingBullet != NULL)
	{// テクスチャの開放
		D3DTextureAceHomingBullet->Release();
		D3DTextureAceHomingBullet = NULL;
	}

	if (D3DVtxBuffAceHomingBullet != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffAceHomingBullet->Release();
		D3DVtxBuffAceHomingBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAceHomingBullet(void)
{
	Ace *ace;
	ace = GetAce();

	// プレイヤーを取得
	Player *Player;
	Player = GetPlayer();

	if (ace->use == true)
	{// ボスが使用していれば更新
		
		// カメラの回転を取得
		D3DXVECTOR3 rotCamera;
		rotCamera = GetRotCamera();

		// ホーミングバレットをMAX_ACEHOMINGBULLET分繰り返す
		for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++)
		{
			// ホーミングバレットが使用していれば
			if (AceHomingBullet[CntAceHomingBullet].use)
			{
				AceHomingBullet[CntAceHomingBullet].AceHoming();

				// プレイヤーとの当たり判定
				if (AttackCheak(AceHomingBullet[CntAceHomingBullet].pos, Player->pos,
					AceHomingBullet[CntAceHomingBullet].radius, Player->radius))
				{
					// ボスのホーミング弾の消去
					DeleteAceHomingBullet(CntAceHomingBullet);

					// 爆発の発生
					SetExplosion(Player->pos, 40.0f, 40.0f);

					// SE再生
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					// プレイヤーのHPの減少
					Player->hp -= ACEHOMINGBULLET_DAMAGE;

					break;
				}
			}
			if (ace->use == false)
			{
				AceHomingBullet->use = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAceHomingBullet(void)
{
	// ACEの取得
	Ace *ace;
	ace = GetAce();

	if (ace->use == true)
	{// ボスが使用していれば更新
		LPDIRECT3DDEVICE9 Device = GetDevice();
		D3DXMATRIX MtxView, MtxScale, MtxTranslate;

		// ライティングを無効に
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// αテストを有効に
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHAREF, 0);
		Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++)
		{
			if (AceHomingBullet[CntAceHomingBullet].use)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&MtxWorldAceHomingBullet);

				//// ビューマトリックスを取得
				MtxView = GetMtxView();

				MtxWorldAceHomingBullet._11 = MtxView._11;
				MtxWorldAceHomingBullet._12 = MtxView._21;
				MtxWorldAceHomingBullet._13 = MtxView._31;
				MtxWorldAceHomingBullet._21 = MtxView._12;
				MtxWorldAceHomingBullet._22 = MtxView._22;
				MtxWorldAceHomingBullet._23 = MtxView._32;
				MtxWorldAceHomingBullet._31 = MtxView._13;
				MtxWorldAceHomingBullet._32 = MtxView._23;
				MtxWorldAceHomingBullet._33 = MtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&MtxScale, AceHomingBullet[CntAceHomingBullet].scl.x, AceHomingBullet[CntAceHomingBullet].scl.y, AceHomingBullet[CntAceHomingBullet].scl.z);
				D3DXMatrixMultiply(&MtxWorldAceHomingBullet, &MtxWorldAceHomingBullet, &MtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&MtxTranslate, AceHomingBullet[CntAceHomingBullet].pos.x, AceHomingBullet[CntAceHomingBullet].pos.y, AceHomingBullet[CntAceHomingBullet].pos.z);
				D3DXMatrixMultiply(&MtxWorldAceHomingBullet, &MtxWorldAceHomingBullet, &MtxTranslate);

				// ワールドマトリックスの設定
				Device->SetTransform(D3DTS_WORLD, &MtxWorldAceHomingBullet);

				// 頂点バッファをデバイスのデータストリームにバインド
				Device->SetStreamSource(0, D3DVtxBuffAceHomingBullet, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				Device->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				Device->SetTexture(0, D3DTextureAceHomingBullet);

				// ポリゴンの描画
				Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAceHomingBullet * 4), NUM_POLYGON);
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
HRESULT MakeVertexAceHomingBullet(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_ACEHOMINGBULLET,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffAceHomingBullet,						// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffAceHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(-ACEHOMINGBULLET_SIZE_X / 2, -ACEHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(ACEHOMINGBULLET_SIZE_X / 2, -ACEHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-ACEHOMINGBULLET_SIZE_X / 2, ACEHOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(ACEHOMINGBULLET_SIZE_X / 2, ACEHOMINGBULLET_SIZE_Y / 2, 0.0f);

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
		D3DVtxBuffAceHomingBullet->Unlock();
	}

	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexAceHomingBullet(int IdxAceHomingBullet, float SizeX, float SizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffAceHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxAceHomingBullet * 4);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(-SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, SizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(SizeX / 2, SizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffAceHomingBullet->Unlock();
	}
}

//=============================================================================
// 弾の設定
//=============================================================================
int SetAceHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY)
{
	int IdxAceHomingBullet = -1;

	for (int CntAceHomingBullet = 0; CntAceHomingBullet < MAX_ACEHOMINGBULLET; CntAceHomingBullet++)
	{
		if (!AceHomingBullet[CntAceHomingBullet].use)
		{
			AceHomingBullet[CntAceHomingBullet].start = pos;
			AceHomingBullet[CntAceHomingBullet].bc1 = D3DXVECTOR3((float)(rand() % 400), (float)(rand() % 650 - 500), (float)(rand() % 550)) + pos;
			AceHomingBullet[CntAceHomingBullet].bc2 = D3DXVECTOR3((float)(rand() % 400), (float)(rand() % 650 - 300), (float)(rand() % 90)) + pos;
			AceHomingBullet[CntAceHomingBullet].Frame = 0;
			AceHomingBullet[CntAceHomingBullet].SizeX = SizeX;
			AceHomingBullet[CntAceHomingBullet].SizeY = SizeY;
			AceHomingBullet[CntAceHomingBullet].use = true;
			AceHomingBullet[CntAceHomingBullet].Timer = 2000;
			AceHomingBullet[CntAceHomingBullet].targetpos = GetPositionPlayer();

			// 頂点座標の設定
			SetVertexAceHomingBullet(CntAceHomingBullet, SizeX, SizeY);

			IdxAceHomingBullet = CntAceHomingBullet;

			break;
		}
	}
	return IdxAceHomingBullet;
}

//=============================================================================
// 弾の削除
//=============================================================================
void DeleteAceHomingBullet(int IdxAceHomingBullet)
{
	if (IdxAceHomingBullet >= 0 && IdxAceHomingBullet < MAX_ACEHOMINGBULLET)
	{
		AceHomingBullet[IdxAceHomingBullet].use = false;
	}
}
