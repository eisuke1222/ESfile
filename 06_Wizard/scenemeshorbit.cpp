//=============================================================================
//
// シーンメッシュオービット処理 [scenemeshorbit.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scenemeshorbit.h"	// シーンメッシュオービット
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "font.h"			// フォント

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
// コンストラクタ								(public)	*** CSceneMeshOrbit ***
//=============================================================================
CSceneMeshOrbit::CSceneMeshOrbit(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture		= NULL;										// テクスチャ情報へのポインタ
	m_pVtxBuff		= NULL;										// 頂点バッファ情報へのポインタ

	m_nMaxVtx		= 0;										// 最大頂点数

	m_aPosVtx		= NULL;										// 頂点位置
	m_fMaxcol_a		= 1.0f;										// 最大透明度
	m_fMincol_a		= 0.0f; 									// 最小透明度
																
	for (int nCount = 0; nCount < m_MAXOFFSET; nCount++)
		m_aOffSet[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// オフセット位置
}

//=============================================================================
// デストラクタ								(public)	*** CSceneMeshOrbit ***
//=============================================================================
CSceneMeshOrbit::~CSceneMeshOrbit()
{

}

//=============================================================================
// ロード処理								(public)	*** CSceneMeshOrbit ***
//=============================================================================
HRESULT CSceneMeshOrbit::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理								(public)	*** CSceneMeshOrbit ***
//=============================================================================
void CSceneMeshOrbit::Unload(void)
{

}

//=============================================================================
// 確保処理									(public)	*** CSceneMeshOrbit ***
//=============================================================================
CSceneMeshOrbit *CSceneMeshOrbit::Create(D3DXVECTOR3 OffSet_0, D3DXVECTOR3 OffSet_1, D3DXVECTOR3 col_rgb, float fMaxcol_a, float fMincol_a, int nMaxVtx, LPDIRECT3DTEXTURE9 pTexture)
{
	CSceneMeshOrbit *pOrbit = NULL;			// ポインタ

	if (pOrbit == NULL)
	{// NULLの場合
		pOrbit = new CSceneMeshOrbit(CScene::PRIORITY_3, CScene::OBJTYPE_MESHFIELD);	// メモリ確保
	}

	if (pOrbit != NULL)
	{// NULL以外の場合
		pOrbit->m_pTexture		= pTexture;		// テクスチャ
		pOrbit->m_nMaxVtx		= nMaxVtx;		// 最大頂点数
		pOrbit->m_aOffSet[0]	= OffSet_0;		// オフセット０
		pOrbit->m_aOffSet[1]	= OffSet_1;		// オフセット１
		pOrbit->m_col_rgb		= col_rgb;		// 色
		pOrbit->m_fMaxcol_a		= fMaxcol_a;	// 最大透明度
		pOrbit->m_fMincol_a		= fMincol_a;	// 最小透明度

		if (pOrbit->m_aPosVtx == NULL)
			pOrbit->m_aPosVtx = new D3DXVECTOR3[nMaxVtx];		// メモリ確保

		pOrbit->Init();		// 初期化処理
	}

	return pOrbit;		// 値を返す
}

//=============================================================================
// 初期化処理									(public)	*** CSceneMeshOrbit ***
//=============================================================================
HRESULT CSceneMeshOrbit::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// デバイス取得

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nMaxVtx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < m_nMaxVtx; nCount++)
	{
		// 頂点情報を設定
		pVtx[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 法線の設定
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (nCount % 2 == 0)
		{// 上
			// 頂点カラー設定
			pVtx[nCount].col = D3DXCOLOR(m_col_rgb.x, m_col_rgb.y, m_col_rgb.z, (m_fMincol_a - ((m_fMincol_a / (m_nMaxVtx / 2)) * (nCount / 2))));
			// テクスチャ設定
			pVtx[nCount].tex = D3DXVECTOR2(((1.0f / (m_nMaxVtx * 0.5f)) * (nCount / 2)), 1.0f);
		}
		else
		{// 下
			// 頂点カラー設定
			pVtx[nCount].col = D3DXCOLOR(m_col_rgb.x, m_col_rgb.y, m_col_rgb.z, (m_fMaxcol_a - ((m_fMaxcol_a / (m_nMaxVtx / 2)) * (nCount / 2))));
			// テクスチャ設定
			pVtx[nCount].tex = D3DXVECTOR2(((1.0f / (m_nMaxVtx * 0.5f)) * (nCount / 2)), 0.0f);
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理									(public)	*** CSceneMeshOrbit ***
//=============================================================================
void CSceneMeshOrbit::Uninit(void)
{
	if (m_aPosVtx != NULL)
	{// NULL以外の場合
		delete[] m_aPosVtx;			// 開放
		m_aPosVtx = NULL;			// NULLへ
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{// NULL以外の場合
		m_pVtxBuff->Release();		// 解放
		m_pVtxBuff = NULL;			// NULLへ
	}

	CScene::SetDeath();		// 死亡フラグ
}

//=============================================================================
// 更新処理									(public)	*** CSceneMeshOrbit ***
//=============================================================================
void CSceneMeshOrbit::Update(void)
{
	// 入れ替え
	for (int nCount = m_nMaxVtx - 1; 1 < nCount; nCount--)
	{// 頂点数カウント
		m_aPosVtx[nCount] = m_aPosVtx[nCount - 2];
	}

	// オフセット位置設定
	for (int nCount = 0; nCount < m_MAXOFFSET; nCount++)
	{// オフセット数カウント
		D3DXVec3TransformCoord(&m_aPosVtx[nCount], &m_aOffSet[nCount], m_pMtxParent);
	}

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < m_nMaxVtx; nCount++)
	{// 頂点数カウント
		// 頂点情報を設定		
		pVtx[nCount].pos = m_aPosVtx[nCount];

		if (nCount % 2 == 0)
		{// 上
		 // 頂点カラー設定
			pVtx[nCount].col = D3DXCOLOR(m_col_rgb.x, m_col_rgb.y, m_col_rgb.z, (m_fMincol_a - ((m_fMincol_a / (m_nMaxVtx / 2)) * (nCount / 2))));
		}
		else
		{// 下
		 // 頂点カラー設定
			pVtx[nCount].col = D3DXCOLOR(m_col_rgb.x, m_col_rgb.y, m_col_rgb.z, (m_fMaxcol_a - ((m_fMaxcol_a / (m_nMaxVtx / 2)) * (nCount / 2))));
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理									(public)	*** CSceneMeshOrbit ***
//=============================================================================
void CSceneMeshOrbit::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();	// デバイス取得

	D3DXMATRIX			mtxWorld;										// 計算用マトリックス

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 両面をカリング

	// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	pDevice->SetTexture(0, NULL);
	//pDevice->SetTexture(0, m_pTexture);
	
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nMaxVtx - 2);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
}