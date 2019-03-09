//=============================================================================
//
// ランキングロゴ処理 [rankinglogo.cpp]
// Author : Shun Yokomichi
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "rankinglogo.h"	// ランキングロゴ
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "font.h"			// フォント

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_NAME000	"data\\TEXTURE\\ranking\\RankingLogo000.png"	// テクスチャ
#define	TEXTURE_NAME001	"data\\TEXTURE\\ranking\\Ranking001.png"	// テクスチャ
#define	TEXTURE_NAME002	"data\\TEXTURE\\ranking\\Ranking002.png"	// テクスチャ
#define	TEXTURE_NAME003	"data\\TEXTURE\\ranking\\Ranking003.png"	// テクスチャ
#define	TEXTURE_NAME004	"data\\TEXTURE\\ranking\\Ranking004.png"	// テクスチャ
#define	TEXTURE_NAME005	"data\\TEXTURE\\ranking\\Ranking005.png"	// テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CRnkingLogo::m_pTexture[MAX_RANK_LOGO] = {};			// テクスチャ情報へのポインタ

//=============================================================================
// コンストラクタ									(public)	*** CTitleLogo ***
//=============================================================================
CRnkingLogo::CRnkingLogo() : CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_TITLE)
{

}

//=============================================================================
// デストラクタ									(public)	*** CTitleLogo ***
//=============================================================================
CRnkingLogo::~CRnkingLogo()
{

}

//=============================================================================
// ロード処理									(public)	*** CTitleLogo ***
//=============================================================================
HRESULT CRnkingLogo::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntLogo = 0; nCntLogo < MAX_RANK_LOGO; nCntLogo++)
	{
		if (m_pTexture[nCntLogo] == NULL)
		{// NULLの場合
			// テクスチャの生成
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME000, &m_pTexture[0]);
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME001, &m_pTexture[1]);
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME002, &m_pTexture[2]);
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME003, &m_pTexture[3]);
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME004, &m_pTexture[4]);
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME005, &m_pTexture[5]);
		}
	}

	return S_OK;
}

//=============================================================================
// アンロード処理									(public)	*** CTitleLogo ***
//=============================================================================
void CRnkingLogo::Unload(void)
{
	for (int nCntLogo = 0; nCntLogo < MAX_RANK_LOGO; nCntLogo++)
	{
		if (m_pTexture[nCntLogo] != NULL)
		{// NULLの場合
		 // テクスチャの生成
			m_pTexture[nCntLogo]->Release();		// 解放
			m_pTexture[nCntLogo] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 確保処理										(public)	*** CTitleLogo ***
//=============================================================================
CRnkingLogo *CRnkingLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTexNum)
{
	CRnkingLogo *pRnkingLogo = NULL;				// ポインタ

	if (pRnkingLogo == NULL)
	{// NULLの場合
		pRnkingLogo = new CRnkingLogo;			// メモリ確保
	}

	if (pRnkingLogo != NULL)
	{// NULL以外の場合
		pRnkingLogo->Init(pos, size);			// 初期化処理
		pRnkingLogo->BindTexture(m_pTexture[nTexNum]);	// テクスチャ設定
	}

	return pRnkingLogo;
}

//=============================================================================
// 初期化処理										(public)	*** CTitleLogo ***
//=============================================================================
HRESULT CRnkingLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScene2D::SetInitAll(pos, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));			// 初期値設定
	m_size = size;							// サイズの代入
	CScene2D::Init();						// 初期化処理
	
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CTitleLogo ***
//=============================================================================
void CRnkingLogo::Uninit(void)
{
	CScene2D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CTitleLogo ***
//=============================================================================
void CRnkingLogo::Update(void)
{
	CScene2D::Update();						// 更新処理
}

//=============================================================================
// 描画処理											(public)	*** CTitleLogo ***
//=============================================================================
void CRnkingLogo::Draw(void)
{
	CScene2D::Draw();						// 描画処理
}