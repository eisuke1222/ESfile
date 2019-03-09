//=============================================================================
//
// タイトル処理 [title.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// タイトル
#include "manager.h"	// マネージャー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_OBJ_MAX (1800)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRanking;
class CPress;
class CMeshField;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitle
{// タイトル
public:
	typedef struct
	{
		int				nCnter;					//	オブジェクトの数を数える
		D3DXVECTOR3		pos[TITLE_OBJ_MAX];			//	オブジェクトポジションの設定
		int				nType[TITLE_OBJ_MAX];			//	オブジェクト種類の設定
		int				ncolType[TITLE_OBJ_MAX];		//	オブジェクト判定の設定
		D3DXVECTOR3		size[TITLE_OBJ_MAX];			//	オブジェクトサイズの設定
		bool			bUse[TITLE_OBJ_MAX];			// 使ったか
	}TEXTSTATE;

	typedef enum
	{
		TYPE_NONE = 0,	// 何もない状態
		TYPE_SCENEN000,	// 草をかき分ける
		TYPE_SCENEN001,	// タイトルロゴ表示
		TYPE_SCENEN002,	// ランキング表示
		TYPE_SCENE003,	// クレジット表示
		TYPE_MAX		// 総数
	}TYPE;
	CTitle();						// コンストラクタ
	~CTitle();						// デストラクタ

	static HRESULT Load(void);		// ロード
	static void Unload(void);		// アンロード

	void SetStage(void);

	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理
	void ObjectLoad(void);			// オブジェクトの読み込み

private:

protected:
	int		m_nType;
	float	m_fColA;		// カラー(α値)
	int		m_nCounter;		// 時間
	TYPE	m_type;			// タイプの管理
	CPress *m_pPress;

	// 静的メンバ変数
	static CRanking *m_pRanking;		// ランキング
	static CMeshField *m_pMeshField[8];	// メッシュフィールド
	TEXTSTATE m_textState;		//	テキスト読み込む変数[読み込むオブジェクトの種類]
};

#endif