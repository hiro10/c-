//=============================================================================
//
// ���͏��� [input.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "main.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // �}�E�X�̏�����
void UninitMouse();						// �}�E�X�̏I������
HRESULT UpdateMouse();					// �}�E�X�̍X�V����

HRESULT InitializePad(void);			// �p�b�h������
										//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// �p�b�h�����R�[���o�b�N
void UpdatePad(void);
void UninitPad(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//------------------------------- keyboard
LPDIRECTINPUT8			DInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	DIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					KeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					KeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					KeyStateRepeat[NUM_KEY_MAX];	// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					KeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						KeyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

//--------------------------------- Mouse
static LPDIRECTINPUTDEVICE8 Mouse = NULL;	// Mouse

static DIMOUSESTATE2   MouseState;			// �}�E�X�̃_�C���N�g�ȏ��
static DIMOUSESTATE2   MouseTrigger;		// �����ꂽ�u�Ԃ���ON

//--------------------------------- game Pad

static LPDIRECTINPUTDEVICE8	GamePad[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// �p�b�h�f�o�C�X

static DWORD	PadState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
static DWORD	PadTrigger[GAMEPADMAX];
static DWORD	PadRelease[GAMEPADMAX];
static D3DXVECTOR3	PadDiction[GAMEPADMAX][2]; // ���̕���
static int		PadCount = 0;			// ���o�����p�b�h�̐�


//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT Hr;

	if (!DInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		Hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&DInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �}�E�X�̏�����
	InitializeMouse(hInst, hWnd);

	// �p�b�h�̏�����
	InitializePad();

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMouse();

	// �p�b�h�̏I������
	UninitPad();

	if (DInput)
	{
		DInput->Release();
		DInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMouse();

	// �p�b�h�̍X�V
	UpdatePad();

}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT Hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	Hr = DInput->CreateDevice(GUID_SysKeyboard, &DIDevKeyboard, NULL);
	if (FAILED(Hr) || DIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return Hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	Hr = DIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(Hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return Hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	Hr = DIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(Hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return Hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	DIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
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
// �L�[�{�[�h�̍X�V
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT Hr;
	BYTE KeyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(KeyStateOld, KeyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
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
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		DIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (KeyState[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (KeyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (KeyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (KeyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// �}�E�X�֌W�̏���
//=============================================================================

// �}�E�X�̏�����
HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow)
{
	HRESULT Result;
	// �f�o�C�X�쐬
	Result = DInput->CreateDevice(GUID_SysMouse, &Mouse, NULL);
	if (FAILED(Result) || Mouse == NULL)
	{
		MessageBox(hWindow, "No Mouse", "Warning", MB_OK | MB_ICONWARNING);
		return Result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	Result = Mouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(Result))
	{
		MessageBox(hWindow, "Can't setup Mouse", "Warning", MB_OK | MB_ICONWARNING);
		return Result;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	Result = Mouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(Result))
	{
		MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return Result;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD Prop;

	Prop.diph.dwSize = sizeof(Prop);
	Prop.diph.dwHeaderSize = sizeof(Prop.diph);
	Prop.diph.dwObj = 0;
	Prop.diph.dwHow = DIPH_DEVICE;
	Prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	Result = Mouse->SetProperty(DIPROP_AXISMODE, &Prop.diph);
	if (FAILED(Result))
	{
		MessageBox(hWindow, "Mouse Property error", "Warning", MB_OK | MB_ICONWARNING);
		return Result;
	}

	// �A�N�Z�X���𓾂�
	Mouse->Acquire();
	return Result;
}
//---------------------------------------------------------
// �}�E�X�̏I������
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
// �}�E�X�̍X�V����
//-----------------------------------------------------------
HRESULT UpdateMouse()
{
	HRESULT Result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = MouseState;

	// �f�[�^�擾
	Result = Mouse->GetDeviceState(sizeof(MouseState), &MouseState);
	if (SUCCEEDED(Result))
	{
		MouseTrigger.lX = MouseState.lX;
		MouseTrigger.lY = MouseState.lY;
		MouseTrigger.lZ = MouseState.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i < 8; i++)
		{
			MouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				MouseState.rgbButtons[i]) & MouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		Result = Mouse->Acquire();
	}
	return Result;

}
//----------------------------------------------
// �}�E�X�̃{�^������
//----------------------------------------------
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(MouseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
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
//---------------------------------------- �R�[���o�b�N�֐�
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT Result;

	Result = DInput->CreateDevice(lpddi->guidInstance, &GamePad[PadCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}
//---------------------------------------- ������
HRESULT InitializePad(void)			// �p�b�h������
{
	HRESULT		Result;
	int			i;

	PadCount = 0;
	// �W���C�p�b�h��T��
	DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i < PadCount; i++)
	{
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		Result = GamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(Result))
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

							// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
							//	Result = GamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
								//if ( FAILED(Result) )
									//return false; // ���[�h�̐ݒ�Ɏ��s

						  // ���̒l�͈̔͂�ݒ�
						  // X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
						  // (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
						  // ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				Diprg;
		ZeroMemory(&Diprg, sizeof(Diprg));
		Diprg.diph.dwSize = sizeof(Diprg);
		Diprg.diph.dwHeaderSize = sizeof(Diprg.diph);
		Diprg.diph.dwHow = DIPH_BYOFFSET;
		Diprg.lMin = RANGE_MIN;
		Diprg.lMax = RANGE_MAX;

		// X���͈̔͂�ݒ�
		Diprg.diph.dwObj = DIJOFS_X;
		GamePad[i]->SetProperty(DIPROP_RANGE, &Diprg.diph);

		// Y���͈̔͂�ݒ�
		Diprg.diph.dwObj = DIJOFS_Y;
		GamePad[i]->SetProperty(DIPROP_RANGE, &Diprg.diph);

		// RX���͈̔͂�ݒ�
		Diprg.diph.dwObj = DIJOFS_Z;
		GamePad[i]->SetProperty(DIPROP_RANGE, &Diprg.diph);

		// RY���͈̔͂�ݒ�
		Diprg.diph.dwObj = DIJOFS_RZ;
		GamePad[i]->SetProperty(DIPROP_RANGE, &Diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				Dipdw;
		Dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		Dipdw.diph.dwHeaderSize = sizeof(Dipdw.diph);
		Dipdw.diph.dwHow = DIPH_BYOFFSET;
		Dipdw.dwData = DEADZONE;

		//X���̖����]�[����ݒ�
		Dipdw.diph.dwObj = DIJOFS_X;
		GamePad[i]->SetProperty(DIPROP_DEADZONE, &Dipdw.diph);

		//Y���̖����]�[����ݒ�
		Dipdw.diph.dwObj = DIJOFS_Y;
		GamePad[i]->SetProperty(DIPROP_DEADZONE, &Dipdw.diph);

		//RX���̖����]�[����ݒ�
		Dipdw.diph.dwObj = DIJOFS_Z;
		GamePad[i]->SetProperty(DIPROP_DEADZONE, &Dipdw.diph);

		//RY���̖����]�[����ݒ�
		Dipdw.diph.dwObj = DIJOFS_RZ;
		GamePad[i]->SetProperty(DIPROP_DEADZONE, &Dipdw.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		GamePad[i]->Acquire();
		PadDiction[i][0] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		PadDiction[i][1] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}

	return true;

}
//-------------------------------------------
// �Q�[���p�b�h�̏I������
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
// �Q�[���p�b�h�̍X�V����
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
		PadState[i] = 0x00000000l;	// ������

		Result = GamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(Result)) {
			Result = GamePad[i]->Acquire();
			while (Result == DIERR_INPUTLOST)
				Result = GamePad[i]->Acquire();
		}

		Result = GamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &Dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (Result == DIERR_INPUTLOST || Result == DIERR_NOTACQUIRED) {
			Result = GamePad[i]->Acquire();
			while (Result == DIERR_INPUTLOST)
				Result = GamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if (Dijs.lY < 0)					PadState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (Dijs.lY > 0)					PadState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (Dijs.lX < 0)					PadState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (Dijs.lX > 0)					PadState[i] |= BUTTON_RIGHT;
		//* �`�{�^��
		if (Dijs.rgbButtons[0] & 0x80)		PadState[i] |= BUTTON_R;
		//* �a�{�^��
		if (Dijs.rgbButtons[1] & 0x80)		PadState[i] |= BUTTON_RD;
		//* �b�{�^��
		if (Dijs.rgbButtons[2] & 0x80)		PadState[i] |= BUTTON_RR;
		//* �w�{�^��
		if (Dijs.rgbButtons[3] & 0x80)		PadState[i] |= BUTTON_RU;
		//* �x�{�^��
		if (Dijs.rgbButtons[4] & 0x80)		PadState[i] |= BUTTON_L1;
		//* �y�{�^��
		if (Dijs.rgbButtons[5] & 0x80)		PadState[i] |= BUTTON_R1;
		//* �k�{�^��
		if (Dijs.rgbButtons[6] & 0x80)		PadState[i] |= BUTTON_L2;
		//* �q�{�^��
		if (Dijs.rgbButtons[7] & 0x80)		PadState[i] |= BUTTON_R2;
		//* �r�s�`�q�s�{�^��
		if (Dijs.rgbButtons[8] & 0x80)		PadState[i] |= BUTTON_SHARE;
		//* �l�{�^��
		if (Dijs.rgbButtons[9] & 0x80)		PadState[i] |= BUTTON_OPTIONS;
		//* �l�{�^��
		if (Dijs.rgbButtons[10] & 0x80)		PadState[i] |= BUTTON_L3;
		//* �l�{�^��
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






		// Trigger�ݒ�
		PadTrigger[i] = ((LastPadState ^ PadState[i])	// �O��ƈ���Ă���
			& PadState[i]);					// ��������ON�̂��
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
//	�{�^���̌���
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



