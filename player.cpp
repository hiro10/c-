//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "wallleft.h"
#include "wallright.h"
#include "item.h"
#include "homingbullet.h"
#include "bossbullet.h"
#include "fade.h"
#include "score.h"
#include "timer.h"
#include "enemy.h"
#include "boss.h"
#include "spacebox.h"
#include "explosion.h"
#include "sound.h"
#include "collision.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTexturePlayer;		// テクスチャ読み込み場所
LPD3DXMESH			MeshPlayer;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXMatBuffPlayer;		// メッシュのマテリアル情報を格納
DWORD				NumMatPlayer;			// 属性情報の総数

D3DXMATRIX			WorldPlayer;			// ワールドマトリックス
Player				player;					// プレイヤーワーク

//*****************************************************************************
// 1stステージ用の初期化（メンバー関数）
//*****************************************************************************
void Player::FirstBattlePlayerInitStatus()
{
	pos = D3DXVECTOR3(0.0f, 380.0f, -1000.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	RotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	radius = PLAYER_RADIUS;
	use = true;
	PlayerMove = 0;
	hp = PLAYER_LIFEMAX;
	mp = 0;
	Flame = 0;
}

//*****************************************************************************
// ボスステージ用の初期化（メンバー関数）
//*****************************************************************************
void Player::BossBattlePlayerInitStatus()
{
	pos = D3DXVECTOR3(0.0f, 1280.0f, -1000.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	RotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	radius = PLAYER_RADIUS;
	use = true;
	PlayerMove = 0;
	mp = 0;
	Flame = 0;
}

//*****************************************************************************
// プレイヤーの死亡演出（メンバー関数）
//*****************************************************************************
void Player::PlayerDeadMove(float a,float b)
{
	// 爆発の設定
	SetExplosion(pos, a, b);

	//　プレイヤーの動きのカウントを2に
	PlayerMove = 2;

	if (PlayerMove == 2)
	{
		// 墜落演出用の回転
		rot.y -= 0.2f;

		// 墜落演出用の下降
		move.y -= VALUE_MOVE_PLAYER;

		// プレイヤーのy軸位置が10以下になったら
		if (pos.y <= 10)
		{
			// 爆発の設定
			SetExplosion(pos, a, b);

			// SE再生
			PlaySound(SOUND_LABEL_SE_ENEMYEXPLOSION);

			// SE再生
			PlaySound(SOUND_LABEL_SE_FALSE);

			// 未使用に
			use = false;

			// フェードアウト
			SetFade(FADE_OUT);
		}
	}
}

//*****************************************************************************
// プレイヤーの勝利演出（メンバー関数）
//*****************************************************************************
void Player::PlayerWinMove(int z)
{
	// プレイヤーの動きのカウントを3に
	PlayerMove = 3;

	if (PlayerMove == 3)
	{
		// 向きを初期値に戻す
		rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

		// z軸方向へ移動
		move.z += VALUE_MOVE_PLAYER;

		if (pos.z >= z)
		{
			// フェードアウト
			SetFade(FADE_OUT);
		}
	}
}

//*****************************************************************************
// プレイヤーの移動制限（メンバー関数）
//*****************************************************************************
void Player::PlayerMoveLimit()
{
	//MODE_GAMEの時の移動範囲
	if (GetMode() == MODE_GAME)
	{
		if (PlayerMove == 1)
		{
			// ｙ軸の移動範囲
			if (pos.y > 590)
			{
				pos.y = 590;
			}
			if (pos.y <= 30)
			{
				pos.y = 30;
			}

			// x軸の移動範囲
			if (pos.x > WALLRIGNT_VTX_X - 30)
			{
				pos.x = WALLRIGNT_VTX_X - 30;
			}
			if (pos.x <= -WALLLEFE_VTX_X + 30)
			{
				pos.x = -WALLLEFE_VTX_X + 30;
			}

			// z軸の移動範囲
			if (pos.z > 3000)
			{
				pos.z = 3000;
			}
			if (pos.z <= 80)
			{
				pos.z = 80;
			}
		}
	}

	// ボス戦での移動範囲
	else if (GetMode() == MODE_BOSS)
	{
		if (PlayerMove == 1)
		{
			// ｙ軸の移動範囲
			if (pos.y > 2600)
			{
				pos.y = 2600;
			}
			if (pos.y <= 230)
			{
				pos.y = 230;
			}

			// x軸の移動範囲
			if (pos.x > 1200)
			{
				pos.x = 1200;
			}
			if (pos.x <= -1200)
			{
				pos.x = -1200;
			}

			// z軸の移動範囲
			if (pos.z > 0)
			{
				pos.z = 0;
			}
			if (pos.z <= -0)
			{
				pos.z = -0;
			}
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DTexturePlayer = NULL;
	MeshPlayer = NULL;
	D3DXMatBuffPlayer = NULL;

	// ノーマルモードの時
	if (GetMode() == MODE_GAME)
	{
		// 1stステージ用の初期化用メンバ関数呼び出し
		player.FirstBattlePlayerInitStatus();
	}
	// ボスモードの時
	else if (GetMode() == MODE_BOSS)
	{
		// ボスステージ用の初期化用メンバ関数呼び出し
		player.BossBattlePlayerInitStatus();
	}

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER,
		D3DXMESH_SYSTEMMEM,
		Device,
		NULL,
		&D3DXMatBuffPlayer,
		NULL,
		&NumMatPlayer,
		&MeshPlayer)))
	{
		return E_FAIL;
	}

#if 1
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,		// デバイスへのポインタ
		PLAER_TEXTURE_FILENAME,				// ファイルの名前
		&D3DTexturePlayer);				// 読み込むメモリー
#endif

	// 影の設定
	player.IdxShadow = SetShadow(player.pos, player.radius * 2.0f, player.radius * 2.0f, player.radius * 2.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if (D3DTexturePlayer != NULL)
	{// テクスチャの開放
		D3DTexturePlayer->Release();
		D3DTexturePlayer = NULL;
	}

	if (MeshPlayer != NULL)
	{// メッシュの開放
		MeshPlayer->Release();
		MeshPlayer = NULL;
	}

	if (D3DXMatBuffPlayer != NULL)
	{// マテリアルの開放
		D3DXMatBuffPlayer->Release();
		D3DXMatBuffPlayer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//カメラの向きの取得
	D3DXVECTOR3 RotCamera;

	Camera *Cwk;
	Cwk = GetCamerawk();

	RotCamera = GetRotCamera();


	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	// アイテムの取得
	Item *item;
	item = GetItem();
	
	if (player.use == true)
	{
		if (player.PlayerMove == 0)
		{
			player.move.z = 5.0f;

			if (player.pos.z > 0.0f)
			{
				player.PlayerMove = 1;
			}

			else if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_TOUCH))
			{
				player.pos.z = 0.0f;

				player.PlayerMove = 1;
			}
		}

		// プレイヤーの動ける範囲の制限
		player.PlayerMoveLimit();
		
		// プレイヤーの移動処理
		if (player.PlayerMove == 1)
		{
			player.RotDest.y = D3DX_PI + RotCamera.y;

			if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
			{
				if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
				{// 左前移動
					player.move.x -= sinf(RotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

				}
				else
				{// 左移動
					player.move.x -= sinf(RotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

					player.rot.z -= 0.01f;

					if (player.rot.z <= -0.3f)
					{
						player.rot.z = -0.3f;
					}
				}
			}

			else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
			{
				{// 右移動
					player.move.x -= sinf(RotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
					player.move.z -= cosf(RotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

					player.rot.z += 0.01f;

					if (player.rot.z >= 0.3f)
					{
						player.rot.z = 0.3f;
					}
				}
			}
			else
			{
				// 違う場合、ｚ軸の回転を0に戻す
				player.rot.z = 0.0f;
			}

			if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
			{// 上昇
				player.rot.x += 0.009f;
				player.move.y += VALUE_MOVE_PLAYER;
				if (player.rot.x >= 0.3f)
				{
					player.rot.x = +0.3f;
				}
			}

			else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
			{// 下降
				player.rot.x -= 0.002f;
				player.move.y -= VALUE_MOVE_PLAYER;
				if (player.rot.x <= -0.3f)
				{
					player.rot.x = -0.3f;
				}
			}
			else
			{// 違う場合、x軸の回転を0に戻す
				player.rot.x = 0.0f;
			}

			// 弾発射
			if (GetKeyboardPress(DIK_SPACE) || IsButtonPressed(0, BUTTON_R))
			{
				// 発射感覚の調整
				player.Flame++;
				player.Flame %= PLAYER_BULLET_FLAME;

				if (player.Flame == 0 && (GetKeyboardPress(DIK_SPACE) || IsButtonPressed(0, BUTTON_R)))
				{
					D3DXVECTOR3 pos;
					D3DXVECTOR3 move;

					pos.x = player.pos.x - sinf(player.rot.y) * 10.0f;
					pos.y = player.pos.y;
					pos.z = player.pos.z - cosf(player.rot.y) * 20.0f;

					move.x = -sinf(player.rot.y) * VALUE_MOVE_PLAYER_BULLET;
					move.y = 0.0f;
					move.z = -cosf(player.rot.y) * VALUE_MOVE_PLAYER_BULLET;

					// バレットのセット
					SetBullet(pos, move, 14.0f, 14.0f, 100);

					// SE再生
					PlaySound(SOUND_LABEL_SE_SHOT);
				}

			}
			// ロックオンしたときのみホーミングが撃てる
			if (Cwk->LockOnTrigger == true)
			{
				if (GetPlayer()->mp >= 100)
				{
					// キーボードならHキー、ゲームパッドならR1を押したら
					if (GetKeyboardPress(DIK_H) || IsButtonPressed(0, BUTTON_R1))
					{
						// ホーミング弾のセット
						SetHomingBullet(player.pos, 0.01f, 0.01f);
					}
				}
			}
		}
		// 位置移動
		player.pos.x += player.move.x;
		player.pos.y += player.move.y;
		player.pos.z += player.move.z;

		// 移動量に慣性をかける
		player.move.x += (0.0f - player.move.x) * RATE_MOVE_PLAYER;
		player.move.y += (0.0f - player.move.y) * RATE_MOVE_PLAYER;
		player.move.z += (0.0f - player.move.z) * RATE_MOVE_PLAYER;

		float colA = (200.0f - (player.pos.y - 10.0f)) / 400.0f;
		if (colA < 0.0f)
		{
			colA = 0.0f;
		}
		SetColorShadow(player.IdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));

		// 影の位置設定
		if (player.pos.x >= WALLRIGNT_VTX_X / 2)
		{
			SetPositionShadow(player.IdxShadow, D3DXVECTOR3(WALLRIGNT_VTX_X - 1, player.pos.y, player.pos.z));
			SetVertexShadow(player.IdxShadow, 0, 30.0f, 30.0f, 30.0f);
		}
		else if (player.pos.x <= -WALLLEFE_VTX_X / 2)
		{
			SetPositionShadow(player.IdxShadow, D3DXVECTOR3(-WALLLEFE_VTX_X + 1, player.pos.y, player.pos.z));
			SetVertexShadow(player.IdxShadow, 1, 30.0f, 30.0f, 30.0f);
		}


		// エフェクトの設定
		SetEffect(D3DXVECTOR3(player.pos.x, player.pos.y, player.pos.z - 14), D3DXVECTOR3(0.0f, 0.0f, -5.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 1.2f), 20.0f, 20.0f, 5);
		SetEffect(D3DXVECTOR3(player.pos.x, player.pos.y, player.pos.z - 14), D3DXVECTOR3(0.0f, 0.0f, -6.0f),
			D3DXCOLOR(0.65f, 0.85f, 0.05f, 1.30f), 10.0f, 10.0f, 5);

		// 敗北演出
		if (player.hp <= PLAYER_DEAD)
		{
			player.PlayerDeadMove(50,50);
		}

		// 勝利演出
		else if (GetMode() == MODE_BOSS && boss->use == false && boss->hp <= PLAYER_DEAD)
		{
			player.PlayerWinMove(2000);
		}

		// 実際の移動
		player.pos += player.move;

		// アイテム（機雷）との当たり判定
		for (int CntItem = 0; CntItem < MAX_ITEM; CntItem++, item++)
		{
			if (item->use == true)
			{
				// プレイヤーとの当たり判定
				if (AttackCheak(player.pos, item->pos,
					player.radius, item->radius))
				{
					SetExplosion(item->pos, 40.0f, 40.0f);

					// アイテム消去
					DeleteItem(CntItem);

					// HPの減少
					player.hp -= ITEM_DAMAGE;

					// SE再生
					PlaySound(SOUND_LABEL_SE_ENEMYEXPLOSION);

					break;
				}
			}
		}

		// エネミーとの当たり判定
		Enemy *enemy;

		for (int CntEnemy = 0; CntEnemy < MAX_ENEMY; CntEnemy++, enemy++)
		{
			enemy = GetEnemy(CntEnemy);

			if (enemy->use == true)
			{
				// プレイヤーとの当たり判定
				if (AttackCheak(player.pos, enemy->pos,
					player.radius, enemy->radius))
				{
					SetExplosion(enemy->pos, 40.0f, 40.0f);

					player.hp -= ENEMY_DAMAGE;
					// アイテム消去
					DeleteEnemy(CntEnemy);

					// SE再生
					PlaySound(SOUND_LABEL_SE_ENEMYEXPLOSION);

					break;
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	if (player.use == true)
	{
		LPDIRECT3DDEVICE9 Device = GetDevice();
		D3DXMATRIX MtxRot, MtxTranslate;
		D3DXMATERIAL *D3DXMat;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&WorldPlayer);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&MtxRot, player.rot.y, player.rot.x, player.rot.z);
		D3DXMatrixMultiply(&WorldPlayer, &WorldPlayer, &MtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&MtxTranslate, player.pos.x, player.pos.y, player.pos.z);
		D3DXMatrixMultiply(&WorldPlayer, &WorldPlayer, &MtxTranslate);

		// ワールドマトリックスの設定
		Device->SetTransform(D3DTS_WORLD, &WorldPlayer);

		// マテリアル情報に対するポインタを取得
		D3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer->GetBufferPointer();

		for (int CntMat = 0; CntMat < (int)NumMatPlayer; CntMat++)
		{
			// マテリアルの設定
			Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTexturePlayer);

			// 描画
			MeshPlayer->DrawSubset(CntMat);
		}

		{// マテリアルをデフォルトに戻す
			D3DXMATERIAL Mat;

			Mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			Mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			Mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			Device->SetMaterial(&Mat.MatD3D);
		}
	}
}

//=============================================================================
// プレイヤーを取得
//=============================================================================
Player *GetPlayer(void)
{
	return &player;
}

//=============================================================================
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return player.pos;
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return player.rot;
}

//=============================================================================
// 目的の向き取得
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return player.RotDest;
}

//=============================================================================
// 移動量取得
//=============================================================================
D3DXVECTOR3 GetMovePayer(void)
{
	return player.move;
}