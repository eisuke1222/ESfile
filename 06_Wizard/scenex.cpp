//=============================================================================
//
// シーンX処理 [scenex.h]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scenex.h"			// シーンX
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー

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
// コンストラクタ									(public)	*** CSceneX ***
//=============================================================================
CSceneX::CSceneX(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture			= NULL;									// テクスチャ情報へのポインタ
	m_pMesh				= NULL;									// メッシュ情報へのポインタ
	m_pBuffMat			= NULL;									// マテリアル情報へのポインタ
	m_nNumMat			= NULL;									// マテリアル情報の数

	m_pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_col				= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_bcolChange_rgb	= false;								// 色変更rgb
	m_bcolChange_a		= false;								// 色変更a
}

//=============================================================================
// デストラクタ									(public)	*** CSceneX ***
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// 初期化処理									(public)	*** CSceneX ***
//=============================================================================Z
HRESULT CSceneX::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイス取得

	int		nNumVtx;		// 頂点数
	DWORD	sizeFVF;		// 頂点フォーマットのサイズ
	BYTE	*pVtxBuff;		// 頂点バッファのポインタ

	if (m_pMesh != NULL)
	{// NULL以外の場合
		// 頂点数の取得
		nNumVtx = m_pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		// 頂点バッファをロック
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{// カウント
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

			// 最大を求める
			if (vtx.x > m_vtxMax.x)	m_vtxMax.x = vtx.x;
			if (vtx.y > m_vtxMax.y)	m_vtxMax.y = vtx.y;
			if (vtx.z > m_vtxMax.z)	m_vtxMax.z = vtx.z;

			// 最小を求める
			if (vtx.x < m_vtxMin.x)	m_vtxMin.x = vtx.x;
			if (vtx.y < m_vtxMin.y)	m_vtxMin.y = vtx.y;
			if (vtx.z < m_vtxMin.z)	m_vtxMin.z = vtx.z;

			pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		m_pMesh->UnlockVertexBuffer();
	}

	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CSceneX ***
//=============================================================================
void CSceneX::Uninit(void)
{
	// シーン開放
	CScene::SetDeath();
}

//=============================================================================
// 更新処理										(public)	*** CSceneX ***
//=============================================================================
void CSceneX::Update(void)
{
	// 現在向き調整
	if (m_rot.x > D3DX_PI)	{ m_rot.x -= D3DX_PI * 2.0f; }
	if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }
	if (m_rot.y > D3DX_PI)	{ m_rot.y -= D3DX_PI * 2.0f; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }
	if (m_rot.z > D3DX_PI)	{ m_rot.z -= D3DX_PI * 2.0f; }
	if (m_rot.z < -D3DX_PI) { m_rot.z += D3DX_PI * 2.0f; }
}

//=============================================================================
// 描画処理										(public)	*** CSceneX ***
//=============================================================================
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX			mtxRot, mtxTrans;			// 計算用マトリックス
	D3DXMATRIX			mtxWorld;					// 親マトリックス
	D3DMATERIAL9		matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL		*pmat = NULL;				// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (m_pBuffMat != NULL)
	{// NULL以外の場合
		// マテリアルデータへのポインタを取得
		pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	}
	D3DXCOLOR col;	// 色

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// カウント
		if (m_pBuffMat != NULL)
		{// NULL以外の場合
			if (m_bcolChange_rgb == true)
			{// 色変更
				col = pmat[nCntMat].MatD3D.Diffuse;
				pmat[nCntMat].MatD3D.Diffuse = m_col;
			}
			if (m_bcolChange_a == true)
			{// 色変更
				col.a = pmat[nCntMat].MatD3D.Diffuse.a;
				pmat[nCntMat].MatD3D.Diffuse.a = m_col.a;
			}

			// マテリアルの設定
			pDevice->SetMaterial(&pmat[nCntMat].MatD3D);
		}

		if (m_pTexture[nCntMat] != NULL)
		{// NULL以外の場合
			// テクスチャ設定
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}
		else
		{// NULLの場合
			// テクスチャ設定
			pDevice->SetTexture(0, NULL);
		}

		if (m_pMesh != NULL)
		{
			// オブジェクト(パーツ)の描画
			m_pMesh->DrawSubset(nCntMat);
		}

		if (m_bcolChange_rgb == true)
		{// 色変更
			pmat[nCntMat].MatD3D.Diffuse = col;
		}
		if (m_bcolChange_a == true)
		{// 色変更
			pmat[nCntMat].MatD3D.Diffuse.a = col.a;
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	m_bcolChange_rgb	= false;		// 変更終了
	m_bcolChange_a		= false;		// 変更終了
}