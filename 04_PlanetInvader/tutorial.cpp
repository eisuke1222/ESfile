//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "tutorial.h"		// チュートリアル
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "scene2d.h"		// シーン2D
#include "fade.h"			// フェード
#include "camera.h"			// カメラ
#include "meshfield.h"		// メッシュフィールド
#include "object3d.h"		// オブジェクト3D
#include "effect3d.h"		// エフェクト3D
#include "bg.h"				// 背景
#include "enemy.h"			// 敵
#include "player.h"			// プレイヤー
#include "bullet.h"			// 弾
#include "effect.h"			// エフェクト
#include "explosion.h"		// 爆発
#include "cloud.h"			// 雲
#include "number.h"			// 数字
#include "bomb.h"			// 爆弾
#include "gauge.h"			// ゲージ
#include "life.h"			// 体力
#include "score.h"			// スコア
#include "pause.h"			// ポーズ

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\playerNum.png"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CTutorial::m_pTexture				= NULL;		// テクスチャ情報へのポインタ
CScene2D			*CTutorial::m_pScene2D[MAX_PLAYER]	= {};		// シーン2D
CPause				*CTutorial::m_pPause			= NULL;			// ポーズ
bool				CTutorial::m_bPause				= false;		// ポーズ使用

//=============================================================================
// コンストラクタ									(public)	*** CTutorial ***
//=============================================================================
CTutorial::CTutorial()
{
	// 初期設定
	m_bPause	= false;		// ポーズ使用
	m_mapfield = 0;				// フィールド番号
	m_mapfieldOld	= 0;		// フィールド番号
}

//=============================================================================
// デストラクタ										(public)	*** CTutorial ***
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// ロード処理							(public)	*** CTutorial ***
//=============================================================================
HRESULT CTutorial::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイス取得

	if (m_pTexture == NULL)
	{// NULLの場合
	 // テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CTutorial ***
//=============================================================================
void CTutorial::Unload(void)
{
	if (m_pTexture != NULL)
	{// NULL以外の場合
		m_pTexture->Release();					// 解放
		m_pTexture = NULL;						// NULLへ
	}
}

//=============================================================================
// 初期化処理										(public)	*** CTutorial ***
//=============================================================================
HRESULT CTutorial::Init(void)
{
	CCamera *pCamera = CManager::GetCamera();		// カメラ取得
	pCamera->SetPos(D3DXVECTOR3(0.0f, 750.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// カメラ初期設定

#ifdef _DEBUG
	CManager::SetPlayNum(CInputXPad::GetInputNum());
#endif // !1

	// 3Dロード
	CMeshField::Load();		// メッシュフィールド
	CObject3D::Load();		// モデル
	CEffect3D::Load();		// モデル
	CEnemy::Load();			// 敵
	CPlayer::Load();		// プレイヤー
	CBullet::Load();		// 弾
	CEffect::Load();		// エフェクト
	CExplosion::Load();		// 爆発
	CCloud::Load();			// 雲
	// 3Dクリエイト
	CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
		D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);		// メッシュフィールド
	CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
		D3DXVECTOR3(0.0f, -100.0f, 1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);	// メッシュフィールド

	// 2Dロード
	CBg::Load();			// 背景
	CLife::Load();			// 体力
	CBomb::Load();			// 爆弾
	CGauge::Load();			// ゲージ
	CNumber::Load();		// 数字
	// 2Dクリエイト
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, CBg::TEXTURE_TUTORIAL);	// 背景

	for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
	{
		CLife::Create(D3DXVECTOR3(50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 180, 0.0f), D3DXVECTOR3(LIFE_SIZE_X, LIFE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);					// 体力
		CBomb::Create(3, D3DXVECTOR3(50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 260, 0.0f), D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);				// 爆弾
		CGauge::Create(D3DXVECTOR3(100.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 340, 0.0f), D3DXVECTOR3(GAUGE_SIZE_X, GAUGE_SIZE_Y, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.1f, 1.0f), nCntCont);				// ゲージ
		CScore::Create(D3DXVECTOR3(SCREEN_WIDTH_0 - 50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 100.0f, 0.0f), D3DXVECTOR3(12.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);			// スコア
	}
	CPause::Load();			// ポーズ

	for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
	{
		if (m_pScene2D[nCnt] == NULL)
		{
			m_pScene2D[nCnt] = new CScene2D(CScene::PRIORITY_UI);
			m_pScene2D[nCnt]->SetInitAll(D3DXVECTOR3(0.0f + 60.0f + ((SCREEN_WIDTH - 60.0f - 60.0f) * nCnt), SCREEN_HEIGHT - 40.0f, 0.0f), D3DXVECTOR3(35.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
			m_pScene2D[nCnt]->Init();
			m_pScene2D[nCnt]->SetObjType(CScene::OBJTYPE_PLAYERNUM);
			m_pScene2D[nCnt]->BindTexture(m_pTexture);

			m_pScene2D[nCnt]->SetTex(1, nCnt, D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	// 初期設定
	m_pPause	= NULL;			// ポーズ
	m_bPause	= false;		// ポーズ使用
	m_mapfield	= 0;			// フィールド番号
	
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CTutorial ***
//=============================================================================
void CTutorial::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pScene2D[nCnt] != NULL)
		{
			m_pScene2D[nCnt]->Uninit();		// 終了処理
			m_pScene2D[nCnt] = NULL;		// NULLへ
		}
	}

	if (m_pPause != NULL)
	{// NULL以外の場合
		m_pPause->Uninit();	// 終了処理
		m_pPause = NULL;	// NULL
	}

	// 3Dアンロード
	CMeshField::Unload();	// メッシュフィールド
	CObject3D::Unload();	// モデル
	CEffect3D::Unload();	// モデル
	CEnemy::Unload();		// 敵
	CPlayer::Unload();		// プレイヤー
	CBullet::Unload();		// 弾
	CEffect::Unload();		// エフェクト
	CExplosion::Unload();	// 爆発

	// 2Dアンロード
	CBg::Unload();			// 背景
	CLife::Unload();		// 体力
	CBomb::Unload();		// 爆弾
	CGauge::Unload();		// ゲージ
	CNumber::Unload();		// 数字

	CPause::Unload();		// ポーズ

	CScene *pScene;			// シーンポインタ

	for (int nCntPriority = 0; nCntPriority <= CScene::PRIORITY_PAUSE; nCntPriority++)
	{// プライオリティカウント
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// シーンカウント
			pScene = CScene::GetScene(nCntPriority, nCntScene);		// シーン読み込み

			if (pScene != NULL)
			{// NULL以外の場合
				pScene->Uninit();		// 終了処理
				pScene = NULL;			// NULLへ
			}
		}
	}
}

//=============================================================================
// 更新処理											(public)	*** CTutorial ***
//=============================================================================
void CTutorial::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// 入力デバイス
	CInputXPad *pInputXPad = CManager::GetInputXPad();				// 入力デバイス
	CFade::FADE fade = CFade::GetFade();	// フェード状態取得

	if (m_bPause == false)
	{
		SetMap();		// マップ設定
	}

	//if (pInputKeyboard->GetTrigger(DIK_P) == true && fade == CFade::FADE_NONE)
	for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
	{
		if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_START, nCnt) == true && fade == CFade::FADE_NONE)
		{// 任意のキー（P）を押した場合
			m_bPause = m_bPause ? false : true;		// 表示or非表示

			switch (m_bPause)
			{// ポーズし使用
			case true:
				if (m_pPause == NULL)
				{// 生成と更新制御
					m_pPause = CPause::Create();					// ポーズクリエイト

					CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// 更新範囲をポーズから
				}

				break;
			case false:
				if (m_pPause != NULL)
				{// NULL以外の場合
					m_pPause->Uninit();		// 終了処理
					m_pPause = NULL;		// NULL

					CScene::SetStartUp(0);	// 更新範囲を初期化
				}

				break;
			}
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true && m_bPause == false)
	{
		CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
	}
}

//=============================================================================
// 描画処理											(public)	*** CTutorial ***
//=============================================================================
void CTutorial::Draw(void)
{
	
}

//=============================================================================
// ポーズ設定処理									(public)	*** CTutorial ***
//=============================================================================
void CTutorial::SetPause(bool bPause)
{
	m_bPause = bPause;		// ポーズ

	switch (CTutorial::m_bPause)
	{// ポーズ
	case true:
		if (CTutorial::m_pPause == NULL)
		{// NULLの場合
			m_pPause = CPause::Create();					// ポーズクリエイト

			CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// 更新範囲をポーズから
		}

		break;
	case false:
		if (m_pPause != NULL)
		{// NULL以外の場合
			m_pPause->Uninit();		// 終了処理
			m_pPause = NULL;		// NULL

			CScene::SetStartUp(0);	// 更新範囲を初期化
		}

		break;
	}
}

//=============================================================================
// マップ設定処理									(public)	*** CTutorial ***
//=============================================================================
void CTutorial::SetMap(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposV();		// カメラ視点取得
	int nNummap = m_mapfieldOld;

	if ((int)(Camerapos.z - 1.0f) % 1200 == 0)
	{// カメラが一定間隔進んだら
		if (m_mapfield % 6 < 3)
		{// 海
			CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
				D3DXVECTOR3(0.0f, -100.0f, 2400.0f + (m_mapfield * 1200.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);				// メッシュフィールド

			m_mapfieldOld = 0;
		}
		else
		{// 地面
			CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
				D3DXVECTOR3(0.0f, 0.0f, 2400.0f + (m_mapfield * 1200.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_G, CMesh3D::MESHTEX_NUMSIZE);				// メッシュフィールド

			m_mapfieldOld = 1;
		}

		if (0 < m_mapfield && nNummap != m_mapfieldOld)
		{
			CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 2400.0f + (m_mapfield * 1200.0f) - 600.0f), D3DXVECTOR3(0.0f, (D3DX_PI * (m_mapfieldOld ^ 1)), 0.0f), CObject3D::OBJECTTYPE_O_G);
		}

		m_mapfield++;		// フィールドカウント
	}

	if ((int)(Camerapos.z - 1.0f + 690.0f) % 1200 == 0)
	{// カメラが一定間隔進んだら
		if ((m_mapfield - 1) % 2 == 0)
		{
			CEnemy::Create(D3DXVECTOR3(-200.0f + (200.0f * (rand() % 3)), 300.0f, Camerapos.z + 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), CEnemy::ENEMYTYPE_S_1, 60);
		}
		else if ((m_mapfield - 1) % 6 < 3)
		{
			CEnemy::Create(D3DXVECTOR3(-300.0f + (300.0f * (rand() % 3)), -100.0f, Camerapos.z + 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), CEnemy::ENEMYTYPE_O_1, 80);
		}
		else
		{
			CEnemy::Create(D3DXVECTOR3(0.0f, 10.0f, Camerapos.z + 1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), CEnemy::ENEMYTYPE_G_1, 70);

			CObject3D::Create(D3DXVECTOR3(-200.0f, 0.0f, Camerapos.z + 1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_BILL_1);		// モデル
			CObject3D::Create(D3DXVECTOR3(200.0f, 0.0f, Camerapos.z + 1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_BILL_1);		// モデル
		}
	}

	if ((int)(Camerapos.z - 1.0f) % 300 == 0)
	{// カメラが一定間隔進んだら
		float size = (float)((rand() % 50) * 0.01f) + 1.0f;		// 大きさランダム

		CCloud::Create(D3DXVECTOR3(-150.0f + (float)(rand() % 300), 400.0f, Camerapos.z + 600.0f + (float)(rand() % 100)), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f), 
			D3DXVECTOR3(50.0f * size, 30.0f * size, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f - ((rand() % 2) * 2.0f), 1.0f - ((rand() % 2) * 2.0f)), CScene3D::SCENE3DTYPE_NORMAL);			// 雲
	}
}