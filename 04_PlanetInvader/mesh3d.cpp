//=============================================================================
//
//	メッシュ3D処理 [mesh3d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "mesh3d.h"			// メッシュ3D
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
// コンストラクタ								(public)	*** CMesh3D ***
//=============================================================================
CMesh3D::CMesh3D(int nPriority) : CScene(nPriority)
{
	// 初期値設定
	m_pTexture	= NULL;								// テクスチャ情報へのポインタ
	m_pVtxBuff	= NULL;								// マテリアル情報へのポインタ
	m_pIdxBuff	= NULL;								// インデックス情報へのポインタ

	m_meshType	= MESHTYPE_FIELD;					// メッシュタイプ
	m_num		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 枚数
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);			// UV
}

//=============================================================================
// コンストラクタ								(public)	*** CMesh3D ***
//=============================================================================
CMesh3D::CMesh3D(int nPriority, int nID) : CScene(nPriority, nID)
{
	// 初期値設定
	m_pTexture	= NULL;								// テクスチャ情報へのポインタ
	m_pVtxBuff	= NULL;								// マテリアル情報へのポインタ
	m_pIdxBuff	= NULL;								// インデックス情報へのポインタ

	m_meshType	= MESHTYPE_FIELD;					// メッシュタイプ
	m_num		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 枚数
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);			// UV
}

//=============================================================================
// デストラクタ									(public)	*** CMesh3D ***
//=============================================================================
CMesh3D::~CMesh3D()
{

}

//=============================================================================
// 初期化処理									(public)	*** CMesh3D ***
//=============================================================================
HRESULT CMesh3D::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{// NULL以外の場合
		m_pVtxBuff->Release();		// 解放
		m_pVtxBuff = NULL;			// NULLへ
	}

	// 頂点バッファの破棄
	if (m_pIdxBuff != NULL)
	{// NULL以外の場合
		m_pIdxBuff->Release();		// 解放
		m_pIdxBuff = NULL;			// NULLへ
	}

	CScene::Release();				// リリース
}

//=============================================================================
// 更新処理										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::Update(void)
{

}

//=============================================================================
// 描画処理										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	// フィールドの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nMaxIdx, 0, m_nMaxPolygon);

}

//=============================================================================
// 設定処理										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::SetInitAll(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	// デバイス取得6
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_meshType = meshType;		// メッシュタイプ
	m_meshTex = meshTex;		// テクスチャタイプ
	m_num = num;				// 枚数
	m_pos = pos;				// 位置
	m_rot = rot;				// 向き
	m_TexUV = TexUV;			// UV

	D3DXVECTOR2 tex;			// UV計算用
	int nCount;					// yz枚数

	switch (meshType)
	{// メッシュタイプ
	case MESHTYPE_FIELD:		// フィールド
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.z + 1);							// 頂点数
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.z + (((int)num.z - 1) * 2);	// インデックス数
		m_nMaxPolygon = ((int)num.x * (int)num.z * 2) + (((int)num.z - 1) * 4);		// ポリゴン数

		nCount = (int)num.z;	// z枚数

		break;
	case MESHTYPE_WALL:			// 壁
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.y + 1);							// 頂点数
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// インデックス数
		m_nMaxPolygon = ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// ポリゴン数

		nCount = (int)num.y;	// y枚数

		break;
	case MESHTYPE_CYLINDER:		// シリンダー
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.y + 1);							// 頂点数
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// インデックス数
		m_nMaxPolygon = ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// ポリゴン数

		if (size.z == 0.0f) size.z = 1.0f;		// 表示面
		nCount = (int)num.y;	// y枚数

		break;
	}

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nMaxVtx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// インデックスバッファ生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nMaxIdx, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	for (int nCount_yz = 0; nCount_yz < nCount + 1; nCount_yz++)
	{// yzをカウント
		for (int nCount_x = 0; nCount_x < num.x + 1; nCount_x++)
		{// xをカウント
			switch (meshType)
			{// メッシュタイプ
			case MESHTYPE_FIELD:		// フィールド
				// 頂点情報の設定
				pVtx[0].pos =
					//D3DXVECTOR3(-((size.x / 2) * num.x) + (size.x * nCount_x), 0.0f,((size.z / 2) * num.z) - (size.z * (nCount_yz)));
					D3DXVECTOR3(-size.x + ((size.x * 2) / num.x) * nCount_x, 0.0f, size.z - ((size.z * 2) / num.z) * nCount_yz);
				
				switch (m_meshTex)
				{// テクスチャタイプ
				case MESHTEX_ALL:		// 全面
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCount_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCount_yz));	
					break;
				case MESHTEX_NUMSIZE:	// 分割
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// テクスチャの設定
				pVtx[0].tex = tex;

				// 法線の設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				
				break;
			case MESHTYPE_WALL:			// ウォール
				// 頂点情報の設定
				pVtx[0].pos =
					//D3DXVECTOR3(-((size.x / 2) * num.x) + (size.x * nCount_x), 0.0f,((size.y / 2) * num.y) - (size.y * (nCount_yz)));
					D3DXVECTOR3(-size.x + ((size.x * 2) / num.x) * nCount_x, ((size.y * num.y) - (size.y * nCount_yz)), 0.0f);

				switch (m_meshTex)
				{// テクスチャタイプ
				case MESHTEX_ALL:		// 全面
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCount_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// 分割
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// テクスチャの設定
				pVtx[0].tex = tex;
				
				// 法線の設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			case MESHTYPE_CYLINDER:		// シリンダー
				// 頂点情報の設定
				pVtx[0].pos = D3DXVECTOR3(
					sinf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCount_x) * size.z) * size.x,
					((size.y) * num.y) - (size.y * (nCount_yz)),
					cosf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCount_x) * size.z) * size.x);

				switch (m_meshTex)
				{// テクスチャタイプ
				case MESHTEX_ALL:		// 全面
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCount_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// 分割
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// テクスチャの設定
				pVtx[0].tex = tex;

				// 法線の設定
				D3DXVECTOR3 vecNor;
				vecNor.x = pos.x - (pVtx[0].pos.x / pVtx[0].pos.x);
				vecNor.z = pos.z - (pVtx[0].pos.z / pVtx[0].pos.z);
				D3DXVec3Normalize(&vecNor, &vecNor);					// 正規化する
				pVtx[0].nor = D3DXVECTOR3(vecNor.x, 1.0f, vecNor.z);
				//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			}

			// 頂点カラーの設定
			pVtx[0].col = col;

			pVtx += 1;			// 頂点バッファを進める
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	WORD *pIdx;					// インデックスデータへのポインタ

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int nCount_yz = 0; nCount_yz < nCount; nCount_yz++)
	{// yzをカウント
		for (int nCount_x = 0; nCount_x < num.x + 1; nCount_x++)
		{// xをカウント
			// インデックス情報の設定
			pIdx[0] = (WORD)(((nCount_yz + 1) * (num.x + 1)) + nCount_x);
			pIdx[1] = (WORD)(((nCount_yz) * (num.x + 1)) + nCount_x);

			pIdx += 2;			// インデックスバッファを進める

			if (nCount > 1 && nCount_yz != nCount - 1 && nCount_x == num.x)
			{// 末端に到達したら
			 // インデックス情報の設定（縮退ポリゴン）
				pIdx[0] = (WORD)(((nCount_yz) * (num.x + 1)) + nCount_x);
				pIdx[1] = (WORD)(((nCount_yz + 2) * (num.x + 1)));

				pIdx += 2;		// インデックスバッファを進める
			}
		}
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
}

//=============================================================================
// UV移動処理										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::SetTex(D3DXVECTOR2 TexMoveUV)
{
	int nCount;						// yz枚数

	switch (m_meshType)
	{// メッシュタイプ
	case MESHTYPE_FIELD:			// フィールド
		nCount = (int)m_num.z;		// z枚数

		break;
	case MESHTYPE_WALL:				// ウォール
		nCount = (int)m_num.y;		// y枚数

		break;
	case MESHTYPE_CYLINDER:			// シリンダー
		nCount = (int)m_num.y;		// y枚数

		break;
	}

	VERTEX_3D *pVtx;				// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount_yz = 0; nCount_yz < nCount + 1; nCount_yz++)
	{// yzをカウント
		for (int nCount_x = 0; nCount_x < m_num.x + 1; nCount_x++)
		{// xをカウント
			 // テクスチャの設定
			switch (m_meshTex)
			{// テクスチャタイプ
			case MESHTEX_ALL:		// 全面
				pVtx[0].tex =
					D3DXVECTOR2((m_TexUV.x / (m_num.x)) * (nCount_x % ((int)m_num.x + 1)) + TexMoveUV.x, (m_TexUV.y / (m_num.y)) * (nCount_yz)+TexMoveUV.y);

				break;
			case MESHTEX_NUMSIZE:	// 分割
				pVtx[0].tex =
					D3DXVECTOR2(m_TexUV.x * (nCount_x % ((int)m_num.x + 1)) + TexMoveUV.x, m_TexUV.y * (nCount_yz)+TexMoveUV.y);

				break;
			}
								
			pVtx += 1;		// 頂点バッファを進める
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}