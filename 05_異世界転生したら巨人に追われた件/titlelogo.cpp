//=============================================================================
//
// タイトルロゴ処理 [titlelogo.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "titlelogo.h"		// タイトルロゴ
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "font.h"			// フォント

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_NAME000	"data\\TEXTURE\\title\\TitleLogo3.png"	// テクスチャ
#define	TEXTURE_NAME001	"data\\TEXTURE\\title\\credit.png"		// テクスチャ
#define CHANGE_TEMI		(600)									// 画面が変わるまでの時間
#define SIZE_X			(500.0f)								// サイズの上限(X軸)
#define SIZE_Y			(200.0f)								// サイズの上限(Y軸)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CTitleLogo::m_pTexture[MAX_LOGO] = {};			// テクスチャ情報へのポインタ
int					CTitleLogo::m_Pattern = 0;
int					CTitleLogo::m_nData = 0;
//=============================================================================
// コンストラクタ									(public)	*** CTitleLogo ***
//=============================================================================
CTitleLogo::CTitleLogo() : CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_TITLE)
{

}

//=============================================================================
// デストラクタ									(public)	*** CTitleLogo ***
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
// ロード処理									(public)	*** CTitleLogo ***
//=============================================================================
HRESULT CTitleLogo::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < MAX_LOGO; nCntTex++)
	{
		if (m_pTexture[nCntTex] == NULL)
		{// NULLの場合
		 // テクスチャの生成
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME000, &m_pTexture[0]);
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME001, &m_pTexture[1]);
		}
	}

	return S_OK;
}

//=============================================================================
// アンロード処理									(public)	*** CTitleLogo ***
//=============================================================================
void CTitleLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_LOGO; nCntTex++)
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
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, int nTex)
{
	CTitleLogo *pTitleLogo = NULL;				// ポインタ

	if (pTitleLogo == NULL)
	{// NULLの場合
		pTitleLogo = new CTitleLogo;			// メモリ確保
	}

	if (pTitleLogo != NULL)
	{// NULL以外の場合
		pTitleLogo->Init(pos, size, type);			// 初期化処理
		pTitleLogo->BindTexture(m_pTexture[nTex]);	// テクスチャ設定
	}

	return pTitleLogo;
}

//=============================================================================
// 初期化処理										(public)	*** CTitleLogo ***
//=============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	CScene2D::SetInitAll(pos, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));			// 初期値設定
	m_size = size;							// サイズの代入
	CScene2D::Init();						// 初期化処理
	m_Pattern = 0;
	m_nTime = 0;
	m_fColA = 0.0f;
	m_type = type;
	m_nData = 0;

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CTitleLogo ***
//=============================================================================
void CTitleLogo::Uninit(void)
{
	CScene2D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CTitleLogo ***
//=============================================================================
void CTitleLogo::Update(void)
{
	switch (m_type)
	{
	case TYPE_LOGO:
		// ロゴの場合
		if (SIZE_X >= m_size.x && SIZE_Y >= m_size.y && m_Pattern == 0)
		{	// ある程度サイズになるまで
			m_size.x += 4.0f;
			m_size.y += 1.6f;
			m_fColA = 1.0f;
		}
		else if (m_Pattern == 0)
		{	// フラグが0なら
			m_Pattern = 1;
		}

		m_nTime++;			// 時間の増加

		if (m_nTime >= CHANGE_TEMI)
		{	// 指定した時間以上なら
			m_fColA -= 0.02f;		// 透明度を下げる
			if (m_fColA <= 0.0f)
			{	// 透明度が0.0f以下なら(透明なら)
				m_Pattern = 3;
				Uninit();
				return;
			}
		}
		break;

	case TYPE_CREDIT:
		m_nTime++;			// 時間の増加

		if (m_nTime >= CHANGE_TEMI)
		{	// 指定した時間以上なら
			m_fColA -= 0.02f;		// 透明度を下げる
			if (m_fColA <= 0.0f)
			{	// 透明度が0.0f以下なら(透明なら)
				m_Pattern = 1;
				m_nData = 1;
				Uninit();
				return;
			}
		}
		else
		{
			m_fColA += 0.02f;		// 透明度を下げる
			if (m_fColA >= 1.0f)
			{	// 透明度が0.0f以下なら(透明なら)
				m_fColA = 1.0f;
			}
		}
		break;
	}

	CScene2D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));	// 色の更新
	CScene2D::Setsize(m_size);				// サイズの更新
	CScene2D::Update();						// 更新処理
}

//=============================================================================
// 描画処理											(public)	*** CTitleLogo ***
//=============================================================================
void CTitleLogo::Draw(void)
{
	CScene2D::Draw();						// 描画処理
}