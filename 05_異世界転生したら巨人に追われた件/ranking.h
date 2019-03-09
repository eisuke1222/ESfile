//*****************************************************************************
//
//				ランキングの処理[ranking.h]
//				Auther : Shun Yokomichi
//
//*****************************************************************************
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"		// シーン
#include "score.h"		// スコア

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;			// 数字
class CRnkingLogo;		// ランキングロゴ
class CUiPolygon;				// UI

//*****************************************************************************
//		マクロ定義
//*****************************************************************************
#define MAX_RANKING		(5)		// 順位
#define MAX_RANK_LOGO	(6)		// ランキングロゴ

//*****************************************************************************
//		クラス定義(ランキング)
//*****************************************************************************
class CRanking
{
public:
	CRanking();
	~CRanking();

	HRESULT Init(D3DXVECTOR3 pos);		// 初期化
	void	Uninit(void);				// 終了処理
	void	Update(void);				// 更新処理
	void	Draw(void);					// 描画処理
	static void SaveRanking(int nAdd = 0);			// ランキングのセーブ
	static void LoadRanking();			// ランキングの読み込み

	// 静的メンバ変数
	static CRanking *Create(D3DXVECTOR3 pos);
	static int GetPattern(void) { return m_Pattern; };
	static void SetPattern(int nPattern) { m_Pattern = nPattern; };

private:
	int		m_nTime;
	float   m_fColA;
	int		m_nSize;

	// 静的メンバ変数
	static int m_Pattern;
	static CNumber2D *m_aRanking[MAX_RANKING][MAX_SCORE];
	static int m_nScore[MAX_RANKING];
	static CRnkingLogo *m_pLogo[MAX_RANK_LOGO];
	static CUiPolygon *m_apUi[MAX_RANKING];
	static int m_nNum;

};

#endif // !_RANKING_H_
