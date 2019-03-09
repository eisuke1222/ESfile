//=============================================================================
//
// モーション処理 [motion.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
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
class CMotion
{// モーション
public:
	typedef struct
	{// キー	
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
	}KEY;

	typedef struct
	{// キー情報
		int nFrame;				// フレーム
		KEY *aKey;				// キー
	}KEY_INFO;

	typedef struct
	{// モーション情報
		int bLoop;				// ループ
		int nKeyMax;			// キー最大数
		int nBrendFrame;		// ブレンドフレーム
		KEY_INFO *aKeyInfo;		// キー情報
	}MOTION_INFO;

	CMotion();							// コンストラクタ
	~CMotion();							// デストラクタ

	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
										// 更新処理
	void Update(int nModelMax, MOTION_INFO *MotionInfo, CModel *pModel);	
	void Draw(void);					// 描画処理

	void SetMotionNum(int nNum) 							// モーション設定
		{ if (m_nMotionNextNum != nNum) { m_nMotionNextNum = nNum; m_nMotionKeyNextNum = 0; m_nEditBFrame = -1; m_bEditBFrame = false; } }
	void SetMotionNum(int nNum, int nKeyNum) 				// モーション設定（スタートキー設定キー）
		{ if (m_nMotionNextNum != nNum) { m_nMotionNextNum = nNum; m_nMotionKeyNextNum = nKeyNum; m_nEditBFrame = -1; m_bEditBFrame = false; } }
	void SetMotionNum(int nNum, int nKeyNum, int nBFrame)	// モーション設定（スタートキー設定キー、ブレンドフレーム一時変更）
		{ if (m_nMotionNextNum != nNum) { m_nMotionNextNum = nNum; m_nMotionKeyNextNum = nKeyNum; m_nEditBFrame = nBFrame; m_bEditBFrame = true; } }
	void SetMotionNumrot(int nNum, int nKeyNum) 			// モーション向き設定（キーの角度から）
		{ m_nMotionNextNum = nNum; m_nMotionKeyNextNum = nKeyNum; m_bSetrot = true; }

	int GetMotionNum(void)				// モーション取得
		{ return m_nMotionNum; }	
	int GetMotionKeyNum(void) 			// キー取得
		{ return m_nMotionKeyNum; }
	int GetCntFrame(void) 				// フレーム取得
		{ return m_nCntFrame; }
	void SetStop(bool bStop) 			// モーション停止
		{ m_bMotionStop = bStop; }
	void SetKeyStop(bool bKeyStop) 		// モーションキー停止
		{ m_bMotionKeyStop = bKeyStop; }

private:
	int		m_nCntFrame;				// フレームカウンタ
	int		m_nEditBFrame;				// ブレンドフレームカウンタ
	bool	m_bEditBFrame;				// ブレンドフレーム変更
	bool	m_bSetrot;					// 向き指定

	bool	m_bMotionStop;				// モーション停止
	bool	m_bMotionKeyStop;			// キー停止
	bool	m_bMotionChange;			// 切り替え
	int		m_nMotionNum;				// モーション番号
	int		m_nMotionNextNum;			// 次モーション番号
	int		m_nMotionKeyNextNum;		// キー
	int		m_nMotionKeyNum;			// 次キー

protected:

};

#endif