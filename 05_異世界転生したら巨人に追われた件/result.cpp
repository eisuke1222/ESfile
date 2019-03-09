//**************************************************************
//
//				リザルトの処理[result.cpp]
//				Auther : Shun Yokomichi
//
//**************************************************************
#include "scene2d.h"		// シーン2D
#include "result.h"			// リザルト
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "fade.h"			// フェード
#include "input.h"			// 入力
#include "sound.h"			// サウンド
#include "scene.h"			// シーン
#include "press.h"			// ボタン
//**************************************************************
//				マクロ定義
//**************************************************************
#define DEFAULT_COLOR_OPACITY_WHITE		(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))						//不透明の白
#define DEFAULT_COLOR_OPACITY_BLACK		(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))						//不透明の黒
#define DEFAULT_COLOR_HALFCLEAN_WHITE	(D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))						//半透明の白
#define DEFAULT_COLOR_HALFCLEAN_BLACK	(D3DXCOLOR(0.0f,0.0f,0.0f,0.5f))						//半透明の黒
#define DEFAULT_COLOR_CLEAN_WHITE		(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))						//透明の白
#define DEFAULT_COLOR_CLEAN_BLACK		(D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))						//透明の黒
#define MENU_TEX						"data/TEXTURE/Pause/Menu.png"							// メニュー
#define RESULT_FALLING_LOGO				"data/TEXTURE/result/Result1_Falling/Logo.png"			// ロゴ

//===============================
// 落下ED
//===============================
#define RESULT_FALLING_BROCK			"data/TEXTURE/result/Result1_Falling/00_BG.png"			// 落下ED：岩
#define RESULT_FALLING_PLAYER			"data/TEXTURE/result/Result1_Falling/Player.png"		// 落下ED：プレイヤー
#define RESULT_FALLING_SPEECH			"data/TEXTURE/result/Result1_Falling/00_speech.png"		// 落下ED：台詞
#define RESULT_FALLING_LIGHT			"data/TEXTURE/result/Result1_Falling/01_Light.png"		// 落下ED：ライト
#define RESULT_FALLING_SOUL				"data/TEXTURE/result/Result1_Falling/01_Soul.png"		// 落下ED：魂
#define FALLING_POS_X_KEY				(275.0f)												// キーボードの位置
#define FALLING_POS_X_DIFFERENCE		(100.0f)												// プレスボタンの位置の差分
#define FALLING_POS_X_JOIPAD			(FALLING_POS_X_KEY + FALLING_POS_X_DIFFERENCE)			// コントローラーのボタン位置
#define FALLING_POS_Y_DIFFERENCE		(140.0f)												// posYの差分
#define	FALLING_POS_Y_NUM_DIF			(650.0f)												// 選択した時の位置差分
//===============================
// 捕獲ED
//===============================
#define RESULT_PREY_SKY					"data/TEXTURE/result/Result2_prey/Sky.png"				// 捕獲ED：空
#define RESULT_PREY_WOOD				"data/TEXTURE/result/Result2_prey/Wood.png"				// 捕獲ED：木
#define RESULT_PREY_CLOUD				"data/TEXTURE/result/Result2_prey/Cloud.png"			// 捕獲ED：雲
#define RESULT_PREY_GROUND				"data/TEXTURE/fieldmesh.png"							// 捕獲ED：地面
#define RESULT_PREY_PLAYER				"data/TEXTURE/result/Result2_prey/Player.png"			// 逃走ED：プレイヤー
#define RESULT_PREY_OPTION				"data/TEXTURE/result/Result2_prey/option.png"			// 逃走ED：驚き
#define PREY_POS_X_KEY					(465.0f)												// キーボードの位置
#define PREY_POS_X_DIFFERENCE			(100.0f)												// プレスボタンの位置の差分
#define PREY_POS_X_JOIPAD				(PREY_POS_X_KEY + PREY_POS_X_DIFFERENCE)				// コントローラーのボタン位置
#define PREY_POS_Y_DIFFERENCE			(250.0f)												// posYの差分
#define	PREY_POS_Y_NUM_DIF				(500.0f)												// 選択した時の位置差分
//===============================
// 逃走ED
//===============================
#define RESULT_ESCAPE_PLAYER			"data/TEXTURE/result/Result3_escape/Player.png"			// 逃走ED：プレイヤー
#define RESULT_ESCAPE_ENEMY				"data/TEXTURE/result/Result3_escape/Enemy.png"			// 逃走ED：巨人
#define RESULT_ESCAPE_BG				"data/TEXTURE/result/Result3_escape/Bg.png"				// 逃走ED：背景
#define RESULT_ESCAPE_SKY				"data/TEXTURE/result/Result3_escape/Sky.png"				// 逃走ED：背景

//**************************************************************
//				静的メンバ変数
//**************************************************************
CResult *CResult::m_pResult = NULL;
int CResult::m_nCntfade = 0;
LPDIRECT3DTEXTURE9 CFallingEnd::m_pTexture[MAX_FALLINGMODE_TEXTURE] = {};
LPDIRECT3DTEXTURE9 CPreyEnd::m_pTexture[MAX_PREYMODE_TEXTURE] = {};
LPDIRECT3DTEXTURE9 CEscapeEnd::m_pTexture[MAX_ESCAPEMODE_TEXTURE] = {};

//**************************************************************
//		コンストラクタ
//**************************************************************
CResult::CResult()
{
	// デフォルト値
	m_nCntfade = 0;
}

//**************************************************************
//		デストラクタ
//**************************************************************
CResult::~CResult()
{

}
//**************************************************************
//				読み込むテクスチャ
//**************************************************************
HRESULT CResult::Load(void)
{
	return S_OK;
}

//**************************************************************
//				開放するテクスチャ
//**************************************************************
void CResult::Unload(void)
{

}

CResult *CResult::Create(CGame::MODEGAME gameMode)
{
	switch (gameMode)
	{
	case CGame::MODE_GAMECLEAR:		// クリア状態
		if (m_pResult == NULL)
		{
			m_pResult = new CEscapeEnd;
		}
		break;

	case CGame::MODE_GAMEOVER000:	// 巨人に捕まった状態
		if (m_pResult == NULL)
		{
			m_pResult = new CPreyEnd;
		}
		break;

	case CGame::MODE_GAMEOVER001:	//体力が0になった状態
		if (m_pResult == NULL)
		{
			m_pResult = new CFallingEnd;
		}
		break;
	}


	return m_pResult;
}

//**************************************************************
//		初期化処理
//**************************************************************
HRESULT CResult::Init(void)
{
	return S_OK;
}

//**************************************************************
//		終了処理
//**************************************************************
void CResult::Uninit(void)
{
	if (m_pResult != NULL)
	{
		m_pResult = NULL;
	}

	CScene::ReleaseMode();	// モード終了
}

//**************************************************************
//		更新処理
//**************************************************************
void CResult::Update(void)
{

}

//**************************************************************
//		描画処理
//**************************************************************
void CResult::Draw(void)
{

}
//**************************************************************
//		フェードのカウントダウン
//**************************************************************
void CResult::CntDownFadeTime(void)
{
	m_nCntfade++;

	if (m_nCntfade >= MAX_FADERESTRICTION)
	{//8～10秒経ったらランキングへ遷移
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードしていない状態
			CFade::Create(CManager::MODE_RANKING, CFade::FADE_OUT);
		}
	}
}

//=============================================================================
// ▽CFallingEnd :(落下ED)
//=============================================================================
//**************************************************************
//		コンストラクタ
//**************************************************************
CFallingEnd::CFallingEnd()
{
	m_FallingMode = FALLINGMODE_MOVENOSTART;
	m_MenuState = MENUSTATE_NONE;
	m_MenuCol = MENUSTATECOLOR_NONE;

	for (int nCnt = 0; nCnt < MAX_FALLINGMODE_POLYGON; nCnt++)
	{
		m_pScene[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_PRESS_BUTTON; nCnt++)
	{
		m_apPress[nCnt] = NULL;
	}

	m_Scalvalue = 0.0f;
	m_fChengeMove = -0.01f;
	m_fCntMove = 0.0f;
	m_bSkipProduction = false;
	m_nSelectNum = 0;
}
//**************************************************************
//		デストラクタ
//**************************************************************
CFallingEnd::~CFallingEnd()
{

}
//**************************************************************
//				読み込むテクスチャ
//**************************************************************
HRESULT CFallingEnd::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, MENU_TEX, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, RESULT_FALLING_BROCK, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, RESULT_FALLING_PLAYER, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, RESULT_FALLING_SPEECH, &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, RESULT_FALLING_LIGHT, &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, RESULT_FALLING_SOUL, &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, RESULT_FALLING_LOGO, &m_pTexture[6]);

	return S_OK;
}

//**************************************************************
//				開放するテクスチャ
//**************************************************************
void CFallingEnd::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_FALLINGMODE_TEXTURE; nCnt++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//**************************************************************
//		初期化処理
//**************************************************************
HRESULT CFallingEnd::Init()
{
	CPress::Load();
	for (int nCnt = 0; nCnt < MAX_FALLINGMODE_POLYGON; nCnt++)
	{
		if (m_pScene[nCnt] == NULL)
		{
			if (nCnt == SCENETYPE_MASK)
			{//黒背景
				m_pScene[nCnt] = new CScene2D(CScene::CScene::PRIORITY_BG, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), DEFAULT_COLOR_OPACITY_BLACK, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
			}
			else if (nCnt == SCENETYPE_BLOCK)
			{//岩
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_BG, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), DEFAULT_COLOR_OPACITY_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_PLAYER)
			{//プレイヤー
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_1, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(800.0f, 800.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(0.5f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_SPEECH)
			{//断末魔
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_2, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(500.0f, 250.0f, 0.0f),
					D3DXVECTOR3(50.0f, 100.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->Setrot(0.8f);
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_LIGHT)
			{//照明
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_BG, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, 320.0f, 0.0f),
					D3DXVECTOR3(350.0f, 320.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_SOUL)
			{//魂
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_2, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f),
					D3DXVECTOR3(50.0f, 100.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(0.5f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_LOGO)
			{//ロゴ
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_3, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f),
					D3DXVECTOR3(500.0f, 80.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 0.5f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_MENU_RETRY)
			{//メニュー(リトライ）
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_4, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(300.0f, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(150.0f, 100.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 1.0f / 3));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->SetTex(1, 1, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->BindTexture(m_pTexture[0]);
			}
			else if (nCnt == SCENETYPE_MENU_QUIT)
			{//メニュー（クイット）
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_4, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(950.0f, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(150.0f, 100.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 1.0f / 3));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->SetTex(1, 2, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->BindTexture(m_pTexture[0]);
			}

		}
	}

	for (int nCnt = 0; nCnt < MAX_PRESS_BUTTON; nCnt++)
	{
		if (m_apPress[nCnt] == NULL)
		{
			m_apPress[nCnt] = new CPress;

			if (nCnt == 0)
			{	// キーボード用のボタン
				m_apPress[nCnt]->Init(D3DXVECTOR3(FALLING_POS_X_KEY, SCREEN_HEIGHT / 2 + 200.0f, 0.0f), D3DXVECTOR3(70.0f, 50.0f, 0.0f),
					D3DXCOLOR(DEFAULT_COLOR_CLEAN_WHITE), 9, 0.0f);
				m_apPress[nCnt]->SetType(CPress::TYPE_MANUAL);
			}
			else
			{	// コントローラー用のボタン
				m_apPress[nCnt]->Init(D3DXVECTOR3(FALLING_POS_X_JOIPAD, SCREEN_HEIGHT / 2 + FALLING_POS_Y_DIFFERENCE, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
					D3DXCOLOR(DEFAULT_COLOR_CLEAN_WHITE), 8, 0.0f);
				m_apPress[nCnt]->SetType(CPress::TYPE_MANUAL);
			}
		}
	}

	return S_OK;
}
//**************************************************************
//		終了処理
//**************************************************************
void CFallingEnd::Uninit(void)
{
	CPress::Unload();

	for (int nCnt = 0; nCnt < MAX_FALLINGMODE_POLYGON; nCnt++)
	{
		if (m_pScene[nCnt] != NULL)
		{	// NULLじゃない場合
			m_pScene[nCnt]->Uninit();
			m_pScene[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_PRESS_BUTTON; nCnt++)
	{
		if (m_apPress[nCnt] != NULL)
		{	// NULLじゃない場合
			m_apPress[nCnt]->Uninit();
			m_apPress[nCnt] = NULL;
		}
	}

	CResult::Uninit();
}
//**************************************************************
//		更新処理
//**************************************************************
void CFallingEnd::Update(void)
{
	CSound *pSound = CManager::GetSound();

	switch (m_FallingMode)
	{
	case FALLINGMODE_NONE:			//何もない状態
		MoveScene(SCENETYPE_SOUL);
		CntDownFadeTime();
		break;

	case FALLINGMODE_MOVENOSTART:	//アニメーション未開始状態
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			if (m_pScene[SCENETYPE_PLAYER] != NULL)
			{
				m_pScene[SCENETYPE_PLAYER]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
			}
			if (m_pScene[SCENETYPE_SPEECH] != NULL)
			{
				m_pScene[SCENETYPE_SPEECH]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
			}
			m_FallingMode = FALLINGMODE_PLAYERFALL;
		}
		break;

	case FALLINGMODE_PLAYERFALL:	//プレイヤーが落ちていく状態
		if (m_pScene[SCENETYPE_PLAYER] != NULL)
		{/* プレイヤーの動きと岩の透明度の処理 */

			D3DXVECTOR3 PlayerSize = m_pScene[SCENETYPE_PLAYER]->Getsize();	// プレイヤーの現在のサイズを取得

			if (PlayerSize.x <= 1.0f&&PlayerSize.y <= 1.0f)
			{//サイズが小さくなりきったら岩をフェードアウトして次の画面へ行く準備をする
				m_pScene[SCENETYPE_PLAYER]->Setcol(DEFAULT_COLOR_CLEAN_WHITE);			//プレイヤーを不透明にする
				if (m_pScene[SCENETYPE_SPEECH] != NULL)
				{/* 断末魔の処理 */
					m_pScene[SCENETYPE_SPEECH]->Setcol(DEFAULT_COLOR_CLEAN_WHITE);
				}
				if (m_pScene[SCENETYPE_BLOCK] != NULL)
				{
					float fFadeColor = 0.08f;
					D3DXCOLOR col = m_pScene[SCENETYPE_BLOCK]->Getcol();
					if (col.a <= 0.0f)
					{//岩の透明度が0以下になり、断末魔の透明度も0になったら次のプレイヤーが死んでる画面へ
						m_FallingMode = FALLINGMODE_DEATH;
					}
					else
					{//岩の透明度が0以上だったら0にしていく
						col.a -= fFadeColor;
						m_pScene[SCENETYPE_BLOCK]->Setcol(col);
					}
				}
			}
			else
			{//小さくなってなかったら小さくしていく
				if (PlayerSize.x == 800.0f && PlayerSize.y == 800.0f)
				{
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE039);
				}

				if (m_pScene[SCENETYPE_SPEECH] != NULL)
				{/* 断末魔の処理 */

					D3DXCOLOR SpeechCol = m_pScene[SCENETYPE_SPEECH]->Getcol();
					D3DXVECTOR3 SpeechPos = m_pScene[SCENETYPE_SPEECH]->Getpos();
					SpeechPos.x -= 0.5f;
					SpeechPos.y -= 0.5f;

					SpeechCol.a -= 0.01f;
					m_pScene[SCENETYPE_SPEECH]->Setcol(SpeechCol);
					m_pScene[SCENETYPE_SPEECH]->Setpos(SpeechPos);
				}

				/* プレイヤーの縮小処理 */
				m_Scalvalue += 0.3f;
				float fPlayerFadeColor = 0.005f;
				D3DXCOLOR col = m_pScene[SCENETYPE_PLAYER]->Getcol();

				D3DXVECTOR3 Scal = D3DXVECTOR3(PlayerSize.x - m_Scalvalue, PlayerSize.y - m_Scalvalue, 0.0f);

				col.a -= fPlayerFadeColor;
				m_pScene[SCENETYPE_PLAYER]->Setsize(D3DXVECTOR3(Scal.x, Scal.y, 0.0f));
				m_pScene[SCENETYPE_PLAYER]->Setcol(col);
			}
		}
		break;

	case FALLINGMODE_DEATH:			//プレイヤーが死んだ状態
		if (m_pScene[SCENETYPE_PLAYER] != NULL)
		{/* プレイヤーの処理 */
			m_pScene[SCENETYPE_PLAYER]->Setsize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));	//サイズを次に使うサイズにする
			m_pScene[SCENETYPE_PLAYER]->SetTex(1, 1, D3DXVECTOR2(0.5f, 1.0f));		//倒れたプレイヤーのコマに変更
			m_pScene[SCENETYPE_PLAYER]->Setpos(D3DXVECTOR3(630.0f, 450.0f, 0.0f));	//倒れたプレイヤーの位置に変更

			D3DXCOLOR col = m_pScene[SCENETYPE_PLAYER]->Getcol();
			float fLightFadeColor = 0.01f;
			col.a += fLightFadeColor;
			if (col.a >= 1.0f)
			{//
				m_pScene[SCENETYPE_PLAYER]->Setcol(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
				m_FallingMode = FALLINGMODE_LIGHTON;
			}
			else
			{
				m_pScene[SCENETYPE_PLAYER]->Setcol(D3DXCOLOR(0.3f, 0.3f, 0.3f, col.a));
			}
		}
		break;
	case FALLINGMODE_LIGHTON:
		if (m_pScene[SCENETYPE_LIGHT] != NULL)
		{//ライトの透明度を上げる
			m_pScene[SCENETYPE_LIGHT]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);

			if (m_pScene[SCENETYPE_PLAYER] != NULL)
			{//プレイヤーの色を戻す
				m_pScene[SCENETYPE_PLAYER]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
				if (m_pScene[SCENETYPE_SOUL] != NULL)
				{
					m_pScene[SCENETYPE_SOUL]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
					if (m_pScene[SCENETYPE_LOGO] != NULL)
					{
						m_pScene[SCENETYPE_LOGO]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
						m_FallingMode = FALLINGMODE_NONE;

						//BGM：ゲームオーバー
						pSound->PlaySoundA(CSound::SOUND_LABEL_BGM004);

						//SE ：チーン
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE019);

						m_MenuState = MENUSTATE_RETRY;
					}
				}
			}
		}
		break;
	}

	/* メニューの処理 */
	if (m_MenuState != MENUSTATE_NONE)
	{//NONEじゃないときだけ操作可能にする
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true || CManager::GetInputXPad()->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true)
		{
			m_nSelectNum = (m_nSelectNum + 1) % 2;
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true || CManager::GetInputXPad()->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
		{
			m_nSelectNum = (m_nSelectNum + 1) % 2;
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
		}

		for (int nCnt = 0; nCnt < MAX_PRESS_BUTTON; nCnt++)
		{
			if (m_apPress[nCnt] != NULL)
			{
				m_apPress[nCnt]->Setpos(D3DXVECTOR3((FALLING_POS_X_KEY + (nCnt * FALLING_POS_X_DIFFERENCE)) + (m_nSelectNum * FALLING_POS_Y_NUM_DIF), SCREEN_HEIGHT / 2 + FALLING_POS_Y_DIFFERENCE, 0.0f));
				m_apPress[nCnt]->SetType(CPress::TYPE_NONE);
			}
		}

		/* メニューが選ばれている時の処理 */
		if (m_nSelectNum == 0)
		{//リトライ
			m_MenuState = MENUSTATE_RETRY;
		}
		else if (m_nSelectNum == 1)
		{//クイット
			m_MenuState = MENUSTATE_QUIT;
		}
	}
	switch (m_MenuState)
	{//選ばれてるモード別の処理
	case MENUSTATE_NONE:
		break;

	case MENUSTATE_RETRY:
		m_MenuCol = MENUSTATECOLOR_RETRY;
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードしていない状態
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
			{// キーボード（ENTER）を入力したら
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				CFade::Create(CManager::MODE_GAME, CFade::FADE_OUT);
				m_MenuState = MENUSTATE_NONE;
				pSound->StopSound(CSound::SOUND_LABEL_BGM004);
			}
		}
		break;

	case MENUSTATE_QUIT:
		m_MenuCol = MENUSTATECOLOR_QUIT;
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードしていない状態
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
			{// キーボード（ENTER）を入力したら
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
				m_MenuState = MENUSTATE_NONE;
				pSound->StopSound(CSound::SOUND_LABEL_BGM004);
			}
		}
		break;
	}
	switch (m_MenuCol)
	{//選ばれてるモードの色と位置
	case MENUSTATECOLOR_NONE:
		break;

	case MENUSTATECOLOR_RETRY:
		if (m_pScene[SCENETYPE_MENU_RETRY] != NULL)
		{
			m_pScene[SCENETYPE_MENU_RETRY]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
		}
		if (m_pScene[SCENETYPE_MENU_QUIT] != NULL)
		{
			m_pScene[SCENETYPE_MENU_QUIT]->Setcol(DEFAULT_COLOR_HALFCLEAN_WHITE);
		}
		break;

	case MENUSTATECOLOR_QUIT:
		if (m_pScene[SCENETYPE_MENU_RETRY] != NULL)
		{
			m_pScene[SCENETYPE_MENU_RETRY]->Setcol(DEFAULT_COLOR_HALFCLEAN_WHITE);
		}
		if (m_pScene[SCENETYPE_MENU_QUIT] != NULL)
		{
			m_pScene[SCENETYPE_MENU_QUIT]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
		}
		break;
	}

	/* 演出スキップの処理 */
	if (m_bSkipProduction == false)
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードしていない状態
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
			{// キーボード（ENTER）を入力したら
				SkipProduction();
				m_bSkipProduction = true;
			}
		}
	}
}
//**************************************************************
//		描画処理
//**************************************************************
void CFallingEnd::Draw(void)
{

}
//**************************************************************
//		上下移動処理
//**************************************************************
void CFallingEnd::MoveScene(SceneType type)
{
	if (m_pScene[type] != NULL)
	{/* 魂の処理 */
		D3DXVECTOR3 pos = m_pScene[type]->Getpos();

		if (m_fCntMove >= 1 || m_fCntMove <= -1)
		{
			m_fChengeMove *= -1.0f;//折り返し
		}
		m_fCntMove += m_fChengeMove;
		pos.y += m_fCntMove;

		m_pScene[type]->Setpos(pos);
	}
}
//**************************************************************
//		演出飛ばした時の処理
//**************************************************************
void CFallingEnd::SkipProduction(void)
{
	CSound *pSound = CManager::GetSound();

	if (m_pScene[SCENETYPE_BLOCK] != NULL)
	{//岩の透明度を0にする
		m_pScene[SCENETYPE_BLOCK]->Setcol(DEFAULT_COLOR_CLEAN_WHITE);
	}
	if (m_pScene[SCENETYPE_PLAYER] != NULL)
	{//プレイヤーの情報を変更
		m_pScene[SCENETYPE_PLAYER]->Setsize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		m_pScene[SCENETYPE_PLAYER]->SetTex(1, 1, D3DXVECTOR2(0.5f, 1.0f));
		m_pScene[SCENETYPE_PLAYER]->Setpos(D3DXVECTOR3(630.0f, 450.0f, 0.0f));
		m_pScene[SCENETYPE_PLAYER]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}
	if (m_pScene[SCENETYPE_SPEECH] != NULL)
	{/* 断末魔の処理 */
		m_pScene[SCENETYPE_SPEECH]->Setcol(DEFAULT_COLOR_CLEAN_WHITE);
	}
	if (m_pScene[SCENETYPE_LIGHT] != NULL)
	{//ライトの透明度を上げる
		m_pScene[SCENETYPE_LIGHT]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}
	if (m_pScene[SCENETYPE_SOUL] != NULL)
	{//魂を出現
		m_pScene[SCENETYPE_SOUL]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}
	if (m_pScene[SCENETYPE_LOGO] != NULL)
	{//ロゴを出現
		m_pScene[SCENETYPE_LOGO]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}
	//BGM：ゲームオーバー
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM004);
	//SE ：チーン
	pSound->PlaySoundA(CSound::SOUND_LABEL_SE019);

	m_FallingMode = FALLINGMODE_NONE;
	m_MenuState = MENUSTATE_RETRY;
}

//=============================================================================
// ▽CPreyEnd :(落下ED)
//=============================================================================
//**************************************************************
//		コンストラクタ
//**************************************************************
CPreyEnd::CPreyEnd()
{
	m_PreyEndMode = PREYMODE_MOVENOSTART;
	m_MenuState = MENUSTATE_NONE;
	m_MenuCol = MENUSTATECOLOR_NONE;

	for (int nCnt = 0; nCnt < MAX_PREYMODE_POLYGON; nCnt++)
	{
		m_pScene[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_PRESS_BUTTON; nCnt++)
	{
		m_apPress[nCnt] = NULL;
	}

	m_fCntPlayerMove = 0.0f;
	m_fChangePlayerMove = 0.05f;
	nCntframe = 0;
	m_fChengeMove = 0.01f;
	m_fCntMove = 0.0f;
	m_bSkipProduction = false;
	m_nSelectNum = 0;
	m_fCntWoodMove = 0.0f;
	m_fChangeWoodMove = 0.3f;
	m_fOptionCol = 0.3f;
	m_fCloudScroll = 0.0f;
}
//**************************************************************
//		デストラクタ
//**************************************************************
CPreyEnd::~CPreyEnd()
{

}
//**************************************************************
//				読み込むテクスチャ
//**************************************************************
HRESULT CPreyEnd::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, MENU_TEX, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, RESULT_PREY_SKY, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, RESULT_PREY_WOOD, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, RESULT_PREY_CLOUD, &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, RESULT_PREY_GROUND, &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, RESULT_PREY_PLAYER, &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, RESULT_PREY_OPTION, &m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice, RESULT_FALLING_SOUL, &m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice, RESULT_FALLING_LOGO, &m_pTexture[8]);

	return S_OK;
}

//**************************************************************
//				開放するテクスチャ
//**************************************************************
void CPreyEnd::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_PREYMODE_TEXTURE; nCnt++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//**************************************************************
//		初期化処理
//**************************************************************
HRESULT CPreyEnd::Init()
{
	CPress::Load();
	for (int nCnt = 0; nCnt < MAX_PREYMODE_POLYGON; nCnt++)
	{
		if (m_pScene[nCnt] == NULL)
		{
			if (nCnt == SCENETYPE_SKY)
			{//空
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_BG, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), DEFAULT_COLOR_OPACITY_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt + 1]);
			}
			else if (nCnt == SCENETYPE_WOOD)
			{//森
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_BG, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), D3DXVECTOR2(2.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->SetTex(1, 1, D3DXVECTOR2(2.0f, 1.0f));
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt + 1]);
			}
			else if (nCnt == SCENETYPE_CLOUD)
			{//雲
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_BG, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, 320.0f, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2, 320.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt + 1]);
			}
			else if (nCnt == SCENETYPE_GROUND)
			{//地面
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_BG, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, 660.0f, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2, 60.0f, 0.0f), DEFAULT_COLOR_OPACITY_WHITE, D3DXVECTOR2(10.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt + 1]);
			}
			else if (nCnt == SCENETYPE_PLAYER)
			{//プレイヤー
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_1, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2 + 5.0f, SCREEN_HEIGHT / 2 + 5.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(0.5f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt + 1]);
			}
			else if (nCnt == SCENETYPE_OPTION)
			{//驚き記号
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_2, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(1200.0f, 200.0f, 0.0f),
					D3DXVECTOR3(50.0f, 50.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt + 1]);
			}
			else if (nCnt == SCENETYPE_SOUL)
			{//魂
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_2, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(1100.0f, 100.0f, 0.0f),
					D3DXVECTOR3(50.0f, 50.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(0.5f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->SetTex(1, 1, D3DXVECTOR2(0.5f, 1.0f));
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt + 1]);
			}
			else if (nCnt == SCENETYPE_LOGO)
			{//ロゴ
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_3, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(500.0f, 100.0f, 0.0f),
					D3DXVECTOR3(480.0f, 80.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 0.5f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt + 1]);
			}
			else if (nCnt == SCENETYPE_MENU_RETRY)
			{//メニュー(リトライ）
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_4, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(500.0f, 500.0f, 0.0f),
					D3DXVECTOR3(150.0f, 100.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 1.0f / 3));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->SetTex(1, 1, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->BindTexture(m_pTexture[0]);
			}
			else if (nCnt == SCENETYPE_MENU_QUIT)
			{//メニュー（クイット）
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_4, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(1000.0f, 500.0f, 0.0f),
					D3DXVECTOR3(150.0f, 100.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 1.0f / 3));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->SetTex(1, 2, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->BindTexture(m_pTexture[0]);
			}
		}
	}

	for (int nCnt = 0; nCnt < MAX_PRESS_BUTTON; nCnt++)
	{
		if (m_apPress[nCnt] == NULL)
		{
			m_apPress[nCnt] = new CPress;

			if (nCnt == 0)
			{	// キーボード用のボタン
				m_apPress[nCnt]->Init(D3DXVECTOR3(PREY_POS_X_KEY, SCREEN_HEIGHT / 2 + 200.0f, 0.0f), D3DXVECTOR3(70.0f, 50.0f, 0.0f),
					D3DXCOLOR(DEFAULT_COLOR_CLEAN_WHITE), 9, 0.0f);
				m_apPress[nCnt]->SetType(CPress::TYPE_MANUAL);
			}
			else
			{	// コントローラー用のボタン
				m_apPress[nCnt]->Init(D3DXVECTOR3(PREY_POS_X_KEY, SCREEN_HEIGHT / 2 + PREY_POS_Y_DIFFERENCE, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f),
					D3DXCOLOR(DEFAULT_COLOR_CLEAN_WHITE), 8, 0.0f);
				m_apPress[nCnt]->SetType(CPress::TYPE_MANUAL);
			}
		}
	}

	return S_OK;
}
//**************************************************************
//		終了処理
//**************************************************************
void CPreyEnd::Uninit(void)
{
	CPress::Unload();
	for (int nCnt = 0; nCnt < MAX_PREYMODE_POLYGON; nCnt++)
	{
		if (m_pScene[nCnt] != NULL)
		{	// NULLじゃない場合
			m_pScene[nCnt]->Uninit();
			m_pScene[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_PRESS_BUTTON; nCnt++)
	{
		if (m_apPress[nCnt] != NULL)
		{	// NULLじゃない場合
			m_apPress[nCnt]->Uninit();
			m_apPress[nCnt] = NULL;
		}
	}

	CResult::Uninit();
}
//**************************************************************
//		更新処理
//**************************************************************
void CPreyEnd::Update(void)
{
	CSound *pSound = CManager::GetSound();
	switch (m_PreyEndMode)
	{
	case PREYMODE_NONE:			//何もない状態
		m_fCloudScroll -= 0.001f;
		MoveScene(SCENETYPE_SOUL);
		ScrollCloud(m_fCloudScroll);
		CntDownFadeTime();
		break;

	case PREYMODE_MOVENOSTART:	//アニメーション未開始状態
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			if (m_pScene[SCENETYPE_PLAYER] != NULL)
			{
				m_pScene[SCENETYPE_PLAYER]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
			}
			m_PreyEndMode = PREYMODE_PLAYERPREY;
		}
		break;

	case PREYMODE_PLAYERPREY:	//プレイヤーがつかまった状態
		if (nCntframe == 0)
		{
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE038);
		}

		nCntframe++;
		if (m_pScene[SCENETYPE_OPTION] != NULL)
		{//驚き記号Nullチェック
			D3DXCOLOR col = m_pScene[SCENETYPE_OPTION]->Getcol();

			col.a += m_fOptionCol;

			m_pScene[SCENETYPE_OPTION]->Setcol(col);
			if (col.a >= 1.0f || col.a <= 0.0f)
			{
				m_fOptionCol *= -1.0f;
			}
		}
		if (nCntframe >= 30)
		{//0.5秒つかまっているところを見せてから動かす
			if (m_pScene[SCENETYPE_OPTION] != NULL)
			{//驚き記号は透明にする
				m_pScene[SCENETYPE_OPTION]->Setcol(DEFAULT_COLOR_CLEAN_WHITE);
			}
			if (m_pScene[SCENETYPE_WOOD] != NULL)
			{//森Nullチェック
				if (m_pScene[SCENETYPE_PLAYER] != NULL)
				{//プレイヤーNullチェック
					if (m_pScene[SCENETYPE_GROUND] != NULL)
					{//地面Nullチェック

						D3DXVECTOR3 Playerpos = m_pScene[SCENETYPE_PLAYER]->Getpos();
						D3DXVECTOR3 WoodPos = m_pScene[SCENETYPE_WOOD]->Getpos();
						D3DXVECTOR3 GraundPos = m_pScene[SCENETYPE_GROUND]->Getpos();

						/* プレイヤーの処理 */
						//上にあげる
						m_fCntPlayerMove += m_fChangePlayerMove;
						Playerpos.y += m_fCntPlayerMove;

						if (m_fCntPlayerMove >= 0.5f)
						{//少し下に下がったら
							m_fChangePlayerMove = -1.0f;
						}

						m_pScene[SCENETYPE_PLAYER]->Setpos(Playerpos);

						/* 森の処理 */
						// 下に落とす
						m_fCntWoodMove += m_fChangeWoodMove;
						WoodPos.y -= m_fCntPlayerMove;

						m_pScene[SCENETYPE_WOOD]->Setpos(WoodPos);

						/* 地面の処理 */

						GraundPos.y -= m_fCntPlayerMove;

						if (GraundPos.y >= SCREEN_HEIGHT + m_pScene[SCENETYPE_GROUND]->Getsize().y)
						{//画面外に行ったら
							m_pScene[SCENETYPE_GROUND]->Setpos(D3DXVECTOR3(GraundPos.x, SCREEN_HEIGHT + m_pScene[SCENETYPE_GROUND]->Getsize().y, 0.0f));
							m_pScene[SCENETYPE_GROUND]->Setcol(DEFAULT_COLOR_CLEAN_WHITE);
						}
						m_pScene[SCENETYPE_GROUND]->Setpos(GraundPos);

						if (Playerpos.y <= 0.0f - SCREEN_HEIGHT / 2 && WoodPos.y >= SCREEN_HEIGHT + 200.0f)
						{//プレイヤーが画面外に出たら
							m_pScene[SCENETYPE_PLAYER]->Setcol(DEFAULT_COLOR_CLEAN_WHITE); //次に備えて透明状態
							m_pScene[SCENETYPE_PLAYER]->SetTex(1, 1, D3DXVECTOR2(0.5f, 1.0f));
							nCntframe = 0;
							m_PreyEndMode = PREYMODE_DEATH;
						}
					}
				}
			}
		}
		break;

	case PREYMODE_DEATH:		//プレイヤーが死んだ状態
		nCntframe++;
		ScrollCloud(m_fCloudScroll);

		if (m_pScene[SCENETYPE_CLOUD] != NULL)
		{//雲Nullチェック
			D3DXCOLOR Cloudcol = m_pScene[SCENETYPE_CLOUD]->Getcol();
			float fCloudcol = 0.1f;

			Cloudcol.a += fCloudcol;
			m_pScene[SCENETYPE_CLOUD]->Setcol(Cloudcol);
			if (Cloudcol.a >= 1.0f)
			{
				m_pScene[SCENETYPE_CLOUD]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
			}
		}
		if (nCntframe >= 40)
		{//0.6秒空を見せる
			if (m_pScene[SCENETYPE_PLAYER] != NULL)
			{
				/* プレイヤーの処理 */
				D3DXCOLOR col = m_pScene[SCENETYPE_PLAYER]->Getcol();
				float fFadePLCol = 0.01f;

				m_pScene[SCENETYPE_PLAYER]->Setpos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

				col.a += fFadePLCol;
				m_pScene[SCENETYPE_PLAYER]->Setcol(col);
				if (col.a >= 1.0f)
				{//透明度が上がり切った
					if (m_pScene[SCENETYPE_SOUL] != NULL)
					{
						m_pScene[SCENETYPE_SOUL]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
					}
				}
				if (m_pScene[SCENETYPE_LOGO] != NULL)
				{
					D3DXCOLOR Logocol = m_pScene[SCENETYPE_LOGO]->Getcol();
					float fFadeLogoCol = 0.01f;

					Logocol.a += fFadeLogoCol;
					m_pScene[SCENETYPE_LOGO]->Setcol(Logocol);

					if (Logocol.a >= 1.0f)
					{//透明度が上がり切る
						if (m_pScene[SCENETYPE_MENU_RETRY])
						{
							m_pScene[SCENETYPE_MENU_RETRY]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
						}
						if (m_pScene[SCENETYPE_MENU_QUIT])
						{
							m_pScene[SCENETYPE_MENU_QUIT]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
						}

						//BGM：ゲームオーバー
						pSound->PlaySoundA(CSound::SOUND_LABEL_BGM004);

						//SE ：チーン
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE019);

						m_PreyEndMode = PREYMODE_NONE;
						m_MenuState = MENUSTATE_RETRY;
					}
				}
			}
		}
		break;
	}

	/* メニューの処理 */
	if (m_MenuState != MENUSTATE_NONE)
	{//NONEじゃないときだけ操作可能にする
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true || CManager::GetInputXPad()->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true)
		{
			m_nSelectNum = (m_nSelectNum + 1) % 2;
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true || CManager::GetInputXPad()->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
		{
			m_nSelectNum = (m_nSelectNum + 1) % 2;
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
		}

		for (int nCnt = 0; nCnt < MAX_PRESS_BUTTON; nCnt++)
		{
			if (m_apPress[nCnt] != NULL)
			{
				m_apPress[nCnt]->Setpos(D3DXVECTOR3((PREY_POS_X_KEY + (nCnt * PREY_POS_X_DIFFERENCE)) + (m_nSelectNum * PREY_POS_Y_NUM_DIF), SCREEN_HEIGHT / 2 + PREY_POS_Y_DIFFERENCE, 0.0f));
				m_apPress[nCnt]->SetType(CPress::TYPE_NONE);
			}
		}

		/* メニューが選ばれている時の処理 */
		if (m_nSelectNum == 0)
		{//リトライ
			m_MenuState = MENUSTATE_RETRY;
		}
		else if (m_nSelectNum == 1)
		{//クイット
			m_MenuState = MENUSTATE_QUIT;
		}
	}
	switch (m_MenuState)
	{//選ばれてるモード別の処理
	case MENUSTATE_NONE:
		break;

	case MENUSTATE_RETRY:
		m_MenuCol = MENUSTATECOLOR_RETRY;
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードしていない状態
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
			{// キーボード（ENTER）を入力したら
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				CFade::Create(CManager::MODE_GAME, CFade::FADE_OUT);
				m_MenuState = MENUSTATE_NONE;
				pSound->StopSound(CSound::SOUND_LABEL_BGM004);
			}
		}
		break;

	case MENUSTATE_QUIT:
		m_MenuCol = MENUSTATECOLOR_QUIT;
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードしていない状態
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
			{// キーボード（ENTER）を入力したら
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
				m_MenuState = MENUSTATE_NONE;
				pSound->StopSound(CSound::SOUND_LABEL_BGM004);
			}
		}
		break;
	}
	switch (m_MenuCol)
	{//選ばれてるモードの色と位置
	case MENUSTATECOLOR_NONE:
		break;

	case MENUSTATECOLOR_RETRY:
		if (m_pScene[SCENETYPE_MENU_RETRY] != NULL)
		{
			m_pScene[SCENETYPE_MENU_RETRY]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
		}
		if (m_pScene[SCENETYPE_MENU_QUIT] != NULL)
		{
			m_pScene[SCENETYPE_MENU_QUIT]->Setcol(DEFAULT_COLOR_HALFCLEAN_WHITE);
		}
		break;

	case MENUSTATECOLOR_QUIT:
		if (m_pScene[SCENETYPE_MENU_RETRY] != NULL)
		{
			m_pScene[SCENETYPE_MENU_RETRY]->Setcol(DEFAULT_COLOR_HALFCLEAN_WHITE);
		}
		if (m_pScene[SCENETYPE_MENU_QUIT] != NULL)
		{
			m_pScene[SCENETYPE_MENU_QUIT]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
		}
		break;
	}

	/* 演出スキップの処理 */
	if (m_bSkipProduction == false)
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードしていない状態
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
			{// キーボード（ENTER）を入力したら
				SkipProduction();
				m_bSkipProduction = true;
			}
		}
	}
}
//**************************************************************
//		描画処理
//**************************************************************
void CPreyEnd::Draw(void)
{

}
//**************************************************************
//		上下移動処理
//**************************************************************
void CPreyEnd::MoveScene(SceneType type)
{
	if (m_pScene[type] != NULL)
	{/* 魂の処理 */
		D3DXVECTOR3 pos = m_pScene[type]->Getpos();

		if (m_fCntMove >= 0.5f || m_fCntMove <= -0.5f)
		{
			m_fChengeMove *= -1.0f;//折り返し
		}
		m_fCntMove += m_fChengeMove;
		pos.x += m_fCntMove;

		m_pScene[type]->Setpos(pos);
	}
}
//**************************************************************
//		演出飛ばした時の処理
//**************************************************************
void CPreyEnd::SkipProduction(void)
{
	CSound *pSound = CManager::GetSound();

	if (m_pScene[SCENETYPE_CLOUD] != NULL)
	{//雲
		m_pScene[SCENETYPE_CLOUD]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}
	if (m_pScene[SCENETYPE_WOOD] != NULL)
	{//森
		m_pScene[SCENETYPE_WOOD]->Setpos(D3DXVECTOR3(m_pScene[SCENETYPE_WOOD]->Getpos().x, SCREEN_HEIGHT + 200.0f, 0.0f));
	}
	if (m_pScene[SCENETYPE_GROUND] != NULL)
	{//地面
		m_pScene[SCENETYPE_GROUND]->Setcol(DEFAULT_COLOR_CLEAN_WHITE);
	}
	if (m_pScene[SCENETYPE_PLAYER] != NULL)
	{//プレイヤー
		m_pScene[SCENETYPE_PLAYER]->Setpos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		m_pScene[SCENETYPE_PLAYER]->SetTex(1, 1, D3DXVECTOR2(0.5f, 1.0f));
		m_pScene[SCENETYPE_PLAYER]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}
	if (m_pScene[SCENETYPE_SOUL] != NULL)
	{//魂
		m_pScene[SCENETYPE_SOUL]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}
	if (m_pScene[SCENETYPE_OPTION] != NULL)
	{//驚き記号
		m_pScene[SCENETYPE_OPTION]->Setcol(DEFAULT_COLOR_CLEAN_WHITE);
	}
	if (m_pScene[SCENETYPE_LOGO] != NULL)
	{//ロゴ
		m_pScene[SCENETYPE_LOGO]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}
	if (m_pScene[SCENETYPE_MENU_RETRY])
	{//リトライ
		m_pScene[SCENETYPE_MENU_RETRY]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}
	if (m_pScene[SCENETYPE_MENU_QUIT])
	{//クイット
		m_pScene[SCENETYPE_MENU_QUIT]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
	}

	//BGM：ゲームオーバー
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM004);
	//SE ：チーン
	pSound->PlaySoundA(CSound::SOUND_LABEL_SE019);

	m_PreyEndMode = PREYMODE_NONE;
	m_MenuState = MENUSTATE_RETRY;
}
//**************************************************************
//		雲のスクロール
//**************************************************************
void CPreyEnd::ScrollCloud(float Scroll)
{
	if (m_pScene[SCENETYPE_CLOUD] != NULL)
	{//雲Nullチェック
		m_pScene[SCENETYPE_CLOUD]->SetTex(1, 1, D3DXVECTOR2(Scroll, 1.0f));
	}
}
//=============================================================================
// ▽CEscapeEnd :(逃走ED)
//=============================================================================
//**************************************************************
//		コンストラクタ
//**************************************************************
CEscapeEnd::CEscapeEnd()
{
	for (int nCnt = 0; nCnt < MAX_ESCAPEMODE_POLYGON; nCnt++)
	{
		m_pScene[nCnt] = NULL;
	}
	m_fScrollCloud = 0.0f;
	m_fCntMove = 0.0f;
	m_fChengeMove = 0.001f;
}
//**************************************************************
//		デストラクタ
//**************************************************************
CEscapeEnd::~CEscapeEnd()
{

}
//**************************************************************
//				読み込むテクスチャ
//**************************************************************
HRESULT CEscapeEnd::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, RESULT_ESCAPE_SKY, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, RESULT_ESCAPE_BG, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, RESULT_PREY_CLOUD, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, RESULT_ESCAPE_ENEMY, &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, RESULT_ESCAPE_PLAYER, &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, RESULT_FALLING_LOGO, &m_pTexture[5]);

	return S_OK;
}

//**************************************************************
//				開放するテクスチャ
//**************************************************************
void CEscapeEnd::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_ESCAPEMODE_TEXTURE; nCnt++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//**************************************************************
//		初期化処理
//**************************************************************
HRESULT CEscapeEnd::Init()
{
	CSound *pSound = CManager::GetSound();
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM003);

	for (int nCnt = 0; nCnt < MAX_ESCAPEMODE_POLYGON; nCnt++)
	{
		if (m_pScene[nCnt] == NULL)
		{
			if (nCnt == SCENETYPE_SKY)
			{//空
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_BG, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), DEFAULT_COLOR_OPACITY_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_BG)
			{//背景
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_2, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), DEFAULT_COLOR_OPACITY_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_CLOUD)
			{//雲
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_BG, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(500.0f, 200.0f, 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f), DEFAULT_COLOR_OPACITY_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->Setrot(0.2f);
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_ENEMY)
			{//敵
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_1, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(180.0f, 380.0f, 0.0f),
					D3DXVECTOR3(150.0f, 150.0f, 0.0f), DEFAULT_COLOR_OPACITY_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_PLAYER)
			{//プレイヤー
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_3, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(980.0f, 405.0f, 0.0f),
					D3DXVECTOR3(250.0f, 400.0f, 0.0f), DEFAULT_COLOR_OPACITY_WHITE, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->Setrot(-0.2f);
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}
			else if (nCnt == SCENETYPE_LOGO)
			{//ロゴ
				m_pScene[nCnt] = new CScene2D(CScene::PRIORITY_4, CScene::OBJTYPE_RESULT);
				m_pScene[nCnt]->SetInitAll(D3DXVECTOR3(320.0f, 80.0f, 0.0f),
					D3DXVECTOR3(300.0f, 80.0f, 0.0f), DEFAULT_COLOR_CLEAN_WHITE, D3DXVECTOR2(1.0f, 0.5f));
				m_pScene[nCnt]->Init();
				m_pScene[nCnt]->SetTex(1, 1, D3DXVECTOR2(1.0f, 1.0f));
				m_pScene[nCnt]->BindTexture(m_pTexture[nCnt]);
			}

		}
	}

	return S_OK;
}
//**************************************************************
//		終了処理
//**************************************************************
void CEscapeEnd::Uninit(void)
{
	CSound *pSound = CManager::GetSound();

	for (int nCnt = 0; nCnt < MAX_ESCAPEMODE_POLYGON; nCnt++)
	{
		if (m_pScene[nCnt] != NULL)
		{	// NULLじゃない場合
			m_pScene[nCnt]->Uninit();
			m_pScene[nCnt] = NULL;
		}
	}

	CResult::Uninit();
}
//**************************************************************
//		更新処理
//**************************************************************
void CEscapeEnd::Update(void)
{
	CSound *pSound = CManager::GetSound();

	/* 雲の処理 */
	m_fScrollCloud += 0.001f;
	ScrollCloud(m_fScrollCloud);
	CntDownFadeTime();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{
		if (m_pScene[SCENETYPE_LOGO] != NULL)
		{
			/* ロゴの処理 */
			D3DXCOLOR col = m_pScene[SCENETYPE_LOGO]->Getcol();
			float FadeCol = 0.1f;

			col.a += FadeCol;

			m_pScene[SCENETYPE_LOGO]->Setcol(col);

			if (col.a >= 1.0f)
			{
				m_pScene[SCENETYPE_LOGO]->Setcol(DEFAULT_COLOR_OPACITY_WHITE);
			}
		}
	}
	if (m_pScene[SCENETYPE_PLAYER] != NULL)
	{
		/* プレイヤーの処理 */
		float rot = m_pScene[SCENETYPE_PLAYER]->Getrot();

		if (m_fCntMove >= 0.015f || m_fCntMove <= -0.015f)
		{
			m_fChengeMove *= -1.0f;//折り返し
		}

		m_fCntMove += m_fChengeMove;
		rot += m_fCntMove;

		m_pScene[SCENETYPE_PLAYER]->Setrot(rot);
	}
	if (m_pScene[SCENETYPE_ENEMY] != NULL)
	{
		/* 敵の処理 */
		D3DXVECTOR3 size = m_pScene[SCENETYPE_ENEMY]->Getsize();
		D3DXVECTOR3 pos = m_pScene[SCENETYPE_ENEMY]->Getpos();
		float Shrink = 0.2f;

		D3DXVECTOR3 Scal = D3DXVECTOR3(size.x - Shrink, size.y - Shrink, 0.0f);
		D3DXVECTOR3 NextPos = D3DXVECTOR3(pos.x - Shrink, pos.y + Shrink + Shrink, 0.0f);

		m_pScene[SCENETYPE_ENEMY]->Setsize(Scal);
		m_pScene[SCENETYPE_ENEMY]->Setpos(NextPos);
		if (Scal.x <= 0.0f&&Scal.y <= 0.0f)
		{//小さくなりきった
			m_pScene[SCENETYPE_ENEMY]->Setsize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	if (CFade::GetFade() == CFade::FADE_NONE)
	{// フェードしていない状態
		if (CManager::GetInputKeyboard()->GetALL(1) || CManager::GetInputXPad()->GetALL(1, 0) == true)
		{// キーボード（ENTER）を入力したら
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
			CFade::Create(CManager::MODE_RANKING, CFade::FADE_OUT);
		}
	}
}
//**************************************************************
//		描画処理
//**************************************************************
void CEscapeEnd::Draw(void)
{

}
//**************************************************************
//		雲のスクロール
//**************************************************************
void CEscapeEnd::ScrollCloud(float Scroll)
{
	if (m_pScene[SCENETYPE_CLOUD] != NULL)
	{//雲Nullチェック
		m_pScene[SCENETYPE_CLOUD]->SetTex(1, 1, D3DXVECTOR2(Scroll, 1.0f));
	}
}
