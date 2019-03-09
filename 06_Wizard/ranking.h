//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン
#include "scene2d.h"	// シーン2D
#include "scene3d.h"	// シーン3D
#include "number.h"		// 番号

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRanking : public CScene
{// ランキング（親：CScene）
public:
	const static int m_RANKING_NUM		= 5;
	const static int m_RANKING_DIGIT	= 6;

	typedef enum
	{
		TYPE_2D = 0,
		TYPE_3D,
		TYPE_MAX
	}TYPE;

	CRanking();									// コンストラクタ
	~CRanking();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード

	static void SaveRanking(void);				// ランキングセーブ
	static void LoadRanking(void);				// ランキングロード

	static CRanking *Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 nextInterval);		// 生成

	HRESULT Init(void) = 0;						// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void) = 0;						// 更新処理
	void Draw(void) = 0;						// 描画処理

	static void SetScore(int nScore)			// スコア設定
		{ m_nScore = nScore; }

private:

protected:
	static int m_nRanking[m_RANKING_NUM];		// ランキング
	static int m_nScore;						// スコア
	static int m_RankInNum;						// ランクイン番号

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
	D3DXCOLOR	m_col;
	D3DXVECTOR3 m_NextInterval;

};

class CRanking_2D : public CRanking
{// ランキング2D（親：CRanking）
public:
	CRanking_2D();								// コンストラクタ
	~CRanking_2D();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	CNumber2D *m_pNumber[m_RANKING_NUM][m_RANKING_DIGIT];	// 番号
	CScene2D *m_pScene2D[m_RANKING_NUM];		// シーン2D

	int m_nCntTimer;							// 時間カウント

protected:

};

#endif