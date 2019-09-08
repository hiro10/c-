//=============================================================================
//
// ACE処理 [ace.cpp]
// Author : 西村 任弘
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "bullet.h"
#include "effect.h"
#include "ace.h"
#include "camera.h"
#include "input.h"
#include "explosion.h"
#include "player.h"
#include "score.h"
#include "fade.h"
#include "enemy.h"
#include "acehomingbullet.h"
#include "sound.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureAce = NULL;;	// テクスチャへのポインタ
LPD3DXMESH			D3DXMeshAce = NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER		D3DXBuffMatAce = NULL;	// マテリアル情報へのポインタ
DWORD				NumMatAce;				// マテリアル情報の数
D3DXMATRIX			MtxWorld;				// ワールドマトリックス
Ace					ace;					// ワーク

//=============================================================================
// エースのメンバ変数に初期値を代入する
//=============================================================================
void Ace::AceInitStatus()
{
	// エースは未使用
	use = false;

	// 位置・回転・スケールの初期設定
	pos = D3DXVECTOR3(0.0f, 40.0f, -1000.0f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	scl = D3DXVECTOR3(3.0f, 3.0f, 3.0f);

	// エースの動き用の変数の初期化
	ace.AceMoveCount = 0;

	// 半径の設定
	radius = ACE_RADIUS;

	// HP(エース)の初期値
	hp = ACE_HP_MAX;

}

//=============================================================================
// エースの死亡時の動き
//=============================================================================
void Ace::AceDeadMove()
{
	// 墜落演出のためにy軸移動
	move.y -= 5.0f;

	// 爆発の発生
	SetExplosion(ace.pos, (float)(rand() % 200), (float)(rand() % 200), 0);

	// 墜落地点になったら
	if (pos.y < -700)
	{
		// エースを未使用
		use = false;

		if (use == false && hp <= 0)
		{
			// 爆発の発生
			SetExplosion(ace.pos, 1200, 1200, 0);

			// フェードアウト
			SetFade(FADE_OUT);

			// スコアを増やす
			ChangeScore(2000);
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAce(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// エースのメンバ変数に値を代入
	ace.AceInitStatus();

	// D3DXMATERIAL構造体の数
	NumMatAce = 0;

	// 胴体のXファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_ACE,			// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
		Device,										// IDirect3DDevice9インターフェイスへのポインタ
		NULL,										// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatAce,							// マテリアルデータを含むバッファへのポインタ
		NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatAce,									// D3DXMATERIAL構造体の数
		&D3DXMeshAce)))								// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

#if 1
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,				// デバイスへのポインタ
		ACE_TEXTURE_FILENAME,						// ファイルの名前
		&D3DTextureAce);							// 読み込むメモリー
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAce(void)
{
	if (D3DXMeshAce != NULL)
	{// メッシュの開放
		D3DXMeshAce->Release();
		D3DXMeshAce = NULL;
	}

	if (D3DXBuffMatAce != NULL)
	{// マテリアルの開放
		D3DXBuffMatAce->Release();
		D3DXBuffMatAce = NULL;
	}

	if (D3DTextureAce != NULL)
	{// テクスチャの開放
		D3DTextureAce->Release();
		D3DTextureAce = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAce(void)
{
	// プレイヤーの取得
	Player *player;
	player = GetPlayer();

	// カメラの取得
	Camera		*Cwk = GetCamerawk();

	// バレットの発射間隔
	int ShotCount = 3;

	ace.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// エースが使用なら
	if (ace.use == true)
	{
		if (ace.hp > ACE_DEAD)
		{
			// ボスの動き（ランダムでボスを動かす)
			if (ace.AceMoveCount == 0)
			{
				//最初の動き
				ace.move.z += 20;
				ace.pos.z += ace.move.z;
				if (ace.pos.z >= ACE_FIRST_POS_Z)
				{
					ace.pos.z = ACE_FIRST_POS_Z;
					ace.AceMoveCount = 1;
				}
			}
			// 動きの変数が１なら
			else if (ace.AceMoveCount == 1)
			{
				ace.move.y -= ACE_VALUE_MOVE;
				ace.move.z += ACE_VALUE_MOVE;

				if (ShotCount == 2)
				{
					ace.pos.x += ace.move.x;
					ace.pos.y += ace.move.y;
					ace.pos.z += ace.move.z;
				}
				if (ace.AceMoveCount == 1 && ace.pos.y <= 50)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}

			// 動きの変数が２なら
			else if (ace.AceMoveCount == 2)
			{
				ace.move.x -= ACE_VALUE_MOVE;
				ace.move.y += ACE_VALUE_MOVE * 3;
				if (ShotCount == 2)
				{

					ace.pos.x += ace.move.x;
					ace.pos.y += ace.move.y;
					ace.pos.z += ACE_VALUE_MOVE * 2;
				}
				if (ace.AceMoveCount == 2 && ace.pos.x <= -300)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}

			// 動きの変数が３なら
			else if (ace.AceMoveCount == 3)
			{
				SetAceHomingBullet(GetAcePos(), 10.0f, 10.0f);
				ace.move.x += ACE_VALUE_MOVE;
				ace.pos.x += ace.move.x;

				if (ace.AceMoveCount == 3 && ace.pos.x >= 400)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}

			// 動きの変数が４なら
			else if (ace.AceMoveCount == 4)
			{
				SetAceHomingBullet(GetAcePos(), 10.0f, 10.0f);
				ace.move.x -= ACE_VALUE_MOVE;
				ace.move.y -= ACE_VALUE_MOVE;
				ace.move.z -= ACE_VALUE_MOVE;
				ace.pos.x -= ace.move.x;
				ace.pos.y -= ace.move.y;
				ace.pos.z += ace.move.z;
				if (ace.pos.x <= -400 && ace.AceMoveCount == 4)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}
			// 動きの変数が５なら
			else if (ace.AceMoveCount == 5)
			{
				SetAceHomingBullet(GetAcePos(), 10.0f, 10.0f);
				ace.move.x -= ACE_VALUE_MOVE;
				ace.move.y -= ACE_VALUE_MOVE;
				ace.pos.x += ace.move.x;
				ace.pos.y *= ace.move.y;
				if (ace.AceMoveCount == 5 && ace.pos.y <= 40)
				{
					ace.AceMoveCount = (rand() % 4);
				}
			}

			// エースの移動可能領域
			if (ace.pos.y > 530.0f)
			{
				ace.pos.y = 530.0f;
			}
			if (ace.pos.y < 15.0f)
			{
				ace.pos.y = 15.0f;
			}

			//壁との判定
			if (ace.pos.x <= ACE_MOVE_PLACE_LEFT)
			{//右側の壁
				ace.pos.x = ACE_MOVE_PLACE_LEFT;
			}

			if (ace.pos.x >= ACE_MOVE_PLACE_RIGHT)
			{//左側の壁
				ace.pos.x = ACE_MOVE_PLACE_RIGHT;
			}

		}

		else if (ace.hp > ACE_DEAD&&ace.hp <= ACE_DEAD + 5500)
		{
			ace.move.z -= 0.0005f;
			ace.pos.z -= ace.move.z;

			if (ace.pos.z = -100)
			{
				SetAceHomingBullet(GetAcePos(), 10.0f, 10.0f);
			}

			if (FLT_EPSILON < D3DXVec3Dot(&ace.pos, &ace.pos))
			{
				ace.rot.y = Cwk->RotCameraP.y + D3DX_PI;
			}
		}
	}
	// HPが０以下になったら
	if (ace.hp <= ACE_DEAD)
	{
		ace.AceDeadMove();
	}

	// 実際の移動
	ace.pos += ace.move;

	// モデルの見た目にコリジョンを合わせてみる
	ace.move = ace.pos + ace.move * 5;


	if (FLT_EPSILON < D3DXVec3Dot(&ace.pos, &ace.pos))
	{
		ace.rot.y = Cwk->RotCameraP.y + D3DX_PI;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAce(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxScl, MtxRot, MtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;

	//ボスがtrueなら描画する
	if (ace.use == true)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&MtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&MtxScl, ace.scl.x, ace.scl.y, ace.scl.z);
		D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &MtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&MtxRot, ace.rot.y, ace.rot.x, ace.rot.z);
		D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &MtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&MtxTranslate, ace.pos.x, ace.pos.y, ace.pos.z);
		D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &MtxTranslate);

		// ワールドマトリックスの設定
		Device->SetTransform(D3DTS_WORLD, &MtxWorld);

		// 現在のマテリアルを取得
		Device->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		D3DXMat = (D3DXMATERIAL*)D3DXBuffMatAce->GetBufferPointer();

		for (int CntMat = 0; CntMat < (int)NumMatAce; CntMat++)
		{
			// マテリアルの設定
			Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureAce);

			// 描画
			D3DXMeshAce->DrawSubset(CntMat);
		}
		// マテリアルをデフォルトに戻す
		Device->SetMaterial(&matDef);



		// エンジンエフェクト

			// 右側
		D3DXVECTOR3 Pos;

		Pos.x = ace.pos.x + sinf(ace.rot.y) * 10.0f + 10;
		Pos.y = ace.pos.y;
		Pos.z = ace.pos.z + cosf(ace.rot.y) * 12.0f - 40;
		ace.rot.z = 0.0f;

		// エフェクトの設定
		SetEffect(Pos, D3DXVECTOR3(0.0f, 0.0f, -10.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.850f), 24.0f, 24.0f, 20);

		// 左側
		D3DXVECTOR3 Posl;

		Posl.x = ace.pos.x + sinf(ace.rot.y) * 10.0f - 10;
		Posl.y = ace.pos.y;
		Posl.z = ace.pos.z + cosf(ace.rot.y) * 12.0f - 40;
		ace.rot.z = 0.0f;

		// エフェクトの設定
		SetEffect(Posl, D3DXVECTOR3(0.0f, 0.0f, -10.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.850f), 24.0f, 24.0f, 20);

	}
}

//=============================================================================
// エースを取得(GET)
//=============================================================================
Ace *GetAce(void)
{
	return &ace;
}

//=============================================================================
// エースの位置を取得(GET)
//=============================================================================
D3DXVECTOR3	GetAcePos(void)
{
	return ace.pos;
}
//=============================================================================
// エースの位置を取得(GET)
//=============================================================================
D3DXVECTOR3	*GetPosAce(void)
{
	return (&ace.pos);
}
//=============================================================================
// エースの回転を取得(GET)
//=============================================================================
D3DXVECTOR3	GetRotAce(void)
{
	return(ace.rot);
}

//=============================================================================
// エースのワールドマトリクスを取得(GET)
//=============================================================================
D3DXMATRIX *GetAceWorldMatrix(void)
{
	return (&MtxWorld);
}
