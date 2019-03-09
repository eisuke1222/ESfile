//=============================================================================
//
// ゲージ処理 [gauge.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "gauge.h"			// 背景
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "scene2d.h"		// シーン2D
#include "bomb.h"			// 爆弾

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\bg000.png"		// テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			CGauge::m_apTexture = NULL;					// テクスチャ情報へのポインタ
bool						CGauge::m_bResetGauge[MAX_PLAYER] = {};		// ゲージリセット

//=============================================================================
// コンストラクタ									(public)	*** CGauge ***
//=============================================================================
CGauge::CGauge() : CScene(CScene::PRIORITY_UI)
{
	// 初期値設定
	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX_TEXTURE; nCntGauge++)
	{// カウント
		m_apScene2D[nCntGauge] = NULL;
	}
}

//=============================================================================
// デストラクタ										(public)	*** CGauge ***
//=============================================================================
CGauge::~CGauge()
{

}

//=============================================================================
// ロード処理							(public)	*** CGauge ***
//=============================================================================
HRESULT CGauge::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	if (m_apTexture == NULL)
	{// NULLの場合
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_apTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CGauge ***
//=============================================================================
void CGauge::Unload(void)
{
	// テクスチャの破棄
	if (m_apTexture != NULL)
	{// NULL以外の場合
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
// 確保処理											(public)	*** CGauge ***
//=============================================================================
CGauge *CGauge::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nGaugeNum)
{
	CGauge *pGauge = NULL;					// ポインタ

	if (pGauge == NULL)
	{// NULLの場合
		pGauge = new CGauge;				// メモリ確保
	}

	if (pGauge != NULL)
	{// NULL以外の場合
		pGauge->m_nGaugeNum = nGaugeNum;	// ゲージ番号

		for (int nCntGauge = 0; nCntGauge < GAUGE_MAX_TEXTURE; nCntGauge++)
		{// カウント
			if (pGauge->m_apScene2D[nCntGauge] == NULL)
			{// NULL以外の場合
				pGauge->m_apScene2D[nCntGauge] = new CScene2D(CScene::PRIORITY_UI);		// メモリ確保

				switch (nCntGauge)
				{// カウント番号
				case 0:
					pGauge->m_apScene2D[nCntGauge]->SetInitAll(pos, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));					// 初期値設定
					pGauge->m_pos[pGauge->m_nGaugeNum][nCntGauge] = pos;																										// 位置
					pGauge->m_size[pGauge->m_nGaugeNum][nCntGauge] = size;																									// 大きさ

					break;
				case 1:
					pGauge->m_apScene2D[nCntGauge]->SetInitAll(pos, D3DXVECTOR3(size.x * 0.95f, size.y * 0.8f, 0.0f), col, D3DXVECTOR2(1.0f, 1.0f));	// 初期値設定
					pGauge->m_pos[pGauge->m_nGaugeNum][nCntGauge] = pos;																										// 位置
					pGauge->m_size[pGauge->m_nGaugeNum][nCntGauge] = D3DXVECTOR3(size.x * 0.95f, size.y * 0.8f, 0.0f);														// 大きさ

					break;
				}
				pGauge->m_apScene2D[nCntGauge]->Init();										// 初期化処理
				pGauge->m_apScene2D[nCntGauge]->SetObjType(CScene::OBJTYPE_GAUGE);			// オブジェクトタイプ設定
				//pGauge->m_apScene2D[nCntGauge]->BindTexture(m_apTexture);					// テクスチャ設定
			}
		}
		pGauge->m_bResetGauge[pGauge->m_nGaugeNum] = false;		// リセット

		pGauge->Init();		// 初期化処理
	}

	return pGauge;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CGauge ***
//=============================================================================
HRESULT CGauge::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CGauge ***
//=============================================================================
void CGauge::Uninit(void)
{
	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX_TEXTURE; nCntGauge++)
	{// カウント
		if (m_apScene2D[nCntGauge] != NULL)
		{// NULL以外の場合
			m_apScene2D[nCntGauge]->Uninit();		// 終了処理
			m_apScene2D[nCntGauge] = NULL;			// NULLへ
		}
	}

	CScene::Release();		// リリース
}

//=============================================================================
// 更新処理											(public)	*** CGauge ***
//=============================================================================
void CGauge::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();			// 入力デバイス
	float fDiff = (m_pos[m_nGaugeNum][1].x - m_size[m_nGaugeNum][1].x) - (m_pos[m_nGaugeNum][0].x - m_size[m_nGaugeNum][0].x);	// 最背面との位置の差分を計算
	D3DXVECTOR3 pos = m_apScene2D[1]->Getpos();		// 位置を取得
	D3DXVECTOR3 size = m_apScene2D[1]->Getsize();	// 大きさを取得
	
#ifdef _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_X) == true)
	{
		m_apScene2D[1]->Setsize(D3DXVECTOR3(0.0f, m_size[m_nGaugeNum][1].y, m_size[m_nGaugeNum][1].z));
	}
#endif

	if (size.x != m_size[m_nGaugeNum][1].x)
	{// 大きさが変更された
		if (m_size[m_nGaugeNum][1].x < size.x)
		{// 大きさが最大以下
			size.x = m_size[m_nGaugeNum][1].x;	// 大きさを設定の値へ
			CBomb::SetAdd(m_nGaugeNum, true);
		}
		else
		{
			size.x += 0.23f;				// 大きくしていく
		}

		m_apScene2D[1]->Setpos(D3DXVECTOR3((m_pos[m_nGaugeNum][0].x - m_size[m_nGaugeNum][0].x) + fDiff + size.x, m_pos[m_nGaugeNum][1].y, m_pos[m_nGaugeNum][1].z));	// 位置を設定
		m_apScene2D[1]->Setsize(size);		// 大きさを設定
	}
	else
	{
		if (m_bResetGauge[m_nGaugeNum] == true)
		{
			m_apScene2D[1]->Setsize(D3DXVECTOR3(0.0f, m_size[m_nGaugeNum][1].y, m_size[m_nGaugeNum][1].z));
			m_bResetGauge[m_nGaugeNum] = false;
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CGauge ***
//=============================================================================
void CGauge::Draw(void)
{

}