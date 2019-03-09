//=============================================================================
//
// タイトル処理 [title.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// タイトル
#include "manager.h"	// マネージャー
#include "input.h"		// 入力

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel3D;			// モデル3D
class CScene3D;			// シーン3D
class CScene2D;			// シーン2D
class CRanking;			// ランキング
class CPlayer;			// プレイヤー

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitle
{// タイトル
public:
	typedef enum
	{// タイトル
		TITLE_NONE = 0,		// 何もしていない
		TITLE_LOGO,			// タイトルロゴ
		TITLE_MENU,			// メニュー
		TITLE_CUSTOM,		// カスタム
		TITLE_RANKING,		// ランキング
		TITLE_MAX
	}TITLE;

	CTitle();						// コンストラクタ
	~CTitle();						// デストラクタ

	static HRESULT Load(void);		// ロード
	static void Unload(void);		// アンロード

	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	static void SetTitle(TITLE title);								// タイトル設定
	static TITLE GetTitle(void) { return m_title; }					// タイトル取得

	static bool GetTitlePress(void) { return m_bPress_R; }			// プレス取得
	static void SetTitleRDelete(void) { m_pRanking = NULL; }		// タイトル破棄

	static void SetPlayGame(void) { if (m_bPlayGame != true) { m_bPlayGame = true; m_nCntTitle = 0; } }			// プレイゲーム
	static bool GetPlayGame(void) { return m_bPlayGame; }			// プレイゲーム

private:
	static LPDIRECT3DTEXTURE9	*m_pModelTexture;			// テクスチャ情報へのポインタ
	static LPD3DXMESH			m_pMesh;					// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat;					// マテリアル情報へのポインタ
	static DWORD				m_nNumMat;					// マテリアル情報の数
	static CModel3D				*m_pModel3D;				// モデル3D

	static LPDIRECT3DTEXTURE9	m_pPressTexture;			// テクスチャ情報へのポインタ
	static CScene3D				*m_pPress3D;				// プレス3D

	static LPDIRECT3DTEXTURE9	m_pStarTexture;				// テクスチャ情報へのポインタ
	static CScene3D				*m_pStar3D;					// スター3D

	static LPDIRECT3DTEXTURE9	m_pSceneTexture;			// テクスチャ情報へのポインタ
	static CScene2D				*m_pScene2D[MAX_PLAYER];		// シーン2D

	static CRanking				*m_pRanking;				// ランキング

	static TITLE				m_title;					// タイトル

	static int					m_nCntTitle;				// タイトルカウンタ
	static bool					m_bTitleSet_T;				// タイトル設定T
	static bool					m_bTitleSet_R;				// タイトル設定R
	static bool					m_bPressSet;				// プレス設定
	static bool					m_bPress_R;					// プレス入力

	static CPlayer				*m_pPlayer[MAX_PLAYER];		// プレイヤー
	static bool					m_bPlayGame;				// プレイゲーム

	static bool					m_bPlayerUse[MAX_CONTROLLERS];			// プレイヤー使用
	static bool					m_bPlayerPress[MAX_CONTROLLERS];		// プレイヤープレス

protected:

};

#endif