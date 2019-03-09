//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : Shun Yokomichi
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "title.h"			// タイトル
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "fade.h"			// フェード
#include "titlelogo.h"		// タイトルロゴ
#include "titleframe.h"		// タイトルフレーム
#include "press.h"			// プレスボタン
#include "ranking.h"		// ランキング
#include "camera.h"			// カメラ
#include "meshfield.h"		// メッシュフィールド
#include "bg.h"				// 背景
#include "block.h"
#include "wall.h"
#include "3dobject.h"		// 3D背景オブジェクト

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
CMeshField *CTitle::m_pMeshField[8] = {};
CRanking *CTitle::m_pRanking = NULL;

//=============================================================================
// コンストラクタ									(public)	*** CTitle ***
//=============================================================================
CTitle::CTitle()
{
	m_textState.nCnter = NULL;

	for (int nCount = 0; nCount < TITLE_OBJ_MAX; nCount++)
	{//	テキストの中身の初期化
		m_textState.nType[nCount] = NULL;
		m_textState.ncolType[nCount] = NULL;
		m_textState.pos[nCount] = {};
		m_textState.size[nCount] = {};
		m_textState.bUse[nCount] = false;
	}
}

//=============================================================================
// デストラクタ										(public)	*** CTitle ***
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ロード処理							(public)	*** CTitle ***
//=============================================================================
HRESULT CTitle::Load(void)
{
	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CTitle ***
//=============================================================================
void CTitle::Unload(void)
{

}

//=============================================================================
// 初期化処理										(public)	*** CTitle ***
//=============================================================================
HRESULT CTitle::Init(void)
{
	ObjectLoad();					//	アイテムのテキスト読み込み

	CTitleLogo::Load();			// タイトルロゴ
	CPress::Load();				// プレスボタン

	CTitleFrame::Load();		// タイトルフレーム

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	CTitleFrame::Create(pos, D3DXVECTOR3(95.0f, 54.0f, 0.0f), D3DX_PI, 1);
	CTitleFrame::Create(pos, D3DXVECTOR3(70.0f, 100.0f, 0.0f), (D3DX_PI * 0.5f), 0);
	CTitleFrame::Create(pos, D3DXVECTOR3(130.0f, 75.0f, 0.0f), 0.0f, 0);
	CTitleFrame::Create(pos, D3DXVECTOR3(100.0f, 145.0f, 0.0f), (D3DX_PI * -0.5f), 1);
	CTitleFrame::Create(pos, D3DXVECTOR3(185.0f, 110.0f, 0.0f), D3DX_PI, 0);
	CTitleFrame::Create(pos, D3DXVECTOR3(135.0f, 200.0f, 0.0f), (D3DX_PI * 0.5f), 1);
	CTitleFrame::Create(pos, D3DXVECTOR3(255.0f, 140.0f, 0.0f), 0.0f, 1);
	CTitleFrame::Create(pos, D3DXVECTOR3(180.0f, 270.0f, 0.0f), (D3DX_PI * -0.5f), 0);
	CTitleFrame::Create(pos, D3DXVECTOR3(330.0f, 210.0f, 0.0f), D3DX_PI, 1);
	CTitleFrame::Create(pos, D3DXVECTOR3(222.0f, 350.0f, 0.0f), (D3DX_PI * 0.5f), 0);
	CTitleFrame::Create(pos, D3DXVECTOR3(440.0f, 230.0f, 0.0f), 0.0f, 0);
	CTitleFrame::Create(pos, D3DXVECTOR3(290.0f, 450.0f, 0.0f), (D3DX_PI * -0.5f), 1);
	CTitleFrame::Create(pos, D3DXVECTOR3(550.0f, 300.0f, 0.0f), D3DX_PI, 0);
	CTitleFrame::Create(pos, D3DXVECTOR3(370.0f, 560.0f, 0.0f), (D3DX_PI * 0.5f), 0);
	CTitleFrame::Create(pos, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 0.0f, 0);

	CCamera *pCamera = CManager::GetCamera();		// カメラ取得
	pCamera->SetPos(D3DXVECTOR3(50.0f, 230.0f, -500.0f), D3DXVECTOR3(50.0f, 170.0f, 0.0f));

	CWall::Load();
	CWall::Create(D3DXVECTOR3(3430.0f, -150.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 150.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(4450.0f, -150.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 150.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(4925.0f, -150.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(25.0f, 150.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(6595.0f, -150.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(45.0f, 150.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(7615.0f, -150.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(55.0f, 150.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(11785.0f, -150.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(115.0f, 150.0f, 0.0f));

	CMeshField::Load();			// メッシュフィールド
	m_pMeshField[0] = CMeshField::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(110.0f, 0.0f, 15.0f), D3DXVECTOR3(1180.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(2200.0f, 0.0f, 300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE);

	m_pMeshField[1] = CMeshField::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(28.0f, 0.0f, 15.0f), D3DXVECTOR3(3940, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(460.0f, 0.0f, 300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE);

	m_pMeshField[2] = CMeshField::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(10.0f, 0.0f, 15.0f), D3DXVECTOR3(4700.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(200.0f, 0.0f, 300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE);

	m_pMeshField[3] = CMeshField::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(30.0f, 0.0f, 15.0f), D3DXVECTOR3(5750.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(800.0f, 0.0f, 300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE);

	m_pMeshField[4] = CMeshField::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(23.0f, 0.0f, 15.0f), D3DXVECTOR3(7100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(460.0f, 0.0f, 300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE);

	m_pMeshField[5] = CMeshField::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(100.0f, 0.0f, 15.0f), D3DXVECTOR3(9670.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(2000.0f, 0.0f, 300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE);

	m_pMeshField[6] = CMeshField::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(12.0f, 0.0f, 15.0f), D3DXVECTOR3(12020.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(240.0f, 0.0f, 300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE);

	m_pMeshField[7] = CMeshField::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(40.0f, 0.0f, 15.0f), D3DXVECTOR3(13700.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(800.0f, 0.0f, 300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE);

	//CBlock::Load();
	//for (int nCount = 0; nCount < m_textState.nCnter; nCount++)
	//{//	ブロックの設定
	//	CBlock::Create(m_textState.pos[nCount], (CBlock::MODELTYPE)m_textState.nType[nCount], (CBlock::COLTYPE)m_textState.ncolType[nCount]);
	//}

	C3Dobject::Load();

	CBg::Load();				// ゲーム背景
	CBg::Create();


	// 初期化
	m_nType = 1;
	m_fColA = 0.0f;
	m_nCounter = 0;
	m_type = TYPE_SCENEN001;	// タイトルフレームへ
	m_pPress = NULL;

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CTitle ***
//=============================================================================
void CTitle::Uninit(void)
{
	CMeshField::Unload();
	C3Dobject::Unload();
	CBg::Unload();
	CTitleLogo::Unload();
	CPress::Unload();
	CTitleFrame::Unload();
	CBlock::Unload();

	if (m_pRanking != NULL)
	{	// NULLじゃないなら
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;
	}
	for (int nCount = 0; nCount < 8; nCount++)
	{
		if (m_pMeshField[nCount] != NULL)
		{// NULL以外の場合
			m_pMeshField[nCount]->Uninit();
			m_pMeshField[nCount] = NULL;
		}
	}
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	CScene::ReleaseMode();	// モード終了
}

//=============================================================================
// 更新処理											(public)	*** CTitle ***
//=============================================================================
void CTitle::Update(void)
{
	CSound *pSound = CManager::GetSound();

	SetStage();

	switch (m_type)
	{
	case TYPE_SCENEN000:
		m_nCounter++;
		if ((m_nCounter % 60) == 0)
		{	// 1秒経過後
			m_nType = 1;
			m_type = TYPE_SCENEN001;  // タイトル表示へ
		}
		break;

	case TYPE_SCENEN001:
		if (m_nType == 1)
		{	// タイトルロゴの生成
			CTitleLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTitleLogo::TYPE_LOGO, 0);
			m_nType = 0;		// フラグを0に
		}

		if (int nPattern = CTitleLogo::GetPattern() == 1)
		{	// プレスボタンの生成
			if (m_pPress == NULL)
			{
				m_pPress = new CPress;
				m_pPress->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f), D3DXVECTOR3(550.0f, 200.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 0, 0.0f);
			}
			nPattern = 2;		// フラグを2へ
			CTitleLogo::SetPattern(nPattern);
		}
		else if (int nPattern = CTitleLogo::GetPattern() == 3)
		{	// フラグが3なら
			m_nType = 1;				// フラグの1に
			m_type = TYPE_SCENEN002;	//　ランキング表示へ
			if (m_pPress != NULL)
			{
				m_pPress->Uninit();
				m_pPress = NULL;
			}
		}

		break;

	case TYPE_SCENEN002:
		if (m_nType == 1)
		{	// フラグが１の場合
			if (m_pRanking == NULL)
			{
				m_pRanking = new CRanking;
				if (m_pRanking != NULL)
				{	// ランキングのInitへ
					m_pRanking->Init(D3DXVECTOR3(1000.0f, 260.0f, 0.0f));
				}
			}
			if (m_pPress == NULL)
			{
				m_pPress = new CPress;
				m_pPress->Init(D3DXVECTOR3(1100.0f, 680.0f, 0.0f), D3DXVECTOR3(250.0f, 130.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 0, 0.0f);
			}
			m_nType = 0;		// フラグを０に
		}

		if (m_pRanking != NULL)
		{	// NULLじゃないなら
			m_pRanking->Update();
		}
		if (int nPattern = CRanking::GetPattern() == 1)
		{	// フラグが1なら
			m_nType = 1;				// フラグを１に
			m_type = TYPE_SCENE003;		// タイトルロゴへ
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
		}

		break;

	case TYPE_SCENE003:
		if (m_nType == 1)
		{	// タイトルロゴの生成
			CTitleLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), D3DXVECTOR3(600.0f, 300.0f, 0.0f), CTitleLogo::TYPE_CREDIT, 1);
			m_nType = 0;		// フラグを0に
		}

		if (int nPattern = CTitleLogo::GetPattern() == 1)
		{	// フラグが3なら
			m_nType = 1;				// フラグの1に
			m_type = TYPE_SCENEN001;	// ランキング表示へ

			if (m_pPress != NULL)
			{
				m_pPress->Uninit();
				m_pPress = NULL;
			}
		}
		break;
	}

	if (CFade::GetFade() == CFade::FADE_NONE)
	{// フェードしていない状態
		if (CManager::GetInputKeyboard()->GetALL(1) == true || CManager::GetInputXPad()->GetALL(1, 0) == true)
		{// キーボード（ENTER）を入力したら
			CFade::Create(CManager::MODE_TUTORIAL, CFade::FADE_OUT);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CTitle ***
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// テキスト読み込み
//=============================================================================
void CTitle::ObjectLoad(void)
{
	FILE *pFile;
	char read[128];
									//	読み込むテキスト
	pFile = fopen("data/TEXT/3dobject.txt", "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		if (strcmp(&read[0], "3DPORIGON_MAX") == 0)
		{//	ブロックの必要な数の取得
			fscanf(pFile, "%s%d", &read[0], &m_textState.nCnter);
		}

		//---------------------------------------------------------------
		//	ブロックの設定のテキスト上コメントアウト
		//---------------------------------------------------------------
		for (int nCount = 0; nCount < 12; nCount++) { fscanf(pFile, "%s", &read[0]); if (strcmp(&read[0], "#") == 0) { do { fscanf(pFile, "%c", &read[0]); } while (strcmp(&read[0], "\n") != 0); } }
		for (int nCount = 0; nCount < m_textState.nCnter; nCount++)
		{
			fscanf(pFile, "%s", &read[0]);
			if (strcmp(&read[0], "3DPORIGON_SET") == 0)
			{//	文字列がアイテムセットだったら
				do
				{
					fscanf(pFile, "%s", &read[0]);
					if (strcmp(&read[0], "POS") == 0)
					{//	位置の設定
						fscanf(pFile, "%s%f%f%f", &read[0], &m_textState.pos[nCount].x, &m_textState.pos[nCount].y, &m_textState.pos[nCount].z);
					}
					else if (strcmp(&read[0], "SIZE") == 0)
					{//	種類の設定
						fscanf(pFile, "%s%f%f%f", &read[0], &m_textState.size[nCount].x, &m_textState.size[nCount].y, &m_textState.size[nCount].z);
					}
					else if (strcmp(&read[0], "TYPE") == 0)
					{//	種類の設定
						fscanf(pFile, "%s%d", &read[0], &m_textState.nType[nCount]);
					}
				} while (strcmp(&read[0], "3DPORIGON_END") != 0);
			}
		}
		fclose(pFile);
	}
}

void CTitle::SetStage(void)
{
	// カメラ設定
	D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// 視点取得

	for (int nCount = 0; nCount < m_textState.nCnter; nCount++)
	{//	ブロックの設定
		if (m_textState.pos[nCount].x < posVDest.x + 1000.0f && m_textState.bUse[nCount] == false)
		{
			C3Dobject::Create(m_textState.pos[nCount], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_textState.size[nCount], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), m_textState.nType[nCount]);
			m_textState.bUse[nCount] = true;
		}
	}
}