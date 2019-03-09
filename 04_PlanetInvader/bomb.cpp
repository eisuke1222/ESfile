//=============================================================================
//
// 爆弾（残数）処理 [bomb.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "bomb.h"			// 爆弾
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "scene2d.h"		// シーン2D
#include "gauge.h"			// ゲージ

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\bomb.png"		// テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CBomb::m_pTexture				= NULL;					// テクスチャ情報へのポインタ
bool				CBomb::m_bShotBomb[MAX_PLAYER]	= {};					// 撃つ
bool				CBomb::m_bAddBomb[MAX_PLAYER]	= {};					// 加算
bool				CBomb::m_bResetBomb[MAX_PLAYER]	= {};					// リセット
bool				CBomb::m_bDeleteBomb[MAX_PLAYER]= {};					// 破棄
int 				CBomb::m_nMaxBomb[MAX_PLAYER]	= {};					// 最大数
int 				CBomb::m_nBombAll[MAX_PLAYER]	= {};					// 現在数

//=============================================================================
// コンストラクタ									(public)	*** CBomb ***
//=============================================================================
CBomb::CBomb() : CScene(CScene::PRIORITY_UI)
{
	// 初期設定
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{// カウント
		m_apScene2D[nCntBomb] = NULL;
	}
}

//=============================================================================
// デストラクタ										(public)	*** CBomb ***
//=============================================================================
CBomb::~CBomb()
{

}

//=============================================================================
// ロード処理							(public)	*** CBomb ***
//=============================================================================
HRESULT CBomb::Load(void)
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
// アンロード処理								(public)	*** CBomb ***
//=============================================================================
void CBomb::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{// NULL以外の場
		m_pTexture->Release();		// 解放
		m_pTexture = NULL;			// NULLへ
	}
}

//=============================================================================
// 確保処理											(public)	*** CBomb ***
//=============================================================================
CBomb *CBomb::Create(int nMaxBomb, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nBombNum)
{
	CBomb *pBomb = NULL;				// ポインタ

	if (pBomb == NULL)
	{// NULLの場合
		pBomb = new CBomb;				// メモリ確保
	}

	if (pBomb != NULL)
	{// NULL以外の場合
		pBomb->m_nBombNum = nBombNum;		// 爆弾番号

		for (int nCntBomb = 0; nCntBomb < nMaxBomb; nCntBomb++)
		{// カウント
			if (pBomb->m_apScene2D[nCntBomb] == NULL)
			{
				pBomb->m_apScene2D[nCntBomb] = new CScene2D(CScene::PRIORITY_UI);																			// メモリ確保
				pBomb->m_apScene2D[nCntBomb]->SetInitAll(D3DXVECTOR3(pos.x + ((size.x * 2.0f) * nCntBomb), pos.y, 0.0f), size, col, D3DXVECTOR2(1.0f, 1.0f));		// 初期値設定
				pBomb->m_apScene2D[nCntBomb]->Init();																												// 初期化処理
				pBomb->m_apScene2D[nCntBomb]->SetObjType(CScene::OBJTYPE_BOMB);																						// オブジェクトタイプ設定
				pBomb->m_apScene2D[nCntBomb]->BindTexture(m_pTexture);																								// テクスチャ設定		
			}
		}
		
		pBomb->m_bShotBomb[pBomb->m_nBombNum] = true;			// 撃つ
		pBomb->m_bAddBomb[pBomb->m_nBombNum] = false;			// 加算
		pBomb->m_bResetBomb[pBomb->m_nBombNum] = false;			// リセット
		pBomb->m_bDeleteBomb[pBomb->m_nBombNum] = false;		// 破棄
		pBomb->m_nMaxBomb[pBomb->m_nBombNum] = nMaxBomb;		// 爆弾最大数
		pBomb->m_nBombAll[pBomb->m_nBombNum] = nMaxBomb;		// 爆弾現在数

		pBomb->Init();				// 初期化処理
	}

	return pBomb;		// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CBomb ***
//=============================================================================
HRESULT CBomb::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CBomb ***
//=============================================================================
void CBomb::Uninit(void)
{
	for (int nCntBomb = 0; nCntBomb < m_nBombAll[m_nBombNum]; nCntBomb++)
	{// カウント
		if (m_apScene2D[nCntBomb] != NULL)
		{// NULL以外の場合
			m_apScene2D[nCntBomb]->Uninit();	// 終了処理
			m_apScene2D[nCntBomb] = NULL;		// NULLへ
		}
	}

	CScene::Release();
}

//=============================================================================
// 更新処理											(public)	*** CBomb ***
//=============================================================================
void CBomb::Update(void)
{
	if (m_bAddBomb[m_nBombNum] == true && m_nBombAll[m_nBombNum] < m_nMaxBomb[m_nBombNum])
	{
		AddBomb();
		m_bAddBomb[m_nBombNum] = false;
	}
	if (m_bResetBomb[m_nBombNum] == true)
	{
		ResetBomb();
		m_bResetBomb[m_nBombNum] = false;
	}
	if (m_bDeleteBomb[m_nBombNum] == true && 0 < m_nBombAll[m_nBombNum])
	{
		DeleteBomb();
		m_bDeleteBomb[m_nBombNum] = false;
	}

	if (m_nBombAll[m_nBombNum] == m_nMaxBomb[m_nBombNum])
	{
		CGauge::SetReset(m_nBombNum, false);
	}
	else
	{
		CGauge::SetReset(m_nBombNum, true);
	}
}

//=============================================================================
// 描画処理											(public)	*** CBomb ***
//=============================================================================
void CBomb::Draw(void)
{

}

//=============================================================================
// 追加処理											(public)	*** CBomb ***
//=============================================================================
void CBomb::AddBomb(void)
{
	if (m_nBombAll[m_nBombNum] < m_nMaxBomb[m_nBombNum])
	{// 最大数より少ない
		m_apScene2D[m_nBombAll[m_nBombNum]]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nBombAll[m_nBombNum]++;	// 現在数を増やす
		m_bShotBomb[m_nBombNum] = true;
	}
}

//=============================================================================
// リセット処理											(public)	*** CBomb ***
//=============================================================================
void CBomb::ResetBomb(void)
{
	if (m_nBombAll[m_nBombNum] < m_nMaxBomb[m_nBombNum])
	{// 最大数より少ない
		for (int nCntBomb = m_nBombAll[m_nBombNum]; nCntBomb < m_nMaxBomb[m_nBombNum]; nCntBomb++)
		{// カウント
			m_apScene2D[nCntBomb]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		m_nBombAll[m_nBombNum] = m_nMaxBomb[m_nBombNum];	// 現在数を最大へ
		m_bShotBomb[m_nBombNum] = true;
	}
}

//=============================================================================
// 解放処理											(public)	*** CBomb ***
//=============================================================================
void CBomb::DeleteBomb(void)
{
	if (0 < m_nBombAll)
	{// １以上あるとき
		m_nBombAll[m_nBombNum]--;							// 現在数を減らす
		m_apScene2D[m_nBombAll[m_nBombNum]]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

		if (m_nBombAll[m_nBombNum] == 0)
		{
			m_bShotBomb[m_nBombNum] = false;
		}
	}
}