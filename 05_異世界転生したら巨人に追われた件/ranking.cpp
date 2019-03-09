//*****************************************************************************
//
//				ランキングの処理[ranking.cpp]
//				Auther : Shun Yokomichi
//
//*****************************************************************************
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "score.h"			// スコア
#include "number.h"			// 数字
#include "ranking.h"		// ランキング
#include "scene2D.h"		// シーン2D
#include "scene.h"			// シーン
#include "fade.h"			// フェード
#include "input.h"			// 入力
#include "rankinglogo.h"	// ランキングロゴ
#include "press.h"			// プレスボタン
#include "scene2d.h"		// シーン2D
#include "ui.h"
#include "sound.h"

//*****************************************************************************
//		マクロ定義
//*****************************************************************************
#define SIZE_X						(30.0f)		// 横幅
#define	SIZE_Y						(30.0f)		// 縦幅
#define POS_X_INTERVAL				(100)		// 横幅の間隔
#define POS_Y_INTERVAL				(90)		// 縦幅の間隔
#define TIME						(600)		// ランキングが終わる時間
#define RANK_SIZE_X					(55.0f)		// ランクの表示の横幅
#define RANK_SIZE_Y					(40.0f)		// ランクの表示の縦幅
#define RANK_SIZE_X_SCAlE			(7.0f)		// ランクの表示の横幅減少
#define RANK_SIZE_Y_SCAlE			(5.0f)		// ランクの表示の横幅減少
#define RANK_BOARD_POS_X			(660.0f)	// ランク板のposX
#define RANK_BOARD_POS_Y			(630.0f)	// ランク板のposY
#define RANK_BOARD_POS_Y_INTERVAL	(90.0f)		// ランク板のposYの間隔
#define RANK_BOARD_SIZE_X			(440.0f)	// ランク板のサイズX
#define RANK_BOARD_SIZE_Y			(80.0f)		// ランク板のサイズY

//*****************************************************************************
//		静的メンバ変数
//*****************************************************************************
CNumber2D		 *CRanking::m_aRanking[MAX_RANKING][MAX_SCORE] = {};
CRnkingLogo		 *CRanking::m_pLogo[MAX_RANK_LOGO] = {};
CUiPolygon		*CRanking::m_apUi[MAX_RANKING] = {};
int				 CRanking::m_nScore[MAX_RANKING] = { 1800, 1500, 1200, 1000, 800 };
int				 CRanking::m_Pattern = 0;
int				 CRanking::m_nNum = MAX_RANKING;

//*****************************************************************************
//		ランキングの生成
//*****************************************************************************
CRanking *CRanking::Create(D3DXVECTOR3 pos)
{
	CRanking *pRanking = NULL;	// ポインタの生成

	if (pRanking == NULL)
	{
		pRanking = new CRanking;

		if (pRanking != NULL)
		{
			pRanking->Init(pos);
		}
	}

	return pRanking;
}

//*****************************************************************************
//		コンストラクタ
//*****************************************************************************
CRanking::CRanking()
{
	// デフォルト値に
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntRanking001 = 0; nCntRanking001 < MAX_SCORE; nCntRanking001++)
		{
			m_aRanking[nCntRanking][nCntRanking001] = NULL;
		}
	}

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		m_apUi[nCntRanking] = NULL;
	}
}

//*****************************************************************************
//		デストラクタ
//*****************************************************************************
CRanking::~CRanking() {}

//*****************************************************************************
//		初期化
//*****************************************************************************
HRESULT CRanking::Init(D3DXVECTOR3 pos)
{
	CManager::MODE pMode = CManager::GetMode();

	CNumber::Load();	//ナンバーの読み込み
	LoadRanking();		//ロードランキング読み込む
	CRnkingLogo::Load();	// ランキングロゴの読み込み
	CUiPolygon::Load();
	int nScore;

	if (pMode == CManager::MODE_RESULT)
	{	// リザルトからランキング
		CPress::Load();		// プレスボタンの読み込み

							// ランキング裏の看板
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			CUiPolygon::Create(D3DXVECTOR3(RANK_BOARD_POS_X, RANK_BOARD_POS_Y - (nCntRanking * RANK_BOARD_POS_Y_INTERVAL), 0.0f),
				D3DXVECTOR3(RANK_BOARD_SIZE_X, RANK_BOARD_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);
		}

		// ランキング
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			nScore = m_nScore[nCntRanking];

			if (nCntRanking == 0)
			{	// カウントが0の場合
				m_nSize = 0;
			}
			else if (nCntRanking == 1 || nCntRanking == 2)
			{	// カウントが１の場合
				m_nSize = 1;
			}
			else
			{	// カウントが２の場合
				m_nSize = 2;
			}

			for (int nCntRanking001 = 0; nCntRanking001 < MAX_SCORE; nCntRanking001++)
			{
				m_aRanking[nCntRanking][nCntRanking001] = new CNumber2D;
				m_aRanking[nCntRanking][nCntRanking001]->Init(D3DXVECTOR3(pos.x - (nCntRanking001 * POS_X_INTERVAL), pos.y + (nCntRanking * POS_Y_INTERVAL), 0.0f),
					D3DXVECTOR3(SIZE_X - (RANK_SIZE_X_SCAlE * m_nSize), SIZE_Y - (RANK_SIZE_Y_SCAlE * m_nSize), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_aRanking[nCntRanking][nCntRanking001]->SetTex((nScore % 1), 1, D3DXVECTOR2((nScore * 0.1f), 1.0f));
				//ランキング画面で反映させる
				nScore /= 10;
			}
		}
		// ロゴの生成
		CRnkingLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), 0);
		CRnkingLogo::Create(D3DXVECTOR3(330.0f, 260.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X, RANK_SIZE_Y, 0.0f), 1);
		CRnkingLogo::Create(D3DXVECTOR3(330.0f, 355.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X - (RANK_SIZE_X_SCAlE * 2), RANK_SIZE_Y - (RANK_SIZE_Y_SCAlE * 1), 0.0f), 2);
		CRnkingLogo::Create(D3DXVECTOR3(330.0f, 450.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X - (RANK_SIZE_X_SCAlE * 2), RANK_SIZE_Y - (RANK_SIZE_Y_SCAlE * 1), 0.0f), 3);
		CRnkingLogo::Create(D3DXVECTOR3(330.0f, 540.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X - (RANK_SIZE_X_SCAlE * 3), RANK_SIZE_Y - (RANK_SIZE_Y_SCAlE * 2), 0.0f), 4);
		CRnkingLogo::Create(D3DXVECTOR3(330.0f, 640.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X - (RANK_SIZE_X_SCAlE * 3), RANK_SIZE_Y - (RANK_SIZE_Y_SCAlE * 2), 0.0f), 5);


		// プレスボタンの生成
		CPress::Create(D3DXVECTOR3(1100.0f, 680.0f, 0.0f), D3DXVECTOR3(250.0f, 130.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 0, 0.0f);

		m_fColA = 1.0f;
	}

	if (pMode == CManager::MODE_TITLE)
	{	// タイトル内のランキング表示

		// ランキング裏の看板
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			m_apUi[nCntRanking] = CUiPolygon::Create(D3DXVECTOR3(RANK_BOARD_POS_X, RANK_BOARD_POS_Y - (nCntRanking * RANK_BOARD_POS_Y_INTERVAL), 0.0f),
				D3DXVECTOR3(RANK_BOARD_SIZE_X, RANK_BOARD_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);
		}

		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			nScore = m_nScore[nCntRanking];

			if (nCntRanking == 0)
			{	// カウントが0の場合
				m_nSize = 0;
			}
			else if (nCntRanking == 1 || nCntRanking == 2)
			{	// カウントが１の場合
				m_nSize = 1;
			}
			else
			{	// カウントが２の場合
				m_nSize = 2;
			}

			for (int nCntRanking001 = 0; nCntRanking001 < MAX_SCORE; nCntRanking001++)
			{
				m_aRanking[nCntRanking][nCntRanking001] = new CNumber2D;
				m_aRanking[nCntRanking][nCntRanking001]->Init(D3DXVECTOR3(pos.x - (nCntRanking001 * POS_X_INTERVAL), pos.y + (nCntRanking * POS_Y_INTERVAL), 0.0f),
					D3DXVECTOR3(SIZE_X - (RANK_SIZE_X_SCAlE * m_nSize), SIZE_Y - (RANK_SIZE_Y_SCAlE * m_nSize), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_aRanking[nCntRanking][nCntRanking001]->SetTex((nScore % 1), 1, D3DXVECTOR2((nScore * 0.1f), 1.0f));
				//ランキング画面で反映させる
				nScore /= 10;
			}
		}

		// ロゴの生成
		m_pLogo[0] = CRnkingLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), 0);
		m_pLogo[1] = CRnkingLogo::Create(D3DXVECTOR3(330.0f, 260.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X, RANK_SIZE_Y, 0.0f), 1);
		m_pLogo[2] = CRnkingLogo::Create(D3DXVECTOR3(330.0f, 355.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X - (RANK_SIZE_X_SCAlE * 2), RANK_SIZE_Y - (RANK_SIZE_Y_SCAlE * 1), 0.0f), 2);
		m_pLogo[3] = CRnkingLogo::Create(D3DXVECTOR3(330.0f, 450.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X - (RANK_SIZE_X_SCAlE * 2), RANK_SIZE_Y - (RANK_SIZE_Y_SCAlE * 1), 0.0f), 3);
		m_pLogo[4] = CRnkingLogo::Create(D3DXVECTOR3(330.0f, 540.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X - (RANK_SIZE_X_SCAlE * 3), RANK_SIZE_Y - (RANK_SIZE_Y_SCAlE * 2), 0.0f), 4);
		m_pLogo[5] = CRnkingLogo::Create(D3DXVECTOR3(330.0f, 640.0f, 0.0f), D3DXVECTOR3(RANK_SIZE_X - (RANK_SIZE_X_SCAlE * 3), RANK_SIZE_Y - (RANK_SIZE_Y_SCAlE * 2), 0.0f), 5);

		// 変数の初期化
		m_Pattern = 0;
		m_fColA = 0.0f;
	}

	// 変数の初期化
	m_nTime = 0;


	return S_OK;
}

//*****************************************************************************
//		終了処理
//*****************************************************************************
void CRanking::Uninit()
{
	CManager::MODE pMode = CManager::GetMode();
	CNumber::Unload();
	CRnkingLogo::Unload();
	if (pMode == CManager::MODE_RANKING)
	{
		CPress::Unload();
	}
	CUiPolygon::Unload();

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntRanking001 = 0; nCntRanking001 < MAX_SCORE; nCntRanking001++)
		{
			if (m_aRanking[nCntRanking][nCntRanking001] != NULL)
			{
				//CScene2Dの終了
				m_aRanking[nCntRanking][nCntRanking001]->Uninit();
				m_aRanking[nCntRanking][nCntRanking001] = NULL;
			}
		}
	}

	if (pMode == CManager::MODE_TITLE)
	{	// タイトル状態のみ
		for (int nCntLogo = 0; nCntLogo < MAX_RANK_LOGO; nCntLogo++)
		{
			if (m_pLogo[nCntLogo] != NULL)
			{	// CScene2Dの終了へ
				m_pLogo[nCntLogo]->Uninit();
				m_pLogo[nCntLogo] = NULL;
			}
		}

		for (int nCntLogo = 0; nCntLogo < MAX_RANKING; nCntLogo++)
		{
			if (m_apUi[nCntLogo] != NULL)
			{	// CScene2Dの終了へ
				m_apUi[nCntLogo]->Uninit();
				m_apUi[nCntLogo] = NULL;
			}
		}
	}


	if (pMode == CManager::MODE_RANKING)
	{
		CSound *pSound = CManager::GetSound();
		if (pSound != NULL) pSound->StopSound(CSound::SOUND_LABEL_BGM004);
	
		CScene::ReleaseMode();
	}
}

//*****************************************************************************
//		更新処理
//*****************************************************************************
void CRanking::Update()
{
	CSound *pSound = CManager::GetSound();
	CManager::MODE pMode = CManager::GetMode();

	if (pMode == CManager::MODE_RANKING)
	{	// ランキング状態なら
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードしていない状態
			if (CManager::GetInputKeyboard()->GetALL(1) || CManager::GetInputXPad()->GetALL(1, 0) == true)
			{// キーボード（ENTER）を入力したら && 回数が3回なら
				CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
			}
		}

		m_nTime++;		// カウンターの増加
		if (m_nTime == TIME)
		{	// 時間経過後
			if (CFade::GetFade() == CFade::FADE_NONE)
			{	// フェードしていない状態
				CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);	// タイトルへ
			}
		}

		m_fColA -= 0.2f;		//　色の変化

		if (m_fColA <= 0.0f)
		{	// 0.0fになったら
			m_fColA = 1.0f;		// 1.0fに戻す
		}
		for (int nCntRanking001 = 0; nCntRanking001 < MAX_SCORE; nCntRanking001++)
		{
			if (m_aRanking[m_nNum][nCntRanking001] != NULL && m_nNum <= MAX_RANKING - 1)
			{	// ランクインしている場合
				m_aRanking[m_nNum][nCntRanking001]->Setcol(D3DXCOLOR(m_fColA, m_fColA, m_fColA, m_fColA));
			}
		}

	}

	if (pMode == CManager::MODE_TITLE)
	{	// タイトルの場合
		if (m_Pattern == 0)
		{	// プラグが０の場合
			m_fColA += 0.02f;
			if (m_fColA >= 1.0f)
			{	// 透明度が1.0f以上なら
				m_fColA = 1.0f;
				m_nTime++;		// カウンターの増加

				if (m_nTime >= TIME)
				{	// 指定の時間になったら
					m_Pattern = 2;
				}
			}
		}
		else if (m_Pattern == 2)
		{	// フラグが２の場合
			m_fColA -= 0.02f;
			if (m_fColA <= 0.0f)
			{	// 透明度が0.0f以下なら
				m_Pattern = 1;
			}
		}

		// ランキングの表示(色の変化)
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			for (int nCntRanking001 = 0; nCntRanking001 < MAX_SCORE; nCntRanking001++)
			{
				if (m_aRanking[nCntRanking][nCntRanking001] != NULL)
				{
					m_aRanking[nCntRanking][nCntRanking001]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));
				}
			}
		}

		// ロゴの表示(色の変化)
		for (int nCntLogo = 0; nCntLogo < MAX_RANK_LOGO; nCntLogo++)
		{
			if (m_pLogo[nCntLogo] != NULL)
			{
				m_pLogo[nCntLogo]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));
			}
		}

		for (int nCntLogo = 0; nCntLogo < MAX_RANK_LOGO; nCntLogo++)
		{
			if (m_apUi[nCntLogo] != NULL)
			{
				m_apUi[nCntLogo]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));
			}
		}


	}
}

//*****************************************************************************
//			描画処理
//*****************************************************************************
void CRanking::Draw()
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntRanking001 = 0; nCntRanking001 < MAX_SCORE; nCntRanking001++)
		{
			if (m_aRanking[nCntRanking][nCntRanking001] == NULL)
			{	// NULLなら
				if (m_aRanking[nCntRanking][nCntRanking001] != NULL)
				{	// NULLじゃないなら
					m_aRanking[nCntRanking][nCntRanking001]->Draw();

				}
			}
		}
	}
}

//*****************************************************************************
//			セーブデータ処理
//*****************************************************************************
void CRanking::SaveRanking(int nAdd)
{
	CScore *pSocre = NULL;		// ポインタ生成

	int nData;		// データ入れ替え用
	int nScore;		// 現在のスコア保管
	m_nNum = MAX_RANKING;

	nScore = pSocre->GetScore() + nAdd;

	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{
		if (m_nScore[MAX_RANKING - 1] <= nScore)
		{ // ランクインするなら
			m_nNum -= 1;
			m_nScore[MAX_RANKING - 1] = nScore;
		}

		for (int nCount001 = 0; nCount001 < MAX_RANKING - 1; nCount001++)
		{
			for (int nCount002 = nCount001 + 1; nCount002 < MAX_RANKING; nCount002++)
			{
				if (m_nScore[nCount002] > m_nScore[nCount001])
				{	// 順位の入れ替え
					nData = m_nScore[nCount001];
					m_nScore[nCount001] = m_nScore[nCount002];
					m_nScore[nCount002] = nData;
					m_nNum--;
				}
			}
		}
		break;
	}

	// ファイルの書き込み
	FILE *pFile;

	pFile = fopen("data/TEXT/ranking.bin", "wb");

	if (pFile != NULL)
	{
		fwrite(m_aRanking, sizeof(int), MAX_RANKING, pFile);

		fclose(pFile);
	}
}

//*****************************************************************************
//			読み込み処理
//*****************************************************************************
void CRanking::LoadRanking()
{
	// 読み込み
	FILE *pFile;

	pFile = fopen("data/TEXT/ranking.bin", "rb");

	if (pFile != NULL)
	{
		fread(m_aRanking, sizeof(int), MAX_RANKING, pFile);

		fclose(pFile);
	}
}