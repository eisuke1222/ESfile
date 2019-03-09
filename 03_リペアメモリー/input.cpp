//=========================================================================================================================
// 
// 入力処理 [input.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define INITGUID
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "input.h"
#include "dinputex.h"

#include "camera.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	NUM_KEY_MAX			(256)									// キーの最大数
#define	NUM_MOUSE_MAX		(8)										// マウスの最大数
#define	NUM_BUTTON_MAX		(32)									// ボタンの最大数

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECTINPUT8				g_pInput = NULL;					// DirerctInputオブジェクトへのポインタ

LPDIRECTINPUTDEVICE8		g_pDevKeyboard = NULL;					// 入力デバイス（キーボード）へのポインタ
BYTE						g_aKeyStatePress[NUM_KEY_MAX];			// キーボードの入力情報（プレス情報）
BYTE						g_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードの入力情報（トリガー情報）
BYTE						g_aKeyStateRelease[NUM_KEY_MAX];		// キーボードの入力情報（リリース情報）

LPDIRECTINPUTDEVICE8		g_pDevMouse = NULL;						// 入力デバイス（マウス）へのポインタ
POINT					    Pos, WPos;
DIMOUSESTATE2				g_MouseState;
DIMOUSESTATE2				g_aMouseStatePress;						// マウスの入力情報（プレス情報）
DIMOUSESTATE2				g_aMouseStateTrigger;					// マウスの入力情報（トリガー情報）
DIMOUSESTATE2				g_aMouseStateRelease;					// マウスの入力情報（リリース情報


LPDIRECTINPUTDEVICE8		g_pDevJoystick = NULL;					// 入力デバイス（ジョイスティック）へのポインタ
DIDEVCAPS			        g_pDevCaps;								// 入力デバイス（ジョイスティック）へのポインタ
DIJOYSTATE					g_aButtonStatePress;					// ボタンの入力情報（プレス情報）
DIJOYSTATE					g_aButtonStateTrigger;					// ボタンの入力情報（トリガー情報）
DIJOYSTATE					g_aButtonStateRelease;					// ボタンの入力情報（リリース情報）
DIJOYSTATE					g_aStickState = {};						// ジョイコンの入力情報（スティック）
bool						g_bUseJoycon = false;

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*, VOID*);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE*, VOID*);

//=========================================================================================================================
// キーボードの初期化
//=========================================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=========================================================================================================================
// キーボードの更新処理
//=========================================================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateRelease[nCntKey] = (g_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyStatePress[nCntKey];
			g_aKeyStateTrigger[nCntKey] = (g_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyStatePress[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();		// キーボードへのアクセス権を獲得
	}
}

//=========================================================================================================================
// キーボードの入力情報(プレス情報)を取得
//=========================================================================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// キーボードの入力情報(トリガー情報)を取得
//=========================================================================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// キーボードの入力情報(リリース情報)を取得
//=========================================================================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// マウスの初期化
//=========================================================================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	// 軸モードを設定（相対値モードに設定）
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;
	//diprop.dwData = DIPROPAXISMODE_ABS; // 絶対値モードの場合
	if (FAILED(g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得(入力制御開始)
	g_pDevMouse->Acquire();

	return S_OK;
}

//=========================================================================================================================
// マウスの更新処理
//=========================================================================================================================
void UpdateMouse(void)
{
	int nCntMouse;

	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(g_MouseState), &g_MouseState)))
	{
		for (nCntMouse = 0; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			g_aMouseStateRelease.rgbButtons[nCntMouse] = (g_aMouseStatePress.rgbButtons[nCntMouse] ^ g_MouseState.rgbButtons[nCntMouse]) & g_aMouseStatePress.rgbButtons[nCntMouse];
			g_aMouseStateTrigger.rgbButtons[nCntMouse] = (g_aMouseStatePress.rgbButtons[nCntMouse] ^ g_MouseState.rgbButtons[nCntMouse]) & g_MouseState.rgbButtons[nCntMouse];
			g_aMouseStatePress.rgbButtons[nCntMouse] = g_MouseState.rgbButtons[nCntMouse];
		}
	}
	else
	{
		g_pDevMouse->Acquire();
	}
}

//=========================================================================================================================
// マウスの入力情報(プレス情報)を取得
//=========================================================================================================================
bool GetMousePress(int nKey)
{
	return (g_aMouseStatePress.rgbButtons[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// マウスの入力情報(トリガー情報)を取得
//=========================================================================================================================
bool GetMouseTrigger(int nKey)
{
	return (g_aMouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// マウスの入力情報(リリース情報)を取得
//=========================================================================================================================
bool GetMouseRelease(int nKey)
{
	return (g_aMouseStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// マウスの入力情報(ポインター)を取得
//=========================================================================================================================
DIMOUSESTATE2 GetMousePointer(void)
{
	return g_MouseState;
}

//=========================================================================================================================
// ジョイスティックの初期化
//=========================================================================================================================
HRESULT InitJoystick(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 
	if (FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	if (g_pDevJoystick != NULL)
	{
		// データフォーマットを設定
		if (FAILED(g_pDevJoystick->SetDataFormat(&c_dfDIJoystick)))
		{
			return E_FAIL;
		}

		// 協調モードを設定
		if (FAILED(g_pDevJoystick->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return E_FAIL;
		}

		// 
		g_pDevCaps.dwSize = sizeof(DIDEVCAPS);

		// デバイスの能力を取得
		if (FAILED(g_pDevJoystick->GetCapabilities(&g_pDevCaps)))
		{
			return E_FAIL;
		}

		// 十字キーの範囲等を指定
		if (FAILED(g_pDevJoystick->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS)))
		{
			return E_FAIL;
		}

		// デバイスからデータを取得
		g_pDevJoystick->Acquire();
	}
	else
	{
		g_aStickState.rgdwPOV[0] = 9999;
		g_bUseJoycon = true;
	}

	return S_OK;
}

//=========================================================================================================================
// ジョイスティックの更新処理
//=========================================================================================================================
void UpdateJoystick(void)
{
	if (g_bUseJoycon != true)
	{
		DIJOYSTATE aButtonState;
		int nCntButton;

		g_pDevJoystick->Poll();

		if (SUCCEEDED(g_pDevJoystick->GetDeviceState(sizeof(aButtonState), &aButtonState)))
		{
			for (nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
			{
				g_aButtonStateRelease.rgbButtons[nCntButton] = (g_aButtonStatePress.rgbButtons[nCntButton] ^ aButtonState.rgbButtons[nCntButton]) & g_aButtonStatePress.rgbButtons[nCntButton];
				g_aButtonStateTrigger.rgbButtons[nCntButton] = (g_aButtonStatePress.rgbButtons[nCntButton] ^ aButtonState.rgbButtons[nCntButton]) & aButtonState.rgbButtons[nCntButton];
				g_aButtonStatePress.rgbButtons[nCntButton] = aButtonState.rgbButtons[nCntButton];
			}

			g_aButtonStateRelease.rgdwPOV[0] = (g_aButtonStatePress.rgdwPOV[0] ^ aButtonState.rgdwPOV[0]) & g_aButtonStatePress.rgdwPOV[0];
			g_aButtonStateTrigger.rgdwPOV[0] = (g_aButtonStatePress.rgdwPOV[0] ^ aButtonState.rgdwPOV[0]) & aButtonState.rgdwPOV[0];
			g_aButtonStatePress.rgdwPOV[0] = aButtonState.rgdwPOV[0];
		}
		else
		{
			g_pDevJoystick->Acquire();
		}
	}
}

//------------------------------------------------------------------------------
//    Joysticks Callback.
//------------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	if (FAILED(g_pInput->CreateDevice(pdidInstance->guidInstance, &g_pDevJoystick, NULL))) return DIENUM_CONTINUE;

	return DIENUM_STOP;
}

//------------------------------------------------------------------------------
//    Axes Callback.
//------------------------------------------------------------------------------
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;		// ジョイスティックの十字キーの最小値（上、左）
	diprg.lMax = 0 + 1000;		// ジョイスティックの十字キーの最大値（下、右）

	if (FAILED(g_pDevJoystick->SetProperty(DIPROP_RANGE, &diprg.diph))) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

//=========================================================================================================================
// ボタンの入力情報(プレス情報)を取得
//=========================================================================================================================
bool GetButtonPress(int nKey)
{
	if (g_bUseJoycon != true)
	{
		return (g_aButtonStatePress.rgbButtons[nKey] & 0x80) ? true : false;
	}
	else
	{
		return 0;
	}
}

//=========================================================================================================================
// ボタンの入力情報(トリガー情報)を取得
//=========================================================================================================================
bool GetButtonTrigger(int nKey)
{
	if (g_bUseJoycon != true)
	{
		return (g_aButtonStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
	}
	else
	{
		return 0;
	}
}

//=========================================================================================================================
// ボタンの入力情報(リリース情報)を取得
//=========================================================================================================================
bool GetButtonRelease(int nKey)
{
	if (g_bUseJoycon != true)
	{
		return (g_aButtonStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
	}
	else
	{
		return 0;
	}
}

//=========================================================================================================================
// 十字キーの入力情報(プレス情報)を取得
//=========================================================================================================================
bool GetCrossPress(int nKey)
{
	if (g_bUseJoycon != true)
	{
		return (g_aButtonStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
	}
	else
	{
		return 0;
	}
}

//=========================================================================================================================
// 十字キーの入力情報(トリガー情報)を取得
//=========================================================================================================================
bool GetCrossTrigger(int nKey)
{
	if (g_bUseJoycon != true)
	{
		return (g_aButtonStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
	}
	else
	{
		return 0;
	}
}

//=========================================================================================================================
// 十字キーの入力情報(リリース情報)を取得
//=========================================================================================================================
bool GetCrossRelease(int nKey)
{
	if (g_bUseJoycon != true)
	{
		return (g_aButtonStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
	}
	else
	{
		return 0;
	}
}

//=========================================================================================================================
// ジョイコンの入力情報(アナログスティック)を取得
//=========================================================================================================================
DIJOYSTATE GetStick(void)
{
	if (g_bUseJoycon != true)
	{
		g_pDevJoystick->GetDeviceState(sizeof(DIJOYSTATE), &g_aStickState);

		return g_aStickState;
	}
	else
	{
		return g_aStickState;
	}
}

//=========================================================================================================================
// 入力の終了処理
//=========================================================================================================================
void UninitInput(void)
{
	// 入力デバイス(キーボード)の開放
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();	// キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力デバイス(マウス)の開放
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();	// マウスへのアクセス権を放棄
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	// 入力デバイス(ジョイスティック)の開放
	if (g_pDevJoystick != NULL)
	{
		g_pDevJoystick->Unacquire();	// ジョイスティックへのアクセス権を放棄
		g_pDevJoystick->Release();
		g_pDevJoystick = NULL;
	}

	// DirectInputオブジェクトの開放
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}