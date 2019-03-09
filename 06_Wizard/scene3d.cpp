//=============================================================================
//
// シーン3D処理 [scene3d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3d.h"		// シーン3D
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
// コンストラクタ									(public)	*** CScene3D ***
//=============================================================================
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture		= NULL;									// テクスチャ情報へのポインタ
	m_pVtxBuff		= NULL;									// 頂点バッファ情報へのポインタ

	m_bVtxSize		= false;								// 頂点位置、大きさ調整

	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ
	m_col			= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_TexUV			= D3DXVECTOR2(0.0f, 0.0f);				// UV

	m_bDraw			= true;									// 描画

	m_bDrawAdd		= false;								// 加算合成
	m_bDrawSubtract = false;								// 減産合成
	m_bDrawATest	= false;								// αテスト
	m_bDrawZWrite	= false;								// Zライト
	m_bDrawLight	= false;								// ライト影響
	m_nATest		= 1;									// αテスト数値
	m_nCullMode		= D3DCULL_CCW;							// カリング
	m_dCmpValue		= D3DCMP_LESSEQUAL;						// Zライト設定

	m_scene3dType	= SCENE3DTYPE_NORMAL;					// 種類
}

//=============================================================================
// デストラクタ									(public)	*** CScene3D ***
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
// 初期化処理									(public)	*** CScene3D ***
//=============================================================================
HRESULT CScene3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// デバイス取得

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);

	/*D3DXVECTOR3 nor[2], vec0[2], vec1[2];		// 法線計算用（正規化用、ベクトル）

	// 左下
	vec0[0] = pVtx[1].pos - pVtx[0].pos;
	vec0[1] = pVtx[2].pos - pVtx[0].pos;
	// 右上
	vec1[0] = pVtx[2].pos - pVtx[3].pos;
	vec1[1] = pVtx[1].pos - pVtx[3].pos;
	// 内積計算
	D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
	D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
	// 正規化
	D3DXVec3Normalize(&nor[0], &nor[0]);
	D3DXVec3Normalize(&nor[1], &nor[1]);

	// 法線の設定
	pVtx[0].nor = nor[0];
	pVtx[1].nor = (nor[0] + nor[1]) * 0.5f;
	pVtx[2].nor = (nor[0] + nor[1]) * 0.5f;
	pVtx[3].nor = nor[1];*/
	if (m_size.z != 0.0f)
	{// 地面
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	if (m_size.y != 0.0f)
	{// 壁
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
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

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CScene3D ***
//=============================================================================
void CScene3D::Uninit(void)
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
// 更新処理										(public)	*** CScene3D ***
//=============================================================================
void CScene3D::Update(void)
{
	// 回転調整
	if (m_rot.x < -D3DX_PI)		m_rot.x += D3DX_PI * 2.0f;
	if (D3DX_PI < m_rot.x)		m_rot.x -= D3DX_PI * 2.0f;
	if (m_rot.y < -D3DX_PI)		m_rot.y += D3DX_PI * 2.0f;
	if (D3DX_PI < m_rot.y)		m_rot.y -= D3DX_PI * 2.0f;
	if (m_rot.z < -D3DX_PI)		m_rot.z += D3DX_PI * 2.0f;
	if (D3DX_PI < m_rot.z)		m_rot.z -= D3DX_PI * 2.0f;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bVtxSize == false)
	{// 頂点変更していない
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);
	}
	// 頂点カラー設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理										(public)	*** CScene3D ***
//=============================================================================
void CScene3D::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX			mtxRot, mtxTrans, mtxView;							// 計算用マトリックス

	if (m_bDraw == true)
	{
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
		if (m_bDrawZWrite == true)
		{// Zライト
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, m_dCmpValue);
		}
		if (m_bDrawLight == true)
		{// ライト影響
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		}

		pDevice->SetRenderState(D3DRS_CULLMODE, m_nCullMode);				// 裏面をカリング

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		if (m_scene3dType == SCENE3DTYPE_NORMAL)
		{
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
		}
		else if (m_scene3dType == SCENE3DTYPE_BILLBOARD)
		{//	ビルボード
			// 逆行列
			m_mtxWorld._11 = mtxView._11;
			m_mtxWorld._12 = mtxView._21;
			m_mtxWorld._13 = mtxView._31;
			m_mtxWorld._21 = mtxView._12;
			m_mtxWorld._22 = mtxView._22;
			m_mtxWorld._23 = mtxView._32;
			m_mtxWorld._31 = mtxView._13;
			m_mtxWorld._32 = mtxView._23;
			m_mtxWorld._33 = mtxView._33;

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
		}

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマット
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャ設定
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
		if (m_bDrawZWrite == true)
		{// Zライト
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
		if (m_bDrawLight == true)
		{// ライト影響
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	}
}

//=============================================================================
// 頂点位置、大きさ調整処理						(public)	*** CScene3D ***
//=============================================================================
void CScene3D::SetVtxSize(D3DXVECTOR3 *VtxSize)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = VtxSize[0];
	pVtx[1].pos = VtxSize[1];
	pVtx[2].pos = VtxSize[2];
	pVtx[3].pos = VtxSize[3];

	if (m_bVtxSize == false)
		m_bVtxSize = true;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点位置、大きさ取得処理						(public)	*** CScene3D ***
//=============================================================================
void CScene3D::GetVtxSize(D3DXVECTOR3 *VtxSize)
{
	D3DXVECTOR3 pos[4];

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	VtxSize[0] = pVtx[0].pos;
	VtxSize[1] = pVtx[1].pos;
	VtxSize[2] = pVtx[2].pos;
	VtxSize[3] = pVtx[3].pos;

	if (m_bVtxSize == false)
		m_bVtxSize = true;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// アニメーション処理							(public)	*** CScene3D ***
//=============================================================================
void CScene3D::SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV)
{
	if (nAnimLineNum <= 0) nAnimLineNum = 1;	// 番号調整

	m_TexUV = TexUV;	// UV変更

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

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

//=============================================================================
// 頂点取得処理								(public)	*** CScene3D ***
//=============================================================================
D3DXVECTOR3 CScene3D::GetVtx(int nNum)
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
// 法線取得処理								(public)	*** CScene3D ***
//=============================================================================
D3DXVECTOR3 CScene3D::GetNor(int nNum)
{
	D3DXVECTOR3 nor;	// 法線

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 法線を設定
	nor = pVtx[nNum].nor;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return nor;		// 値を返す
}

//=============================================================================
// 高さ取得処理								(public)	*** CScene3D ***
//=============================================================================
float CScene3D::GetHeight(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fHeight = 0.0f;						// 高さ

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_DATA);

	for (int nCntPolygon = 0; nCntPolygon < 2; nCntPolygon++)
	{// ポリゴン数カウント
		bool bRange = false;					// ポリゴン範囲内か
		int nCntLand = 0;						// 各ラインチェック

		for (int nCntVtx = 0; nCntVtx < 3; nCntVtx++)
		{// ポリゴン内の３頂点をカウント
			D3DXVECTOR3 vecA, vecC;				// 計算用ベクトル（頂点同士、頂点とPOS）
			int nChack1 = 0, nChack2 = 0;		// 値調整用（誤差解消用）

			// カウント順の誤差をなくす（1*1 = 013; for = 031 → 013）
			if (nCntVtx + 1 == 2) { nChack1 = 1; }
			else if (nCntVtx + 1 == 3) { nChack1 = -1; }
			if (nCntVtx + 2 == 2) { nChack2 = 1; }
			else if (nCntVtx + 2 == 3) { nChack2 = -1; }

			// ベクトル計算
			vecA = pVtx[((nCntVtx + 1) % 3 + nCntPolygon) + (nChack2 * (nCntPolygon % 2))].pos - pVtx[((nCntVtx) % 3 + nCntPolygon) + (nChack1 * (nCntPolygon % 2))].pos;
			vecC = pos - pVtx[((nCntVtx) % 3 + nCntPolygon) + (nChack1 * (nCntPolygon % 2))].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）にいることを確認
				nCntLand++;						// チェック数をカウント

				if (nCntLand == 3)
				{// チェック数が３になったら
					bRange = true;				// ポリゴン内にいることを確定

					int nVtxNum = 0;			// 使用頂点の番号

					if (nCntPolygon % 2 == 0)
					{// 左下
						nVtxNum = ((0) % 3 + nCntPolygon);
					}
					else
					{// 右上
						nVtxNum = ((2) % 3 + nCntPolygon);
					}

					// ベクトル計算
					D3DXVECTOR3 vecP = pos - pVtx[nVtxNum].pos;
					vecP.y -= pVtx[nVtxNum].pos.y;

					//pFont->SetMessage(CFont::SETTYPE_ADD, "nVtxNum[%d]\n\n", nVtxNum);

					// 高さ計算
					fHeight = (((pVtx[nVtxNum].nor.x * vecP.x) + (pVtx[nVtxNum].nor.z * vecP.z)) / -pVtx[nVtxNum].nor.y) + pVtx[nVtxNum].pos.y;

					//pFont->SetMessage(CFont::SETTYPE_ADD, "Polygon3D : 判定[%d] [%d]\n\n", nCntPolygon, bRange);
				}
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fHeight;		// 値を返す
}

//=============================================================================
// ビルボード回転処理								(public)	*** CScene3D ***
//=============================================================================
void CScene3D::SetBillRot(float rot)
{
	// 回転調整
	if (rot < -D3DX_PI)	rot += D3DX_PI * 2.0f;
	if (D3DX_PI < rot)	rot -= D3DX_PI * 2.0f;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(rot) * m_size.y - cosf(rot) * m_size.x,	 m_pos.y + sinf(rot) * m_size.x + cosf(rot) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(rot) * m_size.y + cosf(rot) * m_size.x,	 m_pos.y - sinf(rot) * m_size.x + cosf(rot) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - sinf(rot) * m_size.y - cosf(rot) * m_size.x,	 m_pos.y + sinf(rot) * m_size.x - cosf(rot) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - sinf(rot) * m_size.y + cosf(rot) * m_size.x,	 m_pos.y - sinf(rot) * m_size.x - cosf(rot) * m_size.y, 0.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}