//=============================================================================
//
// シーン2D処理 [scene2d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2d.h"		// シーン2D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力

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
// コンストラクタ								(public)	*** CScene2D ***
//=============================================================================
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture		= NULL;									// テクスチャ情報へのポインタ
	m_pVtxBuff		= NULL;									// マテリアル情報へのポインタ

	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot			= 0.0f;									// 向き
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ
	m_col			= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_TexUV			= D3DXVECTOR2(0.0f, 0.0f);				// UV

	m_bDrawAdd		= false;								// 加算合成
	m_bDrawSubtract = false;								// 減産合成
	m_bDrawATest	= false;								// αテスト
	m_nATest		= 1;									// αテスト数値
}

//=============================================================================
// デストラクタ									(public)	*** CScene2D ***
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 初期化処理									(public)	*** CScene2D ***
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラー設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(m_TexUV.x - m_TexUV.x, m_TexUV.y - m_TexUV.y);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x,			 m_TexUV.y - m_TexUV.y);
	pVtx[2].tex = D3DXVECTOR2(m_TexUV.x - m_TexUV.x, m_TexUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x,			 m_TexUV.y);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CScene2D ***
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{// NULL以外の場合
		m_pVtxBuff->Release();		// 解放
		m_pVtxBuff = NULL;			// NULLへ
	}

	CScene::SetDeath();		// 死亡フラグ
}

//=============================================================================
// 更新処理										(public)	*** CScene2D ***
//=============================================================================
void CScene2D::Update(void)
{
	// 回転調整
	if (m_rot < -D3DX_PI)	m_rot += D3DX_PI * 2.0f;
	if (D3DX_PI < m_rot)	m_rot -= D3DX_PI * 2.0f;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	// 頂点カラー設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理										(public)	*** CScene2D ***
//=============================================================================
void CScene2D::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_bDrawAdd == true)
	{// 加算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	if (m_bDrawSubtract == true)
	{// 減算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	if (m_bDrawATest == true)
	{// アルファテスト
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, m_nATest);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (m_bDrawAdd == true)
	{// 加算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	if (m_bDrawSubtract == true)
	{// 減算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	if (m_bDrawATest == true)
	{// アルファテスト
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

//=============================================================================
// アニメーション処理							(public)	*** CScene2D ***
//=============================================================================
void CScene2D::SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV)
{
	if (nAnimLineNum <= 0) nAnimLineNum = 1;	// 番号調整

	m_TexUV = TexUV;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum)				+ TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum)			 + TexMoveUV.y);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum) + m_TexUV.x + TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum)			 + TexMoveUV.y);
	pVtx[2].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum)				+ TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum) + m_TexUV.y + TexMoveUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum) + m_TexUV.x + TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum) + m_TexUV.y + TexMoveUV.y);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ								(public)	*** CScene2D_Preset ***
//=============================================================================
CScene2D_Preset::CScene2D_Preset(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// 初期値設定
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// 種類カウント
		m_nType[nCntType] = TYPE_NONE;			// 種類
		m_nCntFrame[nCntType] = 0;					// フレームカウント
		m_nMaxFrame[nCntType] = 0;					// フレームの最大値
		m_colDest_1[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 目標の色1
		m_colDest_2[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 目標の色2
		m_sizeDest[nCntType]  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		// 目標の大きさ
	}
}

//=============================================================================
// デストラクタ								(public)	*** CScene2D_Preset ***
//=============================================================================
CScene2D_Preset::~CScene2D_Preset()
{

}

//=============================================================================
// 初期化処理								(public)	*** CScene2D_Preset ***
//=============================================================================
HRESULT CScene2D_Preset::Init(void)
{
	CScene2D::Init();		// 初期化処理

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理									(public)	*** CScene2D_Preset ***
//=============================================================================
void CScene2D_Preset::Uninit(void)
{
	CScene2D::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理									(public)	*** CScene2D_Preset ***
//=============================================================================
void CScene2D_Preset::Update(void)
{
	Preset();				// プリセット

	CScene2D::Update();		// 更新処理
}

//=============================================================================
// 描画処理									(public)	*** CScene2D_Preset ***
//=============================================================================
void CScene2D_Preset::Draw(void)
{
	CScene2D::Draw();		// 描画処理
}

//=============================================================================
// プリセット処理								(public)	*** CScene2D_Preset ***
//=============================================================================
void CScene2D_Preset::Preset(void)
{
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// 種類カウント
		switch (m_nType[nCntType])
		{// 種類
		case TYPE_NONE:		// 何もしていない
		{
			// 初期化
			m_nCntFrame[nCntType] = 0;	// フレーム初期化
			m_nMaxFrame[nCntType] = 0;	// フレーム最大値初期化
			break;
		}
		case TYPE_COL:		// 色変更
		{
			D3DXCOLOR col = CScene2D::Getcol();			// 色取得

			if (m_nCntFrame[nCntType] == 0)
			{
				m_colDest_1[nCntType].r = (m_colDest_1[nCntType].r - col.r);
				m_colDest_1[nCntType].g = (m_colDest_1[nCntType].g - col.g);
				m_colDest_1[nCntType].b = (m_colDest_1[nCntType].b - col.b);
				m_colDest_1[nCntType].a = (m_colDest_1[nCntType].a - col.a);
			}

			m_nCntFrame[nCntType]++;		// フレームカウント

			col.r += (m_colDest_1[nCntType].r) / (float)m_nMaxFrame[nCntType];
			col.g += (m_colDest_1[nCntType].g) / (float)m_nMaxFrame[nCntType];
			col.b += (m_colDest_1[nCntType].b) / (float)m_nMaxFrame[nCntType];
			col.a += (m_colDest_1[nCntType].a) / (float)m_nMaxFrame[nCntType];

			if (m_nCntFrame[nCntType] == m_nMaxFrame[nCntType])
			{// 最大フレームになったら
				m_nCntFrame[nCntType] = 0;
				m_nMaxFrame[nCntType] = 0;
				m_colDest_1[nCntType] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				m_nType[nCntType] = TYPE_NONE;
			}

			CScene2D::Setcol(col);						// 色設定

			break;
		}
		case TYPE_SIZE:		// 大きさ変更
		{
			D3DXVECTOR3 size = CScene2D::Getsize();		// サイズ取得

			if (m_nCntFrame[nCntType] == 0)
			{
				m_sizeDest[nCntType] = (m_sizeDest[nCntType] - size);
			}

			m_nCntFrame[nCntType]++;		// フレームカウント

			size.x += (m_sizeDest[nCntType].x) / (float)m_nMaxFrame[nCntType];
			size.y += (m_sizeDest[nCntType].y) / (float)m_nMaxFrame[nCntType];
			size.z += (m_sizeDest[nCntType].z) / (float)m_nMaxFrame[nCntType];

			if (m_nCntFrame[nCntType] == m_nMaxFrame[nCntType])
			{// 最大フレームになったら
				m_nCntFrame[nCntType] = 0;
				m_nMaxFrame[nCntType] = 0;
				m_sizeDest[nCntType] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_nType[nCntType] = TYPE_NONE;
			}

			CScene2D::Setsize(size);					// サイズ設定

			break;
		}
		case TYPE_FLASING:	// 点滅
		{

			break;
		}
		}
	}
}