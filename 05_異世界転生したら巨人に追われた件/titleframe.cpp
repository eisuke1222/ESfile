//=============================================================================
//
// タイトルフレーム処理 [titleframe.cpp]
// Author : Shun Yokomichi
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "titleframe.h"		// タイトルフレーム
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "font.h"			// フォント

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_NAME000	"data\\TEXTURE\\title\\UI_Frame001.png"	// テクスチャ
#define	TEXTURE_NAME001	"data\\TEXTURE\\title\\UI_Frame000.png"	// テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CTitleFrame::m_pTexture[MAX_FRAME] = {};			// テクスチャ情報へのポインタ

																		//=============================================================================
																		// コンストラクタ									(public)	*** CTitleLogo ***
																		//=============================================================================
CTitleFrame::CTitleFrame() : CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_TITLE)
{

}

//=============================================================================
// デストラクタ									(public)	*** CTitleLogo ***
//=============================================================================
CTitleFrame::~CTitleFrame()
{

}

//=============================================================================
// ロード処理									(public)	*** CTitleLogo ***
//=============================================================================
HRESULT CTitleFrame::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture[0] == NULL)
	{// NULLの場合
	 // テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME000, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME001, &m_pTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理									(public)	*** CTitleLogo ***
//=============================================================================
void CTitleFrame::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{// NULLの場合
		 // テクスチャの生成
			m_pTexture[nCntTex]->Release();		// 解放
			m_pTexture[nCntTex] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 確保処理										(public)	*** CTitleLogo ***
//=============================================================================
CTitleFrame *CTitleFrame::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float rot, int nTex)
{
	CTitleFrame *pTitleFrame = NULL;				// ポインタ

	if (pTitleFrame == NULL)
	{// NULLの場合
		pTitleFrame = new CTitleFrame;			// メモリ確保
	}

	if (pTitleFrame != NULL)
	{// NULL以外の場合
		pTitleFrame->Init(pos, size, rot);			// 初期化処理
		pTitleFrame->BindTexture(m_pTexture[nTex]);	// テクスチャ設定
	}

	return pTitleFrame;
}

//=============================================================================
// 初期化処理										(public)	*** CTitleLogo ***
//=============================================================================
HRESULT CTitleFrame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float rot)
{
	CScene2D::SetInitAll(pos, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));			// 初期値設定
	CScene2D::Setrot(rot);					// 向きの設定
	CScene2D::Init();						// 初期化処理

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CTitleLogo ***
//=============================================================================
void CTitleFrame::Uninit(void)
{
	CScene2D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CTitleLogo ***
//=============================================================================
void CTitleFrame::Update(void)
{
	m_msize = CScene2D::Getsize();
	m_msize.x += 7.0f;
	m_msize.y += 3.5f;
	CScene2D::Setsize(D3DXVECTOR3(m_msize.x, m_msize.y, m_msize.z));
	CScene2D::Update();						// 更新処理
	if (m_msize.x >= 1800.0f || m_msize.y >= 1200.0f)
	{	// ある程度のサイズに行ったら終了
		CTitleFrame::Uninit();
	}
}

//=============================================================================
// 描画処理											(public)	*** CTitleLogo ***
//=============================================================================
void CTitleFrame::Draw(void)
{
	CScene2D::Draw();						// 描画処理
}