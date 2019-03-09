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
#include "scene3d.h"		// シーン3D
#include "camera.h"			// カメラ
#include "score.h"			// スコア

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\ranking.png"		// テクスチャ
#define RANKING_TEXT_NAME	"data\\TEXT\\ranking.bin"			// テキスト

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CRanking::m_pTexture					= NULL;						// テクスチャ情報へのポインタ
CNumber3D			*CRanking::m_apNumber3D[RANKING_MAX_NUM][RANKING_MAX_DIGIT] = {};	// ナンバー
CScene3D			*CRanking::m_apScene3D[RANKING_MAX_NUM] = {};						// シーン3D
int					CRanking::m_nRanking[RANKING_MAX_NUM]	= {};						// ランキング
int					CRanking::m_RankInNum = 99;

//=============================================================================
// コンストラクタ									(public)	*** CRanking ***
//=============================================================================
CRanking::CRanking() : CScene(CScene::PRIORITY_UI)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
	{// カウント
		for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
		{// カウント
			m_apNumber3D[nCntRanking][nCntDigit] = NULL;
		}
		m_pos[nCntRanking] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_fScene3DDiff = 0.0f;
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
	if (m_pTexture == NULL)
	{// NULLの場合
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CNumber ***
//=============================================================================
void CRanking::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{// NULL以外の場合
		m_pTexture->Release();			// 解放
		m_pTexture = NULL;				// NULLへ
	}
}

//=============================================================================
// ランキングセーブ									(public)	*** CRanking ***
//=============================================================================
void CRanking::SaveRanking(void)
{
	FILE *pFile;						// ファイルポインタ
	int nScore = 0;
	
	for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
	{
		nScore += CScore::GetScore(nCnt);
	}

	if (m_nRanking[4] < nScore)
	{
		m_nRanking[4] = nScore;

		int nMin;	// ソート対象
		int nData;	// スコア

		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
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

	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
	{// カウント
		if (nScore == m_nRanking[nCntRanking] && nScore != 0)
		{
			m_RankInNum = nCntRanking;

			break;
		}
	}

	/*m_nRanking[0] = 5000;
	m_nRanking[1] = 4000;
	m_nRanking[2] = 3000;
	m_nRanking[3] = 2000;
	m_nRanking[4] = 1000;*/

	pFile = fopen(RANKING_TEXT_NAME, "wb");		// ファイルを開く

	if (pFile != NULL)
	{// NULL以外の場合
		fwrite(&m_nRanking, sizeof(int), RANKING_MAX_NUM, pFile);		// 書き込み

		fclose(pFile);							// ファイルを閉じる
	}
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
		fread(&m_nRanking, sizeof(int), RANKING_MAX_NUM, pFile);		// 読み込み

		fclose(pFile);							// ファイルを閉じる
	}
}

//=============================================================================
// 確保処理											(public)	*** CRanking ***
//=============================================================================
CRanking *CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 nextInterval, int nNum)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// カメラ注視点取得

	CRanking *pRanking = NULL;					// ポインタ

	if (nNum != -1) nNum = 1;					// 表示タイプ

	if (pRanking == NULL)
	{// NULLの場合
		pRanking = new CRanking;				// メモリ確保
	}

	if (pRanking != NULL)
	{// NULL以外の場合
		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
		{// カウント
			for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
			{// カウント
				if (pRanking->m_apNumber3D[nCntRanking][nCntDigit] == NULL)
				{
					pRanking->m_apNumber3D[nCntRanking][nCntDigit] = new CNumber3D;																	// メモリ確保
					pRanking->m_apNumber3D[nCntRanking][nCntDigit]->Init(D3DXVECTOR3(
						((pos.x + (((size.x * 1.8f) * (RANKING_MAX_DIGIT - 1)) * 0.5f)) * nNum) - (((size.x * 1.8f) * nCntDigit) * nNum),
						pos.y + cosf(rot.x + (D3DX_PI * 0.0f)) * (nextInterval.z * nCntRanking),
						pos.z + sinf(rot.x + (D3DX_PI * 0.0f)) * (nextInterval.z * nCntRanking)), rot, size, col, CScene3D::SCENE3DTYPE_NORMAL);	// 初期値設定
					pRanking->m_apNumber3D[nCntRanking][nCntDigit]->SetObjType(CScene::OBJTYPE_RANKING);											// オブジェクトタイプ設定

					pRanking->m_pos[nCntRanking] = D3DXVECTOR3(
						(pos.x - ((size.x * 1.8f) * nCntDigit)),
						pos.y + cosf(rot.x + (D3DX_PI * 0.0f)) * (nextInterval.z * nCntRanking),
						pos.z + sinf(rot.x + (D3DX_PI * 0.0f)) * (nextInterval.z * nCntRanking)) - Camerapos;										// 位置
				}
			}

			D3DXVECTOR3 pos = pRanking->m_apNumber3D[nCntRanking][RANKING_MAX_DIGIT - 1]->Getpos();													// 位置取得
			pRanking->m_fScene3DDiff = ((size.x * 1.8f) * nNum);																					// 順位とスコアの差

			if (pRanking->m_apScene3D[nCntRanking] == NULL)
			{
				pRanking->m_apScene3D[nCntRanking] = new CScene3D(CScene3D::PRIORITY_UI);															// メモリ確保
				pRanking->m_apScene3D[nCntRanking]->SetInitAll(
					D3DXVECTOR3(pos.x - ((size.x * 2.0f) * nNum), pos.y, pos.z), rot,
					D3DXVECTOR3(size.x * 1.5f, size.y, size.z), col, D3DXVECTOR2(1.0f, 0.2f), CScene3D::SCENE3DTYPE_NORMAL);						// 初期値設定
				pRanking->m_apScene3D[nCntRanking]->Init();																							// 初期化処理
				pRanking->m_apScene3D[nCntRanking]->SetObjType(CScene::OBJTYPE_RANKING);															// オブジェクトタイプ設定
				pRanking->m_apScene3D[nCntRanking]->BindTexture(m_pTexture);																		// テクスチャを反映
				pRanking->m_apScene3D[nCntRanking]->SetTex(0, nCntRanking, D3DXVECTOR2(1.0f, 0.2f), D3DXVECTOR2(0.0f, 0.0f));						// テクスチャ設定
			}
		}
		pRanking->Init();						// 初期化処理
	}

	return pRanking;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CRanking ***
//=============================================================================
HRESULT CRanking::Init(void)
{
	float fScoreData;				// スコア格納用

	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
	{// カウント
		fScoreData = (float)m_nRanking[nCntRanking];	// スコアを格納

		for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
		{// カウント
			if (fScoreData < 1.0f && nCntDigit != 0)
			{
				m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			}
			else
			{
				m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}


			m_apNumber3D[nCntRanking][nCntDigit]->SetTex(10, (int)fScoreData % 10, D3DXVECTOR2(0.1f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));					// テクスチャ設定

			fScoreData /= 10;		// 桁を進める
		}
	}

	if (m_RankInNum < 0 || RANKING_MAX_NUM <= m_RankInNum)
	{
		m_RankInNum = 99;
	}

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CRanking ***
//=============================================================================
void CRanking::Uninit(void)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
	{// カウント
		for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
		{// カウント
			if (m_apNumber3D[nCntRanking][nCntDigit] != NULL)
			{// NULL以外の場合
				m_apNumber3D[nCntRanking][nCntDigit]->Uninit();			// 終了処理
				m_apNumber3D[nCntRanking][nCntDigit] = NULL;			// NULLへ
			}
		}

		if (m_apScene3D[nCntRanking] != NULL)
		{// NULL以外の場合
			m_apScene3D[nCntRanking]->Uninit();							// 終了処理
			m_apScene3D[nCntRanking] = NULL;   							// NULLへ
		}
	}

	CScene::Release();
}

//=============================================================================
// 更新処理											(public)	*** CRanking ***
//=============================================================================
void CRanking::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// カメラ注視点取得
	CManager::MODE mode = CManager::GetMode();							// モード取得
	bool bTitlePress = CTitle::GetTitlePress();							// タイトルプレス取得
	D3DXVECTOR3 pos;													// 位置
	D3DXVECTOR3 sizeNum, sizeScene;										// 大きさ
	bool bDelete = false;												// 破棄

	switch (mode)
	{// モード
	case CManager::MODE_TITLE:		// タイトル
		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
		{// カウント
			for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
			{// カウント
				pos = m_apNumber3D[nCntRanking][nCntDigit]->Getpos();				// 位置取得
				sizeNum = m_apNumber3D[nCntRanking][nCntDigit]->Getsize();			// 大きさ取得

				if (bTitlePress == true)
				{// タイトルプレス
					// 大きさを減少
					sizeNum.x = sizeNum.x * 0.9f;
					sizeNum.y = sizeNum.y * 0.9f;

					if (sizeNum.x < 0.1f || sizeNum.y < 0.1f)
					{// 見えなくなったら
						bDelete = true;			// 破棄する
					}
				}
				else
				{
					pos.x += 0.2f;				// 移動
				}
				
				m_apNumber3D[nCntRanking][nCntDigit]->Setpos(pos);					// 位置を設定
				m_apNumber3D[nCntRanking][nCntDigit]->Setsize(sizeNum);				// 大きさを設定
				m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			sizeScene = m_apScene3D[nCntRanking]->Getsize();						// 大きさ取得

			if (bTitlePress == true)
			{// タイトルプレス
				// 大きさを減少
				sizeScene.x = sizeScene.x * 0.9f;
				sizeScene.y = sizeScene.y * 0.9f;

				if (sizeScene.x < 0.1f || sizeScene.y < 0.1f)
				{// 見えなくなったら
					bDelete = true;				// 破棄する
				}
			}

			m_apScene3D[nCntRanking]->Setpos(D3DXVECTOR3(pos.x - m_fScene3DDiff, pos.y, pos.z));		// 位置を設定
			m_apScene3D[nCntRanking]->Setsize(sizeScene);												// 大きさを設定
		}

		break;
	case CManager::MODE_GAME:		// ゲーム
		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
		{// カウント
			for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
			{// カウント
				pos = m_apNumber3D[nCntRanking][nCntDigit]->Getpos();				// 位置取得

				// カメラに対しての位置を設定
				//pos.x = Camerapos.x + m_pos[nCntRanking].x;						
				pos.z = Camerapos.z + m_pos[nCntRanking].z;

				if (m_RankInNum != 99 && nCntRanking == m_RankInNum)
				{
					m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				}
				else
				{
					m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}

				m_apNumber3D[nCntRanking][nCntDigit]->Setpos(pos);					// 位置を設定

				m_pos[nCntRanking] = pos - Camerapos;								// 現在位置を計算
			}

			m_apScene3D[nCntRanking]->Setpos(D3DXVECTOR3(pos.x - m_fScene3DDiff, pos.y, pos.z));		// 位置を設定
		}

		break;
	}

	if (bDelete == true)
	{// 破棄する
		Uninit();		// 終了処理

		if (mode == CManager::MODE_TITLE)
		{// モードがタイトル
			CTitle::SetTitleRDelete();		// タイトル破棄
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CRanking ***
//=============================================================================
void CRanking::Draw(void)
{

}