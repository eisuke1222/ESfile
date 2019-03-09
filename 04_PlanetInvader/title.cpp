//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "title.h"			// タイトル
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "sound.h"			// サウンド
#include "fade.h"			// フェード
#include "model3d.h"		// モデル3D
#include "scene3d.h"		// シーン3D
#include "scene2d.h"		// シーン2D
#include "camera.h"			// カメラ
#include "menu.h"			// メニュー
#include "number.h"			// 数字
#include "ranking.h"		// ランキング
#include "player.h"			// プレイヤー
#include "effect.h"			// エフェクト
#include "meshcylinder.h"	// メッシュシリンダー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME		"data\\MODEL\\titlelogo.x"				// モデル
#define TEXTURE_NAME_1	"data\\TEXTURE\\press.png"				// テクスチャ1
#define TEXTURE_NAME_2	"data\\TEXTURE\\titlestar000.png"		// テクスチャ2
#define TEXTURE_NAME_3	"data\\TEXTURE\\titleplayer.png"		// テクスチャ3

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	*CTitle::m_pModelTexture	= NULL;					// テクスチャ情報へのポインタ
LPD3DXMESH			CTitle::m_pMesh				= NULL;					// メッシュ情報へのポインタ
LPD3DXBUFFER		CTitle::m_pBuffMat			= NULL;					// マテリアル情報へのポインタ
DWORD				CTitle::m_nNumMat			= NULL;					// マテリアル情報の数
CModel3D			*CTitle::m_pModel3D			= NULL;					// モデル

LPDIRECT3DTEXTURE9	CTitle::m_pPressTexture		= NULL;					// テクスチャ情報へのポインタ
CScene3D			*CTitle::m_pPress3D			= NULL;					// プレス3D

LPDIRECT3DTEXTURE9	CTitle::m_pStarTexture		= NULL;					// テクスチャ情報へのポインタ
CScene3D			*CTitle::m_pStar3D			= NULL;					// スター3D

LPDIRECT3DTEXTURE9	CTitle::m_pSceneTexture		= NULL;					// テクスチャ情報へのポインタ
CScene2D			*CTitle::m_pScene2D[MAX_PLAYER] = {};				// シーン2D

CRanking			*CTitle::m_pRanking			= NULL;					// ランキング

CTitle::TITLE		CTitle::m_title				= CTitle::TITLE_NONE;	// タイトル

int					CTitle::m_nCntTitle			= 0;					// タイトルカウンタ
bool				CTitle::m_bTitleSet_T		= false;				// タイトルセット
bool				CTitle::m_bTitleSet_R		= false;				// ランキングセット
bool				CTitle::m_bPressSet			= false;				// プレスセット
bool				CTitle::m_bPress_R			= false;				// プレスランキング

CPlayer				*CTitle::m_pPlayer[MAX_PLAYER] = {};				// プレイヤー
bool				CTitle::m_bPlayGame			= false;				// プレイゲーム

bool				CTitle::m_bPlayerUse[MAX_CONTROLLERS] = {};			// プレイヤー使用
bool				CTitle::m_bPlayerPress[MAX_CONTROLLERS] = {};		// プレイヤープレス

//=============================================================================
// コンストラクタ									(public)	*** CTitle ***
//=============================================================================
CTitle::CTitle()
{
	// 初期値設定
	m_nCntTitle		= 0;			// タイトルカウンタ
	m_bTitleSet_T	= false;		// タイトルセット 
	m_bTitleSet_R	= false;		// ランキングセット
	m_bPressSet		= false;		// プレスセット
	m_bPress_R		= false;		// プレスランキング
	m_bPlayGame		= false;		// プレイゲーム

	for (int nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{// カウント
		m_bPlayerUse[nCntCont] = false;
	}
}

//=============================================================================
// デストラクタ										(public)	*** CTitle ***
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CTitle::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイス取得

	if (m_pBuffMat == NULL && m_pMesh == NULL)
	{// NULLの場合
		// Xファイルの読み込み
		D3DXLoadMeshFromX(MODEL_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat];
		m_pModelTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

		for (DWORD tex = 0; tex < m_nNumMat; tex++)
		{// カウント
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pModelTexture[tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// テクスチャを使用している
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pModelTexture[tex])))
				{
					MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}
	}

	if (m_pPressTexture == NULL)
	{// NULLの場合
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pPressTexture);
	}

	if (m_pStarTexture == NULL)
	{// NULLの場合
	 // テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pStarTexture);
	}

	if (m_pSceneTexture == NULL)
	{// NULLの場合
	 // テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_pSceneTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CTitle::Unload(void)
{
	// テクスチャの破棄
	for (DWORD tex = 0; tex < m_nNumMat; tex++)
	{// カウント
		if (m_pModelTexture[tex] != NULL)
		{// NULL以外の場合
			m_pModelTexture[tex]->Release();		// 解放
			m_pModelTexture[tex] = NULL;			// NULLへ
		}
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{// NULL以外の場合
		m_pBuffMat->Release();						// 解放
		m_pBuffMat = NULL;							// NULLへ
	}

	// マテリアルの開放
	if (m_pMesh != NULL)
	{// NULL以外の場合
		m_pMesh->Release();							// 解放
		m_pMesh = NULL;								// NULLへ
	}

	// テクスチャの破棄
	if (m_pPressTexture != NULL)
	{// NULL以外の場合
		m_pPressTexture->Release();					// 解放
		m_pPressTexture = NULL;						// NULLへ
	}

	if (m_pStarTexture != NULL)
	{// NULL以外の場合
		m_pStarTexture->Release();					// 解放
		m_pStarTexture = NULL;						// NULLへ
	}

	if (m_pSceneTexture != NULL)
	{// NULL以外の場合
		m_pSceneTexture->Release();					// 解放
		m_pSceneTexture = NULL;						// NULLへ
	}
}

//=============================================================================
// 初期化処理										(public)	*** CTitle ***
//=============================================================================
HRESULT CTitle::Init(void)
{
	// 3Dロード
	CMeshCylinder::Load();			// メッシュシリンダー
	CMenu::Load();					// メニュー
	CPlayer::Load();				// プレイヤー
	// 3Dクリエイト
	CMeshCylinder::Create(CMesh3D::MESHTYPE_CYLINDER, D3DXVECTOR3(24, 1, 0),
		D3DXVECTOR3(0.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1000.0f, -1.0f), 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(6.0f, 1.0f), 0, D3DXVECTOR2(0.0005f, -0.0002f), CMesh3D::MESHTEX_ALL);			// メッシュシリンダー
	CMeshCylinder::Create(CMesh3D::MESHTYPE_CYLINDER, D3DXVECTOR3(24, 1, 0),
		D3DXVECTOR3(0.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1000.0f, -1.0f), 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(6.0f, 1.0f), 1, D3DXVECTOR2(0.001f, -0.0005f), CMesh3D::MESHTEX_ALL);			// メッシュシリンダー
	CMeshCylinder::Create(CMesh3D::MESHTYPE_CYLINDER, D3DXVECTOR3(24, 1, 0),
		D3DXVECTOR3(0.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1000.0f, -1.0f), 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(6.0f, 1.0f), 2, D3DXVECTOR2(0.0015f, -0.001f), CMesh3D::MESHTEX_ALL);			// メッシュシリンダー

	CNumber::Load();				// 数字				
	CRanking::Load();				// ランキング
	CRanking::LoadRanking();		// ランキングロード
	CEffect::Load();				// エフェクト

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pScene2D[nCnt] == NULL)
		{
			m_pScene2D[nCnt] = new CScene2D(CScene::PRIORITY_UI);
			m_pScene2D[nCnt]->SetInitAll(D3DXVECTOR3(0.0f + 100.0f + ((SCREEN_WIDTH - 100.0f - 100.0f) * nCnt), SCREEN_HEIGHT - 80.0f, 0.0f), D3DXVECTOR3(100.0f, 80.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f));
			m_pScene2D[nCnt]->Init();
			m_pScene2D[nCnt]->SetObjType(CScene::OBJTYPE_TITLEPLAYER);
			m_pScene2D[nCnt]->BindTexture(m_pSceneTexture);

			m_pScene2D[nCnt]->SetTex(1, nCnt * MAX_PLAYER, D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	m_pModel3D = NULL;
	m_pPress3D = NULL;
	m_pStar3D = NULL;

	SetTitle(TITLE_LOGO);			// タイトル設定

	m_nCntTitle		= 0;			// タイトルカウンタ
	m_bTitleSet_T	= false;		// タイトルセット 
	m_bTitleSet_R	= false;		// ランキングセット
	m_bPressSet		= false;		// プレスセット
	m_bPress_R		= false;		// プレスランキング

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;		// プレイヤー
	}
	m_bPlayGame		= false;		// プレイゲーム

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CTitle ***
//=============================================================================
void CTitle::Uninit(void)
{
	// モデル
	if (m_pModel3D != NULL)
	{// NULL以外の場合
		m_pModel3D->Uninit();		// 終了処理
		m_pModel3D = NULL;			// NULLへ
	}

	// ランキング
	if (m_pRanking != NULL)
	{// NULL以外の場合
		m_pRanking->Uninit();		// 終了処理
		m_pRanking = NULL;			// NULLへ
	}

	// シーン3D
	if (m_pPress3D != NULL)
	{// NULL以外の場合
		m_pPress3D->Uninit();		// 終了処理
		m_pPress3D = NULL;			// NULLへ
	}

	// プレイヤー
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{// NULL以外の場合
			m_pPlayer[nCntPlayer]->Uninit();		// 終了処理
			m_pPlayer[nCntPlayer] = NULL;			// NULLへ
		}
	}

	// シーン3D
	if (m_pStar3D != NULL)
	{// NULL以外の場合
		m_pStar3D->Uninit();		// 終了処理
		m_pStar3D = NULL;			// NULLへ
	}

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pScene2D[nCnt] != NULL)
		{
			m_pScene2D[nCnt]->Uninit();		// 終了処理
			m_pScene2D[nCnt] = NULL;		// NULLへ
		}
	}

	// 3Dアンロード
	CMeshCylinder::Unload();		// メッシュシリンダー
	CMenu::Unload();				// メニュー
	CPlayer::Unload();				// プレイヤー

	// 2Dアンロード
	CNumber::Unload();				// 数字
	CRanking::Unload();				// ランキング
	CRanking::SaveRanking();		// ランキングセーブ
	CEffect::Unload();				// エフェクト

	CScene *pScene;					// シーンポインタ

	for (int nCntPriority = 0; nCntPriority <= CScene::PRIORITY_PAUSE; nCntPriority++)
	{// プライオリティカウント
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// シーンカウント
			pScene = CScene::GetScene(nCntPriority, nCntScene);		// シーン読み込み

			if (pScene != NULL)
			{// NULL以外の場合
				pScene->Uninit();	// 終了処理
				pScene = NULL;		// NULLへ
			}
		}
	}
}

//=============================================================================
// 更新処理											(public)	*** CTitle ***
//=============================================================================
void CTitle::Update(void)
{
	CCamera *pCamera = CManager::GetCamera();						// カメラ取得
	//CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// 入力デバイス
	CInputXPad *pInputXPad = CManager::GetInputXPad();				// 入力デバイス
	CSound *pSound = CManager::GetSound();							// サウンド
	CFade::FADE fade = CFade::GetFade();							// フェード取得
	D3DXVECTOR3 posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	D3DXVECTOR3 rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動量
	D3DXVECTOR3 posPress = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	D3DXVECTOR3 sizePress = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 大きさ
	D3DXVECTOR2 TexUVPress = D3DXVECTOR2(0.0f, 0.0f);				// UV
	D3DXCOLOR colPress = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			// 色

	// 途中で抜くとエラーが出る、コントローラーソートなど
	if (/*pCamera->GetDest() == false &&*/ fade == CFade::FADE_NONE && m_bPlayGame == false)
	{
		for (int nCntCont = 0; nCntCont < CInputXPad::GetInputNum(); nCntCont++)
		{// カウント
			if (m_bPlayerUse[nCntCont] == false)
			{
				if (pInputXPad->GetRelease(XINPUT_GAMEPAD_START, nCntCont) == true)
				{// キーを押したら
					if (m_bPlayerPress[nCntCont] == false)
					{
						m_bPlayerUse[nCntCont] = true;

						CManager::SetPlayNum(CManager::GetPlayNum() + 1);
					}
				}
				else if (pInputXPad->GetPress(XINPUT_GAMEPAD_START, nCntCont) == false)
				{// キーを押したら
					m_bPlayerPress[nCntCont] = false;
				}

				if (m_pScene2D[nCntCont] != NULL && nCntCont < 2)
				{
					m_pScene2D[nCntCont]->SetTex(1, nCntCont * MAX_PLAYER, D3DXVECTOR2(0.0f, 0.0f));
				}
			}
			else
			{
				if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_START, nCntCont) == true)
				{// キーを押したら
					m_bPlayerUse[nCntCont] = false;
					m_bPlayerPress[nCntCont] = true;

					CManager::SetPlayNum(CManager::GetPlayNum() - 1);
				}

				if (m_pScene2D[nCntCont] != NULL && nCntCont < 2)
				{
					m_pScene2D[nCntCont]->SetTex(1, (nCntCont * MAX_PLAYER) + 1, D3DXVECTOR2(0.0f, 0.0f));
				}
			}
		}
	}

	switch (m_title)
	{// タイトルモード
	case TITLE_LOGO:		// ロゴ
	{
		if (m_pModel3D != NULL)
		{// NULL以外の場合
			posModel = m_pModel3D->Getpos();		// 位置取得
		}
		if (m_pPress3D != NULL)
		{// NULL以外の場合
			posPress = m_pPress3D->Getpos();		// 位置取得
			sizePress = m_pPress3D->Getsize();		// 大きさ取得
			TexUVPress = m_pPress3D->GetTex();		// UV取得
			colPress = m_pPress3D->Getcol();		// 色取得
		}

		if (m_bTitleSet_T == false)
		{// タイトルロゴ設定
			if (m_nCntTitle == 0)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE007);
			}

			m_nCntTitle++;							// カウント

			// 移動量計算
			move.x = ((0.0f - posModel.x) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));
			move.y = ((3.0f - posModel.y) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));
			move.z = ((-50.0f - posModel.z) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));

			if (m_nCntTitle == 120)
			{// 目的位置決定
				// 位置調整
				posModel.x = 0.0f;
				posModel.y = 3.0f;
				posModel.z = -50.0f;
				m_bTitleSet_T = true;		// セット完了
				m_nCntTitle = 0;			// カウント初期化
			}
			else
			{// 移動
				posModel += move;			// 移動量加算
			}
		}
		else
		{// タイトルロゴ設定終了
			m_nCntTitle++;					// カウント

			if (m_bPressSet == false)
			{// プレス設定
				if (30 < m_nCntTitle)
				{// 一定フレーム後
					if (sizePress.x != 16.0f)
					{// 大きさが設定値以下
						if (16.0f < sizePress.x + (16.0f / 60.0f))
						{// 大きさが設定値を超えた
							// 大きさ調整
							sizePress.x = 16.0f;
							TexUVPress.x = 1.0f;
						}
						else
						{// その他
							// 大きさ加算
							sizePress.x += 16.0f / 60.0f;
							TexUVPress.x = (sizePress.x / 16.0f);
						}

						posPress.x = (16.0f - sizePress.x);			// 移動量加算
					}

					if (m_nCntTitle == 120)
					{// 一定フレーム
						m_bPressSet = true;							// セット完了
					}
				}
			}

			// 自動移動
			if (m_nCntTitle % 240 < 120)
			{// 一定フレーム
				posModel.y += 0.003f;		// 移動量加算

				if (m_bPressSet == true)
				{// セット完了
					// 色変更
					colPress.r -= 0.5f / 120.0f;
					colPress.g -= 0.5f / 120.0f;
					colPress.b -= 0.5f / 120.0f;
				}
			}
			else
			{// その他
				posModel.y -= 0.003f;		// 移動量加算

				if (m_bPressSet == true)
				{// セット完了
					// 色変更
					colPress.r += 0.5f / 120.0f;
					colPress.g += 0.5f / 120.0f;
					colPress.b += 0.5f / 120.0f;
				}
			}
		}

		if (pCamera->GetDest() == false)
		{// カメラが動いていない
			if (m_bTitleSet_T == true && m_bPressSet == true)
			{// すべてセット完了
				//if (pInputKeyboard->GetALL(2) == true)
				for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
				{// カウント
					if (pInputXPad->GetALL(2, nCntCont) == true)
					{// キーを押したら
						pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 0.0f, 0.0f), 1200, 90, CCamera::CPOSDEST_TITLE_LOGO);

						pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
					}
				}
			}
			else
			{// その他
				//if (pInputKeyboard->GetALL(2) == true)
				for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
				{// カウント
					if (pInputXPad->GetALL(2, nCntCont) == true)
					{// キーを押したら
						// 目標値にスキップ
						posModel = D3DXVECTOR3(0.0f, 3.0f, -50.0f);			// 位置
						posPress = D3DXVECTOR3(0.0f, -13.0f, -50.0f);		// 位置
						sizePress = D3DXVECTOR3(16.0f, 2.5f, 0.0f);			// 大きさ	
						TexUVPress = D3DXVECTOR2(1.0f, 1.0f);				// UV
						colPress = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色

						// セット完了
						m_bTitleSet_T = true;
						m_bPressSet = true;

						m_nCntTitle = 0;			// カウント初期化
					}
				}
			}
		}

		if (m_pModel3D != NULL)
		{// NULL以外の場合
			m_pModel3D->Setpos(posModel);										// 位置設定
		}
		if (m_pPress3D != NULL)
		{// NULL以外の場合
			m_pPress3D->Setpos(posPress);										// 位置設定
			m_pPress3D->Setsize(sizePress);										// 大きさ設定
			m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));		// テクスチャ設定
			m_pPress3D->Setcol(colPress);										// 色
		}

		if (pCamera->GetPosDest(CCamera::CPOSDEST_TITLE_LOGO) == true)
		{// タイトルカメラセット完了
			pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1200.0f), 1200, 90, CCamera::CPOSDEST_TITLE_LOGO);
			SetTitle(TITLE_MENU);			// メニューへ
		}

		if (m_nCntTitle == 720 && pCamera->GetDest() == false)
		{// 一定フレーム
			posModel = D3DXVECTOR3(0.0f, 3.0f, -50.0f);				// 位置
			posPress = D3DXVECTOR3(0.0f, -13.0f, -50.0f);			// 位置
			sizePress = D3DXVECTOR3(16.0f, 2.5f, 0.0f);				// 大きさ
			TexUVPress = D3DXVECTOR2(1.0f, 1.0f);					// UV
			colPress = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 色

			if (m_pModel3D != NULL)
			{// NULL以外の場合
				m_pModel3D->Setpos(posModel);									// 位置設定
			}
			if (m_pPress3D != NULL)
			{
				m_pPress3D->Setpos(posPress);									// 位置設定
				m_pPress3D->Setsize(sizePress);									// 大きさ設定
				m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));	// テクスチャ設定
				m_pPress3D->Setcol(colPress);									// 色
			}

			SetTitle(TITLE_RANKING);		// ランキングへ
		}
	}
		break;
	case TITLE_MENU:		// メニュー
		if (m_pPlayer != NULL)
		{// NULL以外の場合
			posModel = m_pPlayer[0]->Getpos();		// 位置取得
			rotModel = m_pPlayer[0]->Getrot();		// 向き取得
		}

		if (m_bPlayGame == false)
		{
			m_nCntTitle++;					// カウント

			// 自動移動
			if (m_nCntTitle % 240 < 120)
			{// 一定フレーム
				posModel.y += 0.005f;		// 移動量加算
			}
			else
			{// その他
				posModel.y -= 0.005f;		// 移動量加算
			}
		}
		else
		{
			if (m_nCntTitle == 0)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE008);

				if (CManager::GetPlayNum() == MAX_PLAYER)
				{
					if (m_pPlayer[1] == NULL)
					{
						m_pPlayer[1] = CPlayer::Create(D3DXVECTOR3(-15.0f, 0.0f, -140.0f), D3DXVECTOR3(0.0f, -(D3DX_PI * 0.25f), 0.0f), 0);
					}
				}
			}

			m_nCntTitle++;					// カウント

			if (m_nCntTitle < 90)
			{// 一定フレームまで
				rotModel.y += -(D3DX_PI * 0.75f) / 90.0f;		// 回転
			}

			if (120 < m_nCntTitle)
			{// 一定フレーム以上
				CEffect::Create(D3DXVECTOR3(posModel.x, posModel.y, posModel.z - 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_PLAYER_T, CEffect::TEXTURE_EFFECT);		// エフェクト
			
				posModel.z += (5.0f * ((m_nCntTitle - 120.0f) * 0.006f)) * 5.0f * ((m_nCntTitle - 120.0f) * 0.006f);		// 移動
			}

			if (m_nCntTitle == 240)
			{// 一定フレームになったら
				CFade::Create(CManager::MODE_GAME, CFade::FADE_OUT);		// ゲームヘ
			}
		}

		if (m_pPlayer[0] != NULL)
		{// NULL以外の場合
			m_pPlayer[0]->Setpos(posModel);		// 位置設定
			m_pPlayer[0]->Setrot(rotModel);		// 向き設定
		}

		if (m_pPlayer[1] != NULL)
		{
			CEffect::Create(D3DXVECTOR3(posModel.x, posModel.y, posModel.z - 220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_PLAYER_T, CEffect::TEXTURE_EFFECT);		// エフェクト

			m_pPlayer[1]->Setpos(D3DXVECTOR3(posModel.x, posModel.y, posModel.z - 200.0f));		// 位置設定
			m_pPlayer[1]->Setrot(rotModel);		// 向き設定
		}

		break;
	case TITLE_CUSTOM:		// カスタム
		if (pCamera->GetDest() == false)
		{// カメラが動いていない
		//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
			{// カウント
				if (pInputXPad->GetALL(2, nCntCont) == true)
				{// キーを押したら
					pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1200.0f, 0.0f, 0.0f), 1200, 90, CCamera::CPOSDEST_CUSTOM);
				}
			}
		}

		if (pCamera->GetPosDest(CCamera::CPOSDEST_CUSTOM) == true)
		{// 
			pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1200.0f), 1200, 90, CCamera::CPOSDEST_CUSTOM);
			SetTitle(TITLE_MENU);			// メニューへ
		}

		break;
	case TITLE_RANKING:		// ランキング
	{
		if (m_pModel3D != NULL)
		{// NULL以外の場合
			posModel = m_pModel3D->Getpos();		// 位置取得
		}
		if (m_pPress3D != NULL)
		{// NULL以外の場合
			posPress = m_pPress3D->Getpos();		// 位置取得
			sizePress = m_pPress3D->Getsize();		// 大きさ取得
			TexUVPress = m_pPress3D->GetTex();		// UV取得
			colPress = m_pPress3D->Getcol();		// 色取得
		}

		if (m_bTitleSet_R == false)
		{// タイトルロゴ設定
			if (m_nCntTitle == 0)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE007);
			}

			m_nCntTitle++;					// カウント

			// 移動量計算
			move.x = ((100.0f - posModel.x) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));
			move.y = ((60.0f - posModel.y) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));
			move.z = ((-180.0f - posModel.z) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));

			if (m_nCntTitle == 120)
			{// 目的位置決定
				// 位置調整
				posModel.x = 100.0f;
				posModel.y = 60.0f;
				posModel.z = -180.0f;
				m_bTitleSet_R = true;		// セット完了
				m_nCntTitle = 0;			// カウント初期化
			}
			else
			{// 移動
				posModel += move;			// 移動量加算
			}
		}
		else
		{// タイトルロゴ設定終了
			m_nCntTitle++;					// カウント

			// 自動移動
			if (m_nCntTitle % 240 < 120)
			{// 一定フレーム
				posModel.y += 0.003f;		// 移動量加算

				if (m_bPressSet == true)
				{// セット完了
					// 色変更
					colPress.r -= 0.5f / 120.0f;
					colPress.g -= 0.5f / 120.0f;
					colPress.b -= 0.5f / 120.0f;
				}
			}
			else
			{
				posModel.y -= 0.003f;		// 移動量加算

				if (m_bPressSet == true)
				{// セット完了
					// 色変更
					colPress.r += 0.5f / 120.0f;
					colPress.g += 0.5f / 120.0f;
					colPress.b += 0.5f / 120.0f;
				}
			}
		}

		if (m_pModel3D != NULL)
		{// NULL以外の場合
			m_pModel3D->Setpos(posModel);										// 位置設定
		}
		if (m_pPress3D != NULL)
		{// NULL以外の場合
			m_pPress3D->Setpos(posPress);										// 位置設定
			m_pPress3D->Setsize(sizePress);										// 大きさ設定
			m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));		// テクスチャ設定
			m_pPress3D->Setcol(colPress);										// 色
		}

		//if (m_nCntTitle == 480 || pInputKeyboard->GetALL(2) == true)
		if (CManager::GetPlayNum() != 0)
		{
			if (pCamera->GetDest() == false)
			{// カメラが動いていない
				for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
				{// カウント
					if (m_nCntTitle == 480 || pInputXPad->GetALL(2, nCntCont) == true)
					{// 一定フレームorキーを押したら
						// 目標値にスキップ
						posModel = D3DXVECTOR3(100.0f, 60.0f, -180.0f);			// 位置
						posPress = D3DXVECTOR3(0.0f, -13.0f, -50.0f);			// 位置
						sizePress = D3DXVECTOR3(16.0f, 2.5f, 0.0f);				// 大きさ
						TexUVPress = D3DXVECTOR2(1.0f, 1.0f);					// UV
						colPress = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 色

						if (m_pModel3D != NULL)
						{// NULL以外の場合
							m_pModel3D->Setpos(posModel);										// 位置設定
						}
						if (m_pPress3D != NULL)
						{// NULL以外の場合
							m_pPress3D->Setpos(posPress);										// 位置設定
							m_pPress3D->Setsize(sizePress);										// 大きさ設定
							m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));		// テクスチャ設定
							m_pPress3D->Setcol(colPress);										// 色
						}

						m_bPress_R = true;				// セット完了

						SetTitle(TITLE_LOGO);			// タイトルへ
					}
				}
			}
		}
		else
		{
			if (m_nCntTitle == 480)
			{// 一定フレームorキーを押したら
				// 目標値にスキップ
				posModel = D3DXVECTOR3(100.0f, 60.0f, -180.0f);			// 位置
				posPress = D3DXVECTOR3(0.0f, -13.0f, -50.0f);			// 位置
				sizePress = D3DXVECTOR3(16.0f, 2.5f, 0.0f);				// 大きさ
				TexUVPress = D3DXVECTOR2(1.0f, 1.0f);					// UV
				colPress = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 色

				if (m_pModel3D != NULL)
				{// NULL以外の場合
					m_pModel3D->Setpos(posModel);										// 位置設定
				}
				if (m_pPress3D != NULL)
				{// NULL以外の場合
					m_pPress3D->Setpos(posPress);										// 位置設定
					m_pPress3D->Setsize(sizePress);										// 大きさ設定
					m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));		// テクスチャ設定
					m_pPress3D->Setcol(colPress);										// 色
				}

				m_bPress_R = true;				// セット完了

				SetTitle(TITLE_LOGO);			// タイトルへ
			}
		}
	}
	break;
	}
}

//=============================================================================
// 描画処理											(public)	*** CTitle ***
//=============================================================================
void CTitle::Draw(void)
{
	
}

//=============================================================================
// タイトル設定処理									(public)	*** CGame ***
//=============================================================================
void CTitle::SetTitle(CTitle::TITLE title)
{
	CScene *pScene = NULL;					// シーンポインタ

	switch (m_title)
	{// タイトル（現在）
	case TITLE_LOGO:		// ロゴ
		if (title != TITLE_RANKING)
		{// 次回がランキング以外
			// モデル
			if (m_pModel3D != NULL)
			{// NULL以外の場合
				m_pModel3D->Uninit();		// 終了処理
				m_pModel3D = NULL;			// NULLへ
			}

			// シーン3D
			if (m_pPress3D != NULL)
			{// NULL以外の場合
				m_pPress3D->Uninit();		// 終了処理
				m_pPress3D = NULL;			// NULLへ
			}
		}

		break;
	case TITLE_MENU:		// メニュー
		// プレイヤー
		if (m_pPlayer[0] != NULL)
		{// NULL以外の場合
			m_pPlayer[0]->Uninit();		// 終了処理
			m_pPlayer[0] = NULL;			// NULLへ
		}

		// シーン3D
		if (m_pStar3D != NULL)
		{// NULL以外の場合
			m_pStar3D->Uninit();		// 終了処理
			m_pStar3D = NULL;			// NULLへ
		}

		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// シーンカウント
			pScene = CScene::GetScene(CScene::PRIORITY_4, nCntScene);	// シーン読み込み

			// シーン3D
			if (pScene != NULL)
			{// NULL以外の場合
				pScene->Uninit();			// 終了処理
				pScene = NULL;				// NULLへ
			}
		}
		break;
	case TITLE_CUSTOM:		// カスタム
		break;
	case TITLE_RANKING:		// ランキング
		if (m_bPress_R == false)
		{// プレスセット未完了
			// ランキング
			if (m_pRanking != NULL)
			{// NULL以外の場合
				m_pRanking->Uninit();		// 終了処理
				m_pRanking = NULL;			// NULLへ
			}
		}

		break;
	}

	CScene::SetStartUp(0);	// 更新範囲を初期化

	// カメラ
	CCamera *pCamera = CManager::GetCamera();													// 取得
	pCamera->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 初期値設定

	switch (title)
	{// タイトル（次回）
	case TITLE_LOGO:		// ロゴ
		pCamera->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1200.0f));		// 初期値設定
		
		if (m_title != TITLE_RANKING)
		{// 現在がランキング以外
			if (m_pModel3D == NULL)
			{// NULLの場合
				m_pModel3D = new CModel3D(CScene::PRIORITY_4);									// メモリ確保

				// タイトルセット完了有無
				if (m_bTitleSet_T == false)
					m_pModel3D->SetInitAll(m_pModelTexture, m_pMesh, m_pBuffMat, m_nNumMat, 
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));		// 初期値設定
				else
					m_pModel3D->SetInitAll(m_pModelTexture, m_pMesh, m_pBuffMat, m_nNumMat, 
						D3DXVECTOR3(0.0f, 3.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));		// 初期値設定

				m_pModel3D->Init();																// 初期化処理
				m_pModel3D->SetObjType(CScene::OBJTYPE_TITLE);									// オブジェクトタイプ設定
			}

			if (m_pPress3D == NULL)
			{// NULL以外の場合
				m_pPress3D = new CScene3D(CScene::PRIORITY_4);															// メモリ確保					

				// プレスセット完了有無
				if (m_bPressSet == false)
					m_pPress3D->SetInitAll(D3DXVECTOR3(16.0f, -13.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 2.5f, 0.0f), 
						D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR2(0.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);		// 初期化処理
				else
					m_pPress3D->SetInitAll(D3DXVECTOR3(0.0f, -13.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(16.0f, 2.5f, 0.0f), 
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);		// 初期化処理

				m_pPress3D->Init();																						// 初期化処理
				m_pPress3D->SetObjType(CScene::OBJTYPE_PRESS);															// オブジェクトタイプ設定
				m_pPress3D->BindTexture(m_pPressTexture);																// テクスチャを反映
			}
		}

		// セット設定
		if (m_title == TITLE_MENU)
		{
			m_bTitleSet_T = true;
		}
		else
		{
			m_bTitleSet_T = false;
		}

		m_bTitleSet_R = true;

		m_nCntTitle = 0;			// カウント初期化

		break;
	case TITLE_MENU:		// メニュー
		pCamera->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1200.0f));		// 初期値設定

		// 3Dクリエイト
		CMenu::Create();

		// プレイヤー
		if (m_pPlayer[0] == NULL)
		{// NULLの場合
			m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(-15.0f, 0.0f, 60.0f), D3DXVECTOR3(0.0f, -(D3DX_PI * 0.25f), 0.0f), 0);
		}

		if (m_pStar3D == NULL)
		{// NULL以外の場合
			m_pStar3D = new CScene3D(CScene::PRIORITY_4);															// メモリ確保					

			m_pStar3D->SetInitAll(D3DXVECTOR3(10.0f, 20.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);			// 初期化処理

			m_pStar3D->Init();																						// 初期化処理
			m_pStar3D->SetObjType(CScene::OBJTYPE_STAR);															// オブジェクトタイプ設定
			m_pStar3D->BindTexture(m_pStarTexture);																	// テクスチャを反映
		}

		m_nCntTitle = 0;			// カウント初期化
		m_bPlayGame = false;		// プレイゲーム

		break;
	case TITLE_CUSTOM:		// カスタム
		break;
	case TITLE_RANKING:		// ランキング
		pCamera->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1200.0f));		// 初期値設定

		// ランキング
		if (m_pRanking == NULL)
		{// NULLの場合
			m_pRanking = CRanking::Create(D3DXVECTOR3(60.0f, 8.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(1.2f, 1.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), -1);
		}

		// セット設定
		m_bTitleSet_T = true;
		m_bTitleSet_R = false;
		m_bPress_R = false;

		m_nCntTitle = 0;			// カウント初期化

		break;
	}

	m_title = title;		// タイトル変更
}