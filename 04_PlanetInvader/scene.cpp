//=============================================================================
//
// シーン処理 [scene.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_SCENE] = {};		// シーン
int CScene::m_nNumAll = 0;										// 総数
int CScene::m_nStartUp = 0;										// 更新範囲

//=============================================================================
// コンストラクタ									(public)	*** CScene ***
//=============================================================================
CScene::CScene(int nPriority)
{
	for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
	{// シーンカウント
		if (m_apScene[nPriority][nCntScene] == NULL)
		{// NULLの場合
			m_apScene[nPriority][nCntScene] = this;				// 自分を代入
			m_nPriority = nPriority;							// 順位設定
			m_nID = nCntScene;									// ID設定
			m_objType = OBJTYPE_NONE;							// 種類
			m_nNumAll++;										// 総数カウント

			break;
		}
	}
}

//=============================================================================
// コンストラクタ									(public)	*** CScene ***
//=============================================================================
CScene::CScene(int nPriority, int nID)
{
	for (int nCntScene = nID; nCntScene < MAX_SCENE; nCntScene++)
	{// シーンカウント
		if (m_apScene[nPriority][nCntScene] == NULL)
		{// NULLの場合
			m_apScene[nPriority][nCntScene] = this;				// 自分を代入
			m_nPriority = nPriority;							// 順位設定
			m_nID = nCntScene;									// ID設定
			m_objType = OBJTYPE_NONE;							// 種類
			m_nNumAll++;										// 総数カウント

			break;
		}
	}
}

//=============================================================================
// デストラクタ										(public)	*** CScene ***
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// 全開放処理										(public)	*** CScene ***
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// プライオリティカウント
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// シーンカウント
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{// NUll以外の場合
				// 終了処理
				m_apScene[nCntPriority][nCntScene]->Uninit();	// 終了処理

				delete m_apScene[nCntPriority][nCntScene];		// メモリ開放
				m_apScene[nCntPriority][nCntScene] = NULL;		// NULL
			}
		}
	}
}

//=============================================================================
// 全更新処理										(public)	*** CScene ***
//=============================================================================
void CScene::UpadteAll(void)
{
	for (int nCntPriority = m_nStartUp; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// プライオリティカウント
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// シーンカウント
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{// NULL以外の場合
				// ポリゴン更新処理
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
// 全描画処理										(public)	*** CScene ***
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// プライオリティカウント
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// シーンカウント
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{// NULL以外の場合
				// ポリゴン更新処理
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
// 総数取得処理										(public)	*** CScene ***
//=============================================================================
int CScene::GetNumAll(void)
{
	return m_nNumAll;		// 総数を返す
}

//=============================================================================
// 開放処理											(protected)	*** CScene ***
//=============================================================================
void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{// NUll以外の場合
		int nID = m_nID;					// IDを保存（データの不定防止）
		int nPri = m_nPriority;				// 順位を保存（データの不定防止）

		delete m_apScene[nPri][nID];		// メモリ開放
		m_apScene[nPri][nID] = NULL;		// NULLへ
		m_nNumAll--;						// 総数を減らす
	}
}