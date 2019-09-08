//=============================================================================
//
// 入力処理 [input.h]
// Author : 西村任弘
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// プログラム分けするときに使う
#define	USE_KEYBOARD										// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE											// 宣言するとマウスで操作可能になる
#define	USE_PAD												// 宣言するとパッドで操作可能になる


/* game pad情報 */
#define BUTTON_UP			0x00000001l						// 方向キー上(.IY<0)
#define BUTTON_DOWN			0x00000002l						// 方向キー下(.IY>0)
#define BUTTON_LEFT			0x00000004l						// 方向キー左(.IX<0)
#define BUTTON_RIGHT		0x00000008l						// 方向キー右(.IX>0)
#define BUTTON_R			0x00000010l						// 〇ボタン(.rgbButtons[0]&0x80)
#define BUTTON_RD			0x00000020l						// ×ボタン(.rgbButtons[1]&0x80)
#define BUTTON_RR			0x00000040l						// Ｃボタン(.rgbButtons[2]&0x80)
#define BUTTON_RU			0x00000080l						// Ｘボタン(.rgbButtons[3]&0x80)
#define BUTTON_L1			0x00000100l						// Ｙボタン(.rgbButtons[4]&0x80)
#define BUTTON_R1			0x00000200l						// Ｚボタン(.rgbButtons[5]&0x80)
#define BUTTON_L2			0x00000400l						// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R2			0x00000800l						// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_SHARE		0x00001000l						// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
#define BUTTON_OPTIONS		0x00002000l						// Ｍボタン(.rgbButtons[9]&0x80)
#define BUTTON_L3			0x00004000l						// Ｍボタン(.rgbButtons[10]&0x80)
#define BUTTON_R3			0x00008000l						// Ｍボタン(.rgbButtons[11]&0x80)
#define BUTTON_TOUCH		0x00010000l						// touchボタン(.rgbButtons[12]&0x80)
#define BUTTON_LL			0x00020000l						// touchボタン(.rgbButtons[12]&0x80)
#define BUTTON_LD			0x00040000l						// touchボタン(.rgbButtons[12]&0x80)
#define BUTTON_LR			0x00080000l						// touchボタン(.rgbButtons[12]&0x80)
#define BUTTON_LU			0x00100000l						// touchボタン(.rgbButtons[12]&0x80)
#define GAMEPADMAX			4								// 同時に接続するジョイパッドの最大数をセット

#define DEADZONE			2500							// 各軸の25%を無効ゾーンとする
#define RANGE_MAX			1000							// 有効範囲の最大値
#define RANGE_MIN			-1000							// 有効範囲の最小値

#define OPERATION_MENUE_UP				(GetKeyboardTrigger(DIK_LEFT)||GetKeyboardTrigger(DIK_A)||GetKeyboardTrigger(DIK_W)||IsButtonTriggered(0,BUTTON_LL)||IsButtonTriggered(0,BUTTON_LU)||IsButtonTriggered(0,BUTTON_UP)||IsButtonTriggered(0,BUTTON_LEFT))
#define OPERATION_MENUE_DOWN			(GetKeyboardTrigger(DIK_RIGHT)||GetKeyboardTrigger(DIK_D)||GetKeyboardTrigger(DIK_S)||IsButtonTriggered(0,BUTTON_LR)||IsButtonTriggered(0,BUTTON_LD)||IsButtonTriggered(0,BUTTON_DOWN)||IsButtonTriggered(0,BUTTON_RIGHT))
#define OPERATION_FIND_NEXT_ENEMY		(GetKeyboardPress(DIK_E)||IsButtonPressed(0,BUTTON_R3))
#define OPERATION_CHANGE_VIEW			(GetKeyboardTrigger(DIK_TAB)||IsButtonTriggered(0,BUTTON_L3))
#define OPERATION_JUMP					(GetKeyboardTrigger(DIK_SPACE)||IsButtonTriggered(0,BUTTON_RD))
#define OPERATION_SELECT				(GetKeyboardTrigger(DIK_RETURN)||IsButtonTriggered(0,BUTTON_RR))
#define OPERATION_PAUSE					(GetKeyboardTrigger(DIK_BACK)||IsButtonTriggered(0,BUTTON_OPTIONS))
#define OPERATION_SHOOT					(IsMouseLeftPressed()||IsButtonPressed(0,BUTTON_R2))
#define OPERATION_MISSILE				(IsMouseCenterPressed()||IsButtonPressed(0,BUTTON_L2))
#define OPERATION_SPEED_UP				(GetKeyboardPress(DIK_LSHIFT)||IsButtonPressed(0,BUTTON_R1))
#define OPERATION_SNIPER_MODE			(IsMouseRightPressed()||IsButtonPressed(0,BUTTON_L1))
#define OPERATION_SNIPER_MODE			(IsMouseRightPressed()||IsButtonPressed(0,BUTTON_L1))

#define OPERATION_DICTION				(GetKeyboardTrigger(DIK_UP)||GetKeyboardTrigger(DIK_DOWN)||GetKeyboardTrigger(DIK_RIGHT)||GetKeyboardTrigger(DIK_LEFT))
#define OPERATION_MOVE					(GetKeyboardPress(DIK_W)||GetKeyboardPress(DIK_A)||GetKeyboardPress(DIK_S)||GetKeyboardPress(DIK_D))
#define OPERATION_MOVE_FRONT			(GetKeyboardPress(DIK_W))
#define OPERATION_MOVE_LEFT				(GetKeyboardPress(DIK_A))
#define OPERATION_MOVE_RIGHT			(GetKeyboardPress(DIK_D))
#define OPERATION_MOVE_BACK				(GetKeyboardPress(DIK_S))


#define CHEAT_KILL_ALL					(GetKeyboardTrigger(DIK_F4)||IsButtonTriggered(0,BUTTON_TOUCH))
#define CHEAT_ENEMY1_ADD				(GetKeyboardTrigger(DIK_F1)||(IsButtonPressed(0,BUTTON_RU)&&IsButtonTriggered(0,BUTTON_LL)))
#define CHEAT_ENEMY2_ADD				(GetKeyboardTrigger(DIK_F2)||(IsButtonPressed(0,BUTTON_RU)&&IsButtonTriggered(0,BUTTON_LU)))
#define CHEAT_ENEMY3_ADD				(GetKeyboardTrigger(DIK_F3)||(IsButtonPressed(0,BUTTON_RU)&&IsButtonTriggered(0,BUTTON_LR)))


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);								// 左クリックした状態
BOOL IsMouseLeftTriggered(void);							// 左クリックした瞬間
BOOL IsMouseRightPressed(void);								// 右クリックした状態
BOOL IsMouseRightTriggered(void);							// 右クリックした瞬間
BOOL IsMouseCenterPressed(void);							// 中クリックした状態
BOOL IsMouseCenterTriggered(void);							// 中クリックした瞬間
long GetMouseX(void);										// マウスがX方向に動いた相対値
long GetMouseY(void);										// マウスがY方向に動いた相対値
long GetMouseZ(void);										// マウスホイールが動いた相対値

//---------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);
D3DXVECTOR3 GetButtonDictionL(int padNo);
D3DXVECTOR3 GetButtonDictionR(int padNo);
BOOL IsButtonReleased(int padNo, DWORD button);


#endif