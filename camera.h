//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	GAME_MODE_POS_CAMERA_P_X		(0.0f)								// カメラ視点の初期位置(X座標)
#define	GAME_MODE_POS_CAMERA_P_Y		(925.0f)							// カメラ視点の初期位置(Y座標)
#define	GAME_MODE_POS_CAMERA_P_Z		(-100.0f)							// カメラ視点の初期位置(Z座標)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z			(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(100000.0f)										// ビュー平面のFarZ
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)								// 回転量
#define CAMERA_CMK			(100.0f)										// 注視点までの距離
#define CAMERA_LOCK			(170.0f)										// ロックオン時の注視点までの距離
#define FIRST_CAMERA_POS_LIMIT (-30.0f)
#define BOSS_CAMERA_POS_LIMIT (100.0f)
//*****************************************************************************
// 構造体(クラス)定義
//*****************************************************************************

 class  Camera
 {
public:
	D3DXVECTOR3		PosCameraP;				// カメラの視点
	D3DXVECTOR3		PosCameraR;				// カメラの注視点
	D3DXVECTOR3		PosCameraU;				// カメラの上方向
	D3DXMATRIX		MtxView;				// ビューマトリックス
	D3DXMATRIX		MtxProjection;			// プロジェクションマトリックス
	D3DXVECTOR3		Player_To_Target;
	D3DXVECTOR3		Unitvec_Player_To_Target;
	D3DXVECTOR3		RotCameraP;				// カメラの視点
	float			Cam_len;				// 中点から視点までの距離
	bool			LockOnTrigger;			// ロックオンの使用、未使用判定
	
	// カメラのz軸の範囲指定関数
	void			CameraAriaLimit(float z1,float z2);
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float distance);
void DrawCamera(void);
D3DXVECTOR3 GetRotCamera(void);
D3DXMATRIX GetMtxView(void);
Camera *GetCamerawk(void);

#endif
