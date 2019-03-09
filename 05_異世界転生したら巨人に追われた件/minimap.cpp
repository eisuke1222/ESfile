//=============================================================================
//
// ミニマップ処理 [minimap.cpp]
// Author : Shun Yokomichi
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "minimap.h"		// ミニマップ
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "player.h"			// プレイヤー
#include "game.h"			// ゲーム
#include "enemy.h"			

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME000		"data\\TEXTURE\\game\\player.png"	// テクスチャ
#define TEXTURE_NAME001		"data\\TEXTURE\\game\\enemy.png"	// テクスチャ
#define TEX_UV_U			(1.0f)								// テクスチャUV_U
#define TEX_UV_V			(1.0f)								// テクスチャUV_V
#define POS_X_DIVISION		(30.0f)								// POS_Xの除算の数字

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			CMinimap::m_pTexture[MAX_MINI] = {};			// テクスチャ情報へのポインタ

//=============================================================================
// コンストラクタ									(public)	*** CMinimap ***
//=============================================================================
CMinimap::CMinimap() : CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_NUMBER)
{

}

//=============================================================================
// デストラクタ										(public)	*** CMinimap ***
//=============================================================================
CMinimap::~CMinimap()
{

}

//=============================================================================
// ロード処理							(public)	*** CMinimap ***
//=============================================================================
HRESULT CMinimap::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < MAX_MINI; nCntTex++)
	{
		if (m_pTexture[nCntTex] == NULL)
		{
			// テクスチャの生成
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME000, &m_pTexture[0]);
			D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME001, &m_pTexture[1]);
		}
	}


	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CMinimap ***
//=============================================================================
void CMinimap::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_MINI; nCntTex++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCntTex] != NULL)
		{// NULL以外の場合
			m_pTexture[nCntTex]->Release();		// 解放
			m_pTexture[nCntTex] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 生成処理										(public)	*** CMinimap ***
//=============================================================================
CMinimap *CMinimap::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type, int nTex)
{
	CMinimap *pMinimap = NULL;

	// シーンの生成
	if (pMinimap == NULL)
	{
		pMinimap = new CMinimap;
		if (pMinimap != NULL)
		{
			pMinimap->Init(pos, size, col, type, nTex);	// scene3Dの初期化へ
		}
	}

	return pMinimap;
}

//=============================================================================
// 初期化処理										(public)	*** CMinimap ***
//=============================================================================
HRESULT CMinimap::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type, int nTex)
{
	CScene2D::SetInitAll(pos, size, col, D3DXVECTOR2(TEX_UV_U, TEX_UV_V));		// 初期値設定
	CScene2D::Init();															// 初期化処理
	CScene2D::BindTexture(m_pTexture[nTex]);											// テクスチャ反映
	m_Type = type;
	m_fPosX = 0.0f;

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CNumber2D ***
//=============================================================================
void CMinimap::Uninit(void)
{
	CScene2D::Uninit();					// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CNumber2D ***
//=============================================================================
void CMinimap::Update(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ローカルposの初期化
	if (m_Type == TYPE_ENEMY)
	{
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_3);
		CScene *pSceneNext = NULL;

		while (pScene != NULL)
		{
			pSceneNext = pScene->GetSceneNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{//	オブジェクトがモデルだったら
				CEnemy *pEnemy = (CEnemy*)pScene;
				pos = pEnemy->Getpos();
				
			}
			pScene = pSceneNext;
		}
	}
	else if (m_Type == TYPE_PLAYER)
	{
		CPlayer *pPlayer = CGame::GetPlayer();	// プレイヤーを取得
		*pos = *pPlayer->Getpos();				// プレイヤーの位置を取得
	}

	switch (m_Type)
	{
	case TYPE_PLAYER:
		// プレイヤーの場合
		m_fPosX = (pos.x / POS_X_DIVISION) + 330.0f;	// 計算保持用変数
		break;

	case TYPE_ENEMY:
		// 巨人の場合
		m_fPosX = (pos.x / POS_X_DIVISION) + 330.0f;	// 計算保持用変数
		break;
	}

	//	範囲指定
	if (m_fPosX >= 800.0f)
	{
		m_fPosX = 800.0f;
	}
	if (m_fPosX <= 330.0f)
	{
		m_fPosX = 330.0f;
	}

	CScene2D::Setpos(D3DXVECTOR3(m_fPosX, 70.0f, 0.0f));

	CScene2D::Update();					// 更新処理
}

//=============================================================================
// 描画処理											(public)	*** CNumber2D ***
//=============================================================================
void CMinimap::Draw(void)
{
	CScene2D::Draw();					// 描画処理
}