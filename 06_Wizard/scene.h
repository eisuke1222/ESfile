//=============================================================================
//
// シーン処理 [scene.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene
{// シーン
public:
	typedef enum
	{// プライオリティ
		PRIORITY_BG = 0,		// 背景
		PRIORITY_1,				// 1
		PRIORITY_2,				// 2
		PRIORITY_3,				// 3
		PRIORITY_4,				// 4
		PRIORITY_5,				// 5
		PRIORITY_UI,			// UI
		PRIORITY_MAP_1,			// マップ1
		PRIORITY_MAP_2,			// マップ2
		PRIORITY_PAUSE,			// ポーズ
		PRIORITY_FADE,			// フェード
		PRIORITY_MAX			// 最大数
	}PRIORITY;

	typedef enum
	{// オブジェクトタイプ
		OBJTYPE_NONE = 0,		// 何もしていない
		OBJTYPE_MESHFIELD,		// メッシュフィールド
		OBJTYPE_MESHWALL,		// メッシュウォール
		OBJTYPE_MESHCYLINDER,	// メッシュシリンダー
		OBJTYPE_BG,				// 背景
		OBJTYPE_ENEMY,			// 敵
		OBJTYPE_PLAYER,			// プレイヤー
		OBJTYPE_BULLET,			// 弾
		OBJTYPE_EFFECT,			// エフェクト
		OBJTYPE_GAUGE,			// ゲージ
		OBJTYPE_NUMBER,			// 数字
		OBJTYPE_SCORE,			// スコア
		OBJTYPE_RANKING,		// ランキング
		OBJTYPE_MENU,			// メニュー
		OBJTYPE_ROCKON,			// ロックオン
		OBJTYPE_MAPICON,		// マップアイコン
		OBJTYPE_WARP,			// ワープ
		OBJTYPE_GOAL,			// ゴール
		OBJTYPE_GRASS,			// 草
		OBJTYPE_GUARD,			// ガード
		OBJTYPE_PAUSE,			// ポーズ
		OBJTYPE_PRESS,			// プレス
		OBJTYPE_MAP,			// マップ
		OBJTYPE_UI,				// UI
		OBJTYPE_WINDOW,			// ウィンドウ
		OBJTYPE_TITLE,			// タイトル
		OBJTYPE_FADE,			// フェード
		OBJTYPE_CAMERA,			// カメラ
		OBJTYPE_RESULT,			// リザルト
		OBJTYPE_MAX				// 最大数
	}OBJTYPE;

	CScene(int nPriority, OBJTYPE objType);				// コンストラクタ
	virtual ~CScene();									// デストラクタ

	static void ReleaseMode(void);						// フェード以外破棄
	static void ReleaseAll(void);						// 全破棄
	static void UpdateAll(void);						// 全更新
	static void DrawAll(void);							// 全描画
	static void DrawAllMap(void);						// 全描画マップ

	virtual HRESULT Init(void) = 0;						// 初期化処理
	virtual void Uninit(void) = 0;						// 終了処理
	virtual void Update(void) = 0;						// 更新処理
	virtual void Draw(void) = 0;						// 描画処理

	static int GetNumAll(void) { return m_nNumAll; }	// 総数取得
	static int GetNumPri(int nPriority)					// プライオリティ総数取得
		{ return m_nNumPri[nPriority]; }

	void SetPriority(int nPriority)						// プライオリティ設定
		{ m_nPriority = nPriority; }

	void SetObjType(OBJTYPE objType)					// オブジェクトタイプ設定
		{ m_objType = objType; }		
	OBJTYPE GetObjType(void)							// オブジェクトタイプ取得
		{ return m_objType; }			

	static void SetStartUp(int nStartUp)				// 更新設定
		{ m_nStartUp = nStartUp; }		

	static CScene *GetScene(PRIORITY nPriority)			// シーン取得
		{ return m_pTop[nPriority]; }
	CScene *GetSceneNext(void) { return m_pNext; }		// 次シーン取得

	static void SetDrawMode(int nDrawMode)				// 描画モード設定
		{ m_nDrawMode = nDrawMode; }

	static void SetUpdateTime(int nTime)				// 更新カウント設定
		{ m_nUpdateTime = nTime; }

	static void SetDrawUi(bool bDrawUi)					// Ui描画設定
		{ m_bDrawUi = bDrawUi; }
	static bool GetDrawUi(void)							// Ui描画設定
		{ return m_bDrawUi; }

private:
	static bool DrawSelect(int nStartEnd, OBJTYPE objType);	// 描画選択
	void Release(void);									// 開放処理
	void ConObjtype(OBJTYPE objType, char *cObjType);	// オブジェクト文字変換

	static CScene *m_pTop[PRIORITY_MAX];				// 先頭のオブジェクトへのポインタ
	static CScene *m_pCur[PRIORITY_MAX];				// 現在のオブジェクトへのポインタ

	CScene *m_pPrev;									// 前のオブジェクトへのポインタ
	CScene *m_pNext;									// 次のオブジェクトへのポインタ

	int m_nPriority;									// 種類
	OBJTYPE m_objType;									// 種類
	bool m_bDeath;										// 死亡フラグ

	static int m_nNumAll;								// 総数
	static int m_nNumPri[PRIORITY_MAX];					// プライオリティ総数

	static int m_nStartUp;								// 更新開始番号

	static int m_nDrawMode;								// 描画モード

	static int m_nCntTimer;								// 時間カウント
	static int m_nUpdateTime;							// 更新カウント

	static bool m_bDrawUi;								// Ui描画

protected:
	void SetDeath(void) { m_bDeath = true; }			// 破棄する

};

#endif