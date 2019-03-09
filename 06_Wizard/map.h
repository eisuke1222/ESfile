//=============================================================================
//
// マップ処理 [map.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scenemesh.h"	// シーンメッシュ
#include "model.h"		// モデル

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene3D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMap : public CScene
{// マップ（親：CScene）
public:
	const static int m_ROUTEPOS_NUM = 4;
	const static int m_GRASS_NUM = 4;
	const static int m_GRASS_LINE = 3;

	typedef enum
	{// 種類
		TYPE_NO_00 = 0,	// マップ_00
		TYPE_NO_01,		// マップ_01
		TYPE_NO_02,		// マップ_02
		TYPE_NO_03,		// マップ_03
		TYPE_NO_04,		// マップ_04
		TYPE_MAX		// 最大数
	}TYPE;

	typedef enum
	{// エリア
		AREA_START = 0,
		AREA_END,
		AREA_NORMAL,
		AREA_BATTLE,
		AREA_ITEM,
		AREA_MAX
	}AREA;

	typedef struct
	{// エリア範囲
		AREA		nArea;
		D3DXVECTOR3 pos;
		float		fRange;
	}AREA_RANGE;

	typedef struct
	{// ルート範囲
		int nAreaNumStart;
		int nAreaNumEnd;
		D3DXVECTOR3 posCenter;
		D3DXVECTOR3 posStart;
		D3DXVECTOR3 posEnd;
		D3DXVECTOR3 pos[m_ROUTEPOS_NUM];
		D3DXVECTOR3 posCol[m_ROUTEPOS_NUM];
		float		fAngle;
		float		fLenght;
		float		fRange;
	}ROUTE_RANGE;

	typedef struct
	{// ルートパターン
		int nMaxPattern;
		int *nPattern;
		bool bStart;
		bool bEnd;
	}ROUTE_PATTERN;

	typedef struct
	{// ワープ
		int nPattern_1;
		int nPattern_2;
		int nArea_1;
		int nArea_2;
	}WARP;

	typedef struct
	{// 草
		int nCounter;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 posCenter[m_GRASS_NUM][m_GRASS_NUM];
		D3DXVECTOR3 posVtx[m_GRASS_NUM][m_GRASS_NUM][2];
		CScene3D *pGrass[m_GRASS_NUM][m_GRASS_NUM];
		bool bFall[m_GRASS_NUM][m_GRASS_NUM];
		int nFallCounter[m_GRASS_NUM][m_GRASS_NUM];
	}GRASS;

	CMap();												// コンストラクタ
	virtual ~CMap();									// デストラクタ

	static HRESULT Load(void);							// ロード
	static void Unload(void);							// アンロード

	static CMap *Create(TYPE type);						// 生成

	virtual HRESULT Init(void) = 0;						// 初期化処理
	virtual void Uninit(void);							// 終了処理
	virtual void Update(void) = 0;						// 更新処理
	virtual void Draw(void) = 0;						// 描画処理

private:

protected:

};

class CMap_00 : public CMap
{// マップ_00（親：CMap）
public:
	typedef enum
	{// 空
		SKY_NO_0 = 0,	// 空_00
		SKY_NO_1,		// 空_01
		SKY_MAX			// 最大数
	}SKY;

	CMap_00();											// コンストラクタ
	~CMap_00();											// デストラクタ

	static HRESULT Load(void);							// ロード
	static void Unload(void);							// アンロード

	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

private:
	CSceneMesh *m_pField;								// フィールド
	CSceneMesh *m_pSky[SKY_MAX];						// 空

	D3DXVECTOR2 m_SkyRot[SKY_MAX];						// 空回転

protected:

};

class CMap_01 : public CMap
{// マップ_01（親：CMap）
public:
	const static int m_GRASS_MAX = 20;

	typedef enum
	{// 空
		SKY_NO_0 = 0,	// 空_00
		SKY_NO_1,		// 空_01
		SKY_MAX			// 最大数
	}SKY;

	CMap_01();											// コンストラクタ
	~CMap_01();											// デストラクタ

	static HRESULT Load(void);							// ロード
	static void Unload(void);							// アンロード

	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

private:
	static int					m_nMaxModel;			// モデル数

	static LPDIRECT3DTEXTURE9	**m_pTexture;			// テクスチャ情報へのポインタ
	static LPD3DXMESH			*m_pMesh;				// メッシュ情報へのポインタ
	static LPD3DXBUFFER			*m_pBuffMat;			// マテリアル情報へのポインタ
	static DWORD				*m_nNumMat;				// マテリアル情報の数

	static CModel::MODEL_INFO	*m_ModelInfo;			// モデルインフォ

	CModel						*m_pModel;				// モデル

	CSceneMesh					*m_pField;				// フィールド
	CSceneMesh					*m_pSky[SKY_MAX];		// 空

	GRASS						*m_pGrass;				// 草

	D3DXVECTOR2					m_SkyRot[SKY_MAX];		// 空回転

protected:

};

class CMap_02 : public CMap
{// マップ_02（親：CMap）
public:
	const static int m_WARPCYLINDER_NUM = 6;

	typedef enum
	{// 空
		SKY_NO_0 = 0,	// 空_00
		SKY_NO_1,		// 空_01
		SKY_MAX			// 最大数
	}SKY;

	CMap_02();											// コンストラクタ
	~CMap_02();											// デストラクタ

	static HRESULT Load(void);							// ロード
	static void Unload(void);							// アンロード

	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

	void SetField(D3DXVECTOR3 pos);						// フィールド設定
	void SetObject(void);								// オブジェクト設定
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange);
	bool CollisionInOut(D3DXVECTOR3 pos);
	bool CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp);
	bool CollisionGoal(D3DXVECTOR3 *pPos);
	bool CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR);

private:
	static int					m_nMaxModel;			// モデル数

	static LPDIRECT3DTEXTURE9	**m_pTexture;			// テクスチャ情報へのポインタ
	static LPD3DXMESH			*m_pMesh;				// メッシュ情報へのポインタ
	static LPD3DXBUFFER			*m_pBuffMat;			// マテリアル情報へのポインタ
	static DWORD				*m_nNumMat;				// マテリアル情報の数

	static CModel::MODEL_INFO	*m_ModelInfo;			// モデルインフォ

	int							m_nModelWarp;			// モデルワープ数
	CModel						*m_pModelWarp;			// モデルワープ

	int							m_nModelObject;			// モデルオブジェクト数
	CModel						*m_pModelObject;		// モデルオブジェクト

	CSceneMesh					*m_pField;				// フィールド
	CSceneMesh					*m_pSky[SKY_MAX];		// 空

	int							m_nGrassMax;			// 最大草
	GRASS						*m_pGrass;				// 草
	
	D3DXVECTOR2					m_SkyRot[SKY_MAX];		// 空回転

	int							m_nAreaMax;				// エリア最大数
	int							m_nAreaNum[AREA_MAX];	// エリア数
	AREA_RANGE					*m_pArea;				// エリア

	int							m_nRouteMax;				// ルート最大数
	int							m_nRouteNum[AREA_MAX];	// ルート数
	ROUTE_RANGE					*m_pRoute;				// ルート

	int							m_nPatternMax;			// パターン最大数
	ROUTE_PATTERN				*m_pPattern;			// パターン

	WARP						*m_pWarp;				// ワープ
	//CSceneMesh					*m_pWarpCylinder;		// ワープシリンダー
	CSceneMesh					*m_pWarpCylinder[m_WARPCYLINDER_NUM];		// ワープシリンダー
	D3DXVECTOR2					m_WarpCylinderUV;		// ワープシリンダーUV

	CSceneMesh					*m_pGoalCylinder;		// ゴールシリンダー

	bool						m_bDrawIcon;			// アイコン描画
	CScene3D					*m_pIcon;				// アイコン

protected:

};

class CMap_03 : public CMap
{// マップ_03（親：CMap）
public:
	const static int m_WARPCYLINDER_NUM = 6;

	typedef enum
	{// 空
		SKY_NO_0 = 0,	// 空_00
		SKY_NO_1,		// 空_01
		SKY_MAX			// 最大数
	}SKY;

	CMap_03();											// コンストラクタ
	~CMap_03();											// デストラクタ

	static HRESULT Load(void);							// ロード
	static void Unload(void);							// アンロード

	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

	void SetField(D3DXVECTOR3 pos);						// フィールド設定
	void SetObject(void);								// オブジェクト設定
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange);
	bool CollisionInOut(D3DXVECTOR3 pos);
	bool CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp);
	bool CollisionGoal(D3DXVECTOR3 *pPos);
	bool CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR);

private:
	static int					m_nMaxModel;			// モデル数

	static LPDIRECT3DTEXTURE9	**m_pTexture;			// テクスチャ情報へのポインタ
	static LPD3DXMESH			*m_pMesh;				// メッシュ情報へのポインタ
	static LPD3DXBUFFER			*m_pBuffMat;			// マテリアル情報へのポインタ
	static DWORD				*m_nNumMat;				// マテリアル情報の数

	static CModel::MODEL_INFO	*m_ModelInfo;			// モデルインフォ

	int							m_nModelWarp;			// モデルワープ数
	CModel						*m_pModelWarp;			// モデルワープ

	int							m_nModelObject;			// モデルオブジェクト数
	CModel						*m_pModelObject;		// モデルオブジェクト

	CSceneMesh					*m_pField;				// フィールド
	CSceneMesh					*m_pSky[SKY_MAX];		// 空

	int							m_nGrassMax;			// 最大草
	GRASS						*m_pGrass;				// 草

	D3DXVECTOR2					m_SkyRot[SKY_MAX];		// 空回転

	int							m_nAreaMax;				// エリア最大数
	int							m_nAreaNum[AREA_MAX];	// エリア数
	AREA_RANGE					*m_pArea;				// エリア

	int							m_nRouteMax;				// ルート最大数
	int							m_nRouteNum[AREA_MAX];	// ルート数
	ROUTE_RANGE					*m_pRoute;				// ルート

	int							m_nPatternMax;			// パターン最大数
	ROUTE_PATTERN				*m_pPattern;			// パターン

	WARP						*m_pWarp;				// ワープ
	//CSceneMesh					*m_pWarpCylinder;		// ワープシリンダー
	CSceneMesh					*m_pWarpCylinder[m_WARPCYLINDER_NUM];		// ワープシリンダー
	D3DXVECTOR2					m_WarpCylinderUV;		// ワープシリンダーUV

	CSceneMesh					*m_pGoalCylinder;		// ゴールシリンダー

	bool						m_bDrawIcon;			// アイコン描画
	CScene3D					*m_pIcon;				// アイコン

protected:

};

class CMap_04 : public CMap
{// マップ_04（親：CMap）
public:
	const static int m_WARPCYLINDER_NUM = 6;

	typedef enum
	{// 空
		SKY_NO_0 = 0,	// 空_00
		SKY_NO_1,		// 空_01
		SKY_MAX			// 最大数
	}SKY;

	CMap_04();											// コンストラクタ
	~CMap_04();											// デストラクタ

	static HRESULT Load(void);							// ロード
	static void Unload(void);							// アンロード

	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

	void SetField(D3DXVECTOR3 pos);						// フィールド設定
	void SetObject(void);								// オブジェクト設定
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange);
	bool CollisionInOut(D3DXVECTOR3 pos);
	bool CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp);
	bool CollisionGoal(D3DXVECTOR3 *pPos);
	bool CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR);

	void SetGoal(bool bGoal)
		{ m_bGoal = bGoal; }
	bool GetGoal(void)
		{ return m_bGoal; }

private:
	static int					m_nMaxModel;				// モデル数

	static LPDIRECT3DTEXTURE9	**m_pTexture;			// テクスチャ情報へのポインタ
	static LPD3DXMESH			*m_pMesh;				// メッシュ情報へのポインタ
	static LPD3DXBUFFER			*m_pBuffMat;			// マテリアル情報へのポインタ
	static DWORD				*m_nNumMat;				// マテリアル情報の数

	static CModel::MODEL_INFO	*m_ModelInfo;			// モデルインフォ

	int							m_nModelWarp;			// モデルワープ数
	CModel						*m_pModelWarp;			// モデルワープ

	int							m_nModelObject;			// モデルオブジェクト数
	CModel						*m_pModelObject;		// モデルオブジェクト

	CSceneMesh					*m_pField;				// フィールド
	CSceneMesh					*m_pSky[SKY_MAX];		// 空

	int							m_nGrassMax;			// 最大草
	GRASS						*m_pGrass;				// 草

	D3DXVECTOR2					m_SkyRot[SKY_MAX];		// 空回転

	int							m_nAreaMax;				// エリア最大数
	int							m_nAreaNum[AREA_MAX];	// エリア数
	AREA_RANGE					*m_pArea;				// エリア

	int							m_nRouteMax;				// ルート最大数
	int							m_nRouteNum[AREA_MAX];	// ルート数
	ROUTE_RANGE					*m_pRoute;				// ルート

	int							m_nPatternMax;			// パターン最大数
	ROUTE_PATTERN				*m_pPattern;			// パターン

	WARP						*m_pWarp;				// ワープ
	//CSceneMesh					*m_pWarpCylinder;		// ワープシリンダー
	CSceneMesh					*m_pWarpCylinder[m_WARPCYLINDER_NUM];		// ワープシリンダー
	D3DXVECTOR2					m_WarpCylinderUV;		// ワープシリンダーUV

	bool						m_bGoal;				// ゴールできるか
	CSceneMesh					*m_pGoalCylinder;		// ゴールシリンダー

	bool						m_bDrawIcon;			// アイコン描画
	CScene3D					*m_pIcon;				// アイコン

protected:

};

#endif