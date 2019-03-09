//=============================================================================
//
// 雲処理 [cloud.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "cloud.h"			// 雲
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "camera.h"			// カメラ

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\cloud000.png"		// テクスチャ情報へのポインタ
#define MOVE_SPEED			(-1.3f)								// 移動速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CCloud::m_pTexture = NULL;		// テクスチャ

//=============================================================================
// コンストラクタ									(public)	*** CCloud ***
//=============================================================================
CCloud::CCloud() : CScene3D(CScene::PRIORITY_5)
{
	// 初期値設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
}

//=============================================================================
// デストラクタ										(public)	*** CCloud ***
//=============================================================================
CCloud::~CCloud()
{

}

//=============================================================================
// ロード処理							(public)	*** CCloud ***
//=============================================================================
HRESULT CCloud::Load(void)
{
	if (m_pTexture == NULL)
	{// NULLの場合
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CCloud ***
//=============================================================================
void CCloud::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{// NULL以外の場合
		m_pTexture->Release();		// 解放
		m_pTexture = NULL;			// NULLへ
	}
}

//=============================================================================
// 確保処理											(public)	*** CCloud ***
//=============================================================================
CCloud *CCloud::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
{
	CCloud *pMeshCloud = NULL;				// ポインタ

	if (pMeshCloud == NULL)
	{// NULLの場合
		pMeshCloud = new CCloud;			// メモリ確保
	}

	if (pMeshCloud != NULL)
	{// NULL以外の場合
		pMeshCloud->Init(pos, rot, size, col, TexUV, scene3dType);		// 初期化処理
		pMeshCloud->SetObjType(CScene::OBJTYPE_CLOUD);					// オブジェクトタイプ設定
		pMeshCloud->BindTexture(m_pTexture);							// テクスチャ設定
	}

	return pMeshCloud;		// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CCloud ***
//=============================================================================
HRESULT CCloud::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
{
	CScene3D::SetInitAll(pos, rot, size, col, TexUV, scene3dType);		// 初期値設定
	CScene3D::Init();													// 初期化処理

	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// カメラ注視点取得

	m_pos = pos - Camerapos;				// カメラ移動無効化

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CCloud ***
//=============================================================================
void CCloud::Uninit(void)
{
	CScene3D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CCloud ***
//=============================================================================
void CCloud::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// カメラ注視点取得
	D3DXVECTOR3 pos = CScene3D::Getpos();	// 位置を取得

	// カメラに対しての位置を設定
	pos.x = Camerapos.x + m_pos.x;			
	pos.z = Camerapos.z + m_pos.z;

	pos.z += MOVE_SPEED;					// 移動量代入

	CScene3D::Setpos(pos);					// 位置を設定

	m_pos = pos - Camerapos;				// 現在位置を計算

	Camerapos = CManager::GetCamera()->GetposV();						// カメラ視点取得

	if (Camerapos.z - pos.z >= 0)
	{// カメラ外に出たら
		Uninit();		// 終了処理
	}
}

//=============================================================================
// 描画処理											(public)	*** CCloud ***
//=============================================================================
void CCloud::Draw(void)
{
	CScene3D::Draw();						// 描画処理
}