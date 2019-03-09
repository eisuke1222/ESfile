//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "game.h"			// ゲーム
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "fade.h"			// フェード
#include "meshfield.h"		// メッシュフィールド
#include "meshriver.h"		// メッシュリバー
#include "player.h"			// プレイヤー
#include "block.h"			// ブロック
#include "item.h"			// アイテム
#include "minimap.h"		// ミニマップ
#include "time.h"			// タイム
#include "bg.h"				// ゲーム背景
#include "ui.h"				// ui
#include "life.h"			// ライフ
#include "score.h"			// スコア
#include "ranking.h"		// ランキング
#include "effect.h"			// エフェクト
#include "item.h"			// アイテム
#include "itemicon.h"		// アイテムアイコン
#include "enemy.h"			// 敵
#include "3dobject.h"		// 3D背景オブジェクト
#include "wall.h"			// 壁
#include "start.h"			// スタート
#include "pause.h"			// ポーズ

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SOUND_SECOND	(120)		// 秒の最大
#define SOUND_SECOND		(120)		// 秒の最大から足す秒

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CPlayer *CGame::m_pPlayer = NULL;								// プレイヤー
CEnemy *CGame::m_pEnemy = NULL;									// 敵
CMeshField *CGame::m_pMeshField[m_MeshFieldMax] = { NULL };		// メッシュフィールド
CMeshRiver *CGame::m_pMeshRiver[m_MeshRiverMax] = { NULL };		// メッシュリバー
CGame::MODEGAME CGame::m_game = MODE_NONE;						// ゲーム
CPause *CGame::m_pPause = NULL;
bool CGame::m_bPause = false;

//=============================================================================
// コンストラクタ									(public)	*** CGame ***
//=============================================================================
CGame::CGame()
{
	for (int nCountb = 0; nCountb < TEXT_OBJ_MAX; nCountb++)
	{//	テキストのオブジェクトの種類
		m_textState[nCountb].nCnter = NULL;
		for (int nCount = 0; nCount < OBJ_MAX; nCount++)
		{//	テキストの中身の初期化
			m_textState[nCountb].nType[nCount] = NULL;			// 種類
			m_textState[nCountb].ncolType[nCount] = NULL;		// 衝突種類
			m_textState[nCountb].pos[nCount] = {};				// 位置
			m_textState[nCountb].size[nCount] = {};				// 大きさ
			m_textState[nCountb].bUse[nCount] = false;			// 使っているか
		}
	}
}

//=============================================================================
// デストラクタ										(public)	*** CGame ***
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CGame::Load(void)
{
	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CGame::Unload(void)
{

}

//=============================================================================
// 初期化処理										(public)	*** CGame ***
//=============================================================================
HRESULT CGame::Init(void)
{
	ObjectLoad();					//	アイテムのテキスト読み込み

	//	エフェクトのテキストの名前の読み込み
	CEffect::LoadNameEffect();
	CEffect::Load();

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

	CMeshRiver::Load();			// メッシュリバー
	m_pMeshRiver[0] = CMeshRiver::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(12.0f, 0.0f, 15.0f), D3DXVECTOR3(2320, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(240.0f, 0.0f, 300.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.9f), D3DXVECTOR2(0.5f, 0.5f), CSceneMesh::MESHTEX_NUMSIZE, CMeshRiver::TEXTURE_DORO);

	m_pMeshRiver[1] = CMeshRiver::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(14.0f, 0.0f, 15.0f), D3DXVECTOR3(8240, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(280.0f, 0.0f, 300.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.9f), D3DXVECTOR2(0.5f, 0.5f), CSceneMesh::MESHTEX_NUMSIZE, CMeshRiver::TEXTURE_DORO);

	m_pMeshRiver[2] = CMeshRiver::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(21.0f, 0.0f, 15.0f), D3DXVECTOR3(10500, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(420.0f, 0.0f, 300.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.9f), D3DXVECTOR2(0.5f, 0.5f), CSceneMesh::MESHTEX_NUMSIZE, CMeshRiver::TEXTURE_DORO);

	m_pMeshRiver[3] = CMeshRiver::Create(CSceneMesh::MESHTYPE_FIELD, D3DXVECTOR3(16.0f, 0.0f, 15.0f), D3DXVECTOR3(12580, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(320.0f, 0.0f, 300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(0.5f, 0.5f), CSceneMesh::MESHTEX_NUMSIZE, CMeshRiver::TEXTURE_RIVER);

	CPlayer::Load();			// プレイヤー
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-400.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

	CEnemy::Load();				//	敵
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-600.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	CStart::Load();

	CItem::Load();
	CItemicon::Load();

	CBlock::Load();

	C3Dobject::Load();

	CBg::Load();				// ゲーム背景
	CBg::Create();

	CUiPolygon::Load();			// UI
	CUiPolygon::Create(D3DXVECTOR3(150.0f, 70.0f, 0.0f), D3DXVECTOR3(100.0f, 70.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);		// 枠(体力)
	CUiPolygon::Create(D3DXVECTOR3(950.0f, 70.0f, 0.0f), D3DXVECTOR3(120.0f, 70.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);		// 枠(スコア)
	CUiPolygon::Create(D3DXVECTOR3(1182.0f, 70.0f, 0.0f), D3DXVECTOR3(98.0f, 70.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);		// 枠(タイム)
	CUiPolygon::Create(D3DXVECTOR3(560.0f, 70.0f, 0.0f), D3DXVECTOR3(280.0f, 70.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);		// 枠(ミニマップ)
	CUiPolygon::Create(D3DXVECTOR3(570.0f, 70.0f, 0.0f), D3DXVECTOR3(250.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1);		// ミニマップ
	CUiPolygon::Create(D3DXVECTOR3(1125.0f, 68.0f, 0.0f), D3DXVECTOR3(21.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 3);		// 時間
	CUiPolygon::Create(D3DXVECTOR3(100.0f, 70.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4);		// プレイヤー
	CUiPolygon::Create(D3DXVECTOR3(1196.0f, 67.0f, 0.0f), D3DXVECTOR3(17.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0);		// 時間の間の点

	CMinimap::Load();			// ミニマップのプレイヤー & 巨人
	CMinimap::Create(D3DXVECTOR3(330.0f, 70.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CMinimap::TYPE_ENEMY, 1);
	CMinimap::Create(D3DXVECTOR3(330.0f, 70.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CMinimap::TYPE_PLAYER, 0);

	CTime::Create();			// タイム

	CScore::Create();			// スコア

	CLife::Load();				// 体力
	CLife::Create();			

	CPause::Load();				// ポーズ

	m_game = MODE_NONE;
	m_bPause = false;

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CGame ***
//=============================================================================
void CGame::Uninit(void)
{
	int nTimeScore = 0;		// タイムスコア変換用

	if (m_game == MODE_GAMECLEAR)
	{// ゲームクリアのみ
		nTimeScore = CTime::GetTimeNum();	// タイム取得
		nTimeScore -= 35;					// タイム調整

		if (nTimeScore < 0)
		{// 0未満の場合
			nTimeScore = 0;		// タイム調整
		}
		else
		{// ０以上
			nTimeScore = 2100 - ((nTimeScore) * 80);	// スコア変換
			if (nTimeScore < 0) nTimeScore = 0;			// スコア調整
		}
	}

	CRanking::SaveRanking(nTimeScore);	// ランキングセーブ
	CBg::Unload();				// 背景
	CUiPolygon::Unload();		// UI
	CMeshField::Unload();		// メッシュフィールド
	CPlayer::Unload();			// プレイヤー
	CMinimap::Unload();			// ミニマップ
	CItem::Unload();			// アイテム
	CItemicon::Unload();		// アイテムアイコン
	CEffect::Unload();			// エフェクト
	CEnemy::Unload();			// 敵
	CBlock::Unload();			// ブロック
	C3Dobject::Unload();		// ３Dオブジェクト
	CWall::Unload();			// 壁
	CMeshRiver::Unload();		// メッシュリバー
	CStart::Unload();			// スタート
	CLife::Unload();			// 体力
	CPause::Unload();			// ポーズ

	for (int nCount = 0; nCount < m_MeshFieldMax; nCount++)
	{// カウント
		if (m_pMeshField[nCount] != NULL)
		{// NULL以外の場合
			m_pMeshField[nCount]->Uninit();		// 終了処理
			m_pMeshField[nCount] = NULL;		// NULLへ
		}
	}

	if (m_pPlayer != NULL)
	{// NULL以外の場合
		m_pPlayer->Uninit();			// 終了処理
		m_pPlayer = NULL;				// NULLへ
	}

	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}

	CScene::ReleaseMode();	// モード終了
}

//=============================================================================
// 更新処理											(public)	*** CGame ***
//=============================================================================
void CGame::Update(void)
{
	CSound *pSound = CManager::GetSound();

	if (CFade::GetFade() == CFade::FADE_NONE && m_game == MODE_NONE)
	{// フェードしていない状態
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_START, 0) == true)
		{//キーボード（P）を押した
			m_bPause = m_bPause ? false : true;		// 使用切り替え

			if (m_bPause == true)
			{// 使用する
				if (m_pPause == NULL)
				{// NULLの場合
					// ポーズ生成
					m_pPause = new CPause;
					m_pPause->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, 170.0f, 0.0f), D3DXVECTOR3(150.0f, 70.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				}
				// 更新範囲をポーズから
				CScene::SetStartUp(CScene::PRIORITY_PAUSE);
			}
			else
			{// その他
				if (m_pPause != NULL)
				{// NULL以外の場合
					m_pPause->Uninit();		// 終了処理
					delete m_pPause;		// メモリ開放
					m_pPause = NULL;		// NULLへ
				}
				CScene::SetStartUp(0);
			}

			pSound->PlaySoundA(CSound::SOUND_LABEL_SE000);
		}
	}

	if (m_pPause != NULL)
	{// NULL以外の場合
		m_pPause->Update();		// ポーズ更新
	}

	SetStage();		// ステージ設定

#ifdef _DEBUG		// デバッグ
	if (CFade::GetFade() == CFade::FADE_NONE)
	{// フェードしていない状態
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
		{// キーボード（ENTER）を入力したら
			m_game = MODE_GAMECLEAR;
			CFade::Create(CManager::MODE_RESULT, CFade::FADE_OUT);
		}
	}
#endif // _DEBUG

	int nRand = rand() % SOUND_SECOND + MAX_SOUND_SECOND;
	m_nCounter++;		// カウンターの増加

	if ((m_nCounter % nRand) == 0)
	{	// 環境音番号を決める
		//m_nSoundNum = rand() % 6 + 0;
		PlayES(rand() % 6 + 0);
	}

}

//=============================================================================
// 描画処理											(public)	*** CGame ***
//=============================================================================
void CGame::Draw(void)
{
	if (m_pPause != NULL)
	{// NULL以外の場合
		m_pPause->Draw();	// ポーズ描画
	}
}

//=============================================================================
// テキスト読み込み
//=============================================================================
void CGame::ObjectLoad(void)
{
	FILE *pFile;
	char read[128];
	int nObjTypeCounter = 0;		//	オブジェクトの種類の更新

									//	読み込むテキスト
	pFile = fopen("data/TEXT/object.txt", "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		if (strcmp(&read[0], "ITEM_MAX") == 0)
		{//	アイテムの必要な数の取得
			fscanf(pFile, "%s%d", &read[0], &m_textState[0].nCnter);
		}
		fscanf(pFile, "%s", &read[0]);
		if (strcmp(&read[0], "BLOCK_MAX") == 0)
		{//	ブロックの必要な数の取得
			fscanf(pFile, "%s%d", &read[0], &m_textState[1].nCnter);
		}
		//---------------------------------------------------------------
		//	アイテムの設定のテキスト上コメントアウト
		//---------------------------------------------------------------
		for (int nCount = 0; nCount < 6; nCount++) 	{ fscanf(pFile, "%s", &read[0]); if (strcmp(&read[0], "#") == 0) { do { fscanf(pFile, "%c", &read[0]); } while (strcmp(&read[0], "\n") != 0); } }
		for (int nCount = 0; nCount < m_textState[nObjTypeCounter].nCnter;)
		{
			fscanf(pFile, "%s", &read[0]);
			if (strcmp(&read[0], "ITEM_SET") == 0)
			{//	文字列がアイテムセットだったら
				do
				{
					fscanf(pFile, "%s", &read[0]);
					if (strcmp(&read[0], "POS") == 0)
					{//	位置の設定
						fscanf(pFile, "%s%f%f%f", &read[0], &m_textState[nObjTypeCounter].pos[nCount].x, &m_textState[nObjTypeCounter].pos[nCount].y, &m_textState[nObjTypeCounter].pos[nCount].z);
					}
					else if (strcmp(&read[0], "SIZE") == 0)
					{//	大きさの設定
						fscanf(pFile, "%s%f%f%f", &read[0], &m_textState[nObjTypeCounter].size[nCount].x, &m_textState[nObjTypeCounter].size[nCount].y, &m_textState[nObjTypeCounter].size[nCount].z);
					}
					else if (strcmp(&read[0], "TYPE") == 0)
					{//	種類の設定
						fscanf(pFile, "%s%d", &read[0], &m_textState[nObjTypeCounter].nType[nCount]);
					}
				} while (strcmp(&read[0], "ITEM_END") != 0);
				nCount++;
			}
		}
		//---------------------------------------------------------------
		//	ブロックの設定のテキスト上コメントアウト
		//---------------------------------------------------------------
		for (int nCount = 0; nCount < 31; nCount++) { fscanf(pFile, "%s", &read[0]); if (strcmp(&read[0], "#") == 0) { do { fscanf(pFile, "%c", &read[0]); } while (strcmp(&read[0], "\n") != 0); } }
		nObjTypeCounter += 1;	//	ブロックオブジェクト
		for (int nCount = 0; nCount < m_textState[nObjTypeCounter].nCnter;)
		{
			fscanf(pFile, "%s", &read[0]);
			if (strcmp(&read[0], "BLOCK_SET") == 0)
			{//	文字列がアイテムセットだったら
				do
				{
					fscanf(pFile, "%s", &read[0]);
					if (strcmp(&read[0], "POS") == 0)
					{//	位置の設定
						fscanf(pFile, "%s%f%f%f", &read[0], &m_textState[nObjTypeCounter].pos[nCount].x, &m_textState[nObjTypeCounter].pos[nCount].y, &m_textState[nObjTypeCounter].pos[nCount].z);
					}
					else if (strcmp(&read[0], "MODELTYPE") == 0)
					{//	種類の設定
						fscanf(pFile, "%s%d", &read[0], &m_textState[nObjTypeCounter].nType[nCount]);
					}
					else if (strcmp(&read[0], "COLTYPE") == 0)
					{//	種類の設定
						fscanf(pFile, "%s%d", &read[0], &m_textState[nObjTypeCounter].ncolType[nCount]);
					}
				} while (strcmp(&read[0], "BLOCK_END") != 0);
				nCount++;
			}
		}
		fclose(pFile);
	}
	FILE *pFile3DOBJ;
	//	読み込むテキスト
	pFile3DOBJ = fopen("data/TEXT/3dobject.txt", "r");
	if (pFile3DOBJ != NULL)
	{
		fscanf(pFile3DOBJ, "%s", &read[0]);
		if (strcmp(&read[0], "3DPORIGON_MAX") == 0)
		{//	ブロックの必要な数の取得
			fscanf(pFile3DOBJ, "%s%d", &read[0], &m_textState[2].nCnter);
		}
		//---------------------------------------------------------------
		//	背景オブジェクトの設定のテキスト上コメントアウト
		//---------------------------------------------------------------
		for (int nCount = 0; nCount < 11; nCount++) { fscanf(pFile, "%s", &read[0]); if (strcmp(&read[0], "#") == 0) { do { fscanf(pFile, "%c", &read[0]); } while (strcmp(&read[0], "\n") != 0); } }
		nObjTypeCounter += 1;	//	ブロックオブジェクト
		for (int nCount = 0; nCount < m_textState[nObjTypeCounter].nCnter;)
		{
			fscanf(pFile3DOBJ, "%s", &read[0]);
			if (strcmp(&read[0], "3DPORIGON_SET") == 0)
			{//	文字列がアイテムセットだったら
				do
				{
					fscanf(pFile3DOBJ, "%s", &read[0]);
					if (strcmp(&read[0], "POS") == 0)
					{//	位置の設定
						fscanf(pFile3DOBJ, "%s%f%f%f", &read[0], &m_textState[nObjTypeCounter].pos[nCount].x, &m_textState[nObjTypeCounter].pos[nCount].y, &m_textState[nObjTypeCounter].pos[nCount].z);
					}
					else if (strcmp(&read[0], "SIZE") == 0)
					{//	種類の設定
						fscanf(pFile3DOBJ, "%s%f%f%f", &read[0], &m_textState[nObjTypeCounter].size[nCount].x, &m_textState[nObjTypeCounter].size[nCount].y, &m_textState[nObjTypeCounter].size[nCount].z);
					}
					else if (strcmp(&read[0], "TYPE") == 0)
					{//	種類の設定
						fscanf(pFile3DOBJ, "%s%d", &read[0], &m_textState[nObjTypeCounter].nType[nCount]);
					}
				} while (strcmp(&read[0], "3DPORIGON_END") != 0);
				nCount++;
			}
		}
		fclose(pFile3DOBJ);
	}
}

void CGame::SetStage(void)
{
	// カメラ設定
	D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// 視点取得

	for (int nCount = 0; nCount < m_textState[0].nCnter; nCount++)
	{//	アイテムの設定
		if (m_textState[0].pos[nCount].x < posVDest.x + 1000.0f && m_textState[0].bUse[nCount] == false)
		{
			CItem::Create(m_textState[0].pos[nCount], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_textState[0].size[nCount], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), m_textState[0].nType[nCount]);
			m_textState[0].bUse[nCount] = true;
		}
	}

	for (int nCount = 0; nCount < m_textState[1].nCnter; nCount++)
	{//	ブロックの設定
		if (m_textState[1].pos[nCount].x < posVDest.x + 1000.0f && m_textState[1].bUse[nCount] == false)
		{
			CBlock::Create(m_textState[1].pos[nCount], (CBlock::MODELTYPE)m_textState[1].nType[nCount], (CBlock::COLTYPE)m_textState[1].ncolType[nCount]);
			m_textState[1].bUse[nCount] = true;
		}
	}

	for (int nCount = 0; nCount < m_textState[2].nCnter; nCount++)
	{//	ブロックの設定
		if (m_textState[2].pos[nCount].x < posVDest.x + 1000.0f && m_textState[2].bUse[nCount] == false)
		{
			C3Dobject::Create(m_textState[2].pos[nCount], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_textState[2].size[nCount], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), m_textState[2].nType[nCount]);
			m_textState[2].bUse[nCount] = true;
		}
	}
}

//=============================================================================
// 環境音処理											(public)	*** CGame ***
//=============================================================================
void CGame::PlayES(int nSoundNum)
{
	CSound *pSound = CManager::GetSound();
	if (nSoundNum == 0)
	{	// 風の音
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE022);
		nSoundNum = -1;
	}
	else if (nSoundNum == 1)
	{	// 強風の音
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE023);
		nSoundNum = -1;
	}
	else if (nSoundNum == 2)
	{	//	そよ風の音
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE024);
		nSoundNum = -1;
	}
	else if (nSoundNum == 3)
	{	//	鳥の音
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE034);
		nSoundNum = -1;
	}
	else if (nSoundNum == 4)
	{	//	鳥2の音
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE035);
		nSoundNum = -1;
	}
	else if (nSoundNum == 5)
	{	//	鳥3の音
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE036);
		nSoundNum = -1;
	}
	else if (nSoundNum == 6)
	{	//	虫の音
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE037);
		nSoundNum = -1;
	}
}

//=============================================================================
// ポーズ設定											(public)	*** CGame ***
//=============================================================================
void CGame::SetPause(bool bPause)
{
	m_bPause = bPause;		// ポーズ切り替え

	if (m_bPause == true)
	{// 使用する
		if (m_pPause == NULL)
		{// NULLの場合
			// ポーズ生成
			m_pPause = new CPause;	// メモリ確保
			m_pPause->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, 170.0f, 0.0f), D3DXVECTOR3(150.0f, 70.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
		}
		// 更新範囲をポーズから
		CScene::SetStartUp(CScene::PRIORITY_PAUSE);
	}
	else
	{// その他
		if (m_pPause != NULL)
		{// NULL以外の場合
			m_pPause->Uninit();		// 終了処理
			delete m_pPause;		// メモリ開放
			m_pPause = NULL;		// NULLへ
		}
		CScene::SetStartUp(0);
	}
}