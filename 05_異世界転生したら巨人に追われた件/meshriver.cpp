//=============================================================================
//
// メッシュフィールド処理 [meshfield.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "meshriver.h"		// メッシュリバー
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "camera.h"			// カメラ
#include "game.h"			// ゲーム
#include "player.h"			// プレイヤー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\field003.jpg"		// テクスチャ
#define TEXTURE_NAME_2	"data\\TEXTURE\\field002.jpg"		// テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CMeshRiver::m_pTexture[TEXTURE_MAX] = { NULL };		// テクスチャ

//=============================================================================
// コンストラクタ									(public)	*** CMeshRiver ***
//=============================================================================
CMeshRiver::CMeshRiver() : CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_MESHFIELD)
{
	m_Sinrot = 0.0f;
}

//=============================================================================
// デストラクタ										(public)	*** CMeshRiver ***
//=============================================================================
CMeshRiver::~CMeshRiver()
{

}

//=============================================================================
// ロード処理							(public)	*** CMeshRiver ***
//=============================================================================
HRESULT CMeshRiver::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture[0] == NULL)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CMeshRiver ***
//=============================================================================
void CMeshRiver::Unload(void)
{
	// テクスチャの破棄
	for (int nCount = 0; nCount < TEXTURE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// 確保処理											(public)	*** CMeshRiver ***
//=============================================================================
CMeshRiver *CMeshRiver::Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nNum)
{
	CMeshRiver *pMeshRiver = NULL;				// シーンポインタ

	if (pMeshRiver == NULL)
	{// NULLの場合
		pMeshRiver = new CMeshRiver;			// シーンメモリ確保
	}

	if (pMeshRiver != NULL)
	{// NULL以外の場合
	 // 初期化処理（位置、大きさ、色、UV）
		pMeshRiver->Init(meshType, num, pos, rot, size, col, TexUV, meshTex);
		pMeshRiver->BindTexture(m_pTexture[nNum]);	// テクスチャ設定
	}

	return pMeshRiver;
}

//=============================================================================
// 初期化処理										(public)	*** CMeshRiver ***
//=============================================================================
HRESULT CMeshRiver::Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	CSceneMesh::SetInitAll(meshType, num, pos, rot, size, col, D3DXVECTOR2(TexUV.x, TexUV.y), meshTex);
	CSceneMesh::Init();		// 初期化処理

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMeshRiver ***
//=============================================================================
void CMeshRiver::Uninit(void)
{
	CSceneMesh::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMeshRiver ***
//=============================================================================
void CMeshRiver::Update(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();	// プレイヤー取得

	m_Sinrot += 0.05f;	// 波

	CSceneMesh::SinUpdate(CSceneMesh::SINTYPE_RIGHT, CSceneMesh::SINSTART_LU, pPlayer->Getpos(), m_Sinrot, 3.0f, 5.0f);		// 通常

	if (CSceneMesh::ColInMesh(pPlayer->Getpos()))	// 入ったら
		CSceneMesh::SinUpdate(CSceneMesh::SINTYPE_CIRCLE, CSceneMesh::SINSTART_POS, pPlayer->Getpos(), m_Sinrot, 4.0f, 3.0f, 100.0f);


	CSceneMesh::Update();						// 更新処理
}

//=============================================================================
// 描画処理											(public)	*** CMeshRiver ***
//=============================================================================
void CMeshRiver::Draw(void)
{
	CSceneMesh::Draw();						// 描画処理
}