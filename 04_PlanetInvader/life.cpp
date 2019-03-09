//=============================================================================
//
// 体力（残機）処理 [life.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "life.h"			// 体力
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "scene2d.h"		// シーン2D
#include "camera.h"			// カメラ
#include "fade.h"			// フェード
#include "game.h"			// ゲーム
#include "player.h"			// プレイヤー
#include "bomb.h"			// 爆弾

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\heart.png"		// テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			CLife::m_pTexture				= NULL;				// テクスチャ
bool						CLife::m_bAddLife[MAX_PLAYER]	= {};				// 復活使用
bool						CLife::m_bRespawn[MAX_PLAYER]	= {};				// 復活使用
int							CLife::m_nLifeAll[MAX_PLAYER]	= {};				// 最大数
bool						CLife::m_bLiveState[MAX_PLAYER] = {};				// 生存数

//=============================================================================
// コンストラクタ									(public)	*** CLife ***
//=============================================================================
CLife::CLife() : CScene(CScene::PRIORITY_UI)
{
	// 初期設定
	m_nLifeNum = 0;										// 体力番号

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{// カウント
		m_apScene2D[nCntLife] = NULL;
	}
}

//=============================================================================
// デストラクタ										(public)	*** CLife ***
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// ロード処理							(public)	*** CLife ***
//=============================================================================
HRESULT CLife::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	if (m_pTexture == NULL)
	{// NULLの場合
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CLife ***
//=============================================================================
void CLife::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{// NULL以外の場合
		m_pTexture->Release();		// 解放
		m_pTexture = NULL;			// NULLへ
	}
}

//=============================================================================
// 確保処理											(public)	*** CLife ***
//=============================================================================
CLife *CLife::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLifeNum)
{
	CLife *pLife = NULL;			// ポインタ

	if (pLife == NULL)
	{// NULLの場合
		pLife = new CLife;			// メモリ確保
	}

	if (pLife != NULL)
	{// NULL以外の場合
		pLife->m_nLifeNum = nLifeNum;				// 体力番号

		for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
		{// カウント
			if (pLife->m_apScene2D[nCntLife] == NULL)
			{
				pLife->m_apScene2D[nCntLife] = new CScene2D(CScene::PRIORITY_UI);																			// メモリ確保
				pLife->m_apScene2D[nCntLife]->SetInitAll(D3DXVECTOR3(pos.x + ((size.x * 2.0f) * nCntLife), pos.y, 0.0f), size, col, D3DXVECTOR2(1.0f, 1.0f));		// 初期値設定
				pLife->m_apScene2D[nCntLife]->Init();																												// 初期化処理
				pLife->m_apScene2D[nCntLife]->SetObjType(CScene::OBJTYPE_LIFE);																						// オブジェクトタイプ設定
				pLife->m_apScene2D[nCntLife]->BindTexture(m_pTexture);																								// テクスチャ設定
			}
		}
		
		m_bAddLife[pLife->m_nLifeNum] = false;		// 加算
		m_bRespawn[pLife->m_nLifeNum] = true;		// リスポーン
		m_nLifeAll[pLife->m_nLifeNum] = MAX_LIFE;	// 体力最大数
		m_bLiveState[pLife->m_nLifeNum] = true;		// 体力最大数

		pLife->Init();		// 初期化処理
	}

	return pLife;
}

//=============================================================================
// 初期化処理										(public)	*** CLife ***
//=============================================================================
HRESULT CLife::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CLife ***
//=============================================================================
void CLife::Uninit(void)
{
	for (int nCntLife = 0; nCntLife < m_nLifeAll[m_nLifeNum]; nCntLife++)
	{// カウント
		m_apScene2D[nCntLife]->Uninit();			// 終了処理
		m_apScene2D[nCntLife] = NULL;				// NULLへ
	}

	CScene::Release();
}

//=============================================================================
// 更新処理											(public)	*** CLife ***
//=============================================================================
void CLife::Update(void)
{
	CManager::MODE mode = CManager::GetMode();						// モード取得
	CGame::GAME game = CGame::GetGame();							// ゲーム取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// 入力デバイス
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();		// カメラ注視点取得

	if (m_bAddLife[m_nLifeNum] == true && m_nLifeAll[m_nLifeNum] < MAX_LIFE - 1)
	{//
		AddLife();
		m_bAddLife[m_nLifeNum] = false;
	}
	if (m_bRespawn[m_nLifeNum] == true && 0 < m_nLifeAll[m_nLifeNum])
	{//
		if (CManager::GetPlayNum() == MAX_PLAYER)
		{
			CPlayer::Create(D3DXVECTOR3(Camerapos.x - 100.0f + (200.0f * m_nLifeNum), 300.0f, Camerapos.z - 500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), m_nLifeNum);			// プレイヤーの生成
		}
		else
		{
			CPlayer::Create(D3DXVECTOR3(Camerapos.x, 300.0f, Camerapos.z - 500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), m_nLifeNum);			// プレイヤーの生成
		}

		DeleteLife();						// ライフ減少
		m_bRespawn[m_nLifeNum] = false;		// 復活不能
	}

	if (m_bLiveState[m_nLifeNum] == false)
	{

		int nCntLive = 0;
		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			if (m_bLiveState[nCnt] == false)
			{
				nCntLive++;
			}
		}

		if (nCntLive == CManager::GetPlayNum())
		{

			if (mode == CManager::MODE_TUTORIAL)
			{
				CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
			}
			else if (mode == CManager::MODE_GAME)
			{
				if (game == CGame::GAME_PLAY)
				{
					CGame::SetGame(CGame::GAME_OVER);
				}
			}
		}
	}

#ifdef _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_V) == true)
	{
		if (m_nLifeAll[m_nLifeNum] < MAX_LIFE)
		{
			for (int nCntLife = m_nLifeAll[m_nLifeNum]; nCntLife < MAX_LIFE; nCntLife++)
			{
				m_apScene2D[nCntLife]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			m_nLifeAll[m_nLifeNum] = MAX_LIFE;
		}
	}
#endif
}

//=============================================================================
// 描画処理											(public)	*** CLife ***
//=============================================================================
void CLife::Draw(void)
{

}

//=============================================================================
// 追加処理											(public)	*** CBomb ***
//=============================================================================
void CLife::AddLife(void)
{
	if (m_nLifeAll[m_nLifeNum] < MAX_LIFE - 1)
	{// 最大数より少ない
		m_apScene2D[m_nLifeAll[m_nLifeNum]]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nLifeAll[m_nLifeNum]++;
	}
}

//=============================================================================
// 解放処理											(public)	*** CLife ***
//=============================================================================
void CLife::DeleteLife(void)
{
	if (0 < m_nLifeAll[m_nLifeNum])
	{// １以上あるとき
		m_nLifeAll[m_nLifeNum]--;
		m_apScene2D[m_nLifeAll[m_nLifeNum]]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));		// 終了処理

		CBomb::SetReset(m_nLifeNum, true);						// ボムリセット
	}
}