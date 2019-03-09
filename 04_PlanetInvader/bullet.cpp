//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "bullet.h"			// 弾
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "camera.h"			// カメラ
#include "explosion.h"		// 爆発
#include "player.h"			// プレイヤー
#include "enemy.h"			// 敵
#include "object3d.h"		// オブジェクト
#include "effect3d.h"		// エフェクト3D
#include "effect.h"			// エフェクト
#include "life.h"			// 体力
#include "score.h"			// スコア
#include "game.h"			// ゲーム

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_0	"data\\TEXTURE\\bullet000.png"		// テクスチャ1
#define TEXTURE_NAME_1	"data\\TEXTURE\\bomb.png"			// テクスチャ2

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CBullet::m_pTexture[BULLET_MAX_TEXTURE] = {};		// テクスチャ情報へのポインタ

//=============================================================================
// コンストラクタ									(public)	*** CBullet ***
//=============================================================================
CBullet::CBullet() : CScene3D(CScene::PRIORITY_3)
{
	// 初期値設定
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
}

//=============================================================================
// デストラクタ										(public)	*** CBullet ***
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CBullet::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture[0] == NULL)
	{// NULLの場合
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_0, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CBullet::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < BULLET_MAX_TEXTURE; nCntTex++)
	{// カウント
		if (m_pTexture[nCntTex] != NULL)
		{// NULL以外の場合
			m_pTexture[nCntTex]->Release();		// 解放
			m_pTexture[nCntTex] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 確保処理											(public)	*** CBullet ***
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, BULLETTYPE bulletType, BULLETSTATE bulletState)
{
	CBullet *pBullet = NULL;				// ポインタ

	if (pBullet == NULL)
	{// NULLの場合
		pBullet = new CBullet;				// メモリ確保
	}

	if (pBullet != NULL)
	{// NULL以外の場合
		pBullet->Init(pos, move, size, col, bulletType, bulletState);	// 初期化処理
		pBullet->SetObjType(CScene::OBJTYPE_BULLET);					// オブジェクトタイプ設定

		pBullet->BindTexture(m_pTexture[bulletState]);					// テクスチャ設定	
	}

	return pBullet;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CBullet ***
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, BULLETTYPE bulletType, BULLETSTATE bulletState)
{
	CScene3D::SetInitAll(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);			// 初期設定
	CScene3D::Init();						// 初期化処理

	m_bulletType = bulletType;				// 種類
	m_bulletState = bulletState;			// 状態
	m_move = move;							// 移動量代入
	m_Life = 1 * 60;						// 体力

	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();	// カメラ注視点取得

	m_pos = pos - Camerapos;				// カメラ移動無効化

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CBullet ***
//=============================================================================
void CBullet::Uninit(void)
{
	CScene3D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CBullet ***
//=============================================================================
void CBullet::Update(void)
{
	D3DXVECTOR3 CameraposV = CManager::GetCamera()->GetposV();	// カメラ視点取得
	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();	// カメラ注視点取得
	D3DXVECTOR3 pos = CScene3D::Getpos();	// 位置を取得
	int nBulletDamage;						// 弾ダメージ

	if (m_bulletState == BULLETSTATE_NORMAL)
	{
		nBulletDamage = 1;
	}
	else if (m_bulletState == BULLETSTATE_BOMB)
	{
		nBulletDamage = 3;
	}


	// カメラに対しての位置を設定
	pos.x = CameraposR.x + m_pos.x;
	pos.z = CameraposR.z + m_pos.z;			

	pos += m_move;							// 移動量代入

	m_Life--;								// ライフ減少

	CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_BULLET, CEffect::TEXTURE_EFFECT);		// エフェクト

	// 判定チェック------------------------------------------------------------
	CModel3D *pModel3D;						// シーン3D
	CScene::OBJTYPE objType;				// オブジェクト種類
	D3DXVECTOR3 targetpos, targetsize;		// 判定用 : 相手の位置、相手の大きさ											
	CPlayer *pPlayer;						// プレイヤー
	CEnemy *pEnemy;							// 敵
	CEnemy::ENEMYTYPE enemyType;			// 敵種類
	CObject3D *pObject3D;					// オブジェクト3D
	CObject3D::OBJECTTYPE objectType;		// オブジェクト種類
	bool bExplosion = false;				// 爆発
	bool bDelete = false;					// 破棄

	for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
	{// シーンカウント
		pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_1, nCntScene);	// シーン読み込み

		if (pModel3D != NULL)
		{// NULL以外の場合
			objType = pModel3D->GetObjType();			// オブジェクトタイプ取得

			if (objType == CScene::OBJTYPE_OBJ3D && (m_bulletType == BULLETTYPE_PLAYER_1 || m_bulletType == BULLETTYPE_PLAYER_2))
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
						if (((posOBJ.x - pos.x) * (posOBJ.x - pos.x)) + ((posOBJ.z - pos.z) * (posOBJ.z - pos.z)) < (sizeOBJ.x + BULLET_SIZE_X) * (sizeOBJ.z + BULLET_SIZE_X))
						{// 範囲チェック（円、画面外）
							bExplosion = true;					// 爆発する
							m_Life = 0;							// 体力０へ
							bDelete = true;						// 破棄する

							if (pObject3D->HitDamage(nBulletDamage, (CObject3D::BREAKTYPE)m_bulletState, atan2f(posOBJ.x - (pos.x), posOBJ.z - (pos.z)), m_bulletType) == true)
							{
								for (int nCnt = 0; nCnt < 15; nCnt++)
								{// カウント
									CEffect3D::Create(D3DXVECTOR3(targetpos.x, targetpos.y + (targetsize.y * 0.75f), targetpos.z),
										D3DXVECTOR3(-D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f)), -D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f)), -D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f))),
										D3DXVECTOR3(0.02f, -0.001f, 0.03f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f, 2.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f));
								}
							}

							for (int nCnt = 0; nCnt < 15; nCnt++)
							{// カウント
								CEffect::Create(D3DXVECTOR3(targetpos.x, targetpos.y + targetsize.y + EFFECT_SIZE_Y, targetpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f, -1.8f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f - 1.5f),
									D3DXVECTOR3(EFFECT_SIZE_X * 2.5f, EFFECT_SIZE_Y * 2.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);
							}

							// 位置調整
							//pos = posOBJ;

							CScore::AddScore(m_bulletType, 800);		// スコア加算

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

			if (objType == CScene::OBJTYPE_PLAYER && m_bulletType == BULLETTYPE_ENEMY ||
				objType == CScene::OBJTYPE_ENEMY && m_bulletType == BULLETTYPE_PLAYER_1 ||
				objType == CScene::OBJTYPE_ENEMY && m_bulletType == BULLETTYPE_PLAYER_2)
			{// 種類チェック
				// 相手の情報取得
				targetpos = pModel3D->Getpos();			// 位置	
				targetsize = pModel3D->Getsize(0);		// 大きさ

				if (objType == OBJTYPE_PLAYER)
				{	// プレイヤー
					if (((targetpos.x - pos.x) * (targetpos.x - pos.x)) + ((targetpos.z - pos.z) * (targetpos.z - pos.z)) < (targetsize.x + BULLET_SIZE_X) * (targetsize.x + BULLET_SIZE_X))
					{// 範囲チェック（円、画面外）
						bExplosion = true;					// 爆発する
						m_Life = 0;							// 体力０へ
						bDelete = true;						// 破棄する

						pPlayer = (CPlayer*)pModel3D;	// プレイヤー型に変換

						if (pPlayer->HitDamage(nBulletDamage) == true)
						{// HPが０以下になった
							bExplosion = true;			// 爆発する
							D3DXVECTOR3 posModel = pPlayer->Getpos();	// 位置取得
							int nPlayerNum = pPlayer->GetPlayerNum();	// プレイヤー番号

							pPlayer->Uninit();			// 終了処理

							for (int nCnt = 0; nCnt < 15; nCnt++)
							{// カウント
								CEffect::Create(posModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f, -5.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f),
									D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.1f, -0.1f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_EXPLOSION, CEffect::TEXTURE_EFFECT);
							}

							CLife::SetRespawn(nPlayerNum, true);	// 体力（残機）を減らす
							CLife::SetLiveState(nPlayerNum);		// 生存状況
						}

						break;
					}
				}
				else if (objType == OBJTYPE_ENEMY)
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
						if (((posOBJ.x - pos.x) * (posOBJ.x - pos.x)) + ((posOBJ.z - pos.z) * (posOBJ.z - pos.z)) < (sizeOBJ.x + BULLET_SIZE_X) * (sizeOBJ.z + BULLET_SIZE_X))
						{// 範囲チェック（円、画面外）
							bExplosion = true;					// 爆発する
							m_Life = 0;							// 体力０へ
							bDelete = true;						// 破棄する

							if (pEnemy->HitDamage(nBulletDamage) == true)
							{// HPが０以下になった
								bExplosion = true;			// 爆発する
								D3DXVECTOR3 posModel = pEnemy->Getpos();		// 位置取得

								pEnemy->Uninit();			// 終了処理							

								for (int nCnt = 0; nCnt < 15; nCnt++)
								{// カウント
									CEffect::Create(posModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f, -5.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f),
										D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.1f, -0.1f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_EXPLOSION, CEffect::TEXTURE_EFFECT);
								}

								CExplosion::Create(posOBJ, D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_bulletType);						// 爆発を生成

								if (m_bulletState == BULLETSTATE_NORMAL)
								{
									CScore::AddScore(m_bulletType, 1000);		// スコア加算
								}
								else
								{
									CScore::AddScore(m_bulletType, 1200);		// スコア加算
								}

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
												CExplosion::Create(pEnemyDelete->Getpos(), D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_bulletType);						// 爆発を生成

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
								}
							}

							// 位置調整
							//pos = posOBJ;

							break;
						}
					}
				}
			}
		}
	}

	if (pos.x < CameraposR.x - 260.0f || CameraposR.x + 260.0f < pos.x || pos.z < CameraposR.z - 400.0f || CameraposR.z + 180.0f < pos.z)
	{// 範囲チェック（円、画面外）
		bDelete = true;			// 破棄する
	}
	if (m_Life < 0 && m_bulletState == BULLETSTATE_BOMB)
	{// 範囲チェック（円、画面外）		
		bExplosion = true;		// 爆発する
		bDelete = true;			// 破棄する
	}

	if (bExplosion == true)
	{// 爆発する
		switch (m_bulletState)
		{// 弾種類
		case BULLETSTATE_NORMAL:	// 通常
			CExplosion::Create(pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_bulletType);						// 爆発を生成

			break;
		case BULLETSTATE_BOMB:		// ボム
			CExplosion::Create(pos, D3DXVECTOR3(EXPLOSION_SIZE_X * 4.0f, EXPLOSION_SIZE_Y * 4.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_DAMAGE, m_bulletType);		// 爆発を生成

			break;
		}
	}

	CScene3D::Setpos(pos);			// 位置を設定

	m_pos = pos - CameraposR;		// 現在位置を計算

	if (bDelete == true)
	{// 破棄する
		Uninit();					// 終了処理
	}
}

//=============================================================================
// 描画処理											(public)	*** CBullet ***
//=============================================================================
void CBullet::Draw(void)
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

	CScene3D::Draw();		// 描画処理

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//比較方法（より大きい））
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}