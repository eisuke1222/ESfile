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
#include "model.h"

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
	{// アニメーション情報
		int bLoop;				// ループ
		int nKeyMax;			// キー最大数
		int nBrendFrame;		// ブレンドフレーム
		KEY_INFO *aKeyInfo;		// キー情報
	}ANIM_INFO;

	CMotion();										// コンストラクタ
	~CMotion();										// デストラクタ

	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(int nModelMax, ANIM_INFO *AnimInfo, CModel *pModel, D3DXVECTOR3 posParent);	// 更新処理
	void Draw(void);								// 描画処理

	void SetAnimNum(int nNum) { if (m_nAnimNextNum != nNum) { m_nAnimNextNum = nNum; m_nAnimKeyNextNum = 0; m_nEditBFrame = -1; m_bEditBFrame = false; } }											// アニメーション設定
	void SetAnimNum(int nNum, int nKeyNum) { if (m_nAnimNextNum != nNum) { m_nAnimNextNum = nNum; m_nAnimKeyNextNum = nKeyNum; m_nEditBFrame = -1; m_bEditBFrame = false; } }						// アニメーション設定（スタートキー設定キー）
	void SetAnimNum(int nNum, int nKeyNum, int nBFrame) { if (m_nAnimNextNum != nNum) { m_nAnimNextNum = nNum; m_nAnimKeyNextNum = nKeyNum; m_nEditBFrame = nBFrame; m_bEditBFrame = true; } }		// アニメーション設定（スタートキー設定キー、ブレンドフレーム一時変更）
	void SetAnimNumrot(int nNum, int nKeyNum) { m_nAnimNextNum = nNum; m_nAnimKeyNextNum = nKeyNum; m_bSetrot = true; }																				// アニメーション向き設定（キーの角度から）
	int GetAnimNum(void) { return m_nAnimNum; }			// アニメーション取得
	int GetAnimKeyNum(void) { return m_nAnimKeyNum; }	// キー取得
	int GetCntFrame(void) { return m_nCntFrame; }		// フレーム取得
	void SetStop(bool bStop) { m_bAnimStop = bStop; }	// アニメーション停止

private:
	int					m_nCntFrame;					// フレームカウンタ
	int					m_nEditBFrame;					// ブレンドフレームカウンタ
	bool				m_bEditBFrame;					// ブレンドフレーム変更
	bool				m_bSetrot;						// 向き指定

	bool				m_bAnimStop;					// 停止
	bool				m_bAnimChange;					// 切り替え
	int					m_nAnimNum;						// アニメーション
	int					m_nAnimNextNum;					// 次アニメーション
	int					m_nAnimKeyNextNum;				// キー
	int					m_nAnimKeyNum;					// 次キー

protected:

};

#endif