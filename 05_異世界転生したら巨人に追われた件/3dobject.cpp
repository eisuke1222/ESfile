//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"		// シーン3D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "font.h"			// フォント
#include "camera.h"			// カメラ
#include "3dobject.h"		// 3dオブジェクト
#include "itemicon.h"		// アイテムのアイコン
#include "life.h"			// 体力表示
#include "score.h"			// スコア
#include "effect.h"			// エフェクト

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
LPDIRECT3DTEXTURE9 C3Dobject::m_pTexture[TYPE_MAX] = { NULL };		// テクスチャ情報へのポインタ

//=============================================================================
// コンストラクタ								(public)	*** CItem ***
//=============================================================================
C3Dobject::C3Dobject() : CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_3DOBJECT)
{
	// 初期値設定
	m_Type = {};		// 種類
}

//=============================================================================
// デストラクタ									(public)	*** CItem ***
//=============================================================================
C3Dobject::~C3Dobject()
{

}

//=============================================================================
//	アイテムの生成
//=============================================================================
C3Dobject *C3Dobject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	C3Dobject *p3Dobject = NULL;		// ポインタ

	if (p3Dobject == NULL)
	{//	アイテムの生成
		p3Dobject = new C3Dobject;
		if (p3Dobject != NULL)
		{//アイテムの初期化
			p3Dobject->Init(pos, rot, size, col, TexUV, nType);
		}
	}

	return p3Dobject;		// 値を返す
}

//=============================================================================
// 初期化処理									(public)	*** CItem ***
//=============================================================================
HRESULT C3Dobject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	// 値設定
	CScene3D::SetInitAll(pos + D3DXVECTOR3(0.0f, size.y,0.0f), rot, size, col, TexUV, CScene3D::SCENE3DTYPE_BILLBOARD);
	CScene3D::Init();
	CScene3D::BindTexture(m_pTexture[nType]);
	m_Type = (OBJECTTYPE)nType;
	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CItem ***
//=============================================================================
void C3Dobject::Uninit(void)
{
	CScene3D::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CItem ***
//=============================================================================
void C3Dobject::Update(void)
{
	CManager::MODE mode = CManager::GetMode();		// モード取得

	if (mode == CManager::MODE_GAME)
	{// ゲームの場合
		D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// 視点取得
		D3DXVECTOR3 pos = CScene3D::Getpos();							// 位置取得
		D3DXVECTOR3 size = CScene3D::Getsize();							// 大きさ取得
		if (pos.x + size.x < posVDest.x - 800.0f)
		{// 範囲チェック
			Uninit();		// 終了処理
		}
	}
}

//=============================================================================
// 描画処理										(public)	*** CItem ***
//=============================================================================
void C3Dobject::Draw(void)
{
	CScene3D::Draw();		// 描画処理
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT C3Dobject::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree000.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree001.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree002.png", &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree002.png", &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\kusa000.png", &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\kusa001.png", &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\iwa000.png", &m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\iwa001.png", &m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\bamboo001.png", &m_pTexture[8]);

	return S_OK;
}
//=============================================================================
// アンロード処理								(public)	*** CItem ***
//=============================================================================
void C3Dobject::Unload(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// 種類カウント
		// テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{// NULL以外の場合
			m_pTexture[nCount]->Release();		// 開放
			m_pTexture[nCount] = NULL;			// NULLへ
		}
	}
}
