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
#include "tutorial.h"		// チュートリアル
#include "game.h"			// ゲーム
#include "meshfield.h"		// メッシュフィールド

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
	CManager::MODE mode = CManager::GetMode();		// モード取得
	CTitle::TITLE title = CTitle::GetTitle();		// タイトル取得
	bool bPause = false;							// ポーズ
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// 入力デバイス

	if (mode == CManager::MODE_TUTORIAL)
	{
		bPause = CTutorial::GetPause();				// ポーズ取得
	}
	else if (mode == CManager::MODE_GAME)
	{
		bPause = CGame::GetPause();					// ポーズ取得
	}

	static bool bMove = true;						// 動く

#ifdef _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_O) == true)
	{// Oキーを押したら
		bMove = bMove ? false : true;				// 動くor動かない
	}
#endif

	for (int nCount = 0; nCount < CPOSDEST_MAX; nCount++)
	{// カウント
		if (m_bSetEnd[nCount] == true)
		{// TRUEの場合
			m_bSetEnd[nCount] = false;				// FALSEへ
		}
	}

	switch (mode)
	{// モード
	case CManager::MODE_TITLE:		// タイトル
		break;
	case CManager::MODE_TUTORIAL:	// チュートリアル		
	case CManager::MODE_GAME:		// ゲーム
		if (bPause == false)
		{// ポーズをしていない
#ifdef _DEBUG
			// 視点上下回転
			if (pInputKeyboard->GetPress(DIK_T) == true)
			{
				m_posV.y += 5.0f;
			}
			if (pInputKeyboard->GetPress(DIK_G) == true)
			{
				m_posV.y -= 5.0f;
			}

			// 注視点上下回転
			if (pInputKeyboard->GetPress(DIK_Y) == true)
			{
				m_posR.y += 5.0f;
			}
			if (pInputKeyboard->GetPress(DIK_H) == true)
			{
				m_posR.y -= 5.0f;
			}
#endif
			if (bMove == true)
			{
				// 自動移動
				m_posRDest.z += 3.0f;
				m_posVDest.z += 3.0f;
			}
		}

		break;
	case CManager::MODE_RESULT:			// リザルト
		break;
	}

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
		1500.0f
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