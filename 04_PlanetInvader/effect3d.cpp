//=============================================================================
//
// エフェクト3D処理 [effect3d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "effect3d.h"		// エフェクト3D
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "camera.h"			// カメラ
#include "effect.h"			// エフェクト

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME		"data\\MODEL\\break000.x"		// モデル

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CEffect3D::m_pTexture	= {};			// テクスチャ情報へのポインタ
LPD3DXMESH				CEffect3D::m_pMesh		= {};			// メッシュ情報へのポインタ
LPD3DXBUFFER			CEffect3D::m_pBuffMat	= {};			// マテリアル情報へのポインタ
DWORD					CEffect3D::m_nNumMat	= {};			// マテリアル情報の数

//=============================================================================
// コンストラクタ									(public)	*** CEffect3D ***
//=============================================================================
CEffect3D::CEffect3D() : CModel3D(CScene::PRIORITY_1)
{
	m_nCntTimer = 0;								// 時間カウンタ
	m_rotAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き加算
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_bStop = false;								// 止まる
}

//=============================================================================
// デストラクタ										(public)	*** CEffect3D ***
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CEffect3D::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	if (m_pBuffMat == NULL && m_pMesh == NULL)
	{// NULLの場合		
		// Xファイルの読み込み
		D3DXLoadMeshFromX(MODEL_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat];
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

		for (DWORD tex = 0; tex < m_nNumMat; tex++)
		{// カウント
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// テクスチャを使用している
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[tex])))
				{
					MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CEffect3D::Unload(void)
{

	// テクスチャの破棄
	for (DWORD tex = 0; tex < m_nNumMat; tex++)
	{// カウント
		if (m_pTexture[tex] != NULL)
		{// NULL以外の場合
			m_pTexture[tex]->Release();		// 解放
			m_pTexture[tex] = NULL;			// NULLへ
		}
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{// NULL以外の場合
		m_pBuffMat->Release();				// 解放
		m_pBuffMat = NULL;					// NULLへ
	}

	// マテリアルの開放
	if (m_pMesh != NULL)
	{// NULL以外の場合
		m_pMesh->Release();					// 解放
		m_pMesh = NULL;						// NULLへ
	}

	m_nNumMat = 0;				// 初期化
}

//=============================================================================
// 確保処理											(public)	*** CEffect3D ***
//=============================================================================
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 rotAdd, D3DXVECTOR3 move)
{
	CEffect3D *pEffect3D = NULL;			// ポインタ

	if (pEffect3D == NULL)
	{// NULLの場合
		pEffect3D = new CEffect3D;			// メモリ確保
	}

	if (pEffect3D != NULL)
	{// NULL以外の場合
		pEffect3D->m_nID = pEffect3D->GetID();				// 親ID取得

		pEffect3D->Init(pos, rot, rotAdd, move);			// 初期化処理
		pEffect3D->SetObjType(CScene::OBJTYPE_EFFECT3D);	// オブジェクトタイプ設定
	}

	return pEffect3D;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CEffect3D ***
//=============================================================================
HRESULT CEffect3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 rotAdd, D3DXVECTOR3 move)
{
	CModel3D::SetInitAll(m_pTexture, m_pMesh, m_pBuffMat, m_nNumMat, pos, rot);		// 初期値設定
	CModel3D::Init();																// 初期化処理

	m_rotAdd = rotAdd;
	m_move = move;

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Uninit(void)
{
	CModel3D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposV();	// カメラ視点取得
	D3DXVECTOR3 pos = CEffect3D::Getpos();						// 位置取得
	D3DXVECTOR3 rot = CEffect3D::Getrot();						// 向き取得
	D3DXVECTOR3 sizeMax = CEffect3D::Getsize(0);				// 大きさ取得

	m_nCntTimer++;

	if (m_nCntTimer % 10 == 0)
	{
		if (m_bStop == false)
		{
			CEffect::Create(D3DXVECTOR3(pos.x, pos.y + sizeMax.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(EFFECT_SIZE_X * 1.5f, EFFECT_SIZE_Y * 1.5f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.03f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);
		}
		else
		{
			CEffect::Create(D3DXVECTOR3(pos.x, pos.y + sizeMax.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, -2.5f), D3DXVECTOR3(EFFECT_SIZE_X * 2.5f, EFFECT_SIZE_Y * 2.5f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);
		}
	}

	if (m_bStop == false)
	{
		pos += m_move;
		rot += m_rotAdd;

		m_move.y -= 0.1f;

		if (m_bStop == false)
		{
			if (pos.y < 0)
			{
				m_bStop = true;
			}
		}

		if (m_bStop == false)
		{
			// 判定チェック------------------------------------------------------------
			CModel3D *pModel3D;						// シーン3D
			CScene::OBJTYPE objType;				// オブジェクト種類
			CEffect3D *pEffect3D;					// エフェクト3D
			D3DXVECTOR3 targetpos, targetsize;		// 判定用 : 相手の位置、相手の大きさ											

			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{// シーンカウント
				if (nCntScene != m_nID)
				{// 自分以外
					pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_1, nCntScene);	// シーン読み込み

					if (pModel3D != NULL)
					{// NULL以外の場合
						objType = pModel3D->GetObjType();			// オブジェクトタイプ取得
						pEffect3D = (CEffect3D*)pModel3D;

						if (objType == OBJTYPE_EFFECT3D && pEffect3D->m_bStop == true)
						{
							targetpos = pModel3D->Getpos();			// 位置	
							targetsize = pModel3D->Getsize(0);		// 大きさ

							if (((targetpos.x - pos.x) * (targetpos.x - pos.x)) + ((targetpos.y - pos.y) * (targetpos.y - pos.y)) < (targetsize.y + sizeMax.y) * (targetsize.y + sizeMax.y))
							{// 範囲チェック（円、画面外）
								m_bStop = true;

								m_nCntTimer = 0;

								break;
							}
						}
					}
				}
			}
		}
	}

	Setpos(pos);
	Setrot(rot);

	if (Camerapos.z - pos.z >= 0)
	{// カメラ範囲外に出たら
		Uninit();					// 終了処理
	}
}

//=============================================================================
// 描画処理											(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Draw(void)
{
	CModel3D::Draw();						// 描画処理
}