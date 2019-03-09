//=============================================================================
//
// シーン処理 [scene.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCENE	(1500)		// シーンの最大数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene
{// シーン
public:
	typedef enum
	{// プライオリティ
		PRIORITY_BG = 0,		// 背景
		PRIORITY_1,				// 1
		PRIORITY_2,				// 2
		PRIORITY_3,				// 3
		PRIORITY_4,				// 4
		PRIORITY_5,				// 5
		PRIORITY_UI,			// UI
		PRIORITY_PAUSE,			// ポーズ
		PRIORITY_FADE,			// フェード
		PRIORITY_MAX
	}PRIORITY;

	typedef enum
	{// オブジェクトタイプ
		OBJTYPE_NONE = 0,		// 何もしていない
		OBJTYPE_MESHFIELD,		// メッシュフィールド
		OBJTYPE_MESHWALL,		// メッシュウォール
		OBJTYPE_MESHCYLINDER,	// メッシュシリンダー
		OBJTYPE_OBJ3D,			// オブジェクト
		OBJTYPE_EFFECT3D,		// エフェクト3D
		OBJTYPE_BG,				// 背景
		OBJTYPE_ENEMY,			// 敵
		OBJTYPE_PLAYER,			// プレイヤー
		OBJTYPE_BULLET,			// 弾
		OBJTYPE_EFFECT,			// エフェクト
		OBJTYPE_EXPLOSION,		// 爆発
		OBJTYPE_CLOUD,			// 雲
		OBJTYPE_BOMB,			// 爆弾
		OBJTYPE_STAR,			// 惑星
		OBJTYPE_GAUGE,			// ゲージ
		OBJTYPE_LIFE,			// 体力
		OBJTYPE_SCORE,			// スコア
		OBJTYPE_RANKING,		// ランキング
		OBJTYPE_MENU,			// メニュー
		OBJTYPE_PLAYERNUM,		// プレイヤー番号
		OBJTYPE_TITLEPLAYER,	// タイトルプレイヤー
		OBJTYPE_PAUSE,			// ポーズ
		OBJTYPE_PRESS,			// プレス
		OBJTYPE_TITLE,			// タイトル
		OBJTYPE_RESULT,			// リザルト
		OBJTYPE_FADE,			// フェード
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority);						// コンストラクタ
	CScene(int nPriority, int nID);				// コンストラクタ
	virtual ~CScene();							// デストラクタ

	static void ReleaseAll(void);				// 全破棄
	static void UpadteAll(void);				// 全更新
	static void DrawAll(void);					// 全描画

	virtual HRESULT Init(void) = 0;				// 初期化処理
	virtual void Uninit(void) = 0;				// 終了処理
	virtual void Update(void) = 0;				// 更新処理
	virtual void Draw(void) = 0;				// 描画処理

	static int GetNumAll(void);					// 総数取得

	int GetID(void) { return m_nID; }			// ID取得

	void SetObjType(OBJTYPE objType)	{ m_objType = objType; }		// オブジェクトタイプ設定
	OBJTYPE GetObjType(void)			{ return m_objType; }			// オブジェクトタイプ取得

	static CScene *GetScene(int nPri, int nIdx)	{ return m_apScene[nPri][nIdx]; }		// シーン取得

	static void SetStartUp(int nStartUp) { m_nStartUp = nStartUp; }		// 更新設定

private:
	static CScene *m_apScene[PRIORITY_MAX][MAX_SCENE];	// シーン
	static int m_nNumAll;						// 総数

	int m_nPriority;							// 種類
	int m_nID;									// ID番号
	OBJTYPE m_objType;							// 種類

	static int m_nStartUp;						// 更新開始番号

protected:
	void Release(void);							// 開放処理

};

#endif