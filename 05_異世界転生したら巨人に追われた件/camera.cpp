//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "camera.h"			// 背景
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "title.h"			// タイトル
#include "game.h"			// ゲーム
#include "titlelogo.h"		// タイトルロゴ

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
// コンストラクタ									(public)	*** CCamera ***
//=============================================================================
CCamera::CCamera()
{
	m_posV			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在視点
	m_posVDest		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標視点
	m_posR			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在注視点
	m_posRDest		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標注視点
	m_vecU			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 上方向ベクトル
	m_fDistance		= 0.0f;										// 距離

	m_bSetDest		= false;									// SetDest使用
	m_nCntSetDest	= 0;										// SetDestカウント
	m_nLimitSetDest = 0;										// SetDest終了
	m_posVDiff		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;			// 視点差分
	m_posRDiff		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;			// 注視点差分
	m_rotDiff		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;			// 角度差分
	m_fDistanceDiff = 0.0f;										// 距離差分
}

//=============================================================================
// デストラクタ										(public)	*** CCamera ***
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化処理										(public)	*** CCamera ***
//=============================================================================
HRESULT CCamera::Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	m_posV			= posV;										// 現在視点
	m_posVDest		= posV;										// 目標視点
	m_posR			= posR;										// 現在注視点
	m_posRDest		= posR;										// 目標注視点
	m_rot			= D3DXVECTOR3(0.0f, D3DX_PI - atan2f(posR.x - posV.x, posR.z - posV.z), 0.0f);		// 角度
	m_fDistance		= (float)abs((long)(m_posV.z - m_posR.z));	// 距離

	m_bSetDest		= false;									// SetDest使用
	m_nCntSetDest	= 0;										// SetDestカウント
	m_nLimitSetDest = 0;										// SetDest終了
	m_posVDiff		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;			// 視点差分
	m_posRDiff		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;			// 注視点差分
	m_rotDiff		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;			// 角度差分
	m_fDistanceDiff = 0.0f;										// 距離差分

	for (int nCount = 0; nCount < CPOSDEST_MAX; nCount++)
	{// カウント
		m_bSetEnd[nCount] = false;								// 設定終了
	}

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CCamera ***
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// 更新処理											(public)	*** CCamera ***
//=============================================================================
void CCamera::Update(void)
{
	CManager::MODE pMode = CManager::GetMode();		// モード取得
	int nData = CTitleLogo::GetData();				// タイトルロゴ取得

	if (pMode == CManager::MODE_TITLE)
	{// タイトルモードも見
		if (nData == 1)
		{// 等しい場合
			// カメラ位置設定
			m_posV = D3DXVECTOR3(50.0f, 230.0f, -500.0f);
			m_posR = D3DXVECTOR3(50.0f, 170.0f, 0.0f);
			m_posVDest.x = 0.0f;
			m_posRDest.x = 0.0f;
		}
		else
		{// その他
			// カメラを進める
			m_posVDest.x += 2.0f;
			m_posRDest.x += 2.0f;
		}

	}

#ifdef _DEBUG	// デバッグのみ
	// 移動処理
	if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
	{// (I)キーが押された
	 // 移動（前）
		m_posVDest.z += 5.0f;
		m_posRDest.z += 5.0f;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
	{// （K）キーが押された
	 // 移動（後ろ）
		m_posVDest.z -= 5.0f;
		m_posRDest.z -= 5.0f;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
	{// （J）キーが押された
	 // 移動（左）
		m_posVDest.x -= 5.0f;
		m_posRDest.x -= 5.0f;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_L) == true)
	{// （L）キーが押された
	 // 移動（右）
		m_posVDest.x += 5.0f;
		m_posRDest.x += 5.0f;
	}

	// 視点回転
	if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{// （Q）キーが押された
	 // 右回転
		m_rot.y -= 0.02f;
		m_posVDest.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.y) * m_fDistance;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{// （E）キーが押された
	 // 左回転
		m_rot.y += 0.02f;
		m_posVDest.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.y) * m_fDistance;
	}

	// 視点上下回転
	if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
	{// （T）キーが押された
	 // 上
		m_posV.y += 5.0f;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
	{// （G）キーが押された
	 // 下
		m_posV.y -= 5.0f;
	}

	// 注視点上下回転
	if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
	{// （Y）キーが押された
	 // 上
		m_posR.y += 5.0f;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_H) == true)
	{// （H）キーが押された
	 // 下
		m_posR.y -= 5.0f;
	}
#endif // _DEBUG

	// 自動回転
	if (m_bSetDest == true)
	{// セットする
		m_nCntSetDest++;				// カウント

		m_rot.y += (m_rotDiff.y / m_nLimitSetDest);						// 向き調整

		m_fDistance += (m_fDistanceDiff / m_nLimitSetDest);				// 距離調整

		// 位置調整
		m_posRDest.x = m_posV.x - sinf(m_rot.y) * m_fDistance;
		m_posRDest.z = m_posV.z - cosf(m_rot.y) * m_fDistance;
		m_posVDest.x = m_posV.x + (m_posVDiff.x / m_nLimitSetDest);
		m_posVDest.z = m_posV.z + (m_posVDiff.z / m_nLimitSetDest);

		if (m_nCntSetDest == m_nLimitSetDest)
		{// カウントが限界に達したら
			m_bSetDest = false;					// FALSEへ
			m_bSetEnd[m_nNumDest] = true;		// TRUEへ		終了警告
		}
	}

	// 向きの範囲調整
	if (D3DX_PI < m_rot.y)
	{
		m_rot.y -= (D3DX_PI * 2);
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += (D3DX_PI * 2);
	}

	// 値代入
	m_posR.x += (m_posRDest.x - m_posR.x);
	m_posR.z += (m_posRDest.z - m_posR.z);
	m_posV.x += (m_posVDest.x - m_posV.x);
	m_posV.z += (m_posVDest.z - m_posV.z);

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_DATA);		// フォント取得

	// 文字設定
	CFont::SetMessageData(CScene::OBJTYPE_CAMERA, CFont::SETTYPE_ADD, "Camera : 視点　[%.2f, %.2f, %.2f]\n", m_posV.x, m_posV.y, m_posV.z);
	CFont::SetMessageData(CScene::OBJTYPE_CAMERA, CFont::SETTYPE_ADD, "Camera : 注視点[%.2f, %.2f, %.2f]\n", m_posR.x, m_posR.y, m_posR.z);
	CFont::SetMessageData(CScene::OBJTYPE_CAMERA, CFont::SETTYPE_ADD, "\n");
}

//=============================================================================
// 描画処理											(public)	*** CCamera ***
//=============================================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		5000.0f
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH
	(
		&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// 自動設定処理										(public)	*** CCamera ***
//=============================================================================
void CCamera::SetPosDest(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest, float fDistance, int nLimit, int nNumDest)
{
	if (m_bSetDest != true)
	{// セットがTRUE以外の場合
		m_bSetDest = true;					// TRUEへ

		m_nCntSetDest = 0;					// カウント初期化
		m_nLimitSetDest = nLimit;			// 限界カウントを設定
		m_nNumDest = nNumDest;				// セット番号

		// 差分計算
		m_posVDiff = posVDest - m_posV;		// 位置
		m_posRDiff = posRDest - m_posR;
		//m_rotDiff.y = atan2f(posRDest.z - posVDest.z, posRDest.x - posVDest.x);		// 向き
		m_rotDiff.y = atan2f(posRDest.x - posVDest.x, posRDest.z - posVDest.z);

		m_rotDiff.y = D3DX_PI - m_rotDiff.y - m_rot.y;

		// 向きの範囲調整 
		if (D3DX_PI < m_rotDiff.y)
		{
			m_rotDiff.y -= (D3DX_PI * 2);
		}
		if (m_rotDiff.y < -D3DX_PI)
		{
			m_rotDiff.y += (D3DX_PI * 2);
		}

		m_fDistanceDiff = (float)(abs((long)fDistance) - abs((long)m_fDistance));		// 距離
	}
}