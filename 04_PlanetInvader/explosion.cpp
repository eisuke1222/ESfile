//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "explosion.h"		// 爆発
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "sound.h"			// サウンド
#include "camera.h"			// カメラ
#include "game.h"			// ゲーム
#include "player.h"			// プレイヤー
#include "enemy.h"			// 敵
#include "object3d.h"		// オブジェクト
#include "effect3d.h"		// エフェクト3D
#include "effect.h"			// エフェクト
#include "life.h"			// 体力
#include "score.h"			// スコア

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\explosion000.png"		// テクスチャ
#define TEX_UV_U			(0.125f)								// テクスチャUV_U	
#define TEX_UV_V			(0.5f)									// テクスチャUV_V
#define ANIM_SPEED			(3)										// アニメーションスピード
#define ANIM_PATTERN		(16)									// アニメーションパターン
#define ANIM_LINENUM		(8)										// アニメーション折り返し

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			CExplosion::m_pTexture = NULL;			// テクスチャ情報へのポインタ

//=============================================================================
// コンストラクタ									(public)	*** CExplosion ***
//=============================================================================
CExplosion::CExplosion() : CScene3D(CScene::PRIORITY_4)
{
	m_nAnimCounter	= 0;			// アニメーションカウンタ
	m_AnimPattern	= 0;			// アニメーションパターン
	m_explosionType = EXPLOSIONTYPE_NORMAL;		// 爆発タイプ
	m_nShotIdx = 0;
}

//=============================================================================
// デストラクタ										(public)	*** CExplosion ***
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
// ロード処理							(public)	*** CExplosion ***
//=============================================================================
HRESULT CExplosion::Load(void)
{
	if (m_pTexture == NULL)
	{// NULLの場合
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CExplosion ***
//=============================================================================
void CExplosion::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{// NULL以外の場合
		m_pTexture->Release();		// 解放
		m_pTexture = NULL;			// NULLへ
	}
}

//=============================================================================
// 確保処理											(public)	*** CExplosion ***
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, EXPLOSIONTYPE explosionType, int nShotIdx)
{
	CExplosion *pExplosion = NULL;				// ポインタ

	if (pExplosion == NULL)
	{// NULLの場合
		pExplosion = new CExplosion;			// メモリ確保
	}

	if (pExplosion != NULL)
	{// NULL以外の場合
		pExplosion->Init(pos, size, col, explosionType, nShotIdx);		// 初期化処理
		pExplosion->SetObjType(CScene::OBJTYPE_EXPLOSION);				// オブジェクトタイプ設定
		pExplosion->BindTexture(m_pTexture);							// テクスチャ設定
	}

	return pExplosion;		// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CExplosion ***
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, EXPLOSIONTYPE explosionType, int nShotIdx)
{
	CScene3D::SetInitAll(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, col, D3DXVECTOR2(TEX_UV_U, TEX_UV_V), CScene3D::SCENE3DTYPE_BILLBOARD);		// 初期値設定
	CScene3D::Init();																															// 初期化処理

	m_explosionType = explosionType;
	m_nShotIdx = nShotIdx;

	CSound *pSound = CManager::GetSound();							// サウンド

	pSound->PlaySoundA(CSound::SOUND_LABEL_SE005);

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CExplosion ***
//=============================================================================
void CExplosion::Uninit(void)
{
	CScene3D::Uninit();			// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CExplosion ***
//=============================================================================
void CExplosion::Update(void)
{
	m_nAnimCounter++;	// アニメーションカウンタ

	if ((m_nAnimCounter % ANIM_SPEED) == 0)
	{// あまりのない時
		m_AnimPattern = (m_AnimPattern + 1) % ANIM_PATTERN;		// パターンを進める
	}

	CScene3D::SetTex(ANIM_LINENUM, m_AnimPattern, D3DXVECTOR2(TEX_UV_U, TEX_UV_V), D3DXVECTOR2(0.0f, 0.0f));		// パターン更新

	if (m_explosionType == EXPLOSIONTYPE_DAMAGE)
	{
		D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();	// カメラ注視点取得
		D3DXVECTOR3 pos = CScene3D::Getpos();	// 位置を取得

		// 判定チェック------------------------------------------------------------
		CModel3D *pModel3D;						// シーン3D
		CScene::OBJTYPE objType;				// オブジェクト種類
		D3DXVECTOR3 targetpos, targetsize;		// 判定用 : 相手の位置、相手の大きさ											
		CEnemy *pEnemy;							// 敵
		CEnemy::ENEMYTYPE enemyType;			// 敵種類
		CObject3D *pObject3D;					// オブジェクト3D
		CObject3D::OBJECTTYPE objectType;		// オブジェクト種類

		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// シーンカウント
			pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_1, nCntScene);	// シーン読み込み

			if (pModel3D != NULL)
			{// NULL以外の場合
				objType = pModel3D->GetObjType();			// オブジェクトタイプ取得

				if (objType == CScene::OBJTYPE_OBJ3D)
				{// 種類チェック
				 // 相手の情報取得
					pObject3D = (CObject3D*)pModel3D;					// オブジェクト型
					objectType = pObject3D->GetobjectType();			// オブジェクトタイプ取得

					if (objectType != CObject3D::OBJECTTYPE_O_G && pObject3D->GetDelete() == false)
					{
						targetpos = pObject3D->Getpos();		// 位置	
						targetsize = pObject3D->Getsize(0);		// 大きさ

						D3DXVECTOR3 posOBJ;
						D3DXVECTOR3 sizeOBJ;

						if (objectType == CObject3D::OBJECTTYPE_BILL_1)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.8f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.8f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.8f, 0.8f, 0.8f);
						}
						else if (objectType == CObject3D::OBJECTTYPE_BILL_2)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.6f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.6f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.6f, 0.6f, 0.6f);
						}
						else if (objectType == CObject3D::OBJECTTYPE_BILL_3)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.6f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.7f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.8f, 0.8f, 0.8f);
						}

						if (objectType != CObject3D::OBJECTTYPE_LIFE)
						{

							//if (((targetpos.x - posOBJ.x) * (targetpos.x - posOBJ.x)) + ((targetpos.z - posOBJ.z) * (targetpos.z - posOBJ.z)) < (targetsize.x + BULLET_SIZE_X) * (targetsize.x + BULLET_SIZE_X))
							if (((posOBJ.x - pos.x) * (posOBJ.x - pos.x)) + ((posOBJ.z - pos.z) * (posOBJ.z - pos.z)) < (sizeOBJ.x + (EXPLOSION_SIZE_X * 4.0f)) * (sizeOBJ.z + (EXPLOSION_SIZE_X * 4.0f)))
							{// 範囲チェック（円、画面外）
								pObject3D->HitDamage(3, CObject3D::BREAKTYPE_FALL, atan2f(posOBJ.x - (pos.x), posOBJ.z - (pos.z)), m_nShotIdx);			// ダメージ

								for (int nCnt = 0; nCnt < 15; nCnt++)
								{// カウント
									CEffect3D::Create(D3DXVECTOR3(targetpos.x, targetpos.y + (targetsize.y * 0.75f), targetpos.z),
										D3DXVECTOR3(-D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f)), -D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f)), -D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f))),
										D3DXVECTOR3(0.02f, -0.001f, 0.03f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f, 2.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f));
								}
								for (int nCnt = 0; nCnt < 15; nCnt++)
								{// カウント
									CEffect::Create(D3DXVECTOR3(targetpos.x, targetpos.y + targetsize.y, targetpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f, -1.8f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f - 1.5f),
										D3DXVECTOR3(EFFECT_SIZE_X * 2.5f, EFFECT_SIZE_Y * 2.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);
								}

								break;
							}
						}
					}
				}
			}
		}

		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// シーンカウント
			pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_3, nCntScene);	// シーン読み込み

			if (pModel3D != NULL)
			{// NULL以外の場合
				objType = pModel3D->GetObjType();			// オブジェクトタイプ取得

				if (objType == CScene::OBJTYPE_ENEMY)
				{// 種類チェック
				 // 相手の情報取得
					targetpos = pModel3D->Getpos();			// 位置	
					targetsize = pModel3D->Getsize(0);		// 大きさ

					if (objType == OBJTYPE_ENEMY)
					{	// 敵
						pEnemy = (CEnemy*)pModel3D;		// 敵型に変換

						enemyType = pEnemy->GetenemyType();

						D3DXVECTOR3 posOBJ;
						D3DXVECTOR3 sizeOBJ;

						if (enemyType == CEnemy::ENEMYTYPE_S_1 || enemyType == CEnemy::ENEMYTYPE_S_2)
						{
							posOBJ = targetpos;
							sizeOBJ = targetsize;
						}
						else if (enemyType == CEnemy::ENEMYTYPE_G_1 || enemyType == CEnemy::ENEMYTYPE_G_2)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.6f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.55f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.5f, 0.5f, 0.5f);
						}
						else if (enemyType == CEnemy::ENEMYTYPE_O_1)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.55f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.55f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.5f, 0.5f, 0.5f);
						}
						else if (enemyType == CEnemy::ENEMYTYPE_B_1)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.55f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.55f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.3f, 0.5f, 0.5f);
						}
						else if (enemyType == CEnemy::ENEMYTYPE_B_2)
						{
							posOBJ = targetpos;
							sizeOBJ = targetsize;
						}
						/*else if (enemyType == CEnemy::ENEMYTYPE_B_3)
						{
						posOBJ = D3DXVECTOR3(
						CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.6f),
						300.0f,
						CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.55f));
						sizeOBJ = targetsize;
						sizeOBJ *= *D3DXVECTOR3(0.5f, 0.5f, 0.5f);
						}*/

						if (enemyType != CEnemy::ENEMYTYPE_B_3)
						{
							if (((posOBJ.x - pos.x) * (posOBJ.x - pos.x)) + ((posOBJ.z - pos.z) * (posOBJ.z - pos.z)) < (sizeOBJ.x + (EXPLOSION_SIZE_X * 4.0f)) * (sizeOBJ.z + (EXPLOSION_SIZE_X * 4.0f)))
							{// 範囲チェック（円、画面外）
								if (pEnemy->HitDamage(4) == true)
								{// HPが０以下になった
									D3DXVECTOR3 posModel = pEnemy->Getpos();		// 位置取得

									CExplosion::Create(posOBJ, D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_nShotIdx);						// 爆発を生成

									pEnemy->Uninit();			// 終了処理							

									for (int nCnt = 0; nCnt < 15; nCnt++)
									{// カウント
										CEffect::Create(posModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f, -5.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f),
											D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.1f, -0.1f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_EXPLOSION, CEffect::TEXTURE_EFFECT);
									}

									CScore::AddScore(m_nShotIdx, 1300);		// スコア加算

									if (enemyType == CEnemy::ENEMYTYPE_B_1)
									{
										for (int nCnt = 0; nCnt < MAX_SCENE; nCnt++)
										{// シーンカウント
											CModel3D *pEnemyDelete = (CModel3D*)CScene::GetScene(CScene::PRIORITY_3, nCnt);	// シーン読み込み

											if (pEnemyDelete != NULL)
											{// NULL以外の場合
												CScene::OBJTYPE enemyObj = pEnemyDelete->GetObjType();			// オブジェクトタイプ取得

												if (enemyObj == CScene::OBJTYPE_ENEMY)
												{
													CExplosion::Create(pEnemyDelete->Getpos(), D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_nShotIdx);						// 爆発を生成

													pEnemyDelete->Uninit();
												}
											}
										}

										CManager::MODE mode = CManager::GetMode();
										CGame::GAME game = CGame::GetGame();

										if (mode == CManager::MODE_GAME)
										{
											if (game == CGame::GAME_PLAY)
											{
												CGame::SetGame(CGame::GAME_CLEAR);
											}
										}

										CScore::AddScore(m_nShotIdx, 2000);		// スコア加算
									}
								}

								// 位置調整
								pos = posOBJ;

								break;
							}
						}
					}
				}
			}
		}
	}

	CScene3D::Update();			// 更新処理

	if (m_AnimPattern == ANIM_PATTERN - 1)
	{// パターンが最後まで到達したら
		Uninit();				// 終了処理
	}
}

//=============================================================================
// 描画処理											(public)	*** CExplosion ***
//=============================================================================
void CExplosion::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//比較方法（より大きい））
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	CScene3D::Draw();			// 描画処理

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//比較方法（より大きい））
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}