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
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "text.h"			// テキスト
#include "camera.h"			// カメラ
#include "fade.h"			// フェード
#include "texture.h"		// テクスチャ

#include "scenemesh.h"		// シーンメッシュ
#include "scenemeshorbit.h"	// オービット
#include "texanim.h"		// テクスチャアニメーション
#include "title.h"			// タイトル
#include "game.h"			// ゲーム
#include "model.h"			// モデル
#include "motion.h"			// モーション
#include "map.h"			// マップ
#include "ui.h"				// Ui
#include "mapicon.h"		// マップアイコン
#include "effect.h"			// エフェクト
#include "gauge.h"			// ゲージ
#include "shadow.h"			// 影
#include "enemy.h"			// 敵

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MC_MOVE_UP		(2.7f)								// 加速
#define MC_MOVE_DOWN	(0.75f)								// 減速
#define MC_JUMP			(9.0f)								// ジャンプ量
#define MC_GRAVITY		(0.6f)								// 重力

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CPlayer::m_MaxModel		= 0;		// モデル数
int						CPlayer::m_MaxMotion	= 0;		// モーション数

LPDIRECT3DTEXTURE9		**CPlayer::m_pTexture	= NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CPlayer::m_pMesh		= NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER			*CPlayer::m_pBuffMat	= NULL;		// マテリアル情報へのポインタ
DWORD					*CPlayer::m_nNumMat		= NULL;		// マテリアル情報の数

CModel::MODEL_INFO		*CPlayer::m_ModelInfo	= NULL;		// モデルインフォ
CMotion::MOTION_INFO	*CPlayer::m_MotionInfo	= NULL;		// モーションインフォ

//=============================================================================
// コンストラクタ									(public)	*** CPlayer ***
//=============================================================================
CPlayer::CPlayer() : CScene(CScene::PRIORITY_3, CScene::OBJTYPE_PLAYER)
{
	// 初期値設定
	m_MapIcon		= NULL;									// マップアイコン

	m_bTutorialSet	= false;								// チュートリアル設定
	m_nCntTutorialSet = 0;									// チュートリアル設定カウント

	for(int nCount = 0; nCount < ORBIT_MAX; nCount++)
		m_pOrbit[nCount];	// オービット

	m_bWarp			= false;								// ワープしているか
	m_nCntWarp		= 0;									// ワープカウント
	m_fcol_a		= 1.0f;									// 色アルファ
	m_posWarp		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ワープ位置
	m_fWarpLenght	= 0.0f;
	m_fWarpAngle	= 0.0f;

	m_bUpdata		= true;									// 更新する
	m_bDraw			= true;									// 描画する
	m_bMove			= false;								// 移動できる

	m_nHP			= 0;									// HP
	m_nMP			= 0;									// MP

	m_nHatNum		= 0;									// 帽子番号
	m_nCntTimer		= 0;									// タイマーカウント

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
	m_nAttackCnt	= 0;									// 攻撃カウント
	m_nAttackNum	= ATTACK_N_1;							// 攻撃入力数
	m_bAttack_Nor	= false;								// 通常攻撃
	m_bAttack_Sky	= false;								// 空中攻撃
	m_bAction_Sky	= false;								// 空中アクション
	m_nAttackRecast = 0;									// 攻撃リキャスト
	m_nGuardCnt		= 0;									// ガードカウント
	m_bGuard		= false;								// ガード
	m_nCntJastGuard	= 0;									// ジャストガードカウント
	m_fDamageCat	= 1.0f;									// ダメージカット
	m_nStepCnt		= 0;									// ステップカウント
	m_bStep			= false;								// ステップ
	m_bSkill		= false;								// スキル
	m_nSkillRecast	= 0;									// スキルリキャスト

	m_brot			= false;								// 回転可能か

	m_shadowHeight = 0.0f;									// 影高さ

	m_bNotAttack	= false;								// 攻撃禁止
	m_bNotSkill		= false;								// スキル禁止

	for (int nCount = 0; nCount < GUARD_DOME_MAX; nCount++)
		m_pGuardDome[nCount] = NULL;						// ガードドーム

	m_GuardDomeUV = D3DXVECTOR2(0.0f, 0.0f);				// ガードドームUV
}

//=============================================================================
// デストラクタ										(public)	*** CPlayer ***
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ロード処理										(public)	*** CPlayer ***
//=============================================================================
HRESULT CPlayer::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MODEL\\playermodel.txt", &m_MaxModel);		// モデル取得
	m_MotionInfo	= CText::GetMotion("data\\TEXT\\MOTION\\playermotion.txt");					// モーション取得

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
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CPlayer::Unload(void)
{
	// 各情報のメモリ開放、NULLへ
	if (m_pTexture	!= NULL)	{ delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL)	{ delete[] m_pMesh;		m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL)	{ delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL)	{ delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// モデルインフォ
	if (m_ModelInfo != NULL)	m_ModelInfo = NULL;		// NULLへ

	// モーションインフォ
	if (m_MotionInfo != NULL)	m_MotionInfo = NULL;	// NULLへ
}

//=============================================================================
// クリエイト										(public)	*** CPlayer ***
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *pPlayer = NULL;		// ポインタ

	if (pPlayer == NULL)			// NULLの場合
		pPlayer = new CPlayer;		// メモリ確保

	if (pPlayer != NULL)
	{// NULL以外の場合
		pPlayer->m_pos = pos;			// 位置
		pPlayer->m_rot = rot;			// 向き
		pPlayer->m_frotDest = rot.y;	// 目標向き
		pPlayer->Init();				// 初期化処理
	}

	return pPlayer;		// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CPlayer ***
//=============================================================================
HRESULT CPlayer::Init()
{
	CManager::MODE mode = CManager::GetMode();		// モード取得

	// マップアイコン
	if (m_MapIcon == NULL)
	{// NULLの場合
		if (mode == CManager::MODE_TITLE)
			m_MapIcon = CMapIcon::Create(PRIORITY_MAP_2, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), m_rot, D3DXVECTOR3(30.0f, 0.0f, 50.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		else if (mode == CManager::MODE_GAME)
			m_MapIcon = CMapIcon::Create(PRIORITY_MAP_2, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), m_rot, D3DXVECTOR3(80.0f, 0.0f, 100.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_MapIcon->SetTex(2, 2, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	}

	// モデル
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULLの場合
		m_pModel = new CModel[m_MaxModel];		// メモリ確保

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

		m_pMotion->SetMotionNumrot(MOTION_NEUTRAL, 0);	// 待機モーション
	}

	// 判定設定
	m_vtxMax = D3DXVECTOR3(16.0f, 50.0f, 16.0f);	// 最大
	m_vtxMin = D3DXVECTOR3(-16.0f, 0.0f, -16.0f);	// 最小

	// 体力設定
	m_nHP = m_MAX_HP;	// HP
	m_nMP = m_MAX_MP;	// MP

	// オービット
	{
		// 右手
		m_pOrbit[ORBIT_RIGHT] = CSceneMeshOrbit::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.2f, 0.2f, 30);
		m_pOrbit[ORBIT_RIGHT]->SetMtxParent(&m_pModel[5].GetmtxWorld());

		// 左手
		m_pOrbit[ORBIT_LEFT] = CSceneMeshOrbit::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.2f, 0.2f, 30);
		m_pOrbit[ORBIT_LEFT]->SetMtxParent(&m_pModel[11].GetmtxWorld());
	}

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Uninit(void)
{
	// マップアイコン
	if (m_MapIcon != NULL)
	{// NULL以外の場合
		m_MapIcon->Uninit();	// 破棄
		m_MapIcon = NULL;		// NULLへ
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

	// オービット
	for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
	{
		if (m_pOrbit[nCount] != NULL)
		{
			m_pOrbit[nCount]->Uninit();
			m_pOrbit[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < GUARD_DOME_MAX; nCount++)
	{
		if (m_pGuardDome[nCount] != NULL)
		{
			m_pGuardDome[nCount]->Uninit();
			m_pGuardDome[nCount] = NULL;
		}
	}

	CScene::SetDeath();		// 死亡フラグ
}

//=============================================================================
// 更新処理											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();

	CManager::MODE	mode		= CManager::GetMode();		// モード取得
	CCameraMain		*pCamera	= (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// カメラ取得

	if (m_bUpdata == true)
	{// 更新する
		if (mode == CManager::MODE_TITLE)
		{// タイトルのみ
			Title();			// タイトル処理
		}

		State();				// 状態処理

		m_posOld = m_pos;		// 位置保存

		if (m_state != STATE_DEATH)
		{
			if (m_nCntTutorialSet == 0)
			{
				Move();				// 移動処理
				Step();				// ステップ処理
				Jump();				// ジャンプ処理
				Action();			// アクション処理		
			}
		}

		m_pos += m_move;	// 移動量加算

		if (mode == CManager::MODE_TITLE)
		{// タイトルのみ
			// チュートリアルか取得
			bool bTutorial = CManager::GetTitle()->GetTutorial();

			if (m_nCntTutorialSet == 0)
			{// カウントが０だったら
				if (bTutorial == true && m_bTutorialSet == true)
				{// チュートリアル中 + チュートリアルの準備が終わった
					// 判定
					if (m_pos.x < 170.0f) { m_pos.x = 170.0f;		m_move.x = 0.0f; }
					if (690.0f < m_pos.x) { m_pos.x = 690.0f;		m_move.x = 0.0f; }
					if (m_pos.z < -160.0f) { m_pos.z = -160.0f;	m_move.z = 0.0f; }
					if (160.0f < m_pos.z) { m_pos.z = 160.0f;		m_move.z = 0.0f; }
				}
			}
		}
		else if (mode == CManager::MODE_GAME)
		{// ゲームのみ
			// 判定
			if (m_pos.x < -1470.0f) { m_pos.x = -1470.0f;	m_move.x = 0.0f; }
			if (1470.0f < m_pos.x) { m_pos.x = 1470.0f;	m_move.x = 0.0f; }
			if (m_pos.z < -1470.0f) { m_pos.z = -1470.0f;	m_move.z = 0.0f; }
			if (1470.0f < m_pos.z) { m_pos.z = 1470.0f;	m_move.z = 0.0f; }
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

				if (m_bJump == true)
				{
					// アニメーション再生
					CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 3.0f, 0.0f), D3DXVECTOR3(0.0f, 0.5f, 0.0f), D3DXVECTOR3(6.0f, 6.0f, 0.0f),
						D3DXVECTOR2(0.125f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_WALK));
					pTexAnim->SetAnim(8, 8, 2, 0, false, true);

					if (pSound != NULL)
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_LANDING);
				}

				m_bJump = false;		// ジャンプしない
				m_bAttack_Sky = false;	// 空中攻撃しない
				m_bAction_Sky = false;	// 空中アクションしない
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

			if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
			{
				fRatio = 5.0;
			}
			else
			{
				if (pCamera->GetLockOn() == true)
				{
					fRatio = 5.0;
				}
				else
				{
					fRatio = 30.0f;
				}
			}

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
			if (m_bAttack_Sky == false && m_bWarp == false)		m_move.y -= MC_GRAVITY;
			else if (m_bAttack_Sky == true)	m_move.y = 0.0f;
		}

		// マップアイコン
		if (m_MapIcon != NULL)
		{// NULL以外の場合
			m_MapIcon->Setpos(m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f));
			m_MapIcon->Setrot(m_rot);
		}
	}

	// モーション更新
	{
		if (m_pMotion != NULL)	// NULL以外の場合
			m_pMotion->Update(m_MaxModel, &m_MotionInfo[0], &m_pModel[0]);
	}

	{
		// 文字設定
		CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "Player : 位置　[%.2f, %.2f, %.2f]\n", m_pos.x, m_pos.y, m_pos.z);
		CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "Player : 向き　[%.2f, %.2f, %.2f]\n", m_rot.x, m_rot.y, m_rot.z);
		CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "\n");
	}
}

//=============================================================================
// 描画処理											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Draw(void)
{
	CManager::MODE		mode = CManager::GetMode();		// モード取得
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX			mtxRot, mtxTrans;				// 計算用マトリックス

	if (m_bDraw == true)
	{// 描画する
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

				if (m_fcol_a != 0.0f)
				{
					// 影表示
					CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(m_pos.x, m_shadowHeight + 0.1f, m_pos.z));
				}
			}
		}
	}
}

//=============================================================================
// タイトル処理										(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Title(void)
{
	bool bTutorial = CManager::GetTitle()->GetTutorial();

	if (m_nCntTutorialSet == 0)
	{// ０だったら
		if (bTutorial == true && m_bTutorialSet == false)
		{
			m_bTutorialSet = true;
			m_nCntTutorialSet = 20;
		}
		else if (bTutorial == false && m_bTutorialSet == true)
		{
			m_bTutorialSet = false;
			m_nCntTutorialSet = 20;

			m_bGuard = false;			// ガードしていない
			m_nAttackNum = ATTACK_N_1;		// ０撃目
			m_bAttack_Nor = false;			// 通常攻撃していない
			m_bAttack_Sky = false;			// 空中攻撃していない
			m_bSkill = false;
			m_nAttackCnt = 0;

			for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
				if (m_pOrbit[nCount] != NULL)
					m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			m_brot = false;
		}
	}

	if (m_nCntTutorialSet != 0)
	{
		CTexAnim *pTexAnim = NULL;		// テクスチャアニメーション

		if (m_nCntTutorialSet == 20)
		{// 指定されたカウントだったら
			if (m_pMotion->GetMotionNum() != MOTION_LANDING)
				m_pMotion->SetMotionNum(MOTION_LANDING, 0, 3);	//	着地モーション
			m_bDraw = false;	// 非表示

								// アニメーション再生
			pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
				D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
			pTexAnim->SetAnim(10, 10, 3, 0, false, true);
		}

		m_nCntTutorialSet--;	// ステップカウント

		if (m_nCntTutorialSet == 15)
		{
			if (m_bTutorialSet == true)
			{
				m_pos = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
				m_frotDest = D3DX_PI * 0.5f;
			}
			else
			{
				m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_frotDest = D3DX_PI;
			}
		}

		if (m_nCntTutorialSet == 10)
		{// 指定されたカウントだったら
			m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// ニュートラルモーション
			m_bDraw = true;		// 表示

			// アニメーション再生
			pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
				D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
			pTexAnim->SetAnim(10, 10, 3, 0, false, true);
		}

		if (m_nCntTutorialSet < 0)
		{// 一定カウント以下だったら
			m_nCntTutorialSet = 0;		// ステップカウント初期化		
			m_bStep = false;	// ステップしていない
		}
	}
}

//=============================================================================
// 状態処理											(private)	*** CPlayer ***
//=============================================================================
void CPlayer::State(void)
{
	// カメラ取得
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	CGame *pGame = CManager::GetGame();

	float col_a = 1.0f;	// 色設定用

	// 状態処理	
	{
		switch (m_state)
		{// 状態
		case STATE_NORMAL:		// 通常
			if (m_pGuardDome[GUARD_DOME_UP] != NULL)
				m_pGuardDome[GUARD_DOME_UP]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));
			if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
				m_pGuardDome[GUARD_DOME_DOWN]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));

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

			if (m_fDamageCat == 0.3f)
			{// ジャストダメージカット
				if (m_pGuardDome[GUARD_DOME_UP] != NULL)
					m_pGuardDome[GUARD_DOME_UP]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));
				if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
					m_pGuardDome[GUARD_DOME_DOWN]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));

				if (pGame != NULL)
				{// NULL以外の場合
					CGame::GAME game = pGame->GetGame();
					if (game != CGame::GAME_TUTORIAL && game != CGame::GAME_CLEAR && game != CGame::GAME_OVER)
					{// チュートリアル + クリア + オーバー以外
						pGame->AddJustGuard();
					}
				}
			}
			else
			{// その他
				if (m_pGuardDome[GUARD_DOME_UP] != NULL)
					m_pGuardDome[GUARD_DOME_UP]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));
				if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
					m_pGuardDome[GUARD_DOME_DOWN]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));
			}	

			if (pGame != NULL)
			{// NULL以外の場合
				CGame::GAME game = pGame->GetGame();
				if (game != CGame::GAME_TUTORIAL && game != CGame::GAME_CLEAR && game != CGame::GAME_OVER)
				{// チュートリアル + クリア + オーバー以外
					pGame->SetNoDamage();
				}
			}

			break;
		case STATE_DEATH:		// しんだ
			CManager::MODE mode = CManager::GetMode();
			if (mode == CManager::MODE_TITLE)
			{// モードがタイトルだったら
				m_nHP = 100;
				m_nMP = 100;
				m_state = STATE_NORMAL;
			}
			else if (mode == CManager::MODE_GAME)
			{// モードがゲームだったら
				if (pGame != NULL)
				{// NULL以外の場合
					if (pGame->GetGame() == CGame::GAME_TUTORIAL)
					{// チュートリアルだったら
						m_nHP = 100;
						m_nMP = 100;
						m_state = STATE_NORMAL;
					}
					else
					{// その他
						if (pGame->GetGame() != CGame::GAME_OVER)
							pGame->SetGame(CGame::GAME_OVER, false);

						pCamera->SetGameEnd(true);
						m_bMove = false;

						if (m_pMotion->GetMotionNum() != MOTION_DEATH)
							m_pMotion->SetMotionNum(MOTION_DEATH);	// ニュートラルモーション
						m_pMotion->SetKeyStop(true);				// 最終キーで停止
						m_bGuard = false;			// ガードしていない
						m_nAttackNum = ATTACK_N_1;		// ０撃目
						m_bAttack_Nor = false;			// 通常攻撃していない
						m_bAttack_Sky = false;			// 空中攻撃していない
						m_bSkill = false;
						m_nAttackCnt = 0;

						for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
							if (m_pOrbit[nCount] != NULL)
								m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

						m_brot = false;
					}
				}
			}

			for (int nCount = 0; nCount < GUARD_DOME_MAX; nCount++)
			{// ガードドームカウント
				if (m_pGuardDome[nCount] != NULL)
				{// NULL以外の場合
					m_pGuardDome[nCount]->Uninit();		// 終了処理
					m_pGuardDome[nCount] = NULL;		// NULLへ
				}
			}

			break;
		}
	}

	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_TITLE)
	{// モードがタイトルだったら
		CUi_Game *pUi_Game = (CUi_Game*)CManager::GetTitle()->GetUi();
		if (pUi_Game != NULL)
		{// NULL以外の場合
			pUi_Game->SetGauge(CUi_Game::GAUGE_PLAYER_HP, m_nHP, m_MAX_HP);
			pUi_Game->SetGauge(CUi_Game::GAUGE_PLAYER_MP, m_nMP, m_MAX_MP);
		}
	}
	else if (mode == CManager::MODE_GAME)
	{// モードがゲームだったら
		CUi_Game *pUi_Game = (CUi_Game*)CManager::GetGame()->GetUi();
		if (pUi_Game != NULL)
		{// NULL以外の場合
			pUi_Game->SetGauge(CUi_Game::GAUGE_PLAYER_HP, m_nHP, m_MAX_HP);
			pUi_Game->SetGauge(CUi_Game::GAUGE_PLAYER_MP, m_nMP, m_MAX_MP);
		}
	}

	D3DXVECTOR3 Camerapos	= pCamera->Getpos(0);	// 位置取得

	// 距離計算
	float		fLenght		= sqrtf(powf(m_pos.x - Camerapos.x, 2.0f) + powf(m_pos.z - Camerapos.z, 2.0f));

	// 距離によって透過する
	if (fLenght < 80.0f)
		for (int nCount = 0; nCount < MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(fLenght / 80.0f);
	else
		for (int nCount = 0; nCount< MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(col_a);
}

//=============================================================================
// 移動処理											(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Move(void)
{
	CSound *pSound = CManager::GetSound();

	bool			bWalk		= false;							// 歩いたか
	bool			bInput		= false;							// 入力したか
	CCameraMain		*pCamera	= (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// カメラ取得
	D3DXVECTOR3		Camerarot	= pCamera->Getrot();				// カメラ角度取得
	float			fLockOn		= pCamera->GetLockOnRot();			// ロックオン角度取得
	CInputKeyboard	*pKey		= CManager::GetInputKeyboard();		// キーボード取得
	CInputXPad		*pXPad		= CManager::GetInputXPad();			// Xパッド取得

	// 移動処理
	{
		if (m_bMove == true)
		{// 移動できる場合
			if (m_bGuard == false && m_bStep == false && m_brot == false)
			{// ガードしていない、ステップしていない
				// Xパッド
				{
					if (pXPad != NULL)
					{// NULL以外の場合
						if (bInput == false)
						{// 入力していない
							if (pXPad->GetStick(0, 0) == true)
							{// Xパッド（左スティック）を入力した
								// 入力した方向を向く
								m_frotDest = pXPad->GetStickRot(0, 0) + Camerarot.y + D3DX_PI;

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// 歩いた
								bInput = true;	// 入力した
							}
						}
					}
				}

				// キーボード
				{
					if (pKey != NULL)
					{// NULL以外の場合
						if (bInput == false)
						{// 入力していない
							if (pKey->GetPress(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true)
							{// キーボード（W）が押された
								if (pKey->GetPress(DIK_A) == true || pKey->GetTrigger(DIK_LEFT) == true)
									m_frotDest = -D3DX_PI * 0.25f + Camerarot.y + D3DX_PI;		// 左上
								else if (pKey->GetPress(DIK_D) == true || pKey->GetTrigger(DIK_RIGHT) == true)
									m_frotDest = D3DX_PI * 0.25f + Camerarot.y + D3DX_PI;		// 右上
								else
									m_frotDest = D3DX_PI * 0.0f + Camerarot.y + D3DX_PI;		// 上

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// 歩いた
							}
							else if (pKey->GetPress(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true)
							{// キーボード（S）が押された
								if (pKey->GetPress(DIK_A) == true || pKey->GetTrigger(DIK_LEFT) == true)
									m_frotDest = -D3DX_PI * 0.75f + Camerarot.y + D3DX_PI;		// 左下
								else if (pKey->GetPress(DIK_D) == true || pKey->GetTrigger(DIK_RIGHT) == true)
									m_frotDest = D3DX_PI * 0.75f + Camerarot.y + D3DX_PI;		// 右下
								else
									m_frotDest = D3DX_PI * 1.0f + Camerarot.y + D3DX_PI;		// 下

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// 歩いた
							}
							else if (pKey->GetPress(DIK_A) == true || pKey->GetTrigger(DIK_LEFT) == true)
							{// キーボード（A）が押された
								if (pKey->GetPress(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true)
									m_frotDest = -D3DX_PI * 0.25f + Camerarot.y + D3DX_PI;		// 左上
								else if (pKey->GetPress(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true)
									m_frotDest = -D3DX_PI * 0.75f + Camerarot.y + D3DX_PI;		// 左下
								else
									m_frotDest = -D3DX_PI * 0.5f + Camerarot.y + D3DX_PI;		// 左

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// 歩いた
							}
							else if (pKey->GetPress(DIK_D) == true || pKey->GetTrigger(DIK_RIGHT) == true)
							{// キーボード（D）が押された
								if (pKey->GetPress(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true)
									m_frotDest = D3DX_PI * 0.25f + Camerarot.y + D3DX_PI;		// 右上
								else if (pKey->GetPress(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true)
									m_frotDest = D3DX_PI * 0.75f + Camerarot.y + D3DX_PI;		// 右下
								else
									m_frotDest = D3DX_PI * 0.5f + Camerarot.y + D3DX_PI;		// 右

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// 歩いた
							}
						}
					}
				}
			}

			if (bWalk == true)
			{// 歩いた
				if (m_bStep == false)
				{// ステップしていない
					// 移動
					m_move.x += sinf(m_frotDest) * MC_MOVE_UP;
					m_move.z += cosf(m_frotDest) * MC_MOVE_UP;
				}
			}
		}
	}

	// モーション設定
	{
		if (m_bJump == false && m_bAttack_Nor == false && m_bGuard == false && m_bStep == false && m_bSkill == false)
		{// ジャンプしていない、通常攻撃していない、ガードしていない、ステップしていない
			if (bWalk == true)
			{// 歩いた
				if (m_pMotion->GetMotionNum() != MOTION_WALK)
					m_pMotion->SetMotionNum(MOTION_WALK);		// 歩くモーション

				if (m_pMotion->GetMotionKeyNum() % 2 == 0 && m_pMotion->GetCntFrame() == 0)
				{// 一定間隔で
					// 角度
					float fAngle = 0.0f;
					if (m_pMotion->GetMotionKeyNum() / 2 == 0) fAngle = m_rot.y + (D3DX_PI * 0.5f);
					else fAngle = m_rot.y - (D3DX_PI * 0.5f);

					// アニメーション再生
					CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(sinf(fAngle) * 3.0f, 3.0f, cosf(fAngle) * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 4.0f, 0.0f),
						D3DXVECTOR2(0.125f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_WALK));
					pTexAnim->SetAnim(8, 8, 2, 0, false, true);

					if (pSound != NULL)
					{
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WOLK);
					}
				}
			}
			else
			{// 歩いていない
				if (m_pMotion->GetMotionNum() == MOTION_WALK)
				{// 歩くモーションのみ
					if (m_pMotion->GetMotionKeyNum() % 2 == 1 && m_pMotion->GetCntFrame() == m_MotionInfo[MOTION_WALK].aKeyInfo[m_pMotion->GetMotionKeyNum()].nFrame - 1)
					{// 変更タイミング調整
						m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// ニュートラルモーション

						if (pCamera->GetLockOn() == true)
							m_frotDest = fLockOn + D3DX_PI;		// ロックオン方向を向く
					}
				}
				else if (m_pMotion->GetMotionNum() != MOTION_NEUTRAL)
				{// ニュートラルモーション以外だったら
					m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// ニュートラルモーション

					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ロックオン方向を向く
				}
			}
		}
	}
}

//=============================================================================
// ステップ処理										(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Step(void)
{
	CSound			*pSound		= CManager::GetSound();				// サウンド取得

	bool			bStep		= false;							// ステップしたか
	bool			bInput		= false;							// 入力したか
	CCameraMain		*pCamera	= (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// カメラ取得
	D3DXVECTOR3		Camerarot	= pCamera->Getrot();				// カメラ角度取得
	float			fLockOn		= pCamera->GetLockOnRot();			// ロックオン角度取得
	CInputKeyboard	*pKey		= CManager::GetInputKeyboard();		// キーボード取得
	CInputMouse		*pMouse		= CManager::GetInputMouse();		// マウス取得
	CInputXPad		*pXPad		= CManager::GetInputXPad();			// Xパッド取得

	// ステップ処理
	{
		if (m_bMove == true)
		{// 移動できる場合
			if (m_bGuard == false && m_bStep == false)
			{// ガードしていない、ステップしていない
				if ((pKey != NULL && pKey->GetTrigger(DIK_LSHIFT) == true) ||
					(pXPad != NULL && pXPad->GetTrigger(CInputXPad::XPADOTHER_TRIGGER_RIGHT, 0) == true))
				{// NULL以外の場合、（左SHIFT、R2）が押された
					if (m_bAttack_Nor == true || m_bAttack_Sky == true || m_bSkill == true)
					{// 攻撃している
						if (11 < m_nAttackCnt)
						{// 入力調整（攻撃）
							bStep = true;		// ステップした

							if (m_bAttack_Nor == true)
							{// 通常攻撃している
								m_nAttackNum = ATTACK_N_1;	// ０撃目
								m_bAttack_Nor = false;		// 通常攻撃していない
							}

							m_bAttack_Sky = false;		// 空中攻撃していない
							m_bAction_Sky = false;		// 空中アクションしていない

							m_bSkill = false;
							m_brot = false;

							for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
								if (m_pOrbit[nCount] != NULL)
									m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

							m_nStepCnt = 20;		// ステップカウント		
							m_bStep = true;		// ステップした
						}
					}
					else if (0 < m_nGuardCnt)
					{// 攻撃していない、入力調整（ガード）
						if (m_nGuardCnt < 5)
						{// 入力調整（ガード）
							bStep = true;		// ステップした

							m_brot = false;

							m_bGuard = false;	// ガードしていない

							m_nStepCnt = 20;		// ステップカウント
							m_bStep = true;		// ステップした
						}
					}
					else
					{// その他
						m_brot = false;

						m_nStepCnt = 20;		// ステップカウント
						m_bStep = true;		// ステップした
					}
				}
			}
		}

		if (m_bStep == true)
		{// ステップした
			if ((m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false) || bStep == true)
			{// 攻撃していない、ステップした
				m_nAttackCnt = 0;

				CTexAnim *pTexAnim = NULL;		// テクスチャアニメーション

				if (m_nStepCnt == 20)
				{// 指定されたカウントだったら
					if (m_pMotion->GetMotionNum() != MOTION_LANDING)
						m_pMotion->SetMotionNum(MOTION_LANDING, 0, 3);	//	着地モーション
					m_bDraw = false;	// 非表示

					// アニメーション再生
					pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
						D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
					pTexAnim->SetAnim(10, 10, 3, 0, false, true);

					if (pSound != NULL)
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WORP000);
				}

				m_nStepCnt--;	// ステップカウント

				if (10 < m_nStepCnt)
				{// 一定カウント以上だったら
					// 移動
					m_move.x += sinf(m_frotDest) * MC_MOVE_UP * 3.0f;
					m_move.z += cosf(m_frotDest) * MC_MOVE_UP * 3.0f;
				}

				if (m_nStepCnt == 10)
				{// 指定されたカウントだったら
					m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// ニュートラルモーション
					m_bDraw = true;		// 表示

					// アニメーション再生
					pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
						D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
					pTexAnim->SetAnim(10, 10, 3, 0, false, true);

					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ロックオン方向を向く

					//if (pSound != NULL)
					//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WORP000);
				}

				if (m_nStepCnt < 0)
				{// 一定カウント以下だったら
					m_nStepCnt	= 0;		// ステップカウント初期化		
					m_bStep		= false;	// ステップしていない
				}
			}
			else
			{// 攻撃した
				// アニメーション再生
				CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
					D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
				pTexAnim->SetAnim(10, 10, 3, 0, false, true);

				m_bDraw		= true;		// 表示
				m_nStepCnt	= 0;		// ステップカウント初期化
				m_bStep		= false;	// ステップしていない

				m_nAttackCnt = 0;
			}
		}
		else
		{// ステップしていない
			m_bDraw = true;		// 表示
		}
	}
}

//=============================================================================
// アクション処理										(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Action(void)
{
	CCameraMain		*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// カメラ取得
	float			fLockOn = pCamera->GetLockOnRot();			// ロックオン角度取得
	CInputKeyboard	*pKey = CManager::GetInputKeyboard();		// キーボード取得
	CInputMouse		*pMouse = CManager::GetInputMouse();		// マウス取得
	CInputXPad		*pXPad = CManager::GetInputXPad();			// Xパッド取得
	CSound			*pSound = CManager::GetSound();				// サウンド取得

	// 通常攻撃
	{
		if (m_bGuard == false && m_bJump == false && m_bSkill == false)
		{// ガードしていない、ジャンプしていない
			if (m_bMove == true)
			{// 移動できる場合
				if (m_bNotAttack == false)
				{
					if (m_nStepCnt < 12)
					{// 入力調整（ステップ）
						if ((pKey != NULL && pKey->GetTrigger(DIK_B) == true) ||
							(pMouse != NULL && pMouse->GetTrigger(0) == true) ||
							(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_X, 0) == true))
						{// NULL以外の場合、キーボード（B）、Xパッド（X）が押された
							if (m_bAttack_Nor == false && m_nAttackRecast == 0)
							{// 通常攻撃していない
								if (m_pMotion->GetMotionNum() != MOTION_ATTACK_N)
									m_pMotion->SetMotionNum(MOTION_ATTACK_N);		// 通常攻撃モーション
								m_nAttackNum = ATTACK_N_1;			// １撃目
								m_bAttack_Nor = true;				// 通常攻撃する

								m_nAttackRecast = 8;				// リキャストを付ける

								m_brot = false;

								if (pCamera->GetLockOn() == true)
									m_frotDest = fLockOn + D3DX_PI;		// ロックオン方向を向く
								//m_frotDest = cameraRot.y + D3DX_PI;
							}
							else
							{// 通常攻撃した
								if (m_pMotion->GetMotionNum() == MOTION_ATTACK_N)
								{// 通常攻撃モーションのみ
									if (m_nAttackNum == ATTACK_N_1)
									{// １撃目
										if (m_pMotion->GetMotionKeyNum() <= 1)
										{// 一定フレーム以内だったら
											m_nAttackNum = ATTACK_N_2;		// ２撃目
										}
									}
									else if (m_nAttackNum == ATTACK_N_2)
									{// ２撃目
										if (m_pMotion->GetMotionKeyNum() <= 3)
										{// 一定フレーム以内だったら
											m_nAttackNum = ATTACK_N_3;		// ３撃目
											m_nAttackRecast = 0;			// リキャストを初期化
										}
									}
								}
							}
						}
					}
				}
			}

			if (m_bAttack_Nor == true)
			{// 通常攻撃している
				if (m_pMotion->GetMotionNum() == MOTION_ATTACK_N)
				{// 通常攻撃モーションのみ
					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ロックオン方向を向く
					//m_frotDest = cameraRot.y + D3DX_PI;

					// エフェクト
					if (m_pMotion->GetMotionKeyNum() == ATTACK_SHOT_1 && m_pMotion->GetCntFrame() == 0)
					{// １撃
						CEffect::Create(CEffect::TYPE_3D, CEffect::SHOT_PLAYER, m_pos + D3DXVECTOR3(0.0f, 14.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(m_rot.y) * 8.0f, 0.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(7.0f, 7.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SHOT000);
					}
					if (m_pMotion->GetMotionKeyNum() == ATTACK_SHOT_2 && m_pMotion->GetCntFrame() == 0)
					{// ２撃
						CEffect::Create(CEffect::TYPE_3D, CEffect::SHOT_PLAYER, m_pos + D3DXVECTOR3(0.0f, 14.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(m_rot.y) * 8.0f, 0.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(7.0f, 7.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SHOT000);
					}
					if (m_pMotion->GetMotionKeyNum() == ATTACK_SHOT_3 && m_pMotion->GetCntFrame() == 0)
					{// ３撃
						CEffect::Create(CEffect::TYPE_C01, CEffect::SHOT_PLAYER, m_pos + D3DXVECTOR3(0.0f, 14.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(m_rot.y) * 8.0f, 0.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(9.0f, 9.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SHOT000);
					}

					if (m_pMotion->GetMotionKeyNum() <= ATTACK_SHOT_1)
					{// １撃
						if (m_pOrbit[ORBIT_LEFT] != NULL)
							m_pOrbit[ORBIT_LEFT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
						if (m_pOrbit[ORBIT_RIGHT] != NULL)
							m_pOrbit[ORBIT_RIGHT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
					}
					else if (m_pMotion->GetMotionKeyNum() <= ATTACK_SHOT_2)
					{// ２撃
						if (m_pOrbit[ORBIT_LEFT] != NULL)
							m_pOrbit[ORBIT_LEFT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
						if (m_pOrbit[ORBIT_RIGHT] != NULL)
							m_pOrbit[ORBIT_RIGHT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
					}
					else
					{// ３撃
						if (m_pOrbit[ORBIT_LEFT] != NULL)
							m_pOrbit[ORBIT_LEFT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
						if (m_pOrbit[ORBIT_RIGHT] != NULL)
							m_pOrbit[ORBIT_RIGHT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
					}

					if (m_pMotion->GetMotionKeyNum() == m_nAttackNum)
					{// 指定フレームになったら
						if (m_pMotion->GetMotionNum() != MOTION_NEUTRAL)
							m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// ニュートラルモーション
						m_nAttackNum	= ATTACK_N_1;		// ０撃目
						m_bAttack_Nor	= false;			// 通常攻撃していない
						m_nAttackCnt	= 0;

						for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
							if (m_pOrbit[nCount] != NULL)
								m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

						m_brot = false;

						if (m_bStep == true)
						{// ステップしている
							// アニメーション再生
							CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
								D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
							pTexAnim->SetAnim(10, 10, 3, 0, false, true);

							m_bDraw		= true;		// 表示
							m_nStepCnt	= 0;		// ステップカウント初期化
							m_bStep		= false;	// ステップしていない
						}
					}
				}
				m_nAttackCnt++;
			}
			else
			{
				m_nAttackCnt = 0;
			}
		}
		
		if (0 < m_nAttackRecast)
		{// ０より多かったら
			m_nAttackRecast--;		// 減らす
		}
	}

	// 空中攻撃
	{
		if (m_bGuard == false && m_bJump == true && m_bSkill == false)
		{// ガードしていない、ジャンプしている
			if (m_bMove == true)
			{// 移動できる場合
				if (m_bNotAttack == false)
				{
					if (m_bAction_Sky == false)
					{// 空中アクションしていない
						if (m_nStepCnt < 12)
						{// 入力調整（ステップ）
							if ((pKey != NULL && pKey->GetTrigger(DIK_B) == true) ||
								(pMouse != NULL && pMouse->GetTrigger(0) == true) ||
								(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_X, 0) == true))
							{// NULL以外の場合、キーボード（B）、Xパッド（X）が押された
								if (m_pMotion->GetMotionNum() != MOTION_ATTACK_S)
									m_pMotion->SetMotionNum(MOTION_ATTACK_S);		// 空中攻撃モーション
								m_bAttack_Sky = true;			// 空中攻撃する
								m_bAction_Sky = true;			// 空中アクションする

								for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
									if (m_pOrbit[nCount] != NULL)
										m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));

								m_brot = false;

								if (pCamera->GetLockOn() == true)
									m_frotDest = fLockOn + D3DX_PI;		// ロックオン方向を向く
								//m_frotDest = cameraRot.y + D3DX_PI;
							}
						}
					}
				}
			}

			if (m_bAttack_Sky == true)
			{// 空中攻撃している
				if (m_pMotion->GetMotionNum() == MOTION_ATTACK_S)
				{// 空中攻撃モーションのみ
					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ロックオン方向を向く

					// エフェクト
					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_S].nKeyMax - 2 && m_pMotion->GetCntFrame() == 0)
					{
						CEffect_C01 *pEffect = (CEffect_C01*)CEffect::Create(CEffect::TYPE_C01, CEffect::SHOT_PLAYER, m_pos + D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(sinf(m_rot.y) * 6.0f, -5.0f, cosf(m_rot.y) * 6.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						pEffect->SetExplosion(true);

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SHOT000);
					}

					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_S].nKeyMax - 1)
					{// 指定フレームだったら
						if (m_pMotion->GetMotionNum() != MOTION_LANDING)
							m_pMotion->SetMotionNum(MOTION_LANDING);	// 着地モーション
						m_bAttack_Sky = false;			// 空中攻撃していない
						m_nAttackCnt = 0;

						for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
							if (m_pOrbit[nCount] != NULL)
								m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

						m_brot = false;

						if (m_bStep == true)
						{// ステップしている
							// アニメーション再生
							CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
								D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
							pTexAnim->SetAnim(10, 10, 3, 0, false, true);

							m_bDraw		= true;		// 表示
							m_nStepCnt	= 0;		// ステップカウント初期化
							m_bStep		= false;	// ステップしていない
						}	
					}
				}
				m_nAttackCnt++;
			}
			else
			{
				m_nAttackCnt = 0;
			}
		}
	}

	// スキル
	{
		if (m_bGuard == false)
		{// ガードしていない、ジャンプしていない
			if (m_bMove == true)
			{// 移動できる場合
				if (m_bNotSkill == false)
				{
					if (m_bSkill == false)
					{// スキルしていない
						if (m_nStepCnt < 12)
						{// 入力調整（ステップ）
							if (m_nSkillRecast == 0)
							{// スキルリキャストが０だったら
								if ((pKey != NULL && pKey->GetTrigger(DIK_N) == true) ||
									(pMouse != NULL && pMouse->GetTrigger(1) == true) ||
									(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_Y, 0) == true))
								{// NULL以外の場合、キーボード（B）、Xパッド（X）が押された
									if (m_pMotion->GetMotionNum() != MOTION_ATTACK_SKILL_1)
										m_pMotion->SetMotionNum(MOTION_ATTACK_SKILL_1);		// ガードモーション

									m_bSkill = true;
									m_nSkillRecast = 30;

									AddMP(-20);								// MP消費

									if (m_pOrbit[ORBIT_RIGHT] != NULL)
										m_pOrbit[ORBIT_RIGHT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));

									if (m_bAttack_Nor == true)
									{// 通常攻撃している
										m_nAttackNum = ATTACK_N_1;			// ０撃目
										m_bAttack_Nor = false;				// 通常攻撃していない
										m_nAttackCnt = 0;
									}

									if (m_bAttack_Sky == true)
									{// 空中攻撃している
										m_bAttack_Sky = false;				// 空中攻撃していない
									}

									if (m_bJump == true)
									{// ジャンプしている
										m_move.y = 7.0f;					// 少し上昇
									}

									m_brot = false;

									if (pCamera->GetLockOn() == true)
										m_frotDest = fLockOn + D3DX_PI;		// ロックオン方向を向く

									if (pSound != NULL)
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SKILL001);
								}
							}
						}
					}
				}
			}

			if (m_bSkill == true)
			{// スキル使っている
				if (m_pMotion->GetMotionNum() == MOTION_ATTACK_SKILL_1)
				{// 空中攻撃モーションのみ
					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ロックオン方向を向く
			
					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_SKILL_1].nKeyMax - 3)
					{
						m_brot = true;
						m_frotDest = m_rot.y;
					}

					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_SKILL_1].nKeyMax - 2 && m_pMotion->GetCntFrame() == 0)
					{
						CEffect::Create(CEffect::TYPE_C03, CEffect::SHOT_PLAYER, D3DXVECTOR3(m_pModel[MODEL_04_handR].GetmtxWorld()._41, m_pos.y + 1.8f, m_pModel[MODEL_04_handR].GetmtxWorld()._43), m_rot,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(80.0f, 0.0f, 80.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SKILL000);
					}

					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_SKILL_1].nKeyMax - 1)
					{// 指定フレームだったら
						if (m_pMotion->GetMotionNum() != MOTION_NEUTRAL)
							m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// ニュートラルモーション

						m_bSkill = false;

						for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
							if (m_pOrbit[nCount] != NULL)
								m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

						m_brot = false;

						if (m_bStep == true)
						{// ステップしている
						 // アニメーション再生
							CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
								D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
							pTexAnim->SetAnim(10, 10, 3, 0, false, true);

							m_bDraw = true;		// 表示
							m_nStepCnt = 0;		// ステップカウント初期化
							m_bStep = false;	// ステップしていない
						}
					}
				}
				m_nAttackCnt++;
			}
			else
			{
				m_nAttackCnt = 0;
			}
		}

		if (0 < m_nSkillRecast)
		{
			m_nSkillRecast--;
		}
	}

	// ガード
	{
		if ((pKey != NULL && pKey->GetPress(DIK_V) == true) ||
			(pMouse != NULL && pMouse->GetPress(2) == true) ||
			(pXPad != NULL && pXPad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, 0) == true))
		{// NULL以外の場合、キーボード（V）、Xパッド（R1）が押された
			if (m_bMove == true)
			{// 移動できる場合
				if (m_bGuard == false)
				{// ガードしていない
					if (m_pMotion->GetMotionNum() != MOTION_GUARD)
					{// ガードモーション以外
						m_pMotion->SetMotionNum(MOTION_GUARD);		// ガードモーション
						m_pMotion->SetKeyStop(true);				// 最終キーで停止
					}
					m_bGuard = true;				// ガードしている
					m_fDamageCat = 0.3f;			// ダメージカット

					if (m_bJump == true)
					{// ジャンプしている
						m_bAttack_Sky = false;		// 空中攻撃していない
					}

					m_nAttackNum = ATTACK_N_1;		// ０撃目
					m_bAttack_Nor = false;			// 通常攻撃していない
					m_bAttack_Sky = false;			// 空中攻撃していない
					m_bSkill = false;
					m_nAttackCnt = 0;

					for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
						if (m_pOrbit[nCount] != NULL)
							m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

					m_brot = false;

					if (m_pGuardDome[GUARD_DOME_UP] == NULL)
					{// NULLの場合
						m_pGuardDome[GUARD_DOME_UP] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GUARD);

						m_pGuardDome[GUARD_DOME_UP]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(20.0f, 10.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f),
							D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.5f));
						m_pGuardDome[GUARD_DOME_UP]->Init();
						m_pGuardDome[GUARD_DOME_UP]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
						m_pGuardDome[GUARD_DOME_UP]->DrawEdit(false, false, false, false, true);
					}
					if (m_pGuardDome[GUARD_DOME_DOWN] == NULL)
					{// NULLの場合
						m_pGuardDome[GUARD_DOME_DOWN] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GUARD);

						m_pGuardDome[GUARD_DOME_DOWN]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(20.0f, 10.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f),
							D3DXVECTOR3(-D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.5f));
						m_pGuardDome[GUARD_DOME_DOWN]->Init();
						m_pGuardDome[GUARD_DOME_DOWN]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
						m_pGuardDome[GUARD_DOME_DOWN]->DrawEdit(false, false, false, true, true);
					}
					m_GuardDomeUV = D3DXVECTOR2(0.0f, 0.0f);

					if (m_bStep == true)
					{// ステップしている
						// アニメーション再生
						CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
							D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
						pTexAnim->SetAnim(10, 10, 3, 0, false, true);

						m_bDraw = true;		// 表示
						m_nStepCnt = 0;		// ステップカウント初期化
						m_bStep = false;	// ステップしていない
					}
				}
				else
				{
					if (m_bGuard == true)
					{// ガードしている
						if (m_nCntJastGuard < 5)
							m_fDamageCat = 0.3f;			// ダメージカット
						else
							m_fDamageCat = 0.5f;			// ダメージカット					

						if (m_nCntJastGuard < 5)
							m_nCntJastGuard++;

						if (m_pGuardDome[GUARD_DOME_UP] != NULL)
						{// NULL以外の場合
							m_pGuardDome[GUARD_DOME_UP]->Setpos(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f));
							m_pGuardDome[GUARD_DOME_UP]->SetTex(-m_GuardDomeUV);
						}
						if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
						{// NULL以外の場合
							m_pGuardDome[GUARD_DOME_DOWN]->Setpos(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f));
							m_pGuardDome[GUARD_DOME_DOWN]->SetTex(m_GuardDomeUV);
						}
						m_GuardDomeUV.y += 0.02f;
					}
				}
			}
		}
		else
		{// ガード離した
			if (m_bGuard == true)
			{// ガードしている
				if (m_pMotion->GetMotionNum() != MOTION_NEUTRAL)
					m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// ニュートラルモーション
				m_pMotion->SetKeyStop(false);		// 最終キーで停止解除
				m_bGuard		= false;			// ガードしていない
				m_nCntJastGuard = 0;				// ジャストガードカウント
				m_fDamageCat	= 1.0f;				// ダメージカット
				m_nAttackNum	= ATTACK_N_1;		// ０撃目
				m_bAttack_Nor	= false;			// 通常攻撃していない
				m_bAttack_Sky	= false;			// 空中攻撃していない
				m_bSkill = false;
				m_nAttackCnt = 0;

				for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
					if (m_pOrbit[nCount] != NULL)
						m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

				m_brot = false;

				if (m_pGuardDome[GUARD_DOME_UP] != NULL)
				{// NULL以外の場合
					m_pGuardDome[GUARD_DOME_UP]->Uninit();		// 終了処理
					m_pGuardDome[GUARD_DOME_UP] = NULL;			// NULLへ
				}
				if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
				{// NULL以外の場合
					m_pGuardDome[GUARD_DOME_DOWN]->Uninit();	// 終了処理
					m_pGuardDome[GUARD_DOME_DOWN] = NULL;		// NULLへ
				}
			}
		}

		// ガードフレーム
		if (m_bGuard == true)
			m_nGuardCnt = 10;		// ガードカウント設定
		else
			if (0 < m_nGuardCnt)	// 一定フレーム以上だったら
				m_nGuardCnt--;			// ガードカウント
	}
}

//=============================================================================
// ジャンプ処理										(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Jump(void)
{
	CSound			*pSound = CManager::GetSound();				// サウンド取得

	CInputKeyboard	*pKey = CManager::GetInputKeyboard();		// キーボード取得
	CInputXPad		*pXPad = CManager::GetInputXPad();			// Xパッド取得

	// ジャンプ処理
	{
		if (m_bMove == true)
		{// 移動できる場合
			if (m_bJump == false && m_bGuard == false && m_bSkill == false)
			{// ジャンプいていない、ガードしていない
				if (m_nStepCnt < 12)
				{// 入力調整（ステップ）
					if ((pKey != NULL && pKey->GetTrigger(DIK_SPACE) == true) ||
						(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_A, 0) == true))
					{// NULL以外の場合、キーボード（スペース）、Xパッド（A）が押された
						if (m_pMotion->GetMotionNum() != MOTION_JUMP)
							m_pMotion->SetMotionNum(MOTION_JUMP);		// ジャンプモーション
						m_move.y = MC_JUMP;		// 上昇
						m_bJump = true;			// ジャンプした

						if (m_bAttack_Nor == true)
						{// 通常攻撃している
							m_nAttackNum = ATTACK_N_1;	// ０撃目
							m_bAttack_Nor = false;		// 通常攻撃していない
						}

						if (m_bStep == true)
						{// ステップしている
							// アニメーション再生
							CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
								D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
							pTexAnim->SetAnim(10, 10, 3, 0, false, true);

							m_bDraw = true;		// 表示
							m_nStepCnt = 0;		// ステップカウント初期化
							m_bStep = false;	// ステップしていない
						}

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_JUMP);
					}
				}
			}
		}
	}

	// モーション設定
	{
		if (m_bJump == true && m_bGuard == false && m_bSkill == false)
		{// ジャンプしている、ガードしていない
			if (m_move.y < 0.0f)	// 下降している
			{
				if (m_pMotion->GetMotionNum() != MOTION_LANDING)
					m_pMotion->SetMotionNum(MOTION_LANDING);	//	着地モーション
			}
		}
	}
}

//=============================================================================
// 判定チェック処理									(private)	*** CPlayer ***
//=============================================================================
void CPlayer::ColisionChack(float *fHeight)
{
	CSound *pSound = CManager::GetSound();

	CScene *pScene		= NULL;		// シーン
	CScene *pSceneNext	= NULL;		// 次シーン

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

	// 敵
	{
		if (m_bStep == false || (m_bStep == true && m_nStepCnt < 15))
		{// ステップしていない、（ステップしている、一定フレーム以下だったら）
			pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

			while (pScene != NULL)
			{// NULL以外の場合
				pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

				if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
				{// オブジェクトタイプが敵の場合
					CEnemy *pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
					pEnemy->Collision(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin);
				}
				pScene = pSceneNext;	// 次シーンへ
			}
		}
	}

	CInputKeyboard *pKey = CManager::GetInputKeyboard();
	CInputXPad *pXPad = CManager::GetInputXPad();
	CFade::FADE fade = CFade::GetFade();

	// マップ
	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_GAME)
	{// ゲームのみ
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// NULL以外の場合
			CMap *pMap = pGame->GetMap();
			if (pMap != NULL)
			{// NULL以外の場合
				int nMapNum = pGame->GetMapNum();
				if (nMapNum == 2)
				{// マップ２
					CMap_02 *pMap_02 = (CMap_02*)pMap;

					if (m_bWarp == false)
						pMap_02->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f);		// マップ当たり判定

					if (m_bWarp == false)
					{// ワープしていない
						bool bWarpIn = false;
						if (pMap_02->CollisionWarp(&m_pos, &m_posWarp) == true)
						{// ワープ内にいるか
							*fHeight = 6.0f;
							bWarpIn = true;
						}

						if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
						{// アクションを押した
							if (bWarpIn == true)
							{
								m_bWarp = true;			// ワープする
								m_bMove = false;		// 移動制限

								if (pSound != NULL)
								{// NULL以外の場合
									pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
								}
							}
						}
					}

					if (m_bWarp == true)
					{// ワープする
						CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);

						if (m_nCntWarp < 30)
						{// 一定フレームまで
							if (0.0f < m_fcol_a)
							{
								for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
									m_pModel[nCntModel].Setcol_a(m_fcol_a);				// 色設定

								m_fcol_a -= (float)(1.0f / (float)30);
							}
						}
						else if (m_nCntWarp < 120)
						{// 一定フレームまで
							if (m_nCntWarp == 30)
							{// 一定フレームだったら
								m_fWarpLenght = sqrtf(powf(m_posWarp.x - m_pos.x, 2.0f) + powf(m_posWarp.z - m_pos.z, 2.0f));
								m_fWarpAngle = atan2f(m_posWarp.x - m_pos.x, m_posWarp.z - m_pos.z);								

								pCamera->SetMoveCollision(false);

								pCamera->SetDistance(0, pCamera->GetDistance(0) + 500.0f);
								pCamera->Setrot(D3DXVECTOR3(-0.9f, pCamera->Getrot().y, pCamera->Getrot().z));
							}

							if (40 < m_nCntWarp && m_nCntWarp < 110)
							{// 一定フレームの間
								m_pos.x += sinf(m_fWarpAngle) * (m_fWarpLenght / (float)(110 - 40));
								m_pos.z += cosf(m_fWarpAngle) * (m_fWarpLenght / (float)(110 - 40));
							}
							
							if (m_nCntWarp == 110)
							{// 一定フレームだったら
								m_pos = m_posWarp;

								pCamera->SetMoveCollision(true);
							}

							m_fcol_a = 0.0f;
							for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
								m_pModel[nCntModel].Setcol_a(m_fcol_a);					// 色設定							
						}
						else if (m_nCntWarp < 150)
						{// 一定フレームまで
							if (m_nCntWarp == 120)
							{// 一定フレームだったら
								m_bMove = true;
							}

							if (m_fcol_a < 1.0f)
							{
								for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
									m_pModel[nCntModel].Setcol_a(m_fcol_a);				// 色設定

								m_fcol_a += (float)(1.0f / (float)30);
							}
						}

						m_nCntWarp++;

						if (m_nCntWarp == 150)
						{// 一定フレームだったら
							m_nCntWarp = 0;
							m_bWarp = false;
						}
					}

					if (pMap_02->CollisionGoal(&m_pos) == true)
					{// ゴールにいるか
						if (fade == CFade::FADE_NONE)
						{// フェードしていない
							if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
							{// アクションを押した
								CGame::GAME game = pGame->GetGame();
								if (game < CGame::GAME_PLAY_B)
								{// ボスより前だったら
									game = (CGame::GAME)(game + 1);
									CFade::Create(game);
									m_bMove = false;

									if (pSound != NULL)
									{// NULL以外の場合
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
									}
								}
							}
						}
					}
				}
				else if (nMapNum == 3)
				{// マップ３
					CMap_03 *pMap_03 = (CMap_03*)pMap;

					if (m_bWarp == false)
						pMap_03->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f);		// マップ当たり判定

					if (m_bWarp == false)
					{// ワープしていない
						bool bWarpIn = false;
						if (pMap_03->CollisionWarp(&m_pos, &m_posWarp) == true)
						{// ワープ内にいるか
							*fHeight = 6.0f;
							bWarpIn = true;
						}

						if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
						{// アクションを押した
							if (bWarpIn == true)
							{
								m_bWarp = true;			// ワープする
								m_bMove = false;		// 移動制限

								if (pSound != NULL)
								{// NULL以外の場合
									pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
								}
							}
						}
					}

					if (m_bWarp == true)
					{// ワープする
						CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);

						if (m_nCntWarp < 30)
						{// 一定フレームまで
							if (0.0f < m_fcol_a)
							{
								for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
									m_pModel[nCntModel].Setcol_a(m_fcol_a);				// 色設定

								m_fcol_a -= (float)(1.0f / (float)30);
							}
						}
						else if (m_nCntWarp < 120)
						{// 一定フレームまで
							if (m_nCntWarp == 30)
							{// 一定フレームだったら
								m_fWarpLenght = sqrtf(powf(m_posWarp.x - m_pos.x, 2.0f) + powf(m_posWarp.z - m_pos.z, 2.0f));
								m_fWarpAngle = atan2f(m_posWarp.x - m_pos.x, m_posWarp.z - m_pos.z);

								pCamera->SetMoveCollision(false);

								pCamera->SetDistance(0, pCamera->GetDistance(0) + 500.0f);
								pCamera->Setrot(D3DXVECTOR3(-0.9f, pCamera->Getrot().y, pCamera->Getrot().z));
							}

							if (40 < m_nCntWarp && m_nCntWarp < 110)
							{// 一定フレームの間
								m_pos.x += sinf(m_fWarpAngle) * (m_fWarpLenght / (float)(110 - 40));
								m_pos.z += cosf(m_fWarpAngle) * (m_fWarpLenght / (float)(110 - 40));
							}

							if (m_nCntWarp == 110)
							{// 一定フレームだったら
								m_pos = m_posWarp;

								pCamera->SetMoveCollision(true);
							}

							m_fcol_a = 0.0f;
							for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
								m_pModel[nCntModel].Setcol_a(m_fcol_a);					// 色設定							
						}
						else if (m_nCntWarp < 150)
						{// 一定フレームまで
							if (m_nCntWarp == 120)
							{// 一定フレームだったら
								m_bMove = true;
							}

							if (m_fcol_a < 1.0f)
							{
								for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
									m_pModel[nCntModel].Setcol_a(m_fcol_a);				// 色設定

								m_fcol_a += (float)(1.0f / (float)30);
							}
						}

						m_nCntWarp++;

						if (m_nCntWarp == 150)
						{// 一定フレームだったら
							m_nCntWarp = 0;
							m_bWarp = false;
						}
					}

					if (pMap_03->CollisionGoal(&m_pos) == true)
					{// ゴールにいるか
						if (fade == CFade::FADE_NONE)
						{// フェードしていない
							if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
							{// アクションを押した
								CGame::GAME game = pGame->GetGame();
								if (game < CGame::GAME_PLAY_B)
								{// ボスより前だったら
									game = (CGame::GAME)(game + 1);
									CFade::Create(game);
									m_bMove = false;

									if (pSound != NULL)
									{// NULL以外の場合
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
									}
								}
							}
						}
					}
				}
				else if (nMapNum == 4)
				{// マップ４
					CMap_04 *pMap_04 = (CMap_04*)pMap;

					if (m_bWarp == false)
						pMap_04->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f);		// マップ当たり判定

					CGame::GAME game = pGame->GetGame();
					if (game == CGame::GAME_TUTORIAL)
					{// チュートリアルだったら
						if (pMap_04->GetGoal() == true)
						{// ゴールがある
							if (pMap_04->CollisionGoal(&m_pos) == true)
							{// ゴールにいたら
								if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
								{// アクションを押した
									if (game < CGame::GAME_PLAY_B)
									{// ボスより前だったら
										game = (CGame::GAME)(game + 1);
										CFade::Create(game);
										m_bMove = false;

										if (pSound != NULL)
										{// NULL以外の場合
											pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// 衝突判定処理									(private)	*** CPlayer ***
//=============================================================================
bool CPlayer::Collision(D3DXVECTOR3 pos)
{
	bool bLand = false;

	float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
	if (fRange < m_vtxMax.x)
	{
		bLand = true;
	}

	return bLand;
}