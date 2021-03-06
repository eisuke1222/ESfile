//=============================================================================
//
// 数字処理 [number.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "number.h"			// 数字
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
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
// コンストラクタ									(public)	*** CNumber ***
//=============================================================================
CNumber::CNumber()
{

}

//=============================================================================
// デストラクタ										(public)	*** CNumber ***
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// ロード処理							(public)	*** CNumber ***
//=============================================================================
HRESULT CNumber::Load(void)
{
	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CNumber ***
//=============================================================================
void CNumber::Unload(void)
{

}

//=============================================================================
// コンストラクタ									(public)	*** CNumber ***
//=============================================================================
CNumber2D::CNumber2D() : CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_NUMBER)
{

}

//=============================================================================
// デストラクタ										(public)	*** CNumber ***
//=============================================================================
CNumber2D::~CNumber2D()
{

}

//=============================================================================
// 初期化処理										(public)	*** CNumber2D ***
//=============================================================================
HRESULT CNumber2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CScene2D::SetInitAll(pos, size, col, D3DXVECTOR2(0.1f, 1.0f));				// 初期値設定
	CScene2D::Init();															// 初期化処理
	CScene2D::BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_NUMBER));	// テクスチャ反映

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CNumber2D ***
//=============================================================================
void CNumber2D::Uninit(void)
{
	CScene2D::Uninit();					// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CNumber2D ***
//=============================================================================
void CNumber2D::Update(void)
{
	CScene2D::Update();					// 更新処理
}

//=============================================================================
// 描画処理											(public)	*** CNumber2D ***
//=============================================================================
void CNumber2D::Draw(void)
{
	CScene2D::Draw();					// 描画処理
}

//----------------------------------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CNumber3D ***
//=============================================================================
CNumber3D::CNumber3D() : CScene3D(CScene::PRIORITY_UI, CScene::OBJTYPE_NUMBER)
{

}

//=============================================================================
// デストラクタ										(public)	*** CNumber3D ***
//=============================================================================
CNumber3D::~CNumber3D()
{

}

//=============================================================================
// 初期化処理										(public)	*** CNumber3D ***
//=============================================================================
HRESULT CNumber3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, CScene3D::SCENE3DTYPE screenType)
{
	CScene3D::SetInitAll(pos, rot, size, col, D3DXVECTOR2(0.1f, 1.0f), screenType);		// 初期値設定
	CScene3D::Init();																  	// 初期化処理
	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_NUMBER));			// テクスチャ反映

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CNumber3D ***
//=============================================================================
void CNumber3D::Uninit(void)
{
	CScene3D::Uninit();					// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CNumber3D ***
//=============================================================================
void CNumber3D::Update(void)
{
	CScene3D::Update();					// 更新処理
}

//=============================================================================
// 描画処理											(public)	*** CNumber3D ***
//=============================================================================
void CNumber3D::Draw(void)
{
	CScene3D::Draw();					// 描画処理
}