//**************************************************************
//
//				プレスボタンの処理[press.cpp]
//				Auther : Shun Yokomichi
//
//**************************************************************
#include "press.h"			// プレスボタン
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー

//**************************************************************
//			マクロ定義
//**************************************************************
#define TEXNAME000	"data/TEXTURE/title/TitleUI.png"				// 時間の間の点
#define TEXNAME001	"data/TEXTURE/tutorial/TutorialMarker.png"		// 矢印
#define TEXNAME002	"data/TEXTURE/tutorial/TutorialMarker001.png"	// キーボード(A)
#define TEXNAME003	"data/TEXTURE/tutorial/TutorialMarker002.png"	// キーボード(D)
#define TEXNAME004	"data/TEXTURE/tutorial/cross.png"				// 十字キー
#define TEXNAME005	"data/TEXTURE/pause/w.png"						// キーボード(W)
#define TEXNAME006	"data/TEXTURE/pause/s.png"						// キーボード(S)
#define TEXNAME007	"data/TEXTURE/tutorial/Enter.png"				// チュートリアル用
#define TEXNAME008	"data/TEXTURE/tutorial/B.png"					// Bボタン
#define TEXNAME009	"data/TEXTURE/result/Enter.png"					// リザルト用
#define COL_A		(0.01f)											// 透明度の変化量
#define TEX_UV_U	(1.0f)											// テクスチャUV_U
#define TEX_UV_V	(1.0f)											// テクスチャUV_V

//**************************************************************
//		静的メンバ変数
//**************************************************************
LPDIRECT3DTEXTURE9 CPress::m_pTexture[MAX_TEX_PRESS] = {};

//**************************************************************
//		UIポリゴンの生成
//**************************************************************
CPress*CPress::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nTex, float rot)
{
	CPress *pPress = NULL;

	// シーンの生成
	if (pPress == NULL)
	{
		pPress = new CPress;
		if (pPress != NULL)
		{
			pPress->Init(pos, size, col, nTex, rot);
		}
	}

	return pPress;
}

//**************************************************************
//		テクスチャの読み込み
//**************************************************************
HRESULT CPress::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXNAME000, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXNAME001, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, TEXNAME002, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, TEXNAME003, &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, TEXNAME004, &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, TEXNAME005, &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, TEXNAME006, &m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice, TEXNAME007, &m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice, TEXNAME008, &m_pTexture[8]);
	D3DXCreateTextureFromFile(pDevice, TEXNAME009, &m_pTexture[9]);

	return S_OK;
}

//**************************************************************
//		テクスチャの開放
//**************************************************************
void CPress::Unload(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_TEX_PRESS; nCntTex++)
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
CPress::CPress() : CScene2D(PRIORITY_UI, OBJTYPE_NONE)
{
	// デフォルト値
	m_fColA = 0.0f;
	m_type = CPress::TYPE_NONE;
}

//**************************************************************
//		デストラクタ
//**************************************************************
CPress::~CPress()
{

}

//**************************************************************
//		初期化処理
//**************************************************************
HRESULT CPress::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nTex, float rot)
{
	CScene2D::SetInitAll(pos, size, col, D3DXVECTOR2(TEX_UV_U, TEX_UV_V));		// 初期値設定
	CScene2D::Setrot(rot);
	CScene2D::Init();
	CScene2D::BindTexture(m_pTexture[nTex]);
	m_fColA = col.a;
	m_type = CPress::TYPE_OPAQUE;

	return S_OK;
}

//**************************************************************
//		終了処理
//**************************************************************
void CPress::Uninit(void)
{
	CScene2D::Uninit();
}

//**************************************************************
//		更新処理
//**************************************************************
void CPress::Update(void)
{
	switch (m_type)
	{
	case TYPE_OPAQUE:
		// 透明から不透明
		m_fColA += COL_A;	// 透明度の増加
		if (m_fColA > 1.0f)
		{	// タイプ変化
			m_type = TYPE_TRANSPARENCE;
		}
		break;

	case TYPE_TRANSPARENCE:
		// 不透明から透明
		m_fColA -= COL_A;	// 透明度の増減
		if (m_fColA < 0.0f)
		{	// タイプ変化
			m_type = TYPE_OPAQUE;
		}
		break;

	case TYPE_MANUAL:
		// 手動
		m_fColA = 0.0f;
		break;

	case TYPE_NONE:
		m_fColA = 1.0f;
		break;
	}

	CScene2D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));
	CScene2D::Update();
}

//**************************************************************
//		描画処理
//**************************************************************
void CPress::Draw(void)
{
	CScene2D::Draw();
}