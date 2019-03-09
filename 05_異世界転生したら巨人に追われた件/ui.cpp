//**************************************************************
//
//				ポリゴンの処理[polygon.cpp]
//				Auther : Shun Yokomichi
//
//**************************************************************
#include "ui.h"				// UIポリゴン
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー

//**************************************************************
//			マクロ定義
//**************************************************************
#define POLYGON_TEXNAME000		"data/TEXTURE/game/NumberOption.png"	// 時間の間の点
#define POLYGON_TEXNAME001		"data/TEXTURE/game/Frame_Distance.png"	// ミニマップ
#define POLYGON_TEXNAME002		"data/TEXTURE/game/UIPolygon.png"	// ライフゲージ
#define POLYGON_TEXNAME003		"data/TEXTURE/game/TimeIcon.png"		// 時間
#define POLYGON_TEXNAME004		"data/TEXTURE/game/player.png"			// プレイヤー
#define TEX_UV_U				(1.0f)									// テクスチャUV_U
#define TEX_UV_V				(1.0f)									// テクスチャUV_V

//**************************************************************
//		静的メンバ変数
//**************************************************************
LPDIRECT3DTEXTURE9 CUiPolygon::m_pTexture[MAX_POLYGON] = {};
float				CUiPolygon::m_colA = 1.0f;

//**************************************************************
//		UIポリゴンの生成
//**************************************************************
CUiPolygon*CUiPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nTexNum)
{
	CUiPolygon *pPolygon = NULL;

	// シーンの生成
	if (pPolygon == NULL)
	{
		pPolygon = new CUiPolygon;
		if (pPolygon != NULL)
		{
			pPolygon->Init(pos, size, col, nTexNum);
		}
	}

	return pPolygon;
}

//**************************************************************
//		テクスチャの読み込み
//**************************************************************
HRESULT CUiPolygon::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	for (int nCntTex = 0; nCntTex < MAX_POLYGON; nCntTex++)
	{
		if (m_pTexture[nCntTex] == NULL)
		{
			// テクスチャの生成
			if (nCntTex == 0)
			{
				D3DXCreateTextureFromFile(pDevice, POLYGON_TEXNAME000, &m_pTexture[0]);
			}
			if (nCntTex == 1)
			{
				D3DXCreateTextureFromFile(pDevice, POLYGON_TEXNAME001, &m_pTexture[1]);
			}
			if (nCntTex == 2)
			{
				D3DXCreateTextureFromFile(pDevice, POLYGON_TEXNAME002, &m_pTexture[2]);
			}
			if (nCntTex == 3)
			{
				D3DXCreateTextureFromFile(pDevice, POLYGON_TEXNAME003, &m_pTexture[3]);
			}
			if (nCntTex == 4)
			{
				D3DXCreateTextureFromFile(pDevice, POLYGON_TEXNAME004, &m_pTexture[4]);
			}
		}
	}


	return S_OK;
}

//**************************************************************
//		テクスチャの開放
//**************************************************************
void CUiPolygon::Unload(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_POLYGON; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//**************************************************************
//		コンストラクタ
//**************************************************************
CUiPolygon::CUiPolygon() : CScene2D(PRIORITY_UI, OBJTYPE_NUMBER)
{

}

//**************************************************************
//		デストラクタ
//**************************************************************
CUiPolygon::~CUiPolygon()
{

}

//**************************************************************
//		初期化処理
//**************************************************************
HRESULT CUiPolygon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nTexNum)
{
	CScene2D::SetInitAll(pos, size, col, D3DXVECTOR2(TEX_UV_U, TEX_UV_V));		// 初期値設定
	CScene2D::Init();
	CScene2D::BindTexture(m_pTexture[nTexNum]);
	m_colA = 1.0f;

	return S_OK;
}

//**************************************************************
//		終了処理
//**************************************************************
void CUiPolygon::Uninit(void)
{
	CScene2D::Uninit();
}

//**************************************************************
//		更新処理
//**************************************************************
void CUiPolygon::Update(void)
{
	CScene2D::Update();
}

//**************************************************************
//		描画処理
//**************************************************************
void CUiPolygon::Draw(void)
{
	CScene2D::Draw();
}