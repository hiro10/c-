//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // マウスの初期化
void UninitMouse();						// マウスの終了処理
HRESULT UpdateMouse();					// マウスの更新処理

HRESULT InitializePad(void);			// パッド初期化
										//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// パッド検査コールバック
void UpdatePad(void);
void UninitPad(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//------------------------------- keyboard
LPDIRECTINPUT8			DInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	DIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					KeyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
BYTE					KeyStateTrigger[NUM_KEY_MAX];	// キーボードの状態を受け取るワーク
BYTE					KeyStateRepeat[NUM_KEY_MAX];	// キーボードの状態を受け取るワーク
BYTE					KeyStateRelease[NUM_KEY_MAX];	// キーボードの状態を受け取るワーク
int						KeyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

//--------------------------------- Mouse
static LPDIRECTINPUTDEVICE8 Mouse = NULL;	// Mouse

static DIMOUSESTATE2   MouseState;			// マウスのダイレクトな状態
static DIMOUSESTATE2   MouseTrigger;		// 押された瞬間だけON

//--------------------------------- game Pad

static LPDIRECTINPUTDEVICE8	GamePad[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// パッドデバイス

static DWORD	PadState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD	PadTrigger[GAMEPADMAX];
static DWORD	PadRelease[GAMEPADMAX];
static D3DXVECTOR3	PadDiction[GAMEPADMAX][2]; // 軸の方向
static int		PadCount = 0;			// 検出したパッドの数


//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT Hr;

	if (!DInput)
	{
		// DirectInputオブジェクトの作成
		Hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&DInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	// マウスの初期化
	InitializeMouse(hInst, hWnd);

	// パッドの初期化
	InitializePad();

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	UninitMouse();

	// パッドの終了処理
	UninitPad();

	if (DInput)
	{
		DInput->Release();
		DInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	UpdateMouse();

	// パッドの更新
	UpdatePad();

}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT Hr;

	// デバイスオブジェクトを作成
	Hr = DInput->CreateDevice(GUID_SysKeyboard, &DIDevKeyboard, NULL);
	if (FAILED(Hr) || DIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return Hr;
	}

	// データフォーマットを設定
	Hr = DIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(Hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return Hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	Hr = DIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(Hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return Hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	DIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if (DIDevKeyboard)
	{
		DIDevKeyboard->Release();
		DIDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT Hr;
	BYTE KeyStateOld[256];

	// 前回のデータを保存
	memcpy(KeyStateOld, KeyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	Hr = DIDevKeyboard->GetDeviceState(sizeof(KeyState), KeyState);
	if (SUCCEEDED(Hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			KeyStateTrigger[cnt] = (KeyStateOld[cnt] ^ KeyState[cnt]) & KeyState[cnt];
			KeyStateRelease[cnt] = (KeyStateOld[cnt] ^ KeyState[cnt]) & ~KeyState[cnt];
			KeyStateRepeat[cnt] = KeyStateTrigger[cnt];

			if (KeyState[cnt])
			{
				KeyStateRepeatCnt[cnt]++;
				if (KeyStateRepeatCnt[cnt] >= 20)
				{
					KeyStateRepeat[cnt] = KeyState[cnt];
				}
			}
			else
			{
				KeyStateRepeatCnt[cnt] = 0;
				KeyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		DIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (KeyState[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (KeyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (KeyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (KeyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// マウス関係の処理
//=============================================================================

// マウスの初期化
HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow)
{
	HRESULT Result;
	// デバイス作成
	Result = DInput->CreateDevice(GUID_SysMouse, &Mouse, NULL);
	if (FAILED(Result) || Mouse == NULL)
	{
		MessageBox(hWindow, "No Mouse", "Warning", MB_OK | MB_ICONWARNING);
		return Result;
	}
	// データフォーマット設定
	Result = Mouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(Result))
	{
		MessageBox(hWindow, "Can't setup Mouse", "Warning", MB_OK | MB_ICONWARNING);
		return Result;
	}
	// 他のアプリと協調モードに設定
	Result = Mouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(Result))
	{
		MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return Result;
	}

	// デバイスの設定
	DIPROPDWORD Prop;

	Prop.diph.dwSize = sizeof(Prop);
	Prop.diph.dwHeaderSize = sizeof(Prop.diph);
	Prop.diph.dwObj = 0;
	Prop.diph.dwHow = DIPH_DEVICE;
	Prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値

	Result = Mouse->SetProperty(DIPROP_AXISMODE, &Prop.diph);
	if (FAILED(Result))
	{
		MessageBox(hWindow, "Mouse Property error", "Warning", MB_OK | MB_ICONWARNING);
		return Result;
	}

	// アクセス権を得る
	Mouse->Acquire();
	return Result;
}
//---------------------------------------------------------
// マウスの終了処理
//---------------------------------------------------------
void UninitMouse()
{
	if (Mouse)
	{
		Mouse->Unacquire();
		Mouse->Release();
		Mouse = NULL;
	}

}
//-----------------------------------------------------------
// マウスの更新処理
//-----------------------------------------------------------
HRESULT UpdateMouse()
{
	HRESULT Result;
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = MouseState;

	// データ取得
	Result = Mouse->GetDeviceState(sizeof(MouseState), &MouseState);
	if (SUCCEEDED(Result))
	{
		MouseTrigger.lX = MouseState.lX;
		MouseTrigger.lY = MouseState.lY;
		MouseTrigger.lZ = MouseState.lZ;
		// マウスのボタン状態
		for (int i = 0; i < 8; i++)
		{
			MouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				MouseState.rgbButtons[i]) & MouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		Result = Mouse->Acquire();
	}
	return Result;

}
//----------------------------------------------
// マウスのボタン処理
//----------------------------------------------
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(MouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
}
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(MouseTrigger.rgbButtons[0] & 0x80);
}
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(MouseState.rgbButtons[1] & 0x80);
}
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(MouseTrigger.rgbButtons[1] & 0x80);
}
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(MouseState.rgbButtons[2] & 0x80);
}
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(MouseTrigger.rgbButtons[2] & 0x80);
}
//------------------
long GetMouseX(void)
{
	return MouseState.lX;
}
long GetMouseY(void)
{
	return MouseState.lY;
}
long GetMouseZ(void)
{
	return MouseState.lZ;
}
//================================================= game Pad
//---------------------------------------- コールバック関数
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT Result;

	Result = DInput->CreateDevice(lpddi->guidInstance, &GamePad[PadCount++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙

}
//---------------------------------------- 初期化
HRESULT InitializePad(void)			// パッド初期化
{
	HRESULT		Result;
	int			i;

	PadCount = 0;
	// ジョイパッドを探す
	DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for (i = 0; i < PadCount; i++)
	{
		// ジョイスティック用のデータ・フォーマットを設定
		Result = GamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(Result))
			return false; // データフォーマットの設定に失敗

							// モードを設定（フォアグラウンド＆非排他モード）
							//	Result = GamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
								//if ( FAILED(Result) )
									//return false; // モードの設定に失敗

						  // 軸の値の範囲を設定
						  // X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
						  // (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
						  // 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				Diprg;
		ZeroMemory(&Diprg, sizeof(Diprg));
		Diprg.diph.dwSize = sizeof(Diprg);
		Diprg.diph.dwHeaderSize = sizeof(Diprg.diph);
		Diprg.diph.dwHow = DIPH_BYOFFSET;
		Diprg.lMin = RANGE_MIN;
		Diprg.lMax = RANGE_MAX;

		// X軸の範囲を設定
		Diprg.diph.dwObj = DIJOFS_X;
		GamePad[i]->SetProperty(DIPROP_RANGE, &Diprg.diph);

		// Y軸の範囲を設定
		Diprg.diph.dwObj = DIJOFS_Y;
		GamePad[i]->SetProperty(DIPROP_RANGE, &Diprg.diph);

		// RX軸の範囲を設定
		Diprg.diph.dwObj = DIJOFS_Z;
		GamePad[i]->SetProperty(DIPROP_RANGE, &Diprg.diph);

		// RY軸の範囲を設定
		Diprg.diph.dwObj = DIJOFS_RZ;
		GamePad[i]->SetProperty(DIPROP_RANGE, &Diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				Dipdw;
		Dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		Dipdw.diph.dwHeaderSize = sizeof(Dipdw.diph);
		Dipdw.diph.dwHow = DIPH_BYOFFSET;
		Dipdw.dwData = DEADZONE;

		//X軸の無効ゾーンを設定
		Dipdw.diph.dwObj = DIJOFS_X;
		GamePad[i]->SetProperty(DIPROP_DEADZONE, &Dipdw.diph);

		//Y軸の無効ゾーンを設定
		Dipdw.diph.dwObj = DIJOFS_Y;
		GamePad[i]->SetProperty(DIPROP_DEADZONE, &Dipdw.diph);

		//RX軸の無効ゾーンを設定
		Dipdw.diph.dwObj = DIJOFS_Z;
		GamePad[i]->SetProperty(DIPROP_DEADZONE, &Dipdw.diph);

		//RY軸の無効ゾーンを設定
		Dipdw.diph.dwObj = DIJOFS_RZ;
		GamePad[i]->SetProperty(DIPROP_DEADZONE, &Dipdw.diph);

		//ジョイスティック入力制御開始
		GamePad[i]->Acquire();
		PadDiction[i][0] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		PadDiction[i][1] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}

	return true;

}
//-------------------------------------------
// ゲームパッドの終了処理
//------------------------------------------- 
void UninitPad(void)
{
	for (int i = 0; i < GAMEPADMAX; i++) 
	{
		if (GamePad[i])
		{
			GamePad[i]->Unacquire();
			GamePad[i]->Release();
		}
	}

}
//------------------------------------------
// ゲームパッドの更新処理
//------------------------------------------ 
void UpdatePad(void)
{
	HRESULT			Result;
	DIJOYSTATE2		Dijs;
	int				i;

	for (i = 0; i < PadCount; i++)
	{
		DWORD LastPadState;
		LastPadState = PadState[i];
		PadState[i] = 0x00000000l;	// 初期化

		Result = GamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(Result)) {
			Result = GamePad[i]->Acquire();
			while (Result == DIERR_INPUTLOST)
				Result = GamePad[i]->Acquire();
		}

		Result = GamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &Dijs);	// デバイス状態を読み取る
		if (Result == DIERR_INPUTLOST || Result == DIERR_NOTACQUIRED) {
			Result = GamePad[i]->Acquire();
			while (Result == DIERR_INPUTLOST)
				Result = GamePad[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if (Dijs.lY < 0)					PadState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (Dijs.lY > 0)					PadState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (Dijs.lX < 0)					PadState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (Dijs.lX > 0)					PadState[i] |= BUTTON_RIGHT;
		//* Ａボタン
		if (Dijs.rgbButtons[0] & 0x80)		PadState[i] |= BUTTON_R;
		//* Ｂボタン
		if (Dijs.rgbButtons[1] & 0x80)		PadState[i] |= BUTTON_RD;
		//* Ｃボタン
		if (Dijs.rgbButtons[2] & 0x80)		PadState[i] |= BUTTON_RR;
		//* Ｘボタン
		if (Dijs.rgbButtons[3] & 0x80)		PadState[i] |= BUTTON_RU;
		//* Ｙボタン
		if (Dijs.rgbButtons[4] & 0x80)		PadState[i] |= BUTTON_L1;
		//* Ｚボタン
		if (Dijs.rgbButtons[5] & 0x80)		PadState[i] |= BUTTON_R1;
		//* Ｌボタン
		if (Dijs.rgbButtons[6] & 0x80)		PadState[i] |= BUTTON_L2;
		//* Ｒボタン
		if (Dijs.rgbButtons[7] & 0x80)		PadState[i] |= BUTTON_R2;
		//* ＳＴＡＲＴボタン
		if (Dijs.rgbButtons[8] & 0x80)		PadState[i] |= BUTTON_SHARE;
		//* Ｍボタン
		if (Dijs.rgbButtons[9] & 0x80)		PadState[i] |= BUTTON_OPTIONS;
		//* Ｍボタン
		if (Dijs.rgbButtons[10] & 0x80)		PadState[i] |= BUTTON_L3;
		//* Ｍボタン
		if (Dijs.rgbButtons[11] & 0x80)		PadState[i] |= BUTTON_R3;
		//* touchbutton
		if (Dijs.rgbButtons[13] & 0x80)		PadState[i] |= BUTTON_TOUCH;
		//* 
		if (Dijs.rgdwPOV[0] == 0)			PadState[i] |= BUTTON_LU;
		//* 
		if ((int)Dijs.rgdwPOV[0] == 9000)		PadState[i] |= BUTTON_LR;
		//* 
		if ((int)Dijs.rgdwPOV[0] == 18000)		PadState[i] |= BUTTON_LD;
		//* 
		if ((int)Dijs.rgdwPOV[0] == 27000)		PadState[i] |= BUTTON_LL;






		// Trigger設定
		PadTrigger[i] = ((LastPadState ^ PadState[i])	// 前回と違っていて
			& PadState[i]);					// しかも今ONのやつ
		PadRelease[i] = ((LastPadState ^ PadState[i])
			&~PadState[i]);

		if (Dijs.lY || Dijs.lX)
		{
			PadDiction[i][0] = D3DXVECTOR3((float)Dijs.lX, (float)Dijs.lY, 1.0f);
		}
		else
		{
			PadDiction[i][0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (Dijs.lZ || Dijs.lRz)
		{
			PadDiction[i][1] = D3DXVECTOR3((float)Dijs.lZ, (float)Dijs.lRz, 1.0f);
		}
		else
		{
			PadDiction[i][1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

}
//-----------------------------------------------
//	ボタンの検査
//----------------------------------------------- 
BOOL IsButtonPressed(int PadNo, DWORD button)
{
	return (button & PadState[PadNo]);
}

BOOL IsButtonTriggered(int PadNo, DWORD button)
{
	return (button & PadTrigger[PadNo]);
}

BOOL IsButtonReleased(int PadNo, DWORD button)
{
	return (button & PadRelease[PadNo]);
}


D3DXVECTOR3 GetButtonDictionL(int PadNo)
{
	return PadDiction[PadNo][0];
}

D3DXVECTOR3 GetButtonDictionR(int PadNo)
{
	return PadDiction[PadNo][1];
}



