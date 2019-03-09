//=============================================================================
//
// スコア処理 [score.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "score.h"			// スコア
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "number.h"			// 数字

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
int			CScore::m_nScore[MAX_PLAYER]			= {};		// スコア

//=============================================================================
// コンストラクタ									(public)	*** CScore ***
//=============================================================================
CScore::CScore() : CScene(CScene::PRIORITY_UI)
{
	// 初期値設定
	for (int nCntDigit = 0; nCntDigit < SCORE_MAX_DIGIT; nCntDigit++)
	{// カウント
		m_apNumber2D[nCntDigit] = NULL;
	}
}

//=============================================================================
// デストラクタ										(public)	*** CScore ***
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// 確保処理											(public)	*** CScore ***
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nScoreNum)
{
	CScore *pScore = NULL;					// ポインタ

	if (pScore == NULL)
	{// NULLの場合
		pScore = new CScore;				// メモリ確保
	}

	if (pScore != NULL)
	{// NULL以外の場合
		pScore->m_nScoreNum = nScoreNum;	// スコア番号

		for (int nCntDigit = 0; nCntDigit < SCORE_MAX_DIGIT; nCntDigit++)
		{// カウント
			if (pScore->m_apNumber2D[nCntDigit] == NULL)
			{
				pScore->m_apNumber2D[nCntDigit] = new CNumber2D;																		// メモリ確保
				pScore->m_apNumber2D[nCntDigit]->Init(D3DXVECTOR3(pos.x - ((size.x * 1.8f) * nCntDigit), pos.y, 0.0f), size, col);		// 初期化処理
				pScore->m_apNumber2D[nCntDigit]->SetObjType(CScene::OBJTYPE_SCORE);														// オブジェクトタイプ設定
			}
		}
		pScore->m_nScore[pScore->m_nScoreNum] = 0;

		pScore->Init();						// 初期化処理
	}

	return pScore;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CScore ***
//=============================================================================
HRESULT CScore::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CScore ***
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntDigit = 0; nCntDigit < SCORE_MAX_DIGIT; nCntDigit++)
	{// カウント
		m_apNumber2D[nCntDigit]->Uninit();	// 終了処理
		m_apNumber2D[nCntDigit] = NULL;		// NULLへ
	}

	CScene::Release();
}

//=============================================================================
// 更新処理											(public)	*** CScore ***
//=============================================================================
void CScore::Update(void)
{
	float fScoreData = (float)m_nScore[m_nScoreNum];	// スコアを格納

	for (int nCntDigit = 0; nCntDigit < SCORE_MAX_DIGIT; nCntDigit++)
	{// カウント
		if (fScoreData < 1.0f && nCntDigit != 0)
		{
			m_apNumber2D[nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		else
		{
			m_apNumber2D[nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		m_apNumber2D[nCntDigit]->SetTex(10, (int)fScoreData % 10, D3DXVECTOR2(0.0f, 0.0f));		// テクスチャ設定

		fScoreData /= 10;		// 桁を進める
	}
}

//=============================================================================
// 描画処理											(public)	*** CScore ***
//=============================================================================
void CScore::Draw(void)
{

}