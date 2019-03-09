//**************************************************************
//
//				チュートリアルの処理[tutorial.cpp]
//				Auther : Shun Yokomichi
//
//**************************************************************
#include "scene2d.h"		// シーン2D
#include "tutorial.h"		// チュートリアル
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "fade.h"			// フェード
#include "input.h"			// 入力
#include "press.h"			// プレスボタン
#include "sound.h"			// サウンド

//**************************************************************
//				マクロ定義
//**************************************************************
#define TEXTURE	"data/TEXTURE/tutorial/Tutorial.png"	//読み込むテクスチャ000

//**************************************************************
//				静的メンバ変数
//**************************************************************
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture = NULL;
CPress			   *CTutorial::m_apPress[PRESS_KEY] = {};

//**************************************************************
//				読み込むテクスチャ
//**************************************************************
HRESULT CTutorial::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	if (m_pTexture == NULL)
	{
		D3DXCreateTextureFromFile(pDevice, TEXTURE, &m_pTexture);
	}

	return S_OK;
}

//**************************************************************
//				開放するテクスチャ
//**************************************************************
void CTutorial::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//**************************************************************
//		コンストラクタ
//**************************************************************
CTutorial::CTutorial()
{
	// デフォルト値
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}
}

//**************************************************************
//		デストラクタ
//**************************************************************
CTutorial::~CTutorial()
{

}

//**************************************************************
//		初期化処理
//**************************************************************
HRESULT CTutorial::Init(void)
{
	CManager::MODE pMode = CManager::GetMode();

	//Load();
	CPress::Load();

	m_pScene = new CScene2D(CScene::PRIORITY_BG, CScene::OBJTYPE_BG);	// 動的確保

	if (m_pScene != NULL)
	{	// NULLじゃないなら
		m_pScene->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.25, 1.0f));		// 初期値設定
		m_pScene->Init();
		m_pScene->BindTexture(m_pTexture);
	}

	for (int nCntPress = 0; nCntPress < PRESS_KEY; nCntPress++)
	{
		if (m_apPress[nCntPress] == NULL)
		{
			m_apPress[nCntPress] = new CPress;	// 動的確保
		}

		if (m_apPress[nCntPress] != NULL)
		{
			float rot = 0.0f;		// rotの変数
			if (nCntPress == 0)
			{	// 0の場合
				rot = -D3DX_PI * 0.5f;		// 向き設定
				m_apPress[nCntPress]->Init(D3DXVECTOR3(50.0f + (nCntPress * (1150.0f)), SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, rot);
				m_apPress[nCntPress]->SetType(CPress::TYPE_MANUAL);	// タイプの設定
			}
			if (nCntPress == 1)
			{	// １の場合
				rot = D3DX_PI * 0.5f;		// 向きの設定
				m_apPress[nCntPress]->Init(D3DXVECTOR3(50.0f + (nCntPress * (1170.0f)), SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, rot);
			}
			else if (nCntPress == 2)
			{
				m_apPress[nCntPress]->Init(D3DXVECTOR3(70.0f + ((nCntPress - nCntPress) * (1170.0f)), 600.0f, 0.0f),
					D3DXVECTOR3(80.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2, rot);
				m_apPress[nCntPress]->SetType(CPress::TYPE_NONE);	// タイプの設定
			}
			else if (nCntPress == 3)
			{
				m_apPress[nCntPress]->Init(D3DXVECTOR3(30.0f + ((nCntPress - 2) * (1170.0f)), 600.0f, 0.0f),
					D3DXVECTOR3(80.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 3, rot);
				m_apPress[nCntPress]->SetType(CPress::TYPE_NONE);	// タイプの設定
			}
			else if (nCntPress == 4)
			{
				m_apPress[nCntPress]->Init(D3DXVECTOR3(100.0f + ((nCntPress - nCntPress) * (1170.0f)), 680.0f, 0.0f),
					D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, D3DX_PI);
				m_apPress[nCntPress]->SetType(CPress::TYPE_NONE);	// タイプの設定
			}
			else if (nCntPress == 5)
			{
				m_apPress[nCntPress]->Init(D3DXVECTOR3(20.0f + ((nCntPress - 4) * (1170.0f)), 680.0f, 0.0f),
					D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, 0.0f);
				m_apPress[nCntPress]->SetType(CPress::TYPE_NONE);	// タイプの設定
			}
		}
	}

	// 初期化
	m_nPress = 0;
	m_fTexX = 0.0f;
	m_bPress = false;
	m_bmove = false;

	return S_OK;
}

//**************************************************************
//		終了処理
//**************************************************************
void CTutorial::Uninit(void)
{
	CPress::Unload();
	if (m_pScene != NULL)
	{	// NULLじゃない場合
		m_pScene->Uninit();
		m_pScene = NULL;
	}

	for (int nCntPress = 0; nCntPress < PRESS_KEY; nCntPress++)
	{
		if (m_apPress[nCntPress] != NULL)
		{	// NULLじゃない場合
			m_apPress[nCntPress]->Uninit();
			m_apPress[nCntPress] = NULL;
		}
	}

	CScene::ReleaseMode();
}

//**************************************************************
//		更新処理
//**************************************************************
void CTutorial::Update(void)
{
	int nCreate = 0;	// 透明度をかえるかどうか
	CSound *pSound = CManager::GetSound();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{// フェードしていない状態
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) && m_nPress >= 3
			|| CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_B, 0) == true && m_nPress >= 3)
		{// キーボード（ENTER）を入力したら && 回数が3回なら
			CFade::Create(CManager::MODE_GAME, CFade::FADE_OUT);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) && m_bPress == false
			|| CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, 0) == true && m_bPress == false
			|| CManager::GetInputXPad()->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true && m_bPress == false)
		{	// ENTER && falseなら
			if (m_nPress < 3)
			{
				m_nPress++;			// 回数の増加
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
			}
			nCreate++;
			m_bPress = true;	// 判定をtrueに
			m_bmove = false;	// 判定をfalseに
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) && m_bPress == false
			|| CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_DPAD_LEFT, 0) == true && m_bPress == false
			|| CManager::GetInputXPad()->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true && m_bPress == false)
		{
			if (m_nPress > 0)
			{
				m_nPress--;			// 回数の増減
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
			}
			nCreate++;
			m_bPress = true;	// 判定をtrueに
			m_bmove = true;		// 判定をfalseに
		}
	}

	if (m_nPress == 0 && nCreate == 1)
	{	// 最初の画面の場合
		m_apPress[1]->SetType(CPress::TYPE_OPAQUE);
		m_apPress[0]->SetType(CPress::TYPE_MANUAL);
	}
	else if (m_nPress == 1 && nCreate == 1 || m_nPress == 2 && nCreate == 1)
	{	// ２番目の画面の場合
		m_apPress[0]->SetType(CPress::TYPE_OPAQUE);
		m_apPress[3]->SetTex(3);
		m_apPress[5]->SetTex(4);
		m_apPress[1]->SetType(CPress::TYPE_OPAQUE);
	}
	else if (m_nPress == 3 && nCreate == 1)
	{	// 最後の画面の場合
		m_apPress[0]->SetType(CPress::TYPE_OPAQUE);
		m_apPress[3]->SetTex(7);
		m_apPress[5]->SetTex(8);
		m_apPress[1]->SetType(CPress::TYPE_MANUAL);
	}

	// textureのUV移動
	if (m_fTexX < (0.25f * m_nPress) && m_nPress <= 3 && m_bmove == false)
	{	// UV移動範囲　&& 回数が2回以下 && 進むなら
		m_fTexX += 0.01f;		// UVをずらす
	}
	else if (m_fTexX >(0.25f * m_nPress) && m_nPress <= 3 && m_bmove == true)
	{	// UV移動範囲　&& 回数が2回以下 && もどるなら
		m_fTexX -= 0.01f;		// UVをずらす
	}
	else if (m_nPress <= 3)
	{	// 2回以下なら
		m_bPress = false;				// 判定をfalseに
		m_fTexX = (0.25f * m_nPress);	// UVを代入
	}

	if (m_pScene != NULL)
	{
		m_pScene->SetTex(0, 4, D3DXVECTOR2(m_fTexX, 1.0f));
		m_pScene->Update();
	}

}

//**************************************************************
//		描画処理
//**************************************************************
void CTutorial::Draw(void)
{
	if (m_pScene != NULL)
	{
		m_pScene->Draw();
	}

	for (int nCntPress = 0; nCntPress < PRESS_KEY; nCntPress++)
	{
		if (m_apPress[nCntPress] != NULL)
		{
			m_apPress[nCntPress]->Draw();
		}
	}
}