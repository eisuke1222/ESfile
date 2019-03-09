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
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "sound.h"			// サウンド
#include "fade.h"			// フェード
#include "texture.h"		// テクスチャ
#include "texanim.h"		// テクスチャアニメーション
#include "text.h"			// テキスト
#include "map.h"			// マップ
#include "player.h"			// プレイヤー
#include "enemy.h"			// 敵

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
// コンストラクタ										(public)	*** CEffect ***
//=============================================================================
CEffect::CEffect() : CScene(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT)
{
	// 初期値設定
	m_nShot = SHOT_NONE;							// 撃った人
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ
	m_move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ
	m_col	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
}

//=============================================================================
// デストラクタ										(public)	*** CEffect ***
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ロード処理										(public)	*** CEffect ***
//=============================================================================
HRESULT CEffect::Load(void)
{
	CEffect_C04::Load();

	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CEffect ***
//=============================================================================
void CEffect::Unload(void)
{
	CEffect_C04::Unload();
}

//=============================================================================
// クリエイト										(public)	*** CEffect ***
//=============================================================================
CEffect *CEffect::Create(TYPE type, SHOT shot, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CEffect *pEffect = NULL;			// ポインタ

	if (pEffect == NULL)
	{// NULLの場合
		if(type == TYPE_2D)				// エフェクト2D
			pEffect = new CEffect2D;		// メモリ確保
		else if (type == TYPE_3D)		// エフェクト3D
			pEffect = new CEffect3D;		// メモリ確保
		else if (type == TYPE_C00)		// エフェクト_カスタム00
			pEffect = new CEffect_C00;		// メモリ確保
		else if (type == TYPE_C01)		// エフェクト_カスタム01
			pEffect = new CEffect_C01;		// メモリ確保
		else if (type == TYPE_C02)		// エフェクト_カスタム02
			pEffect = new CEffect_C02;		// メモリ確保
		else if (type == TYPE_C03)		// エフェクト_カスタム03
			pEffect = new CEffect_C03;		// メモリ確保
		else if (type == TYPE_C04)		// エフェクト_カスタム04
			pEffect = new CEffect_C04;		// メモリ確保
		else if (type == TYPE_FIRE)		// エフェクト_火
			pEffect = new CEffect_Fire;		// メモリ確保
		else if (type == TYPE_CREATE)	// エフェクト_クリエイト
			pEffect = new CEffect_Create;	// メモリ確保
	}

	if (pEffect != NULL)
	{// NULL以外の場合
		pEffect->m_nShot	= shot;		// 撃った人
		pEffect->m_pos		= pos;		// 位置
		pEffect->m_rot		= rot;		// 回転
		pEffect->m_move		= move;		// 移動量
		pEffect->m_size		= size;		// 大きさ
		pEffect->m_col		= col;		// 色
		pEffect->Init();				// 初期化処理
	}

	return pEffect;		// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CEffect ***
//=============================================================================
void CEffect::Uninit(void)
{
	CScene::SetDeath();		// 死亡フラグ
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CEffect2D ***
//=============================================================================
CEffect2D::CEffect2D() : CEffect()
{
	// 初期値設定
	m_pEffect2D = NULL;		// エフェクト2D
}

//=============================================================================
// デストラクタ									(public)	*** CEffect2D ***
//=============================================================================
CEffect2D::~CEffect2D()
{

}

//=============================================================================
// ロード処理									(public)	*** CEffect2D ***
//=============================================================================
HRESULT CEffect2D::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::Unload(void)
{

}

//=============================================================================
// 初期化処理									(public)	*** CEffect2D ***
//=============================================================================
HRESULT CEffect2D::Init(void)
{
	return S_OK;	// 値を返す
}	

//=============================================================================
// 終了処理										(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::Uninit(void)
{
	CEffect::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::Update(void)
{

}

//=============================================================================
// 描画処理										(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::Draw(void)
{

}

//=============================================================================
// 判定チェック処理								(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::ColisionChack(void)
{

}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CEffect3D ***
//=============================================================================
CEffect3D::CEffect3D() : CEffect()
{
	// 初期値設定
	m_pEffect3D = NULL;		// エフェクト3D

	m_nCntTimer = 0;		// 時間カウント
	m_nPownTime = 0;		// 生成時間
}

//=============================================================================
// デストラクタ									(public)	*** CEffect3D ***
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// ロード処理									(public)	*** CEffect3D ***
//=============================================================================
HRESULT CEffect3D::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Unload(void)
{

}

//=============================================================================
// 初期化処理									(public)	*** CEffect3D ***
//=============================================================================
HRESULT CEffect3D::Init(void)
{
	if (m_pEffect3D == NULL)
		m_pEffect3D = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);		// メモリ確保

	if (m_pEffect3D != NULL)
	{// NULL以外の場合
		// 初期値設定
		m_pEffect3D->SetInitAll(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		// 初期化処理
		m_pEffect3D->Init();
		// テクスチャ反映
		m_pEffect3D->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_EFFECT));
		// 描画編集
		m_pEffect3D->DrawEdit(true, false, false, true, true);
	}

	m_nCntTimer = 0;
	m_nPownTime = 3;

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Uninit(void)
{
	if (m_pEffect3D != NULL)
	{// NULL以外の場合
		m_pEffect3D->Uninit();	// 終了処理
		m_pEffect3D = NULL;		// NULLへ
	}

	CEffect::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Update(void)
{
	m_pos = m_pEffect3D->Getpos();	// 位置取得（エフェクト）
	m_pos += m_move;				// 移動量を加算
	m_pEffect3D->Setpos(m_pos);		// 位置設定（エフェクト）

	m_nCntTimer++;

	if (m_nCntTimer % m_nPownTime == 0)
		CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col);

	ColisionChack();	// 判定チェック
}

//=============================================================================
// 描画処理										(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Draw(void)
{

}

//=============================================================================
// 判定チェック処理								(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::ColisionChack(void)
{
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_TITLE)
	{
		if (m_pos.x < 160.0f || 690.0f < m_pos.x ||
			m_pos.z < -160.0f || 160.0f < m_pos.z)
		{// 範囲チェック
			Uninit();	// 終了処理
			return;		// 値を返す
		}
	}
	else
	{
		if (m_pos.x < -1500.0f || 1500.0f < m_pos.x ||
			m_pos.z < -1500.0f || 1500.0f < m_pos.z)
		{// 範囲チェック
			Uninit();	// 終了処理
			return;		// 値を返す
		}
	}

	if (mode == CManager::MODE_GAME)
	{// ゲームのみ
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			// マップ
			CMap *pMap = pGame->GetMap();
			if (pMap != NULL)
			{
				int nMapNum = pGame->GetMapNum();
				if (nMapNum == 2)
				{
					CMap_02 *pMap_02 = (CMap_02*)pMap;
					if (pMap_02->CollisionInOut(m_pos) == false)
					{
						Uninit();	// 終了処理
						return;		// 値を返す
					}
				}
				else if (nMapNum == 3)
				{
					CMap_03 *pMap_03 = (CMap_03*)pMap;
					if (pMap_03->CollisionInOut(m_pos) == false)
					{
						Uninit();	// 終了処理
						return;		// 値を返す
					}
				}
				else if (nMapNum == 4)
				{
					CMap_04 *pMap_04 = (CMap_04*)pMap;
					if (pMap_04->CollisionInOut(m_pos) == false)
					{
						Uninit();	// 終了処理
						return;		// 値を返す
					}
				}
			}

			if (m_nShot == SHOT_PLAYER)
			{
				// 敵
				{
					CScene *pScene = NULL;		// シーン
					CScene *pSceneNext = NULL;		// 次シーン

					pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

					while (pScene != NULL)
					{// NULL以外の場合
						pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

						if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
						{// オブジェクトタイプが敵の場合
							CEnemy *pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
							if (pEnemy->Collision(&m_pos, NULL, NULL, m_pEffect3D->Getsize(), m_pEffect3D->Getsize()))
							{// 衝突した
								pEnemy->SetLockOn(true);
								if (pEnemy->HitDamage(-6) == true)
								{// ダメージ処理						
									if (pEnemy->GetHP() == 0)
									{// HPが０になった
										pEnemy->Uninit();		// 終了処理
									}
									else
									{
										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											CPlayer *pPlayer = pGame->GetPlayer();
											if (pPlayer != NULL)
											{
												pPlayer->AddMP(2);
											}
										}
									}
								}

								Uninit();	// 終了処理
								return;		// 値を返す
							}
						}
						pScene = pSceneNext;	// 次シーンへ
					}
				}
			}
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					if (pPlayer->Collision(m_pos) == true)
					{
						pPlayer->AddHP(-10);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CEffect_C00 ***
//=============================================================================
CEffect_C00::CEffect_C00() : CEffect()
{
	// 初期値設定
	m_pEffect3D = NULL;		// エフェクト3D

	m_DeleteTime = 0;

	m_bSizeUp = false;		// 大きく
}

//=============================================================================
// デストラクタ									(public)	*** CEffect_C00 ***
//=============================================================================
CEffect_C00::~CEffect_C00()
{

}

//=============================================================================
// ロード処理									(public)	*** CEffect_C00 ***
//=============================================================================
HRESULT CEffect_C00::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CEffect_C00 ***
//=============================================================================
void CEffect_C00::Unload(void)
{

}

//=============================================================================
// 初期化処理									(public)	*** CEffect_C00 ***
//=============================================================================
HRESULT CEffect_C00::Init(void)
{
	if (m_pEffect3D == NULL)
		m_pEffect3D = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);		// メモリ確保

	if (m_pEffect3D != NULL)
	{// NULL以外の場合
		// 初期値設定
		if (m_bSizeUp == false)
			m_pEffect3D->SetInitAll(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		else
			m_pEffect3D->SetInitAll(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		// 初期化処理
		m_pEffect3D->Init();
		// テクスチャ反映
		m_pEffect3D->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_EFFECT));
		// 描画編集
		m_pEffect3D->DrawEdit(true, false, false, true, true);
	}

	m_DeleteTime = 30;

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CEffect_C00 ***
//=============================================================================
void CEffect_C00::Uninit(void)
{
	if (m_pEffect3D != NULL)
	{// NULL以外の場合
		m_pEffect3D->Uninit();	// 終了処理
		m_pEffect3D = NULL;		// NULLへ
	}

	CEffect::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CEffect_C00 ***
//=============================================================================
void CEffect_C00::Update(void)
{
	if (m_pEffect3D != NULL)
	{// NULL以外の場合
		D3DXVECTOR3 pos = m_pEffect3D->Getpos();
		pos += m_move;

		D3DXVECTOR3 size = m_pEffect3D->Getsize();
		if (m_bSizeUp == false)
		{
			size.x -= (float)(m_size.x / (float)m_DeleteTime);
			size.y -= (float)(m_size.y / (float)m_DeleteTime);
		}
		else
		{
			size.x += (float)(m_size.x / (float)m_DeleteTime);
			size.y += (float)(m_size.y / (float)m_DeleteTime);
		}

		D3DXCOLOR col = m_pEffect3D->Getcol();
		col.a -= (float)(m_col.a / (float)m_DeleteTime);

		if (col.a < 0.0f)
		{
			Uninit();
			return;
		}
		else
		{
			m_pEffect3D->Setpos(pos);
			m_pEffect3D->Setsize(size);
			m_pEffect3D->Setcol(col);
		}
	}
}

//=============================================================================
// 描画処理										(public)	*** CEffect_C00 ***
//=============================================================================
void CEffect_C00::Draw(void)
{

}

//=============================================================================
// 判定チェック処理								(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C00::ColisionChack(void)
{

}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CEffect_C01 ***
//=============================================================================
CEffect_C01::CEffect_C01() : CEffect()
{
	// 初期値設定
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		m_pEffect3D[nCount] = NULL;		// エフェクト3D
		m_frot[nCount] = 0.0f;
	}

	m_nCntTimer = 0;		// 時間カウント
	m_nPownTime = 0;		// 生成時間

	m_bExplosion = false;	// 起爆
}

//=============================================================================
// デストラクタ									(public)	*** CEffect_C01 ***
//=============================================================================
CEffect_C01::~CEffect_C01()
{

}

//=============================================================================
// ロード処理									(public)	*** CEffect_C01 ***
//=============================================================================
HRESULT CEffect_C01::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::Unload(void)
{

}

//=============================================================================
// 初期化処理									(public)	*** CEffect_C01 ***
//=============================================================================
HRESULT CEffect_C01::Init(void)
{
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		m_pEffect3D[nCount] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);		// メモリ確保

		if (m_pEffect3D[nCount] != NULL)
		{// NULL以外の場合
			// 初期値設定
			if (nCount == 0)
			{
				m_pEffect3D[nCount]->SetInitAll(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
			}
			else
			{
				D3DXVECTOR3 pos = m_pos;
				float fAngle = ((D3DX_PI * 2.0f) / (EFFECT_MAX - 1)) * (nCount - 1);
				pos = pos + D3DXVECTOR3(sinf(fAngle), cosf(fAngle), cosf(fAngle)) * 8.0f;
				m_frot[nCount] = fAngle;

				m_pEffect3D[nCount]->SetInitAll(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size * 0.5f, m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
			}
			// 初期化処理
			m_pEffect3D[nCount]->Init();
			// テクスチャ反映
			m_pEffect3D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_EFFECT));
			// 描画編集
			m_pEffect3D[nCount]->DrawEdit(true, false, false, true, true);
		}
	}

	m_nCntTimer = 0;
	m_nPownTime = 3;

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::Uninit(void)
{
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		if (m_pEffect3D[nCount] != NULL)
		{// NULL以外の場合
			m_pEffect3D[nCount]->Uninit();	// 終了処理
			m_pEffect3D[nCount] = NULL;		// NULLへ
		}
	}

	CEffect::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::Update(void)
{
	m_nCntTimer++;

	for (int nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		if (nCount == 0)
		{
			m_pos = m_pEffect3D[nCount]->Getpos();	// 位置取得（エフェクト）
			m_pos += m_move;						// 移動量を加算
			m_pEffect3D[nCount]->Setpos(m_pos);		// 位置設定（エフェクト）

			if (m_nCntTimer % m_nPownTime == 0)
				CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col);
		}
		else
		{
			m_frot[nCount] += 0.1f;

			D3DXVECTOR3 posNext = m_pos + m_move;
			float fAngle = atan2f(posNext.x - m_pos.x, posNext.z - m_pos.z);

			D3DXVECTOR3 pos = m_pos;
			if ((-(D3DX_PI * 0.5f) < fAngle && fAngle < 0.0f) || (D3DX_PI * 0.5f) < fAngle)
			{
				pos = pos + D3DXVECTOR3(sinf(m_frot[nCount]), cosf(m_frot[nCount]), sinf(m_frot[nCount])) * 8.0f;
			}
			else
			{
				pos = pos + D3DXVECTOR3(-sinf(m_frot[nCount]), cosf(m_frot[nCount]), sinf(m_frot[nCount])) * 8.0f;
			}
			pos += m_move;							// 移動量を加算
			m_pEffect3D[nCount]->Setpos(pos);		// 位置設定（エフェクト）

			if (m_nCntTimer % m_nPownTime == 0)
				CEffect::Create(CEffect::TYPE_C00, m_nShot, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size * 0.5f, m_col);
		}
	
		if (m_pos.x < -1500.0f || 1500.0f < m_pos.x ||
			m_pos.z < -1500.0f || 1500.0f < m_pos.z)
		{// 範囲チェック
			Uninit();	// 終了処理
			return;		// 値を返す
		}
	}

	ColisionChack();	// 判定チェック			
}

//=============================================================================
// 描画処理										(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::Draw(void)
{

}

//=============================================================================
// 判定チェック処理								(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::ColisionChack(void)
{
	CSound			*pSound = CManager::GetSound();				// サウンド取得
	CManager::MODE	 mode = CManager::GetMode();

	if (mode == CManager::MODE_TITLE)
	{
		if (m_pos.x < 160.0f || 690.0f < m_pos.x ||
			m_pos.z < -160.0f || 160.0f < m_pos.z)
		{// 範囲チェック
			Uninit();	// 終了処理
			return;		// 値を返す
		}
	}
	else
	{
		if (m_pos.x < -1500.0f || 1500.0f < m_pos.x ||
			m_pos.z < -1500.0f || 1500.0f < m_pos.z)
		{// 範囲チェック
			Uninit();	// 終了処理
			return;		// 値を返す
		}
	}

	if (mode == CManager::MODE_GAME)
	{// ゲームのみ
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			// マップ
			CMap *pMap = pGame->GetMap();
			if (pMap != NULL)
			{
				int nMapNum = pGame->GetMapNum();
				if (nMapNum == 2)
				{
					CMap_02 *pMap_02 = (CMap_02*)pMap;
					if (pMap_02->CollisionInOut(m_pos) == false)
					{
						Uninit();	// 終了処理
						return;		// 値を返す
					}
				}
				else if (nMapNum == 3)
				{
					CMap_03 *pMap_03 = (CMap_03*)pMap;
					if (pMap_03->CollisionInOut(m_pos) == false)
					{
						Uninit();	// 終了処理
						return;		// 値を返す
					}
				}
				else if (nMapNum == 4)
				{
					CMap_04 *pMap_04 = (CMap_04*)pMap;
					if (pMap_04->CollisionInOut(m_pos) == false)
					{
						Uninit();	// 終了処理
						return;		// 値を返す
					}
				}
			}
		}
	}
	CScene *pScene = NULL;		// シーン
	CScene *pSceneNext = NULL;		// 次シーン

	float fHeight = 0.0f;

	// フィールド
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// シーン取得

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();			// 次シーンを記憶

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// オブジェクトタイプがメッシュフィールドの場合
				CSceneMesh *pMesh = (CSceneMesh*)pScene;	// シーンメッシュの型にキャスト

				if (pMesh->ColInMesh(m_pos) == true)
				{// 範囲チェック
					fHeight = pMesh->GetHeight(m_pos);		// 高さ取得

					if (m_pos.y < fHeight)
					{
						CEffect::Create(CEffect::TYPE_C02, m_nShot, D3DXVECTOR3(m_pos.x, fHeight, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_RANGE000);

						Uninit();	// 終了処理
						return;		// 値を返す
					}
					break;
				}
			}
			pScene = pSceneNext;	// 次シーンへ
		}
	}

	if (mode == CManager::MODE_GAME)
	{// ゲームのみ
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			if (m_nShot == SHOT_PLAYER)
			{
				// 敵
				{
					pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

					while (pScene != NULL)
					{// NULL以外の場合
						pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

						if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
						{// オブジェクトタイプが敵の場合
							CEnemy *pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
							if (pEnemy->Collision(&m_pos, NULL, NULL, m_pEffect3D[0]->Getsize(), m_pEffect3D[0]->Getsize()))
							{// 衝突した
								pEnemy->SetLockOn(true);
								if (pEnemy->HitDamage(-15) == true)
								{// ダメージ処理
									if (pEnemy->GetHP() == 0)
									{// HPが０になった
										pEnemy->Uninit();		// 終了処理

										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											pGame->SetSS(true);
										}
									}
									else
									{
										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											CPlayer *pPlayer = pGame->GetPlayer();
											if (pPlayer != NULL)
											{
												pPlayer->AddMP(3);
											}
										}
									}
								}

								if (m_bExplosion == true)
								{
									CEffect::Create(CEffect::TYPE_C02, m_nShot, D3DXVECTOR3(m_pos.x, fHeight, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

									if (pSound != NULL)
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_RANGE000);
								}

								Uninit();	// 終了処理
								return;		// 値を返す
							}
						}
						pScene = pSceneNext;	// 次シーンへ
					}
				}
			}
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					if (pPlayer->Collision(m_pos) == true)
					{
						pPlayer->AddHP(-8);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CEffect_C02 ***
//=============================================================================
CEffect_C02::CEffect_C02() : CEffect()
{
	// 初期値設定
	m_pSceneMesh = NULL;						// シーンメッシュ
}

//=============================================================================
// デストラクタ									(public)	*** CEffect_C02 ***
//=============================================================================
CEffect_C02::~CEffect_C02()
{

}

//=============================================================================
// ロード処理									(public)	*** CEffect_C02 ***
//=============================================================================
HRESULT CEffect_C02::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::Unload(void)
{

}

//=============================================================================
// 初期化処理									(public)	*** CEffect_C02 ***
//=============================================================================
HRESULT CEffect_C02::Init(void)
{
	if (m_pSceneMesh == NULL)
	{
		m_pSceneMesh = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);

		m_pSceneMesh->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(20.0f, 10.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, -5.0f, 0.0f), 
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col, D3DXVECTOR2(1.0f, 1.0f));
		m_pSceneMesh->Init();
	}

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::Uninit(void)
{
	if (m_pSceneMesh != NULL)
	{
		m_pSceneMesh->Uninit();
		m_pSceneMesh = NULL;
	}

	CEffect::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::Update(void)
{
	if (m_pSceneMesh != NULL)
	{
		D3DXVECTOR3 size = m_pSceneMesh->Getsize();
		if (size.x < 60.0f)
		{
			size.x += 6.0f;
			size.y += 6.0f;

			m_size = size;
		}
		else
		{
			m_col.a -= 0.05f;
		}

		if (m_col.a < 0.0f)
		{
			Uninit();
			return;
		}
		else
		{
			m_pSceneMesh->SetStart(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, m_col, D3DXVECTOR2(1.0f, 1.0f));
		}

		ColisionChack();
	}
}

//=============================================================================
// 描画処理										(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::Draw(void)
{

}

//=============================================================================
// 判定チェック処理								(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::ColisionChack(void)
{
	CManager::MODE	 mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// ゲームのみ
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			if (m_nShot == SHOT_PLAYER)
			{
				CScene *pScene = NULL;		// シーン
				CScene *pSceneNext = NULL;		// 次シーン

				if (m_pSceneMesh != NULL)
				{
					pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

					while (pScene != NULL)
					{// NULL以外の場合
						pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

						if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
						{// オブジェクトタイプが敵の場合
							CEnemy *pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
							D3DXVECTOR3 pos = pEnemy->Getpos();
							float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
							if (fRange < m_size.x)
							{
								pEnemy->SetLockOn(true);
								if (pEnemy->HitDamage(-12) == true)
								{// ダメージ処理
									float fAngle = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);
									pEnemy->SetMove(D3DXVECTOR3(sinf(fAngle), 0.1f, cosf(fAngle)) * 30.0f);
									pEnemy->SetKnockBack(true);

									if (pEnemy->GetHP() == 0)
									{// HPが０になった
										pEnemy->Uninit();		// 終了処理

										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											pGame->SetSS(true);
										}
									}
									else
									{
										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											CPlayer *pPlayer = pGame->GetPlayer();
											if (pPlayer != NULL)
											{
												pPlayer->AddMP(1);
											}
										}
									}
								}

								//Uninit();
							}
						}
						pScene = pSceneNext;	// 次シーンへ
					}
				}
			}		
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					D3DXVECTOR3 pos = pPlayer->Getpos();
					float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
					if (fRange < m_size.x)
					{
						pPlayer->AddHP(-8);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CEffect_C03 ***
//=============================================================================
CEffect_C03::CEffect_C03() : CEffect()
{
	// 初期値設定
	m_pSceneMesh = NULL;		// シーンメッシュ

	m_nCntTimer = 0;			// 時間カウント
}

//=============================================================================
// デストラクタ									(public)	*** CEffect_C03 ***
//=============================================================================
CEffect_C03::~CEffect_C03()
{

}

//=============================================================================
// ロード処理									(public)	*** CEffect_C03 ***
//=============================================================================
HRESULT CEffect_C03::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::Unload(void)
{

}

//=============================================================================
// 初期化処理									(public)	*** CEffect_C03 ***
//=============================================================================
HRESULT CEffect_C03::Init(void)
{
	CScene *pScene = NULL;		// シーン
	CScene *pSceneNext = NULL;		// 次シーン
	CSceneMesh *pMesh = NULL;

	// フィールド
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// シーン取得

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();			// 次シーンを記憶

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// オブジェクトタイプがメッシュフィールドの場合
				pMesh = (CSceneMesh*)pScene;	// シーンメッシュの型にキャスト

				break;
			}
			pScene = pSceneNext;	// 次シーンへ
		}
	}

	CTexAnim *pTexAnim = NULL; 
	// 魔法陣
	pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size,
		D3DXVECTOR2(0.0714f, 1.0f), m_col, CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
	pTexAnim->SetAnim(14, 14, 3, 0, false, true);

	for (int nCount = 0; nCount < 5; nCount++)
	{
		D3DXVECTOR3 pos = m_pos;
		pos = pos + D3DXVECTOR3(sinf((D3DX_PI * 2.0f / 5) * nCount + m_rot.y), 0.0f, cosf((D3DX_PI * 2.0f / 5) * nCount + +m_rot.y)) * 100.0f;

		if (pMesh != NULL)
		{// 
			if (pMesh->ColInMesh(m_pos) == true)
			{// 範囲チェック
				pos.y = pMesh->GetHeight(pos);
				pos.y += 1.8f;

				if (m_pos.y + 10.0f < pos.y)
				{
					pos.y = m_pos.y;
				}
			}
		}

		// 魔法陣
		pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size,
			D3DXVECTOR2(0.0714f, 1.0f), m_col, CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
		pTexAnim->SetAnim(14, 14, 3, 0, false, true);

		CEffect::Create(CEffect::TYPE_C04, m_nShot, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (m_pSceneMesh == NULL)
	{
		m_pSceneMesh = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);

		m_pSceneMesh->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(20.0f, 10.0f, 0.0f), m_pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(40.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(1.0f, 1.0f));
		m_pSceneMesh->Init();
		m_pSceneMesh->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
		m_pSceneMesh->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
	}

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::Uninit(void)
{
	if (m_pSceneMesh != NULL)
	{
		m_pSceneMesh->Uninit();
		m_pSceneMesh = NULL;
	}

	CEffect::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::Update(void)
{
	if (m_pSceneMesh != NULL)
	{
		D3DXVECTOR3 size = m_pSceneMesh->Getsize();
		if (size.y < 8.0f)
		{
			size.y += 1.0f;

			m_size = size;
		}
		else
		{
			m_col.a -= 0.05f;
		}

		if (m_col.a < 0.0f)
		{
			Uninit();
			return;
		}
		else
		{
			m_pSceneMesh->SetStart(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, m_col, D3DXVECTOR2(1.0f, 1.0f));
		}

		ColisionChack();
	}
}

//=============================================================================
// 描画処理										(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::Draw(void)
{

}

//=============================================================================
// 判定チェック処理								(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::ColisionChack(void)
{
	CManager::MODE	 mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// ゲームのみ
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			if (m_nShot == SHOT_PLAYER)
			{
				CScene *pScene = NULL;		// シーン
				CScene *pSceneNext = NULL;		// 次シーン

				pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

				while (pScene != NULL)
				{// NULL以外の場合
					pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

					if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
					{// オブジェクトタイプが敵の場合
						CEnemy *pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
						D3DXVECTOR3 pos = pEnemy->Getpos();
						float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
						if (fRange < 120.0f)
						{
							float fAngle = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);
							if (pEnemy->GetType() == CEnemy::TYPE_N_SLIME)
								pEnemy->SetMove(D3DXVECTOR3(sinf(fAngle), 0.2f, cosf(fAngle)) * 30.0f);
							else
								pEnemy->SetMove(D3DXVECTOR3(sinf(fAngle), 0.01f, cosf(fAngle)) * 50.0f);

							pEnemy->SetKnockBack(true);
							pEnemy->SetLockOn(true);

							if (pEnemy->HitDamage(-26) == true)
							{// ダメージ処理
								if (pEnemy->GetHP() == 0)
								{// HPが０になった
									pEnemy->Uninit();		// 終了処理

									CGame *pGame = CManager::GetGame();
									if (pGame != NULL)
									{
										pGame->SetSS(true);
									}
								}
								else
								{
									CGame *pGame = CManager::GetGame();
									if (pGame != NULL)
									{
										CPlayer *pPlayer = pGame->GetPlayer();
										if (pPlayer != NULL)
										{
											pPlayer->AddMP(1);
										}
									}
								}
							}

							//Uninit();
						}
					}
					pScene = pSceneNext;	// 次シーンへ
				}
			}
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					D3DXVECTOR3 pos = pPlayer->Getpos();
					float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
					if (fRange < 60.0f)
					{
						pPlayer->AddHP(-10);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CEffect_C04::m_nMaxModel	= 0;		// モデル数

LPDIRECT3DTEXTURE9		**CEffect_C04::m_pTexture	= NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CEffect_C04::m_pMesh		= NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER			*CEffect_C04::m_pBuffMat	= NULL;		// マテリアル情報へのポインタ
DWORD					*CEffect_C04::m_nNumMat		= NULL;		// マテリアル情報の数

CModel::MODEL_INFO		*CEffect_C04::m_ModelInfo	= NULL;		// モデルインフォ

//=============================================================================
// コンストラクタ									(public)	*** CEffect_C04 ***
//=============================================================================
CEffect_C04::CEffect_C04() : CEffect()
{
	// 初期値設定
	m_pModel = NULL;		// モデル

	m_nCntTimer = 0;		// 時間カウント
}

//=============================================================================
// デストラクタ									(public)	*** CEffect_C04 ***
//=============================================================================
CEffect_C04::~CEffect_C04()
{

}

//=============================================================================
// ロード処理									(public)	*** CEffect_C04 ***
//=============================================================================
HRESULT CEffect_C04::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MODEL\\skillmodel.txt", &m_nMaxModel);		// モデル取得

	// 各情報のメモリ確保
	if (m_pTexture	== NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_nMaxModel];
	if (m_pMesh		== NULL) m_pMesh	= new LPD3DXMESH[m_nMaxModel];
	if (m_pBuffMat	== NULL) m_pBuffMat = new LPD3DXBUFFER[m_nMaxModel];
	if (m_nNumMat	== NULL) m_nNumMat	= new DWORD[m_nMaxModel];

	// 各情報の初期化
	for (int nCount = 0; nCount < m_nMaxModel; nCount++)		// モデル数カウント
	{	m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;	}

	for (int nCntParnet = 0; nCntParnet < m_nMaxModel; nCntParnet++)
	{// モデル数カウント
		// Xファイルの読み込み
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::Unload(void)
{
	// 各情報のメモリ開放、NULLへ
	if (m_pTexture	!= NULL) { delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL) { delete[] m_pMesh;	m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL) { delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL) { delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// モデルインフォ
	if (m_ModelInfo	 != NULL)	m_ModelInfo = NULL;		// NULLへ
}

//=============================================================================
// 初期化処理									(public)	*** CEffect_C04 ***
//=============================================================================
HRESULT CEffect_C04::Init(void)
{
	// モデル
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULLの場合
		m_pModel = new CModel;	// メモリ確保

		// 初期値設定
		m_pModel->SetInitAll(m_pTexture[0], m_pMesh[0], m_pBuffMat[0], m_nNumMat[0], m_pos + D3DXVECTOR3(0.0f, -110.0f, 0.0f), D3DXVECTOR3(0.0f, (float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f, 0.0f));
		// 初期化処理
		m_pModel->Init();

		// 親モデル
		m_pModel->SetParent(NULL);
	}

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::Uninit(void)
{
	// モデル
	if (m_pModel != NULL)
	{// NULL以外の場合
		m_pModel->Uninit();		// 終了処理

		delete m_pModel;		// メモリ開放
		m_pModel = NULL;		// NULLへ
	}

	CEffect::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::Update(void)
{
	if (m_pMesh != NULL)
	{
		if (m_nCntTimer < 10)
		{
			D3DXVECTOR3 pos = m_pModel->Getpos();
			pos.y += 10.0f;
			m_pModel->Setpos(pos);
		}
		else if (m_nCntTimer < 11)
		{
			D3DXVECTOR3 pos = m_pModel->Getpos();
			pos.y -= 3.0f;
			m_pModel->Setpos(pos);
		}

		if (30 < m_nCntTimer)
		{
			m_col.a -= 0.1f;
			m_pModel->Setcol_a(m_col.a);
		}

		ColisionChack();

		m_nCntTimer++;

		if (m_nCntTimer == 40)
		{
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理										(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// 計算用マトリックス

	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{// モデル数カウント
	 // ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		if (m_pModel != NULL)
		{// NULL以外の場合
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pModel[nCntModel].Draw();

			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// 影表示
	//		CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(0.0f, 0.1f, 0.0f));
		}
	}
}

//=============================================================================
// 判定チェック処理								(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::ColisionChack(void)
{
	/*CManager::MODE	 mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// ゲームのみ
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			if (m_nShot == SHOT_PLAYER)
			{
				CScene *pScene = NULL;		// シーン
				CScene *pSceneNext = NULL;		// 次シーン

				if (m_pModel != NULL)
				{
					pScene = CScene::GetScene(CScene::PRIORITY_3);	// シーン取得

					while (pScene != NULL)
					{// NULL以外の場合
						pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶

						if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
						{// オブジェクトタイプが敵の場合
							CEnemy *pEnemy = (CEnemy*)pScene;		// 敵の型にキャスト
							D3DXVECTOR3 pos = pEnemy->Getpos();
							float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
							if (fRange < 60.0f)
							{
								if (pEnemy->HitDamage(-10) == true)
								{// ダメージ処理
									if (pEnemy->GetHP() == 0)
									{// HPが０になった
										pEnemy->Uninit();		// 終了処理

										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											pGame->SetSS(true);
										}
									}
									else
									{
										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											CPlayer *pPlayer = pGame->GetPlayer();
											if (pPlayer != NULL)
											{
												pPlayer->AddMP(1);
											}
										}
									}
								}

								//Uninit();
								return;
							}
						}
						pScene = pSceneNext;	// 次シーンへ
					}
				}
			}
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					D3DXVECTOR3 pos = pPlayer->Getpos();
					float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
					if (fRange < 60.0f)
					{
						pPlayer->AddHP(-10);
					}
				}
			}		
		}
	}*/
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CEffect_Fire ***
//=============================================================================
CEffect_Fire::CEffect_Fire() : CEffect()
{
	// 初期値設定
	m_nCntTimer = 0;			// 時間カウント
}

//=============================================================================
// デストラクタ									(public)	*** CEffect_Fire ***
//=============================================================================
CEffect_Fire::~CEffect_Fire()
{

}

//=============================================================================
// ロード処理									(public)	*** CEffect_Fire ***
//=============================================================================
HRESULT CEffect_Fire::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::Unload(void)
{

}

//=============================================================================
// 初期化処理									(public)	*** CEffect_Fire ***
//=============================================================================
HRESULT CEffect_Fire::Init(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::Uninit(void)
{
	CEffect::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::Update(void)
{
	if (m_nCntTimer % 3 == 0)
	{
		CEffect_C00 *pEffect = NULL;
		pEffect = (CEffect_C00*)CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			D3DXVECTOR3((-1.0f + (float)(rand() % 3))* 0.08f, 0.3f, (-1.0f + (float)(rand() % 3))* 0.08f), m_size * 0.6f, D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));
		pEffect->SetDeleteTime(60);
		pEffect->SetSizeUp(true);

		pEffect = (CEffect_C00*)CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3((-1.0f + (float)(rand() % 3))* 0.1f, 0.3f, (-1.0f + (float)(rand() % 3))* 0.1f), m_size * 0.6f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		pEffect->SetDeleteTime(20);

		pEffect = (CEffect_C00*)CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3((-1.0f + (float)(rand() % 3))* 0.1f, 0.5f, (-1.0f + (float)(rand() % 3))* 0.1f), m_size, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		pEffect->SetDeleteTime(25);
	}

	m_nCntTimer++;
}

//=============================================================================
// 描画処理										(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::Draw(void)
{

}

//=============================================================================
// 判定チェック処理								(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::ColisionChack(void)
{
	
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CEffect_Create ***
//=============================================================================
CEffect_Create::CEffect_Create() : CEffect()
{
	// 初期値設定
	m_nCntTimer = 0;			// 時間カウント
}

//=============================================================================
// デストラクタ									(public)	*** CEffect_Create ***
//=============================================================================
CEffect_Create::~CEffect_Create()
{

}

//=============================================================================
// ロード処理									(public)	*** CEffect_Create ***
//=============================================================================
HRESULT CEffect_Create::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::Unload(void)
{

}

//=============================================================================
// 初期化処理									(public)	*** CEffect_Create ***
//=============================================================================
HRESULT CEffect_Create::Init(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::Uninit(void)
{
	CEffect::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::Update(void)
{
	for (int nCount = 0; nCount < 10; nCount++)
	{
		CEffect_C00 *pEffect = NULL;
		pEffect = (CEffect_C00*)CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3((-1.0f + (float)(rand() % 3))* 0.08f, 0.3f, (-1.0f + (float)(rand() % 3))* 0.08f), m_size * 0.6f, D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));
		pEffect->SetDeleteTime(60);
		pEffect->SetSizeUp(true);
	}

	Uninit();
}

//=============================================================================
// 描画処理										(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::Draw(void)
{

}

//=============================================================================
// 判定チェック処理								(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::ColisionChack(void)
{

}