//=============================================================================
//
// Class処理 [gameobject.h]
// Author : 西村任弘
//
//=============================================================================
#include "main.h"

//******************************************************************************
// ゲームオブジェクトクラス（基底クラス）
//******************************************************************************
class GameObject
{
public:

	D3DXVECTOR3			pos;				// プレイヤーの位置
	D3DXVECTOR3			rot;				// プレイヤーの向き(回転)
	D3DXVECTOR3			scl;				// プレイヤーの大きさ(スケール)
	D3DXVECTOR3			move;				// 移動量
	D3DXVECTOR3			RotDest;			// 目的の向き
	bool				use;				// 使用、未使用の判定
	int					hp;					// 共通のHP項目
	float               radius;				// 半径
};


//*******************************************************************************
//　子クラス
//*******************************************************************************

//=============================================================================
// プレイヤークラス（親：GameObject)
//=============================================================================
class Player :public GameObject
{
public:

	int                PlayerMove;			// プレイヤーの動き(0,入場時、1,ゲームプレイ時、2,敗北時、3,勝利時)
	int                mp;					// バレット発射までのｍｐの設定
	int                IdxShadow;			// 影ID
	int                Flame;
	void               FirstBattlePlayerInitStatus();	// 1stステージ用のプレイヤーの初期化処理
	void               BossBattlePlayerInitStatus();	// ボスステージ用のプレイヤーの初期化処理
	void			   PlayerDeadMove(float a, float b);
	void			   PlayerWinMove(int z);
	void			   PlayerMoveLimit();
};

//=============================================================================
// エースクラス（親：GameObject)
//=============================================================================
class Ace :public GameObject
{
public:
	
	int                ShotCount;		// バレットの発射間隔
	int				   AceMoveCount;	// 動きの判定用変数
	void			   AceInitStatus(); // エースのクラスパラメータ初期化関数
	void			   AceDeadMove();	// エースの死亡時の動きの関数
	
};

//=============================================================================
// ボスクラス（親：GameObject)
//=============================================================================
class Boss:public GameObject
{
public:
	void BossInitStatus();					// ボスのクラスパラメータ初期化の関数

};

//=============================================================================
// エネミークラス（親：GameObject)
//=============================================================================
class Enemy :public GameObject
{
public:
	
	
	int		          IdxShadow;	// 影ID
	int		          Type;			// 種類

	//ベジェ
	D3DXVECTOR3       start;		//ホーミング開始位置
	D3DXVECTOR3       c1;			//通過ポイント１
	D3DXVECTOR3       c2;			//通過ポイント２

	void EnemyInit();				// エネミーのクラスパラメータ初期化の関数

};

//=============================================================================
// バレットクラス（親：GameObject)
//=============================================================================
class Bullet :public GameObject
{
public:
	float SizeX;			// 幅
	float SizeY;			// 高さ
	int Timer;				// タイマー
	int IdxShadow;			// 影ID

	void BulletInit();		// バレットのクラスパラメータ初期化の関数
};

//=============================================================================
// ホーミングクラス（親：Bullet)
//=============================================================================

class  Homingbullet :public Bullet
{
public:
	int  Frame;
	int HomingGage;

	//ベジェ
	D3DXVECTOR3 start;		// ホーミング開始位置
	D3DXVECTOR3 bc1;		// 通貨ポイント１
	D3DXVECTOR3 bc2;		// 通貨ポイント２
	D3DXVECTOR3 targetpos;  // ホーミングの終点

	void BossHomingbulletInit();	//ボス用の初期化関数
	void AceHomingbulletInit();		//エース用の初期化関数
	void PlayerHomingbulletInit();	//プレイヤー用の初期化関数

	void PlayerHoming(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);	//プレイヤーのホーミング
	void AceHoming();										// エースのホーミング
	void BossHoming();										// ボスのホーミング
};

//=============================================================================
// エフェクトクラス（親：GameObject)
//=============================================================================
class Effect1 :public GameObject
{
public:
	D3DXCOLOR col;			// 色
	float SizeX;			// 幅
	float SizeY;			// 高さ
	int   Count;			// カウント
	int   Pattern;			// パターン
	int	  Timer;			// タイマー
	float DecAlpha;			// 減衰値
};

//=============================================================================
// アイテムクラス（親：GameObject)
//=============================================================================
class Item :public GameObject
{
public:

	int IdxShadow;			// 影ID
	int Type;				// 種類
};


#pragma once