//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "enemy.h"
#include "effect.h"
#include "boss.h"
#include "ace.h"
#include "explosion.h"
#include "player.h"
#include "sound.h"
#include "collision.h"
#include "score.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 Device);
void SetVertexBullet(int nIdxBullet, float fSizeX, float fSizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBullet = NULL;	// 頂点バッファインターフェースへのポインタ
D3DXMATRIX				MtxWorldBullet;				// ワールドマトリックス
Bullet					bullet[MAX_BULLET];			// 弾ワーク

//*****************************************************************************
// バレットクラスの変数初期化用のメンバー関数
//*****************************************************************************
void Bullet::BulletInit()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// スケール
	SizeX = BULLET_SIZE_X;					// サイズX
	SizeY = BULLET_SIZE_Y;					// サイズY
	Timer = 0;								// 寿命
	radius = BULLET_RADIUS;					// 半径
	use = false;							// 使用しているか
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexBullet(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
		TEXTURE_BULLET,			// ファイルの名前
		&D3DTextureBullet);	// 読み込むメモリー

	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		bullet[CntBullet].BulletInit();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	if (D3DTextureBullet != NULL)
	{// テクスチャの開放
		D3DTextureBullet->Release();
		D3DTextureBullet = NULL;
	}

	if (D3DVtxBuffBullet != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffBullet->Release();
		D3DVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	// プレイヤーを取得
	Player *player;
	player = GetPlayer();

	// カメラの取得
	Camera	*Cwk;
	Cwk = GetCamerawk();

	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	// エースの取得
	Ace*ace;
	ace = GetAce();

	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		// バレットが使用中なら
		if (bullet[CntBullet].use)
		{
			if (Cwk->LockOnTrigger == false)
			{
				// 通常時
				bullet[CntBullet].pos.x += bullet[CntBullet].move.x;
				bullet[CntBullet].pos.z += bullet[CntBullet].move.z;

			}
			else if (Cwk->LockOnTrigger == true && GetMode() == MODE_GAME)
			{
				// 追尾弾（エース用)のセット
				SetTuibi(ace->pos, TUIBI_BULLET_SPEED);
			}
			else if (Cwk->LockOnTrigger == true && GetMode() == MODE_BOSS)
			{
				// 追尾弾（ボス用)のセット
				SetTuibi(boss->pos, TUIBI_BULLET_SPEED);
			}

			// 寿命を減らす
			bullet[CntBullet].Timer--;

			// バレットの寿命が0以下の時、プレイヤーの勝利、敗北演出の時
			if (bullet[CntBullet].Timer <= 0 || player->PlayerMove== 4 || player->PlayerMove == 3)
			{
				// バレットを未使用に
				bullet[CntBullet].use = false;
			}
			// そうでないなら
			else
			{
				// エフェクトの設定
				SetEffect(bullet[CntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.65f, 1.0f, 0.85f, 1.0f), 16.0f, 16.0f, 40);
			}

			// MODE_GAMEの時
			if (GetMode() == MODE_GAME)
			{
				Enemy *enemy;

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, enemy++)
				{
					// エネミーを取得
					enemy = GetEnemy(nCntEnemy);

					// エネミーとバレットの当たり判定
					if (enemy->use == true && bullet->use == true)
					{
						if (AttackCheak(bullet[CntBullet].pos, enemy->pos,
							bullet[CntBullet].radius, enemy->radius) == true)
						{
							if (player->mp < 100)
							{
								player->mp += 10;
							}
							// SEの再生
							PlaySound(SOUND_LABEL_SE_EXPLOSION2);

							// 爆発の発生
							SetExplosion(enemy->pos, 170.0f, 170.0f);

							// エネミー消去
							DeleteEnemy(nCntEnemy);

							// バレットの消去
							DeleteBullet(CntBullet);

							// スコア加算
							ChangeScore(100);

							break;
						}
					}
				}

				// エースとバレットの当たり判定
				if (AttackCheak(bullet[CntBullet].pos, ace->pos,
					bullet[CntBullet].radius, ace->radius) == true)
				{
					if (player->mp < 100)
					{
						player->mp += 10;
					}
					// SEの再生
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					// 爆発の発生
					SetExplosion(ace->pos, 40.0f, 40.0f);

					// HPの減少
					ace->hp -= BULLET_DAMAGE;

					// バレットの消去
					DeleteBullet(CntBullet);

					// スコア加算
					ChangeScore(100);

					break;
				}
			}

			// ボス戦の時
			if (GetMode() == MODE_BOSS)
			{
				if (boss->use == true&&boss->hp>0)
				{
					if (AttackCheak(bullet[CntBullet].pos, boss->pos,
						bullet[CntBullet].radius, boss->radius) == true)
					{
						if (player->mp < 100)
						{
							player->mp += 10;
						}
						// バレットの消去
						DeleteBullet(CntBullet);

						// スコアの増加
						ChangeScore(10);

						// HPの減少
						boss->hp -= BULLET_DAMAGE;

						// 爆発の発生
						SetExplosion(bullet[CntBullet].pos, 240.0f, 240.0f);

						// SEの再生
						PlaySound(SOUND_LABEL_SE_EXPLOSION2);

						break;
					}
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	// ライティングを無効に
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, 0);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		if (bullet[CntBullet].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&MtxWorldBullet);

			// ビューマトリックスを取得
			mtxView = GetMtxView();

			MtxWorldBullet._11 = mtxView._11;
			MtxWorldBullet._12 = mtxView._21;
			MtxWorldBullet._13 = mtxView._31;
			MtxWorldBullet._21 = mtxView._12;
			MtxWorldBullet._22 = mtxView._22;
			MtxWorldBullet._23 = mtxView._32;
			MtxWorldBullet._31 = mtxView._13;
			MtxWorldBullet._32 = mtxView._23;
			MtxWorldBullet._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, bullet[CntBullet].scl.x, bullet[CntBullet].scl.y, bullet[CntBullet].scl.z);
			D3DXMatrixMultiply(&MtxWorldBullet, &MtxWorldBullet, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, bullet[CntBullet].pos.x, bullet[CntBullet].pos.y, bullet[CntBullet].pos.z);
			D3DXMatrixMultiply(&MtxWorldBullet, &MtxWorldBullet, &mtxTranslate);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &MtxWorldBullet);

			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureBullet);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntBullet * 4), NUM_POLYGON);
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
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLET,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffBullet,						// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBullet->Lock(0, 0, (void**)&Vtx, 0);

		for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++, Vtx += 4)
		{
			// 頂点座標の設定
			Vtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);

			// 法線の設定
			Vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 201.0f);
			Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 201.0f);
			Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 201.0f);
			Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 201.0f);

			// テクスチャ座標の設定
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		D3DVtxBuffBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBullet(int nIdxBullet, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBullet->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxBullet * 4);

		// 頂点座標の設定
		Vtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// 弾の設定
//=============================================================================
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer)
{
	int IdxBullet = -1;

	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		if (!bullet[CntBullet].use)
		{
			bullet[CntBullet].pos = pos;
			bullet[CntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			bullet[CntBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			bullet[CntBullet].move = move;
			bullet[CntBullet].SizeX = fSizeX;
			bullet[CntBullet].SizeY = fSizeY;
			bullet[CntBullet].Timer = nTimer;
			bullet[CntBullet].use = true;

			// 頂点座標の設定
			SetVertexBullet(CntBullet, fSizeX, fSizeY);

			IdxBullet = CntBullet;

			break;
		}
	}
	return IdxBullet;
}

//=============================================================================
// 弾の削除
//=============================================================================
void DeleteBullet(int nIdxBullet)
{
	if (nIdxBullet >= 0 && nIdxBullet < MAX_BULLET)
	{
		bullet[nIdxBullet].use = false;
	}
}

//=============================================================================
//追尾弾の設定(ロックオン時の通常バレット）
//=============================================================================
void SetTuibi(D3DXVECTOR3 pos1,float Speed)
{
	for (int CntBullet = 0; CntBullet < MAX_BULLET; CntBullet++)
	{
		//ホーミング用の処理
		D3DXVECTOR3 Sabun = (pos1 - bullet[CntBullet].pos);
		D3DXVec3Normalize(&Sabun, &Sabun);

		// 内積
		FLOAT Kakudo = D3DXVec3Dot(&Sabun, &bullet[CntBullet].pos);
		Kakudo = acosf(Kakudo);

		if (bullet[CntBullet].pos.z == pos1.z)
		{
			bullet[CntBullet].pos.z -= Speed;
		}

		bullet[CntBullet].pos += (Sabun)*(Speed);
	}
}
