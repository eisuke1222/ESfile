//*****************************************************************************
//
//				タイムの処理[time.h]
//				Auther : Shun Yokomichi
//
//*****************************************************************************
#ifndef _TIME_H_
#define _TIME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"		// シーン

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber2D;		// シーン2D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_NUMBER		(3)		// 桁数の最大数

//*****************************************************************************
// クラス定義(タイム)
//*****************************************************************************
class CTime : public CScene
{
public:
	CTime();
	~CTime();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetTime(void) { return m_nTime; };	// 時間の取得

	// 静的メンバ関数
	static CTime *Create();
	static void SetTime(int nTime);					// Texture設定
	static void AddTime(int nValue);				// タイムの加算
	static int GetTimeNum(void) { return m_nTime; }	// タイム取得

	static void SetTimer(bool bTimer) { m_bTimer = bTimer; }
private:
	int m_nCntTime;					// 時間のカウンター

	// 静的メンバ変数
	static CNumber2D *m_apNumber[MAX_NUMBER];		// CNumberへのポインタ
	static int m_nSecond;			// 時間(秒)の変数
	static int m_nMinute;			// 時間(分)の変数
	static int m_nTime;				// 総合時間の変数

	static bool m_bTimer;
};

#endif // !_TIME_H_

