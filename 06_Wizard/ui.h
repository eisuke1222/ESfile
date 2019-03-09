//=============================================================================
//
// UI処理 [ui.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene2d.h"	// シーン2D
#include "number.h"		// 番号

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CGauge;			// ゲージ

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CUi : public CScene
{// Ui（親：CScene）
public:
	typedef enum
	{// UI
		UI_TITLE = 0,	// タイトル
		UI_GAME,		// ゲーム
		UI_RESULT,		// リザルト
		UI_MAX			// 最大数
	}UI;

	CUi();										// コンストラクタ
	virtual ~CUi();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード

	static CUi *Create(UI ui);					// クリエイト

	virtual HRESULT Init(void) = 0;				// 初期化処理
	virtual void Uninit(void);					// 終了処理
	virtual void Update(void) = 0;				// 更新処理
	virtual void Draw(void) = 0;				// 描画処理

private:

protected:

};

class CUi_Title : public CUi
{// タイトルUi（親：CUi）
public:
	typedef enum
	{// 種類
		TYPE_TITLELOGO_BG = 0,	// タイトルロゴエフェクト用
		TYPE_TITLELOGO,			// タイトルロゴ
		TYPE_MENU_SELECT,		// メニュー（選択）
		TYPE_MENU_SELECT_MARK,	// メニュー（選択マーク）
		TYPE_MENU_GAME,			// メニュー（ゲーム）
		TYPE_MENU_TUTORIAL,		// メニュー（チュートリアル）
		TYPE_MENU_EDIT,			// メニュー（編集）
		TYPE_MENU_TAITLE,		// メニュー（タイトル）
		TYPE_MAX				// 最大数
	}TYPE;

	CUi_Title();								// コンストラクタ
	~CUi_Title();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	static bool GetUiEdit(void)					// Ui変更中か
		{ return m_bUiEdit; }

private:
	static bool			m_bUiEdit;				// Ui変更中か
	CScene2D			*m_pUi[TYPE_MAX];		// Ui
	CScene2D_Preset		*m_pPress;				// プレス
	int					m_nCntPress;			// プレスカウント

	int					m_nCntTitle;			// タイトルカウント

protected:

};

class CUi_Game : public CUi
{// ゲームUi（親：CUi）
public:
	typedef enum
	{// 種類
		TYPE_ENEMY_DATA = 0,	// 敵データ
		TYPE_PLAYER_DATA,		// プレイヤーデータ
		TYPE_MAP,				// マップ
		TYPE_LOG,				// ログ
		TYPE_WEAPON,			// 武器
		TYPE_WEAPON_BUTTON,		// 武器ボタン
		TYPE_PLAYER_ICON,		// プレイヤーアイコン
		TYPE_PLAYER_NAME,		// プレイヤー名
		TYPE_PLAYER_HP,			// プレイヤーHP
		TYPE_PLAYER_MP,			// プレイヤーMP
		TYPE_PLAYER_RANK,		// プレイヤーランク
		TYPE_ENEMY_ICON,		// 敵アイコン
		TYPE_ENEMY_NAME,		// 敵名
		TYPE_ENEMY_HP,			// 敵HP
		TYPE_MAX				// 最大数
	}TYPE;

	typedef enum
	{
		MESSAGE_00 = 0,
		MESSAGE_01,
		MESSAGE_02,
		MESSAGE_03,
		MESSAGE_MAX
	}MESSAGE;

	typedef enum
	{
		GAUGE_PLAYER_HP = 0,
		GAUGE_PLAYER_MP,
		GAUGE_ENEMY_HP,
		GAUGE_MAX
	}GAUGE;

	CUi_Game();									// コンストラクタ
	~CUi_Game();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	void SetGauge(GAUGE gauge, int nNow, int nMax);		// ゲージ設定

	static void SetEnemyName(int nNum)			// 敵名前番号
		{ m_nEnemyName = nNum; }

private:
	CScene2D			*m_pUi[TYPE_MAX];		// Ui
	CScene2D			*m_Message[MESSAGE_MAX];// メッセージ
	D3DXVECTOR2			m_TutorialUV;			// チュートリアルUV
	int					m_nCntTutorial;			// チュートリアルカウント

	CGauge				*m_pGauge[GAUGE_MAX];	// ゲージ

	static int			m_nEnemyName;			// 敵名前番号

protected:

};

class CUi_Result : public CUi
{// リザルトUi（親：CUi）
public:
	typedef enum
	{// 種類
		TYPE_LOGO_BG = 0,	// ロゴbg
		TYPE_LOGO,			// ロゴ
		TYPE_PLAYER_NAME,	// プレイヤー名 
		TYPE_PLAYER_RANK,	// プレイヤーランク
		TYPE_PLAY_SS00,		// スクリーンショット00 
		TYPE_PLAY_SS01,		// スクリーンショット01 
		//TYPE_PLAY_SS02,		// スクリーンショット02 
		TYPE_MAX			// 最大数
	}TYPE;

	typedef enum
	{
		SS_00 = 0,
		SS_01,
		//SS_02,
		SS_MAX
	}SS;

	CUi_Result();								// コンストラクタ
	~CUi_Result();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[SS_MAX];		// テクスチャ

	CScene2D			*m_pUi[TYPE_MAX];		// Ui

	int					m_nCntTimer;			// 時間カウント
	int					m_nCntLogo;				// ロゴカウント

	CNumber2D *m_pNumber[6];	// 番号

protected:

};

#endif