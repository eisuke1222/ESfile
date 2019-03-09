//=============================================================================
//
// シーンメッシュオービット処理 [scenemeshorbit.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scenemeshskin.h"	// シーンメッシュスキン
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "font.h"			// フォント

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_NAME		"data\\TEXTURE\\MODE\\GAME\\gradation005.jpg"

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
// コンストラクタ								(public)	*** CSceneMeshSkin ***
//=============================================================================
CSceneMeshSkin::CSceneMeshSkin(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture	= NULL;										// テクスチャ情報へのポインタ
	m_pVtxBuff	= NULL;										// 頂点バッファ情報へのポインタ

	for (int nCount = 0; nCount < MAX_VTX; nCount++)
		m_aPosVtx[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	for (int nCount = 0; nCount < MAX_OFFSET; nCount++)
		m_aOffSet[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// オフセット
}

//=============================================================================
// デストラクタ								(public)	*** CSceneMeshSkin ***
//=============================================================================
CSceneMeshSkin::~CSceneMeshSkin()
{

}

//=============================================================================
// ロード処理								(public)	*** CSceneMeshSkin ***
//=============================================================================
HRESULT CSceneMeshSkin::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理								(public)	*** CSceneMeshSkin ***
//=============================================================================
void CSceneMeshSkin::Unload(void)
{

}

//=============================================================================
// 確保処理									(public)	*** CSceneMeshSkin ***
//=============================================================================
CSceneMeshSkin *CSceneMeshSkin::Create(D3DXVECTOR3 OffSet_0, D3DXVECTOR3 OffSet_1)
{
	CSceneMeshSkin *pSkin = NULL;			// ポインタ

	if (pSkin == NULL)
	{// NULLの場合
		pSkin = new CSceneMeshSkin(3, CScene::OBJTYPE_MESHFIELD);		// メモリ確保
	}

	if (pSkin != NULL)
	{// NULL以外の場合
		pSkin->m_aOffSet[0] = OffSet_0;	// オフセット０
		pSkin->m_aOffSet[1] = OffSet_1;	// オフセット１
		pSkin->Init();					// 初期化処理
	}

	return pSkin;		// 値を返す
}

//=============================================================================
// 初期化処理								(public)	*** CSceneMeshSkin ***
//=============================================================================
HRESULT CSceneMeshSkin::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイス取得

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VTX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_VTX; nCount++)
	{// 頂点数カウント
		// 頂点情報を設定
		//pVtx[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 法線の設定
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 頂点カラー設定
		pVtx[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (nCount % 2 == 0)
		{// 上
			// 頂点情報を設定
			pVtx[nCount].pos = D3DXVECTOR3(50.0f * (nCount / 2), 100.0f, 0.0f);
			// テクスチャ設定
			pVtx[nCount].tex = D3DXVECTOR2(((1.0f / (MAX_VTX * 0.5f)) * (nCount / 2)), 1.0f);
		}
		else
		{// 下
			// 頂点情報を設定
			pVtx[nCount].pos = D3DXVECTOR3(50.0f * (nCount / 2), 0.0f, 0.0f);
			// テクスチャ設定
			pVtx[nCount].tex = D3DXVECTOR2(((1.0f / (MAX_VTX * 0.5f)) * (nCount / 2)), 0.0f);
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CSceneMeshSkin ***
//=============================================================================
void CSceneMeshSkin::Uninit(void)
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
// 更新処理										(public)	*** CSceneMeshSkin ***
//=============================================================================
void CSceneMeshSkin::Update(void)
{
	static D3DXVECTOR3 pos[2];
	D3DXVECTOR3 skin;

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_6))
	{
		pos[0].z -= 2.0f;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_7))
	{
		pos[0].z += 2.0f;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_8))
	{
		pos[1].z -= 2.0f;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_9))
	{
		pos[1].z += 2.0f;
	}

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_VTX; nCount++)
	{// カウント
		if (nCount / 2 == 0)
		{
			skin.x = ((pos[0].x + pVtx[nCount].pos.x) * 1.0f) + ((pos[1].x + pVtx[nCount].pos.x) * 0.0f);
			skin.y = ((pos[0].y + pVtx[nCount].pos.y) * 1.0f) + ((pos[1].y + pVtx[nCount].pos.y) * 0.0f);
			skin.z = ((pos[0].z + pVtx[nCount].pos.z) * 1.0f) + ((pos[1].z + pVtx[nCount].pos.z) * 0.0f);
		}
		else if (nCount / 2 == 1)
		{
			skin.x = ((pos[0].x + pVtx[nCount].pos.x) * 0.2f) + ((pos[1].x + pVtx[nCount].pos.x) * 0.8f);
			skin.y = ((pos[0].y + pVtx[nCount].pos.y) * 0.2f) + ((pos[1].y + pVtx[nCount].pos.y) * 0.8f);
			skin.z = ((pos[0].z + pVtx[nCount].pos.z) * 0.2f) + ((pos[1].z + pVtx[nCount].pos.z) * 0.8f);
		}
		else if (nCount / 2 == 2)
		{
			skin.x = ((pos[0].x + pVtx[nCount].pos.x) * 0.0f) + ((pos[1].x + pVtx[nCount].pos.x) * 1.0f);
			skin.y = ((pos[0].y + pVtx[nCount].pos.y) * 0.0f) + ((pos[1].y + pVtx[nCount].pos.y) * 1.0f);
			skin.z = ((pos[0].z + pVtx[nCount].pos.z) * 0.0f) + ((pos[1].z + pVtx[nCount].pos.z) * 1.0f);
		}

		// 頂点情報を設定		
		pVtx[nCount].pos = skin;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理										(public)	*** CSceneMeshSkin ***
//=============================================================================
void CSceneMeshSkin::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイス取得

	D3DXMATRIX mtxWorld;												// 計算用マトリックス

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 両面をカリング

	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MAX_VTX - 2);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
}