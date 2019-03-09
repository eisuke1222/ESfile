//*****************************************************************************
//
//				ポーズの処理[pause.cpp]
//				Auther : Shun Yokomichi
//
//*****************************************************************************

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "input.h"		// 入力
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "scene2D.h"	// シーン2D
#include "pause.h"		// ポーズ
#include "fade.h"		// フェード
#include "pausebg.h"	// ポーズ背景
#include "press.h"		// プレスボタン
#include "sound.h"		// サウンド
#include "game.h"		// ゲーム

//*****************************************************************************
//	マクロ定義
//*****************************************************************************
#define TEXTURE_NAME000	"data/Texture/pause/continue.png"	// 読み込むテクスチャ000
#define TEXTURE_NAME001	"data/Texture/pause/retry.png"		// 読み込むテクスチャ001
#define TEXTURE_NAME002	"data/Texture/pause/Quit.png"		// 読み込むテクスチャ002
#define COL_A			(0.009f)							// 透明度の変化量
#define COL_A_MAX		(0.65f)								// 透明度の限度量
#define POS_X_INTERVAL	(150.0f)							// X座標の間隔
#define PRESS_BUTTON_X	(50.0f)								// ボタンのサイズ(X軸)
#define PRESS_BUTTON_Y	(50.0f)								// ボタンのサイズ(Y軸)
#define PRESS_INTERVAL	(200.0f)							// メニューとの距離

//*****************************************************************************
//				静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPause::m_pTexture[POLYGON_PAUSE] = {};

//*****************************************************************************
//				生成処理
//*****************************************************************************
CPause *CPause::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 UV)
{
	CPause *pPause = NULL;

	// シーンの生成
	if (pPause == NULL)
	{
		pPause = new CPause;

		if (pPause != NULL)
		{
			pPause->Init(pos, size, col, UV);
		}
	}
	return pPause;
}
//*****************************************************************************
//				ロード
//*****************************************************************************
HRESULT CPause::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	for (int nCntTex = 0; nCntTex < POLYGON_PAUSE; nCntTex++)
	{
		if (m_pTexture[nCntTex] == NULL)
		{	// NULLなら
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME000, &m_pTexture[0]);
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME001, &m_pTexture[1]);
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME002, &m_pTexture[2]);
		}
	}

	CPress::Load();

	return S_OK;
}
//*****************************************************************************
//				アンロード
//*****************************************************************************
void CPause::Unload(void)
{
	for (int nCntTex = 0; nCntTex < POLYGON_PAUSE; nCntTex++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}

	CPress::Unload();	// textureの破棄
}
//*****************************************************************************
//				コンストラクタ
//*****************************************************************************
CPause::CPause()
{
	for (int nCntPause = 0; nCntPause < POLYGON_PAUSE; nCntPause++)
	{
		m_apScene2D[nCntPause] = NULL;
	}
	m_pPauseBg = NULL;

	for (int nCntPause = 0; nCntPause < POLYGON_PAUSE; nCntPause++)
	{
		m_pPress[nCntPause] = NULL;
	}
}
//*****************************************************************************
//				デストラクタ
//*****************************************************************************
CPause::~CPause()
{
}

//*****************************************************************************
//				初期化処理
//*****************************************************************************
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 UV)
{
	if (m_pPauseBg == NULL)
	{	// NULLなら
		m_pPauseBg = new CPauseBg;
	}
	if (m_pPauseBg != NULL)
	{	// NULLじゃないなら
		m_pPauseBg->Init();
	}

	for (int nCntPause = 0; nCntPause < POLYGON_PAUSE; nCntPause++)
	{
		m_apScene2D[nCntPause] = new CScene2D(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_MENU);
		if (m_apScene2D[nCntPause] != NULL)
		{	// NULLじゃないなら
			m_apScene2D[nCntPause]->SetInitAll(D3DXVECTOR3(pos.x, pos.y + (nCntPause * POS_X_INTERVAL), pos.x), size, col, UV);
			m_apScene2D[nCntPause]->Init();
			m_apScene2D[nCntPause]->BindTexture(m_pTexture[nCntPause]);
		}
	}

	for (int nCntPause = 0; nCntPause < POLYGON_PRESS; nCntPause++)
	{
		if (m_pPress[nCntPause] == NULL)
		{	// NULLなら
			m_pPress[nCntPause] = new CPress;
		}
		if (m_pPress[nCntPause] != NULL)
		{	// NULLじゃないなら
			if (nCntPause == 0)
			{
				m_pPress[nCntPause]->Init(D3DXVECTOR3(pos.x - PRESS_INTERVAL, pos.y, pos.z), D3DXVECTOR3(PRESS_BUTTON_X, PRESS_BUTTON_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, D3DX_PI * -0.5f);
			}
			if (nCntPause == 1)
			{
				m_pPress[nCntPause]->Init(D3DXVECTOR3(580.0f, 60.0f, pos.z), D3DXVECTOR3(PRESS_BUTTON_X - 20.0f, PRESS_BUTTON_Y - 20.0f , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, D3DX_PI * -0.5f);
			}
			if (nCntPause == 2)
			{
				m_pPress[nCntPause]->Init(D3DXVECTOR3(580.0f, 580.0f, pos.z), D3DXVECTOR3(PRESS_BUTTON_X - 20.0f, PRESS_BUTTON_Y - 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, D3DX_PI * 0.5f);
			}
			if (nCntPause == 3)
			{
				m_pPress[nCntPause]->Init(D3DXVECTOR3(660.0f, 60.0f, pos.z), D3DXVECTOR3(PRESS_BUTTON_X - 20.0f, PRESS_BUTTON_Y - 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5, D3DX_PI);
			}
			if (nCntPause == 4)
			{
				m_pPress[nCntPause]->Init(D3DXVECTOR3(660.0f, 580.0f, pos.z), D3DXVECTOR3(PRESS_BUTTON_X - 20.0f, PRESS_BUTTON_Y - 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 6, D3DX_PI);
			}

			m_pPress[nCntPause]->SetType(CPress::TYPE_NONE);
		}
	}

	//　初期化
	m_nNum = 0;
	m_fColA = 1.0f;

	return S_OK;
}

//*****************************************************************************
//				終了処理
//*****************************************************************************
void CPause::Uninit()
{
	for (int nCntPoylgon = 0; nCntPoylgon < POLYGON_PAUSE; nCntPoylgon++)
	{
		if (m_apScene2D[nCntPoylgon] != NULL)
		{	//CScene2Dの終了
			m_apScene2D[nCntPoylgon]->Uninit();
			m_apScene2D[nCntPoylgon] = NULL;
		}
	}

	if (m_pPauseBg != NULL)
	{	// CPressBgの終了へ
		m_pPauseBg->Uninit();
		m_pPauseBg = NULL;
	}

	for (int nCntPoylgon = 0; nCntPoylgon < POLYGON_PRESS; nCntPoylgon++)
	{
		if (m_pPress[nCntPoylgon] != NULL)
		{	// CPressの終了へ
			m_pPress[nCntPoylgon]->Uninit();
			m_pPress[nCntPoylgon] = NULL;
		}
	}
}

//*****************************************************************************
//				更新処理
//*****************************************************************************
void CPause::Update()
{
	CSound *pSound = CManager::GetSound();
	CFade::FADE fade = CFade::GetFade();

	if (fade == CFade::FADE_NONE)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) || CManager::GetInputKeyboard()->GetTrigger(DIK_UP)
			|| CManager::GetInputXPad()->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, 0) == true || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, 0) == true)
		{	// W 又は　↑キーを押した場合
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
			m_nNum = (m_nNum + 2) % POLYGON_PAUSE;
			m_fColA = 1.0f;
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) || CManager::GetInputKeyboard()->GetTrigger(DIK_DOWN)
			|| CManager::GetInputXPad()->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, 0) == true || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN, 0) == true)
		{	// S 又は　↓キーを押した場合
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
			m_nNum = (m_nNum + 1) % POLYGON_PAUSE;
			m_fColA = 1.0f;
		}
	}

	D3DXVECTOR3 pos = m_apScene2D[0]->Getpos();		// メニューの一番目の座標を代入

	m_fColA -= COL_A;	// 透明度の変化

	if (m_fColA < COL_A_MAX)
	{	// ある程度の値に達した場合
		m_fColA = 1.0f;
	}

	for (int nCntPoylgon = 0; nCntPoylgon < POLYGON_PAUSE; nCntPoylgon++)
	{
		if (m_apScene2D[nCntPoylgon] != NULL)
		{
			if (m_nNum == nCntPoylgon)
			{	// 番号と同じ数字の場合
				m_apScene2D[nCntPoylgon]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));
			}
			else
			{	// それ以外の場合
				m_apScene2D[nCntPoylgon]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
		}
	}

	if (fade == CFade::FADE_NONE)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
		{
			if (m_nNum == 0)
			{	// ポーズ解除
				CGame::SetPause(false);
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE003);
			}
			else if (m_nNum == 1)
			{	// ゲーム画面へ
				if (CFade::GetFade() == CFade::FADE_NONE)
				{
					CFade::Create(CManager::MODE_GAME, CFade::FADE_OUT);
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				}
			}
			else if (m_nNum == 2)
			{	//　タイトル画面へ
				if (CFade::GetFade() == CFade::FADE_NONE)
				{
					CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				}
			}
		}
	}
	if (m_pPress[0] != NULL)
	{

		m_pPress[0]->Setpos(D3DXVECTOR3(pos.x - PRESS_INTERVAL, pos.y + (m_nNum * POS_X_INTERVAL), pos.z));	// 位置更新
		m_pPress[0]->Setrot(D3DX_PI * 0.5f);		// 向きの設定
		m_pPress[0]->Update();						// 更新

	}

}

//*****************************************************************************
//				描画処理
//*****************************************************************************
void CPause::Draw()
{
	for (int nCntPause = 0; nCntPause < POLYGON_PAUSE; nCntPause++)
	{
		if (m_apScene2D[nCntPause] != NULL)
		{
			m_apScene2D[nCntPause]->Draw();
		}
	}
}
