//=============================================================================
//
// メイン処理 [camera.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "ace.h"
#include "boss.h"
#include "sound.h"
#include "enemy.h"
#include "main.h"

//=============================================================================
//グローバル変数
//=============================================================================
Camera		Cwk[2];

//=============================================================================
// カメラ視点移動のz軸制限（メンバー関数
//=============================================================================
void Camera::CameraAriaLimit(float z1,float z2)
{
	// カメラの移動制限
	if (GetMode() == MODE_GAME)
	{
		// １ｓｔステージのz軸の範囲
		if (PosCameraP.z <= z1)
		{
			PosCameraP.z = z1;
		}
	}
	else if (GetPlayer()->PlayerMove== 3)
	{
		// BOSSステージのz軸の範囲
		if (Cwk->PosCameraP.z >= z2)
		{
			Cwk->PosCameraP.z = z2;
		}
	}
}

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Cwk->PosCameraP = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Cwk->PosCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Cwk->PosCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// カメラ視点の回転角度初期化
	Cwk->RotCameraP = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ロックオントリガーの設定
	Cwk->LockOnTrigger=false;

}
//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}
//=============================================================================
// カメラ視点移動処理
//=============================================================================

// カメラの位置 = カメラの注視点 + 注視点からの相対的位置
// カメラの位置 = カメラの注視点 + (注視点からの角度 * 視点までの距離)

void CalcPosCameraP(D3DXVECTOR3 *at)
{
	D3DXVECTOR3 Vec(0.0f, 0.25f, -1.0f);		// 正規化された注視点からの向き
	D3DXMATRIX	Mtx;							// マトリクス		

	Cwk->PosCameraR = *at;

	// カメラの相対的な回転角度を行列に変換する
	D3DXMatrixRotationYawPitchRoll(&Mtx, Cwk->RotCameraP.y, Cwk->RotCameraP.x, Cwk->RotCameraP.z);

	// 正規化された点に回転行列を掛ける
	D3DXVec3TransformCoord(&Vec, &Vec, &Mtx);

	// カメラの位置 = カメラの注視点 + (注視点からの角度 * 視点までの距離)
	Cwk->PosCameraP = Cwk->PosCameraR + (Vec * Cwk->Cam_len);

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float distance)
{
	// 注視点と視点の距離
	Cwk->Cam_len = CAMERA_CMK;

	// ACEの取得
	Ace *ace;
	ace = GetAce();

	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	// プレイヤーの取得
	Player *player;
	player = GetPlayer();

	// 通常時の注視点取得
	CalcPosCameraP(&D3DXVECTOR3(GetPositionPlayer().x*0.8f, GetPositionPlayer().y+20, GetPositionPlayer().z-70));
	
	// 勝利,敗北演出時のカメラ
	if (player->PlayerMove == 3|| player->PlayerMove == 2)
	{
		// 通常時の注視点取得
		CalcPosCameraP(&D3DXVECTOR3(GetPositionPlayer().x, GetPositionPlayer().y + 20, GetPositionPlayer().z - 70));
	}

	// ロックオンの使用時、未使用時の設定
	if ((GetKeyboardTrigger(DIK_G) || IsButtonTriggered(0, BUTTON_RR)) && player->PlayerMove == 1)
	{
		if (boss->use == true || ace->use == true)
		{
			// ロックオントリガーが未使用なら
			if (Cwk->LockOnTrigger == false)
			{
				// ロック音の再生
				PlaySound(SOUND_LABEL_SE_ROCK);

				// ロックオントリガーを使用に
				Cwk->LockOnTrigger = true;
			}
			else
			{
				// キャンセル音の再生
				PlaySound(SOUND_LABEL_SE_CANCEL);

				// ロックオントリガーを未使用に
				Cwk->LockOnTrigger = false;
			}
		}
	}

	// ロックオンの処理
	// 敵とプレイヤーを結んだ直線上にカメラを設置
	//(※プレイヤーとターゲットの座標が完全に一致するとカメラがバグる)
	if (player->PlayerMove == 1 && Cwk->LockOnTrigger == true)
	{
		// プレイヤーからターゲットヘのベクトル
		D3DXVECTOR3 PlayerToTarget = D3DXVECTOR3(pos1.x - pos2.x, (pos1.y - pos2.y) + 100, pos1.z - pos2.z);

		// プレイヤーからターゲットヘのベクトルの単位ベクトル
		float Length = sqrtf(powf(PlayerToTarget.x, 2.0f) +
			powf(PlayerToTarget.y + 40, 2.0f) + powf(PlayerToTarget.z, 2.0f));

		// 上の逆ベクトルの取得
		D3DXVECTOR3 UnitvecPlayerToTarget =
			D3DXVECTOR3(PlayerToTarget.x / Length, PlayerToTarget.y / Length, PlayerToTarget.z / Length);

		// カメラ位置取得
		Cwk->PosCameraP = D3DXVECTOR3(pos2.x - UnitvecPlayerToTarget.x * distance, GetPositionPlayer().y, pos2.z - UnitvecPlayerToTarget.z * distance);

		// 注視点取得
		Cwk->PosCameraR = D3DXVECTOR3(pos1.x + UnitvecPlayerToTarget.x, pos1.y + UnitvecPlayerToTarget.y, pos1.z + UnitvecPlayerToTarget.z);
	}

	//カメラのｚ軸の移動制限
	Cwk->CameraAriaLimit(FIRST_CAMERA_POS_LIMIT, BOSS_CAMERA_POS_LIMIT);

}
//=============================================================================
// カメラの描画処理
//=============================================================================
void DrawCamera(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&Cwk->MtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&Cwk->MtxView,
		&Cwk->PosCameraP,		// カメラの視点
		&Cwk->PosCameraR,		// カメラの注視点
		&Cwk->PosCameraU);		// カメラの上方向

	// ビューマトリックスの設定
	Device->SetTransform(D3DTS_VIEW, &Cwk->MtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&Cwk->MtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&Cwk->MtxProjection,
		VIEW_ANGLE,										// 視野角
		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),	// アスペクト比
		VIEW_NEAR_Z,									// ビュー平面のNearZ値
		VIEW_FAR_Z);									// ビュー平面のFarZ値

// プロジェクションマトリックスの設定
	Device->SetTransform(D3DTS_PROJECTION, &Cwk->MtxProjection);
}

//=============================================================================
// カメラワークのアドレスを取得
//=============================================================================
Camera *GetCamerawk(void)
{
	return(Cwk);
}

//=============================================================================
// カメラの向きの取得
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return Cwk->RotCameraP;
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return Cwk->MtxView;
}


