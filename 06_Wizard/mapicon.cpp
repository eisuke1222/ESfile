//=============================================================================
//
// マップアイコン処理 [mapicon.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "mapicon.h"		// マップアイコン
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "texture.h"		// テクスチャ

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// コンストラクタ										(public)	*** CMapIcon ***
//=============================================================================
CMapIcon::CMapIcon(int nPriority) : CScene3D(nPriority, CScene::OBJTYPE_MAPICON)
{

}

//=============================================================================
// デストラクタ										(public)	*** CMapIcon ***
//=============================================================================
CMapIcon::~CMapIcon()
{

}

//=============================================================================
// ロード処理										(public)	*** CMapIcon ***
//=============================================================================
HRESULT CMapIcon::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CMapIcon ***
//=============================================================================
void CMapIcon::Unload(void)
{

}

//=============================================================================
// クリエイト										(public)	*** CMapIcon ***
//=============================================================================
CMapIcon *CMapIcon::Create(int nPriority, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CMapIcon *pMapIcon = NULL;			// ポインタ

	if (pMapIcon == NULL)
	{// NULLの場合
		pMapIcon = new CMapIcon(nPriority);		// メモリ確保
	}

	if (pMapIcon != NULL)
	{// NULL以外の場合
		// 初期値処理
		pMapIcon->SetInitAll(pos, rot, size, col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
		// 初期化処理
		pMapIcon->Init();				
		// テクスチャ反映
		pMapIcon->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_MAPICON));
		// 描画編集
		pMapIcon->DrawEdit(false, false, false, false, true);			
	}

	return pMapIcon;	// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CMapIcon ***
//=============================================================================
HRESULT CMapIcon::Init(void)
{
	CScene3D::Init();	// 初期化処理

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CMapIcon ***
//=============================================================================
void CMapIcon::Uninit(void)
{
	CScene3D::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMapIcon ***
//=============================================================================
void CMapIcon::Update(void)
{

}

//=============================================================================
// 描画処理											(public)	*** CMapIcon ***
//=============================================================================
void CMapIcon::Draw(void)
{
	CScene3D::Draw();	// 描画処理
}