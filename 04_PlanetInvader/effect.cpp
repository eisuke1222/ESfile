//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "effect.h"			// エフェクト
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "camera.h"			// カメラ

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\effect000.jpg"		// テクスチャ1
#define TEXTURE_NAME_2	"data\\TEXTURE\\effect000.png"		// テクスチャ2
#define TEXTURE_NAME_3	"data\\TEXTURE\\smoke000.png"		// テクスチャ3
#define TEXTURE_NAME_4	"data\\TEXTURE\\tank000.png"		// テクスチャ4

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		CEffect::m_pTexture[CEffect::TEXTURE_MAX] = {};			// テクスチャ情報へのポインタ

//=============================================================================
// コンストラクタ									(public)	*** CEffect ***
//=============================================================================
CEffect::CEffect() : CScene3D(CScene::PRIORITY_2)
{
	m_effectType	= EFFECTTYPE_NONE;						// エフェクト
	m_nCntTimer		= 0;									// 時間カウンタ
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		// 移動量
	m_sizeDown		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		// 大きさ減少
	m_faDown		= 0.0f;									// アルファ減少
}

//=============================================================================
// コンストラクタ									(public)	*** CEffect ***
//=============================================================================
CEffect::CEffect(int) : CScene3D(CScene::PRIORITY_3)
{
	m_effectType	= EFFECTTYPE_NONE;						// エフェクト
	m_nCntTimer		= 0;									// 時間カウンタ
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		// 移動量
	m_sizeDown		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		// 大きさ減少
	m_faDown		= 0.0f;									// アルファ減少
}

//=============================================================================
// デストラクタ										(public)	*** CEffect ***
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ロード処理							(public)	*** CEffect ***
//=============================================================================
HRESULT CEffect::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture[0] == NULL)
	{// NULLの場合
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pTexture[1]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_pTexture[2]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_4, &m_pTexture[3]);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CEffect ***
//=============================================================================
void CEffect::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// カウント
		if (m_pTexture[nCntTex] != NULL)
		{// NULL以外の場合
			m_pTexture[nCntTex]->Release();		// 解放
			m_pTexture[nCntTex] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 確保処理											(public)	*** CEffect ***
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR3 sizeDown, D3DXCOLOR col, float faDown, EFFECTTYPE effectType, TEXTURE texture)
{
	CEffect *pEffect = NULL;				// ポインタ

	if (pEffect == NULL)
	{// NULLの場合
		if (effectType == EFFECTTYPE_PLAYER_T || effectType == EFFECTTYPE_EXPLOSION)
		{// タイトルのみ
			pEffect = new CEffect(0);			// メモリ確保
		}
		else
		{// その他
			pEffect = new CEffect;				// メモリ確保
		}
	}

	if (pEffect != NULL)
	{// NULL以外の場合
		pEffect->Init(pos, rot, move, size, sizeDown, col, faDown, effectType, texture);		// 初期化処理
		pEffect->SetObjType(CScene::OBJTYPE_EFFECT);							// オブジェクトタイプ設定
		pEffect->BindTexture(m_pTexture[texture]);								// テクスチャ設定
	}

	return pEffect;		// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CEffect ***
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR3 sizeDown, D3DXCOLOR col, float faDown, EFFECTTYPE effectType, TEXTURE texture)
{
	CScene3D::SetInitAll(pos, rot, size, col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);		// 初期値設定
	CScene3D::Init();																													// 初期化処理

	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();		// カメラ注視点取得

	m_TexNum = texture;									// テクスチャ番号
	m_pos = pos - CameraposR;							// カメラ移動無効化
	m_effectType = effectType;							// エフェクトタイプ
	m_moveInit = move;									// 移動量初期
	m_sizeDown = sizeDown;								// 大きさ減少
	m_faDown = faDown;									// アルファ減少

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CEffect ***
//=============================================================================
void CEffect::Uninit(void)
{
	CScene3D::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CEffect ***
//=============================================================================
void CEffect::Update(void)
{	
	CManager::MODE mode = CManager::GetMode();						// モード取得
	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();		// カメラ注視点取得
	D3DXVECTOR3 CameraposV = CManager::GetCamera()->GetposV();		// カメラ注視点取得
	D3DXVECTOR3 pos = CScene3D::Getpos();		// 位置を取得
	D3DXVECTOR3 size = CScene3D::Getsize();		// 大きさを取得
	D3DXCOLOR col = CScene3D::Getcol();			// 色を取得

	bool bDelete = false;						// 破棄

	if (mode == CManager::MODE_TUTORIAL || mode == CManager::MODE_GAME)
	{
		// カメラに対しての位置を設定
		pos.x = CameraposR.x + m_pos.x;
		pos.z = CameraposR.z + m_pos.z;
	}
	
	m_nCntTimer++;								// カウント

	if (m_effectType == EFFECTTYPE_EXPLOSION)
	{
		if (m_nCntTimer < 30)
		{
			m_move.x = m_moveInit.x;
			m_move.y = (m_moveInit.y - m_move.y) * (m_nCntTimer * 0.03f);
			m_move.z = m_moveInit.z;
		}
	}
	else
	{
		m_move = m_moveInit;
	}

	pos += m_move;								// 移動量を代入
	
	col.a -= m_faDown;							// アルファ減少

	size += m_sizeDown;							// 大きさを減少

	CScene3D::Setpos(pos);						// 位置を設定
	CScene3D::Setsize(size);					// 大きさを設定
	CScene3D::Setcol(col);						// 色を設定
	
	if (mode == CManager::MODE_TUTORIAL || mode == CManager::MODE_GAME)
	{// ゲームのみ
		m_pos = pos - CameraposR;				// 現在位置を計算
	}

	CScene3D::Update();							// 更新処理

	if (size.x < 0 || size.y < 0 || col.a < 0.0f)
	{// 大きさが０より小さい
		bDelete = true;				// 破棄する
	}

	if (mode != CManager::MODE_TITLE)
	{
		if (pos.x < CameraposR.x - 260.0f || CameraposR.x + 260.0f < pos.x || pos.z < CameraposR.z - 400.0f || CameraposR.z + 250.0f < pos.z)
		{// カメラ範囲外に出たら
			bDelete = true;				// 破棄する
		}
	}

	if (bDelete == true)
	{// 破棄する
		Uninit();					// 終了処理
	}
}

//=============================================================================
// 描画処理											(public)	*** CEffect ***
//=============================================================================
void CEffect::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//比較方法（より大きい））
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (m_TexNum == TEXTURE_EFFECT)
	{
		// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	CScene3D::Draw();				// 描画処理

	if (m_TexNum == TEXTURE_EFFECT)
	{
		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//比較方法（より大きい））
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}