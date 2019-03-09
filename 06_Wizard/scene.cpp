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
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力
#include "font.h"		// フォント
#include "imgui_es.h"	// ImGui_es

#include "camera.h"		// カメラ

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
CScene		*CScene::m_pTop[PRIORITY_MAX]	= {};		// 先頭のオブジェクトへのポインタ			
CScene		*CScene::m_pCur[PRIORITY_MAX]	= {};		// 現在のオブジェクトへのポインタ

int			CScene::m_nNumAll				= 0;		// 総数
int			CScene::m_nNumPri[PRIORITY_MAX] = {};		// 総数

int			CScene::m_nStartUp				= 0;		// 更新範囲

int			CScene::m_nDrawMode				= 0;		// 描画モード

int			CScene::m_nCntTimer				= 0;		// 時間カウント
int			CScene::m_nUpdateTime			= 1;		// 更新カウント

bool		CScene::m_bDrawUi				= true;		// Ui描画

//=============================================================================
// コンストラクタ									(public)	*** CScene ***
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	m_pPrev = m_pCur[nPriority];						// 前のオブジェクト設定
	m_pNext = NULL;										// 次のオブジェクト設定

	m_pCur[nPriority] = this;							// 現在のオブジェクト設定

	if (m_pPrev != NULL)
	{// 前のオブジェクトがNULL以外の場合
		m_pPrev->m_pNext = m_pCur[nPriority];			// 前の次のオブジェクト設定
	}

	m_nPriority = nPriority;							// 順位設定
	m_objType = objType;								// 種類
	m_bDeath = false;									// 死亡フラグ
	m_nNumAll++;										// 総数カウント
	m_nNumPri[nPriority]++;								// 総数カウント

	if (m_pTop[nPriority] == NULL)
	{// 先頭のオブジェクトがNULL以外の場合
		m_pTop[nPriority] = m_pCur[nPriority];			// 先頭のオブジェクト設定
	}
	else if (m_pTop[nPriority] == m_pCur[nPriority]->m_pPrev)
	{// 先頭のオブジェクトと前のオブジェクトが同じ場合
		m_pTop[nPriority]->m_pNext = m_pCur[nPriority];	// 先頭の次のオブジェクト設定
	}

	char cObjName[32];					// オブジェクト名
	ConObjtype(objType, cObjName);		// オブジェクト文字変換

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);		// フォント取得

	// 文字設定
	if (pFont != NULL)
	{// NULL以外の場合、
		pFont->SetMessage(CFont::SETTYPE_LOG, "○ Create : [%s(%d)]\n", cObjName, nPriority);
	}

	// ImGuiLog設定
	CImgui_ES::SetMessage(" Create : [%s(%d)]\n", cObjName, nPriority);
}

//=============================================================================
// デストラクタ										(public)	*** CScene ***
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// 全フェード以外開放処理								(public)	*** CScene ***
//=============================================================================
void CScene::ReleaseMode(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_FADE; nCntPriority++)
	{// プライオリティ数カウント
		while (m_pTop[nCntPriority] != NULL)
		{// 先頭のオブジェクトがNULLになるまで
			m_pTop[nCntPriority]->Uninit();			// 先頭のオブジェクト終了
			m_pTop[nCntPriority]->Release();		// 開放処理
		}
	}
}

//=============================================================================
// 全開放処理										(public)	*** CScene ***
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// プライオリティ数カウント
		while (m_pTop[nCntPriority] != NULL)
		{// 先頭のオブジェクトがNULLになるまで
			m_pTop[nCntPriority]->Uninit();			// 先頭のオブジェクト終了
			m_pTop[nCntPriority]->Release();		// 開放処理
		}
	}
}

//=============================================================================
// 全更新処理										(public)	*** CScene ***
//=============================================================================
void CScene::UpdateAll(void)
{
	static bool bStop = false;
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	//	bStop = bStop ? false : true;

	if (bStop == false)
	{
		if (m_nCntTimer == 0)
		{
			for (int nCntPriority = m_nStartUp; nCntPriority < PRIORITY_MAX; nCntPriority++)
			{// プライオリティ数カウント
				CScene *pScene = m_pTop[nCntPriority];		// 更新のオブジェクト（先頭を記憶）
				CScene *pNext = NULL;						// 次のオブジェクト記憶

				while (pScene != NULL)
				{// 更新のオブジェクトがNULL以外の場合
					pNext = pScene->m_pNext;				// 次のオブジェクト設定
					if (pScene->m_bDeath == false)
					{// 破棄しない
						pScene->Update();					// 更新処理
					}
					if (pScene->m_bDeath == true)
					{// 破棄する
						pScene->Release();					// 開放処理
					}
					pScene = pNext;							// 更新のオブジェクトに次のオブジェクト設定
				}
			}
		}

		if (m_nUpdateTime < 1) m_nUpdateTime = 1;
		m_nCntTimer = (m_nCntTimer + 1) % m_nUpdateTime;
	}
	else
	{
		CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
		pCamera->SetMove(false);
	}
}
//=============================================================================
// 全描画処理										(public)	*** CScene ***
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// プライオリティ数カウント
		if (m_bDrawUi == true || (m_bDrawUi == false && nCntPriority < PRIORITY_UI))
		{
			if (nCntPriority != PRIORITY_MAP_1 && nCntPriority != PRIORITY_MAP_2)
			{// プライオリティ制限
				CScene *pScene = m_pTop[nCntPriority];		// 描画のオブジェクト（先頭を記憶）
				CScene *pNext = NULL;						// 次のオブジェクト記憶

				while (pScene != NULL)
				{// 描画のオブジェクトがNULL以外の場合
					pNext = pScene->m_pNext;				// 次のオブジェクト設定

					if (DrawSelect(0, pScene->GetObjType()) != true)
					{
						if (pScene->m_bDeath == false)
						{// 破棄しない
							pScene->Draw();						// 描画処理
						}
						if (pScene->m_bDeath == true)
						{// 破棄する
							pScene->Release();					// 開放処理
						}
					}
					DrawSelect(1, pScene->GetObjType());

					pScene = pNext;							// 描画のオブジェクトに次のオブジェクト設定
				}
			}
		}
	}
}


//=============================================================================
// 全描画マップ処理									(public)	*** CScene ***
//=============================================================================
void CScene::DrawAllMap(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// プライオリティ数カウント
		CScene *pScene = m_pTop[nCntPriority];		// 描画のオブジェクト（先頭を記憶）
		CScene *pNext = NULL;						// 次のオブジェクト記憶

		while (pScene != NULL)
		{// 描画のオブジェクトがNULL以外の場合
			pNext = pScene->m_pNext;				// 次のオブジェクト設定

			if (DrawSelect(0, pScene->GetObjType()) != true)
			{
				if (pScene->GetObjType() != OBJTYPE_PLAYER && pScene->GetObjType() != OBJTYPE_ENEMY && pScene->GetObjType() != OBJTYPE_GAUGE)
				{// オブジェクト制限
					if (pScene->m_bDeath == false)
					{// 破棄しない
						pScene->Draw();					// 描画処理
					}
				}
				if (pScene->m_bDeath == true)
				{// 破棄する
					pScene->Release();					// 開放処理
				}
			}
			DrawSelect(1, pScene->GetObjType());

			pScene = pNext;							// 描画のオブジェクトに次のオブジェクト設定
		}
	}
}

//=============================================================================
// 開放処理											(protected)	*** CScene ***
//=============================================================================
void CScene::Release(void)
{
	if (this != NULL)
	{// 自分がNULL以外の場合
		int nPriority = m_nPriority;
		OBJTYPE objType = m_objType;

		if (m_pPrev != NULL)	// m_pTop以外
		{// 前のオブジェクトがNULL以外の場合
			if (m_pTop[nPriority] == m_pPrev)
			{// 先頭のオブジェクトが前のオブジェクトと同じ場合
				m_pTop[nPriority]->m_pNext = m_pNext;		// 先頭の次のオブジェクト設定
			}

			m_pPrev->m_pNext = m_pNext;						// 前の次のオブジェクト設定
		}

		if (m_pNext != NULL)	// m_pCur以外
		{// 次のオブジェクトがNULL以外の場合
			if (m_pCur[nPriority] == m_pNext)
			{// 現在のオブジェクトが前のオブジェクトと同じ場合
				m_pCur[nPriority]->m_pPrev = m_pPrev;		// 現在の前のオブジェクト設定
			}

			m_pNext->m_pPrev = m_pPrev;						// 次の前のオブジェクト設定
		}

		if (m_pTop[nPriority] == this)
		{// 先頭のオブジェクトが自分と同じ場合
			m_pTop[nPriority] = m_pNext;					// 先頭のオブジェクトを次に更新
		}
		if (m_pCur[nPriority] == this)
		{// 現在のオブジェクトが自分と同じ場合
			m_pCur[nPriority] = m_pPrev;					// 現在のオブジェクトを前に更新
		}

		m_pPrev = NULL;										// 前のオブジェクト
		m_pNext = NULL;										// 次のオブジェクト

		char cObjName[32];									// オブジェクト名
		ConObjtype(objType, cObjName);						// オブジェクト文字変換

		CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);		// フォント取得

		// 文字設定
		if (pFont != NULL)
		{// NULL以外の場合、
			pFont->SetMessage(CFont::SETTYPE_LOG, "○ Delete : [%s(%d)]\n", cObjName, nPriority);
		}

		// ImGuiLog設定
		CImgui_ES::SetMessage(" Delete : [%s(%d)]\n", cObjName, nPriority);

		delete this;										// 破棄
		m_nNumPri[nPriority]--;								// プライオリティ総数を減らす
		m_nNumAll--;										// 総数を減らす
	}
}

//=============================================================================
// オブジェクトタイプ文字変換処理						(public)	*** CScene ***
//=============================================================================
bool CScene::DrawSelect(int nStartEnd, OBJTYPE objType)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (nStartEnd == 0)
	{
		if (m_nDrawMode == 0)
		{
			// フィルモード設定
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		else if (m_nDrawMode == 1)
		{
			// フィルモード設定
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else if (m_nDrawMode == 2)
		{
			if (objType != OBJTYPE_PLAYER)
			{
				// フィルモード設定
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
		}
		else if (m_nDrawMode == 3)
		{
			if (objType == OBJTYPE_PLAYER)
			{
				// フィルモード設定
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
		}
		else if (m_nDrawMode == 4)
		{
			if (objType != OBJTYPE_PLAYER)
			{
				return true;
			}
		}
		else if (m_nDrawMode == 5)
		{
			if (objType == OBJTYPE_PLAYER)
			{
				return true;
			}
		}
	}
	else
	{
		// フィルモード設定
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	return false;
}

//=============================================================================
// オブジェクトタイプ文字変換処理						(public)	*** CScene ***
//=============================================================================
void CScene::ConObjtype(CScene::OBJTYPE objType, char *cObjType)
{
	switch (objType)
	{// オブジェクトタイプ
	case OBJTYPE_MESHFIELD:		// メッシュフィールド
		wsprintf(cObjType, "Meshfield");
		break;
	case OBJTYPE_MESHWALL:		// メッシュウォール
		wsprintf(cObjType, "Meshwall");
		break;
	case OBJTYPE_MESHCYLINDER:	// メッシュシリンダー
		wsprintf(cObjType, "Meshcylinder");
		break;
	case OBJTYPE_BG:			// 背景
		wsprintf(cObjType, "Bg");
		break;
	case OBJTYPE_ENEMY:			// 敵
		wsprintf(cObjType, "Enemy");
		break;
	case OBJTYPE_PLAYER:		// プレイヤー
		wsprintf(cObjType, "Player");
		break;
	case OBJTYPE_BULLET:		// 弾
		wsprintf(cObjType, "Bullet");
		break;
	case OBJTYPE_EFFECT:		// エフェクト
		wsprintf(cObjType, "Effect");
		break;
	case OBJTYPE_NUMBER:		// 番号
		wsprintf(cObjType, "Number");
		break;
	case OBJTYPE_SCORE:			// スコア
		wsprintf(cObjType, "Score");
		break;
	case OBJTYPE_RANKING:		// ランキング
		wsprintf(cObjType, "Ranking");
		break;
	case OBJTYPE_MENU:			// メニュー
		wsprintf(cObjType, "Menu");
		break;
	case OBJTYPE_PAUSE:			// ポーズ
		wsprintf(cObjType, "Pause");
		break;
	case OBJTYPE_TITLE:			// タイトル
		wsprintf(cObjType, "Title");
		break;
	case OBJTYPE_FADE:			// フェード
		wsprintf(cObjType, "Fade");
		break;
	case OBJTYPE_CAMERA:		// カメラ
		wsprintf(cObjType, "Camera");
		break;
	default:					// その他
		wsprintf(cObjType, "Unknow");
		break;
	}
}