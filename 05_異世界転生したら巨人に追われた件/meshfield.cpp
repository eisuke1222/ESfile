//=============================================================================
//
// メッシュフィールド処理 [meshfield.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "meshfield.h"		// メッシュフィールド
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "camera.h"			// カメラ
#include "game.h"			// ゲーム
#include "player.h"			// プレイヤー
#include "wall.h"			// 壁

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\fieldmesh.png"		// テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CMeshField::m_pTexture = {};		// テクスチャ

//=============================================================================
// コンストラクタ									(public)	*** CMeshField ***
//=============================================================================
CMeshField::CMeshField() : CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD)
{

}

//=============================================================================
// デストラクタ										(public)	*** CMeshField ***
//=============================================================================
CMeshField::~CMeshField()
{

}

//=============================================================================
// ロード処理							(public)	*** CMeshField ***
//=============================================================================
HRESULT CMeshField::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture == NULL)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 確保処理											(public)	*** CMeshField ***
//=============================================================================
CMeshField *CMeshField::Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	CMeshField *pMeshField = NULL;				// シーンポインタ

	if (pMeshField == NULL)
	{// NULLの場合
		pMeshField = new CMeshField;			// シーンメモリ確保
	}

	if (pMeshField != NULL)
	{// NULL以外の場合
	 // 初期化処理（位置、大きさ、色、UV）
		pMeshField->Init(meshType, num, pos, rot, size, col, TexUV, meshTex);
		pMeshField->BindTexture(m_pTexture);	// テクスチャ設定
	}

	return pMeshField;
}

//=============================================================================
// 初期化処理										(public)	*** CMeshField ***
//=============================================================================
HRESULT CMeshField::Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	CSceneMesh::SetInitAll(meshType, num, pos, rot, size, col, D3DXVECTOR2(TexUV.x, TexUV.y), meshTex);
	CSceneMesh::Init();		// 初期化処理

	CWall::Create(D3DXVECTOR3(pos.x - size.x, -150.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), D3DXVECTOR3(size.z, 150.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(pos.x + size.x, -150.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), D3DXVECTOR3(size.z, 150.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Uninit(void)
{
//	CGame::SetMeshField(this);
	CSceneMesh::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Update(void)
{
	/*D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// 視点取得
	D3DXVECTOR3 pos = CSceneMesh::Getpos();
	D3DXVECTOR3 size = CSceneMesh::Getsize();
	if (pos.x + size.x < posVDest.x - 500.0f)
	{
		Uninit();
	}*/
}

//=============================================================================
// 描画処理											(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Draw(void)
{
	CSceneMesh::Draw();						// 描画処理
}