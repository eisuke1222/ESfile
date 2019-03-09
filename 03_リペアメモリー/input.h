//=========================================================================================================================
// 
// 入力処理 [input.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);			// キーボード初期化処理
void UpdateKeyboard(void);										// キーボード更新処理

bool GetKeyboardPress(int Key);									// キーボードプレス
bool GetKeyboardTrigger(int Key);								// キーボードトリガー
bool GetKeyboardRelease(int Key);								// キーボードリリース

HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);				// マウス初期化処理
void UpdateMouse(void);											// マウス更新処理

bool GetMousePress(int Key);									// マウスプレス
bool GetMouseTrigger(int Key);									// マウストリガー
bool GetMouseRelease(int Key);									// マウスリリース
DIMOUSESTATE2 GetMousePointer(void);							// マウスポインター

HRESULT InitJoystick(HINSTANCE hInstance, HWND hWnd);			// ジョイスティック初期化処理
void UpdateJoystick(void);										// ジョイスティック更新処理

bool GetButtonPress(int Key);									// ボタンプレス
bool GetButtonTrigger(int Key);									// ボタントリガー
bool GetButtonRelease(int Key);									// ボタンリリース
bool GetCrossPress(int Key);									// 十字キープレス
bool GetCrossTrigger(int Key);									// 十字キートリガー
bool GetCrossRelease(int Key);									// 十字キーリリース
DIJOYSTATE GetStick(void);										// スティック

void UninitInput(void);											// 入力終了処理

#endif