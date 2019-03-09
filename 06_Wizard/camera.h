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
#include "scene2d.h"	// シーン2D
#include "scene3d.h"	// シーン3D
#include "enemy.h"		// 敵

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
	CCamera();							// コンストラクタ
	virtual ~CCamera();					// デストラクタ

	virtual HRESULT Init(void);			// 初期化処理
	virtual void Uninit(void);			// 終了処理
	virtual void Update(void) = 0;		// 更新処理
	virtual void SetCamera(void);		// 描画処理

	void SetInitPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR)			// 初期設定（視点、注視点）
		{ m_posVInit = posV; m_posRInit = posR; }
	D3DXVECTOR3 GetInitPos(int nNum)							// 初期取得（視点、注視点）
		{ if(nNum == 0) return m_posVInit; else return m_posRInit; }

	void SetPosDest(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest)	// 目標設定（視点、注視点）
		{ m_posVDest = posVDest; m_posRDest = posRDest;}
	D3DXVECTOR3 GetPosDest(int nNum)							// 目標取得（視点、注視点）
		{ if (nNum == 0) return m_posVDest; else return m_posRDest; }	

										// 設定（視点、注視点）
	void SetPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNum = 0)
	{
		m_posV		= posV;								// 視点
		m_posVDest	= posV;								// 目標視点
		m_posR		= posR;								// 注視点
		m_posRDest	= posR;								// 目標視点
		m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 角度
		if (nNum == 0)
		{
			m_rot.x = D3DX_PI - atan2f(posR.z - posV.z, posR.y - posV.y);		// 角度
			m_rot.y = D3DX_PI - atan2f(posR.x - posV.x, posR.z - posV.z);		// 角度
		}
		// 角度調整
		if (D3DX_PI < m_rot.y)
			m_rot.y -= (D3DX_PI * 2);
		if (m_rot.y < -D3DX_PI)
			m_rot.y += (D3DX_PI * 2);

		m_fDistance = sqrtf(powf(posR.x - posV.x, 2.0f) + powf(posR.z - posV.z, 2.0f));		// 距離
		m_fDistanceInit = m_fDistance;
	}
	void SetPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot)
	{
		m_posV		= posV;								// 視点
		m_posVDest	= posV;								// 目標視点
		m_posR		= posR;								// 注視点
		m_posRDest	= posR;								// 目標視点
		m_rot		= rot;								// 角度

		// 角度調整
		if (D3DX_PI < m_rot.y)
			m_rot.y -= (D3DX_PI * 2);
		if (m_rot.y < -D3DX_PI)
			m_rot.y += (D3DX_PI * 2);

		m_fDistance = sqrtf(powf(posR.x - posV.x, 2.0f) + powf(posR.z - posV.z, 2.0f));		// 距離
		m_fDistanceInit = m_fDistance;
	}
	void SetPos(D3DXVECTOR3 posR)
	{
		m_posR = posR;									// 注視点
		m_posRDest = posR;								// 目標視点
		
		// 視点計算用
		D3DXMATRIX mat;									// 回転
		D3DXVECTOR3	v(0, 0, m_fDistance);				// 注視点との距離
		D3DXVECTOR3	vout;								// 計算結果

		// 視点計算
		D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);		// 回転
		D3DXVec3TransformCoord(&vout, &v, &mat);								// 距離と合わせる
		m_posVDest = vout + m_posRDest;					// 注視点からの位置

		m_posV = m_posVDest;							// 視点
	}
	D3DXVECTOR3 Getpos(int nNum)		// 取得（視点、注視点）
		{ if(nNum == 0) return m_posV; else return m_posR; }		

	void Setrot(D3DXVECTOR3 rot)		// 角度（設定）
		{ m_rot = rot;	}
	D3DXVECTOR3 Getrot(void)			// 角度（取得）
		{ return m_rot;	}

	void SetDistance(int nNum, float fDistance)			// 距離設定（現在、初期）
		{ if (nNum == 0) m_fDistance = fDistance; else m_fDistanceInit = fDistance; }
	float GetDistance(int nNum)							// 距離取得（現在、初期）
		{ if (nNum == 0) return m_fDistance; else return m_fDistanceInit; }

	D3DXMATRIX GetmtxPro(void)			// プロジェクションマトリックス取得
		{ return m_mtxProjection; }
	D3DXMATRIX GetmtxView(void)			// ビューマトリックス取得
		{ return m_mtxView; }

										// ビューポート設定
	void SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height);

private:

protected:
	void Debug(void);					// デバッグ処理

	D3DXVECTOR3		m_posVInit;			// 初期視点
	D3DXVECTOR3		m_posV;				// 視点
	D3DXVECTOR3		m_posVDest;			// 目標視点
	D3DXVECTOR3		m_posRInit;			// 初期注視点
	D3DXVECTOR3		m_posR;				// 注視点
	D3DXVECTOR3		m_posRDest;			// 目標注視点
	D3DXVECTOR3		m_vecU;				// 上方向ベクトル
	D3DXVECTOR3		m_rot;				// 角度
	float			m_fDistance;		// 距離
	float			m_fDistanceInit;	// 初期距離

	D3DXMATRIX		m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX		m_mtxView;			// ビューマトリックス
	D3DVIEWPORT9	m_viewport;			// ビューポート

};

class CCameraMain : public CCamera
{// メインカメラ（親：CCamera）
public:
	typedef enum
	{// ロックオンマーカー
		ROCKON_MARKER_FRAME = 0,	// フレーム
		ROCKON_MARKER_PLAY,			// ロックオン中
		ROCKON_MARKER_MAX			// 最大数
	}ROCKON_MARKER;

	CCameraMain();						// コンストラクタ
	~CCameraMain();						// デストラクタ

	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void SetCamera(void);				// 描画処理

	void SetPickUp(bool bPickUp)		// ピックアップ処理
		{ m_bPickUp = bPickUp; }
	bool GetPickUp(void)				// ピックアップ取得
		{ return m_bPickUp; }

	void SetGameEnd(bool bGameEnd)		// ゲームエンド処理
		{ m_bGameEnd = bGameEnd; }
	bool GetGameEnd(void)				// ゲームエンド取得
		{ return m_bGameEnd; }

	void SetMove(bool bMove)			// 移動処理
		{ m_bMove = bMove; }
	bool GetMove(void)					// 移動取得
		{ return m_bMove; }

	void SetMoveCollision(bool bMoveCollision)	// 移動処理
		{ m_bMoveCollision = bMoveCollision; }
	bool GetMoveCollision(void)					// 移動取得
		{ return m_bMoveCollision; }

	void SetLockOn(bool bLockOn)		// ロックオン設定
		{ m_bLockOn = bLockOn; }	
	bool GetLockOn(void)				// ロックオンしているか取得
		{ return m_bLockOn; }
	float GetLockOnRot(void)			// ロックオン角度取得
		{ return m_fLockOn; }
	CEnemy *GetLockOnEnemy(void)		// ロックオンしてる敵取得
		{ return m_pLockOn; }

private:
	void PickUp(void);					// ピックアップ
	void GameEnd(void);					// ゲームエンド
	void Following(void);				// 追従処理
	void Move(void);					// 移動処理
	void LockOn(void);					// ロックオン処理
	void ColisionChack(void);			// 判定チェック処理

	bool m_bPickUp;						// ピックアップ
	int m_nCntPickUp;					// ピックアップカウント
	bool m_bGameEnd;					// ゲームエンド
	int m_nCntGameEnd;					// ゲームエンドカウント
	bool m_bMove;						// 移動できるか
	bool m_bMoveCollision;				// 移動コリジョン
	CEnemy *m_pLockOn;					// ロックオン敵
	bool m_bLockOn;						// ロックオン
	float m_fLockOn;					// ロックオン角度

	CEnemy *m_pLockOnL;					// ロックオン敵L
	CEnemy *m_pLockOnR;					// ロックオン敵R

	CScene3D *m_pLockOnMarker[ROCKON_MARKER_MAX];	// ロックオンマーカー

protected:

};

class CCameraMap : public CCamera
{// ミニマップ（親：CCamera）
public:
	CCameraMap();						// コンストラクタ
	~CCameraMap();						// デストラクタ

	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void SetCamera(void);				// 描画処理

private:

protected:

};

#endif