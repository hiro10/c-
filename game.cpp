//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "game.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "meshfield.h"
#include "player.h"
#include "boss.h"
#include "bossbullet.h"
#include "field.h"
#include "bossgauge.h"
#include "fieldup.h"
#include "enemy.h"
#include "wallleft.h"
#include "wallright.h"
#include "bosshomingbullet.h"
#include "bossgauge.h"
#include "gauge.h"
#include "homingbullet.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "billboard.h"
#include "explosion.h"
#include "ace.h"
#include "timer.h"
#include "pause.h"
#include "score.h"
#include "item.h"
#include "centerlock.h" 
#include "spacebox.h"
#include "sound.h"
#include "acehomingbullet.h"
#include "ui.h"
#include "mpgauge.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
bool StopTrigger ;		// ポーズ用のトリガー

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	// ポーズトリガーの設定
	StopTrigger = false;

	// ライトの初期化
	InitLight();

	// カメラの初期化
	InitCamera();

	// フィールドの初期化
	InitField();

	// 左の壁の初期化
	InitWallleft();

	// 右の壁の初期化
	InitWallright();

	// 天井の壁の初期化
	InitFieldup();

	// 中央のカーソルの初期化
	InitCenterLock();

	// ポーズ画面の初期化
	InitPause();

	// 影の初期化
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	// ロックオンの初期化
	InitBillboard();

	// ボスの初期化
	InitAce();

	// 弾の初期化
	InitBullet();

	// ホーミング弾の初期化
	InitHomingBullet();

	// ホーミング弾の初期化
	InitAceHomingBullet();

	// 爆発の初期化
	InitExplosion();

	// エフェクトの初期化
	InitEffect();

	// ライフの初期化
	InitGauge();

	// MPゲージの初期化処理
	InitMpGauge();

	// タイマーの初期化
	InitTimer();
	ResetTimer(60);

	// スコアの初期化
	InitScore();

	// skyboxの初期化
	InitSpace();

	// UIの初期化
	InitUi();

	// アイテムの初期化
	InitItem();

	for(int CntCoin = 0; CntCoin < 600000; CntCoin++)
	{
		float fPosX, fPosY, fPosZ;

		fPosX = (float)(rand() % 12000) / 10.0f - 600.0f;
		fPosY = (float)(rand()%600)/1.0f;
		fPosZ = (float)(rand() % 20000)+ ITEM_FIRST_POS;
		SetItem(D3DXVECTOR3(fPosX, fPosY, fPosZ),  D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_COIN); 
	}

	// BGM再生
	PlaySound(SOUND_LABEL_BGM000);

	return S_OK;
}

//=============================================================================
// ボス戦用の初期化処理
//=============================================================================
HRESULT InitBossGame(void)
{
	// カメラの初期化
	InitCamera();

	// 地面の初期化
	InitMeshField(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 16, 2800.0f, 2800.0f);

	// プレイヤーの初期化
	InitPlayer();

	// ボスの初期化
	InitBoss();

	// ホーミングバレットの初期化
	InitHomingBullet();

	// ボスのゲージの初期化
	InitBossGauge();

	// ボスバレットの初期化
	InitBossBullet();

	// ボスのホーミングの初期化
	InitBossHomingBullet();

	// バレットの初期化
	InitBullet();

	// 爆発の初期化
	InitExplosion();

	// エフェクトの初期化
	InitEffect();

	// タイマーの初期化
	InitTimer();
	ResetTimer(90);

	// UIの初期化
	InitUi();

	// BGM再生
	PlaySound(SOUND_LABEL_BGM001);

	return S_OK;
}

//=============================================================================
// エネミー戦の終了処理
//=============================================================================
void UninitGame(void)
{
	// フィールドの終了処理
	UninitField();

	// 爆発の終了処理
	UninitExplosion();

	// エフェクトの終了処理
	UninitEffect();

	// 左の壁の終了処理
	UninitWallleft();

	// 右の壁の終了処理
	UninitWallright();

	// 天井の終了処理
	UninitFieldup();

	// プレイヤーの終了処理
	UninitPlayer();

	// タイムの終了処理
	UninitTimer();

	// エフェクトの終了処理
	UninitBullet();

	// アイテムの終了処理
	UninitHomingBullet();

	// アイテムの終了処理
	UninitAceHomingBullet();

	// アイテムの終了処理
	UninitItem();

	// エースの終了処理
	UninitAce();

	// エネミーの終了処理
	UninitEnemy();

	// ACEの終了処理
	UninitAce();

	// UIの終了処理
	UninitUi();

	// BGM停止
	StopSound(SOUND_LABEL_BGM000);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBossGame(void)
{
	// ライトの終了処理
	UninitLight();

	// カメラの終了処理
	UninitCamera();

	// 地面の終了処理
	UninitMeshField();

	// 影の終了処理
	UninitShadow();

	// プレイヤーの終了処理
	UninitPlayer();

	// ボスの終了処理
	UninitBoss();

	// ビルボードの終了処理
	UninitBillboard();

	// 弾の終了処理
	UninitBullet();

	// ホーミング弾の初期化
	UninitHomingBullet();

	// ボスバレットの初期化
	UninitBossBullet();

	// ボスのゲージの終了処理
	UninitBossGauge();

	// ボスのホーミングの終了処理
	UninitBossHomingBullet();

	// 爆発の終了処理
	UninitExplosion();

	// エフェトの終了処理
	UninitEffect();

	// ライフの終了処理
	UninitGauge();

	// Mpゲージの終了処理
	UninitMpGauge();

	// タイマーの終了処理
	UninitTimer();

	// スコアの終了処理
	UninitScore();

	// spaceboxの終了処理
	UninitSpace();

	// ポーズ画面の終了処理
	UninitPause();

	// 中央のロックオンの終了処理
	UninitCenterLock();

	// UIの終了処理
	UninitUi();

	// BGM停止
	StopSound(SOUND_LABEL_BGM001);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	// ポーズの更新
	UpdatePause();

	if (StopTrigger == true)
	{
		if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_OPTIONS))
		{
			StopTrigger = false;

			// キャンセル音の再生
			PlaySound(SOUND_LABEL_SE_CANCEL);
		}

		else if (GetKeyboardTrigger(DIK_L))
		{
			// 終了処理
			UninitGame();
			UninitBossGame();

			// タイトルへ
			SetMode(MODE_TITLE);
			
		}

	}
	else if ((GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_OPTIONS)) && StopTrigger == false)
	{
		// 停止のトリガーを使用に
		StopTrigger = true;
	}
	
	// 停止のトリガーが未使用なら
	if (StopTrigger == false)
	{
		// カメラ更新
		UpdateCamera(GetAcePos(), GetPositionPlayer(), CAMERA_LOCK);

		// フィールドの更新
		UpdateField();

		// 左の壁の更新
		UpdateWallleft();

		// 右の壁の更新
		UpdateWallright();

		// 天井の更新
		UpdateFieldup();

		// 影処理の更新
		UpdateShadow();

		// エフェクト処理の更新
		UpdateEffect();

		// プレイヤー処理の更新
		UpdatePlayer();

		// エネミーの更新
		UpdateEnemy();

		// ビルボードの更新
		UpdateBillboard();

		// ボス処理の更新
		UpdateAce();

		// 弾処理の更新
		UpdateBullet();

		// ホーミング弾の更新
		UpdateHomingBullet();

		// ホーミング弾の更新
		UpdateAceHomingBullet();

		// 爆発処理の更新
		UpdateExplosion();

		if (GetPositionPlayer().z > 0.0f)
		{
			// ライフ処理の更新
			UpdateGauge();

			// Mpゲージの更新処理
			UpdateMpGauge();

			// タイマー処理の更新
			UpdateTimer();

			// スコア処理の更新
			UpdateScore();
		}
		// アイテム処理の更新
		UpdateItem();

		// 中央のロックオンカーソルの更新
		UpdateCenterLock();

		// UIの更新
		UpdateUi();
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBossGame(void)
{
	// ポーズの更新
	UpdatePause();

	if (StopTrigger == true)
	{
		if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_OPTIONS))
		{
			StopTrigger = false;
			
			// キャンセル音の再生
			PlaySound(SOUND_LABEL_SE_CANCEL);
		}

		else if (GetKeyboardTrigger(DIK_L))
		{
			// 終了処理
			UninitGame();

			// Bossステージの終了処理
			UninitBossGame();

			// タイトルへ
			SetMode(MODE_TITLE);

		}
	}
	else if ((GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_OPTIONS)) && StopTrigger == false)
	{
		// ストップトリガーを使用に
		StopTrigger = true;
	}
	
	// ストップトリガーが未使用なら
	if (StopTrigger == false)
	{
		// カメラ更新
		UpdateCamera( GetBossPos(),GetPositionPlayer(),  CAMERA_LOCK);

		// Skybox_Saceの更新
		UpdateSpace();

		// 地面処理の更新
		UpdateMeshField();

		// 影処理の更新
		UpdateShadow();

		// プレイヤー処理の更新
		UpdatePlayer();

		// 中央ロックオンカーソルの更新
		UpdateCenterLock();

		// boss処理の更新
		UpdateBoss();

		// エフェクト処理の更新
		UpdateEffect();

		// ボスバレットの更新
		UpdateBossBullet();

		// ボスのホーミングの更新
		UpdateBossHomingBullet();

		// ロックオンの更新
		UpdateBillboard();

		// 弾処理の更新
		UpdateBullet();

		// ホーミング弾の更新
		UpdateHomingBullet();

		// 爆発処理の更新
		UpdateExplosion();

		// ライフ処理の更新
		UpdateGauge();

		// タイマー処理の更新
		UpdateTimer();

		// スコア処理の更新
		UpdateScore();

		// アイテム処理の更新
		UpdateItem();

		// Mpゲージの更新処理
		UpdateMpGauge();

		// ボスのゲージの更新
		UpdateBossGauge();

		// UIの更新
		UpdateUi();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	// カメラの設定
	DrawCamera();

	// spacebox
	DrawSpace();

	// ボス処理の描画
	DrawAce();

	// プレイヤー処理の描画
	DrawPlayer();

	// エネミーの描画
	DrawEnemy();

	// アイテム処理の描画
	DrawItem();

	// 弾処理の描画
	DrawBullet();

	// ホーミング弾の描画
	DrawHomingBullet();

	// ホーミング弾の描画
	DrawAceHomingBullet();

	// 右の壁の描画処理
	DrawWallright();

	// 左の壁の描画処理
	DrawWallleft();

	// 天井の描画処理
	DrawFieldup();

	// フィールドの描画処理
	DrawField();

	// 爆発処理の描画
	DrawExplosion();

	// エフェク()+ト処理の描画
	DrawEffect();

	if (GetPositionPlayer().z >= 100.0f)
	{
		// ビルボードの描画
		DrawBillboard();
	}

	if (GetPositionPlayer().z > 0.0f)
	{
		if (StopTrigger == false)
		{
			// 中央のロックオンの描画
			DrawCenterLock();

			// ライフ処理の描画
			DrawGauge();

			// Mpゲージの描画処理
			DrawMpGauge();

			// タイマー処理の描画
			DrawTimer();

			// スコア処理の描画
			DrawScore();

			// UIの描画
			DrawUi();
		}
	}

	// 影処理の描画
	DrawShadow();


	if (StopTrigger == true)
	{
		// ポーズ画面の描画
		DrawPause();
	}
}

void DrawBossGame(void)
{
	// カメラの設定
	DrawCamera();

	// spaceboxの描画
	DrawSpace();

	// 地面処理の描画
	DrawMeshField();

	// boss処理の描画
	DrawBoss();

	// プレイヤー処理の描画
	DrawPlayer();

	// 爆発処理の描画
	DrawExplosion();

	if (GetPositionPlayer().z >= 100.0f)
	{
		// ビルボードの描画
		DrawBillboard();
	}

	// ボスの弾の描画
	DrawBossBullet();

	// ボスのホーミングの描画
	DrawBossHomingBullet();

	// エフェク()+ト処理の描画
	DrawEffect();

	if (StopTrigger == false)
	{
		// ライフ処理の描画
		DrawGauge();

		// タイマー処理の描画
		DrawTimer();

		// スコア処理の描画
		DrawScore();

		// ボスのゲージの描画
		DrawBossGauge();

		// Mpゲージの描画処理
		DrawMpGauge();

		if (GetPositionPlayer().z > 0.0f)
		{
			// 中央のカーソルの描画
			DrawCenterLock();
		}
	}

	// UIの描画
	DrawUi();

	if (StopTrigger == true)
	{
		// ポーズの描画
		DrawPause();
	}
}
