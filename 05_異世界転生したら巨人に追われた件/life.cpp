//**************************************************************
//
//				ライフの処理[life.cpp]
//				Auther : Shun Yokomichi
//
//**************************************************************
#include "life.h"			// ライフ
#include "manager.h"		// マネージャー
#include "game.h"			// ゲーム
#include "player.h"			//プレイヤー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "fade.h"			// フェード
#include "sound.h"			//サウンド

//**************************************************************
//				マクロ定義
//**************************************************************
#define LIFE_TEXTURENAME000		"data/TEXTURE/game/LifeCore.png"	//読み込むテクスチャ000
#define LIFE_SIZE_X				(15.0f)								// Xの大きさ(横幅)
#define LIFE_SIZE_Y				(15.0f)								// Yの大きさ(縦幅)
#define POS_X					(145.0f)							// 位置座標(X軸)
#define POS_Y					(70.0f)								// 位置座標(Y軸)
#define POS_X_INTERVAL			(36.0f)								// 距離間隔

//**************************************************************
//				静的メンバ変数
//**************************************************************
LPDIRECT3DTEXTURE9	CLife::m_pTexture = NULL;						// テクスチャ情報へのポインタ
bool				CLife::m_bUse[MAX_LIFE] = {true, true, true };	// 使う
int					CLife::m_nLife = MAX_LIFE;						// 体力

//**************************************************************
//				ライフの生成
//**************************************************************
CLife *CLife::Create()
{
	CLife *pLife = NULL;	// ポインタ

	// シーンの生成
	if (pLife == NULL)
	{// NULLの場合
		pLife = new CLife;	// メモリ確保

		if (pLife != NULL)
		{// NULL以外の場合
			pLife->Init();	// 初期化処理
		}
	}

	return pLife;
}

//**************************************************************
//				読み込むテクスチャ
//**************************************************************
HRESULT CLife::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, LIFE_TEXTURENAME000, &m_pTexture);

	return S_OK;
}

//**************************************************************
//				開放するテクスチャ
//**************************************************************
void CLife::Unload(void)
{
	if (m_pTexture != NULL)
	{// NULL以外の場合
		m_pTexture->Release();		// 開放
		m_pTexture = NULL;			// NULLへ
	}
}

//**************************************************************
//		コンストラクタ
//**************************************************************
CLife::CLife() : CScene(PRIORITY_UI, OBJTYPE_NONE)
{
	// デフォルト値
	m_nLife = MAX_LIFE -1;
}

//**************************************************************
//		デストラクタ
//**************************************************************
CLife::~CLife()
{

}

//**************************************************************
//		初期化処理
//**************************************************************
HRESULT CLife::Init(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{// カウント
		m_apScene[nCntLife] = new CScene2D(PRIORITY_UI, OBJTYPE_NONE);		// メモリ確保

		if (m_apScene[nCntLife] != NULL)
		{	// NULLチェック
			m_apScene[nCntLife]->SetInitAll(D3DXVECTOR3(POS_X + (nCntLife * POS_X_INTERVAL), POS_Y, 0.0f), D3DXVECTOR3(LIFE_SIZE_X, LIFE_SIZE_Y, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_apScene[nCntLife]->Init();
			m_apScene[nCntLife]->BindTexture(m_pTexture);
		}
	}

	// 変数の初期化
	m_nLife = MAX_LIFE - 1;
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{// カウント
		m_bUse[nCntLife] = true;
	}

	return S_OK;
}

//**************************************************************
//		終了処理
//**************************************************************
void CLife::Uninit(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{// カウント
		if (m_apScene[nCntLife] != NULL)
		{	// NULLチェック
			m_apScene[nCntLife]->Uninit();
			m_apScene[nCntLife] = NULL;
		}
	}

	CScene::SetDeath();		// 死亡フラグ
}

//**************************************************************
//		更新処理
//**************************************************************
void CLife::Update(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{// カウント
		if (m_bUse[nCntLife] == false)
		{// 使ってない
			m_apScene[nCntLife]->Setcol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}
		else
		{// 使う
			m_apScene[nCntLife]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}


	if (CFade::GetFade() == CFade::FADE_NONE && m_nLife < 0)
	{// フェードしていない状態(リザルト画面へ)
		if (CPlayer::GetFalling() == true)
		{// 落ちた
			CFade::Create(CManager::MODE_RESULT, CFade::FADE_OUT);
			CGame::SetGame(CGame::MODE_GAMEOVER001);				// 落下ED

		}
		else
		{// 落ちていない
			CFade::Create(CManager::MODE_RESULT, CFade::FADE_OUT);
			CGame::SetGame(CGame::MODE_GAMEOVER000);				// ライフが０のゲームオーバー
		}
	}
}

//**************************************************************
//		描画処理
//**************************************************************
void CLife::Draw(void)
{

}

//**************************************************************
//		ダメージ処理
//**************************************************************
void CLife::HitDamage()
{
	// サウンド
	CSound *pSound = CManager::GetSound();

	if (m_bUse[m_nLife] == true)
	{// 使っている
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE018);
		m_bUse[m_nLife] = false;	// 使っていない
		m_nLife--;	// 減らす
		return;
	}
}

//**************************************************************
//		回復処理
//**************************************************************
void CLife::Recovery()
{
	if (m_nLife < 2)
	{// 一定以下
		m_nLife++;	// 増やす

		if (m_bUse[m_nLife] == false)
		{// 使っていない
			m_bUse[m_nLife] = true;	// 使う
			return;
		}
	}
}