//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン
#include "motion.h"		// モーション
#include "fade.h"		// フェード
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CSceneMesh;		// メッシュ
class CSceneMeshOrbit;	// オービット
class CGauge;			// ゲージ
class CMapIcon;			// マップアイコン
class CGauge;			// ゲージ

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CScene
{// プレイヤー（親：CScene）
public:
	typedef enum
	{// 状態
		STATE_NORMAL = 0,		// 通常
		STATE_DAMAGE,			// ダメージ
		STATE_DEATH,			// しんだ
		STATE_MAX				// 最大数
	}STATE;

	typedef enum
	{// モデル
		MODEL_00_body = 0,		// 腰
		MODEL_01_head,			// 頭
		MODEL_02_hat,			// 帽子
		MODEL_03_none,			// 右腕
		MODEL_04_handR,			// 右手
		MODEL_05_fingerR_1,		// 右手指１
		MODEL_06_fingerR_2,		// 右手指２
		MODEL_07_fingerR_3,		// 右手指３
		MODEL_08_fingerR_4,		// 右手指４
		MODEL_09_none,			// 左腕
		MODEL_10_handL,			// 左手
		MODEL_11_fingerL_1,		// 左手指１
		MODEL_12_fingerL_2,		// 左手指２
		MODEL_13_fingerL_3,		// 左手指３
		MODEL_14_fingerL_4,		// 左手指４
		MODEL_15_none,			// 右腿
		MODEL_16_legR,			// 右足
		MODEL_17_none,			// 左腿
		MODEL_18_legL,			// 左足
		MODEL_MAX				// 最大数
	}MODEL;

	typedef enum
	{// モーション
		MOTION_NEUTRAL = 0,		// ニュートラル
		MOTION_WALK,			// 歩く
		MOTION_JUMP,			// ジャンプ
		MOTION_LANDING,			// 着地
		MOTION_GUARD,			// ガード
		MOTION_DEATH,			// しぬ
		MOTION_DAMAGE_1,		// ダメージ１
		MOTION_DAMAGE_2,		// ダメージ２（吹っ飛び）
		MOTION_RECOVERY_1,		// 起き上がる１
		MOTION_RECOVERY_2,		// 起き上がる２（クイック）
		MOTION_ATTACK_N,		// 通常攻撃（３撃）
		MOTION_ATTACK_S,		// 空中攻撃
		MOTION_ATTACK_SKILL_1,	// スキル攻撃１
		MOTION_ATTACK_SPECIAL_1,// スペシャル攻撃１
		MOTION_MAX				// 最大数
	}MOTION;

	typedef enum
	{// 通常攻撃
		ATTACK_N_1		= 1,	// １撃目キー番号
		ATTACK_N_2		= 3,	// ２撃目キー番号
		ATTACK_N_3		= 6,	// ３撃目キー番号
		ATTACK_SHOT_1	= 1,	// １撃目発射番号
		ATTACK_SHOT_2	= 3,	// ２撃目発射番号
		ATTACK_SHOT_3	= 5,	// ３撃目発射番号
		ATTACK_N_MAX	= 3		// 最大攻撃数
	}ATTACK_N;

	typedef enum
	{
		GUARD_DOME_UP = 0,
		GUARD_DOME_DOWN,
		GUARD_DOME_MAX
	}ORBIT;

	typedef enum
	{
		ORBIT_RIGHT = 0,
		ORBIT_LEFT,
		ORBIT_MAX
	}GUARD_DOME;

	const static int m_MAX_HP = 500;				// HPの最大値
	const static int m_MAX_MP = 100;				// MPの最大値

	CPlayer();										// コンストラクタ
	~CPlayer();										// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード

													// 生成
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

	void Setpos(D3DXVECTOR3 pos)					// 位置設定
		{ m_pos = pos; }	
	D3DXVECTOR3 Getpos(void)						// 位置取得
		{ return m_pos; }		

	D3DXVECTOR3 Getrot(void)						// 角度取得
		{ return m_rot; }	

	D3DXVECTOR3 GetvtxMax(void)						// 大きさ取得
		{ return m_vtxMax; }

	D3DXVECTOR3 Getmove(void)						// 移動量取得
		{ return m_move; }		

	void SetrotDest(float rotDest)					// 目標角度設定
		{ m_frotDest = rotDest; }

	void SetUpdate(bool bUpdate)					// 更新設定
		{ m_bUpdata = bUpdate; }
	bool GetUpdate(void)							// 更新取得
		{ return m_bUpdata; }
	void SetMove(bool bMove)						// 移動設定
		{ m_bMove = bMove; }
	bool GetMove(void)								// 移動取得
		{ return m_bMove; }

	void AddHP(int nValue)
	{// HP
		if (0 <= nValue)
		{
			m_nHP += nValue;
			if (m_MAX_HP < m_nHP) m_nHP = m_MAX_HP;
		}
		else
		{
			CSound *pSound = CManager::GetSound();
			CFade::FADE	fade = CFade::GetFade();	// フェード取得

			if (fade == CFade::FADE_NONE)
			{
				if (m_state == STATE_NORMAL)
				{
					if (m_bStep == false)
					{
						m_state = STATE_DAMAGE;
						m_nHP += (int)((float)nValue * m_fDamageCat);
						if (m_nHP < 0)
						{
							m_state = STATE_DEATH;
							m_nHP = 0;
						}

						if (pSound != NULL) pSound->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE);
					}
				}
			}
		}
	}
	void SetHP(int nHP)
		{ m_nHP = nHP; }
	int GetHP(void)
		{ return m_nHP; }

	void AddMP(int nValue)
	{// MP
		if (0 <= nValue)
		{
			m_nMP += nValue;
			if (100 < m_nMP) m_nMP = 100;
		}
		else
		{
			m_nMP += nValue;
			if (m_nMP < 0)
			{
				AddHP(m_nMP);
				m_nMP = 0;
				if (m_nHP < 0) m_nHP = 0;
			}
		}
	}
	void SetMP(int nMP)
		{ m_nMP = nMP; }
	int GetMP(void)
		{ return m_nMP; }

	void SetAction(bool bNotAttack, bool bNotSkill)
		{ m_bNotAttack = bNotAttack; m_bNotSkill = bNotSkill; }

	bool Collision(D3DXVECTOR3 pos);

private:
	void Title(void);								// タイトル処理
	void State(void);								// 状態処理
	void Move(void);								// 移動処理
	void Step(void);								// ステップ処理
	void Action(void);								// アクション処理
	void Jump(void);								// ジャンプ処理
	void ColisionChack(float *fHeight);				// 判定チェック処理

	static int					m_MaxModel;			// モデル数
	static int					m_MaxMotion;		// モーション数

	static LPDIRECT3DTEXTURE9	**m_pTexture;		// テクスチャ情報へのポインタ
	static LPD3DXMESH			*m_pMesh;			// メッシュ情報へのポインタ
	static LPD3DXBUFFER			*m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD				*m_nNumMat;			// マテリアル情報の数

	static CModel::MODEL_INFO	*m_ModelInfo;		// モデルインフォ
	static CMotion::MOTION_INFO	*m_MotionInfo;		// モーションインフォ

	D3DXMATRIX					m_mtxWorld;			// ワールドマトリックス
	CModel						*m_pModel;			// モデル
	CMotion						*m_pMotion;			// モーション

	CMapIcon					*m_MapIcon;			// マップアイコン

	bool						m_bTutorialSet;		// チュートリアル設定
	int							m_nCntTutorialSet;	// チュートリアル設定カウント

	CSceneMeshOrbit				*m_pOrbit[ORBIT_MAX];	// オービット

	bool						m_bWarp;			// ワープしているか
	int							m_nCntWarp;			// ワープカウント
	float						m_fcol_a;			// アルファ色
	D3DXVECTOR3					m_posWarp;			// ワープ位置
	float						m_fWarpLenght;
	float						m_fWarpAngle;

	bool						m_bUpdata;			// 更新
	bool						m_bDraw;			// 描画
	bool						m_bMove;			// 移動できるか

	int							m_nHP;				// HP
	int							m_nMP;				// MP

	STATE						m_state;			// 状態
	int							m_nCntState;		// 状態カウント

	int							m_nHatNum;			// 帽子番号
	int							m_nCntTimer;		// タイマーカウント

	D3DXVECTOR3					m_vtxMax;			// 判定MAX
	D3DXVECTOR3					m_vtxMin;			// 判定MIN
	D3DXVECTOR3					m_pos;				// 位置
	D3DXVECTOR3					m_posOld;			// 位置前回
	D3DXVECTOR3					m_rot;				// 向き
	D3DXVECTOR3					m_move;				// 移動量
	float						m_frotDest;			// 目標向き
	bool						m_bJump;			// ジャンプ
	int							m_nAttackCnt;		// 攻撃カウント
	int							m_nAttackNum;		// 攻撃入力数
	bool						m_bAttack_Nor;		// 通常攻撃
	bool						m_bAttack_Sky;		// 空中攻撃
	bool						m_bAction_Sky;		// 空中アクション
	int							m_nAttackRecast;	// 攻撃リキャスト
	int							m_nGuardCnt;		// ガードカウント
	bool						m_bGuard;			// ガード
	int							m_nCntJastGuard;	// ジャストガードカウント
	float						m_fDamageCat;		// ダメージカット
	int							m_nStepCnt;			// ステップカウント
	bool						m_bStep;			// ステップ
	bool						m_bSkill;			// スキル
	int							m_nSkillRecast;		// スキルリキャスト

	bool						m_brot;				// 回転可能か

	float						m_shadowHeight;		// 影高さ

	bool						m_bNotAttack;		// 攻撃禁止
	bool						m_bNotSkill;		// スキル禁止

	CSceneMesh					*m_pGuardDome[GUARD_DOME_MAX];	// ガードドーム
	D3DXVECTOR2					m_GuardDomeUV;		// ガードドームUV

protected:

};

#endif