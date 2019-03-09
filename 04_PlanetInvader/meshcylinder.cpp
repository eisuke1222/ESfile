//=============================================================================
//
// メッシュシリンダー処理 [meshcylinder.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "meshcylinder.h"	// メッシュシリンダー
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\bg000.png"		// テクスチャ
#define TEXTURE_NAME_2	"data\\TEXTURE\\bg001.png"		// テクスチャ
#define TEXTURE_NAME_3	"data\\TEXTURE\\bg002.png"		// テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CMeshCylinder::m_pTexture[MESHCYLINDER_MAX_TEXTURE] = {};		// テクスチャ

//=============================================================================
// コンストラクタ									(public)	*** CMeshCylinder ***
//=============================================================================
CMeshCylinder::CMeshCylinder() : CMesh3D(CScene::PRIORITY_1)
{
	m_posUV = D3DXVECTOR2(0.0f, 0.0f);
	m_moveUV = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// デストラクタ										(public)	*** CMeshCylinder ***
//=============================================================================
CMeshCylinder::~CMeshCylinder()
{

}

//=============================================================================
// ロード処理							(public)	*** CMeshCylinder ***
//=============================================================================
HRESULT CMeshCylinder::Load(void)
{
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture[0] == NULL)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pTexture[1]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_pTexture[2]);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CMeshCylinder ***
//=============================================================================
void CMeshCylinder::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MESHCYLINDER_MAX_TEXTURE; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 確保処理											(public)	*** CMeshCylinder ***
//=============================================================================
CMeshCylinder *CMeshCylinder::Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nTexNum, D3DXVECTOR2 TexMoveUV, MESHTEX meshTex)
{
	CMeshCylinder *pMeshCylinder = NULL;				// シーンポインタ

	if (pMeshCylinder == NULL)
	{// NULLの場合
		pMeshCylinder = new CMeshCylinder;				// シーンメモリ確保
	}

	if (pMeshCylinder != NULL)
	{// NULL以外の場合
		// 初期化処理（位置、大きさ、色、UV）
		pMeshCylinder->Init(meshType, num, pos, rot, size, col, TexUV, TexMoveUV, meshTex);
		pMeshCylinder->SetObjType(CScene::OBJTYPE_MESHCYLINDER);
		pMeshCylinder->BindTexture(m_pTexture[nTexNum]);	// テクスチャ設定
	}

	return pMeshCylinder;
}

//=============================================================================
// 初期化処理										(public)	*** CMeshCylinder ***
//=============================================================================
HRESULT CMeshCylinder::Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV, MESHTEX meshTex)
{
	CMesh3D::SetInitAll(meshType, num, pos, rot, size, col, TexUV, meshTex);
	CMesh3D::Init();		// 初期化処理
	m_moveUV = TexMoveUV;

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMeshCylinder ***
//=============================================================================
void CMeshCylinder::Uninit(void)
{
	CMesh3D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMeshCylinder ***
//=============================================================================
void CMeshCylinder::Update(void)
{
	m_posUV += m_moveUV;

	CMesh3D::SetTex(m_posUV);
}

//=============================================================================
// 描画処理											(public)	*** CMeshCylinder ***
//=============================================================================
void CMeshCylinder::Draw(void)
{
	CMesh3D::Draw();						// 描画処理
}