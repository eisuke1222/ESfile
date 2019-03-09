//=============================================================================
//
// オブジェクト処理 [object3d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "object3d.h"		// オブジェクト
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "sound.h"			// サウンド
#include "renderer.h"		// レンダラー
#include "camera.h"			// カメラ
#include "player.h"			// プレイヤー
#include "enemy.h"			// 敵
#include "effect.h"			// エフェクト
#include "life.h"			// ライフ
#include "explosion.h"		// 爆発
#include "score.h"			// スコア

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME_1		"data\\MODEL\\o_g000.x"		// モデル1
#define MODEL_NAME_2		"data\\MODEL\\bill000.x"	// モデル2
#define MODEL_NAME_3		"data\\MODEL\\bill001.x"	// モデル3
#define MODEL_NAME_4		"data\\MODEL\\bill002.x"	// モデル4
#define MODEL_NAME_5		"data\\MODEL\\bill003.x"	// モデル5

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CObject3D::m_pTexture[OBJECTTYPE_MAX] = {};		// テクスチャ情報へのポインタ
LPD3DXMESH				CObject3D::m_pMesh[OBJECTTYPE_MAX] = {};			// メッシュ情報へのポインタ
LPD3DXBUFFER			CObject3D::m_pBuffMat[OBJECTTYPE_MAX] = {};			// マテリアル情報へのポインタ
DWORD					CObject3D::m_nNumMat[OBJECTTYPE_MAX] = {};			// マテリアル情報の数

//=============================================================================
// コンストラクタ									(public)	*** CObject3D ***
//=============================================================================
CObject3D::CObject3D() : CModel3D(CScene::PRIORITY_1)
{
	m_objectType = OBJECTTYPE_BILL_1;	// モデルタイプ
	m_breakType = BREAKTYPE_NORMAL;		// 破壊タイプ
	m_bDelete = false;					// 破棄
	m_nLife = 0;						// 体力
	m_nCntTimer = 0;					// 時間カウンタ
	m_breakrot = 0.0f;					// 破壊向き
	m_breakrotDiff = 0.0f;				// 破壊向き残り
	m_nShotIdx = 0;						// 発射ID
	m_nCntBreak = 0;
}

//=============================================================================
// デストラクタ										(public)	*** CObject3D ***
//=============================================================================
CObject3D::~CObject3D()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CObject3D::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	char cName[255] = {};		// 名前格納

	for (int nCntModel = 0; nCntModel < OBJECTTYPE_MAX; nCntModel++)
	{// カウント
		if (m_pBuffMat[nCntModel] == NULL && m_pMesh[nCntModel] == NULL)
		{// NULLの場合		
			switch (nCntModel)
			{
			case OBJECTTYPE_O_G:
				wsprintf(cName, MODEL_NAME_1);

				break;
			case OBJECTTYPE_BILL_1:
				wsprintf(cName, MODEL_NAME_2);

				break;
			case OBJECTTYPE_BILL_2:
				wsprintf(cName, MODEL_NAME_3);

				break;
			case OBJECTTYPE_BILL_3:
				wsprintf(cName, MODEL_NAME_4);

				break;
			case OBJECTTYPE_LIFE:
				wsprintf(cName, MODEL_NAME_5);

				break;
			}

			// Xファイルの読み込み
			D3DXLoadMeshFromX(cName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCntModel], NULL, &m_nNumMat[nCntModel], &m_pMesh[nCntModel]);

			D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
			D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
			pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
			matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
			m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

			for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
			{// カウント
				matDef[tex] = pmat[tex].MatD3D;
				matDef[tex].Ambient = matDef[tex].Diffuse;
				m_pTexture[nCntModel][tex] = NULL;
				if (pmat[tex].pTextureFilename != NULL &&
					lstrlen(pmat[tex].pTextureFilename) > 0)
				{// テクスチャを使用している
					if (FAILED(D3DXCreateTextureFromFile(pDevice,
						pmat[tex].pTextureFilename,
						&m_pTexture[nCntModel][tex])))
					{
						MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
					}
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CObject3D::Unload(void)
{

	for (int nCntModel = 0; nCntModel < OBJECTTYPE_MAX; nCntModel++)
	{// カウント
		// テクスチャの破棄
		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{// カウント
			if (m_pTexture[nCntModel][tex] != NULL)
			{// NULL以外の場合
				m_pTexture[nCntModel][tex]->Release();		// 解放
				m_pTexture[nCntModel][tex] = NULL;			// NULLへ
			}
		}

		// マテリアルの開放
		if (m_pBuffMat[nCntModel] != NULL)
		{// NULL以外の場合
			m_pBuffMat[nCntModel]->Release();				// 解放
			m_pBuffMat[nCntModel] = NULL;					// NULLへ
		}

		// マテリアルの開放
		if (m_pMesh[nCntModel] != NULL)
		{// NULL以外の場合
			m_pMesh[nCntModel]->Release();					// 解放
			m_pMesh[nCntModel] = NULL;						// NULLへ
		}

		m_nNumMat[nCntModel] = 0;				// 初期化
	}
}

//=============================================================================
// 確保処理											(public)	*** CObject3D ***
//=============================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE objectType)
{
	CObject3D *pObject3D = NULL;			// ポインタ

	if (pObject3D == NULL)
	{// NULLの場合
		pObject3D = new CObject3D;			// メモリ確保
	}

	if (pObject3D != NULL)
	{// NULL以外の場合
		pObject3D->Init(pos, rot, objectType);				// 初期化処理
		pObject3D->SetObjType(CScene::OBJTYPE_OBJ3D);		// オブジェクトタイプ設定
	}

	return pObject3D;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CObject3D ***
//=============================================================================
HRESULT CObject3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE objectType)
{
	CModel3D::SetInitAll(m_pTexture[objectType], m_pMesh[objectType], m_pBuffMat[objectType], m_nNumMat[objectType], pos, rot);		// 初期値設定
	CModel3D::Init();																// 初期化処理

	m_objectType = objectType;				// オブジェクトタイプ
	m_bDelete = false;						// 破棄

	switch (m_objectType)
	{
	case OBJECTTYPE_O_G:
	case OBJECTTYPE_LIFE:
		m_nLife = 0;

		break;
	case OBJECTTYPE_BILL_1:
		m_nLife = 5;

		break;
	case OBJECTTYPE_BILL_2:
		m_nLife = 3;

		break;
	case OBJECTTYPE_BILL_3:
		m_nLife = 7;

		break;
	}

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CObject3D ***
//=============================================================================
void CObject3D::Uninit(void)
{
	CModel3D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CObject3D ***
//=============================================================================
void CObject3D::Update(void)
{
	CSound *pSound = CManager::GetSound();							// サウンド
	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();	// カメラ視点取得
	D3DXVECTOR3 CameraposV = CManager::GetCamera()->GetposV();	// カメラ視点取得
	D3DXVECTOR3 pos = CObject3D::Getpos();						// 位置取得
	D3DXVECTOR3 sizeMax = CObject3D::Getsize(0);				// 大きさ取得
	D3DXVECTOR3 sizeMin = CObject3D::Getsize(1);				// 大きさ取得
	bool bDelete = false;

	if (m_objectType != OBJECTTYPE_O_G)
	{
		if (m_objectType == OBJECTTYPE_LIFE)
		{
			pos.z -= 3.0f;

			// 判定チェック------------------------------------------------------------
			CModel3D *pModel3D;						// シーン3D
			CScene::OBJTYPE objType;				// オブジェクト種類
			D3DXVECTOR3 targetpos, targetsize;		// 判定用 : 相手の位置、相手の大きさ											
			CPlayer *pPlayer;						// プレイヤー

			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{// シーンカウント
				pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_3, nCntScene);	// シーン読み込み

				if (pModel3D != NULL)
				{// NULL以外の場合
					objType = pModel3D->GetObjType();			// オブジェクトタイプ取得

					if (objType == OBJTYPE_PLAYER)
					{	// プレイヤー
						// 相手の情報取得
						targetpos = pModel3D->Getpos();			// 位置	
						targetsize = pModel3D->Getsize(0);		// 大きさ

						D3DXVECTOR3 posOBJ = D3DXVECTOR3(
							CameraposR.x + (((pos.x) - (CameraposR.x)) * 0.6f),
							300.0f,
							CameraposR.z - 350.0f + (((pos.z) - (CameraposR.z - 300.0f)) * 0.55f));
						D3DXVECTOR3 sizeOBJ = sizeMax;
						sizeOBJ *= *D3DXVECTOR3(0.5f, 0.5f, 0.5f);

						if (((targetpos.x - posOBJ.x) * (targetpos.x - posOBJ.x)) + ((targetpos.z - posOBJ.z) * (targetpos.z - posOBJ.z)) < (targetsize.x + sizeOBJ.x) * (targetsize.x + sizeOBJ.x))
						{// 範囲チェック（円、画面外）
							Uninit();

							/*for (int nCnt = 0; nCnt < 15; nCnt++)
							{// カウント
								CEffect::Create(posOBJ, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f, 5.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f),
									D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.2f, -0.2f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_EXPLOSION, CEffect::TEXTURE_EFFECT);
							}*/

							pPlayer = (CPlayer*)pModel3D;

							CLife::SetAdd(pPlayer->GetPlayerNum(), true);	// 体力（残機）を減らす

							pSound->PlaySoundA(CSound::SOUND_LABEL_SE009);

							break;
						}
					}
				}
			}
		}
		else
		{// 境界以外
			if (m_bDelete == true)
			{// 破壊する
				m_nCntTimer++;

				if (m_objectType != OBJECTTYPE_BILL_3)
				{
					if (m_breakType == BREAKTYPE_NORMAL)
					{
						if (30.0f < pos.y + sizeMax.y)
						{// 一定位置まで
							pos.y -= 2.0f;			// 下げる
						}

						if (m_nCntTimer % 3 == 0)
						{
							CEffect::Create(D3DXVECTOR3(pos.x + sizeMin.x + (rand() % (int)(-sizeMin.x + sizeMax.x) + 1), pos.y + sizeMax.y + EFFECT_SIZE_Y, pos.z + sizeMin.z + (rand() % (int)(-sizeMin.z + sizeMax.z) + 1)),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * (rand() % 15 * 1.0f)) * 0.01f, 0.01f, cosf(((D3DX_PI * 2.0f) / 15.0f) * (rand() % 15 * 1.0f)) * 0.01f),
								D3DXVECTOR3(0.1f, 0.1f, 0.0f), D3DXVECTOR3(0.8f, 0.8f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.015f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);		// エフェクト
						}
					}
					else if (m_breakType == BREAKTYPE_FALL)
					{
						if (0.0f < m_breakrotDiff)
						{
							D3DXVECTOR3 rot = CObject3D::Getrot();
							float frot_x, frot_z;
							frot_x = 0.01f + (0.01f * (m_nCntTimer / 10));
							frot_z = 0.01f + (0.01f * (m_nCntTimer / 10));

							m_breakrotDiff -= frot_x;
							m_breakrotDiff -= frot_z;

							CObject3D::Setrot(D3DXVECTOR3(CObject3D::Getrot().x + frot_x, CObject3D::Getrot().y, CObject3D::Getrot().z + sinf(-m_breakrot) * frot_z));		// 位置更新

							if (m_breakrotDiff <= D3DX_PI * 0.5f && m_nCntBreak < 10)
							{
								CEffect::Create(D3DXVECTOR3(pos.x + sinf(m_breakrot) * ((sizeMax.y / 10) * m_nCntBreak), pos.y + sizeMax.x + EFFECT_SIZE_Y, pos.z + cosf(-m_breakrot) * ((sizeMax.y / 10) * m_nCntBreak)),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.8f, -2.8f),
									D3DXVECTOR3(0.1f, 0.1f, 0.0f), D3DXVECTOR3(1.3f, 1.3f, 0.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), 0.008f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);		// エフェクト

								m_nCntBreak++;
							}

							if (m_breakrotDiff <= 0.0f)
							{
								for (int nCnt = 0; nCnt < 15; nCnt++)
								{
									CEffect::Create(D3DXVECTOR3(pos.x + sinf(m_breakrot) * ((sizeMax.y / 15) * nCnt), pos.y + sizeMax.x + EFFECT_SIZE_Y, pos.z + cosf(-m_breakrot) * ((sizeMax.y / 15) * nCnt)),
										D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(m_breakrot + (((D3DX_PI * 2.0f) / 15) * nCnt)) * 2.2f, 0.0f, cosf(m_breakrot + (((D3DX_PI * 2.0f) / 15) * nCnt)) * -2.2f + -2.0f),
										D3DXVECTOR3(0.1f, 0.1f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);		// エフェクト
								}

								// 判定チェック------------------------------------------------------------
								CModel3D *pModel3D;						// シーン3D
								CScene::OBJTYPE objType;				// オブジェクト種類
								D3DXVECTOR3 targetpos, targetsize;		// 判定用 : 相手の位置、相手の大きさ											
								bool bExplosion = false;				// 爆発
								bool bDelete = false;					// 破棄

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

											CEnemy *pEnemy = (CEnemy*)pModel3D;						// 敵
											CEnemy::ENEMYTYPE enemyType = pEnemy->GetenemyType();	// 敵種類

											if (enemyType == CEnemy::ENEMYTYPE_G_1)
											{
												for (int nCnt = 0; nCnt < 10; nCnt++)
												{
													D3DXVECTOR3 posObj = D3DXVECTOR3(
														pos.x + sinf(m_breakrot) * ((sizeMax.y / 10) * nCnt),
														pos.y,
														pos.z + cosf(-m_breakrot) * ((sizeMax.y / 10) * nCnt));

													if ((((targetpos.x - posObj.x) * (targetpos.x - posObj.x)) + ((targetpos.z - posObj.z) * (targetpos.z - posObj.z))) < (((targetsize.x - (sizeMax.x * 5.0f)) * (targetsize.x - (sizeMax.x * 5.0f)))))
													{
														CExplosion::Create(D3DXVECTOR3(targetpos.x, targetpos.y + targetsize.y, targetpos.z), D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_nShotIdx);						// 爆発を生成

														pEnemy->Uninit();

														CScore::AddScore(m_nShotIdx, 1500);		// スコア加算

														break;
													}
												}
											}
										}
									}
								}
							}
						}
						else
						{
							// 判定チェック------------------------------------------------------------
							CModel3D *pModel3D;						// シーン3D
							CScene::OBJTYPE objType;				// オブジェクト種類
							D3DXVECTOR3 targetpos, targetsize;		// 判定用 : 相手の位置、相手の大きさ											
							bool bExplosion = false;				// 爆発
							bool bDelete = false;					// 破棄

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

										CEnemy *pEnemy = (CEnemy*)pModel3D;						// 敵
										CEnemy::ENEMYTYPE enemyType = pEnemy->GetenemyType();	// 敵種類

										if (enemyType == CEnemy::ENEMYTYPE_G_1)
										{
											for (int nCnt = 0; nCnt < 10; nCnt++)
											{
												D3DXVECTOR3 posObj = D3DXVECTOR3(
													pos.x + sinf(m_breakrot) * ((sizeMax.y / 10) * nCnt),
													pos.y,
													pos.z + cosf(-m_breakrot) * ((sizeMax.y / 10) * nCnt));

												if ((((targetpos.x - posObj.x) * (targetpos.x - posObj.x)) + ((targetpos.z - posObj.z) * (targetpos.z - posObj.z))) < (((targetsize.x - (sizeMax.x * 5.0f)) * (targetsize.x - (sizeMax.x * 5.0f)))))
												{
													CExplosion::Create(D3DXVECTOR3(targetpos.x, targetpos.y + targetsize.y, targetpos.z), D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_nShotIdx);						// 爆発を生成

													pEnemy->Uninit();

													CScore::AddScore(m_nShotIdx, 1500);		// スコア加算

													break;
												}
											}
										}
									}
								}
							}
						}
					}
				}
				else
				{
					m_nCntBreak++;

					if (m_nCntBreak < 45)
					{
						if (m_nCntBreak % 3 == 0)
						{
							CExplosion::Create(D3DXVECTOR3(pos.x - sizeMax.x + ((sizeMax.x * 2.0f / 5.0f) * (rand() % 5)), pos.y + sizeMax.y + EFFECT_SIZE_Y, pos.z - sizeMax.z + ((sizeMax.z * 2.0f / 5.0f) * (rand() % 5))),
								D3DXVECTOR3(EXPLOSION_SIZE_X * 4.0f, EXPLOSION_SIZE_Y * 4.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_DAMAGE, m_nShotIdx);		// 爆発を生成
						}
					}
					else
					{
						if (m_nCntBreak == 345)
						{
							for (int nCnt = 0; nCnt < 15; nCnt++)
							{
								CEffect::Create(D3DXVECTOR3(pos.x - sizeMax.x + ((sizeMax.x * 2.0f / 5.0f) * (rand() % 5)), pos.y + sizeMax.y + EFFECT_SIZE_Y, pos.z - sizeMax.z + ((sizeMax.z * 2.0f / 5.0f) * (rand() % 5))),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf((((D3DX_PI * 2.0f) / 15) * nCnt)) * 2.2f, 0.0f, cosf((((D3DX_PI * 2.0f) / 15) * nCnt)) * -2.2f + -2.0f),
									D3DXVECTOR3(0.1f, 0.1f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);		// エフェクト
							}
						}
						bDelete = true;
					}
				}

				CObject3D::Setpos(pos);		// 位置更新
			}
		}
	}

	if (CameraposV.z - pos.z >= 0 || bDelete == true)
	{// カメラ範囲外に出たら
		Uninit();					// 終了処理
	}
}

//=============================================================================
// 描画処理											(public)	*** CObject3D ***
//=============================================================================
void CObject3D::Draw(void)
{
	CModel3D::Draw();						// 描画処理
}