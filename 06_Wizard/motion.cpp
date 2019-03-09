//=============================================================================
//
// モーション処理 [motion.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "motion.h"			// モーション
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// コンストラクタ										(public)	*** CMotion ***
//=============================================================================
CMotion::CMotion()
{
	// 初期値設定
	m_nCntFrame			= 0;				// フレームカウンタ
	m_nEditBFrame		= -1;				// ブレンドフレームカウンタ
	m_bEditBFrame		= false;			// ブレンドフレーム変更
	m_bSetrot			= false;			// 向き指定

	m_bMotionStop		= false;			// モーション停止
	m_bMotionKeyStop	= false;			// キー停止
	m_bMotionChange		= true;				// 切り替え
	m_nMotionNextNum	= 0;				// モーション番号
	m_nMotionNum		= 0;				// 次モーション番号
	m_nMotionKeyNextNum = 0;				// キー
	m_nMotionKeyNum		= 0;				// 次キー
}

//=============================================================================
// デストラクタ										(public)	*** CMotion ***
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// 初期化処理										(public)	*** CMotion ***
//=============================================================================
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMotion ***
//=============================================================================
void CMotion::Uninit(void)
{

}

//=============================================================================
// 更新処理											(public)	*** CMotion ***
//=============================================================================
void CMotion::Update(int nModelMax, MOTION_INFO *MotionInfo, CModel *pModel)
{
	if (m_nMotionNum != m_nMotionNextNum || m_bSetrot == true)
	{
		m_nCntFrame		= 0;					// フレーム初期化
		m_bMotionChange = true;					// モーション変更した
		m_nMotionNum	= m_nMotionNextNum;		// モーション番号変更
		m_nMotionKeyNum = m_nMotionKeyNextNum;	// モーションキー番号変更
		m_bMotionStop	= false;				// モーション停止解除
		//m_bMotionKeyStop = false;

		if (m_bEditBFrame == false)
		{// ブレンドフレーム変更していない
			m_nEditBFrame = -1;		// ブレンドフレーム初期化
		}

		if (m_bSetrot == true)
		{// 角度変更
			for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
			{// モデル数カウント 
				pModel[0].Setpos(pModel[0].GetInitpos() + MotionInfo[m_nMotionNum].aKeyInfo[(m_nMotionKeyNum) % MotionInfo[m_nMotionNum].nKeyMax].aKey[0].pos);
				pModel[nCntModel].Setrot(MotionInfo[m_nMotionNum].aKeyInfo[(m_nMotionKeyNum) % MotionInfo[m_nMotionNum].nKeyMax].aKey[nCntModel].rot);
			}
			m_nCntFrame = MotionInfo[m_nMotionNum].aKeyInfo[m_nMotionKeyNum].nFrame;	// フレーム変更

			m_bSetrot = false;		// 角度変更解除
		}
	}

	if (m_bMotionStop == false)
	{// モーション停止していない
		D3DXVECTOR3 posDest, posAdd;	// 位置計算用
		D3DXVECTOR3 rotDest, rotAdd;	// 回転計算用
		float		fDiff, fRatio;		// 計算結果反映用

		if (m_bMotionChange == true)
		{// モーション変更した
			if (m_bEditBFrame == false)
			{// ブレンドフレーム変更していない
				fRatio = (MotionInfo[m_nMotionNum].nBrendFrame - m_nCntFrame) * 1.0f;
			}
			else
			{// ブレンドフレーム変更した
				fRatio = (m_nEditBFrame - m_nCntFrame) * 1.0f;
			}
		}
		else
		{// モーション変更していない
			fRatio = (MotionInfo[m_nMotionNum].aKeyInfo[m_nMotionKeyNum].nFrame - m_nCntFrame) * 1.0f;
		}

		/*CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_TEST);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", fDiff);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", Dest.y);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", fRatio);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", pModel[0].Getpos().y);*/

		for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
		{// モデル数カウント
			if (m_bMotionChange == true)
			{// モーション変更した
				// 位置変更
				posDest = MotionInfo[m_nMotionNum].aKeyInfo[0].aKey[nCntModel].pos;
				// 向き変更
				rotDest = MotionInfo[m_nMotionNum].aKeyInfo[0].aKey[nCntModel].rot;
			}
			else
			{// モーション変更していない
				// 次キー番号
				int nNum = (m_nMotionKeyNum + 1) % MotionInfo[m_nMotionNum].nKeyMax;

				if ((m_nMotionKeyNum + 1) % MotionInfo[m_nMotionNum].nKeyMax == 0)
				{// キーが最後まで行った
					if (MotionInfo[m_nMotionNum].bLoop == 0)
						nNum = m_nMotionKeyNum;		// キー調整
				}
				// 位置変更
				posDest = MotionInfo[m_nMotionNum].aKeyInfo[nNum].aKey[nCntModel].pos;
				// 向き変更
				rotDest = MotionInfo[m_nMotionNum].aKeyInfo[nNum].aKey[nCntModel].rot;
			}

			posDest += pModel[nCntModel].GetInitpos();	// 位置調整

			{
				// 差分設定
				fDiff = (posDest.x - pModel[nCntModel].Getpos().x);
				// 現在向き設定
				posAdd.x = fDiff / fRatio;

				// 差分設定
				fDiff = (posDest.y - pModel[nCntModel].Getpos().y);
				// 現在向き設定
				posAdd.y = fDiff / fRatio;

				// 差分設定
				fDiff = (posDest.z - pModel[nCntModel].Getpos().z);
				// 現在向き設定
				posAdd.z = fDiff / fRatio;

				// 位置設定
				pModel[nCntModel].Setpos(pModel[nCntModel].Getpos() + posAdd);
			}

			{
				// 目標向き調整
				if (rotDest.x > D3DX_PI)	{ rotDest.x -= D3DX_PI * 2.0f; }
				if (rotDest.x < -D3DX_PI)	{ rotDest.x += D3DX_PI * 2.0f; }

				// 差分設定
				fDiff = (rotDest.x - pModel[nCntModel].Getrot().x);

				// 差分向き調整
				if (fDiff > D3DX_PI)		{ fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI)		{ fDiff += D3DX_PI * 2.0f; }

				// 現在向き設定
				rotAdd.x = fDiff / fRatio;

				// 目標向き調整
				if (rotDest.y > D3DX_PI)	{ rotDest.y -= D3DX_PI * 2.0f; }
				if (rotDest.y < -D3DX_PI)	{ rotDest.y += D3DX_PI * 2.0f; }

				// 差分設定
				fDiff = (rotDest.y - pModel[nCntModel].Getrot().y);

				// 差分向き調整
				if (fDiff > D3DX_PI)		{ fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI)		{ fDiff += D3DX_PI * 2.0f; }

				// 現在向き設定
				rotAdd.y = fDiff / fRatio;

				// 目標向き調整
				if (rotDest.z > D3DX_PI)	{ rotDest.z -= D3DX_PI * 2.0f; }
				if (rotDest.z < -D3DX_PI)	{ rotDest.z += D3DX_PI * 2.0f; }

				// 差分設定
				fDiff = (rotDest.z - pModel[nCntModel].Getrot().z);

				// 差分向き調整
				if (fDiff > D3DX_PI)		{ fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI)		{ fDiff += D3DX_PI * 2.0f; }

				// 現在向き設定
				rotAdd.z = fDiff / fRatio;
			}

			// 回転設定
			pModel[nCntModel].Setrot(pModel[nCntModel].Getrot() + rotAdd);
		}

		m_nCntFrame++;		// フレームカウント

		if (m_bMotionChange == true)
		{// モーション変更した
			if (m_bEditBFrame == false)
			{// ブレンドフレーム変更していない
				if (m_nCntFrame % MotionInfo[m_nMotionNum].nBrendFrame == 0)
				{// フレームが最後まで行った
					m_nCntFrame		= 0;		// フレーム初期化	
					m_bMotionChange = false;	// モーション変更解除
				}
			}
			else
			{// ブレンドフレーム変更した
				if (m_nCntFrame % m_nEditBFrame == 0)
				{// フレームが最後まで行った
					m_nCntFrame		= 0;		// フレーム
					m_bMotionChange = false;	// モーション変更解除
					m_nEditBFrame	= -1;		// ブレンドフレーム初期化
					m_bEditBFrame	= false;	// ブレンドフレーム変更解除
				}
			}
		}
		else
		{// モーション変更していない
			if (m_nCntFrame % MotionInfo[m_nMotionNum].aKeyInfo[m_nMotionKeyNum].nFrame == 0)
			{// フレームが最後まで行った
				if (m_nMotionKeyNum + 1 == MotionInfo[m_nMotionNum].nKeyMax)
				{// キーが最後まで行った
					if (m_bMotionKeyStop == false)
					{// キー停止していない
						if (MotionInfo[m_nMotionNum].bLoop == 0)
						{// ループしない
							SetMotionNum(0);	// モーション変更
						}

						m_nCntFrame		= 0;	// フレーム初期化		
						m_nMotionKeyNum = 0;	// キー番号初期化
					}
					else
					{// キー停止している
						m_bMotionStop	= true;	// モーション停止
					}
				}
				else
				{// キー再生途中
					m_nCntFrame		= 0;	// フレーム初期化
					m_nMotionKeyNum = (m_nMotionKeyNum + 1) % MotionInfo[m_nMotionNum].nKeyMax;		// キーを進める
				}
			}
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CMotion ***
//=============================================================================
void CMotion::Draw(void)
{

}