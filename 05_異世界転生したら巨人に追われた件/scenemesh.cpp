//=============================================================================
//
//	シーンメッシュ処理 [scenemesh.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scenemesh.h"		// シーンメッシュ
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
// コンストラクタ								(public)	*** CSceneMesh ***
//=============================================================================
CSceneMesh::CSceneMesh(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture = NULL;								// テクスチャ情報へのポインタ
	m_pVtxBuff = NULL;								// マテリアル情報へのポインタ
	m_pIdxBuff = NULL;								// インデックス情報へのポインタ

	m_meshType = MESHTYPE_FIELD;					// メッシュタイプ
	m_num = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 枚数
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_TexUV = D3DXVECTOR2(0.0f, 0.0f);			// UV
}

//=============================================================================
// デストラクタ									(public)	*** CSceneMesh ***
//=============================================================================
CSceneMesh::~CSceneMesh()
{

}

//=============================================================================
// 初期化処理									(public)	*** CSceneMesh ***
//=============================================================================
HRESULT CSceneMesh::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::Uninit(void)
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

	CScene::SetDeath();				// リリース
}

//=============================================================================
// 更新処理										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::Update(void)
{

}

//=============================================================================
// 描画処理										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::Draw(void)
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
// 設定処理										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetInitAll(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	// デバイス取得6
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_meshType = meshType;		// メッシュタイプ
	m_meshTex = meshTex;		// テクスチャタイプ
	m_num = num;				// 枚数
	m_pos = pos;				// 位置
	m_rot = rot;				// 向き
	m_size = size;				// 大きさ
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

				pVtx[0].pos.y = -12.0f + (float)(rand() % 12);

				/*if (nCount_x == 0 || nCount_x == num.x)
				{
				pVtx[0].pos.y = 200.0f;
				}*/

				/*if ((int)(m_num.x / 2) < nCount_x)
				{
				pVtx[0].pos.y = 100.0f * (nCount_x - (m_num.x / 2));
				}*/

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

	// 法線を計算
	SetVecNor();

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
// UV移動処理										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetTex(D3DXVECTOR2 TexMoveUV)
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

//=============================================================================
// 頂点取得処理										(public)	*** CSceneMesh ***
//=============================================================================
D3DXVECTOR3 CSceneMesh::GetVtx(int nNum)
{
	D3DXVECTOR3 pos;	// 位置

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pos = pVtx[nNum].pos;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return pos;
}

//=============================================================================
// 法線取得処理										(public)	*** CSceneMesh ***
//=============================================================================
D3DXVECTOR3 CSceneMesh::GetNor(int nNum)
{
	D3DXVECTOR3 nor;	// 法線

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 法線を設定
	nor = pVtx[nNum].nor;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return nor;
}

//=============================================================================
// 判定に使用するかチェック処理						(public)	*** CSceneMesh ***
//=============================================================================
bool CSceneMesh::ColInMesh(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	bool bLand = false;
	D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
	D3DXVECTOR3 posmtx = pos - m_pos;
	/*
	// ベクトル計算
	vecA = pVtx[(int)(m_num.x)].pos - pVtx[(int)(0)].pos;
	vecC = posmtx - pVtx[(int)(0)].pos;

	if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
	{// 範囲内（右側）にいることを確認
	 // ベクトル計算
		vecA = pVtx[(int)(m_nMaxVtx - 1)].pos - pVtx[(int)(m_num.x)].pos;
		vecC = posmtx - pVtx[(int)(m_num.x)].pos;

		if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
		{// 範囲内（右側）にいることを確認
		 // ベクトル計算
			vecA = pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos - pVtx[(int)(m_nMaxVtx - 1)].pos;
			vecC = posmtx - pVtx[(int)(m_nMaxVtx - 1)].pos;

			if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）にいることを確認
			 // ベクトル計算
				vecA = pVtx[(int)(0)].pos - pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos;
				vecC = posmtx - pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos;

				if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
				{// 範囲内（右側）にいることを確認
					bLand = true;
				}
			}
		}
	}*/

	if (m_pos.x - m_size.x <= pos.x && pos.x <= m_pos.x + m_size.x &&
		m_pos.z - m_size.z <= pos.z && pos.z <= m_pos.z + m_size.z)
	{
		bLand = true;
	}

	return bLand;
}

//=============================================================================
// サイン更新処理										(public)	*** CSceneMesh ***
//=============================================================================
float CSceneMesh::GetUpDown(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtxNum[4];					// 使用頂点の番号を保存
	D3DXVECTOR3 Hitnor[2];				// 使用頂点の法線を保存
	bool bRange_Block = false;			// ブロック範囲内か
	float fUpDown = 0.0f;				// 坂

										// 現在のブロックを確認
	int nHitBlock =
		(int)((pos.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
		(int)((int)(((pos.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;			// 各ラインチェック

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{// ブロック内の４頂点をカウント
			D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
			int nData = 0, nChack1 = 0, nChack2 = 0;	// 値調整用（次のカウント、誤差解消用）
			int nVtxNum_N = 0, nVtxNum = 0;				// 使用する頂点番号（次の頂点番号、現在の頂点番号）

			nData = (nCntVtx + 1) % 4;					// 次のカウントを見る

														// カウント順の誤差をなくす（2*2 = 0134; for = 0143 → 0134）
			if (nCntVtx == 2) { nChack1 = 1; }
			else if (nCntVtx == 3) { nChack1 = -1; }
			if (nData == 2) { nChack2 = 1; }
			else if (nData == 3) { nChack2 = -1; }

			// 頂点番号を計算
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// 頂点番号を保存
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// ベクトル計算
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = pos - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）にいることを確認
				nCntLand_Block++;						// チェック数をカウント

				if (nCntLand_Block == 4)
				{// チェック数が４になったら
					bRange_Block = true;				// ブロック内にいることを確定

					D3DXVECTOR3 nor[2], vec0[2], vec1[2];		// 法線計算用（正規化用、ベクトル）

																// 左下
					vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
					vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;
					// 右上
					vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
					vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;
					// 内積計算
					D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
					D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
					// 正規化
					D3DXVec3Normalize(&nor[0], &nor[0]);
					D3DXVec3Normalize(&nor[1], &nor[1]);

					// 法線の設定
					Hitnor[0] = nor[0];
					Hitnor[1] = nor[1];

					break;
				}
			}
		}

		if (bRange_Block == true)
		{// ブロック内にいたら
			D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
			int nVtxNum = 0;							// 使用する頂点番号

														// ベクトル計算
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = pos - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）にいることを確認
				nVtxNum = 2;
			}
			else
			{// 範囲内（左側）にいることを確認
				nVtxNum = 1;
			}

			// ベクトル計算
			D3DXVECTOR3 vecP = D3DXVECTOR3(pos.x + sinf(rot.y), pos.y, pos.z + cosf(rot.y)) - pos;

			fUpDown = ((pVtx[nCntVtxNum[nVtxNum]].nor.x * vecP.x) + (pVtx[nCntVtxNum[nVtxNum]].nor.y * vecP.y) + (pVtx[nCntVtxNum[nVtxNum]].nor.z * vecP.z)) /
				(sqrtf((float)(pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.x, 2) + pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.y, 2) + pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.z, 2))) *
					sqrtf((float)(pow((double)vecP.x, 2) + pow((double)vecP.y, 2) + pow(vecP.z, 2))));
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fUpDown;
}

//=============================================================================
// ブロック法線取得処理										(public)	*** CSceneMesh ***
//=============================================================================
float CSceneMesh::GetBlockNor(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtxNum[4];					// 使用頂点の番号を保存
	bool bRange_Block = false;			// ブロック範囲内か
	float fNor = 0.0f;					// 坂

										// 現在のブロックを確認
	int nHitBlock =
		(int)((pos.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
		(int)((int)(((pos.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;			// 各ラインチェック

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{// ブロック内の４頂点をカウント
			D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
			int nData = 0, nChack1 = 0, nChack2 = 0;	// 値調整用（次のカウント、誤差解消用）
			int nVtxNum_N = 0, nVtxNum = 0;				// 使用する頂点番号（次の頂点番号、現在の頂点番号）

			nData = (nCntVtx + 1) % 4;					// 次のカウントを見る

														// カウント順の誤差をなくす（2*2 = 0134; for = 0143 → 0134）
			if (nCntVtx == 2) { nChack1 = 1; }
			else if (nCntVtx == 3) { nChack1 = -1; }
			if (nData == 2) { nChack2 = 1; }
			else if (nData == 3) { nChack2 = -1; }

			// 頂点番号を計算
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// 頂点番号を保存
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// ベクトル計算
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = pos - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）にいることを確認
				nCntLand_Block++;						// チェック数をカウント

				if (nCntLand_Block == 4)
				{// チェック数が４になったら
					bRange_Block = true;				// ブロック内にいることを確定

					break;
				}
			}
		}

		if (bRange_Block == true)
		{// ブロック内にいたら
			D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
			int nVtxNum = 0;							// 使用する頂点番号

														// ベクトル計算
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = pos - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）にいることを確認
				nVtxNum = 2;
			}
			else
			{// 範囲内（左側）にいることを確認
				nVtxNum = 1;
			}

			fNor = pVtx[nCntVtxNum[nVtxNum]].nor.y;
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fNor;
}

//=============================================================================
// 高さ設定処理										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

								// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	for (int nCount_yz = 0; nCount_yz < m_num.z + 1; nCount_yz++)
	{// yzをカウント
		for (int nCount_x = 0; nCount_x < m_num.x + 1; nCount_x++)
		{// xをカウント
			float fLenght = sqrtf((float)(pow((double)(pVtx[0].pos.x - pos.x), 2) + pow((double)(pVtx[0].pos.z - pos.z), 2)));

			if (fLenght < fRange)
			{
				float fPercent = fLenght / fRange;

				//pVtx[0].pos.y += sinf(((D3DX_PI * 0.5f) * (1.0f - fPercent))) * fValue;
				pVtx[0].pos.y += cosf(((D3DX_PI * 0.5f) * (fPercent))) * fValue;

			}
			pVtx += 1;
		}
	}
}

//=============================================================================
// 高さ取得処理										(public)	*** CSceneMesh ***
//=============================================================================
float CSceneMesh::GetHeight(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtxNum[4];					// 使用頂点の番号を保存
	D3DXVECTOR3 Hitnor[2];				// 使用頂点の法線を保存
	bool bRange_Block = false;			// ブロック範囲内か
	float fHeight = -500.0f;			// 高さ

	D3DXVECTOR3 posmtx = pos - m_pos;

	// 現在のブロックを確認
	int nHitBlock =
		(int)((posmtx.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
		(int)((int)(((posmtx.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;			// 各ラインチェック

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{// ブロック内の４頂点をカウント
			D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
			int nData = 0, nChack1 = 0, nChack2 = 0;	// 値調整用（次のカウント、誤差解消用）
			int nVtxNum_N = 0, nVtxNum = 0;				// 使用する頂点番号（次の頂点番号、現在の頂点番号）

			nData = (nCntVtx + 1) % 4;					// 次のカウントを見る

			// カウント順の誤差をなくす（2*2 = 0134; for = 0143 → 0134）
			if (nCntVtx == 2) { nChack1 = 1; }
			else if (nCntVtx == 3) { nChack1 = -1; }
			if (nData == 2) { nChack2 = 1; }
			else if (nData == 3) { nChack2 = -1; }

			// 頂点番号を計算
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// 頂点番号を保存
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// ベクトル計算
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = posmtx - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）にいることを確認
				nCntLand_Block++;						// チェック数をカウント

				if (nCntLand_Block == 4)
				{// チェック数が４になったら
					bRange_Block = true;				// ブロック内にいることを確定

					D3DXVECTOR3 nor[2], vec0[2], vec1[2];		// 法線計算用（正規化用、ベクトル）

																// 左下
					vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
					vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;
					// 右上
					vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
					vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;
					// 内積計算
					D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
					D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
					// 正規化
					D3DXVec3Normalize(&nor[0], &nor[0]);
					D3DXVec3Normalize(&nor[1], &nor[1]);

					// 法線の設定
					Hitnor[0] = nor[0];
					Hitnor[1] = nor[1];

					break;
				}
			}
		}

		if (bRange_Block == true)
		{// ブロック内にいたら
			D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
			int nVtxNum = 0;							// 使用する頂点番号

														// ベクトル計算
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = posmtx - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）にいることを確認
				nVtxNum = 2;
			}
			else
			{// 範囲内（左側）にいることを確認
				nVtxNum = 1;
			}

			// ベクトル計算
			D3DXVECTOR3 vecP = posmtx - pVtx[nCntVtxNum[nVtxNum]].pos;
			vecP.y -= pVtx[nCntVtxNum[nVtxNum]].pos.y;

			// 高さ計算
			//if ((((Hitnor[nVtxNum / 2 ^ 1].x * vecP.x) + (Hitnor[nVtxNum / 2 ^ 1].z * vecP.z)) / -Hitnor[nVtxNum / 2 ^ 1].y) + pVtx[nCntVtxNum[nVtxNum]].pos.y <= posmtx.y)
			{
				fHeight = (((Hitnor[nVtxNum / 2 ^ 1].x * vecP.x) + (Hitnor[nVtxNum / 2 ^ 1].z * vecP.z)) / -Hitnor[nVtxNum / 2 ^ 1].y) + pVtx[nCntVtxNum[nVtxNum]].pos.y;
				fHeight += m_pos.y;
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fHeight;
}

//=============================================================================
// 法線計算処理										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetVecNor(void)
{
	/*VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	typedef struct
	{// 各ブロックの法線計算用
		D3DXVECTOR3 nor[4];
	}NORDATA;

	// ブロック数分メモリ確保
	NORDATA *norData = NULL;
	norData = new NORDATA[((m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)];

	if (norData != NULL)
	{// NULL以外だったら
		for (int nCntBlock = 0; nCntBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2; nCntBlock++)
		{// ブロック数カウント
			int nCntVtxNum[4];		// 頂点の番号計算用

			// ブロックの頂点を調べる
			nCntVtxNum[0] = (0 % 2) + (((int)m_num.x + 1) * (0 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[1] = (1 % 2) + (((int)m_num.x + 1) * (1 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[2] = (2 % 2) + (((int)m_num.x + 1) * (2 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[3] = (3 % 2) + (((int)m_num.x + 1) * (3 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));

			D3DXVECTOR3 nor[2], vec0[2], vec1[2];			// 法線計算用（正規化用、ベクトル）

			// 左下
			vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
			vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;
			// 右上
			vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
			vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;
			// 内積計算
			D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
			D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
			// 正規化
			D3DXVec3Normalize(&nor[0], &nor[0]);
			D3DXVec3Normalize(&nor[1], &nor[1]);

			// 法線の保存
			norData[nCntBlock].nor[0] = (nor[0] + nor[1]) * 0.5f;
			norData[nCntBlock].nor[1] = nor[1];
			norData[nCntBlock].nor[2] = nor[0];
			norData[nCntBlock].nor[3] = (nor[0] + nor[1]) * 0.5f;
		}

		for (int nCntVtx = 0; nCntVtx < m_nMaxVtx; nCntVtx++)
		{// 頂点数カウント
			D3DXVECTOR3 nor;	// 法線計算結果
			int SetBlock[4];	// ブロック番号

			// 頂点に接するブロックを調べる
			SetBlock[0] = (nCntVtx - (int)(m_num.x + 1)) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[1] = (nCntVtx - (int)(m_num.x)) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[2] = (nCntVtx - 1) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[3] = (nCntVtx)-(nCntVtx / (int)(m_num.x + 1));

			if (nCntVtx != 0 && nCntVtx != 0 + m_num.x && nCntVtx != (m_nMaxVtx - 1) - m_num.x && nCntVtx != (m_nMaxVtx - 1))
			{// 角以外
				if (nCntVtx <= m_num.x)
				{// 上横ライン
					nor = (norData[SetBlock[2]].nor[1] + norData[SetBlock[3]].nor[2] + norData[SetBlock[3]].nor[1]) / 3;
				}
				else if (nCntVtx % (int)(m_num.x + 1) == 0)
				{// 左縦ライン
					nor = (norData[SetBlock[1]].nor[2] + norData[SetBlock[3]].nor[1] + norData[SetBlock[3]].nor[2]) / 3;
				}
				else if (nCntVtx % (int)(m_num.x + 1) == (int)(m_num.x))
				{// 右縦ライン
					nor = (norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2] + norData[SetBlock[2]].nor[1]) / 3;
				}
				else if ((m_nMaxVtx - 1) - m_num.x <= nCntVtx)
				{// 下横ライン
					nor = (norData[SetBlock[0]].nor[2] + norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2]) / 3;
				}
				else
				{// その他内側
					nor = (norData[SetBlock[0]].nor[2] + norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2] +
						norData[SetBlock[2]].nor[1] + norData[SetBlock[3]].nor[1] + norData[SetBlock[3]].nor[2]) / 6;
				}
			}
			else
			{// 角
				if (nCntVtx == 0)
				{// 左上
					nor = norData[SetBlock[3]].nor[0];
				}
				else if (nCntVtx == 0 + m_num.x)
				{// 右上
					nor = norData[SetBlock[2]].nor[1];
				}
				else if (nCntVtx == (m_nMaxVtx - 1) - m_num.x)
				{// 左下
					nor = norData[SetBlock[1]].nor[2];
				}
				else if (nCntVtx == (m_nMaxVtx - 1))
				{// 右下
					nor = norData[SetBlock[0]].nor[3];
				}
			}

			// 法線の設定
			pVtx[nCntVtx].nor = nor;
		}

		if (norData != NULL)
		{// NULL以外の場合
			delete[] norData;	// 破棄
			norData = NULL;		// NULLへ
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();*/
}

//=============================================================================
// サイン更新処理										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine)
{
	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);			// フォント取得

	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

								// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posStart;

	switch (sinStart)
	{
	case SINSTART_POS:
		posStart = pos;
		break;
	case SINSTART_CENTER:
		posStart = m_size;
		break;
	case SINSTART_LU:
		posStart = pVtx[0].pos;
		break;
	case SINSTART_RU:
		posStart = pVtx[(int)(m_num.x + 1)].pos;
		break;
	case SINSTART_LD:
		posStart = pVtx[(m_nMaxVtx - 1) - (int)(m_num.x + 1)].pos;
		break;
	case SINSTART_RD:
		posStart = pVtx[(m_nMaxVtx - 1)].pos;
		break;
	}

	float fMax;

	switch (sinType)
	{
	case SINTYPE_CIRCLE:
		fMax = sqrtf((float)(pow((double)m_size.x, 2) + pow((double)m_size.z, 2)));
		break;
	case SINTYPE_RIGHT:
		fMax = m_size.x * 2.0f;
		break;
	case SINTYPE_LEFT:
		fMax = m_size.x * 2.0f;
		break;
	case SINTYPE_UP:
		fMax = m_size.z * 2.0f;
		break;
	case SINTYPE_DOWN:
		fMax = m_size.z * 2.0f;
		break;
	}

	// 頂点情報の設定
	for (int nCount_yz = 0; nCount_yz < m_num.z + 1; nCount_yz++)
	{// yzをカウント
		for (int nCount_x = 0; nCount_x < m_num.x + 1; nCount_x++)
		{// xをカウント
			float fnow = 0.0f;

			switch (sinType)
			{
			case SINTYPE_CIRCLE:
				fnow = sqrtf((float)(pow((double)(pVtx[0].pos.x - posStart.x), 2) + pow((double)(pVtx[0].pos.z - posStart.z), 2)));
				break;
			case SINTYPE_RIGHT:
				fnow = pVtx[0].pos.x - posStart.x;
				frot *= -1;
				break;
			case SINTYPE_LEFT:
				fnow = pVtx[0].pos.x - posStart.x;
				break;
			case SINTYPE_UP:
				fnow = pVtx[0].pos.z - posStart.z;
				break;
			case SINTYPE_DOWN:
				fnow = pVtx[0].pos.z - posStart.z;
				break;
			}

			float fPP = fnow / fMax;

			pVtx[0].pos.y = sinf(((D3DX_PI) * (fPP * fLine)) + frot) * (fUp/* * (1.2f - fPP)*/);

			pVtx += 1;
		}
	}

	// 法線を計算
	SetVecNor();
}
void CSceneMesh::SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine, float fRange)
{
	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);			// フォント取得

	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

								// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posStart;

	switch (sinStart)
	{
	case SINSTART_POS:
		posStart = pos - m_pos;
		break;
	case SINSTART_CENTER:
		posStart = m_size;
		break;
	case SINSTART_LU:
		posStart = pVtx[0].pos;
		break;
	case SINSTART_RU:
		posStart = pVtx[(int)(m_num.x + 1)].pos;
		break;
	case SINSTART_LD:
		posStart = pVtx[(m_nMaxVtx - 1) - (int)(m_num.x + 1)].pos;
		break;
	case SINSTART_RD:
		posStart = pVtx[(m_nMaxVtx - 1)].pos;
		break;
	}

	float fMax;

	switch (sinType)
	{
	case SINTYPE_CIRCLE:
		fMax = sqrtf((float)(pow((double)m_size.x, 2) + pow((double)m_size.z, 2)));
		break;
	case SINTYPE_RIGHT:
		fMax = m_size.x * 2.0f;
		break;
	case SINTYPE_LEFT:
		fMax = m_size.x * 2.0f;
		break;
	case SINTYPE_UP:
		fMax = m_size.z * 2.0f;
		break;
	case SINTYPE_DOWN:
		fMax = m_size.z * 2.0f;
		break;
	}

	// 頂点情報の設定
	for (int nCount_yz = 0; nCount_yz < m_num.z + 1; nCount_yz++)
	{// yzをカウント
		for (int nCount_x = 0; nCount_x < m_num.x + 1; nCount_x++)
		{// xをカウント
			float fLenght = sqrtf((float)(pow((double)(pVtx[0].pos.x - posStart.x), 2) + pow((double)(pVtx[0].pos.z - posStart.z), 2)));

			if (fLenght < fRange)
			{
				float fPercent = fLenght / fRange;
				float fnow = 0.0f;

				pVtx[0].pos.y = sinf(((D3DX_PI) * (fPercent * fLine)) + frot) * (fUp * (1.0f - fPercent));
			}

			pVtx += 1;
		}
	}

	// 法線を計算
	SetVecNor();
}