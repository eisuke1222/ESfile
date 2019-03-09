//*****************************************************************************
//
//				リザルトの処理[result.h]
//				Auther : Shun Yokomichi
//
//*****************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "game.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CPress;

//*****************************************************************************
//		マクロ定義
//*****************************************************************************
#define MAX_FALLINGMODE_TEXTURE			(7)			// 落下EDの最大テクスチャ数
#define MAX_FALLINGMODE_POLYGON			(9)			// 落下EDの最大シーン数
#define MAX_PREYMODE_TEXTURE			(9)			// 捕食EDの最大テクスチャ数
#define MAX_PREYMODE_POLYGON			(10)		// 捕食EDの最大シーン数
#define MAX_ESCAPEMODE_TEXTURE			(6)			// 捕食EDの最大テクスチャ数
#define MAX_ESCAPEMODE_POLYGON			(6)			// 捕食EDの最大シーン数
#define MAX_FADERESTRICTION				(60*10)		// リザルトからランキングへ遷移する時間
#define MAX_PRESS_BUTTON				(2)			// ボタンの数

//*****************************************************************************
//		クラス定義（リザルト)
//*****************************************************************************
class CResult
{
public:
	CResult();
	virtual ~CResult();

	static CResult *Create(CGame::MODEGAME gameMode);

	virtual HRESULT Init();
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual HRESULT Load(void);	//	読み込む関数(テクスチャ)
	virtual void Unload(void);	// 開放する関数(テクスチャ)

								//	静的メンバ関数
	static void CntDownFadeTime(void);

private:
	static CResult *m_pResult;
	static int m_nCntfade;
};
//*****************************************************************************
//		クラス定義（落下ED)
//*****************************************************************************
class CFallingEnd : public CResult
{
public:
	typedef enum
	{//シーンで使ってるテクスチャの種類
		SCENETYPE_MASK = 0,		//黒背景
		SCENETYPE_BLOCK,		//岩
		SCENETYPE_PLAYER,		//プレイヤー
		SCENETYPE_SPEECH,		//断末魔
		SCENETYPE_LIGHT,		//照明
		SCENETYPE_SOUL,			//魂
		SCENETYPE_LOGO,			//ロゴ
		SCENETYPE_MENU_RETRY,	//メニュー（リトライ）
		SCENETYPE_MENU_QUIT,	//メニュー（クイット）
		SCENETYPE_MAX
	}SceneType;

	typedef enum
	{// 落下EDの状態
		FALLINGMODE_NONE = 0,
		FALLINGMODE_MOVENOSTART,//アニメーション未開始状態
		FALLINGMODE_PLAYERFALL,	//プレイヤーが落ちていく状態
		FALLINGMODE_DEATH,		//プレイヤーが死んだ状態
		FALLINGMODE_LIGHTON,	//ライトが付く
		FALLIMGMODE_MAX
	}FallingMode;

	typedef enum
	{// メニュー選択の状態
		MENUSTATE_NONE = 0,
		MENUSTATE_RETRY,
		MENUSTATE_QUIT,
		MENUSTATE_MAX
	}MenuState;

	typedef enum
	{// メニュー選択状態の色と位置
		MENUSTATECOLOR_NONE = 0,
		MENUSTATECOLOR_RETRY,
		MENUSTATECOLOR_QUIT,
		MENUSTATECOLOR_MAX
	}MenuStateCol;

	CFallingEnd();
	~CFallingEnd();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	HRESULT Load(void);										//	読み込む関数(テクスチャ)
	void Unload(void);										// 開放する関数(テクスチャ)

	void MoveScene(SceneType type);
	void SkipProduction(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_FALLINGMODE_TEXTURE];	// 共有テクスチャへのポインタ
	CScene2D *m_pScene[MAX_FALLINGMODE_POLYGON];					//シーンのポインタ（テクスチャの最大数)
	FallingMode m_FallingMode;
	MenuState m_MenuState;
	MenuStateCol m_MenuCol;
	CPress *m_apPress[MAX_PRESS_BUTTON];
	float m_Scalvalue, m_fCntMove, m_fChengeMove;
	bool m_bSkipProduction;											//演出を飛ばすかどうか
	int m_nSelectNum;
};
//*****************************************************************************
//		クラス定義（捕食ED)
//*****************************************************************************
class CPreyEnd : public CResult
{
public:
	typedef enum
	{//シーンで使ってるテクスチャの種類
		SCENETYPE_SKY = 0,		//空
		SCENETYPE_WOOD,			//森
		SCENETYPE_CLOUD,		//雲
		SCENETYPE_GROUND,		//地面
		SCENETYPE_PLAYER,		//プレイヤー
		SCENETYPE_OPTION,		//驚き記号
		SCENETYPE_SOUL,			//魂
		SCENETYPE_LOGO,			//ロゴ
		SCENETYPE_MENU_RETRY,	//メニュー（リトライ）
		SCENETYPE_MENU_QUIT,	//メニュー（クイット）
		SCENETYPE_MAX
	}SceneType;

	typedef enum
	{// 落下EDの状態
		PREYMODE_NONE = 0,
		PREYMODE_MOVENOSTART,	//アニメーション未開始状態
		PREYMODE_PLAYERPREY,	//プレイヤーがつかまった状態
		PREYMODE_DEATH,			//プレイヤーが死んだ状態
		PREYMODE_MAX
	}PreyMode;

	typedef enum
	{// メニュー選択の状態
		MENUSTATE_NONE = 0,
		MENUSTATE_RETRY,
		MENUSTATE_QUIT,
		MENUSTATE_MAX
	}MenuState;

	typedef enum
	{// メニュー選択状態の色と位置
		MENUSTATECOLOR_NONE = 0,
		MENUSTATECOLOR_RETRY,
		MENUSTATECOLOR_QUIT,
		MENUSTATECOLOR_MAX
	}MenuStateCol;

	CPreyEnd();
	~CPreyEnd();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	HRESULT Load(void);									//	読み込む関数(テクスチャ)
	void Unload(void);									// 開放する関数(テクスチャ)

	void MoveScene(SceneType type);
	void SkipProduction(void);
	void ScrollCloud(float Scroll);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_PREYMODE_TEXTURE];	// 共有テクスチャへのポインタ
	CScene2D *m_pScene[MAX_PREYMODE_POLYGON];					//シーンのポインタ（テクスチャの最大数)
	PreyMode m_PreyEndMode;
	MenuState m_MenuState;
	MenuStateCol m_MenuCol;
	CPress *m_apPress[MAX_PRESS_BUTTON];
	int nCntframe;
	float m_fCntMove, m_fChengeMove;							//魂の縦移動用
	float m_fCntPlayerMove, m_fChangePlayerMove;				//プレイヤーの縦移動用
	float m_fCntWoodMove, m_fChangeWoodMove;					//森の縦移動に使う
	bool m_bSkipProduction;										//演出を飛ばすかどうか
	float m_fOptionCol;											//驚き記号の色いじりに使う
	int m_nSelectNum;
	float m_fCloudScroll;
};
//*****************************************************************************
//		クラス定義（逃走ED)
//*****************************************************************************
class CEscapeEnd : public CResult
{
public:
	typedef enum
	{//シーンで使ってるテクスチャの種類
		SCENETYPE_SKY = 0,		//空
		SCENETYPE_BG,			//背景
		SCENETYPE_CLOUD,		//雲
		SCENETYPE_ENEMY,		//敵
		SCENETYPE_PLAYER,		//プレイヤー
		SCENETYPE_LOGO,			//ロゴ
		SCENETYPE_MAX
	}SceneType;

	CEscapeEnd();
	~CEscapeEnd();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	HRESULT Load(void);									//	読み込む関数(テクスチャ)
	void Unload(void);									// 開放する関数(テクスチャ)

	void ScrollCloud(float Scroll);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_ESCAPEMODE_TEXTURE];	// 共有テクスチャへのポインタ
	CScene2D *m_pScene[MAX_ESCAPEMODE_POLYGON];						//シーンのポインタ（テクスチャの最大数)
	float m_fScrollCloud;											//雲スクロール
	float m_fCntMove, m_fChengeMove;								//プレイヤーの左右揺れ
};

#endif
