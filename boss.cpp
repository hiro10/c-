
//=============================================================================
//
// ボス処理 [boss.cpp]
// Author : 西村任弘
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "bullet.h"
#include "effect.h"
#include "boss.h"
#include "camera.h"
#include "bossbullet.h"
#include "bosshomingbullet.h"
#include "input.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
#include "effekseer.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureBoss;		// テクスチャへのポインタ
LPD3DXMESH			D3DXMeshBoss;		// メッシュ情報へのポインタ
LPD3DXBUFFER		D3DXBuffMatBoss;	// マテリアル情報へのポインタ
DWORD				NumMatBoss;			// マテリアル情報の数
D3DXMATRIX			BossWorld;			// ワールドマトリックス
Boss				boss;				// ボスワーク

//*****************************************************************************
// ボス用バレットデータ(ShotCount=0)
//*****************************************************************************
static D3DXVECTOR3 BossBulletData0[BOSS_BULLET_QUANTITY_0]
{// ボスのバレット０の発射地点 D3DXVECTOR3(x,y,z)
	
	{BossBulletData0[0] = D3DXVECTOR3(0,1400,0)},
	{BossBulletData0[1] = D3DXVECTOR3(0,1100,-30)},
	{BossBulletData0[2] = D3DXVECTOR3(0,1000,30)},
	{BossBulletData0[3] = D3DXVECTOR3(0,930,30)},
	{BossBulletData0[4] = D3DXVECTOR3(0,740,30)},
	{BossBulletData0[5] = D3DXVECTOR3(0,640,30)},
	{BossBulletData0[6] = D3DXVECTOR3(300,640,100)},
	{BossBulletData0[7] = D3DXVECTOR3(-100,330,0)},
	{BossBulletData0[8] = D3DXVECTOR3(100,240,0)},
	{BossBulletData0[9] = D3DXVECTOR3(0,-100,0)},
	{BossBulletData0[10] = D3DXVECTOR3(100,-200,0)},
	{BossBulletData0[11] = D3DXVECTOR3(100,-500,0)},
};
//*****************************************************************************
// ボス用バレットデータ(ShotCount=1)
//*****************************************************************************
static D3DXVECTOR3 BossBulletData1[BOSS_BULLET_QUANTITY_1]
{// ボスのバレット１の発射地点 D3DXVECTOR3(x,y,z)

	{BossBulletData1[0] = D3DXVECTOR3(0,1300,0)},
	{BossBulletData1[1] = D3DXVECTOR3(0,1000,0)},
	{BossBulletData1[2] = D3DXVECTOR3(0,800,30)},
	{BossBulletData1[3] = D3DXVECTOR3(0,700,30)},
	{BossBulletData1[4] = D3DXVECTOR3(0,600,300)},
	{BossBulletData1[5] = D3DXVECTOR3(0,500,300)},
	{BossBulletData1[6] = D3DXVECTOR3(300,400,100)},
	{BossBulletData1[7] = D3DXVECTOR3(200,130,10)},
	{BossBulletData1[8] = D3DXVECTOR3(-200,40,10)},
	{BossBulletData1[9] = D3DXVECTOR3(0,-100,10)},
	{BossBulletData1[10] = D3DXVECTOR3(100,-400,0)},
	{BossBulletData1[11] = D3DXVECTOR3(100,-600,10)},
};

//*****************************************************************************
// ボス用ホーミングバレットデータ(
//*****************************************************************************
static D3DXVECTOR3 BossHomingBulletData[BOSS_HOMING_QUANTITY]
{// ボスのホーミングバレットの発射地点 D3DXVECTOR3(x,y,z)

	{BossHomingBulletData[0] = D3DXVECTOR3(0, 550,0)},
	{BossHomingBulletData[1] = D3DXVECTOR3(450,0,0)},
	{BossHomingBulletData[2] = D3DXVECTOR3(-450,0,0)},
};

//*****************************************************************************
// ボスクラスに初期値を代入
//*****************************************************************************
void Boss::BossInitStatus()
{
	//ボスは未使用
	use = false;

	// 位置・回転・スケールの初期設定
	pos = D3DXVECTOR3(0.0f, 3240.0f, 2600.0f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	scl = D3DXVECTOR3(40.0f, 40.0f, 40.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//半径の設定
	radius = BOSS_RADIUS;

	//HP(ボス)の初期値
	hp = BOSS_HP_MAX;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBoss(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ボスのパラメータに値を設定
	boss.BossInitStatus();

	// テクスチャの初期化
	D3DTextureBoss = NULL;

	// メッシュの初期化
	D3DXMeshBoss = NULL;

	// マテリアルの初期化
	D3DXBuffMatBoss = NULL;
	NumMatBoss = 0;

	Device->SetRenderState(D3DRS_BLENDOPALPHA, TRUE);

	//Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,								// メッシュの作成オプションを指定
		Device,											// IDirect3DDevice9インターフェイスへのポインタ
		NULL,											// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatBoss,								// マテリアルデータを含むバッファへのポインタ
		NULL,											// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatBoss,									// D3DXMATERIAL構造体の数
		&D3DXMeshBoss)))								// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

#if 1
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
		BOSS_TEXTURE_FILENAME,							// ファイルの名前
		&D3DTextureBoss);								// 読み込むメモリー
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{
	if (boss.use == true)
	{// もしボスが使用中なら未使用に
		boss.use = false;
	}

	if (D3DXMeshBoss != NULL)
	{// メッシュの開放
		D3DXMeshBoss->Release();
		D3DXMeshBoss = NULL;
	}

	if (D3DXBuffMatBoss != NULL)
	{// マテリアルの開放
		D3DXBuffMatBoss->Release();
		D3DXBuffMatBoss = NULL;
	}

	if (D3DTextureBoss != NULL)
	{//テクスチャの開放
		D3DTextureBoss->Release();
		D3DTextureBoss = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{
	
	

	// バレットの発射間隔
	int ShotCount = rand() % 6;						

	// ボスをtrueに
	boss.use = true;								

	// ボスの回転
	boss.rot.y -= BOSS_ROT_Y;

	boss.move.x = -sinf(boss.rot.y) * VALUE_MOVE_BOSS_BULLET;
	boss.move.y = 0;
	boss.move.z = -cosf(boss.rot.y) * VALUE_MOVE_BOSS_BULLET;

	// ボスが使用していれば
	if (boss.use == true)
	{
		if (boss.pos.y >= 1200)
		{
			// ボス戦導入時の下降演出
			boss.pos.y -= 20.0f;
		}

		// y軸が一定値になったら
		if (boss.pos.y <= 1200)
		{

			// ボスバレットのセット
			if (ShotCount == 0)
			{//ShotCountが０のときのボスのバレット

				for (int BulletDataCount0 = 0; BulletDataCount0 < BOSS_BULLET_QUANTITY_0; BulletDataCount0++)
				{// BOSS_BULLET_QUANTITY_0回繰り返す
					SetBossBullet(D3DXVECTOR3(boss.pos.x+BossBulletData0[BulletDataCount0].x,
						boss.pos.y + BossBulletData0[BulletDataCount0].y,
						boss.pos.z + BossBulletData0[BulletDataCount0].z),
						boss.move, BOSS_BULLET_SIZE_X, BOSS_BULLET_SIZE_Y, 100 * 3);
				}
			}
			else if (ShotCount == 1)
			{//ShotCountが１のときのボスのバレット
				for (int BulletDataCount1 = 0; BulletDataCount1 < BOSS_BULLET_QUANTITY_1; BulletDataCount1++)
				{// BOSS_BULLET_QUANTITY_1回繰り返す
					SetBossBullet(D3DXVECTOR3(boss.pos.x + BossBulletData1[BulletDataCount1].x,
						boss.pos.y + BossBulletData1[BulletDataCount1].y,
						boss.pos.z + BossBulletData1[BulletDataCount1].z),
						boss.move, BOSS_BULLET_SIZE_X, BOSS_BULLET_SIZE_Y, 100 * 3);
				}
			}
		}

		// HPが半分より下になったら
		if (boss.hp <= BOSS_HP_MAX / 2)
		{
			for (int BulletHomingDataCount = 0; BulletHomingDataCount < 3; BulletHomingDataCount++)
			{
				// ボス用のホーミング弾をセット
				SetBossHomingBullet(D3DXVECTOR3(boss.pos.x + BossHomingBulletData[BulletHomingDataCount].x,
					boss.pos.y + BossHomingBulletData[BulletHomingDataCount].y,
					boss.pos.z + BossHomingBulletData[BulletHomingDataCount].z), 10.0f, 10.0f);
			}
		}
		
		// HPが0以下になったら
		if (boss.hp <= BOSS_DEAD)
		{
			
			// 爆発の発生
			SetExplosion(boss.pos, 2200, 2200, 0);
			
			// SEの再生
			PlaySound(SOUND_LABEL_SE_TRUE);
		
			// ボスを未使用に
			boss.use = false;	
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX Scl, Rot, Translate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 Def;

	//ボスがtrueなら描画する
	if (boss.use == true)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&BossWorld);

		// スケールを反映
		D3DXMatrixScaling(&Scl, boss.scl.x, boss.scl.y, boss.scl.z);
		D3DXMatrixMultiply(&BossWorld, &BossWorld, &Scl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&Rot, boss.rot.y, boss.rot.x, boss.rot.z);
		D3DXMatrixMultiply(&BossWorld, &BossWorld, &Rot);

		// 移動を反映
		D3DXMatrixTranslation(&Translate, boss.pos.x, boss.pos.y, boss.pos.z);
		D3DXMatrixMultiply(&BossWorld, &BossWorld, &Translate);


		
		// ワールドマトリックスの設定
		Device->SetTransform(D3DTS_WORLD, &BossWorld);

		// 現在のマテリアルを取得
		Device->GetMaterial(&Def);

		// マテリアル情報に対するポインタを取得
		D3DXMat = (D3DXMATERIAL*)D3DXBuffMatBoss->GetBufferPointer();

		for (int CntMat = 0; CntMat < (int)NumMatBoss; CntMat++)
		{
			// マテリアルの設定
			Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureBoss);

			// 描画
			D3DXMeshBoss->DrawSubset(CntMat);
		}
		// マテリアルをデフォルトに戻す
		Device->SetMaterial(&Def);
	}
}

//=============================================================================
// ボスを取得(GET)
//=============================================================================
Boss *GetBoss(void)
{
	return &boss;
}

//=============================================================================
// ボスの位置を取得(GET)
//=============================================================================
D3DXVECTOR3	GetBossPos(void)
{
	return boss.pos;
}
//=============================================================================
// ボスの位置を取得(GET)
//=============================================================================
D3DXVECTOR3	*GetPosBoss(void)
{
	return (&boss.pos);
}
//=============================================================================
// ボスの回転を取得(GET)
//=============================================================================
D3DXVECTOR3	GetRotBoss(void)
{
	return(boss.rot);
}

//=============================================================================
// ボスのワールドマトリクスを取得(GET)
//=============================================================================
D3DXMATRIX *GetBossWorldMatrix(void)
{
	return (&BossWorld);
}