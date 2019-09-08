//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 西村任弘
//
//=============================================================================
#include "enemy.h"
#include "shadow.h"
#include "effect.h"
#include "boss.h"
#include "timer.h"
#include "player.h"
#include "ace.h"
#include "fade.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureEnemy[ENEMYTYPE_MAX];	// テクスチャ読み込み場所
LPD3DXMESH			MeshEnemy[ENEMYTYPE_MAX];		// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXMatBuffEnemy[ENEMYTYPE_MAX];// メッシュのマテリアル情報を格納
DWORD				NumMatEnemy[ENEMYTYPE_MAX];		// 属性情報の総数

static D3DXMATRIX	MtxWorldEnemy;					// ワールドマトリックス

Enemy				enemy[MAX_ENEMY];				

const char *FileNameEnemy[ENEMYTYPE_MAX] =
{
	"data/MODEL/UFO(big).x",		// UFO
	"data/MODEL/ミサイル.x",		// ミサイル
	"data/MODEL/UFO(big).x",
};

//*****************************************************************************
// エネミーの初期化用（メンバ関数）
//*****************************************************************************
void Enemy::EnemyInit()
{
	use = true;								// useの判定 
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転
	scl = D3DXVECTOR3(2.0f, 2.0f, 2.0f);	// サイズ
	radius = 2.0f;							// 半径
	IdxShadow = -1;							// 影ID
	Type = ENEMYTYPE_ENEMY;					// エネミーのモデル

	// エネミーの初期配置
	float PosX, PosY, PosZ;

	PosX = (float)(rand() % 500 - 200);
	PosY = (float)(rand() % 600);
	PosZ = (float)(rand() % 300000) + 7300;
	pos = D3DXVECTOR3(PosX, PosY, PosZ);	// 位置
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	for (int CntEnemyType = 0; CntEnemyType < ENEMYTYPE_MAX; CntEnemyType++)
	{
		// テクスチャの初期化
		D3DTextureEnemy[CntEnemyType] = NULL;

		// メッシュの初期化
		MeshEnemy[CntEnemyType] = NULL;

		// マテリアルの初期化
		D3DXMatBuffEnemy[CntEnemyType] = NULL;


		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(FileNameEnemy[CntEnemyType],
			D3DXMESH_SYSTEMMEM,
			Device,
			NULL,
			&D3DXMatBuffEnemy[CntEnemyType],
			NULL,
			&NumMatEnemy[CntEnemyType],
			&MeshEnemy[CntEnemyType])))
		{
			return E_FAIL;
		}

#if 1
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(Device,		// デバイスへのポインタ
			ENEMY_TEXTURE_FILENAME,				// ファイルの名前
			&D3DTextureEnemy[CntEnemyType]);	// 読み込むメモリー
#endif
	}

	//エネミーの設定
	for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++)
	{
		// エネミーの初期化処理
		enemy[CntEnemy].EnemyInit();
	}
	
	return S_OK;
}

//=============================================================================
// エネミーの終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int CntEnemyType = 0; CntEnemyType < ENEMYTYPE_MAX;CntEnemyType++)
	{
		if (D3DTextureEnemy[CntEnemyType] != NULL)
		{// テクスチャの開放
			D3DTextureEnemy[CntEnemyType]->Release();
			D3DTextureEnemy[CntEnemyType] = NULL;
		}

		if (MeshEnemy[CntEnemyType] != NULL)
		{// メッシュの開放
			MeshEnemy[CntEnemyType]->Release();
			MeshEnemy[CntEnemyType] = NULL;
		}

		if (D3DXMatBuffEnemy[CntEnemyType] != NULL)
		{// マテリアルの開放
			D3DXMatBuffEnemy[CntEnemyType]->Release();
			D3DXMatBuffEnemy[CntEnemyType] = NULL;
		}
	}
}

//=============================================================================
// エネミーの更新処理
//=============================================================================
void UpdateEnemy(void)
{
	// ace出現までのカウント
	int acecount=20;

	// スピードの設定
	float Speed = 2.0f;

	D3DXVECTOR3 Sabun;

	// プレイヤーの取得
	Player *player;
	player = GetPlayer();

	// エースの取得
	Ace*ace;
	ace = GetAce();
	
	for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++)
	{
		// 4で割り切れる配列の時ホーミング
		if (CntEnemy % 4 == 0)
		{
			if (ace->use == false)
			{
				enemy[CntEnemy].rot.y = 3.17f;
				enemy[CntEnemy].Type = ENEMYTYPE_ENEMY2;
				enemy[CntEnemy].scl = D3DXVECTOR3(4.0f, 4.0f, 2.0f);
				//ホーミング用の処理
				Sabun = (player->pos - enemy[CntEnemy].pos);
				D3DXVec3Normalize(&Sabun, &Sabun);

				// 内積
				FLOAT Kakudo = D3DXVec3Dot(&Sabun, &enemy[CntEnemy].pos);
				Kakudo = acosf(Kakudo);

			
				enemy[CntEnemy].pos.z -= 8.0f;
				
				Speed++;
				enemy[CntEnemy].pos += (Sabun)*(Speed);
			}

			else
			{
				enemy[CntEnemy].use = false;
			}
		}

		// 3で割り切れるなら
		else if (CntEnemy % 3 == 0)
		{
			enemy[CntEnemy].rot.y += 2.0f;
			enemy[CntEnemy].pos.z -= 5.0f;
		}

		// すべての条件に当てはまらないのなら
		else
		{
			enemy[CntEnemy].rot.y += 2.0f;
			enemy[CntEnemy].pos.z -= 20.0f;
		}

		if (enemy[CntEnemy].pos.z < -1 || ace->hp <= 0)
		{
			enemy[CntEnemy].use=false;

			acecount--;
			// カウントが0になったら
			if (acecount == 0)
			{
				// ACEを出現させる
				ace->use = true;
			}
		}
	}
	
}
//=============================================================================
// エネミーの描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxScl, MtxRot, MtxTranslate;

	// エースの取得
	Ace*ace;
	ace = GetAce();

	for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++)
	{
		if (enemy[CntEnemy].use == true && (GetPositionPlayer().z - enemy[CntEnemy].pos.z > -4000))
		{//エネミーがtrueなら

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&MtxWorldEnemy);

			// スケールを反映
			D3DXMatrixScaling(&MtxScl, enemy[CntEnemy].scl.x, enemy[CntEnemy].scl.y, enemy[CntEnemy].scl.z);
			D3DXMatrixMultiply(&MtxWorldEnemy, &MtxWorldEnemy, &MtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&MtxRot, enemy[CntEnemy].rot.y, enemy[CntEnemy].rot.x, enemy[CntEnemy].rot.z);
			D3DXMatrixMultiply(&MtxWorldEnemy, &MtxWorldEnemy, &MtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&MtxTranslate, enemy[CntEnemy].pos.x, enemy[CntEnemy].pos.y, enemy[CntEnemy].pos.z);
			D3DXMatrixMultiply(&MtxWorldEnemy, &MtxWorldEnemy, &MtxTranslate);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &MtxWorldEnemy);

			D3DXMATERIAL *D3DXMat = (D3DXMATERIAL*)D3DXMatBuffEnemy[enemy[CntEnemy].Type]->GetBufferPointer();

			for (int CntMat = 0; CntMat < (int)NumMatEnemy[enemy[CntEnemy].Type]; CntMat++)
			{
				// マテリアルの設定
				Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

				// テクスチャの設定
				Device->SetTexture(0, D3DTextureEnemy[enemy[CntEnemy].Type]);

				// 描画
				MeshEnemy[enemy[CntEnemy].Type]->DrawSubset(CntMat);
			}
		}

	}

	{// マテリアルをデフォルトに戻す
		D3DXMATERIAL Mat;

		Mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
		Mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		Mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		Device->SetMaterial(&Mat.MatD3D);
	}

}

//=============================================================================
// エネミーの設定
//=============================================================================
void SetEnemy(D3DXVECTOR3 disp, D3DXVECTOR3 Rot, int Type)
{
	for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++)
	{
		if (!enemy[CntEnemy].use)
		{
			enemy[CntEnemy].start = disp;
			enemy[CntEnemy].c1 = D3DXVECTOR3((float)(rand() % 400 - 350 / 1.0f), (float)(rand() % 750 - 600) / 1.0f, (float)(rand() % 20250) / 1.0f) + disp;
			enemy[CntEnemy].c2 = D3DXVECTOR3((float)(rand() % 200 - 150 / 1.0f), (float)(rand() % 450 - 100) / 1.0f, (float)(rand() % 10250) / 1.0f) + disp;
			enemy[CntEnemy].rot = Rot;
			enemy[CntEnemy].radius = ENEMY_RADIUS;
			enemy[CntEnemy].Type = Type;
			enemy[CntEnemy].use = true;
	
			break;
		}
	}
}

//=============================================================================
// エネミーの削除
//=============================================================================
void DeleteEnemy(int IdxEnemy)
{
	if (IdxEnemy >= 0 && IdxEnemy < MAX_ENEMY)
	{
		enemy[IdxEnemy].use = false;
	}
}

//=============================================================================
// エネミーの取得
//=============================================================================
Enemy *GetEnemy(int IdxEnemy)
{
	return &enemy[IdxEnemy];
}

//=============================================================================
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionEnemy(int IdxEnemy)
{
	return enemy[IdxEnemy].pos;
}

//=============================================================================
// 大きさ取得
//=============================================================================
D3DXVECTOR3 GetSclEnemy(int IdxEnemy)
{
	return enemy[IdxEnemy].scl;
}