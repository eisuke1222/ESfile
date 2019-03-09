//=============================================================================
//
// アイテムの取得時表記処理 [itemicon.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"		// シーン3D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "font.h"			// フォント
#include "itemicon.h"		// アイテム

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ICON_DELCNT (4)		//	アイコンが消える時間
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CItemicon::m_pTexture[ITEMICON_MAXTYPE] = {};

//=============================================================================
// コンストラクタ								(public)	*** CScene3D ***
//=============================================================================
CItemicon::CItemicon() : CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_ITEMICON)
{
	m_ItemIconType = {};
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ									(public)	*** CScene3D ***
//=============================================================================
CItemicon::~CItemicon()
{
}

//=============================================================================
//	アイテムの生成
//=============================================================================
CItemicon *CItemicon::Create(D3DXVECTOR3 pos,  D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int itemType)
{
	CItemicon *pItemIcon = {};

	if (pItemIcon == NULL)
	{//	アイテムの生成
		pItemIcon = new CItemicon;
		if (pItemIcon != NULL)
		{//アイテムの初期化
			pItemIcon->Init(pos,rot, size, col, TexUV, itemType);
		}
	}
	return pItemIcon;
}

//=============================================================================
// 初期化処理									(public)	*** CScene3D ***
//=============================================================================
HRESULT CItemicon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int itemType)
{
	// 初期値設定
	CScene3D::SetInitAll(pos, rot, size, col, TexUV, CScene3D::SCENE3DTYPE_BILLBOARD);
	// 初期化処理
	CScene3D::Init();
	// テクスチャ設定
	CScene3D::BindTexture(m_pTexture[itemType]);

	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CScene3D ***
//=============================================================================
void CItemicon::Uninit(void)
{
	CScene3D::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CScene3D ***
//=============================================================================
void CItemicon::Update(void)
{
	D3DXVECTOR3 pos = CScene3D::Getpos();	// 位置取得
	m_posold = pos;			//	位置の保存

	// 移動設定
	m_move.y += 0.075f;
	pos += m_move;

	CScene3D::Setpos(pos);		// 位置設定
	CScene3D::Setcol(CScene3D::Getcol() - D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.015f));		// 色設定
	CScene3D::Update();			// 更新処理

	if (m_move.y > ICON_DELCNT)
	{// 一定位置で
		Uninit();		// 終了処理
	}
}

//=============================================================================
// 描画処理										(public)	*** CScene3D ***
//=============================================================================
void CItemicon::Draw(void)
{
	CScene3D::Draw();	// 描画処理
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CItemicon::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\itemicon\\itemiconspeed.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\itemicon\\itemiconlife.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item\\coin.png", &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item\\coin2.png", &m_pTexture[3]);

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CItem ***
//=============================================================================
void CItemicon::Unload(void)
{
	for (int nCount = 0; nCount < ITEMICON_MAXTYPE; nCount++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}