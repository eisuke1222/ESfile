//=============================================================================
//
// スコア処理 [score.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_MAX_DIGIT		(6)			// スコア桁数
#define SCORE_SIZE_X		(10)		// 大きさ（X)
#define SCORE_SIZE_Y		(10)		// 大きさ（Y)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber2D;		// 数字2D

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScore : public CScene
{// 背景（親：CScene）
public:
	CScore();									// コンストラクタ
	~CScore();									// デストラクタ

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nScoreNum);		// 生成

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	static void AddScore(int nScoreNum, int nScore) { m_nScore[nScoreNum] += nScore; }			// スコア加算
	static int GetScore(int nScoreNum) { return m_nScore[nScoreNum]; }							// スコア取得

private:
	int m_nScoreNum;							// スコア番号

	CNumber2D *m_apNumber2D[SCORE_MAX_DIGIT];				// 数字2D
	static int m_nScore[MAX_PLAYER];						// スコア

protected:

};

#endif