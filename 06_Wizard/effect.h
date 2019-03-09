//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene2d.h"	// シーン2D
#include "scene3d.h"	// シーン3D
#include "scenemesh.h"	// シーンメッシュ
#include "model.h"		// モデル

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffect : public CScene
{// エフェクト（親：CScene）
public:
	typedef enum
	{// 種類
		TYPE_2D = 0,	// 2D
		TYPE_3D,		// 3D
		TYPE_C00,		// カスタム00
		TYPE_C01,		// カスタム01
		TYPE_C02,		// カスタム02
		TYPE_C03,		// カスタム03
		TYPE_C04,		// カスタム04
		TYPE_FIRE,		// 火
		TYPE_CREATE,	// クリエイト
		TYPE_MAX		// 最大数
	}TYPE;

	typedef enum
	{// 撃つ人
		SHOT_NONE = 0,	// 設定しない
		SHOT_PLAYER,	// プレイヤー
		SHOT_ENEMY,		// 敵
		SHOT_MAX		// 最大数
	}SHOT;

	CEffect();												// コンストラクタ
	virtual ~CEffect();										// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

															// クリエイト
	static CEffect *Create(TYPE type, SHOT shot, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col);

	virtual HRESULT Init(void) = 0;							// 初期化処理
	virtual void Uninit(void);								// 終了処理
	virtual void Update(void) = 0;							// 更新処理
	virtual void Draw(void) = 0;							// 描画処理

private:
	virtual void ColisionChack(void) = 0;					// 判定チェック処理

protected:
	SHOT		m_nShot;									// 撃った人
	D3DXVECTOR3 m_pos;										// 位置
	D3DXVECTOR3 m_rot;										// 回転
	D3DXVECTOR3 m_move;										// 移動量
	D3DXVECTOR3 m_size;										// 大きさ
	D3DXCOLOR	m_col;										// 色

};

class CEffect2D : public CEffect
{// エフェクト2D（親：CEffect）
public:
	CEffect2D();											// コンストラクタ
	~CEffect2D();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

private:
	void ColisionChack(void);								// 判定チェック処理

	CScene2D	*m_pEffect2D;								// エフェクト2D

protected:

};

class CEffect3D : public CEffect
{// エフェクト3D（親：CEffect）
public:
	CEffect3D();											// コンストラクタ
	~CEffect3D();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

private:
	void ColisionChack(void);								// 判定チェック処理

	CScene3D	*m_pEffect3D;								// エフェクト3D

	int			m_nCntTimer;								// 時間カウント
	int         m_nPownTime;								// 生成時間

protected:

};

class CEffect_C00 : public CEffect
{// エフェクト_カスタム00（親：CEffect）
public:
	CEffect_C00();											// コンストラクタ
	~CEffect_C00();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	void SetDeleteTime(int nTime)							// 破棄時間設定
		{ m_DeleteTime = nTime; }

	void SetSizeUp(bool bSizeUp)							// 大きくする
		{ m_bSizeUp = bSizeUp; }

private:
	void ColisionChack(void);								// 判定チェック処理

	CScene3D	*m_pEffect3D;								// エフェクト3D

	int			m_DeleteTime;								// 破棄時間

	bool		m_bSizeUp;									// 大きく

protected:

};

class CEffect_C01 : public CEffect
{// エフェクト_カスタム01（親：CEffect）
public:
	typedef enum
	{// 種類
		EFFECT_ATK = 0,		// 攻撃
		EFFECT_H00,			// エフェクト用0
		EFFECT_H01,			// エフェクト用1
		EFFECT_H02,			// エフェクト用2
		EFFECT_MAX			// 最大数
	}EFFECT;

	CEffect_C01();											// コンストラクタ
	~CEffect_C01();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	void SetExplosion(bool bExplosion)						// 起爆設定
		{ m_bExplosion = bExplosion; }

private:
	void ColisionChack(void);								// 判定チェック処理

	CScene3D	*m_pEffect3D[EFFECT_MAX];					// エフェクト3D
	float		m_frot[EFFECT_MAX];							// エフェクト向き

	int			m_nCntTimer;								// 時間カウント
	int         m_nPownTime;								// 生成時間

	bool		m_bExplosion;								// 起爆

protected:

};

class CEffect_C02 : public CEffect
{// エフェクト_カスタム02（親：CEffect）
public:
	CEffect_C02();											// コンストラクタ
	~CEffect_C02();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

private:
	void ColisionChack(void);								// 判定チェック処理

	CSceneMesh	*m_pSceneMesh;								// シーンメッシュ

protected:

};

class CEffect_C03 : public CEffect
{// エフェクト_カスタム03（親：CEffect）
public:
	CEffect_C03();											// コンストラクタ
	~CEffect_C03();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

private:
	void ColisionChack(void);								// 判定チェック処理

	CSceneMesh	*m_pSceneMesh;								// シーンメッシュ

	int			m_nCntTimer;								// 時間カウント

protected:

};

class CEffect_C04 : public CEffect
{// エフェクト_カスタム04（親：CEffect）
public:
	CEffect_C04();											// コンストラクタ
	~CEffect_C04();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

private:
	void ColisionChack(void);								// 判定チェック処理

	static int						m_nMaxModel;			// モデル数

	static LPDIRECT3DTEXTURE9		**m_pTexture;			// テクスチャ情報へのポインタ
	static LPD3DXMESH				*m_pMesh;				// メッシュ情報へのポインタ
	static LPD3DXBUFFER				*m_pBuffMat;			// マテリアル情報へのポインタ
	static DWORD					*m_nNumMat;				// マテリアル情報の数

	static CModel::MODEL_INFO		*m_ModelInfo;			// モデルインフォ

	CModel							*m_pModel;				// モデル

	int								m_nCntTimer;			// 時間カウント

protected:

};

class CEffect_Fire : public CEffect
{// エフェクト_火（親：CEffect）
public:
	CEffect_Fire();											// コンストラクタ
	~CEffect_Fire();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

private:
	void ColisionChack(void);								// 判定チェック処理

	int			m_nCntTimer;								// 時間カウント

protected:

};

class CEffect_Create : public CEffect
{// エフェクト_クリエイト（親：CEffect）
public:
	CEffect_Create();										// コンストラクタ
	~CEffect_Create();										// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

private:
	void ColisionChack(void);								// 判定チェック処理

	int			m_nCntTimer;								// 時間カウント

protected:

};

#endif