//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ranking.h"		// ランキング
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "title.h"			// タイトル
#include "number.h"			// 数字
#include "texture.h"		// テクスチャ
#include "scene3d.h"		// シーン3D
#include "camera.h"			// カメラ

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_TEXT_NAME	"data\\TEXT\\SCORE\\ranking.bin"		// テキスト

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int		CRanking::m_nRanking[m_RANKING_NUM] = { 0 };		// ランキング
int		CRanking::m_nScore					= 0;			// スコア
int		CRanking::m_RankInNum				= 0;			// ランクイン番号

//=============================================================================
// コンストラクタ									(public)	*** CRanking ***
//=============================================================================
CRanking::CRanking() : CScene(CScene::PRIORITY_UI, CScene::OBJTYPE_RANKING)
{
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 角度
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 大きさ
	m_col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_NextInterval	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 間隔
}

//=============================================================================
// デストラクタ										(public)	*** CRanking ***
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// ロード処理							(public)	*** CNumber ***
//=============================================================================
HRESULT CRanking::Load(void)
{
	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CNumber ***
//=============================================================================
void CRanking::Unload(void)
{
	
}

//=============================================================================
// ランキングセーブ									(public)	*** CRanking ***
//=============================================================================
void CRanking::SaveRanking(void)
{
	FILE *pFile;						// ファイルポインタ
	int nScore = m_nScore;

	if (m_nRanking[4] < nScore)
	{
		m_nRanking[4] = nScore;

		int nMin;	// ソート対象
		int nData;	// スコア

		for (int nCntRanking = 0; nCntRanking < m_RANKING_NUM; nCntRanking++)
		{// カウント
			nMin = nCntRanking;		// 番号を記憶

			while (nMin > 0 && m_nRanking[nMin - 1] < m_nRanking[nMin])
			{// 値が小さかったら
				// マップをソート
				nData = m_nRanking[nMin - 1];
				m_nRanking[nMin - 1] = m_nRanking[nMin];
				m_nRanking[nMin] = nData;

				nMin--;			// ソート対象を移動
			}
		}
	}

	m_RankInNum = 99;

	for (int nCntRanking = 0; nCntRanking < m_RANKING_NUM; nCntRanking++)
	{// カウント
		if (nScore == m_nRanking[nCntRanking] && nScore != 0)
		{// ランクイン番号
			m_RankInNum = nCntRanking;

			break;
		}
	}

/*	m_nRanking[0] = 30000;
	m_nRanking[1] = 25000;
	m_nRanking[2] = 20000;
	m_nRanking[3] = 15000;
	m_nRanking[4] = 10000;*/

	pFile = fopen(RANKING_TEXT_NAME, "wb");		// ファイルを開く

	if (pFile != NULL)
	{// NULL以外の場合
		fwrite(&m_nRanking, sizeof(int), m_RANKING_NUM, pFile);		// 書き込み

		fclose(pFile);							// ファイルを閉じる
	}

	m_nScore = 0;
}

//=============================================================================
// ランキングロード									(public)	*** CRanking ***
//=============================================================================
void CRanking::LoadRanking(void)
{
	FILE *pFile;						// ファイルポインタ

	pFile = fopen(RANKING_TEXT_NAME, "rb");		// ファイルを開く

	if (pFile != NULL)
	{
		fread(&m_nRanking, sizeof(int), m_RANKING_NUM, pFile);		// 読み込み

		fclose(pFile);							// ファイルを閉じる
	}

	m_RankInNum = 99;
	m_nScore = 0;
}

//=============================================================================
// 確保処理											(public)	*** CRanking ***
//=============================================================================
CRanking *CRanking::Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 nextInterval)
{
	CRanking *pRanking = NULL;					// ポインタ

	if (pRanking == NULL)
	{// NULLの場合
		if (type == TYPE_2D)
			pRanking = new CRanking_2D;			// メモリ確保
	}

	if (pRanking != NULL)
	{// NULL以外の場合
		pRanking->m_pos = pos;					// 位置
		pRanking->m_rot = rot;					// 角度
		pRanking->m_size = size;				// 大きさ
		pRanking->m_col = col;					// 色
		pRanking->m_NextInterval = nextInterval;// 間隔
		pRanking->Init();						// 初期化処理
	}

	return pRanking;			// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CRanking ***
//=============================================================================
void CRanking::Uninit(void)
{
	CScene::SetDeath();			// 死亡フラグ
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CRanking_2D ***
//=============================================================================
CRanking_2D::CRanking_2D() : CRanking()
{
	for (int nCntNum = 0; nCntNum < m_RANKING_NUM; nCntNum++)
	{
		for (int nCntDigit = 0; nCntDigit < m_RANKING_DIGIT; nCntDigit++)
			m_pNumber[nCntNum][nCntDigit] = NULL;

		m_pScene2D[nCntNum] = NULL;		// シーン2D
	}

	m_nCntTimer = 0;					// 時間カウント
}

//=============================================================================
// デストラクタ										(public)	*** CRanking_2D ***
//=============================================================================
CRanking_2D::~CRanking_2D()
{

}

//=============================================================================
// ロード処理							(public)	*** CNumber ***
//=============================================================================
HRESULT CRanking_2D::Load(void)
{
	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CNumber ***
//=============================================================================
void CRanking_2D::Unload(void)
{

}

//=============================================================================
// 初期化処理								(public)	*** CNumber ***
//=============================================================================
HRESULT CRanking_2D::Init(void)
{
	for (int nCntNum = 0; nCntNum < m_RANKING_NUM; nCntNum++)
	{// ランキング数カウント
		float fScoreData = (float)m_nRanking[nCntNum];

		for (int nCntDigit = 0; nCntDigit < m_RANKING_DIGIT; nCntDigit++)
		{// 桁数カウント
			if (m_pNumber[nCntNum][nCntDigit] == NULL)
			{// NULLの場合
				m_pNumber[nCntNum][nCntDigit] = new CNumber2D;
				m_pNumber[nCntNum][nCntDigit]->Init(m_pos + D3DXVECTOR3((-((m_size.x * 2.0f) + m_NextInterval.x) * nCntDigit) - m_size.x, (((m_size.y * 2.0f) + m_NextInterval.y) * nCntNum) + m_size.y, 0.0f), m_size, m_col);
				
				m_pNumber[nCntNum][nCntDigit]->SetTex(10, (int)fScoreData % 10, D3DXVECTOR2(0.1f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));					// テクスチャ設定
			}
	
			fScoreData /= 10;		// 桁を進める
		}

		// 順位表記
		m_pScene2D[nCntNum] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
		m_pScene2D[nCntNum]->SetInitAll(m_pos + D3DXVECTOR3((-((m_size.x * 2.0f) + m_NextInterval.x) * m_RANKING_DIGIT) - (m_size.x * 2.0f), (((m_size.y * 2.0f) + m_NextInterval.y) * nCntNum) + (m_size.y * 0.8f), 0.0f),
			D3DXVECTOR3((m_size.x * 2.0f), (m_size.y * 0.8f), m_size.z), m_col, D3DXVECTOR2(1.0f, 0.2f));
		m_pScene2D[nCntNum]->Init();
		m_pScene2D[nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_RANK));
		m_pScene2D[nCntNum]->SetTex(1, nCntNum, D3DXVECTOR2(1.0f, 0.2f), D3DXVECTOR2(0.0f, 0.0f));
	}

	return S_OK;
}

//=============================================================================
// 終了処理								(public)	*** CNumber ***
//=============================================================================
void CRanking_2D::Uninit(void)
{
	for (int nCntNum = 0; nCntNum < m_RANKING_NUM; nCntNum++)
	{// ランキング数カウント
		for (int nCntDigit = 0; nCntDigit < m_RANKING_DIGIT; nCntDigit++)
		{// 桁数カウント
			if (m_pNumber[nCntNum][nCntDigit] != NULL)
			{// NULL以外の場合
				m_pNumber[nCntNum][nCntDigit]->Uninit();	// 終了処理
				m_pNumber[nCntNum][nCntDigit] = NULL;		// NULLへ
			}
		}

		if (m_pScene2D[nCntNum] != NULL)
		{// NULL以外の場合
			m_pScene2D[nCntNum]->Uninit();		// 終了処理
			m_pScene2D[nCntNum] = NULL;			// NULLへ
		}
	}

	CRanking::Uninit();
}

//=============================================================================
// 更新処理								(public)	*** CNumber ***
//=============================================================================
void CRanking_2D::Update(void)
{
	for (int nCntNum = 0; nCntNum < m_RANKING_NUM; nCntNum++)
	{// ランキング数カウント
		for (int nCntDigit = 0; nCntDigit < m_RANKING_DIGIT; nCntDigit++)
		{// 桁数カウント
			if (m_pNumber[nCntNum][nCntDigit] != NULL)
			{// NULL以外の場合
				if (m_RankInNum == nCntNum)
				{// ランクインした番号と同じ
					if (m_nCntTimer < 30)
						m_pNumber[nCntNum][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
					else
						m_pNumber[nCntNum][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{// その他
					m_pNumber[nCntNum][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
		}
	}

	m_nCntTimer = (m_nCntTimer + 1) % 60;
}

//=============================================================================
// 描画処理								(public)	*** CNumber ***
//=============================================================================
void CRanking_2D::Draw(void)
{

}