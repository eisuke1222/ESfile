//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "camera.h"			// カメラ
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "sound.h"			// サウンド
#include "fade.h"			// フェード
#include "texture.h"		// テクスチャ
#include "title.h"			// タイトル
#include "game.h"			// ゲーム
#include "scenemesh.h"		// シーンメッシュ
#include "map.h"			// マップ
#include "ui.h"				// Ui
#include "player.h"			// プレイヤー
#include "enemy.h"			// 敵

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MC_DRAW_MIN		(10.0f)								// 描画最短
#define MC_DRAW_MAX		(10000.0f)							// 描画最長
#define MC_MOVE_POS		(5.0f)								// 移動（位置）
#define MC_MOVE_ROT		(0.02f)								// 移動（回転）
#define MC_SET_RATIO	(0.25f)								// 設定割合
#define MC_XPAD_STICK	(0.0000014f)						// XPadスティック
#define MC_MOUSE_X		(0.0005625f)						// マウスX
#define MC_MOUSE_Y		(0.001f)							// マウスY
#define MC_ROT_X_MIN	(-1.0f)								// 回転X最小
#define MC_ROT_X_MAX	(1.0f)								// 回転X最大
#define MC_ROCKON_RANGE	(600.0f)							// ロックオン範囲
#define MC_ROCKON_FREE	(D3DX_PI * 0.1f)					// ロックオン時のあそび

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
// コンストラクタ										(public)	*** CCamera ***
//=============================================================================
CCamera::CCamera()
{
	// 初期値設定
	m_posVInit			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期視点
	m_posV				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在視点
	m_posVDest			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標視点
	m_posRInit			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期注視点
	m_posR				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在注視点
	m_posRDest			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標注視点
	m_vecU				= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトル
	m_rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 角度
	m_fDistance			= 0.0f;								// 距離

	m_viewport.X		= 0;								// X開始位置
	m_viewport.Y		= 0;								// Y開始位置
	m_viewport.Width	= SCREEN_WIDTH;						// X開始位置からの大きさ
	m_viewport.Height	= SCREEN_HEIGHT;					// Y開始位置からの大きさ
	m_viewport.MinZ		= 0.0f;								// 最小
	m_viewport.MaxZ		= 1.0f;								// 最大
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
HRESULT CCamera::Init(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CCamera ***
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// カメラ設定処理										(public)	*** CCamera ***
//=============================================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// ビューポート設定
	pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f), (float)m_viewport.Width / (float)m_viewport.Height, MC_DRAW_MIN, MC_DRAW_MAX);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// ビューポート処理									(public)	*** CCamera ***
//=============================================================================
void CCamera::SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	m_viewport.X		= X;		// 位置X
	m_viewport.Y		= Y;		// 位置Y
	m_viewport.Width	= Width;	// サイズX
	m_viewport.Height	= Height;	// サイズY
	m_viewport.MinZ		= 0.0f;		// 最小
	m_viewport.MaxZ		= 1.0f;		// 最大
}

//=============================================================================
// デバッグ処理										(protected)	*** CCamera ***
//=============================================================================
void CCamera::Debug(void)
{
	// 移動処理
	if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
	{// (I)キーが押された
	 // 移動（前）
		m_posVDest.z += MC_MOVE_POS;
		m_posRDest.z += MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
	{// （K）キーが押された
	 // 移動（後ろ）
		m_posVDest.z -= MC_MOVE_POS;
		m_posRDest.z -= MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
	{// （J）キーが押された
	 // 移動（左）
		m_posVDest.x -= MC_MOVE_POS;
		m_posRDest.x -= MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_L) == true)
	{// （L）キーが押された
	 // 移動（右）
		m_posVDest.x += MC_MOVE_POS;
		m_posRDest.x += MC_MOVE_POS;
	}

	// 視点回転
	if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{// （Q）キーが押された
	 // 右回転
		m_rot.y -= MC_MOVE_ROT;
		m_posVDest.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.y) * m_fDistance;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{// （E）キーが押された
	 // 左回転
		m_rot.y += MC_MOVE_ROT;
		m_posVDest.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.y) * m_fDistance;
	}

	// 視点上下回転
	if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
	{// （T）キーが押された
	 // 上
		m_posVDest.y += MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
	{// （G）キーが押された
	 // 下
		m_posVDest.y -= MC_MOVE_POS;
	}

	// 注視点上下回転
	if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
	{// （Y）キーが押された
	 // 上
		m_posRDest.y += MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_H) == true)
	{// （H）キーが押された
	 // 下
		m_posRDest.y -= MC_MOVE_POS;
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CCameraMain ***
//=============================================================================
CCameraMain::CCameraMain() : CCamera()
{
	// 初期値設定
	m_bPickUp			= false;	// ピックアップ
	m_nCntPickUp		= 0;		// ピックアップカウント
	m_bGameEnd			= false;	// ゲームエンド
	m_nCntGameEnd		= 0;		// ゲームエンドカウント
	m_bMove				= false;	// 移動できる
	m_bMoveCollision	= true;		// 移動できる
	m_pLockOn			= NULL;		// ロックオン敵
	m_bLockOn			= false;	// ロックオン

	m_pLockOnL			= NULL;		// ロックオン敵L
	m_pLockOnR			= NULL;		// ロックオン敵R

	// ロックオンマーカー
	for (int nCount = 0; nCount < ROCKON_MARKER_MAX; nCount++)
		m_pLockOnMarker[nCount] = NULL;
}

//=============================================================================
// デストラクタ									(public)	*** CCameraMain ***
//=============================================================================
CCameraMain::~CCameraMain()
{

}

//=============================================================================
// 初期化処理									(public)	*** CCameraMain ***
//=============================================================================
HRESULT CCameraMain::Init(void)
{
	CCamera::Init();		// 初期化処理

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::Uninit(void)
{
	m_bPickUp		= false;			// ピックアップ
	m_nCntPickUp	= 0;				// ピックアップカウント
	m_bGameEnd		= false;			// ゲームエンド
	m_nCntGameEnd	= 0;				// ゲームエンドカウント

	m_pLockOn		= NULL;				// ロックオン敵

	m_pLockOnL		= NULL;				// ロックオン敵L
	m_pLockOnR		= NULL;				// ロックオン敵R

	// ロックオンマーカー
	for (int nCount = 0; nCount < ROCKON_MARKER_MAX; nCount++)
	{// ロックオンマーカーカウント
		if (m_pLockOnMarker[nCount] != NULL)
		{// NULL以外の場合
			m_pLockOnMarker[nCount] = NULL;
		}
	}

	CCamera::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::Update(void)
{
#ifdef _DEBUG	// デバッグのみ
	//Debug();	// デバッグ処理
#endif // _DEBUG

	if (m_bMove == true)
	{// 移動できる
		if (m_bGameEnd == false)
		{// ゲームエンド以外
			if (m_bPickUp == true)
			{// ピックアップだったら
				PickUp();			// ピックアップ
			}
			else
			{// ピックアップ以外
				Following();		// 追従
				LockOn();			// ロックオン

				if (m_bMoveCollision == true)
				{// 移動コリジョンをする
					Move();					// 移動
					ColisionChack();		// 判定チェック
				}
			}
		}
		else
		{// ゲームうエンドだったら
			GameEnd();		// ゲームエンド
		}
	}

	// 角度調整
	if (D3DX_PI < m_rot.x)
		m_rot.x -= (D3DX_PI * 2);
	if (m_rot.x < -D3DX_PI)
		m_rot.x += (D3DX_PI * 2);
	if (D3DX_PI < m_rot.y)
		m_rot.y -= (D3DX_PI * 2);
	if (m_rot.y < -D3DX_PI)
		m_rot.y += (D3DX_PI * 2);
	if (D3DX_PI < m_rot.z)
		m_rot.z -= (D3DX_PI * 2);
	if (m_rot.z < -D3DX_PI)
		m_rot.z += (D3DX_PI * 2);

	// 値代入
	m_posR.x += (m_posRDest.x - m_posR.x) * MC_SET_RATIO;
	m_posR.y += (m_posRDest.y - m_posR.y) * MC_SET_RATIO;
	m_posR.z += (m_posRDest.z - m_posR.z) * MC_SET_RATIO;
	m_posV.x += (m_posVDest.x - m_posV.x) * MC_SET_RATIO;
	m_posV.y += (m_posVDest.y - m_posV.y) * MC_SET_RATIO;
	m_posV.z += (m_posVDest.z - m_posV.z) * MC_SET_RATIO;

	// フォント取得
	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_DATA);

	// 値設定
	CFont::SetMessageData(CScene::OBJTYPE_CAMERA, CFont::SETTYPE_ADD, "Camera : 視点　[%.2f, %.2f, %.2f]\n", m_posV.x, m_posV.y, m_posV.z);
	CFont::SetMessageData(CScene::OBJTYPE_CAMERA, CFont::SETTYPE_ADD, "Camera : 注視点[%.2f, %.2f, %.2f]\n", m_posR.x, m_posR.y, m_posR.z);
	CFont::SetMessageData(CScene::OBJTYPE_CAMERA, CFont::SETTYPE_ADD, "\n");
}

//=============================================================================
// カメラ設定処理									(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::SetCamera(void)
{
	CCamera::SetCamera();		// カメラ設定
}

//=============================================================================
// ピックアップ処理								(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::PickUp(void)
{
	CSound *pSound = CManager::GetSound();		// サウンド取得

	// ロックオンマーカー
	for (int nCount = 0; nCount < ROCKON_MARKER_MAX; nCount++)
	{// ロックオンマーカーカウント
		if (m_pLockOnMarker[nCount] != NULL)
		{// NULL以外の場合
			m_pLockOnMarker[nCount]->Uninit();		// 終了処理
			m_pLockOnMarker[nCount] = NULL;			// NULLへ
		}
	}

	CScene	*pScene		= NULL;		// シーン
	CScene	*pSceneNext = NULL;		// 次シーン
	CEnemy	*pEnemy		= NULL;		// 敵

	pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

	while (pScene != NULL)
	{// NULL以外の場合
		pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

		if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
		{// オブジェクトタイプが敵の場合
			pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト

			break;
		}
		pScene = pSceneNext;	// 次シーンへ
	}

	if (pEnemy != NULL)
	{// NULL以外の場合
		D3DXVECTOR3 pos = pEnemy->Getpos();		// 敵の位置取得

		// 距離と角度取得
		float fLenght	= sqrtf(powf(pos.x - m_posR.x, 2.0f) + powf(pos.z - m_posR.z, 2.0f));
		float fAngle	= atan2f(pos.x - m_posR.x, pos.z - m_posR.z);

		if (m_nCntPickUp < 90)
		{// 一定フレームまで
			// 直進
			m_posRDest.x += sinf(fAngle) * (fLenght / (90 - m_nCntPickUp));
			m_posRDest.z += cosf(fAngle) * (fLenght / (90 - m_nCntPickUp));

			m_rot.x += ((0.06f - m_rot.x) / (90 - m_nCntPickUp));
		}
		else if (m_nCntPickUp == 90)
		{// 一定フレームになったら
			// 角度切替
			m_rot.y = D3DX_PI + (D3DX_PI * 2.0f * 0.3f);

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_DON);
			}
		}
		else if (m_nCntPickUp == 120)
		{// 一定フレームになったら
			// 角度切替
			m_rot.y = -D3DX_PI + -(D3DX_PI * 2.0f * 0.3f);

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_DON);
			}
		}
		else if (m_nCntPickUp == 150)
		{// 一定フレームになったら
			// 角度切替
			m_rot.y = D3DX_PI;

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_DON);
			}
		}
		else if (m_nCntPickUp == 180)
		{// 一定フレームになったら
			// 解除
			m_bPickUp = false;
			m_nCntPickUp = 0;
			return;
		}

		// 視点計算用
		D3DXMATRIX mat;						// 回転
		D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
		D3DXVECTOR3	vout;					// 計算結果

		// 視点計算
		D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
		D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
		m_posVDest = vout + m_posRDest;		// 注視点からの位置

		m_nCntPickUp++;
	}
}

//=============================================================================
// ゲームエンド処理								(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::GameEnd(void)
{
	m_pLockOn	= NULL;					// ロックオン敵

	m_pLockOnL	= NULL;					// ロックオン敵L
	m_pLockOnR	= NULL;					// ロックオン敵R

	// ロックオンマーカー
	for (int nCount = 0; nCount < ROCKON_MARKER_MAX; nCount++)
	{// ロックオンマーカーカウント
		if (m_pLockOnMarker[nCount] != NULL)
		{// NULL以外の場合
			m_pLockOnMarker[nCount]->Uninit();		// 終了処理
			m_pLockOnMarker[nCount] = NULL;			// NULLへ
		}
	}

	CGame *pGame = CManager::GetGame();		// ゲーム取得
	if (pGame != NULL)
	{// NULL以外の場合
		CPlayer	*pPlayer = pGame->GetPlayer();		// プレイヤー取得
		if (pPlayer != NULL)
		{// NULL以外の場合
			if (m_nCntGameEnd < 165)
			{// 一定フレームまで
				// 正面
				m_posRDest = pPlayer->Getpos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f);
				m_rot.x = -0.5f;
				m_rot.y = pPlayer->Getrot().y;
				m_fDistance = 100.0f;

				// 視点計算用
				D3DXMATRIX mat;						// 回転
				D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
				D3DXVECTOR3	vout;					// 計算結果

				// 視点計算
				D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
				D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
				m_posVDest = vout + m_posRDest;		// 注視点からの位置
				m_posV = m_posVDest;
				m_posR = m_posRDest;
			}
			else
			{// その他
				// 上から見下ろす＋回転
				if (m_nCntGameEnd == 165)
					pGame->SetSS_Death(true);		// スクショ

				// 注視点更新
				m_posRDest = pPlayer->Getpos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f);

				// 上昇
				if (-1.2f < m_rot.x)
					m_rot.x -= 0.004f;

				// 回転
				m_rot.y += 0.004f;

				// 距離
				if (m_fDistance < 250.0f)
					m_fDistance += 0.5f;

				// 視点計算用
				D3DXMATRIX mat;						// 回転
				D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
				D3DXVECTOR3	vout;					// 計算結果

				// 視点計算
				D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
				D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
				m_posVDest = vout + m_posRDest;		// 注視点からの位置
			}

			m_nCntGameEnd++;
		}
	}
}

//=============================================================================
// 追従処理										(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::Following(void)
{
	CManager::MODE	mode = CManager::GetMode();		// モード取得
	CPlayer	*pPlayer = NULL;
	CInputKeyboard	*pKey = CManager::GetInputKeyboard();	// キーボード取得
	CInputMouse		*pMouse = CManager::GetInputMouse();	// マウス取得
	CInputXPad		*pXPad = CManager::GetInputXPad();		// Xパッド取得

	if (mode == CManager::MODE_TITLE)
		pPlayer = CManager::GetTitle()->GetPlayer();// プレイヤー取得
	else if (mode == CManager::MODE_GAME)
		pPlayer = CManager::GetGame()->GetPlayer();	// プレイヤー取得

	// 追尾設定
	if (pPlayer != NULL)
	{// NULL以外の場合
		if (m_bLockOn == true)
		{// ロックオンしている
			if (m_pLockOn != NULL)
			{// NULL以外の場合
				D3DXVECTOR3 posEnemy = m_pLockOn->Getpos();		// 位置取得（ロックオン）
				float		fLenght = 0.0f;						// 距離計算用

				// 距離計算
				fLenght = sqrtf(powf(posEnemy.x - pPlayer->Getpos().x, 2) + powf(posEnemy.z - pPlayer->Getpos().z, 2));

				if (fLenght < MC_ROCKON_RANGE)
				{// ロックオン範囲
					//if (10.0f < fLenght)
					{
						// 回転設定
						m_rot.x = -0.3f;// D3DX_PI + atan2f(fLenght * 0.25f, posEnemy.y - m_posVDest.y + (pPlayer->Getpos().y * 0.6f));

						D3DXVECTOR3 posPlayer = pPlayer->Getpos();		// 位置取得
						m_fLockOn = (D3DX_PI + atan2f(posEnemy.x - posPlayer.x, posEnemy.z - posPlayer.z));		// 回転計算
						float fDiff = m_rot.y - m_fLockOn;				// 差分計算

						// 回転調整
						if (fDiff < -D3DX_PI)
							fDiff += D3DX_PI * 2.0f;
						if (D3DX_PI < fDiff)
							fDiff -= D3DX_PI * 2.0f;

						// カメラあそび
						if (fDiff < -MC_ROCKON_FREE)
							m_rot.y -= fDiff + MC_ROCKON_FREE;
						if (MC_ROCKON_FREE < fDiff)
							m_rot.y -= fDiff - MC_ROCKON_FREE;
					}
				}
				else
				{// ロックオン範囲外
					m_pLockOn = NULL;		// NULLへ
					m_bLockOn = false;		// ロックオンしていない

					m_pLockOnL = NULL;
					m_pLockOnR = NULL;
				}
			}
			else
			{// ロックオン範囲外
				m_pLockOn = NULL;		// NULLへ
				m_bLockOn = false;		// ロックオンしていない

				m_pLockOnL = NULL;
				m_pLockOnR = NULL;
			}
		}

		// Xパッド
		{
			if (pXPad != NULL)
			{// NULL以外の場合
				if (pXPad->GetTrigger(CInputXPad::XPADOTHER_TRIGGER_LEFT, 0) == true)
				{
					m_rot.y = pPlayer->Getrot().y + D3DX_PI;

					m_pLockOn = NULL;		// NULLへ
					m_bLockOn = false;		// ロックオンしていない

					m_pLockOnL = NULL;
					m_pLockOnR = NULL;
				}
			}
		}

		// キーボード
		{
			if (pKey != NULL)
			{// NULL以外の場合
				if (pKey->GetTrigger(DIK_C) == true)
				{
					m_rot.y = pPlayer->Getrot().y + D3DX_PI;

					m_pLockOn = NULL;		// NULLへ
					m_bLockOn = false;		// ロックオンしていない

					m_pLockOnL = NULL;
					m_pLockOnR = NULL;
				}
			}
		}

		// 注視点更新
		m_posRDest = D3DXVECTOR3(pPlayer->Getpos().x, 50.0f, pPlayer->Getpos().z);

		// 視点計算用
		D3DXMATRIX mat;						// 回転
		D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
		D3DXVECTOR3	vout;					// 計算結果

		// 視点計算
		D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
		D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
		m_posVDest = vout + m_posRDest;		// 注視点からの位置
	}
}

//=============================================================================
// 移動処理										(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::Move(void)
{
	CInputKeyboard	*pKey	= CManager::GetInputKeyboard();	// キーボード取得
	CInputMouse		*pMouse = CManager::GetInputMouse();	// マウス取得
	CInputXPad		*pXPad	= CManager::GetInputXPad();		// Xパッド取得
	bool			bInput	= false;;						// 入力したか

	// Xパッド
	{
		if (bInput == false)
		{// 入力していない
			if (pXPad != NULL)
			{// NULL以外の場合
				if (pXPad->GetStick(1, 0) == true)
				{// Xパッド（右スティック）を入力した
					if (m_bLockOn == false)
					{// ロックオンしていない						
						// X回転
						if (MC_ROT_X_MIN < m_rot.x + pXPad->GetStickMove(1, 0).y * MC_XPAD_STICK &&
							m_rot.x + pXPad->GetStickMove(1, 0).y * MC_XPAD_STICK < MC_ROT_X_MAX)
						{// 回転制限
							m_rot.x += pXPad->GetStickMove(1, 0).y * MC_XPAD_STICK;
						}

						// Y回転
						m_rot.y += pXPad->GetStickMove(1, 0).x * MC_XPAD_STICK;

						bInput = true;		// 入力した
					}
					else
					{// ロックオンしている
						if (pXPad->GetStickNum(1, 0).y < CInputXPad::m_STICKMIN * 0.6f || CInputXPad::m_STICKMAX * 0.6f < pXPad->GetStickNum(1, 0).y)
						{// 右スティックY一定以上でロックオン解除
							m_pLockOn = NULL;		// NULLへ
							m_bLockOn = false;		// ロックオンしていない

							m_pLockOnL = NULL;
							m_pLockOnR = NULL;
						}

						if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_R_LEFT, 0) == true)
						{// 右スティックY一定以上でロックオン解除
							if (m_pLockOnL != NULL)
							{
								m_pLockOn = m_pLockOnL;
								m_pLockOnL = NULL;
							}
						}
						else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_R_RIGHT, 0) == true)
						{// 右スティックY一定以上でロックオン解除
							if (m_pLockOnR != NULL)
							{
								m_pLockOn = m_pLockOnR;
								m_pLockOnR = NULL;
							}
						}
					}
				}
			}
		}
	}

	// マウス
	{
		if (bInput == false)
		{// 入力していない
			if (pMouse != NULL)
			{// NULL以外の場合
				if (m_bLockOn == false)
				{// ロックオンしていない
					// X回転
					if (MC_ROT_X_MIN < m_rot.x - pMouse->GetPointer().y * MC_MOUSE_X &&
						m_rot.x - pMouse->GetPointer().y * MC_MOUSE_X < MC_ROT_X_MAX)
					{// 回転制限
						m_rot.x -= pMouse->GetPointer().y * MC_MOUSE_X;
					}

					// Y回転
					m_rot.y += pMouse->GetPointer().x * MC_MOUSE_Y;

					bInput = true;		// 入力した
				}
				else
				{// ロックオンしている
					if (pMouse->GetPointer().y < -200.0f || 200.0f < pMouse->GetPointer().y)
					{// 右スティックY一定以上でロックオン解除
						m_pLockOn = NULL;		// NULLへ
						m_bLockOn = false;		// ロックオンしていない

						m_pLockOnL = NULL;
						m_pLockOnR = NULL;
					}

					if (pMouse->GetPointer().x < -200.0f)
					{// 右スティックY一定以上でロックオン解除
						if (m_pLockOnL != NULL)
						{
							m_pLockOn = m_pLockOnL;
							m_pLockOnL = NULL;
						}
					}
					else if (200.0f < pMouse->GetPointer().x)
					{// 右スティックY一定以上でロックオン解除
						if (m_pLockOnR != NULL)
						{
							m_pLockOn = m_pLockOnR;
							m_pLockOnR = NULL;
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// ロックオン処理									(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::LockOn(void)
{
	CInputKeyboard	*pKey = CManager::GetInputKeyboard();	// キーボード取得
	CInputMouse		*pMouse = CManager::GetInputMouse();	// マウス取得
	CInputXPad		*pXPad = CManager::GetInputXPad();		// Xパッド取得
	bool			bInput = false;;						// 入力したか
	CFade::FADE		fade = CFade::GetFade();					// フェード取得

	CManager::MODE	mode = CManager::GetMode();		// モード取得
	CPlayer	*pPlayer = NULL;

	if (mode == CManager::MODE_TITLE)
		pPlayer = CManager::GetTitle()->GetPlayer();// プレイヤー取得
	else if (mode == CManager::MODE_GAME)
		pPlayer = CManager::GetGame()->GetPlayer();	// プレイヤー取得

	CScene	*pScene			= NULL;		// シーン
	CScene	*pSceneNext		= NULL;		// 次シーン
	CEnemy	*pLockOnEnemy	= NULL;		// 敵
	D3DXVECTOR3	posLockOnEnemy;			// 敵位置
	float fLockOnLenght = 5000.0f;		// ロックオン可能な敵の最短距離
	bool bRangeInEnemy	= false;		// ロックオンの範囲に敵がいるか

	// 角度計算
	float fLockOnDiffL = 5000.0f;
	float fLockOnDiffR = 5000.0f;

	if (m_bLockOn == false)
	{// ロックオンしていない
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// オブジェクトタイプが敵の場合
				CEnemy		*pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
				D3DXVECTOR3 posEnemy = pEnemy->Getpos();		// 位置取得（敵）
				float		fLenght = 0.0f;					// 距離計算用

				// 距離計算
				fLenght = sqrtf(powf(posEnemy.x - pPlayer->Getpos().x, 2) + powf(posEnemy.z - pPlayer->Getpos().z, 2));

				if (fLenght < MC_ROCKON_RANGE)
				{// 一定距離以下だったら
					if (fLenght < fLockOnLenght)
					{
						fLockOnLenght = fLenght;		// 距離を保管
						pLockOnEnemy = NULL;			// NULLへ
						pLockOnEnemy = pEnemy;		// ロックオンする敵を設定
						posLockOnEnemy = posEnemy;		// ロックオンする敵との距離を保存
						bRangeInEnemy = true;			// ロックオンする敵がいる
					}
				}
			}
			pScene = pSceneNext;	// 次シーンへ
		}
	}
	else
	{// ロックオンしている
		if (m_pLockOn != NULL)
		{// NULL以外の場合
			posLockOnEnemy = m_pLockOn->Getpos();	// 位置取得

			if (m_pLockOnMarker[ROCKON_MARKER_FRAME] != NULL)
			{// NULL以外の場合
				m_pLockOnMarker[ROCKON_MARKER_FRAME]->Setpos((posLockOnEnemy + D3DXVECTOR3(0.0f, 10.0f, 0.0f)) + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 10.0f);
			}
			if (m_pLockOnMarker[ROCKON_MARKER_PLAY] != NULL)
			{// NULL以外の場合
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->Setpos((posLockOnEnemy + D3DXVECTOR3(0.0f, 10.0f, 0.0f)) + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 10.0f);
			}

			CManager::MODE mode = CManager::GetMode();		// モード取得
			if (mode == CManager::MODE_GAME)
			{// モードがゲームだったら
				CUi_Game *pUi_Game = (CUi_Game*)CManager::GetGame()->GetUi();
				if (pUi_Game != NULL)
					pUi_Game->SetGauge(CUi_Game::GAUGE_ENEMY_HP, m_pLockOn->GetHP(), m_pLockOn->GetMAXHP());		// 敵HP更新
			}

			pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得
			float fLockOnAngle = atan2f(posLockOnEnemy.x - pPlayer->Getpos().x, posLockOnEnemy.z - pPlayer->Getpos().z);

			while (pScene != NULL)
			{// NULL以外の場合
				pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

				if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
				{// オブジェクトタイプが敵の場合
					if (pScene != m_pLockOn)
					{
						CEnemy		*pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
						D3DXVECTOR3 posEnemy = pEnemy->Getpos();	// 位置取得（敵）
						float		fLenght = 0.0f;					// 距離計算用

						// 距離計算
						fLenght = sqrtf(powf(posEnemy.x - pPlayer->Getpos().x, 2) + powf(posEnemy.z - pPlayer->Getpos().z, 2));

						if (fLenght < MC_ROCKON_RANGE)
						{// 一定距離以下だったら
							D3DXVECTOR3 vecA, vecC;
							vecA = posLockOnEnemy - pPlayer->Getpos();
							vecC = pEnemy->Getpos() - pPlayer->Getpos();

							if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
							{// 左側
								float fDiff = atan2f(posEnemy.x - pPlayer->Getpos().x, posEnemy.z - pPlayer->Getpos().z);
								fDiff = fDiff - fLockOnAngle;
								fDiff = fabsf(fDiff);

								if (fDiff < fLockOnDiffL)
								{// より近い
									fLockOnDiffL = fDiff;
									m_pLockOnL = NULL;
									m_pLockOnL = pEnemy;
								}
							}
							else
							{// 右側
								float fDiff = atan2f(posEnemy.x - pPlayer->Getpos().x, posEnemy.z - pPlayer->Getpos().z);
								fDiff = fDiff - fLockOnAngle;
								fDiff = fabsf(fDiff);

								if (fDiff < fLockOnDiffR)
								{// より近い
									fLockOnDiffR = fDiff;
									m_pLockOnR = NULL;
									m_pLockOnR = pEnemy;
								}
							}
						}
					}
				}
				pScene = pSceneNext;	// 次シーンへ
			}
		}
	}

	if (bRangeInEnemy == true)
	{// 範囲内に敵がいる
		if (m_pLockOnMarker[ROCKON_MARKER_FRAME] == NULL)
		{// NULLの場合
			m_pLockOnMarker[ROCKON_MARKER_FRAME] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_ROCKON);
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->SetInitAll((posLockOnEnemy + D3DXVECTOR3(0.0f, 10.0f, 0.0f)) + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 10.0f,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->Init();
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_CAMERA_ROCKON_FRAME));
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->DrawEdit(false, false, true, true, true, 0, 3, D3DCMP_ALWAYS);
		}
		else
		{// NULL以外の場合
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->Setpos((posLockOnEnemy + D3DXVECTOR3(0.0f, 10.0f, 0.0f)) + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 10.0f);
		}

		if (m_pLockOnMarker[ROCKON_MARKER_PLAY] != NULL)
		{// NULL以外の場合
			m_pLockOnMarker[ROCKON_MARKER_PLAY]->Uninit();		// 終了処理
			m_pLockOnMarker[ROCKON_MARKER_PLAY] = NULL;			// NULLへ
		}

		CManager::MODE mode = CManager::GetMode();		// モード取得
		if (mode == CManager::MODE_GAME)
		{// モードがゲームだったら
			CUi_Game *pUi_Game = (CUi_Game*)CManager::GetGame()->GetUi();
			if (pUi_Game != NULL)
				pUi_Game->SetGauge(CUi_Game::GAUGE_ENEMY_HP, pLockOnEnemy->GetHP(), pLockOnEnemy->GetMAXHP());		// 敵HP更新

			CUi_Game::SetEnemyName(pLockOnEnemy->GetType() + 1);		// 敵名前更新
		}
	}
	else
	{// 範囲内に敵がいない
		if (m_bLockOn == true)
		{// ロックオンしている
			if (m_pLockOnMarker[ROCKON_MARKER_PLAY] == NULL)
			{// NULLの場合
				m_pLockOnMarker[ROCKON_MARKER_PLAY] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_ROCKON);
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->SetInitAll(m_pLockOnMarker[ROCKON_MARKER_FRAME]->Getpos(),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->Init();
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_CAMERA_ROCKON_PLAY));
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->DrawEdit(false, false, true, true, true, 0, 3, D3DCMP_ALWAYS);
			}
		}
		else
		{// ロックオンしていない
			if (m_pLockOnMarker[ROCKON_MARKER_FRAME] != NULL)
			{// NULL以外の場合
				m_pLockOnMarker[ROCKON_MARKER_FRAME]->Uninit();		// 終了処理
				m_pLockOnMarker[ROCKON_MARKER_FRAME] = NULL;		// NULLへ
			}
			if (m_pLockOnMarker[ROCKON_MARKER_PLAY] != NULL)
			{// NULL以外の場合
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->Uninit();		// 終了処理
				m_pLockOnMarker[ROCKON_MARKER_PLAY] = NULL;			// NULLへ
			}

			CUi_Game::SetEnemyName(0);
		}
	}

	if (fade == CFade::FADE_NONE)
	{// フェードしていない
		if ((pKey != NULL && (pKey->GetTrigger(DIK_LCONTROL) == true || pKey->GetTrigger(DIK_Q) == true)) ||
			(pMouse != NULL && pMouse->GetTrigger(4) == true) || 
			(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER, 0) == true))
		{// NULL以外の場合、キーボード（左CTRL）、Xパッド（L1）が押された
			if (pPlayer != NULL)
			{// NULL以外の場合
				// ロックオン切り替え
				m_bLockOn = m_bLockOn ? false : true;

				if (m_bLockOn == true)
				{// ロックオンしている
					if (bRangeInEnemy == true)
					{// 範囲内に敵がいる
						D3DXVECTOR3 posPlayer = pPlayer->Getpos();		// 位置取得

						m_rot.x = -0.3f;// D3DX_PI + atan2f(fLenght * 0.25f, posEnemy.y - m_posVDest.y);
						m_rot.y = D3DX_PI + atan2f(posLockOnEnemy.x - posPlayer.x, posLockOnEnemy.z - posPlayer.z);

						m_pLockOn = NULL;		// NULLへ
						m_pLockOn = pLockOnEnemy;		// ロックオン相手を設定

						m_pLockOnL = NULL;
						m_pLockOnR = NULL;

						m_fLockOn = (D3DX_PI + atan2f(posLockOnEnemy.x - posPlayer.x, posLockOnEnemy.z - posPlayer.z));		// 回転計算
						pPlayer->SetrotDest(m_fLockOn + D3DX_PI);
						CUi_Game::SetEnemyName(pLockOnEnemy->GetType() + 1);
					}
					else
					{
						m_bLockOn = false;
					}
				}
			}
		}
	}
}

//=============================================================================
// 判定チェック処理								(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::ColisionChack(void)
{
	bool bArea = false;		// エリア判定

	// マップ
	{
		CManager::MODE mode = CManager::GetMode();
		if (mode == CManager::MODE_GAME)
		{// ゲームのみ
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{
				CMap *pMap = pGame->GetMap();
				if (pMap != NULL)
				{
					int nMapNum = pGame->GetMapNum();
					if (nMapNum == 2)
					{
						CMap_02 *pMap_02 = (CMap_02*)pMap;

						if (pMap_02->CollisionCamera(m_posVDest, m_posRDest) == false)
						{
							if (pMap_02->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_02->CollisionCamera(m_posVDest, m_posRDest) == false)
								{// 一定以下だったらループ
									m_fDistance -= 1.0f;		// 距離を調整

									// 視点計算用
									D3DXMATRIX mat;						// 回転
									D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
									D3DXVECTOR3	vout;					// 計算結果

									// 視点計算
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
									D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
									m_posVDest = vout + m_posRDest;		// 注視点からの位置
								}
								bArea = true;	// エリア判定
							}
						}
						else
						{
							if (pMap_02->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_02->CollisionCamera(m_posVDest, m_posRDest) == true)
								{// 一定以上以上だったらループ
									m_fDistance += 1.0f;	// 距離を調整

									if (m_fDistanceInit < m_fDistance)
									{// 初期の値を超えたら
										m_fDistance = m_fDistanceInit;	// 距離を調整
										break;
									}

									// 視点計算用
									D3DXMATRIX mat;						// 回転
									D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
									D3DXVECTOR3	vout;					// 計算結果

																		// 視点計算
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
									D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
									m_posVDest = vout + m_posRDest;		// 注視点からの位置
								}
								bArea = true;	// エリア判定
							}
						}
					}
					else if (nMapNum == 3)
					{
						CMap_03 *pMap_03 = (CMap_03*)pMap;

						if (pMap_03->CollisionCamera(m_posVDest, m_posRDest) == false)
						{
							if (pMap_03->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_03->CollisionCamera(m_posVDest, m_posRDest) == false)
								{// 一定以下だったらループ
									m_fDistance -= 1.0f;		// 距離を調整

									// 視点計算用
									D3DXMATRIX mat;						// 回転
									D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
									D3DXVECTOR3	vout;					// 計算結果

									// 視点計算
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
									D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
									m_posVDest = vout + m_posRDest;		// 注視点からの位置
								}
								bArea = true;	// エリア判定
							}
						}
						else
						{
							if (pMap_03->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_03->CollisionCamera(m_posVDest, m_posRDest) == true)
								{// 一定以上以上だったらループ
									m_fDistance += 1.0f;	// 距離を調整

									if (m_fDistanceInit < m_fDistance)
									{// 初期の値を超えたら
										m_fDistance = m_fDistanceInit;	// 距離を調整
										break;
									}

									// 視点計算用
									D3DXMATRIX mat;						// 回転
									D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
									D3DXVECTOR3	vout;					// 計算結果

									// 視点計算
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
									D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
									m_posVDest = vout + m_posRDest;		// 注視点からの位置
								}
								bArea = true;	// エリア判定
							}
						}
					}
					else if (nMapNum == 4)
					{
						CMap_04 *pMap_04 = (CMap_04*)pMap;

						if (pMap_04->CollisionCamera(m_posVDest, m_posRDest) == false)
						{
							if (pMap_04->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_04->CollisionCamera(m_posVDest, m_posRDest) == false)
								{// 一定以下だったらループ
									m_fDistance -= 1.0f;		// 距離を調整

																// 視点計算用
									D3DXMATRIX mat;						// 回転
									D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
									D3DXVECTOR3	vout;					// 計算結果

																		// 視点計算
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
									D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
									m_posVDest = vout + m_posRDest;		// 注視点からの位置
								}
								bArea = true;	// エリア判定
							}
						}
						else
						{
							if (pMap_04->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_04->CollisionCamera(m_posVDest, m_posRDest) == true)
								{// 一定以上以上だったらループ
									m_fDistance += 1.0f;	// 距離を調整

									if (m_fDistanceInit < m_fDistance)
									{// 初期の値を超えたら
										m_fDistance = m_fDistanceInit;	// 距離を調整
										break;
									}

									// 視点計算用
									D3DXMATRIX mat;						// 回転
									D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
									D3DXVECTOR3	vout;					// 計算結果

																		// 視点計算
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
									D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
									m_posVDest = vout + m_posRDest;		// 注視点からの位置
								}
								bArea = true;	// エリア判定
							}
						}
					}
				}
			}
		}
	}

	CScene *pScene		= NULL;		// シーン
	CScene *pSceneNext	= NULL;		// 次シーン

	// フィールド
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);	// シーン取得

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// オブジェクトタイプがメッシュフィールドの場合
				CSceneMesh *pMesh = (CSceneMesh*)pScene;	// キャスト

				if (pMesh->ColInMesh(m_posVDest) == true)
				{// 範囲チェック
					if (m_posVDest.y < pMesh->Getpos().y + 5.0f)
					{// 一定以下になったら
						while (m_posVDest.y < pMesh->Getpos().y + 5.0f)
						{// 一定以下だったらループ
							m_fDistance -= 1.0f;		// 距離を調整

							// 視点計算用
							D3DXMATRIX mat;						// 回転
							D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
							D3DXVECTOR3	vout;					// 計算結果

							// 視点計算
							D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
							D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
							m_posVDest = vout + m_posRDest;		// 注視点からの位置
						}
					}
					else
					{// 一定以上に場合
						if (bArea == false)
						{// エリア判定していない
							if (m_fDistance != m_fDistanceInit)
							{// 初期の値と違う場合
								while (pMesh->Getpos().y + 5.0f < m_posVDest.y)
								{// 一定以上以上だったらループ
									m_fDistance += 1.0f;	// 距離を調整

									if (m_fDistanceInit < m_fDistance)
									{// 初期の値を超えたら
										m_fDistance = m_fDistanceInit;	// 距離を調整
										break;
									}

									// 視点計算用
									D3DXMATRIX mat;						// 回転
									D3DXVECTOR3	v(0, 0, m_fDistance);	// 注視点との距離
									D3DXVECTOR3	vout;					// 計算結果

									// 視点計算
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// 回転
									D3DXVec3TransformCoord(&vout, &v, &mat);							// 距離と合わせる
									m_posVDest = vout + m_posRDest;		// 注視点からの位置
								}
							}
						}
					}
					break;
				}
			}
			pScene = pSceneNext;	// 次シーンへ
		}
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CCameraMap ***
//=============================================================================
CCameraMap::CCameraMap() : CCamera()
{

}

//=============================================================================
// デストラクタ									(public)	*** CCameraMap ***
//=============================================================================
CCameraMap::~CCameraMap()
{

}

//=============================================================================
// 初期化処理									(public)	*** CCameraMap ***
//=============================================================================
HRESULT CCameraMap::Init(void)
{
	CCamera::Init();	// 初期化処理

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CCameraMap ***
//=============================================================================
void CCameraMap::Uninit(void)
{
	CCamera::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CCameraMap ***
//=============================================================================
void CCameraMap::Update(void)
{

}

//=============================================================================
// 描画処理										(public)	*** CCameraMap ***
//=============================================================================
void CCameraMap::SetCamera(void)
{
	CCamera::SetCamera();
}