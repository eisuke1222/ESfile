//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "enemy.h"			// 敵
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "text.h"			// テキスト
#include "camera.h"			// カメラ
#include "fade.h"			// フェード

#include "scenemesh.h"		// シーンメッシュ
#include "game.h"			// ゲーム
#include "model.h"			// モデル
#include "motion.h"			// モーション
#include "mapicon.h"		// マップアイコン
#include "map.h"			// マップ
#include "texture.h"		// テクスチャ
#include "texanim.h"		// テクスチャアニメーション
#include "gauge.h"			// ゲージ
#include "player.h"			// プレイヤー
#include "shadow.h"			// 影
#include "effect.h"			// エフェクト

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MC_MOVE_UP		(1.5f)								// 加速
#define MC_MOVE_DOWN	(0.75f)								// 減速
#define MC_JUMP			(4.0f)								// ジャンプ量
#define MC_GRAVITY		(0.3f)								// 重力
#define MC_DRAW_RANGE	(80.0f)								// 描画距離

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// コンストラクタ										(public)	*** CEnemy ***
//=============================================================================
CEnemy::CEnemy() : CScene(CScene::PRIORITY_3, CScene::OBJTYPE_ENEMY)
{
	// 初期値設定
	m_MapIcon		= NULL;									// マップアイコン

	m_bUpdata		= true;									// 更新する
	m_bMove			= true;									// 移動できる

	m_nHP			= 0;									// HP
	m_nMAXHP		= 0;									// MAXHP

	m_state			= STATE_NORMAL;							// 状態
	m_nCntState		= 0;									// 状態カウント
	m_vtxMax		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 判定MAX
	m_vtxMin		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 判定MIN
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置前回
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_frotDest		= 0.0f;									// 目標向き
	m_bJump			= false;								// ジャンプ

	m_shadowHeight = 0.0f;									// 影高さ
}

//=============================================================================
// デストラクタ										(public)	*** CEnemy ***
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ロード処理										(public)	*** CEnemy ***
//=============================================================================
HRESULT CEnemy::Load(void)
{
	CSlime::Load();			// スライム
	CSlime_King::Load();	// スライム

	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Unload(void)
{
	CSlime::Unload();			// スライム
	CSlime_King::Unload();		// スライムキング
}

//=============================================================================
// クリエイト										(public)	*** CEnemy ***
//=============================================================================
CEnemy *CEnemy::Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = NULL;		// ポインタ

	if (pEnemy == NULL)
	{// NULLの場合
		if (type == TYPE_N_SLIME)		// スライム
			pEnemy = new CSlime;			// メモリ確保
		if (type == TYPE_B_SLIME)		// スライムキング
			pEnemy = new CSlime_King;		// メモリ確保
	}

	if (pEnemy != NULL)
	{// NULL以外の場合
		pEnemy->m_nType		= type;		// タイプ番号
		pEnemy->m_pos		= pos;		// 位置
		pEnemy->m_rot		= rot;		// 向き
		pEnemy->m_frotDest	= rot.y;	// 目標向き
		pEnemy->Init();					// 初期化処理
	}

	return pEnemy;		// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CEnemy ***
//=============================================================================
HRESULT CEnemy::Init()
{
	// マップアイコン
	if (m_MapIcon == NULL)
	{// NULLの場合
		m_MapIcon = CMapIcon::Create(PRIORITY_MAP_1, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), m_rot, D3DXVECTOR3(70.0f, 0.0f, 70.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_MapIcon->SetTex(2, 1, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	}

	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{
		pGame->SetEnemyNum(pGame->GetEnemyNum() + 1);
	}

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Uninit(void)
{
	// マップアイコン
	if (m_MapIcon != NULL)
	{// NULL以外の場合
		m_MapIcon->Uninit();	// 破棄
		m_MapIcon = NULL;		// NULLへ
	}

	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{
		CGame::GAME game = pGame->GetGame();
		if (game != CGame::GAME_TUTORIAL && game != CGame::GAME_CLEAR && game != CGame::GAME_OVER)
		{
			pGame->AddDeathEnemy();
		}
		pGame->SetEnemyNum(pGame->GetEnemyNum() - 1);

		CPlayer *pPlayer = pGame->GetPlayer();
		if (pPlayer != NULL)
		{
			pPlayer->AddMP(4);
		}
	}

	CScene::SetDeath();		// 死亡フラグ
}

//=============================================================================
// ヒットダメージ処理									(public)	*** CEnemy ***
//=============================================================================
bool CEnemy::HitDamage(int nDamage)
{
	bool bLand = false;

	if (m_state == STATE_NORMAL)
	{
		m_state = STATE_DAMAGE;
		m_nHP += nDamage;	// ダメージ代入
		bLand = true;

		if (m_nHP < 0)
			m_nHP = 0;		// HP調整
	}

	return bLand;
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CSlime::m_MaxModel = 0;		// モデル数
int						CSlime::m_MaxMotion = 0;		// モーション数

LPDIRECT3DTEXTURE9		**CSlime::m_pTexture = NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CSlime::m_pMesh = NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER			*CSlime::m_pBuffMat = NULL;		// マテリアル情報へのポインタ
DWORD					*CSlime::m_nNumMat = NULL;		// マテリアル情報の数

CModel::MODEL_INFO		*CSlime::m_ModelInfo = NULL;		// モデルインフォ
CMotion::MOTION_INFO	*CSlime::m_MotionInfo = NULL;		// モーションインフォ

//=============================================================================
// コンストラクタ										(public)	*** CSlime ***
//=============================================================================
CSlime::CSlime() : CEnemy()
{
	m_pModel = NULL;									// モデル
	m_pMotion = NULL;									// モーション

	m_AiType = AI_NORMAL;								// AIタイプ
	m_nMoveRecast = 0;									// 移動リキャスト
	m_bAttack = false;									// 攻撃している
	m_nCntAttack = 0;									// 攻撃カウント

	m_bAutoMove	= false;								// 自動移動しているか
	m_nCntAutoMove = 0;									// 自動移動カウント
	m_nMaxAutoMove = 0;									// 自動移動カウント最大
	m_nAutoMoveRecast = 0;								// 自動移動リキャスト

	m_bKnockBack = false;								// ノックバック
	m_nCntKnockBack = 0;								// ノックバックカウント

	m_bLockOn = false;									// ロックオン
}

//=============================================================================
// デストラクタ										(public)	*** CSlime ***
//=============================================================================
CSlime::~CSlime()
{

}

//=============================================================================
// ロード処理										(public)	*** CSlime ***
//=============================================================================
HRESULT CSlime::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MODEL\\slimemodel.txt", &m_MaxModel);		// モデル取得
	//m_MotionInfo	= CText::GetMotion(0);					// モーション取得 

	// 各情報のメモリ確保
	if (m_pTexture	== NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_MaxModel];
	if (m_pMesh		== NULL) m_pMesh	= new LPD3DXMESH[m_MaxModel];
	if (m_pBuffMat	== NULL) m_pBuffMat = new LPD3DXBUFFER[m_MaxModel];
	if (m_nNumMat	== NULL) m_nNumMat	= new DWORD[m_MaxModel];

	// 各情報の初期化
	for (int nCount = 0; nCount < m_MaxModel; nCount++)		// モデル数カウント
	{	m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;	}

	for (int nCntParnet = 0; nCntParnet < m_MaxModel; nCntParnet++)
	{// モデル数カウント
		// Xファイルの読み込み
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CSlime ***
//=============================================================================
void CSlime::Unload(void)
{
	// 各情報のメモリ開放、NULLへ
	if (m_pTexture	!= NULL) { delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL) { delete[] m_pMesh;	m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL) { delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL) { delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// モデルインフォ
	if (m_ModelInfo	 != NULL)	m_ModelInfo = NULL;		// NULLへ

	// モーションインフォ
	if (m_MotionInfo != NULL)	m_MotionInfo = NULL;	// NULLへ
}

//=============================================================================
// 初期化処理										(public)	*** CSlime ***
//=============================================================================
HRESULT CSlime::Init(void)
{
	CEnemy::Init();		// 初期化処理

	// モデル
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULLの場合
		m_pModel = new CModel[m_MaxModel];	// メモリ確保

		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// モデル数カウント
		 // 初期値設定
			m_pModel[nCntModel].SetInitAll(m_pTexture[nCntModel], m_pMesh[nCntModel], m_pBuffMat[nCntModel], m_nNumMat[nCntModel], m_ModelInfo[nCntModel].pos, m_ModelInfo[nCntModel].rot);
			// 初期化処理
			m_pModel[nCntModel].Init();

			// 親モデルを設定
			if (0 <= m_ModelInfo[nCntModel].nIndex)		// 親以外
				m_pModel[nCntModel].SetParent(&m_pModel[m_ModelInfo[nCntModel].nIndex]);
		}
	}

	// モーション
	if (m_pMotion == NULL && m_MotionInfo != NULL)
	{// NULLの場合
		m_pMotion = new CMotion;	// メモリ確保

		m_pMotion->SetMotionNum(0);	// 待機モーション
	}

	m_nHP = m_MAX_HP;	// 体力設定
	m_nMAXHP = m_MAX_HP;	// 体力設定

	// 判定設定
	m_vtxMax = D3DXVECTOR3(14.0f, 20.0f, 14.0f);
	m_vtxMin = D3DXVECTOR3(-14.0f, 0.0f, -14.0f);

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CSlime ***
//=============================================================================
void CSlime::Uninit(void)
{
	CEffect::Create(CEffect::TYPE_CREATE, CEffect::SHOT_NONE, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	CCameraMain *pCameraMain = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	if (pCameraMain->GetLockOn() == true)
	{// ロックオンしている
		if (pCameraMain->GetLockOnEnemy() == this)
		{// 同じだったら
			pCameraMain->SetLockOn(false);		// ロックオンしない
		}
	}

	// モデル
	if (m_pModel != NULL)
	{// NULL以外の場合
		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++) // モデル数カウント
			m_pModel[nCntModel].Uninit();	// 終了処理

		delete[] m_pModel;		// メモリ開放
		m_pModel = NULL;		// NULLへ
	}

	// モーション
	if (m_pMotion != NULL)
	{// NULL以外の場合
		m_pMotion->Uninit();	// 終了処理
		delete m_pMotion;		// メモリ開放
		m_pMotion = NULL;		// NULLへ
	}

	CEnemy::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CSlime ***
//=============================================================================
void CSlime::Update(void)
{
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);

	State();	// 状態処理

	m_posOld = m_pos;				// 位置保存

	if (m_bKnockBack == false)
	{
		if (pCamera->GetPickUp() == false)
			Ai();					// Ai
	}
	else
	{
		if (m_nCntKnockBack < 30)
		{
			m_nCntKnockBack++;
		}
		else
		{
			m_bKnockBack = false;
			m_nCntKnockBack = 0;
		}
	}

	m_pos += m_move;				// 移動量加算

	CScene *pScene		= NULL;		// シーン
	CScene *pSceneNext	= NULL;		// 次シーン

	// 敵
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// オブジェクトタイプが敵の場合
				CEnemy *pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
				if (pEnemy != this)
				{
					pEnemy->Collision(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin, true);
				}
			}
			pScene = pSceneNext;	// 次シーンへ
		}
	}

	float fHeight = -500.0f;		// 地面高さ

	ColisionChack(&fHeight);		// 判定チェック処理
	m_shadowHeight = fHeight;		// 影高さ設定

	// 地面調整
	{
		if (m_pos.y < fHeight)
		{// 指定高さ未満
			m_pos.y = fHeight;		// 高さ設定
			m_move.y = 0.0f;		// 移動量（Y）を初期化

			if (m_nMoveRecast == 0)
				m_bJump = false;		// ジャンプしない
			else		
				m_nMoveRecast--;
		}
	}

	// 角度調整
	{
		// 目標角度調整
		if (m_frotDest < -D3DX_PI)
			m_frotDest += D3DX_PI * 2.0f;
		if (D3DX_PI < m_frotDest)
			m_frotDest -= D3DX_PI * 2.0f;

		// 目標角度までの差分を求める
		float rotDiff = (m_frotDest - m_rot.y);

		// 差分調整
		if (rotDiff < -D3DX_PI)
			rotDiff += D3DX_PI * 2.0f;
		if (D3DX_PI < rotDiff)
			rotDiff -= D3DX_PI * 2.0f;

		float fRatio = 0.0f;		// 割合
		fRatio = 5.0;

		// 角度設定
		m_rot.y += ((rotDiff) / fRatio);

		// 角度調整
		if (m_rot.y < -D3DX_PI)
			m_rot.y += D3DX_PI * 2.0f;
		if (D3DX_PI < m_rot.y)
			m_rot.y -= D3DX_PI * 2.0f;
	}

	// 移動量調整
	{
		m_move.x -= (m_move.x - 0.0f) * MC_MOVE_DOWN;		// 移動量（X）を減速
		m_move.z -= (m_move.z - 0.0f) * MC_MOVE_DOWN;		// 移動量（Z）を減速

		// 重力加算
		m_move.y -= MC_GRAVITY;
	}

	// マップアイコン
	if (m_MapIcon != NULL)
	{// NULL以外の場合
		m_MapIcon->Setpos(m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f));
		m_MapIcon->Setrot(m_rot);
	}

	// モーション更新
	{
		if (m_pMotion != NULL)	// NULL以外の場合
			m_pMotion->Update(m_MaxModel, &m_MotionInfo[0], &m_pModel[0]);
	}
}

//=============================================================================
// 描画処理											(public)	*** CSlime ***
//=============================================================================
void CSlime::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
	{// モデル数カウント
		if (m_pModel != NULL)
		{// NULL以外の場合
			m_pModel[nCntModel].Draw();

			// 影表示
			CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(m_pos.x, m_shadowHeight + 0.1f, m_pos.z));
		}
	}
}

//=============================================================================
// 衝突判定処理											(public)	*** CSlime ***
//=============================================================================
bool CSlime::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet)
{
	CCameraMain	*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);	// カメラ取得
	bool		bLand = false;	// 当たったか

	// 距離計算
	float fLenght = sqrtf(powf(m_vtxMax.x, 2.0f) + powf(m_vtxMax.z, 2.0f));

	if (sqrtf(powf(m_pos.x - pPos->x, 2.0f) + powf(m_pos.z - pPos->z, 2.0f)) < fLenght)
	{// 一定距離以下だったら
		if (pCamera->GetLockOn() == true || pCamera->GetLockOn() == false && (m_pos.y + m_vtxMin.y - 5.0f < pPos->y && pPos->y < m_pos.y + m_vtxMax.y) || m_bAttack == true || bSet == true)
		{// 高さ判定
			// 回転計算
			float frot = atan2f(m_pos.x - pPos->x, m_pos.z - pPos->z) + D3DX_PI;

			if (pMove != NULL)
				*pMove = D3DXVECTOR3(sinf(frot) * 8.0f, pMove->y, cosf(frot) * 8.0f);	// 押し返す

			if (sqrtf(powf(m_pos.x - pPos->x, 2.0f) + powf(m_pos.z - pPos->z, 2.0f)) < (fLenght * 0.5f))
			{// 一定距離以下だったら
				if (pPos != NULL)
					*pPos = D3DXVECTOR3(m_pos.x + sinf(frot) * (fLenght * 0.5f), pPos->y, m_pos.z + cosf(frot) * (fLenght * 0.5f));		// 止める
			}

			bLand = true;	// 当たった
		}
	}

	return bLand;	// 値を返す
}

//=============================================================================
// 判定チェック処理									(public)	*** CSlime ***
//=============================================================================
void CSlime::ColisionChack(float *fHeight)
{
	CScene *pScene = NULL;		// シーン
	CScene *pSceneNext = NULL;		// 次シーン

	// フィールド
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// シーン取得

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();			// 次シーンを記憶

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// オブジェクトタイプがメッシュフィールドの場合
				CSceneMesh *pMesh = (CSceneMesh*)pScene;	// シーンメッシュの型にキャスト

				if (pMesh->ColInMesh(m_pos) == true)
				{// 範囲チェック
					*fHeight = pMesh->GetHeight(m_pos);		// 高さ取得

					break;
				}
			}
			pScene = pSceneNext;	// 次シーンへ
		}
	}

	CManager::MODE mode = CManager::GetMode();

	// マップ
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

					if (pMap_02->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f) == true)
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
				else if (nMapNum == 3)
				{
					CMap_03 *pMap_03 = (CMap_03*)pMap;

					if (pMap_03->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f))
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
				else if (nMapNum == 4)
				{
					CMap_04 *pMap_04 = (CMap_04*)pMap;

					if (pMap_04->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f))
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
			}
		}
	}

	CGame *pGame = CManager::GetGame();

	if (mode == CManager::MODE_GAME && pGame != NULL)
	{
		CPlayer *pPlayer = pGame->GetPlayer();
		if (pPlayer != NULL)
		{
			if (m_bAttack == true)
			{
				// 位置取得
				D3DXVECTOR3 pos = pPlayer->Getpos();
				if (pos.y < m_vtxMax.y * 1.2f)
				{
					// 距離計算
					float fLenght = sqrtf(powf(m_vtxMax.x, 2.0f) + powf(m_vtxMax.z, 2.0f));

					if (sqrtf(powf(m_pos.x - pos.x, 2.0f) + powf(m_pos.z - pos.z, 2.0f)) < fLenght)
					{// 一定距離以下だったら
						pPlayer->AddHP(-10);
					}
				}
			}
		}
	}
}

//=============================================================================
// 状態処理											(public)	*** CSlime ***
//=============================================================================
void CSlime::State(void)
{
	float col_a = 1.0f;	// 色設定用

	// 状態処理	
	{
		switch (m_state)
		{// 状態
		case STATE_NORMAL:		// 通常
			break;
		case STATE_DAMAGE:		// ダメージ
			m_nCntState++;		// 状態カウント

			// 点滅
			if (m_nCntState / 5 % 2 != 1)
				col_a = 0.0f;	// 透明
			else
				col_a = 1.0f;	// 不透明

			if (m_nCntState == 15)
			{// 一定のカウントになったら
				m_nCntState = 0;									// 状態カウント初期化
				m_state		= STATE_NORMAL;							// 状態を通常へ
				col_a		= 1.0f;									// 色を初期化
			}

			for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
				m_pModel[nCntModel].Setcol_a(col_a);				// 色設定

			break;
		case STATE_DEATH:		// しんだ
			break;
		}
	}

	// カメラ取得
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 Camerapos = pCamera->Getpos(0);

	// 距離計算
	float fLenght = sqrtf(powf(m_pos.x - Camerapos.x, 2.0f) + powf(m_pos.z - Camerapos.z, 2.0f));

	// 距離によって透過する
	if (fLenght < MC_DRAW_RANGE)
		for (int nCount = 0; nCount < MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(fLenght / MC_DRAW_RANGE);
	else
		for (int nCount = 0; nCount< MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(col_a);
}

//=============================================================================
// 移動処理											(public)	*** CSlime ***
//=============================================================================
void CSlime::Move(void)
{

}

//=============================================================================
// Ai処理											(public)	*** CSlime ***
//=============================================================================
void CSlime::Ai(void)
{
	CSound *pSound = CManager::GetSound();		// サウンド取得

	CManager::MODE mode = CManager::GetMode();	// モード取得
	CGame *pGame = CManager::GetGame();			// ゲーム取得

	if (mode == CManager::MODE_GAME && pGame != NULL)
	{// モードがゲームだったら + NULL以外の場合
		CPlayer *pPlayer = pGame->GetPlayer();			// プレイヤー取得
		if (pPlayer != NULL)
		{// NULL以外の場合
			D3DXVECTOR3 pos		= pPlayer->Getpos();		// 位置取得
			D3DXVECTOR3 size	= pPlayer->GetvtxMax();		// 大きさ取得

			// 距離と角度を計算
			float fRange = sqrtf(powf(m_pos.x - pos.x, 2.0f) + powf(m_pos.z - pos.z, 2.0f));
			float fAngle = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);
		
			// AI
			if (m_AiType == AI_NORMAL)
			{// 通常
				bool bAttack = false;	// 攻撃

				if (m_bAttack == false)
				{// 攻撃していない
					if (fRange < 60.0f)
					{// 範囲内だったら（攻撃）
						m_frotDest = fAngle;
						m_move.x = 0.0f;
						m_move.z = 0.0f;

						// 攻撃
						bAttack = true;
						m_bLockOn = false;

						if (m_bAutoMove == true)
						{// 自動移動中だったら
							// 自動移動をキャンセル
							m_bAutoMove			= false;
							m_nCntAutoMove		= 0;
							m_nMaxAutoMove		= 0;
							m_nAutoMoveRecast	= 0;
						}
					}
					else if (fRange < 400.0f || m_bLockOn == true)
					{// 範囲内だったら（攻撃準備）、ロックオンしている
						m_frotDest = fAngle;

						if (m_nMoveRecast == 20)
						{// 移動を始める
							m_move.x = sinf(fAngle) * MC_MOVE_UP;
							m_move.z = cosf(fAngle) * MC_MOVE_UP;
						}

						if (m_bJump == false)
						{// ジャンプしていない
							// ジャンプして移動開始
							m_move.y += MC_JUMP;
							m_bJump			= true;
							m_nMoveRecast	= 20;
						}

						if (m_bAutoMove == true)
						{// 自動移動中だったら
							// 自動移動をキャンセル
							m_bAutoMove			= false;
							m_nCntAutoMove		= 0;
							m_nMaxAutoMove		= 0;
							m_nAutoMoveRecast	= 0;
						}
					}
					else
					{// その他（範囲外）
						if (m_nAutoMoveRecast == 0)
						{// 自動移動リキャストが０だったら
							if (m_bAutoMove == false)
							{// 自動移動していない
								// 自動移動する
								m_bAutoMove = true;
								m_nCntAutoMove = 0;
								m_nMaxAutoMove = rand() % 120 + 90;

								m_frotDest = (float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f;
							}
							else
							{// 自動移動している
								if (m_nMoveRecast == 20)
								{// 移動を始める
									m_move.x = sinf(m_frotDest) * MC_MOVE_UP;
									m_move.z = cosf(m_frotDest) * MC_MOVE_UP;
								}

								if (m_bJump == false)
								{// ジャンプしていない
									// ジャンプして移動開始
									m_move.y += MC_JUMP;
									m_bJump = true;
									m_nMoveRecast = 20;
								}

								m_nCntAutoMove++;		// 自動移動カウント

								if (m_nCntAutoMove == m_nMaxAutoMove)
								{// カウントが最大と同じになったら
									// 自動移動をキャンセル
									m_bAutoMove = false;
									m_nCntAutoMove = 0;
									m_nMaxAutoMove = 0;
									m_nAutoMoveRecast = 90;
								}
							}
						}
						else
						{// 自動移動リキャストが０以外
							if (0 < m_nAutoMoveRecast)	// ０以上だったら
								m_nAutoMoveRecast--;	// 減らす
						}
					}
				}
				else
				{// 攻撃する
					if (fRange < 80.0f)
					{// 範囲内だったら
						bAttack = true;		// 攻撃する
					}
					else
					{// 範囲外だったら
						m_bAttack = false;	// 攻撃しない
					}
				}

				if (bAttack == true)
				{// 攻撃する
					if (m_bJump == false)
					{// ジャンプしていない
						if (00 < m_nCntAttack && m_nCntAttack < 38)
						{// 一定フレームの間だったら
							m_bAttack = true;		// 攻撃する
						}
						else
						{// その他
							m_bAttack = false;		// 攻撃しない
						}

						if (0 < m_nCntAttack && m_nCntAttack < 30)
						{// 一定フレームの間だったら
							m_move.x = sinf(fAngle) * -0.2f;
							m_move.z = cosf(fAngle) * -0.2f;
						}
						else if (30 < m_nCntAttack && m_nCntAttack < 35)
						{// 一定フレームの間だったら
							m_move.x = sinf(fAngle) * (fRange / 3);
							m_move.z = cosf(fAngle) * (fRange / 3);

							if (m_nCntAttack == 31)
							{// 一定フレームだったら
								m_move.y += MC_JUMP;

								if (pSound != NULL)
								{// NULL以外の場合
									pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMY_ATTACK000);
								}
							}

							CEffect::Create(CEffect::TYPE_C00, CEffect::SHOT_NONE, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
						else if (35 < m_nCntAttack && m_nCntAttack < 38)
						{// 一定フレームの間だったら
							m_move.x = sinf(fAngle) * -(fRange / 3);
							m_move.z = cosf(fAngle) * -(fRange / 3);
						}
						m_nCntAttack++;

						int nNum = rand() % 120 + 150;
						if (nNum < m_nCntAttack)
							m_nCntAttack = 0;
					}
				}
				else
				{// 攻撃していない
					m_nCntAttack = 0;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CSlime_King::m_MaxModel		= 0;		// モデル数
int						CSlime_King::m_MaxMotion	= 0;		// モーション数

LPDIRECT3DTEXTURE9		**CSlime_King::m_pTexture	= NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CSlime_King::m_pMesh		= NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER			*CSlime_King::m_pBuffMat	= NULL;		// マテリアル情報へのポインタ
DWORD					*CSlime_King::m_nNumMat		= NULL;		// マテリアル情報の数

CModel::MODEL_INFO		*CSlime_King::m_ModelInfo	= NULL;		// モデルインフォ
CMotion::MOTION_INFO	*CSlime_King::m_MotionInfo	= NULL;		// モーションインフォ

//=============================================================================
// コンストラクタ										(public)	*** CSlime_King ***
//=============================================================================
CSlime_King::CSlime_King() : CEnemy()
{
	m_pModel = NULL;									// モデル
	m_pMotion = NULL;									// モーション

	m_AiType = AI_NORMAL;								// AIタイプ
	m_nMoveRecast = 0;									// 移動リキャスト
	m_bAttack = false;									// 攻撃している
	m_nCntAttack = 0;									// 攻撃カウント

	m_bAutoMove = false;								// 自動移動しているか
	m_nCntAutoMove = 0;									// 自動移動カウント
	m_nMaxAutoMove = 0;									// 自動移動カウント最大
	m_nAutoMoveRecast = 0;								// 自動移動リキャスト
}

//=============================================================================
// デストラクタ										(public)	*** CSlime_King ***
//=============================================================================
CSlime_King::~CSlime_King()
{

}

//=============================================================================
// ロード処理										(public)	*** CSlime_King ***
//=============================================================================
HRESULT CSlime_King::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	m_ModelInfo = CText::GetModel("data\\TEXT\\MODEL\\slimekingmodel.txt", &m_MaxModel);		// モデル取得
	//m_MotionInfo	= CText::GetMotion(0);					// モーション取得 

	// 各情報のメモリ確保
	if (m_pTexture == NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_MaxModel];
	if (m_pMesh == NULL) m_pMesh = new LPD3DXMESH[m_MaxModel];
	if (m_pBuffMat == NULL) m_pBuffMat = new LPD3DXBUFFER[m_MaxModel];
	if (m_nNumMat == NULL) m_nNumMat = new DWORD[m_MaxModel];

	// 各情報の初期化
	for (int nCount = 0; nCount < m_MaxModel; nCount++)		// モデル数カウント
	{
		m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;
	}

	for (int nCntParnet = 0; nCntParnet < m_MaxModel; nCntParnet++)
	{// モデル数カウント
	 // Xファイルの読み込み
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Unload(void)
{
	// 各情報のメモリ開放、NULLへ
	if (m_pTexture != NULL) { delete[] m_pTexture;	m_pTexture = NULL; }
	if (m_pMesh != NULL) { delete[] m_pMesh;	m_pMesh = NULL; }
	if (m_pBuffMat != NULL) { delete[] m_pBuffMat;	m_pBuffMat = NULL; }
	if (m_nNumMat != NULL) { delete[] m_nNumMat;	m_nNumMat = NULL; }

	// モデルインフォ
	if (m_ModelInfo != NULL)	m_ModelInfo = NULL;		// NULLへ

	// モーションインフォ
	if (m_MotionInfo != NULL)	m_MotionInfo = NULL;	// NULLへ
}

//=============================================================================
// 初期化処理										(public)	*** CSlime_King ***
//=============================================================================
HRESULT CSlime_King::Init(void)
{
	CEnemy::Init();		// 初期化処理

	// モデル
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULLの場合
		m_pModel = new CModel[m_MaxModel];	// メモリ確保

		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// モデル数カウント
		 // 初期値設定
			m_pModel[nCntModel].SetInitAll(m_pTexture[nCntModel], m_pMesh[nCntModel], m_pBuffMat[nCntModel], m_nNumMat[nCntModel], m_ModelInfo[nCntModel].pos, m_ModelInfo[nCntModel].rot);
			// 初期化処理
			m_pModel[nCntModel].Init();

			// 親モデルを設定
			if (0 <= m_ModelInfo[nCntModel].nIndex)		// 親以外
				m_pModel[nCntModel].SetParent(&m_pModel[m_ModelInfo[nCntModel].nIndex]);
		}
	}

	// モーション
	if (m_pMotion == NULL && m_MotionInfo != NULL)
	{// NULLの場合
		m_pMotion = new CMotion;	// メモリ確保

		m_pMotion->SetMotionNum(0);	// 待機モーション
	}

	m_nHP = m_MAX_HP;	// 体力設定
	m_nMAXHP = m_MAX_HP;	// 体力設定

	// 判定設定
	m_vtxMax = D3DXVECTOR3(30.0f, 40.0f, 30.0f);
	m_vtxMin = D3DXVECTOR3(-30.0f, 0.0f, -30.0f);

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Uninit(void)
{
	CEffect::Create(CEffect::TYPE_CREATE, CEffect::SHOT_NONE, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	CCameraMain *pCameraMain = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	if (pCameraMain->GetLockOn() == true)
	{// ロックオンしている
		if (pCameraMain->GetLockOnEnemy() == this)
		{// 同じだったら
			pCameraMain->SetLockOn(false);		// ロックオンしない
		}
	}

	CScene *pScene		= NULL;		// シーン
	CScene *pSceneNext	= NULL;		// 次シーン

	// 敵
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// オブジェクトタイプが敵の場合
				CEnemy *pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
				if (pEnemy != this)
				{
					pEnemy->Uninit();
				}
			}
			pScene = pSceneNext;	// 次シーンへ
		}
	}

	// モデル
	if (m_pModel != NULL)
	{// NULL以外の場合
		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++) // モデル数カウント
			m_pModel[nCntModel].Uninit();	// 終了処理

		delete[] m_pModel;		// メモリ開放
		m_pModel = NULL;		// NULLへ
	}

	// モーション
	if (m_pMotion != NULL)
	{// NULL以外の場合
		m_pMotion->Uninit();	// 終了処理
		delete m_pMotion;		// メモリ開放
		m_pMotion = NULL;		// NULLへ
	}

	CEnemy::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Update(void)
{
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);

	State();	// 状態処理

	m_posOld = m_pos;				// 位置保存

	if (pCamera->GetPickUp() == false)
		Ai();							// Ai

	m_pos += m_move;				// 移動量加算

	CScene *pScene		= NULL;		// シーン
	CScene *pSceneNext	= NULL;		// 次シーン

	// 敵
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// オブジェクトタイプが敵の場合
				CEnemy *pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
				if (pEnemy != this)
				{
					pEnemy->Collision(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin, true);
				}
			}
			pScene = pSceneNext;	// 次シーンへ
		}
	}

	float fHeight = -500.0f;		// 地面高さ

	ColisionChack(&fHeight);		// 判定チェック処理
	m_shadowHeight = fHeight;		// 影高さ設定

	// 地面調整
	{
		if (m_pos.y < fHeight)
		{// 指定高さ未満
			m_pos.y = fHeight;		// 高さ設定
			m_move.y = 0.0f;		// 移動量（Y）を初期化

			if (m_nMoveRecast == 0)
				m_bJump = false;		// ジャンプしない
			else
				m_nMoveRecast--;
		}
	}

	// 角度調整
	{
		// 目標角度調整
		if (m_frotDest < -D3DX_PI)
			m_frotDest += D3DX_PI * 2.0f;
		if (D3DX_PI < m_frotDest)
			m_frotDest -= D3DX_PI * 2.0f;

		// 目標角度までの差分を求める
		float rotDiff = (m_frotDest - m_rot.y);

		// 差分調整
		if (rotDiff < -D3DX_PI)
			rotDiff += D3DX_PI * 2.0f;
		if (D3DX_PI < rotDiff)
			rotDiff -= D3DX_PI * 2.0f;

		float fRatio = 0.0f;		// 割合
		fRatio = 5.0;

		// 角度設定
		m_rot.y += ((rotDiff) / fRatio);

		// 角度調整
		if (m_rot.y < -D3DX_PI)
			m_rot.y += D3DX_PI * 2.0f;
		if (D3DX_PI < m_rot.y)
			m_rot.y -= D3DX_PI * 2.0f;
	}

	// 移動量調整
	{
		m_move.x -= (m_move.x - 0.0f) * MC_MOVE_DOWN;		// 移動量（X）を減速
		m_move.z -= (m_move.z - 0.0f) * MC_MOVE_DOWN;		// 移動量（Z）を減速

		// 重力加算
		m_move.y -= MC_GRAVITY;
	}

	// マップアイコン
	if (m_MapIcon != NULL)
	{// NULL以外の場合
		m_MapIcon->Setpos(m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f));
		m_MapIcon->Setrot(m_rot);
	}

	// モーション更新
	{
		if (m_pMotion != NULL)	// NULL以外の場合
			m_pMotion->Update(m_MaxModel, &m_MotionInfo[0], &m_pModel[0]);
	}
}

//=============================================================================
// 描画処理											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
	{// モデル数カウント
		if (m_pModel != NULL)
		{// NULL以外の場合
			m_pModel[nCntModel].Draw();

			// 影表示
			CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(m_pos.x, m_shadowHeight + 0.1f, m_pos.z));
		}
	}
}

//=============================================================================
// 衝突判定処理											(public)	*** CSlime_King ***
//=============================================================================
bool CSlime_King::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet)
{
	CCameraMain	*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);	// カメラ取得
	bool		bLand = false;	// 当たったか

	// 距離計算
	float fLenght = sqrtf(powf(m_vtxMax.x, 2.0f) + powf(m_vtxMax.z, 2.0f)) + sqrtf(powf(sizeMax.x, 2.0f) + powf(sizeMax.z, 2.0f));

	if (sqrtf(powf(m_pos.x - pPos->x, 2.0f) + powf(m_pos.z - pPos->z, 2.0f)) < fLenght)
	{// 一定距離以下だったら
		if (pCamera->GetLockOn() == true || pCamera->GetLockOn() == false && (m_pos.y + m_vtxMin.y - 5.0f < pPos->y && pPos->y < m_pos.y + m_vtxMax.y) || m_bAttack == true || bSet == true)
		{// 高さ判定
			// 回転計算
			float frot = atan2f(m_pos.x - pPos->x, m_pos.z - pPos->z) + D3DX_PI;

			if (pMove != NULL)
				*pMove = D3DXVECTOR3(sinf(frot) * 8.0f, pMove->y, cosf(frot) * 8.0f);	// 押し返す

			if (sqrtf(powf(m_pos.x - pPos->x, 2.0f) + powf(m_pos.z - pPos->z, 2.0f)) < (fLenght * 0.5f))
			{// 一定距離以下だったら
				if (pPos != NULL)
					*pPos = D3DXVECTOR3(m_pos.x + sinf(frot) * (fLenght * 0.5f), pPos->y, m_pos.z + cosf(frot) * (fLenght * 0.5f));		// 止める
			}

			bLand = true;	// 当たった
		}
	}

	return bLand;	// 値を返す
}

//=============================================================================
// 判定チェック処理									(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::ColisionChack(float *fHeight)
{
	CScene *pScene = NULL;		// シーン
	CScene *pSceneNext = NULL;		// 次シーン

	// フィールド
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// シーン取得

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();			// 次シーンを記憶

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// オブジェクトタイプがメッシュフィールドの場合
				CSceneMesh *pMesh = (CSceneMesh*)pScene;	// シーンメッシュの型にキャスト

				if (pMesh->ColInMesh(m_pos) == true)
				{// 範囲チェック
					*fHeight = pMesh->GetHeight(m_pos);		// 高さ取得

					break;
				}
			}
			pScene = pSceneNext;	// 次シーンへ
		}
	}

	CManager::MODE mode = CManager::GetMode();

	// マップ
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

					if (pMap_02->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f) == true)
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
				else if (nMapNum == 3)
				{
					CMap_03 *pMap_03 = (CMap_03*)pMap;

					if (pMap_03->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f))
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
				else if (nMapNum == 4)
				{
					CMap_04 *pMap_04 = (CMap_04*)pMap;

					if (pMap_04->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f))
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
			}
		}
	}

	CGame *pGame = CManager::GetGame();

	if (mode == CManager::MODE_GAME && pGame != NULL)
	{
		CPlayer *pPlayer = pGame->GetPlayer();
		if (pPlayer != NULL)
		{
			if (m_bAttack == true)
			{
				// 位置取得
				D3DXVECTOR3 pos = pPlayer->Getpos();
				if (pos.y < m_vtxMax.y * 1.2f)
				{
					// 距離計算
					float fLenght = sqrtf(powf(m_vtxMax.x, 2.0f) + powf(m_vtxMax.z, 2.0f));

					if (sqrtf(powf(m_pos.x - pos.x, 2.0f) + powf(m_pos.z - pos.z, 2.0f)) < fLenght)
					{// 一定距離以下だったら
						pPlayer->AddHP(-15);
					}
				}
			}
		}
	}
}

//=============================================================================
// 状態処理											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::State(void)
{
	float col_a = 1.0f;	// 色設定用

	// 状態処理	
	{
		switch (m_state)
		{// 状態
		case STATE_NORMAL:		// 通常
			break;
		case STATE_DAMAGE:		// ダメージ
			m_nCntState++;		// 状態カウント

			// 点滅
			if (m_nCntState / 5 % 2 != 1)
				col_a = 0.0f;	// 透明
			else
				col_a = 1.0f;	// 不透明

			if (m_nCntState == 15)
			{// 一定のカウントになったら
				m_nCntState = 0;									// 状態カウント初期化
				m_state		= STATE_NORMAL;							// 状態を通常へ
				col_a		= 1.0f;									// 色を初期化
			}

			for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
				m_pModel[nCntModel].Setcol_a(col_a);				// 色設定

			break;
		case STATE_DEATH:		// しんだ
			break;
		}
	}

	// カメラ取得
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 Camerapos = pCamera->Getpos(0);

	// 距離計算
	float fLenght = sqrtf(powf(m_pos.x - Camerapos.x, 2.0f) + powf(m_pos.z - Camerapos.z, 2.0f));

	// 距離によって透過する
	if (fLenght < MC_DRAW_RANGE)
		for (int nCount = 0; nCount < MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(fLenght / MC_DRAW_RANGE);
	else
		for (int nCount = 0; nCount< MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(col_a);
}

//=============================================================================
// 移動処理											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Move(void)
{

}

//=============================================================================
// Ai処理											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Ai(void)
{
	CSound *pSound = CManager::GetSound();			// サウンド取得

	CManager::MODE mode = CManager::GetMode();		// モード取得
	CGame *pGame = CManager::GetGame();				// ゲーム取得

	if (mode == CManager::MODE_GAME && pGame != NULL)
	{// モードがゲームだったら + NULL以外の場合
		CPlayer *pPlayer = pGame->GetPlayer();			// プレイヤー取得
		if (pPlayer != NULL)
		{// NULL以外の場合
			D3DXVECTOR3 pos = pPlayer->Getpos();			// 位置取得
			D3DXVECTOR3 size = pPlayer->GetvtxMax();		// 大きさ取得

			// 距離と角度を計算
			float fRange = sqrtf(powf(m_pos.x - pos.x, 2.0f) + powf(m_pos.z - pos.z, 2.0f));
			float fAngle = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);

			// AI
			if (m_AiType == AI_NORMAL)
			{// 通常
				bool bAttack = false;	// 攻撃

				if (m_bAttack == false)
				{// 攻撃していない
					if (fRange < 200.0f)
					{// 範囲内だったら（攻撃）
						m_frotDest = fAngle;
						m_move.x = 0.0f;
						m_move.z = 0.0f;

						// 攻撃
						bAttack = true;

						if (m_bAutoMove == true)
						{// 自動移動中だったら
							// 自動移動をキャンセル
							m_bAutoMove = false;
							m_nCntAutoMove = 0;
							m_nMaxAutoMove = 0;
							m_nAutoMoveRecast = 0;
						}
					}
					else if (fRange < 400.0f)
					{// 範囲内だったら（攻撃準備）
						m_frotDest = fAngle;

						if (m_nMoveRecast == 20)
						{// 移動を始める
							m_move.x = sinf(fAngle) * (MC_MOVE_UP * 1.7f);
							m_move.z = cosf(fAngle) * (MC_MOVE_UP * 1.7f);
						}

						if (m_bJump == false)
						{// ジャンプしていない
							// ジャンプして移動開始
							m_move.y += MC_JUMP;
							m_bJump = true;
							m_nMoveRecast = 20;
						}

						if (m_bAutoMove == true)
						{// 自動移動中だったら
							// 自動移動をキャンセル
							m_bAutoMove = false;
							m_nCntAutoMove = 0;
							m_nMaxAutoMove = 0;
							m_nAutoMoveRecast = 0;
						}

						if (300.0f < fRange)
						{// 範囲内だったら
							int nAi = rand() % 10;
							if (nAi == 1)
							{// １だったら
								m_AiType = AI_MAGIC;	// 魔法
							}
							else if (nAi == 2)
							{// ２だったら
								m_AiType = AI_DASH;		// ダッシュ
							}
						}
					}
					else if (fRange < 600.0f)
					{// 範囲内だったら（魔法）
						if (m_nMoveRecast == 20)
						{// 移動を始める
							m_move.x = sinf(fAngle) * (MC_MOVE_UP * 1.8f);
							m_move.z = cosf(fAngle) * (MC_MOVE_UP * 1.8f);
						}

						if (m_bJump == false)
						{// ジャンプしていない
							m_AiType = AI_MAGIC;	// 魔法
						}
					}
					else
					{// 範囲外
						if (m_bJump == false)
						{// ジャンプしていない
							m_AiType = AI_DASH;		// ダッシュ
						}
					}
				}
				else
				{// 攻撃する
					if (fRange < 250.0f)
					{// 範囲内
						bAttack = true;			// 攻撃する
					}
					else
					{
						m_bAttack = false;		// 攻撃しない
					}
				}

				if (bAttack == true)
				{// 攻撃する
					m_frotDest = fAngle;

					if (m_bJump == false)
					{// ジャンプしていない
						if (0 < m_nCntAttack && m_nCntAttack < 38)
						{// 一定フレームの間だったら
							m_bAttack = true;							
						}
						else
						{// その他
							m_bAttack = false;
						}

						if (0 < m_nCntAttack && m_nCntAttack < 30)
						{// 一定フレームの間だったら
							m_move.x = sinf(fAngle) * -0.2f;
							m_move.z = cosf(fAngle) * -0.2f;
						}
						else if (30 < m_nCntAttack && m_nCntAttack < 35)
						{// 一定フレームの間だったら
							m_move.x = sinf(fAngle) * (fRange / 3);
							m_move.z = cosf(fAngle) * (fRange / 3);

							if (m_nCntAttack == 31)
							{// 一定フレームだったら
								m_move.y += MC_JUMP;

								if (pSound != NULL)
								{// NULL以外の場合
									pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMY_ATTACK001);
								}
							}

							CEffect::Create(CEffect::TYPE_C00, CEffect::SHOT_NONE, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
						else if (35 < m_nCntAttack && m_nCntAttack < 38)
						{// 一定フレームの間だったら
							m_move.x = sinf(fAngle) * -(fRange / 3);
							m_move.z = cosf(fAngle) * -(fRange / 3);
						}
						m_nCntAttack++;

						int nNum = rand() % 120 + 150;
						if (nNum < m_nCntAttack)
						{
							m_nCntAttack = 0;

							int nAi = rand() % 3;
							if (nAi == 0)
							{
								m_AiType = AI_CREATE;
							}					
						}
					}
				}
				else
				{// 攻撃していない
					m_nCntAttack = 0;
				}
			}
			else if (m_AiType == AI_MAGIC)
			{// 魔法
				bool bAttack = false;

				if (fRange < 600.0f)
				{// 範囲内
					m_frotDest = fAngle;
					m_move.x = 0.0f;
					m_move.z = 0.0f;

					bAttack = true;

					if (m_bAutoMove == true)
					{// 自動移動中だったら
						// 自動移動をキャンセル
						m_bAutoMove = false;
						m_nCntAutoMove = 0;
						m_nMaxAutoMove = 0;
						m_nAutoMoveRecast = 0;
					}
				}
				else
				{// 範囲外
					m_frotDest = fAngle;

					if (m_bJump == false)
					{
						m_AiType = AI_DASH;
					}
				}

				if (bAttack == true)
				{// 攻撃する
					m_frotDest = fAngle;

					if (m_nCntAttack % 80 == 0)
					{// 一定フレームだったら
						CTexAnim *pTexAnim = NULL;
						pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(sinf(m_rot.y + (D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + (D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR2(0.0714f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
						pTexAnim->SetAnim(14, 14, 3, 0, false, true);

						CEffect::Create(CEffect::TYPE_C01, CEffect::SHOT_ENEMY, m_pos + D3DXVECTOR3(sinf(m_rot.y + (D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + (D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(sinf(m_rot.y) * 8.0f, -2.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(9.0f, 9.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
					else if (m_nCntAttack % 80 == 40)
					{// 一定フレームだったら
						CTexAnim *pTexAnim = NULL;
						pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(sinf(m_rot.y + -(D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + -(D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR2(0.0714f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
						pTexAnim->SetAnim(14, 14, 3, 0, false, true);

						CEffect::Create(CEffect::TYPE_C01, CEffect::SHOT_ENEMY, m_pos + D3DXVECTOR3(sinf(m_rot.y + -(D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + -(D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(sinf(m_rot.y) * 8.0f, -2.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(9.0f, 9.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

					m_nCntAttack++;

					if (m_nCntAttack == 120)
					{// 一定フレームだったら
						m_nCntAttack = 0;
						m_AiType = AI_NORMAL;

						if (m_bJump == false)
						{// 自動移動中だったら
							// 自動移動をキャンセル
							m_move.y += MC_JUMP;
							m_bJump = true;
							m_nMoveRecast = 20;
						}

						if (420.0f < fRange)
						{// 範囲内
							m_AiType = AI_DASH;		// ダッシュ
						}

						int nAi = rand() % 3;
						if (nAi == 0)
						{// ０だったら
							m_AiType = AI_CREATE;	// クリエイト
						}
					}
				}
				else
				{// 攻撃していない
					m_nCntAttack = 0;
				}
			}
			else if (m_AiType == AI_DASH)
			{// ダッシュ
				m_frotDest = fAngle;

				CEffect::Create(CEffect::TYPE_C00, CEffect::SHOT_NONE, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				if (m_nMoveRecast == 20)
				{// 移動を始める
					m_move.x = sinf(fAngle) * (MC_MOVE_UP * 7.0f);
					m_move.z = cosf(fAngle) * (MC_MOVE_UP * 7.0f);
				}

				if (m_bJump == false)
				{// ジャンプしていない
					if (fRange < 300.0f)
					{// 範囲内（通常）
						m_AiType = AI_NORMAL;
					}
					else if (fRange < 350.0f)
					{// 範囲内（クリエイト）
						m_AiType = AI_CREATE;
					}
					else if (fRange < 400.0f)
					{// 範囲内（ストップ）
						m_AiType = AI_STOP;
					}
					else
					{// 範囲外
						m_move.y += (MC_JUMP * 1.5f);
						m_bJump = true;
						m_nMoveRecast = 20;
					}
				}
			}
			else if (m_AiType == AI_CREATE)
			{// クリエイト
				if (pGame->GetEnemyNum() < 10)
				{// １０体以下だったら
					CTexAnim *pTexAnim = NULL;
					// 魔法陣
					pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D, m_pos + D3DXVECTOR3(sinf(m_rot.y + (D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + (D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 50.0f), D3DXVECTOR2(0.0714f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
					pTexAnim->SetAnim(14, 14, 3, 0, false, true);
					// 魔法陣
					pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D, m_pos + D3DXVECTOR3(sinf(m_rot.y + -(D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + -(D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 50.0f), D3DXVECTOR2(0.0714f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
					pTexAnim->SetAnim(14, 14, 3, 0, false, true);

					// 敵生成
					CEnemy::Create(CEnemy::TYPE_N_SLIME, m_pos + D3DXVECTOR3(sinf(m_rot.y + (D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + (D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
					CEnemy::Create(CEnemy::TYPE_N_SLIME, m_pos + D3DXVECTOR3(sinf(m_rot.y + -(D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + -(D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
				}

				m_AiType = AI_STOP;		// 停止
			}
			else if (m_AiType == AI_STOP)
			{// 停止
				m_frotDest = fAngle;
				m_nCntAttack++;

				if (m_nCntAttack == 90)
				{// 一定フレームになったら
					m_nCntAttack = 0;
					m_AiType = AI_NORMAL;	// 通常
				}
			}
		}
	}
}