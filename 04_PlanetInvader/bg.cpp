//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "bg.h"				// 背景
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "scene2d.h"		// シーン2D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\bg011.png"		// テクスチャ1
#define TEXTURE_NAME_2	"data\\TEXTURE\\bg08.png"		// テクスチャ2
#define MOVE_SPEED_1	(-0.001f)						// 移動速度1
#define MOVE_SPEED_2	(-0.0015f)						// 移動速度2
#define MOVE_SPEED_3	(-0.002f)						// 移動速度3

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CBg::m_apTexture[TEXTURE_MAX]	= {};		// テクスチャ情報へのポインタ
int					CBg::m_nSetNum					= 0;		// セット数

//=============================================================================
// コンストラクタ									(public)	*** CBg ***
//=============================================================================
CBg::CBg() : CScene(CScene::PRIORITY_BG)
{
	// 初期値設定
	for (int nCntBg = 0; nCntBg < TEXTURE_MAX; nCntBg++)
	{// カウント
		m_apScene2D[nCntBg] = NULL;
		m_posUV[nCntBg] = D3DXVECTOR2(0.0f, 0.0f);			// 位置
	}
	m_nSetNum = 0;											// セット数
}

//=============================================================================
// デストラクタ										(public)	*** CBg ***
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// ロード処理							(public)	*** CBg ***
//=============================================================================
HRESULT CBg::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	if (m_apTexture[0] == NULL)
	{// NULLの場合
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_apTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_apTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBg ***
//=============================================================================
void CBg::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// カウント
		if (m_apTexture[nCntTex] != NULL)
		{// NULL以外の場合
			m_apTexture[nCntTex]->Release();		// 解放
			m_apTexture[nCntTex] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 確保処理											(public)	*** CBg ***
//=============================================================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nSetNum, int nTexNum)
{
	CBg *pBg = NULL;				// ポインタ

	if (pBg == NULL)
	{// NULLの場合
		pBg = new CBg;				// メモリ確保
	}

	if (pBg != NULL)
	{// NULL以外の場合
		for (int nCntBg = 0; nCntBg < nSetNum; nCntBg++)
		{// カウント
			if (pBg->m_apScene2D[nCntBg] == NULL)
			{// NULL以外の場合
				pBg->m_apScene2D[nCntBg] = new CScene2D(CScene::PRIORITY_BG);					// メモリ確保
				pBg->m_apScene2D[nCntBg]->SetInitAll(pos, size, col, D3DXVECTOR2(1.0f, 1.0f));	// 初期値設定
				pBg->m_apScene2D[nCntBg]->Init();												// 初期化処理
				pBg->m_apScene2D[nCntBg]->SetObjType(CScene::OBJTYPE_BG);						// オブジェクトタイプ設定
				pBg->m_apScene2D[nCntBg]->BindTexture(m_apTexture[nTexNum]);					// テクスチャ設定
			}
		}
		
		pBg->Init();			// 初期化処理
		m_nSetNum = nSetNum;	// セット数設定
	}

	return pBg;		// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CBg ***
//=============================================================================
HRESULT CBg::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CBg ***
//=============================================================================
void CBg::Uninit(void)
{
	for (int nCntBg = 0; nCntBg < m_nSetNum; nCntBg++)
	{// カウント
		if (m_apScene2D[nCntBg] != NULL)
		{// NULL以外の場合
			m_apScene2D[nCntBg]->Uninit();		// 終了処理
			m_apScene2D[nCntBg] = NULL;			// NULLへ
		}
	}

	CScene::Release();
}

//=============================================================================
// 更新処理											(public)	*** CBg ***
//=============================================================================
void CBg::Update(void)
{

}

//=============================================================================
// 描画処理											(public)	*** CBg ***
//=============================================================================
void CBg::Draw(void)
{

}