//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "enemy.h"			// 敵
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "camera.h"			// カメラ
#include "bullet.h"			// 弾
#include "player.h"			// プレイヤー
#include "effect.h"			// エフェクト

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME_1		"data\\MODEL\\enemy000.x"	// モデル1
#define MODEL_NAME_2		"data\\MODEL\\enemy100.x"	// モデル2
#define MODEL_NAME_3		"data\\MODEL\\enemy200.x"	// モデル3
#define MODEL_NAME_4		"data\\MODEL\\enemy300.x"	// モデル4
#define MAX_LIFE_1			(4)							// 体力1
#define MAX_LIFE_2			(6)							// 体力2
#define MAX_LIFE_3			(7)							// 体力3
#define MAX_LIFE_4			(600)						// 体力4
#define MOVE_SPEED_UP		(3.0f)						// 移動速度
#define MOVE_SPEED_DOWN		(0.3f)						// 移動速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			*CEnemy::m_pTexture[ENEMYTYPE_MAX]	= {};	// テクスチャ情報へのポインタ
LPD3DXMESH					CEnemy::m_pMesh[ENEMYTYPE_MAX]		= {};	// メッシュ情報へのポインタ
LPD3DXBUFFER				CEnemy::m_pBuffMat[ENEMYTYPE_MAX]	= {};	// マテリアル情報へのポインタ
DWORD						CEnemy::m_nNumMat[ENEMYTYPE_MAX]	= {};	// マテリアル情報の数

//=============================================================================
// コンストラクタ									(public)	*** CEnemy ***
//=============================================================================
CEnemy::CEnemy() : CModel3D(CScene::PRIORITY_3)
{
	// 初期値設定
	m_enemyType = ENEMYTYPE_S_1;				// 種類
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_nCntTimer = 0;							// 時間カウンタ
	m_nLife = MAX_LIFE_1;						// 体力

	m_nNumPattern = 0;
}

//=============================================================================
// デストラクタ										(public)	*** CEnemy ***
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	char cName[255] = {};		// 名前格納

	for (int nCntModel = 0; nCntModel < ENEMYTYPE_MAX; nCntModel++)
	{// カウント
		if (m_pBuffMat[nCntModel] == NULL && m_pMesh[nCntModel] == NULL)
		{// NULLの場合
			switch (nCntModel)
			{// カウント番号
			case ENEMYTYPE_S_1:
			case ENEMYTYPE_S_2:
			case ENEMYTYPE_B_2:
				wsprintf(cName, MODEL_NAME_1);
				
				break;
			case ENEMYTYPE_G_1:
			case ENEMYTYPE_G_2:
			case ENEMYTYPE_B_3:
				wsprintf(cName, MODEL_NAME_2);

				break;
			case ENEMYTYPE_O_1:
			case ENEMYTYPE_O_2:
				wsprintf(cName, MODEL_NAME_3);

				break;
			case ENEMYTYPE_B_1:
				wsprintf(cName, MODEL_NAME_4);

				break;
			}

			// Xファイルの読み込み
			D3DXLoadMeshFromX
			(
				cName,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_pBuffMat[nCntModel],
				NULL,
				&m_nNumMat[nCntModel],
				&m_pMesh[nCntModel]
			);

			D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
			D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
			pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
			matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
			m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

			for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
			{
				matDef[tex] = pmat[tex].MatD3D;
				matDef[tex].Ambient = matDef[tex].Diffuse;
				m_pTexture[nCntModel][tex] = NULL;
				if (pmat[tex].pTextureFilename != NULL &&
					lstrlen(pmat[tex].pTextureFilename) > 0)
				{
					if (FAILED(D3DXCreateTextureFromFile(pDevice,
						pmat[tex].pTextureFilename,
						&m_pTexture[nCntModel][tex])))
					{
						MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
					}
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CEnemy::Unload(void)
{
	for (int nCntModel = 0; nCntModel < ENEMYTYPE_MAX; nCntModel++)
	{// カウント
		// テクスチャの破棄
		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{// カウント
			if (m_pTexture[nCntModel][tex] != NULL)
			{// NULL以外の場合
				m_pTexture[nCntModel][tex]->Release();		// 解放
				m_pTexture[nCntModel][tex] = NULL;			// NULLへ
			}
		}

		// マテリアルの開放
		if (m_pBuffMat[nCntModel] != NULL)
		{// NULL以外の場合
			m_pBuffMat[nCntModel]->Release();				// 解放
			m_pBuffMat[nCntModel] = NULL;					// NULLへ
		}

		// マテリアルの開放
		if (m_pMesh[nCntModel] != NULL)
		{// NULL以外の場合
			m_pMesh[nCntModel]->Release();					// 解放
			m_pMesh[nCntModel] = NULL;						// NULLへ
		}

		m_nNumMat[nCntModel] = 0;							// 初期値へ
	}
}

//=============================================================================
// 確保処理											(public)	*** CEnemy ***
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, ENEMYTYPE enemyType, int nShotTiming)
{
	CEnemy *pEnemy = NULL;					// ポインタ

	if (pEnemy == NULL)
	{// NULLの場合
		pEnemy = new CEnemy;				// メモリ確保
	}

	if (pEnemy != NULL)
	{// NULL以外の場合
		pEnemy->Init(pos, rot, move, enemyType, nShotTiming);		// 初期化処理
		pEnemy->SetObjType(CScene::OBJTYPE_ENEMY);			// オブジェクトタイプ設定
	}

	return pEnemy;		// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CEnemy ***
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, ENEMYTYPE enemyType, int nShotTiming)
{
	CModel3D::SetInitAll(m_pTexture[enemyType], m_pMesh[enemyType], m_pBuffMat[enemyType], m_nNumMat[enemyType], pos, rot);		// 初期値設定
	CModel3D::Init();																											// 初期化処理

	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();		// カメラ注視点取得

	m_pos = pos - Camerapos;		// カメラ移動無効化
	m_move = move;					// 移動量
	m_enemyType = enemyType;		// 種類
	m_nShotTiming = nShotTiming;	// 発射タイミング

	switch (enemyType)
	{
	case ENEMYTYPE_S_1:
		m_nLife = MAX_LIFE_1;
		break;
	case ENEMYTYPE_G_1:
		m_nLife = (int)(MAX_LIFE_1 * 1.4);
		break;
	case ENEMYTYPE_O_1:
		m_nLife = (int)(MAX_LIFE_1 * 1.8);
		break;
	case ENEMYTYPE_S_2:
		m_nLife = MAX_LIFE_2;
		break;
	case ENEMYTYPE_G_2:
		m_nLife = (int)(MAX_LIFE_2 * 1.2);
		break;
	case ENEMYTYPE_O_2:
		m_nLife = (int)(MAX_LIFE_2 * 1.4);
		break;
	case ENEMYTYPE_B_1:
		m_nLife = (int)(MAX_LIFE_4);
		m_nLife += (int)(m_nLife * (1.5 * CManager::GetPlayNum()));
		break;
	case ENEMYTYPE_B_2:
		m_nLife = (int)(MAX_LIFE_3 * 1.0);
		break;
	}

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Uninit(void)
{
	CModel3D::Uninit();				// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();		// カメラ注視点取得
	D3DXVECTOR3 pos = CModel3D::Getpos();	// 位置を取得
	D3DXVECTOR3 rot = CModel3D::Getrot();	// 向きを取得
	float frotplayer = 0.0f;				// プレイヤー向き
	float frotDiff = 0.0f;					// 向き差分
	bool bPlayer = false;					// プレイヤーいるか

	// カメラに対しての位置を設定
	pos.x = Camerapos.x + m_pos.x;
	pos.z = Camerapos.z + m_pos.z;

	// 判定チェック------------------------------------------------------------
	CModel3D *pModel3D;						// モデル3D
	CScene::OBJTYPE objType;				// オブジェクト種類
	CPlayer *pPlayer_1 = NULL;				// プレイヤー1
	CPlayer *pPlayer_2 = NULL;				// プレイヤー2
	D3DXVECTOR3 targetpos;					// 判定用 : 相手の位置
	int nCntPlayer = 0;

	for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
	{// シーンカウント
		pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_3, nCntScene);	// シーン読み込み

		if (pModel3D != NULL)
		{// NULL以外の場合
			objType = pModel3D->GetObjType();				// オブジェクト種類の取得

			if (objType == CScene::OBJTYPE_PLAYER)
			{// 種類チェック
				CPlayer *pPlayer = (CPlayer*)pModel3D;

				if (pPlayer->GetPlayerNum() == 0)
				{
					pPlayer_1 = pPlayer;
				}
				else if (pPlayer->GetPlayerNum() == 1)
				{
					pPlayer_2 = pPlayer;
				}

				nCntPlayer++;

				bPlayer = true;

				if (nCntPlayer == CManager::GetPlayNum())
				{
					break;
				}
			}
		}
	}

	if (bPlayer == true)
	{// プレイヤーいるか
		D3DXVECTOR3 targetpos_1, targetpos_2;

		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			if (nCnt == 0)
			{
				if (pPlayer_1 != NULL)
				{
					targetpos_1 = pPlayer_1->Getpos();;
				}
			}
			else if (nCnt == 1)
			{
				if (pPlayer_2 != NULL)
				{
					targetpos_2 = pPlayer_2->Getpos();
				}
			}
		}

		if (CManager::GetPlayNum() == MAX_PLAYER)
		{
			float diff_1, diff_2;

			diff_1 = sqrtf(((targetpos_1.x - pos.x) * (targetpos_1.x - pos.x)) + ((targetpos_1.z - pos.z) * (targetpos_1.z - pos.z)));
			diff_2 = sqrtf(((targetpos_2.x - pos.x) * (targetpos_2.x - pos.x)) + ((targetpos_2.z - pos.z) * (targetpos_2.z - pos.z)));
			
			if (diff_1 < diff_2)
			{
				targetpos = targetpos_1;
			}
			else
			{
				targetpos = targetpos_2;
			}
		}
		else
		{
			targetpos = targetpos_1;
		}
	}

	m_nCntTimer++;							// カウント

	if (m_enemyType == ENEMYTYPE_S_1 || m_enemyType == ENEMYTYPE_S_2)
	{// 空1
		if (bPlayer == true)
		{// プレイヤーいるか
			if (m_enemyType == ENEMYTYPE_S_1)
			{
				frotplayer = atan2f(-targetpos.x - -pos.x, -targetpos.z - -pos.z);		// 向き計算

				if (targetpos.z + 200.0f < pos.z)
				{// 使用範囲
					frotDiff = (frotplayer - rot.y);		// 差分計算

					// 向き調整
					if (frotDiff < -D3DX_PI)
					{
						frotDiff += D3DX_PI * 2.0f;
					}
					if (D3DX_PI < frotDiff)
					{
						frotDiff -= D3DX_PI * 2.0f;
					}

					rot.y += frotDiff / 30.0f;				// 向き加算
				}
			}

			if ((m_nCntTimer % m_nShotTiming) == 0)
			{// パターンで弾を撃つ
				CBullet::Create(pos, D3DXVECTOR3(sinf(rot.y) * -5.0f, 0.0f, cosf(rot.y) * -5.0f),
					D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
			}
		}

		pos += D3DXVECTOR3(sinf(rot.y) * m_move.x, 0.0f, cosf(rot.y) * m_move.z);			// 位置加算

		CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 20.0f, pos.y, pos.z + cosf(rot.y) * 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_EFFECT);	// エフェクト
	}
	else if (m_enemyType == ENEMYTYPE_G_1 || m_enemyType == ENEMYTYPE_G_2)
	{// 陸1
		if (bPlayer == true)
		{// プレイヤーいるか
			// 位置計算
			D3DXVECTOR3 posOBJ = D3DXVECTOR3(
				Camerapos.x + (((pos.x) - (Camerapos.x)) * 0.6f),
				300.0f,
				Camerapos.z - 350.0f + (((pos.z) - (Camerapos.z - 250.0f)) * 0.6f));

			frotplayer = atan2f(-targetpos.x - -pos.x, -targetpos.z - -pos.z);		// 向き計算

			frotDiff = (frotplayer - rot.y);		// 差分計算

			// 向き調整
			if (frotDiff < -D3DX_PI)
			{
				frotDiff += D3DX_PI * 2.0f;
			}
			if (D3DX_PI < frotDiff)
			{
				frotDiff -= D3DX_PI * 2.0f;
			}

			if ((m_nCntTimer % m_nShotTiming) == 0)
			{// パターンで弾を撃つ
				CBullet::Create(posOBJ, D3DXVECTOR3(sinf(frotplayer) * -5.0f, 0.0f, cosf(frotplayer) * -5.0f),
					D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
			}
		}

		if (m_enemyType == ENEMYTYPE_G_1)
		{
			pos += D3DXVECTOR3(m_move.x, 0.0f, m_move.z);		// 位置加算
		}

		rot.y += frotDiff / 20.0f;					// 向き加算

		// 向き調整
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		if (D3DX_PI < rot.y)
		{
			rot.y -= D3DX_PI * 2.0f;
		}

		if (m_enemyType == ENEMYTYPE_G_2)
		{
			if (-D3DX_PI * 0.5f < rot.y && rot.y < D3DX_PI * 0.5f)
			{
				pos += D3DXVECTOR3(sinf(rot.y) * m_move.x, 0.0f, cosf(rot.y) * m_move.z);			// 位置加算
			}
			else
			{
				pos += D3DXVECTOR3(sinf(rot.y) * m_move.x, 0.0f, cosf(rot.y) * (m_move.z * -0.3f));
			}
		}

		if ((m_nCntTimer % 2) == 0)
		{// パターンで弾を撃つ
			CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 20.0f, pos.y, pos.z + cosf(rot.y) * 20.0f), D3DXVECTOR3(0.0f, rot.y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, -2.8f), D3DXVECTOR3(EFFECT_SIZE_X * 2.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.02f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_TANK);	// エフェクト
		}
	}
	else if (m_enemyType == ENEMYTYPE_O_1)
	{// 海1
		if (bPlayer == true)
		{// プレイヤーいるか
			// 位置計算
			D3DXVECTOR3 posOBJ = D3DXVECTOR3(
				Camerapos.x + (((pos.x) - (Camerapos.x)) * 0.55f),
				300.0f,
				Camerapos.z - 350.0f + (((pos.z) - (Camerapos.z - 250.0f)) * 0.55f));

			frotplayer = atan2f(-targetpos.x - -posOBJ.x, -targetpos.z - -posOBJ.z);	// 向き計算

			frotDiff = (frotplayer - rot.y);		// 差分計算

			// 向き調整
			if (frotDiff < -D3DX_PI)
			{
				frotDiff += D3DX_PI * 2.0f;
			}
			if (D3DX_PI < frotDiff)
			{
				frotDiff -= D3DX_PI * 2.0f;
			}

			if (pos.z - Camerapos.z < (Camerapos.z + 120.0f))
			{
				if ((m_nCntTimer % m_nShotTiming) == 0)
				{// パターンで弾を撃つ
					CBullet::Create(posOBJ, D3DXVECTOR3(sinf(frotplayer) * -5.0f, 0.0f, cosf(frotplayer) * -5.0f),
						D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
				}
			}

			if ((m_nCntTimer % 2) == 0)
			{// パターンで弾を撃つ
				CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 20.0f, pos.y, pos.z + cosf(rot.y) * 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, -2.0f), D3DXVECTOR3(EFFECT_SIZE_X * 2.0f, EFFECT_SIZE_Y * 2.0f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.02f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_WATER);	// エフェクト
			}
		}

		pos += D3DXVECTOR3(m_move.x, 0.0f, m_move.z);
	}
	else if (m_enemyType == ENEMYTYPE_B_1)
	{
		if (bPlayer == true)
		{// プレイヤーいるか
			if (m_nNumPattern == 0)
			{
				if (Camerapos.z + 300.0f < pos.z)
				{
					pos += D3DXVECTOR3(m_move.x, 0.0f, m_move.z);
				}
				else
				{
					m_nNumPattern = 1;
					m_nCntTimer = 0;
				}

				if (CManager::GetPlayNum() == MAX_PLAYER)
				{
					if (m_nLife < (MAX_LIFE_4 * 1.5f) * 0.5f)
					{
						m_nNumPattern = 3;
						m_nCntTimer = 0;
					}
				}
				else
				{
					if (m_nLife < MAX_LIFE_4 * 0.5f)
					{
						m_nNumPattern = 3;
						m_nCntTimer = 0;
					}
				}
			}
			else if (m_nNumPattern == 1)
			{
				if (60 < m_nCntTimer)
				{
					if (pos.z < Camerapos.z + 300.0f)
					{
						pos.z += 2.0f;
					}
					else
					{
						if (pos.x <= Camerapos.x + 300.0f)
						{
							pos.x += 2.0f;
							
							if (Camerapos.x + 300.0f < pos.x)
							{
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z + 100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z + 30.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z - 40.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z - 110.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
							}
						}	

						if (m_nCntTimer == 510)
						{
							m_nNumPattern = 2;
							m_nCntTimer = 0;
						}

						if (CManager::GetPlayNum() == MAX_PLAYER)
						{
							if (m_nLife < (MAX_LIFE_4 * 1.5f) * 0.5f)
							{
								m_nNumPattern = 3;
								m_nCntTimer = 0;
							}
						}
						else
						{
							if (m_nLife < MAX_LIFE_4 * 0.5f)
							{
								m_nNumPattern = 3;
								m_nCntTimer = 0;
							}
						}
					}
				}
			}
			else if (m_nNumPattern == 2)
			{
				if (60 < m_nCntTimer)
				{
					if (pos.z < Camerapos.z + 300.0f)
					{
						pos.z += 2.0f;
					}
					else
					{
						if (Camerapos.x - 300.0f <= pos.x)
						{
							pos.x -= 2.0f;

							if (pos.x < Camerapos.x - 300.0f)
							{
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z + 100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z + 30.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z - 40.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z - 110.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
							}
						}

						if (m_nCntTimer == 510)
						{
							m_nNumPattern = 1;
							m_nCntTimer = 0;
						}

						if (CManager::GetPlayNum() == MAX_PLAYER)
						{
							if (m_nLife < (MAX_LIFE_4 * 1.5f) * 0.5f)
							{
								m_nNumPattern = 3;
								m_nCntTimer = 0;
							}
						}
						else
						{
							if (m_nLife < MAX_LIFE_4 * 0.5f)
							{
								m_nNumPattern = 3;
								m_nCntTimer = 0;
							}
						}
					}
				}
			}
			else if (m_nNumPattern == 3)
			{
				if (m_nCntTimer < 60)
				{
					if (pos.x != 0.0f)
					{
						pos.x += ((0.0f - pos.x) / (60.0f - m_nCntTimer));

						if (pos.x < 1.0f && -1.0f < pos.x)
						{
							pos.x = 0.0f;

							CEnemy::Create(D3DXVECTOR3(pos.x - 22.0f, 18.0f, pos.z + 100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_3, 90);
							CEnemy::Create(D3DXVECTOR3(pos.x - 22.0f, 18.0f, pos.z + 30.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_3, 90);
							CEnemy::Create(D3DXVECTOR3(pos.x - 22.0f, 18.0f, pos.z - 40.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_3, 90);
							CEnemy::Create(D3DXVECTOR3(pos.x - 22.0f, 18.0f, pos.z - 110.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_3, 90);
						}
					}
				}
				else
				{
					if (m_nCntTimer % 300 == 0)
					{
						CEnemy::Create(D3DXVECTOR3(pos.x, 40.0f, pos.z + 150.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 75);
						CEnemy::Create(D3DXVECTOR3(pos.x, 40.0f, pos.z + 90.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 75);
						CEnemy::Create(D3DXVECTOR3(pos.x, 40.0f, pos.z + 30.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 75);
					}
				}
			}
		}

		if ((m_nCntTimer % 2) == 0)
		{// パターンで弾を撃つ
			CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 180.0f, pos.y, pos.z + cosf(rot.y) * -180.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, -5.0f), D3DXVECTOR3(EFFECT_SIZE_X * 2.0f, EFFECT_SIZE_Y * 2.0f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.02f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_WATER);	// エフェクト
		}
	}
	else if (m_enemyType == ENEMYTYPE_B_2)
	{
		if (20 < m_nCntTimer)
		{
			if (m_nCntTimer < 100)
			{
				pos += D3DXVECTOR3(0.0f, 2.0f, 4.0f);			// 位置加算
			}
			else 
			{
				if (m_nCntTimer == 100)
				{
					int nNum = rand() % 5;

					pos.x = -300.0f + (150.0f * nNum);
					pos.y = 300.0f;
					CModel3D::Setpos(pos);					// 位置を設定
				}

				if (bPlayer == true)
				{// プレイヤーいるか			
					frotplayer = atan2f(-targetpos.x - -pos.x, -targetpos.z - -pos.z);		// 向き計算

					if (targetpos.z + 200.0f < pos.z)
					{// 使用範囲
						frotDiff = (frotplayer - rot.y);		// 差分計算

						// 向き調整
						if (frotDiff < -D3DX_PI)
						{
							frotDiff += D3DX_PI * 2.0f;
						}
						if (D3DX_PI < frotDiff)
						{
							frotDiff -= D3DX_PI * 2.0f;
						}

						rot.y += frotDiff / 30.0f;				// 向き加算
					}

					if ((m_nCntTimer % m_nShotTiming) == 0)
					{// パターンで弾を撃つ
						CBullet::Create(pos, D3DXVECTOR3(sinf(rot.y) * -5.0f, 0.0f, cosf(rot.y) * -5.0f),
							D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
					}
				}

				pos += D3DXVECTOR3(sinf(rot.y) * m_move.x, 0.0f, cosf(rot.y) * m_move.z);			// 位置加算

				CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 20.0f, pos.y, pos.z + cosf(rot.y) * 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, 3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_EFFECT);	// エフェクト
			}
		}
	}
	else if (m_enemyType == ENEMYTYPE_B_3)
	{
		if (bPlayer == true)
		{// プレイヤーいるか
		 // 位置計算
			D3DXVECTOR3 posOBJ = D3DXVECTOR3(
				Camerapos.x + (((pos.x) - (Camerapos.x)) * 0.6f),
				300.0f,
				Camerapos.z - 350.0f + (((pos.z) - (Camerapos.z - 250.0f)) * 0.6f));

			frotplayer = atan2f(-targetpos.x - -pos.x, -targetpos.z - -pos.z);		// 向き計算

			frotDiff = (frotplayer - rot.y);		// 差分計算

			// 向き調整
			if (frotDiff < -D3DX_PI)
			{
				frotDiff += D3DX_PI * 2.0f;
			}
			if (D3DX_PI < frotDiff)
			{
				frotDiff -= D3DX_PI * 2.0f;
			}

			rot.y += frotDiff / 20.0f;					// 向き加算

			// 向き調整
			if (rot.y < -D3DX_PI)
			{
				rot.y += D3DX_PI * 2.0f;
			}
			if (D3DX_PI < rot.y)
			{
				rot.y -= D3DX_PI * 2.0f;
			}

			if ((m_nCntTimer % m_nShotTiming) == 0)
			{// パターンで弾を撃つ
				CBullet::Create(posOBJ, D3DXVECTOR3(sinf(frotplayer) * -5.0f, 0.0f, cosf(frotplayer) * -5.0f),
					D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
			}
		}
	}

	// 向き調整
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}
	if (D3DX_PI < rot.y)
	{
		rot.y -= D3DX_PI * 2.0f;
	}

	CModel3D::Setpos(pos);					// 位置を設定
	CModel3D::Setrot(rot);					// 向きを設定

	m_pos = pos - Camerapos;				// 現在位置を計算

	if (Camerapos.z - 450.0f > pos.z)
	{// カメラ範囲外にでたら
		Uninit();							// 終了処理
	}
}

//=============================================================================
// 描画処理											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Draw(void)
{
	CModel3D::Draw();						// 描画処理
}