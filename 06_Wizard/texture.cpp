//=============================================================================
//
// テクスチャ処理 [texture.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "texture.h"		// テクスチャ
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
LPDIRECT3DTEXTURE9	CTexture::m_pTexture[CTexture::TYPE_MAX] = { NULL };				// テクスチャ情報へのポインタ
int					CTexture::m_pTexType[CTexture::TYPE_MAX] = { CTexture::TYPE_MAX };	// テクスチャ種類
int					CTexture::m_nMaxTexture = 0;										// テクスチャ数

const char			*CTexture::m_TexName[CTexture::TYPE_MAX] =
{// テクスチャ名
	{ "data\\TEXTURE\\MODE\\OTHER\\press2.png" },				// プレス
	{ "data\\TEXTURE\\MODE\\OTHER\\number.png" },				// 番号
	{ "data\\TEXTURE\\MODE\\OTHER\\end.png" },					// 終了確認
	{ "data\\TEXTURE\\MODE\\OTHER\\yesno.png" },				// YESNO
	{ "data\\TEXTURE\\MODE\\OTHER\\button.png" },				// ボタン
	{ "data\\TEXTURE\\MODE\\TITLE\\titlelogo.png" },			// タイトル
	{ "data\\TEXTURE\\MODE\\TITLE\\titlelogo04.png" },			// タイトルエフェクト用
	{ "data\\TEXTURE\\MODE\\TITLE\\menu000.png" },				// タイトルメニュー
	{ "data\\TEXTURE\\MODE\\TITLE\\menu001.png" },				// タイトルメニューBG
	{ "data\\TEXTURE\\MODE\\GAME\\field003.jpg" },				// フィールド
	{ "data\\TEXTURE\\MODE\\GAME\\effect000.jpg" },				// エフェクト
	{ "data\\TEXTURE\\MODE\\GAME\\map2.png" },					// マップ
	{ "data\\TEXTURE\\MODE\\GAME\\mapicon.png" },				// マップアイコン
	{ "data\\TEXTURE\\MODE\\GAME\\sky001.png" },				// 空１
	{ "data\\TEXTURE\\MODE\\GAME\\sky002.jpg" },				// 空２
	{ "data\\TEXTURE\\MODE\\GAME\\grass1.png" },				// 草
	{ "data\\TEXTURE\\MODE\\GAME\\warp000.png" },				// ワープ
	{ "data\\TEXTURE\\MODE\\GAME\\goal000.png" },				// ゴール
	{ "data\\TEXTURE\\MODE\\GAME\\stepanim.png" },				// ステップ
	{ "data\\TEXTURE\\MODE\\GAME\\walk000.png" },				// 歩く
	{ "data\\TEXTURE\\MODE\\GAME\\skill000.png" },				// スキル
	{ "data\\TEXTURE\\MODE\\GAME\\rockon00.png" },				// ロックオンフレーム
	{ "data\\TEXTURE\\MODE\\GAME\\rockon01.png" },				// ロックオンプレイ
	{ "data\\TEXTURE\\MODE\\GAME\\frame.png" },					// UIフレーム
	{ "data\\TEXTURE\\MODE\\GAME\\charicon.png" },				// UIキャラ
	{ "data\\TEXTURE\\MODE\\GAME\\hpsp.png" },					// UIHPSP
	{ "data\\TEXTURE\\MODE\\GAME\\weapon.png" },				// UI武器
	{ "data\\TEXTURE\\MODE\\GAME\\playername.png" },			// UIプレイヤー名
	{ "data\\TEXTURE\\MODE\\GAME\\enemyname.png" },				// UI敵名
	{ "data\\TEXTURE\\MODE\\GAME\\rank.png" },					// UIランク
	{ "data\\TEXTURE\\MODE\\GAME\\check.png" },					// チェック
	{ "data\\TEXTURE\\MODE\\GAME\\tutorialplus.png" },			// チュートリアルプラス
	{ "data\\TEXTURE\\MODE\\GAME\\tutoriallogo.png" },			// チュートリアルロゴ
	{ "data\\TEXTURE\\MODE\\GAME\\tutorial.png" },				// チュートリアル
	{ "data\\TEXTURE\\MODE\\GAME\\clear.png" },					// クリア
	{ "data\\TEXTURE\\MODE\\GAME\\mision.png" },				// ミッション
	{ "data\\TEXTURE\\MODE\\GAME\\skip.png" },					// スキップ
	{ "data\\TEXTURE\\MODE\\GAME\\pause000.png" },				// ポーズロゴ
	{ "data\\TEXTURE\\MODE\\GAME\\pause001.png" },				// ポーズメニュー
	{ "data\\TEXTURE\\MODE\\GAME\\pause002.png" },				// ポーズ背景
	{ "data\\TEXTURE\\MODE\\RESULT\\clear.png" },				// クリア
	{ "data\\TEXTURE\\MODE\\RESULT\\clear_bg.png" },			// クリアエフェクト用
	{ "data\\TEXTURE\\MODE\\RESULT\\over.png" },				// オーバー
	{ "data\\TEXTURE\\MODE\\RESULT\\over_bg.png" },				// オーバーエフェクト用
	{ "data\\TEXTURE\\MODE\\RESULT\\rank_num.png" },			// ランク
};

//=============================================================================
// コンストラクタ									(public)	*** CTexture ***
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// デストラクタ									(public)	*** CTexture ***
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// 全終了処理									(public)	*** CTexture ***
//=============================================================================
void CTexture::ReleaseAll(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// テクスチャ数カウント
		if (m_pTexture[nCount] != NULL)
		{// NULL以外の場合
			m_pTexture[nCount]->Release();		// 解放
			m_pTexture[nCount] = NULL;			// NULLへ
		}
	}

	m_nMaxTexture = 0;		// テクスチャ数
}

//=============================================================================
// テクスチャ取得処理								(public)	*** CTexture ***
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int nNum)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイス取得

	if (nNum < TYPE_MAX)
	{// テクスチャの最大数以下だったら
		if (0 < m_nMaxTexture)
		{// ０以上だったら
			for (int nCount = 0; nCount < m_nMaxTexture; nCount++)
			{// テクスチャ数カウント
				if (m_pTexType[nCount] == nNum)
				{// 番号が一致
					return m_pTexture[nCount];	// 値を返す
				}
			}

			// 一致する番号がない
			D3DXCreateTextureFromFile(pDevice, m_TexName[nNum], &m_pTexture[m_nMaxTexture]);	// テクスチャ読み込み
			m_pTexType[m_nMaxTexture] = nNum;	// 番号設定

			m_nMaxTexture++;	// テクスチャ数カウント

			return m_pTexture[m_nMaxTexture - 1];	// 値を返す
		}
		else
		{// ０以下
			D3DXCreateTextureFromFile(pDevice, m_TexName[nNum], &m_pTexture[m_nMaxTexture]);	// テクスチャ読み込み
			m_pTexType[m_nMaxTexture] = nNum;		// 番号設定

			m_nMaxTexture++;	// テクスチャ数カウント
		}

		return m_pTexture[0];	// 値を返す
	}

	return NULL;	// 値を返す
}