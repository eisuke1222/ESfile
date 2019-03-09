//=============================================================================
//
// 入力処理 [input.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INPUT_DEADZONE		( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CInput
{// 入力
public:
	CInput();										// コンストラクタ
	virtual ~CInput();								// デストラクタ

													// 初期化処理
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	
	virtual void Uninit(void);						// 終了処理
	virtual void Update(void) = 0;					// 更新処理

protected:
	static LPDIRECTINPUT8 m_pInput;					// 入力オブジェクト
	LPDIRECTINPUTDEVICE8 m_pDevice;					// 入力デバイス

};

class CInputKeyboard : public CInput
{// キーボード（親：CInput）
public:
	const static int m_KEYMAX	= 256;				// キーの最大数

	CInputKeyboard();								// コンストラクタ
	virtual ~CInputKeyboard();						// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理

	bool GetALL(int nType);							// オール
	bool GetPress(int nKey);						// プレス
	bool GetTrigger(int nKey);						// トリガー
	bool GetRelease(int nKey);						// リリース

private:
	BYTE m_aKeyStatePress[m_KEYMAX];				// プレス	
	BYTE m_aKeyStateTrigger[m_KEYMAX];				// トリガー
	BYTE m_aKeyStateRelease[m_KEYMAX];				// リリース
};

class CInputMouse : public CInput
{// マウス（親：CInput）
public:
	const static int m_MOUSEMAX = 8;				// マウス最大数

	CInputMouse();									// コンストラクタ
	virtual ~CInputMouse();							// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理

	bool GetALL(int nType);							// オール
	bool GetPress(int nKey);						// プレス
	bool GetTrigger(int nKey);						// トリガー
	bool GetRelease(int nKey);						// リリース

	D3DXVECTOR2 GetPointer(void);					// ポインタ位置取得
	int GetWheel(void);								// ホイール番号取得

private:
	BYTE m_aMouseStatePress[m_MOUSEMAX];			// プレス	
	BYTE m_aMouseStateTrigger[m_MOUSEMAX];			// トリガー
	BYTE m_aMouseStateRelease[m_MOUSEMAX];			// リリース

	D3DXVECTOR2 m_posPointer;						// ポインタ位置
	int			m_nWheel;							// ホイール番号
};

class CInputDIPad : public CInput
{// DIパッド（親：CInput）
public:
	const static int m_BUTTONMAX	= 32;			// ボタンの最大数

	typedef enum
	{// ボタン以外のタイプ
		DIPADOTHER_AXES_UP = m_BUTTONMAX,		// 十字キー上
		DIPADOTHER_AXES_LEFT,					// 十字キー左
		DIPADOTHER_AXES_RIGHT,					// 十字キー右
		DIPADOTHER_AXES_DOWN,					// 十字キー下
		DIPADOTHER_STICK_L_UP,					// Lスティック上
		DIPADOTHER_STICK_L_LEFT,				// Lスティック左
		DIPADOTHER_STICK_L_RIGHT,				// Lスティック右
		DIPADOTHER_STICK_L_DOWN,				// Lスティック下
		DIPADOTHER_STICK_R_UP,					// Rスティック上
		DIPADOTHER_STICK_R_LEFT,				// Rスティック左
		DIPADOTHER_STICK_R_RIGHT,				// Rスティック右
		DIPADOTHER_STICK_R_DOWN,				// Rスティック下
		DIPADOTHER_MAX
	}DIPADOTHER;

	CInputDIPad();									// コンストラクタ
	virtual ~CInputDIPad();							// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理

	bool GetPress(int nButton);						// プレス
	bool GetTrigger(int nButton);					// トリガー
	bool GetRelease(int nButton);					// リリース
	bool GetStick(int nLR);							// スティック
	float GetStickRot(int nLR);						// スティック向き

private:
	BYTE m_aJoyStatePress[DIPADOTHER_MAX];			// プレス	
	BYTE m_aJoyStateTrigger[DIPADOTHER_MAX];		// トリガー
	BYTE m_aJoyStateRelease[DIPADOTHER_MAX];		// リリース

	float m_LStickRot;								// Lスティック向き
	float m_RStickRot;								// Rスティック向き
};

class CInputXPad
{// Xパッド
public:
	const static int m_CONTROLLER	= 4;			// コントローラの最大数
	const static int m_STICKMAX		= 32767;		// コントローラの最大数
	const static int m_STICKMIN		= -32768;		// コントローラの最大数

	typedef enum
	{// ボタン以外のタイプ
		XPADOTHER_BUTTON = 0,					// ボタン
		XPADOTHER_TRIGGER_LEFT,					// Lトリガー
		XPADOTHER_TRIGGER_RIGHT,				// Rトリガー
		XPADOTHER_STICK_L_UP,					// Lスティック上
		XPADOTHER_STICK_L_LEFT,					// Lスティック左
		XPADOTHER_STICK_L_RIGHT,				// Lスティック右
		XPADOTHER_STICK_L_DOWN,					// Lスティック下
		XPADOTHER_STICK_R_UP,					// Rスティック上
		XPADOTHER_STICK_R_LEFT,					// Rスティック左
		XPADOTHER_STICK_R_RIGHT,				// Rスティック右
		XPADOTHER_STICK_R_DOWN,					// Rスティック下
		XPADOTHER_MAX
	}XPADOTHER;

	typedef struct
	{// XINPUTコントローラー
		XINPUT_STATE state;		// 情報
		bool bConnected;		// 入力有無
	}CONTROLER_STATE;

	CInputXPad();										// コンストラクタ
	virtual ~CInputXPad();								// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理

	static int GetInputNum(void);						// 入力数取得

	bool GetALL(int nType, int nIdxPad);				// オール
	bool GetPress(int nButton, int nIdxPad);			// プレス
	bool GetPress(XPADOTHER nButton, int nIdxPad);		// プレス（その他）
	bool GetTrigger(int nButton, int nIdxPad);			// トリガー
	bool GetTrigger(XPADOTHER nButton, int nIdxPad);	// トリガー（その他）
	bool GetRelease(int nButton, int nIdxPad);			// リリース
	bool GetRelease(XPADOTHER nButton, int nIdxPad);	// リリース（その他）

	bool GetStick(int nLR, int nIdxPad);				// スティック
	D3DXVECTOR2 GetStickNum(int nLR, int nIdxPad);		// スティック数値
	float GetStickRot(int nLR, int nIdxPad);			// スティック向き
	D3DXVECTOR2 GetStickMove(int nLR, int nIdxPad);		// スティック移動量

private:
	HRESULT UpdateControllerState(void);				// コントローラ入力数更新
	static int		m_nInputNum;						// コントローラ入力数

	CONTROLER_STATE m_Controllers[m_CONTROLLER];		// コントローラ
	bool			m_bDeadZoneOn;						// スティック

	WORD m_aJoyStatePress[m_CONTROLLER][XPADOTHER_MAX];		// プレス	
	WORD m_aJoyStateTrigger[m_CONTROLLER][XPADOTHER_MAX];	// トリガー
	WORD m_aJoyStateRelease[m_CONTROLLER][XPADOTHER_MAX];	// リリース

	float m_LStickRot[m_CONTROLLER];					// Lスティック向き
	float m_RStickRot[m_CONTROLLER];					// Rスティック向き
};

#endif