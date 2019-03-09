//=============================================================================
//
// カメラ処理 [camera.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCamera
{// カメラ
public:
	typedef enum
	{// カメラ自動設定
		CPOSDEST_TITLE_LOGO = 0,		// タイトルロゴ
		CPOSDEST_MENU_C,				// メニューカスタム
		CPOSDEST_MENU_T,				// メニュータイトル
		CPOSDEST_CUSTOM,				// カスタム
		CPOSDEST_MAX
	}CPOSDEST;

	CCamera();												// コンストラクタ
	~CCamera();												// デストラクタ

	HRESULT Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR);		// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void SetCamera(void);									// 描画処理

	void SetPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
	{// 位置設定
		if (m_bSetDest != true)
		{// 自動設定
			// 位置設定
			m_posV		= posV;		// 視点			
			m_posVDest	= posV;		// 目標視点
			m_posR		= posR;		// 注視点
			m_posRDest	= posR;		// 目標注視点
			m_rot.y = D3DX_PI - atan2f(posR.x - posV.x, posR.z - posV.z);		// 角度

			// 角度調整
			if (D3DX_PI < m_rot.y)
			{
				m_rot.y -= (D3DX_PI * 2);
			}
			if (m_rot.y < -D3DX_PI)
			{
				m_rot.y += (D3DX_PI * 2);
			}

			m_fDistance = (float)abs((long)(m_posV.z - m_posR.z));				// 距離
		}
	}

	bool GetDest(void) { return m_bSetDest; }				// 自動設定中

	void SetPosDest(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest, float fDistance, int nLimit, int nNumDest);													// 自動設定
	bool GetPosDest(int nNumDest) { bool bSetEnd = m_bSetEnd[nNumDest]; if (m_bSetEnd[nNumDest] == true) m_bSetEnd[nNumDest] = false; return bSetEnd; }		// 自動設定完了

	D3DXVECTOR3 GetposV(void) { return m_posV; }			// 視点取得
	D3DXVECTOR3 GetposR(void) { return m_posR; }			// 注視点取得

	void SetPosVDest(D3DXVECTOR3 posVDest) { m_posVDest = posVDest; }		// 視点設定
	D3DXVECTOR3 GetPosVDest(void) { return m_posVDest; }					// 視点取得
	void SetPosRDest(D3DXVECTOR3 posRDest) { m_posRDest = posRDest; }		// 注視点設定
	D3DXVECTOR3 GetPosRDest(void) { return m_posRDest; }					// 注視点取得

private:
	D3DXVECTOR3 m_posV;							// 視点
	D3DXVECTOR3 m_posVDest;						// 目標視点
	D3DXVECTOR3 m_posR;							// 注視点
	D3DXVECTOR3 m_posRDest;						// 目標注視点
	D3DXVECTOR3 m_vecU;							// 上方向ベクトル
	D3DXVECTOR3	m_rot;							// 角度
	float		m_fDistance;					// 距離

	bool		m_bSetDest;						// SetDest使用
	int			m_nCntSetDest;					// SetDestカウント
	int			m_nLimitSetDest;				// SetDest終了
	int			m_nNumDest;						// 種類
	D3DXVECTOR3 m_posVDiff;						// 視点差分
	D3DXVECTOR3 m_posRDiff;						// 注視点差分
	D3DXVECTOR3 m_rotDiff;						// 角度差分
	float		m_fDistanceDiff;				// 距離差分

	bool		m_bSetEnd[CPOSDEST_MAX];		// 設定終了

	D3DXMATRIX	m_mtxProjection;				// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;						// ビューマトリックス

protected:

};

#endif