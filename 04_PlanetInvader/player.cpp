//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "player.h"			// プレイヤー
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "sound.h"			// サウンド
#include "renderer.h"		// レンダラー
#include "camera.h"			// カメラ
#include "enemy.h"			// 敵
#include "bullet.h"			// 弾
#include "bomb.h"			// 爆弾
#include "effect.h"			// エフェクト

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME			"data\\MODEL\\player.x"			// モデル
#define MAX_LIFE			(1)								// 体力
#define MOVE_SPEED_UP		(1.5f)							// 移動速度（加速）
#define MOVE_SPEED_DOWN		(0.3f)							// 移動速度（減速）

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			*CPlayer::m_pTexture	= NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH					CPlayer::m_pMesh		= NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER				CPlayer::m_pBuffMat		= NULL;		// マテリアル情報へのポインタ
DWORD						CPlayer::m_nNumMat		= NULL;		// マテリアル情報の数

//=============================================================================
// コンストラクタ									(public)	*** CPlayer ***
//=============================================================================
CPlayer::CPlayer() : CModel3D(CScene::PRIORITY_3)
{
	m_nPlayerNum = 0;								// プレイヤー番号

	m_bmove = false;								// 移動可
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_nCntTimer = 0;								// 時間カウンタ
	m_frotDest = 0.0f;								// 向き差分
	m_nCntstate = 0;								// 状態カウント
	m_state = PLAYERSTATE_RESPAWN;					// 状態
	m_nLife = MAX_LIFE;								// 体力

	m_nCntShot = 0;									// 発射カウンタ
}

//=============================================================================
// デストラクタ										(public)	*** CPlayer ***
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CPlayer::Load(void)
{
	if (m_pBuffMat == NULL && m_pMesh == NULL)
	{// NULLの場合
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// Xファイルの読み込み
		D3DXLoadMeshFromX(MODEL_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat];
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

		for (DWORD tex = 0; tex < m_nNumMat; tex++)
		{// カウント
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// テクスチャを使用していたら
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[tex])))
				{
					MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CPlayer::Unload(void)
{
	// テクスチャの破棄
	for (DWORD tex = 0; tex < m_nNumMat; tex++)
	{// カウント
		if (m_pTexture[tex] != NULL)
		{// NULL以外の場合
			m_pTexture[tex]->Release();			// 解放
			m_pTexture[tex] = NULL;				// NULLへ
		}
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{// NULL以外の場合
		m_pBuffMat->Release();					// 解放
		m_pBuffMat = NULL;						// NULLへ
	}

	// マテリアルの開放
	if (m_pMesh != NULL)
	{// NULL以外の場合
		m_pMesh->Release();						// 解放
		m_pMesh = NULL;							// NULLへ
	}

	m_nNumMat = 0;				// 初期化
}

//=============================================================================
// 確保処理											(public)	*** CPlayer ***
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerNum)
{
	CPlayer *pPlayer = NULL;				// ポインタ

	if (pPlayer == NULL)
	{// NULLの場合
		pPlayer = new CPlayer;				// メモリ確保
	}

	if (pPlayer != NULL)
	{// NULL以外の場合
		pPlayer->Init(pos, rot, nPlayerNum);							// 初期化処理
		pPlayer->SetObjType(CScene::OBJTYPE_PLAYER);		// オブジェクトタイプ設定
	}

	return pPlayer;
}

//=============================================================================
// 初期化処理										(public)	*** CPlayer ***
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerNum)
{
	CModel3D::SetInitAll(m_pTexture, m_pMesh, m_pBuffMat, m_nNumMat, pos, rot);			// 初期値設定
	CModel3D::Init();																	// 初期化処理
	
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();		// カメラ注視点取得

	m_pos = pos - Camerapos;				// カメラ移動無効化
	
	m_nPlayerNum = nPlayerNum;				// プレイヤー番号
	m_bmove = false;						// 移動可
	m_nCntstate = 0;						// 状態カウント
	m_state = PLAYERSTATE_RESPAWN;			// 状態
	m_nLife = MAX_LIFE;						// 体力
	m_nCntShot = 0;							// 発射カウンタ

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Uninit(void)
{
	CModel3D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Update(void)
{
	CManager::MODE mode = CManager::GetMode();							// モード取得
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// カメラ注視点取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();		// 入力デバイス(キーボード)
	CInputXPad *pInputXPad = CManager::GetInputXPad();					// 入力デバイス(キーボード)
	CSound *pSound = CManager::GetSound();							// サウンド
	CInputDIPad *pInputDIPad = CManager::GetInputDIPad();				// 入力デバイス(DIパッド)

	D3DXVECTOR3 pos = CModel3D::Getpos();		// 位置を取得
	D3DXVECTOR3 rot = CModel3D::Getrot();		// 向きを取得
	D3DXVECTOR3 sizeMax = CModel3D::Getsize(0);	// 大きさを取得（最大）
	D3DXVECTOR3 sizeMin = CModel3D::Getsize(1);	// 大きさを取得（最小）

	if (mode == CManager::MODE_TUTORIAL || mode == CManager::MODE_GAME)
	{
		// カメラに対しての位置を設定
		pos.x = Camerapos.x + m_pos.x;
		pos.z = Camerapos.z + m_pos.z;

		switch (m_state)
		{// 状態
		case PLAYERSTATE_NORMAL:	// 通常
			break;
		case PLAYERSTATE_DAMAGE:	// ダメージ
			m_nCntstate++;			// 状態カウント

			// 点滅
			if (m_nCntstate / 3 % 2)
			{// 赤
				CModel3D::Setcol(D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f));
			}
			else
			{// 白
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
			}

			if (m_nCntstate == 60)
			{// 一定のカウントに達したら通常へ
				m_nCntstate = 0;
				m_state = PLAYERSTATE_NORMAL;
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			break;
		case PLAYERSTATE_RESPAWN:	// 復活
			m_nCntstate++;			// 状態カウント

			// 点滅
			if (m_nCntstate / 10 % 2)
			{// 不透明
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// 透明
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}

			if (m_nCntstate == 150)
			{// 一定のカウントに達したら通常へ
				m_nCntstate = 0;
				
				m_state = PLAYERSTATE_NORMAL;
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			break;
		}
		if (m_bmove == false)
		{// 移動不可
			pos.z += 3.0f;			// 自動移動

			if (Camerapos.z - 300.0f < pos.z)
			{// 一定位置で止まる
				m_bmove = true;		// 移動可
			}
		}
		else
		{
		/*	{
				// 移動処理
				if (pInputKeyboard->GetPress(DIK_W) == true)
				{// 任意のキー（W）を押したとき
					if (pInputKeyboard->GetPress(DIK_A) == true)
					{// + 任意のキー（A）を押したとき
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.25f) * MOVE_SPEED_UP);

						m_frotDest = -D3DX_PI * 0.25f;		// 目標向き
					}
					else if (pInputKeyboard->GetPress(DIK_D) == true)
					{// + 任意のキー（S）を押したとき
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.25f) * MOVE_SPEED_UP);

						m_frotDest = D3DX_PI * 0.25f;		// 目標向き
					}
					else
					{// 任意のキー（W）のみを押したとき
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.0f) * MOVE_SPEED_UP);

						m_frotDest = D3DX_PI * 0.0f;		// 目標向き
					}
				}
				else if (pInputKeyboard->GetPress(DIK_S) == true)
				{// 任意のキー（S）を押したとき
					if (pInputKeyboard->GetPress(DIK_A) == true)
					{// + 任意のキー（A）を押したとき
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.75f) * (MOVE_SPEED_UP * 0.8f));
					}
					else if (pInputKeyboard->GetPress(DIK_D) == true)
					{// + 任意のキー（S）を押したとき
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.75f) * (MOVE_SPEED_UP * 0.8f));
					}
					else
					{// 任意のキー（S）のみを押したとき
						m_move += D3DXVECTOR3(sinf(D3DX_PI) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI) * (MOVE_SPEED_UP * 0.8f));
					}

					m_frotDest = D3DX_PI * 0.0f;			// 目標向き
				}
				else if (pInputKeyboard->GetPress(DIK_A) == true)
				{// 任意のキー（A）を押したとき
					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// + 任意のキー（W）を押したとき
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.25f) * MOVE_SPEED_UP);
					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// + 任意のキー（S）を押したとき
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.75f) * MOVE_SPEED_UP);
					}
					else
					{// 任意のキー（A）のみを押したとき
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.5f) * MOVE_SPEED_UP);
					}

					m_frotDest = -D3DX_PI * 0.25f;			// 目標向き
				}
				else if (pInputKeyboard->GetPress(DIK_D) == true)
				{// 任意のキー（D）を押したとき
					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// + 任意のキー（W）を押したとき
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.25f) * MOVE_SPEED_UP);
					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// + 任意のキー（S）を押したとき
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.75f) * MOVE_SPEED_UP);
					}
					else
					{// 任意のキー（D）のみを押したとき
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.5f) * MOVE_SPEED_UP);
					}

					m_frotDest = D3DX_PI * 0.25f;			// 目標向き
				}
				else
				{
					m_frotDest = D3DX_PI * 0.0f;			// 目標向き
				}
			}*/

			if (pInputXPad->GetStick(0, m_nPlayerNum) == true)
			{
				m_move += D3DXVECTOR3(sinf(pInputXPad->GetStickRot(0, m_nPlayerNum)) * MOVE_SPEED_UP, 0.0f, cosf(pInputXPad->GetStickRot(0, m_nPlayerNum)) * MOVE_SPEED_UP);

				if (pInputXPad->GetStickRot(0, m_nPlayerNum) < -D3DX_PI * 0.75f ||
					D3DX_PI * 0.75f < pInputXPad->GetStickRot(0, m_nPlayerNum))
				{
					m_frotDest = D3DX_PI * 0.0f;		// 目標向き
				}
				else  if (pInputXPad->GetStickRot(0, m_nPlayerNum) < -D3DX_PI * 0.25f)
				{
					m_frotDest = -D3DX_PI * 0.25f;		// 目標向き
				}
				else if (D3DX_PI * 0.25f < pInputXPad->GetStickRot(0, m_nPlayerNum))
				{
					m_frotDest = D3DX_PI * 0.25f;		// 目標向き
				}
				else
				{
					m_frotDest = D3DX_PI * 0.0f;		// 目標向き
				}
			}
			else
			{
				m_frotDest = D3DX_PI * 0.0f;			// 目標向き
			}

			// 発射処理
			//if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
			if (pInputXPad->GetPress(XINPUT_GAMEPAD_X, m_nPlayerNum) == true || pInputXPad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, m_nPlayerNum) == true)
			{// 任意のキー（スペース）を押したとき
				if (m_nCntShot % 15 == 0)
				{
					CBullet::Create(D3DXVECTOR3(pos.x - 12.0f, pos.y, pos.z + 10.0f), D3DXVECTOR3(0.0f, 0.0f, 8.0f), D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (CBullet::BULLETTYPE)m_nPlayerNum, CBullet::BULLETSTATE_NORMAL);
					CBullet::Create(D3DXVECTOR3(pos.x + 12.0f, pos.y, pos.z + 10.0f), D3DXVECTOR3(0.0f, 0.0f, 8.0f), D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (CBullet::BULLETTYPE)m_nPlayerNum, CBullet::BULLETSTATE_NORMAL);

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE003);
				}

				m_nCntShot++;
			}
			else
			{
				m_nCntShot = 0;
			}

			//else if (pInputKeyboard->GetTrigger(DIK_B) == true)
			if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_Y, m_nPlayerNum) == true || pInputXPad->GetPress(XINPUT_GAMEPAD_LEFT_SHOULDER, m_nPlayerNum) == true)
			{// 任意のキー（スペース）を押したとき
				if (CBomb::SetShot(m_nPlayerNum) == true)
				{
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z + 10.0f), D3DXVECTOR3(0.0f, 0.0f, 5.0f), D3DXVECTOR3(BULLET_SIZE_X * 1.8f, BULLET_SIZE_Y * 1.8f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (CBullet::BULLETTYPE)m_nPlayerNum, CBullet::BULLETSTATE_BOMB);
					CBomb::SetDelete(m_nPlayerNum, true);

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE004);
				}
			}
#ifdef _DEBUG
			if (pInputKeyboard->GetTrigger(DIK_N) == true)
			{// 任意のキー（スペース）を押したとき
				CBomb::SetAdd(m_nPlayerNum, true);
			}

			if (pInputKeyboard->GetTrigger(DIK_M) == true)
			{// 任意のキー（スペース）を押したとき
				CEnemy::Create(D3DXVECTOR3(-200.0f, 10.0f, Camerapos.z + 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.3f), CEnemy::ENEMYTYPE_G_1, 70);
			}
			if (pInputKeyboard->GetTrigger(DIK_COMMA) == true)
			{// 任意のキー（スペース）を押したとき
				CEnemy::Create(D3DXVECTOR3(0.0f, 300.0f, Camerapos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.5f), CEnemy::ENEMYTYPE_S_1, 60);
			}
			if (pInputKeyboard->GetTrigger(DIK_PERIOD) == true)
			{// 任意のキー（スペース）を押したとき
				CEnemy::Create(D3DXVECTOR3(300.0f, -100.0f, Camerapos.z + 900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.15f), CEnemy::ENEMYTYPE_O_1, 80);
			}
#endif
		}

		CEffect::Create(D3DXVECTOR3(pos.x, pos.y, pos.z - 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_PLAYER, CEffect::TEXTURE_EFFECT);		// エフェクト

		// 6453 6773
		// 320
		// 642 352
		// 290
		// (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f)

		// 610(z all)
		// 120(x all)

		// 範囲外の処理
		if (pos.z + m_move.z < (Camerapos.z - 380.0f) - sizeMax.z)
		{
			m_move.z = 0.0f;
		}
		if ((Camerapos.z + 120.0f) + sizeMax.z < pos.z + m_move.z)
		{
			m_move.z = 0.0f;
		}
		if (pos.x + m_move.x < (Camerapos.x - 200.0f) - sizeMax.x)
		{
			m_move.x = 0.0f;
		}
		if ((Camerapos.x + 200.0f) + sizeMax.x < pos.x + m_move.x)
		{
			m_move.x = 0.0f;
		}
		//if (pos.x + m_move.x < (Camerapos.x - 180.0f - (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f))) + sizeMax.x)
		//{
		//	if (m_move.x < 0)
		//	{
		//		m_move.x = 0.0f;
		//	}
		//	pos.x = (Camerapos.x - 180.0f - (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f))) + sizeMax.x;
		//}
		//if ((Camerapos.x + 180.0f + (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f))) - sizeMax.x < pos.x + m_move.x)
		//{
		//	if (0 < m_move.x)
		//	{
		//		m_move.x = 0.0f;
		//	}
		//	pos.x = (Camerapos.x + 180.0f + (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f))) - sizeMax.x;
		//}

		pos += m_move;		// 移動量代入

		float fDiff = (m_frotDest - rot.z) / 8.0f;		// 向きの差分計算
		rot.z += fDiff;		// 向き代入

		// 減速処理
		m_move.x += (0.0f - m_move.x) * MOVE_SPEED_DOWN;
		m_move.z += (0.0f - m_move.z) * MOVE_SPEED_DOWN;

		CModel3D::Setpos(pos);					// 位置を設定
		CModel3D::Setrot(rot);					// 向きを設定

		m_pos = pos - Camerapos;				// 現在位置を計算
	}
}

//=============================================================================
// 描画処理											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Draw(void)
{
	CModel3D::Draw();						// 描画処理
}