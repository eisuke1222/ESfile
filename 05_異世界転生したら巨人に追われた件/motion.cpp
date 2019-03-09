//=============================================================================
//
// モーション処理 [motion.h]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "motion.h"			// モーション
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー

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
// コンストラクタ								(public)	*** CMotion ***
//=============================================================================
CMotion::CMotion()
{
	// 初期値設定
	m_nCntFrame = 0;							// フレームカウンタ
	m_nEditBFrame = -1;							// ブレンドフレーム編集用
	m_bEditBFrame = false;						// ブレンドフレーム編集
	m_bSetrot = false;							// 角度編集

	m_bAnimStop = false;						// モーション停止
	m_bAnimChange = true;						// モーション変更
	m_nAnimNextNum = 0;							// 次モーション番号
	m_nAnimNum = 0;								// モーション番号
	m_nAnimKeyNextNum = 0;						// 次キー番号
	m_nAnimKeyNum = 0;							// キー番号
}

//=============================================================================
// デストラクタ									(public)	*** CMotion ***
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// 初期化処理									(public)	*** CMotion ***
//=============================================================================
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CMotion ***
//=============================================================================
void CMotion::Uninit(void)
{

}

//=============================================================================
// 更新処理										(public)	*** CMotion ***
//=============================================================================
void CMotion::Update(int nModelMax, ANIM_INFO *AnimInfo, CModel *pModel, D3DXVECTOR3 posParent)
{
	if (m_nAnimNum != m_nAnimNextNum || m_bSetrot == true)
	{// モーション編集
		m_nCntFrame = 0;					// フレームカウント
		m_bAnimChange = true;				// モーション変更
		m_nAnimNum = m_nAnimNextNum;		// モーション変更
		m_nAnimKeyNum = m_nAnimKeyNextNum;	// キー変更

		if (m_bEditBFrame == false)
		{// ブレンドフレーム編集していない
			m_nEditBFrame = -1;
		}

		if (m_bSetrot == true)
		{// 角度変更
			// 位置
			pModel[0].Setpos(posParent + AnimInfo[m_nAnimNum].aKeyInfo[(m_nAnimKeyNum) % AnimInfo[m_nAnimNum].nKeyMax].aKey[0].pos);

			for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
			{// カウント
				// 角度
				pModel[nCntModel].Setrot(AnimInfo[m_nAnimNum].aKeyInfo[(m_nAnimKeyNum) % AnimInfo[m_nAnimNum].nKeyMax].aKey[nCntModel].rot);
			}
			// フレーム
			m_nCntFrame = AnimInfo[m_nAnimNum].aKeyInfo[m_nAnimKeyNum].nFrame;

			// 角度
			m_bSetrot = false;
		}
	}

	if (m_bAnimStop == false)
	{// モーションする
		D3DXVECTOR3 Dest, Add;	// 目標、加算
		float fDiff, frac;		// 計算用

		if (m_bAnimChange == true)
		{// モーション変更
			if (m_bEditBFrame == false)
			{// ブレンドフレーム変更していない
				frac = (AnimInfo[m_nAnimNum].nBrendFrame - m_nCntFrame) * 1.0f;
			}
			else
			{// ブレンドフレーム変更する
				frac = (m_nEditBFrame - m_nCntFrame) * 1.0f;
			}
		}
		else
		{// その他
			frac = (AnimInfo[m_nAnimNum].aKeyInfo[m_nAnimKeyNum].nFrame - m_nCntFrame) * 1.0f;
		}

		if (m_bAnimChange == true)
		{// モーション変更
			Dest = posParent + AnimInfo[m_nAnimNum].aKeyInfo[0].aKey[0].pos;
		}
		else
		{// その他
			Dest = posParent + AnimInfo[m_nAnimNum].aKeyInfo[(m_nAnimKeyNum + 1) % AnimInfo[m_nAnimNum].nKeyMax].aKey[0].pos;
		}

		// 差分設定
		fDiff = (Dest.x - pModel[0].Getpos().x);
		// 現在向き設定
		Add.x = fDiff / frac;

		// 差分設定
		fDiff = (Dest.y - pModel[0].Getpos().y);
		// 現在向き設定
		Add.y = fDiff / frac;

		// 差分設定
		fDiff = (Dest.z - pModel[0].Getpos().z);
		// 現在向き設定
		Add.z = fDiff / frac;

		pModel[0].Setpos(pModel[0].Getpos() + Add);

		/*CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_TEST);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", fDiff);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", Dest.y);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", frac);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", pModel[0].Getpos().y);*/

		for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
		{// カモデル数ウント
			if (m_bAnimChange == true)
			{// モーション変更
				// 向き変更
				Dest = AnimInfo[m_nAnimNum].aKeyInfo[0].aKey[nCntModel].rot;
			}
			else
			{// その他
				// 向き変更
				Dest = AnimInfo[m_nAnimNum].aKeyInfo[(m_nAnimKeyNum + 1) % AnimInfo[m_nAnimNum].nKeyMax].aKey[nCntModel].rot;
			}

			{
				// 目標向き調整
				if (Dest.x > D3DX_PI) { Dest.x -= D3DX_PI * 2.0f; }
				if (Dest.x < -D3DX_PI) { Dest.x += D3DX_PI * 2.0f; }

				// 差分設定
				fDiff = (Dest.x - pModel[nCntModel].Getrot().x);

				// 差分向き調整
				if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

				// 現在向き設定
				Add.x = fDiff / frac;

				// 目標向き調整
				if (Dest.y > D3DX_PI) { Dest.y -= D3DX_PI * 2.0f; }
				if (Dest.y < -D3DX_PI) { Dest.y += D3DX_PI * 2.0f; }

				// 差分設定
				fDiff = (Dest.y - pModel[nCntModel].Getrot().y);

				// 差分向き調整
				if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

				// 現在向き設定
				Add.y = fDiff / frac;

				// 目標向き調整
				if (Dest.z > D3DX_PI) { Dest.z -= D3DX_PI * 2.0f; }
				if (Dest.z < -D3DX_PI) { Dest.z += D3DX_PI * 2.0f; }

				// 差分設定
				fDiff = (Dest.z - pModel[nCntModel].Getrot().z);

				// 差分向き調整
				if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

				// 現在向き設定
				Add.z = fDiff / frac;
			}

			// 角度変更
			pModel[nCntModel].Setrot(pModel[nCntModel].Getrot() + Add);
		}

		m_nCntFrame++;		// フレームカウント

		if (m_bAnimChange == true)
		{// モーション変更
			if (m_bEditBFrame == false)
			{// ブレンドフレーム変更していない
				if (m_nCntFrame % AnimInfo[m_nAnimNum].nBrendFrame == 0)
				{// 一定フレームになったら
					m_nCntFrame = 0;			// フレーム初期化
					m_bAnimChange = false;		// モーション変更終了
				}
			}
			else
			{// ブレンドフレーム変更
				if (m_nCntFrame % m_nEditBFrame == 0)
				{// 一定フレームになったら
					m_nCntFrame = 0;			// フレーム初期化
					m_bAnimChange = false;		// モーション変更
					m_nEditBFrame = -1;			// ブレンドフレーム変更
					m_bEditBFrame = false;
				}
			}
		}
		else
		{// モーション続ける
			if (m_nCntFrame % AnimInfo[m_nAnimNum].aKeyInfo[m_nAnimKeyNum].nFrame == 0)
			{// 一定フレームになったら
				if (m_nAnimKeyNum + 1 == AnimInfo[m_nAnimNum].nKeyMax)
				{// キーが最終キーになったら
					if (AnimInfo[m_nAnimNum].bLoop == 0)
					{// ループしない
						m_nAnimNextNum = 0;	// ０番モーションへ
					}

					m_nAnimKeyNum = 0;		// キー番号初期化
					m_nCntFrame = 0;		// フレーム初期化
				}
				else
				{// その他
					m_nAnimKeyNum = (m_nAnimKeyNum + 1) % AnimInfo[m_nAnimNum].nKeyMax;		// キー変更
					m_nCntFrame = 0;		// フレーム初期化
				}
			}
		}
	}
}

//=============================================================================
// 描画処理										(public)	*** CMotion ***
//=============================================================================
void CMotion::Draw(void)
{

}