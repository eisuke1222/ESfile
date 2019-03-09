//**************************************************************
//
//				背景の処理[bg.h]
//				Auther : Shun Yokomichi
//
//**************************************************************
#include "scene3d.h"		// シーン３D
#include "bg.h"				// 背景
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "game.h"			// ゲーム
#include "player.h"			// プレイヤー

//**************************************************************
//				マクロ定義
//**************************************************************
#define BG_TEXTURENAME000	"data/TEXTURE/game/BG001.jpg"		// 読み込むテクスチャ000

//**************************************************************
//				静的メンバ変数
//**************************************************************
LPDIRECT3DTEXTURE9 CBg::m_pTexture = NULL;						// テクスチャ情報へのポインタ

//**************************************************************
//				背景の生成
//**************************************************************
CBg *CBg::Create()
{
	CBg *pBg = NULL;	// ポインタの生成

	// シーンの生成
	if (pBg == NULL)
	{
		pBg = new CBg;
		if (pBg != NULL)
		{
			pBg->Init();
		}
	}

	return pBg;
}

//**************************************************************
//				読み込むテクスチャ
//**************************************************************
HRESULT CBg::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラー取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイス取得
	CManager::MODE pMode = CManager::GetMode();				// モードの取得

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME000, &m_pTexture);
	
	return S_OK;
}

//**************************************************************
//				開放するテクスチャ
//**************************************************************
void CBg::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{// NULL以外の場合
		m_pTexture->Release();		// 開放
		m_pTexture = NULL;			// NULLへ
	}
	
}

//**************************************************************
//		コンストラクタ
//**************************************************************
CBg::CBg() : CScene(PRIORITY_BG, OBJTYPE_BG)
{
	// デフォルト値
	m_mvoetex_x = 0.0f;
}

//**************************************************************
//		デストラクタ
//**************************************************************
CBg::~CBg()
{

}

//**************************************************************
//		初期化処理
//**************************************************************
HRESULT CBg::Init(void)
{
	CManager::MODE pMode = CManager::GetMode();			// モード取得

	m_pScene = new CScene3D(PRIORITY_BG, OBJTYPE_BG);	// メモリ確保

	if (m_pScene != NULL)
	{// NULL以外の場合
		m_pScene->SetInitAll(D3DXVECTOR3(0.0f, 300.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(1000.0f, 300.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);		// 初期値設定
		m_pScene->Init();								// 初期化処理
		m_pScene->BindTexture(m_pTexture);				// テクスチャ設定
	}

	return S_OK;
}

//**************************************************************
//		終了処理
//**************************************************************
void CBg::Uninit(void)
{
	if (m_pScene != NULL)
	{	// NULLじゃない場合
		m_pScene->Uninit();		// 終了処理
		m_pScene = NULL;		// NULLへ
	}
	

	CScene::SetDeath();		// 死亡フラグ
}

//**************************************************************
//		更新処理
//**************************************************************
void CBg::Update(void)
{
	CManager::MODE pMode = CManager::GetMode();		// モード取得

	if (pMode == CManager::MODE_TITLE)
	{// タイトルの場合
		m_mvoetex_x += 0.0008f;		// 移動

		// 位置設定
		m_pScene->Setpos(D3DXVECTOR3(CManager::GetCamera()->GetPosVDest().x, m_pScene->Getpos().y, m_pScene->Getpos().z));
		// テクスチャ設定
		m_pScene->SetTex(1, 0, D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(m_mvoetex_x, 0.0f));
	}
	else if (pMode == CManager::MODE_GAME)
	{// ゲームの場合
		if (CManager::GetCamera()->GetPosVDest().x - 2.0f <= CGame::GetPlayer()->Getpos().x)
		{
				m_mvoetex_x += 0.0004f;
		}
	
		// 位置設定
		m_pScene->Setpos(D3DXVECTOR3(CManager::GetCamera()->GetPosVDest().x, m_pScene->Getpos().y, m_pScene->Getpos().z));
		// テクスチャ設定
		m_pScene->SetTex(1, 0, D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(m_mvoetex_x, 0.0f));
	}
}

//**************************************************************
//		描画処理
//**************************************************************
void CBg::Draw(void)
{
	m_pScene->Draw();		// 描画処理
}