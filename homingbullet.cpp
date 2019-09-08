//=============================================================================
//
// 弾処理 [homingbullet .cpp]
// Author :西村 任弘
//
//=============================================================================
#include "homingbullet.h"
#include "input.h"
#include "camera.h"
#include "boss.h"
#include "shadow.h"
#include "effect.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "fade.h"
#include "Ace.h"
#include "score.h"
#include "boss.h"
#include "sound.h"
#include "collision.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexHomingBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexHomingBullet(int IdxHomingBullet, float SizeX, float SizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureHomingBullet = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffHomingBullet = NULL;		// 頂点バッファインターフェースへのポインタ
D3DXMATRIX				WorldHomingBullet;					// ワールドマトリックス
Homingbullet			HomingBullet[MAX_HOMINGBULLET];		// 弾ワーク

//=============================================================================
// プレイヤーのホーミング弾の処理
//=============================================================================
void Homingbullet::PlayerHoming(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	// タイマーをデクリメント
	Timer--;

	// フレームをインクリメント
	Frame++;

	float t = Frame / 60.0f;
	// 始点
	D3DXVECTOR3 p0 = (1 - t)*(1 - t)*(1 - t)*start;
	// 通過点１
	D3DXVECTOR3 p1 = 3 * (1 - t)*(1 - t)*t*bc1;
	// 通過点２
	D3DXVECTOR3 p2 = 3 * (1 - t)*t*t*bc2;

	// エネミー戦の時
	if (GetMode() == MODE_GAME)
	{
		// 終点(Mode=1の時)
		D3DXVECTOR3 p3 = t * t*t*pos1;//GetAcePos();
		pos = p0 + p1 + p2 + p3;
	}
	// ボス戦の時
	else if (GetMode() == MODE_BOSS)
	{
		// 終点(それ以外の時)
		D3DXVECTOR3 p3 = t * t*t*pos2;//GetBossPos();
		pos = p0 + p1 + p2 + p3;
	}
	if (pos == pos1 || pos == pos2)
	{// 未使用
		use = false;
	}
	else
	{	// エフェクトの設定
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 1.0f),
			D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f), 16.0f, 16.0f, 10);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 1.0f),
			D3DXCOLOR(0.5f, 0.5f, 1.0f, 0.30f), 22.0f, 22.0f, 150);
	}
}

//=============================================================================
// プレイヤーのホーミング弾のパラメータ初期値
//=============================================================================
void Homingbullet::PlayerHomingbulletInit()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 大きさ
	SizeX = HOMINGBULLET_SIZE_X;			// Xサイズ
	SizeY = HOMINGBULLET_SIZE_Y;			// Yサイズ
	radius = 42.0f;							// 半径
	Timer = 100;							// ライフ
	use = false;							// useの判定
	HomingGage = 0;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHomingBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexHomingBullet(Device);

	for (int CntHomingBullet = 0; CntHomingBullet < MAX_HOMINGBULLET ; CntHomingBullet++)
	{
		// 初期化用のメンバ関数を呼ぶ
		HomingBullet[CntHomingBullet].PlayerHomingbulletInit();
	}
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHomingBullet(void)
{
	if (D3DTextureHomingBullet != NULL)
	{// テクスチャの開放
		D3DTextureHomingBullet->Release();
		D3DTextureHomingBullet = NULL;
	}

	if (D3DVtxBuffHomingBullet != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffHomingBullet->Release();
		D3DVtxBuffHomingBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHomingBullet(void)
{
	// ボスを取得
	Boss *boss;
	boss = GetBoss();

	// エースを取得
	Ace *ace;
	ace = GetAce();

	for (int CntHomingBullet = 0; CntHomingBullet < MAX_HOMINGBULLET; CntHomingBullet++)
	{
		//ベジェ曲線
		if (HomingBullet[CntHomingBullet].use)
		{//バレットの位置に移動量を入れている

			// ホーミング処理(1stステージ、Bossステージ)
			HomingBullet[CntHomingBullet].PlayerHoming(GetAcePos(),GetBossPos());
			
			// ボス戦の時
			if (GetMode() ==MODE_BOSS)
			{
				// ボスとの当たり判定
				if(AttackCheak(HomingBullet[CntHomingBullet].pos, boss->pos,
					HomingBullet[CntHomingBullet].radius, boss->radius))
				{
					if (boss->use == true)
					{
						// プレイヤーのメンバ変数のmpを減らす
						GetPlayer()->mp -= 20;

						// SE再生
						PlaySound(SOUND_LABEL_SE_EXPLOSION2);

						// ホーミングの消去
						DeleteHomingBullet(CntHomingBullet);

						// エフェクトの発生
						SetExplosion(boss->pos, 240.0f, 240.0f);

						// Hpの減少
						boss->hp -= HOMING_DAMAGE;

						break;
					}
				}
			}
			else
			{
				// エースとの当たり判定
				if (AttackCheak(HomingBullet[CntHomingBullet].pos, ace->pos,
					HomingBullet[CntHomingBullet].radius, ace->radius))
				{
					// プレイヤーのメンバ変数のmpを減らす
					GetPlayer()->mp -= 20;

					// SE再生
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					// エースの場所で、爆発発生
					SetExplosion(ace->pos, 240.0f, 240.0f);

					// Hpの減少
					ace->hp -= HOMING_DAMAGE;

					break;
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHomingBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxView,MtxScale,MtxTranslate;

	// ライティングを無効に
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, 0);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for(int CntHomingBullet  = 0; CntHomingBullet  < MAX_HOMINGBULLET; CntHomingBullet ++)
	{
		// ホーミングバレットを使用中なら
		if(HomingBullet [CntHomingBullet].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&WorldHomingBullet);

			// ビューマトリックスを取得
			MtxView = GetMtxView();

			// スケールを反映
			D3DXMatrixScaling(&MtxScale, HomingBullet [CntHomingBullet ].scl.x, HomingBullet [CntHomingBullet ].scl.y, HomingBullet [CntHomingBullet ].scl.z);
			D3DXMatrixMultiply(&WorldHomingBullet , &WorldHomingBullet , &MtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&MtxTranslate, HomingBullet [CntHomingBullet ].pos.x, HomingBullet [CntHomingBullet ].pos.y, HomingBullet [CntHomingBullet ].pos.z);
			D3DXMatrixMultiply(&WorldHomingBullet , &WorldHomingBullet , &MtxTranslate);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &WorldHomingBullet );

			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffHomingBullet , 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_3D);
		}
	}

	// ライティングを有効に
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexHomingBullet(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_HOMINGBULLET,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffHomingBullet,					// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntHomingBullet = 0; CntHomingBullet < MAX_HOMINGBULLET; CntHomingBullet++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(-HOMINGBULLET_SIZE_X / 2, -HOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(HOMINGBULLET_SIZE_X / 2, -HOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-HOMINGBULLET_SIZE_X / 2, HOMINGBULLET_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(HOMINGBULLET_SIZE_X / 2, HOMINGBULLET_SIZE_Y / 2, 0.0f);

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
		D3DVtxBuffHomingBullet->Unlock();
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexHomingBullet(int IdxHomingBullet, float SizeX, float SizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffHomingBullet->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxHomingBullet * 4);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(-SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-SizeX / 2, SizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(SizeX / 2, -SizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(SizeX / 2, SizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffHomingBullet->Unlock();
	}
}

//=============================================================================
// 弾の設定
//=============================================================================
int SetHomingBullet(D3DXVECTOR3 pos, float SizeX, float SizeY)
{
	int IdxHomingBullet = -1;

	// 通過点をばらけさせるための変数
	int Angle;

	for (int CntHomingBullet = 0; CntHomingBullet < MAX_HOMINGBULLET; CntHomingBullet++)
	{
		//ホーミングバレットが使用中なら
		if (!HomingBullet[CntHomingBullet].use)
		{
			// SE再生
			PlaySound(SOUND_LABEL_SE_SHOT);

			// 通過点をランダムにするためのランダム
			Angle =rand() % 2;
			HomingBullet[CntHomingBullet].start = pos;

			// 通過点の設定
			if (Angle == 1)
			{
				HomingBullet[CntHomingBullet].bc1 = D3DXVECTOR3((float)(rand() % 400 - 350 / 1.0f), (float)(rand() % 650 ) / 1.0f, (float)(rand() % 250) / 1.0f) + pos;
				HomingBullet[CntHomingBullet].bc2 = D3DXVECTOR3((float)(rand() % 400 - 350 / 1.0f), (float)(rand() % 650 -500) / 1.0f, (float)(rand() % 50) / 1.0f) + pos;
			}
			if (Angle ==0)
			{
				HomingBullet[CntHomingBullet].bc1 = D3DXVECTOR3((float)(rand() % 350 / 1.0f), (float)(rand() %300) / 1.0f, (float)(rand() % 250) / 1.0f) + pos;
				HomingBullet[CntHomingBullet].bc2 = D3DXVECTOR3((float)(rand() % 350 / 1.0f), (float)(rand() % 100) / 1.0f, (float)(rand() % 50) / 1.0f) + pos;
			}

			HomingBullet[CntHomingBullet].Frame = 0;
			HomingBullet[CntHomingBullet].SizeX = SizeX;
			HomingBullet[CntHomingBullet].SizeY = SizeY;
			HomingBullet[CntHomingBullet].use = true;
			HomingBullet[CntHomingBullet].Timer = 2600;

			// 頂点座標の設定
			SetVertexHomingBullet(CntHomingBullet, SizeX, SizeY);

			IdxHomingBullet = CntHomingBullet;

			break;
		}
	}

	return IdxHomingBullet;
}

//=============================================================================
// 弾の削除
//=============================================================================
void DeleteHomingBullet(int IdxHomingBullet)
{
	if (IdxHomingBullet >= 0 && IdxHomingBullet < MAX_HOMINGBULLET)
	{
		HomingBullet[IdxHomingBullet].use = false;
	}
}

