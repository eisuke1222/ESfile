//=============================================================================
//
// 敵処理 [enemy.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン
#include "motion.h"		// モーション

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;			// モデル
class CMotion;			// モーション
class CScene3D;			// シーン3D
class CMapIcon;			// マップアイコン
class CGauge;			// ゲージ

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CScene
{// 敵（親：CScene）
public:
	typedef enum
	{// 種類
		TYPE_N_SLIME = 0,	// スライム
		TYPE_B_SLIME,		// スライム
		TYPE_MAX			// 最大数
	}TYPE;

	typedef enum
	{// 状態
		STATE_NORMAL = 0,		// 通常
		STATE_DAMAGE,			// ダメージ
		STATE_DEATH,			// しんだ
		STATE_MAX				// 最大数
	}STATE;

	CEnemy();										// コンストラクタ
	virtual ~CEnemy();								// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード

													// クリエイト
	static CEnemy *Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	virtual HRESULT Init(void);						// 初期化処理
	virtual void Uninit(void);						// 終了処理
	virtual void Update(void) = 0;					// 更新処理
	virtual void Draw(void) = 0;					// 描画処理

	int GetType(void)								// タイプ番号取得
		{ return m_nType; }

													// 衝突判定処理
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet = false) = 0;
	bool HitDamage(int nDamage);					// ヒットダメージ
	int GetHP(void) { return m_nHP; }				// HP取得
	int GetMAXHP(void) { return m_nMAXHP; }			// MAXHP取得

	void Setpos(D3DXVECTOR3 pos)					// 位置設定
		{ m_pos = pos; }	
	D3DXVECTOR3 Getpos(void)						// 位置取得
		{ return m_pos; }		

	void SetMove(D3DXVECTOR3 move)					// 移動設定
		{ m_move = move; }

	void SetbUpdate(bool bUpdate)					// 更新設定
		{ m_bUpdata = bUpdate; }
	bool GetbUpdate(void)							// 更新取得
		{ return m_bUpdata; }

	void SetKnockBack(bool bKnockBack)				// ノックバック
		{ m_bKnockBack = bKnockBack; }

	void SetLockOn(bool bLockOn)					// ロックオン設定
		{ m_bLockOn = bLockOn; }

private:
	virtual void ColisionChack(float *fHeight) = 0;	// 判定チェック

protected:
	virtual void State(void)	= 0;				// 状態処理
	virtual void Move(void)		= 0;				// 移動処理				
	virtual void Ai(void)		= 0;				// Ai処理				

	int							m_nType;			// タイプ番号
	
	CMapIcon					*m_MapIcon;			// マップアイコン

	bool						m_bUpdata;			// 更新
	bool						m_bMove;			// 移動できるか

	int							m_nHP;				// HP
	int							m_nMAXHP;			// MAXHP

	STATE						m_state;			// 状態
	int							m_nCntState;		// 状態カウント

	D3DXVECTOR3					m_vtxMax;			// 判定MAX
	D3DXVECTOR3					m_vtxMin;			// 判定MIN
	D3DXVECTOR3					m_pos;				// 位置
	D3DXVECTOR3					m_posOld;			// 位置前回
	D3DXVECTOR3					m_rot;				// 向き
	D3DXVECTOR3					m_move;				// 移動量
	float						m_frotDest;			// 目標向き
	bool						m_bJump;			// ジャンプ

	float						m_shadowHeight;		// 影高さ

	bool						m_bKnockBack;		// ノックバック
	int							m_nCntKnockBack;	// ノックバックカウント

	bool						m_bLockOn;			// ロックオン
};

class CSlime : public CEnemy
{// スライム（親：CEnemy）
public:
	const static int m_MAX_HP = 50;				// 体力の最大値

	typedef enum
	{// モデル
		MODEL_00_body = 0,	// 腰
		MODEL_MAX
	}MODEL;	

	typedef enum
	{// AI
		AI_NORMAL = 0,		// 通常
		AI_MAGIC,			// 魔法
		AI_MAX				// 最大数
	}AI;

	CSlime();										// コンストラクタ
	~CSlime();										// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード

	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

													// 衝突判定処理
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet = false);

private:
	void ColisionChack(float *fHeight);				// 判定チェック

	void State(void);								// 状態処理
	void Move(void);								// 移動処理	
	void Ai(void);									// Ai処理	

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

	int							m_AiType;			// AIタイプ
	int							m_nMoveRecast;		// 移動リキャスト
	bool						m_bAttack;			// 攻撃している
	int							m_nCntAttack;		// 攻撃カウント

	bool						m_bAutoMove;		// 自動移動しているか
	int							m_nCntAutoMove;		// 自動移動カウント
	int							m_nMaxAutoMove;		// 自動移動カウント最大
	int							m_nAutoMoveRecast;	// 自動移動リキャスト

protected:

};

class CSlime_King : public CEnemy
{// スライムキング（親：CEnemy）
public:
	const static int m_MAX_HP = 450;				// 体力の最大値

	typedef enum
	{// モデル
		MODEL_00_body = 0,	// 腰
		MODEL_MAX
	}MODEL;

	typedef enum
	{// AI
		AI_NORMAL = 0,		// 通常
		AI_MAGIC,			// 魔法
		AI_DASH,			// ダッシュ
		AI_CREATE,			// クリエイト
		AI_STOP,			// ストップ
		AI_MAX				// 最大数
	}AI;

	CSlime_King();									// コンストラクタ
	~CSlime_King();									// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード

	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

													// 衝突判定処理
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet = false);

private:
	void ColisionChack(float *fHeight);				// 判定チェック

	void State(void);								// 状態処理
	void Move(void);								// 移動処理	
	void Ai(void);									// Ai処理	

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

	int							m_AiType;			// AIタイプ
	int							m_nMoveRecast;		// 移動リキャスト
	bool						m_bAttack;			// 攻撃している
	int							m_nCntAttack;		// 攻撃カウント

	bool						m_bAutoMove;		// 自動移動しているか
	int							m_nCntAutoMove;		// 自動移動カウント
	int							m_nMaxAutoMove;		// 自動移動カウント最大
	int							m_nAutoMoveRecast;	// 自動移動リキャスト

protected:

};

#endif