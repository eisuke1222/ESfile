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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\field000.jpg"		// テクスチャ
#define TEXTURE_NAME_2	"data\\TEXTURE\\field001.jpg"		// テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CMeshField::m_pTexture[MAX_MESHFIELD_TEXTURE] = {};		// テクスチャ
D3DXVECTOR2 CMeshField::m_posUV = m_posUV = D3DXVECTOR2(0.0f, 0.0f);

//=============================================================================
// コンストラクタ									(public)	*** CMeshField ***
//=============================================================================
CMeshField::CMeshField() : CMesh3D(CScene::PRIORITY_1)
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

	if (m_pTexture[0] == NULL)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_MESHFIELD_TEXTURE; nCntTex++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 確保処理											(public)	*** CMeshField ***
//=============================================================================
CMeshField *CMeshField::Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHFIELDTYPE meshfieldType, MESHTEX meshTex)
{
	CMeshField *pMeshField = NULL;				// シーンポインタ

	if (pMeshField == NULL)
	{// NULLの場合
		pMeshField = new CMeshField;			// シーンメモリ確保
	}

	if (pMeshField != NULL)
	{// NULL以外の場合
		// 初期化処理（位置、大きさ、色、UV）
		pMeshField->Init(meshType, num, pos, rot, size, col, TexUV, meshfieldType, meshTex);
		pMeshField->SetObjType(CScene::OBJTYPE_MESHFIELD);
		pMeshField->BindTexture(m_pTexture[meshfieldType]);	// テクスチャ設定
	}

	return pMeshField;
}

//=============================================================================
// 初期化処理										(public)	*** CMeshField ***
//=============================================================================
HRESULT CMeshField::Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHFIELDTYPE meshfieldType, MESHTEX meshTex)
{
	//CMesh3D::SetInitAll(meshType, num, pos, rot, size, col, TexUV, meshTex);
	CMesh3D::SetInitAll(meshType, D3DXVECTOR3(1.0f, 0.0f, 1.0f), pos, rot, size, col, D3DXVECTOR2(TexUV.x * num.x, TexUV.y * num.z), meshTex);
	CMesh3D::Init();		// 初期化処理

	m_meshfieldType = meshfieldType;

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Uninit(void)
{
	CMesh3D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposV();
	D3DXVECTOR3 pos = CMesh3D::Getpos();

	if (m_meshfieldType == MESHFIELDTYPE_O)
	{
		m_posUV.x += -0.001f;
		m_posUV.y += 0.002f;

		CMesh3D::SetTex(m_posUV);
	}

	if (Camerapos.z - pos.z >= 600)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理											(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Draw(void)
{
	CMesh3D::Draw();						// 描画処理
}