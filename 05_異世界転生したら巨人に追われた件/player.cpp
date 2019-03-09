//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "player.h"			// プレイヤー
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "game.h"			// ゲーム
#include "input.h"			// 入力
#include "model.h"			// モデル
#include "motion.h"			// モーション
#include "camera.h"			// カメラ
#include "meshField.h"		// メッシュフィールド
#include "meshriver.h"		// メッシュリバー
#include "block.h"			// ブロック
#include "item.h"			// アイテム
#include "life.h"			// 体力
#include "effect.h"			// エフェクト
#include "enemy.h"			// 敵
#include "wall.h"			// 壁
#include "start.h"			// スタート
#include "fade.h"			//フェイド
#include "shadow.h"			// 影
#include "time.h"			// タイム

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ANIM_NAME			"data\\TEXT\\playermodeldata.txt"	// モーションデータ
#define GRAVITY				(0.7f)								// 重力
#define MOVE_DOWNSPEED_NOR	(0.25f)								// 通常減速スピード
#define MOVE_DOWNSPEED_ICE	(0.13f)								// 滑る減速スピード
#define MOVE_UPSPEED		(1.0f)								// アイテムによって早くなるスピードの値
#define MOVE_UPCOUNTER		(90)								// アイテムによって早くなる時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CPlayer::m_MaxModel = 0;			// モデル数
int						CPlayer::m_MaxAnim = 0;				// アニメーション数

bool					CPlayer::m_bFalling = false;		// 落ちたか

LPDIRECT3DTEXTURE9		**CPlayer::m_pTexture = NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CPlayer::m_pMesh = NULL;			// メッシュ情報へのポインタ
LPD3DXBUFFER			*CPlayer::m_pBuffMat = NULL;		// マテリアル情報へのポインタ
DWORD					*CPlayer::m_nNumMat = NULL;			// マテリアル情報の数

CPlayer::MODEL_INFO		*CPlayer::m_ModelInfo = NULL;		// モデルインフォ
CMotion::ANIM_INFO		*CPlayer::m_AnimInfo = NULL;		// アニムインフォ

//=============================================================================
// コンストラクタ									(public)	*** CPlayer ***
//=============================================================================
CPlayer::CPlayer() : CScene(CScene::PRIORITY_3, CScene::OBJTYPE_PLAYER)
{
	m_bStart = true;										// スタート
	m_nCntStart = 0;										// スタートカウント
	m_bEnd = false;											// エンド	
	m_bClear = false;										// クリア
	m_AnimEditCounter = 0;									// アニメーション編集カウンタ
	m_state = STATE_NONE;									// 状態
	m_nCntEffect = 0;										// エフェクトカウンタ
	m_SoundCounter = 0;										// サウンドカウンタ
	m_nCntState = 0;										// 状態カウンタ
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 判定MAX
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 判定MIN
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置前回
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 移動量
	m_frotDest = 0.0f;										// 目標向き
	m_bJump = false;										// ジャンプ
	m_bMove = false;										// 移動できるか
	m_bSlip = false;										// 滑っているか
	m_fSliprot = 0.0f;										// 滑っている向き
	m_fDownSpeed = MOVE_DOWNSPEED_NOR;						// 減速
	m_nWallJump = 5;										// 壁キックカウント
	m_bWallJump = false;									// 壁キックしてるか
	m_bWallJumpmove = 0.0f;									// 壁キック移動量
	m_bmoveUp = false;										// スピードアップ
	m_nMoveCounter = 0;										// 移動カウンタ
	m_bFalling = false;										// 落下ED遷移用

	m_shadowHeight = 0.0f;									// 影高さ
}

//=============================================================================
// デストラクタ										(public)	*** CPlayer ***
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CPlayer::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	FILE *pFile = NULL;					// ファイルポインタ

	pFile = fopen(ANIM_NAME, "r");		// ファイルオープン

	if (pFile != NULL)
	{// NULL以外の場合
		int nCntModel = 0;	// モデルカウント
		int nCntMotion = 0;	// モーションカウント
		int nCntKey = 0;	// キーカウント

		char *pStrCut;		// 文字列の先頭へのポインタ
		char aLine[256];	// 文字列読み取り用（１行分）
		char aStr[256];		// 文字列抜き出し用

		pStrCut = ReadFile(pFile, &aLine[0]);		// コメントチェック
		strcpy(aStr, pStrCut);						// 文字変換

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{
			do
			{// ループ
				pStrCut = ReadFile(pFile, &aLine[0]);			// コメントチェック
				strcpy(aStr, pStrCut);							// 文字変換

				if (memcmp(aStr, "NUM_MODEL", strlen("NUM_MODEL")) == 0)
				{// モデル数
					pStrCut += strlen("NUM_MODEL");				// ポインタを進める
					PopString(pStrCut, aStr);					// 文字変換
					m_MaxModel = atoi(aStr);					// 整数型に変換

					if (m_ModelInfo == NULL)
					{// NULLの場合
						m_ModelInfo = new MODEL_INFO[m_MaxModel];	// メモリ確保
					}
				}
				else if (memcmp(aStr, "MODEL_FILENAME", strlen("MODEL_FILENAME")) == 0)
				{// モデルファイル名
					pStrCut += strlen("MODEL_FILENAME");						// ポインタを進める
					PopString(pStrCut, aStr);									// 文字変換
					strcpy(m_ModelInfo[nCntModel].cFileName, aStr);				// ファイル名を保存

					nCntModel++;		// モデルカウント
				}
				else if (memcmp(aStr, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
				{// キャラクター設定
					nCntModel = 0;		// モデルカウンタを初期化

					do
					{// ループ
						pStrCut = ReadFile(pFile, &aLine[0]);						// コメントチェック
						strcpy(aStr, pStrCut);										// 文字変換

						if (memcmp(aStr, "PARTSSET", strlen("PARTSSET")) == 0)
						{
							do
							{
								pStrCut = ReadFile(pFile, &aLine[0]);					// コメントチェック
								strcpy(aStr, pStrCut);									// 文字変換

								if (memcmp(aStr, "INDEX", strlen("INDEX")) == 0)
								{// モデル番号
									pStrCut += strlen("INDEX");								// ポインタを進める
									PopString(pStrCut, aStr);								// 文字変換
									m_ModelInfo[nCntModel].nIndex = atoi(aStr);				// 整数型に変換
								}
								else if (memcmp(aStr, "PARENT", strlen("PARENT")) == 0)
								{// 親モデル番号
									pStrCut += strlen("PARENT");							// ポインタを進める
									PopString(pStrCut, aStr);								// 文字変換
									m_ModelInfo[nCntModel].nIndex = atoi(aStr);				// 整数型に変換
								}
								else if (memcmp(aStr, "POS", strlen("POS")) == 0)
								{// 位置
									pStrCut += strlen("POS");								// ポインタを進める
									PopString(pStrCut, aStr);								// 文字変換
									m_ModelInfo[nCntModel].pos.x = (float)atof(aStr);		// 整数型に変換
									PopString(pStrCut, aStr);								// 文字変換
									m_ModelInfo[nCntModel].pos.y = (float)atof(aStr);		// 整数型に変換
									PopString(pStrCut, aStr);								// 文字変換
									m_ModelInfo[nCntModel].pos.z = (float)atof(aStr);		// 整数型に変換
								}
								else if (memcmp(aStr, "ROT", strlen("ROT")) == 0)
								{// 向き
									pStrCut += strlen("ROT");								// ポインタを進める
									PopString(pStrCut, aStr);								// 文字変換
									m_ModelInfo[nCntModel].rot.x = (float)atof(aStr);		// 整数型に変換
									PopString(pStrCut, aStr);								// 文字変換
									m_ModelInfo[nCntModel].rot.y = (float)atof(aStr);		// 整数型に変換
									PopString(pStrCut, aStr);								// 文字変換
									m_ModelInfo[nCntModel].rot.z = (float)atof(aStr);		// 整数型に変換
								}
							} while (memcmp(aStr, "END_PARTSSET", strlen("END_PARTSSET")) != 0);
							nCntModel++;	// モデルカウント
						}
					} while (memcmp(aStr, "END_CHARACTERSET", strlen("END_CHARACTERSET")) != 0);
				}

				if (memcmp(aStr, "NUM_MOTION", strlen("NUM_MOTION")) == 0)
				{// モーション数
					pStrCut += strlen("NUM_MOTION");			// ポインタを進める
					PopString(pStrCut, aStr);					// 文字変換
					m_MaxAnim = atoi(aStr);						// 整数型に変換		

					if (m_AnimInfo == NULL)
					{// NULLの場合
						m_AnimInfo = new CMotion::ANIM_INFO[m_MaxAnim];		// メモリ確保

						for (int nCount = 0; nCount < m_MaxAnim; nCount++)
						{// カウント
							m_AnimInfo[nCount].aKeyInfo = NULL;		// 初期化
						}
					}
				}
				else if (memcmp(aStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
				{// モーション設定
					nCntKey = 0;	// キー初期化

					do
					{// ループ
						pStrCut = ReadFile(pFile, &aLine[0]);				// コメントチェック
						strcpy(aStr, pStrCut);								// 文字変換

						if (memcmp(aStr, "LOOP", strlen("LOOP")) == 0)
						{// ループするか
							pStrCut += strlen("LOOP");							// ポインタを進める
							PopString(pStrCut, aStr);							// 文字変換
							m_AnimInfo[nCntMotion].bLoop = atoi(aStr);			// 整数型に変換
						}
						else if (memcmp(aStr, "NUM_KEY", strlen("NUM_KEY")) == 0)
						{// キー数
							pStrCut += strlen("NUM_KEY");						// ポインタを進める
							PopString(pStrCut, aStr);							// 文字変換
							m_AnimInfo[nCntMotion].nKeyMax = atoi(aStr);		// 整数型に変換

							if (m_AnimInfo[nCntMotion].aKeyInfo == NULL)
							{// NULLの場合
								m_AnimInfo[nCntMotion].aKeyInfo = new CMotion::KEY_INFO[m_AnimInfo[nCntMotion].nKeyMax];	// メモリ確保

								for (int nCount = 0; nCount < m_AnimInfo[nCntMotion].nKeyMax; nCount++)
								{// カウント
									m_AnimInfo[nCntMotion].aKeyInfo[nCount].aKey = NULL;	// 初期化
								}
							}
						}
						else if (memcmp(aStr, "BLEND_FRAME", strlen("BLEND_FRAME")) == 0)
						{// ブレンドフレーム
							pStrCut += strlen("BLEND_FRAME");						// ポインタを進める
							PopString(pStrCut, aStr);								// 文字変換
							m_AnimInfo[nCntMotion].nBrendFrame = atoi(aStr);		// 整数型に変換
						}
						else if (memcmp(aStr, "KEYSET", strlen("KEYSET")) == 0)
						{// キー設定
							nCntModel = 0;		// モデルカウント初期化

							if (nCntKey < m_AnimInfo[nCntMotion].nKeyMax)
							{// キーが最大以下の場合
								if (m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey == NULL)
								{// NULLの場合
									m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey = new CMotion::KEY[m_MaxModel];	// メモリ確保
								}
							}

							do
							{// ループ
								pStrCut = ReadFile(pFile, &aLine[0]);					// コメントチェック
								strcpy(aStr, pStrCut);									// 文字変換

								if (memcmp(aStr, "FRAME", strlen("FRAME")) == 0)
								{// フレーム
									pStrCut += strlen("FRAME");										// ポインタを進める
									PopString(pStrCut, aStr);										// 文字変換
									m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = atoi(aStr);	// 整数型に変換
								}
								else if (memcmp(aStr, "KEY", strlen("KEY")) == 0)
								{// キーモデル設定
									do
									{// ループ
										pStrCut = ReadFile(pFile, &aLine[0]);			// コメントチェック
										strcpy(aStr, pStrCut);							// 文字変換

										if (memcmp(aStr, "POS", strlen("POS")) == 0)
										{// 位置
											pStrCut += strlen("POS");																// ポインタを進める
											PopString(pStrCut, aStr);																// 文字変換
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x = (float)atof(aStr);		// 整数型に変換
											PopString(pStrCut, aStr);																// 文字変換
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y = (float)atof(aStr);		// 整数型に変換
											PopString(pStrCut, aStr);																// 文字変換
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z = (float)atof(aStr);		// 整数型に変換
										}
										else if (memcmp(aStr, "ROT", strlen("ROT")) == 0)
										{// 向き
											pStrCut += strlen("ROT");																// ポインタを進める
											PopString(pStrCut, aStr);																// 文字変換
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x = (float)atof(aStr);		// 整数型に変換
											PopString(pStrCut, aStr);																// 文字変換
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y = (float)atof(aStr);		// 整数型に変換
											PopString(pStrCut, aStr);																// 文字変換
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z = (float)atof(aStr);		// 整数型に変換
										}
									} while (memcmp(aStr, "END_KEY", strlen("END_KEY")) != 0);
									nCntModel++;	// モデルカウンタ
								}
							} while (memcmp(aStr, "END_KEYSET", strlen("END_KEYSET")) != 0);
							nCntKey++;		// キーカウンタ
						}
					} while (memcmp(aStr, "END_MOTIONSET", strlen("END_MOTIONSET")) != 0);
					nCntMotion++;		// モーションカウンタ
				}
			} while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0);
		}

		fclose(pFile);		// ファイルクローズ
	}

	// メモリ確保
	if (m_pTexture == NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_MaxModel];
	if (m_pMesh == NULL)	m_pMesh = new LPD3DXMESH[m_MaxModel];
	if (m_pBuffMat == NULL) m_pBuffMat = new LPD3DXBUFFER[m_MaxModel];
	if (m_nNumMat == NULL)	m_nNumMat = new DWORD[m_MaxModel];


	for (int nCntParnet = 0; nCntParnet < m_MaxModel; nCntParnet++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(m_ModelInfo[nCntParnet].cFileName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCntParnet], NULL, &m_nNumMat[nCntParnet], &m_pMesh[nCntParnet]);

		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCntParnet]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCntParnet]];
		m_pTexture[nCntParnet] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntParnet]];

		for (DWORD tex = 0; tex < m_nNumMat[nCntParnet]; tex++)
		{// カウント
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[nCntParnet][tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// テクスチャを使用している
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[nCntParnet][tex])))
				{
					//MessageBox(NULL, "テクスチャの読み込みに失敗しました(player)", NULL, MB_OK);
				}
			}
		}

		delete[] matDef;		// メモリ開放
		matDef = NULL;			// NULLへ
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CPlayer::Unload(void)
{
	for (int nCntParnet = 0; nCntParnet < m_MaxModel; nCntParnet++)
	{// カウント
	 // テクスチャの破棄
		for (DWORD tex = 0; tex < m_nNumMat[nCntParnet]; tex++)
		{// カウント
			if (m_pTexture[nCntParnet][tex] != NULL)
			{// NULL以外の場合
				m_pTexture[nCntParnet][tex]->Release();		// 解放
				m_pTexture[nCntParnet][tex] = NULL;			// NULLへ
			}
		}

		delete[] m_pTexture[nCntParnet];		// メモリ開放
		m_pTexture[nCntParnet] = NULL;			// NULLへ

												// マテリアルの開放
		if (m_pBuffMat[nCntParnet] != NULL)
		{// NULL以外の場合
			m_pBuffMat[nCntParnet]->Release();				// 解放
			m_pBuffMat[nCntParnet] = NULL;					// NULLへ
		}

		// マテリアルの開放
		if (m_pMesh[nCntParnet] != NULL)
		{// NULL以外の場合
			m_pMesh[nCntParnet]->Release();					// 解放
			m_pMesh[nCntParnet] = NULL;						// NULLへ
		}

		m_nNumMat[nCntParnet] = 0;				// 初期化
	}

	// メモリ開放
	if (m_pTexture != NULL) { delete[] m_pTexture; m_pTexture = NULL; }
	if (m_pMesh != NULL) { delete[] m_pMesh; m_pMesh = NULL; }
	if (m_pBuffMat != NULL) { delete[] m_pBuffMat; m_pBuffMat = NULL; }
	if (m_nNumMat != NULL) { delete[] m_nNumMat; m_nNumMat = NULL; }

	// モデル
	if (m_ModelInfo != NULL)
	{// NULL以外の場合
		delete[] m_ModelInfo;		// メモリ開放
		m_ModelInfo = NULL;			// NULLへ
	}

	// モーション
	if (m_AnimInfo != NULL)
	{// NULL以外の場合
		for (int nCntAnim = 0; nCntAnim < m_MaxAnim; nCntAnim++)
		{// モーションカウント
			for (int nCntKey = 0; nCntKey < m_AnimInfo[nCntAnim].nKeyMax; nCntKey++)
			{// キーカウント
				delete[] m_AnimInfo[nCntAnim].aKeyInfo[nCntKey].aKey;	// メモリ開放
				m_AnimInfo[nCntAnim].aKeyInfo[nCntKey].aKey = NULL;		// NULLへ
			}
			delete[] m_AnimInfo[nCntAnim].aKeyInfo;		// メモリ開放
			m_AnimInfo[nCntAnim].aKeyInfo = NULL;		// NULLへ
		}

		delete[] m_AnimInfo;	// メモリ開放
		m_AnimInfo = NULL;		// NULLへ
	}
}

//=============================================================================
// 確保処理											(public)	*** CPlayer ***
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *pPlayer = NULL;			// ポインタ

	if (pPlayer == NULL)
	{// NULLの場合
		pPlayer = new CPlayer;			// メモリ確保
	}

	if (pPlayer != NULL)
	{// NULL以外の場合
		pPlayer->m_pModel = new CModel[m_MaxModel];		// メモリ確保

		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// モデルカウンタ
			// 初期値設定
			pPlayer->m_pModel[nCntModel].SetInitAll(m_pTexture[nCntModel], m_pMesh[nCntModel], m_pBuffMat[nCntModel], m_nNumMat[nCntModel], m_ModelInfo[nCntModel].pos, m_ModelInfo[nCntModel].rot);
			// 初期化処理
			pPlayer->m_pModel[nCntModel].Init();

			if (0 <= m_ModelInfo[nCntModel].nIndex)
			{// 子の場合
				// 親を設定
				pPlayer->m_pModel[nCntModel].SetParent(&pPlayer->m_pModel[m_ModelInfo[nCntModel].nIndex]);
			}
		}

		// 値設定
		pPlayer->m_pos = pos;				// 位置
		pPlayer->m_rot = rot;				// 角度
		pPlayer->m_frotDest = rot.y;		// 目標角度
		pPlayer->Init();					// 初期化処理
	}

	return pPlayer;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CPlayer ***
//=============================================================================
HRESULT CPlayer::Init()
{
	// モーション
	if (m_pMotion == NULL)
	{// NULLの場合
		m_pMotion = new CMotion;			// メモリ確保
	}

	// 判定設定
	m_vtxMax = D3DXVECTOR3(16.0f, 50.0f, 16.0f);
	m_vtxMin = D3DXVECTOR3(-16.0f, 0.0f, -16.0f);

	m_pMotion->SetAnimNum(0);		// 待機モーション

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Uninit(void)
{
	// モデル
	if (m_pModel != NULL)
	{// NULL以外の場合
		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// カウント
			m_pModel[nCntModel].Uninit();	// 終了処理
		}

		delete[] m_pModel;		// メモリ開放
		m_pModel = NULL;		// NULLへ
	}

	// モーション
	if (m_pMotion != NULL)
	{// NULL以外の場合
		m_pMotion->Uninit();	// 終了処理
		delete m_pMotion;		// メモリ開放
		m_pMotion = NULL;		// NULLへ
	}

	CGame::SetPlayer();			// プレイヤー設定

	CScene::SetDeath();						// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Update(void)
{
	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_TEST);			// フォント取得

	// カメラ設定
	D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// 視点取得
	D3DXVECTOR3 posRDest = CManager::GetCamera()->GetPosRDest();	// 注視点取得

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色

	// サウンド
	CSound *pSound = CManager::GetSound();

	switch (m_state)
	{// 状態
	case STATE_NONE:			// 通常
		break;
	case STATE_DAMAGE:			// ダメージ
		m_nCntState++;			// 状態カウント

		// 点滅
		if (m_nCntState / 9 % 2)
		{// 赤
			col = D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f);
		}
		else
		{// 白
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		if (m_nCntState == 90)
		{// 一定のカウントに達したら通常へ
			m_nCntState = 0;
			m_state = STATE_NONE;
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// モデル
			m_pModel[nCntModel].Setcol(col);
		}

		break;
	case STATE_DEATH:			// しんだ
		break;
	}

	bool bMeshField = false;			// メッシュフィールド有無
	bool bMeshRiver = false;			// メッシュリバー有無
	CMeshField *pMeshField = NULL;		// メッシュフィールド
	CMeshRiver *pMeshRiver = NULL;		// メッシュリバー

	for (int nCount = 0; nCount < CGame::m_MeshFieldMax; nCount++)
	{// フィールドカウンタ
		pMeshField = CGame::GetMeshField(nCount);

		if (pMeshField != NULL)
		{// NULL以外の場合
			if (pMeshField->ColInMesh(m_pos + m_vtxMax))
			{// 判定内にいるか
				bMeshField = true;			// メッシュ有り
				break;
			}
			else if (pMeshField->ColInMesh(m_pos))
			{// 判定内にいるか
				bMeshField = true;			// メッシュ有り
				break;
			}
			else if (pMeshField->ColInMesh(m_pos + m_vtxMin))
			{// 判定内にいるか
				bMeshField = true;			// メッシュ有り
				break;
			}
		}
	}

	for (int nCount = 0; nCount < CGame::m_MeshRiverMax; nCount++)
	{// リバーカウンタ
		pMeshRiver = CGame::GetMeshRiver(nCount);

		if (pMeshRiver != NULL)
		{// NULL以外の場合
			if (pMeshRiver->ColInMesh(m_pos + m_vtxMax))
			{// 判定内にいるか
				bMeshRiver = true;			// メッシュ有り
				break;
			}
			else if (pMeshRiver->ColInMesh(m_pos))
			{// 判定内にいるか
				bMeshRiver = true;			// メッシュ有り
				break;
			}
			else if (pMeshRiver->ColInMesh(m_pos + m_vtxMin))
			{// 判定内にいるか
				bMeshRiver = true;			// メッシュ有り
				break;
			}
		}
	}

	m_posOld = m_pos;		// 位置保存

	float fHeight = -500.0f;		// 高さ
	float fUpDown = 0.0f;			// ソン度
	float frotDest = 0.0f;			// 目標向き
	float fNor = 0.0f;				// 法線

	if (bMeshField == true)
	{// メッシュフィールドがある
		fHeight = pMeshField->GetHeight(m_pos);			// 高さ取得
		//fUpDown = CGame::GetMeshField(nMeshNum)->GetUpDown(m_pos, m_rot);		// 高さ取得

		//fNor = CGame::GetMeshField(nMeshNum)->GetBlockNor(m_pos);				// ブロック法線

		/*
		// 滑る処理
		if (fNor < 0.8f && fNor != 0.0f)
		{// 範囲チェック
			if (m_bSlip == false)
			{// 滑ってなかったら
				m_fSliprot = m_frotDest;	// 向き設定
			}
			m_bSlip = true;		// 滑っている
		}
		else
		{// 範囲外
			m_bSlip = false;	// 滑っていない
			m_fSliprot = 0.0f;	// 向き初期化
		}

		if (m_bSlip == true)
		{// 滑っている
			frotDest = CGame::GetMeshField(nMeshNum)->GetUpDown(m_pos, D3DXVECTOR3(m_rot.x, m_fSliprot, m_rot.z));		// 傾斜設定

																														// 向き変更
			if (frotDest < fUpDown)
			{
				m_fSliprot -= 0.08f;
			}
			else
			{
				m_fSliprot += 0.08f;
			}

			// 滑る
			m_move.x += sinf(m_fSliprot) * (1.2f + (1.5f * frotDest));
			m_move.z += cosf(m_fSliprot) * (1.2f + (1.5f * frotDest));
		}*/
	}

	m_shadowHeight = fHeight;		// 影高さ設定

	if (m_pos.x >= 13508)
	{//	ゲーム終了のフラグ立てる
		CTime::SetTimer(false);
		m_bEnd = true;				// エンド
		m_bMove = false;			// 移動終了
	}
	if (m_bEnd == true)
	{//	フラグがたったら
		if (m_pos.x >= 13508)
		{// 一定以上だったら
			if (m_pos.x <= 14300)
			{// 一定以下だったら
				// 自動走行
				m_frotDest = -D3DX_PI * 0.5f;
				m_move.x += sinf(-m_frotDest) * ((1.3f - m_bWallJumpmove) + (1.3f * fUpDown));
				m_move.z += cosf(-m_frotDest) * ((1.3f - m_bWallJumpmove) + (1.3f * fUpDown));
				m_pMotion->SetAnimNum(1);		// 歩くモーション
			}
			if (m_pos.x >= 13600)
			{//	これ以上の座標に行ったら
				m_bClear = true;			//ゲームclear

				if (m_bClear == true)
				{//	ゲームクリアだったら
					if (CFade::GetFade() == CFade::FADE_NONE)
					{// フェードしていない状態
						//	ゲームクリアに画面遷移
						CGame::SetGame(CGame::MODE_GAMECLEAR);
						CFade::Create(CManager::MODE_RESULT, CFade::FADE_OUT);

					}
				}

			}
		}
	}
	else if (m_bStart == true)
	{// スタート
		if (m_pos.x < 0.0f)
		{// 一定以下だったら
			// 自動走行
			m_frotDest = -D3DX_PI * 0.5f;
			m_move.x += sinf(-m_frotDest) * ((1.3f - m_bWallJumpmove) + (1.3f * fUpDown));
			m_move.z += cosf(-m_frotDest) * ((1.3f - m_bWallJumpmove) + (1.3f * fUpDown));

			if (m_pMotion->GetAnimNum() != 1)
			{// ジャンプしていない
				m_pMotion->SetAnimNum(1);		// 歩くモーション
			}
		}
		else
		{// その他
			if (m_nCntStart == 0)
			{// カウントが０だったら
				if (m_pMotion->GetAnimNum() != 0)
				{// ジャンプしていない
					m_pMotion->SetAnimNum(0);		// 停止モーション
				}
			}

			m_nCntStart++;		// カウントする

			if (m_nCntStart == 30)
			{// カウントが３０だったら
				CEffect::Create(m_pos + D3DXVECTOR3(0.0f, 40.0f, 0.0f), 8, 11);//　テクスチャ　テキスト
			}
			else if (m_nCntStart == 65)
			{// カウントが６５だったら
				CStart::Create(D3DXVECTOR3(posVDest.x, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f), D3DXVECTOR3(100.0f, 100.0f, 0.0f));
			}
			else if (m_nCntStart == 180)
			{// カウントが１８０だったら
				m_bStart = false;		// スタート終了
				m_nCntStart = 0;		// カウント初期化
				m_bMove = true;			// 移動開始

				CTime::SetTimer(true);
			}
		}
	}
	else
	{// その他
		if (m_bmoveUp == true)	//			[スピードUP]
		{//	アイテムによってスピードUPしている状態
			m_nMoveCounter++;
			if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputKeyboard()->GetPress(DIK_D) == true ||
				CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
			{//	アイテムを取得した時の状態
				m_move.x += sinf(-m_frotDest) * (MOVE_UPSPEED + (MOVE_UPSPEED * fUpDown));
			}
			if (m_nMoveCounter > MOVE_UPCOUNTER)
			{//	時間経過によってアイテムの効果がきれ
				m_bmoveUp = false;	//	動きを通常に戻す
				m_nMoveCounter = 0;	//	カウンターを初期化する
			}
		}

		// 移動処理
		if (m_bMove == true)
		{// 移動できる
			if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true)
			{// （A）を押したら
			 // 向き変更、移動量設定
				m_frotDest = D3DX_PI * 0.5f;
				m_move.x += sinf(-m_frotDest) * ((1.3f - m_bWallJumpmove) + (1.3f * fUpDown));
				m_move.z += cosf(-m_frotDest) * ((1.3f - m_bWallJumpmove) + (1.3f * fUpDown));

				if (m_bJump == false && m_pMotion->GetAnimNum() != 6)
				{// ジャンプしていない
					m_pMotion->SetAnimNum(1);		// 歩くモーション
				}
			}
			else if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
			{// （D）を押したら
			 // 向き設定、移動処理
				m_frotDest = -D3DX_PI * 0.5f;
				m_move.x += sinf(-m_frotDest) * ((1.3f - m_bWallJumpmove) + (1.3f * fUpDown));
				m_move.z += cosf(-m_frotDest) * ((1.3f - m_bWallJumpmove) + (1.3f * fUpDown));

				if (m_bJump == false && m_pMotion->GetAnimNum() != 6)
				{// ジャンプしていない
					m_pMotion->SetAnimNum(1);		// 歩くモーション
				}
			}
			else
			{// 移動していない
				if (m_pMotion->GetAnimNum() != 4)
				{// しぬモーション以外
					m_pMotion->SetStop(false);		// モーション動かす
				}

				m_bWallJump = false;				// 壁キックしていない

				if (m_bJump == false && m_pMotion->GetAnimNum() != 3 && m_pMotion->GetAnimNum() != 6 && (m_pMotion->GetAnimNum() != 8 || m_pMotion->GetAnimKeyNum() == 1))
				{// ジャンプしていない、着地モーション以外、（殴りモーション以外、キーが１なら）
					m_pMotion->SetAnimNum(0);		// 待機モーション
				}
			}

			if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true || CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_A, 0) == true)
			{// （スペース）を押したら
				if (m_bWallJump == true)
				{// 壁キックしている
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE017);
					if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true)
					{// （A）を押したら
					 // 向き設定、移動量を初期化、移動処理
						m_frotDest = -D3DX_PI * 0.5f;
						m_move.x = 0.0f;
						m_move.x += sinf(-m_frotDest) * 20.0f;
						m_move.z += cosf(-m_frotDest) * 20.0f;
						m_pMotion->SetStop(false);					// モーション停止
					}
					else if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
					{// （D）を押したら
					 // 向き設定、移動量を初期化、移動処理
						m_frotDest = D3DX_PI * 0.5f;
						m_move.x = 0.0f;
						m_move.x += sinf(-m_frotDest) * 20.0f;
						m_move.z += cosf(-m_frotDest) * 20.0f;
						m_pMotion->SetStop(false);					// モーション停止ddad
					}

					m_bWallJump = false;		// 壁キックしない
					m_bWallJumpmove = 1.3f;		// 壁キック移動量
					m_move.y = 12.0f;			// 上昇
					m_fDownSpeed = MOVE_DOWNSPEED_NOR;		// 減速初期化
					m_bJump = true;				// ジャンプしている
					m_pMotion->SetAnimNum(2);	// ジャンプモーション
				}

				if (m_bJump == false)
				{// ジャンプ以外
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE016);
					m_bWallJump = false;		// 壁キックしない
					m_move.y = 10.0f;			// 上昇
					m_fDownSpeed = MOVE_DOWNSPEED_NOR;		// 減速初期化
					m_bJump = true;				// ジャンプしている

					if (m_pMotion->GetAnimNum() != 6)
						m_pMotion->SetAnimNum(2);	// ジャンプモーション
				}
			}
		}
	}

	if ((m_move.x > 0.5f || m_move.x < -0.5f) && m_pos.y <= 5.0f)
	{// 移動量が一定以上の場合
		m_nCntEffect++;		// カウントする
		m_SoundCounter++;	// カウントする
		if ((m_nCntEffect % 6) == 0)
		{//	煙
			CEffect::Create(m_pos, 4, 4);//　テクスチャ　テキスト
			CEffect::Create(m_pos, 2, 3);//　テクスチャ　テキスト
		}
		if ((m_SoundCounter % 30) == 0)
		{	// ダッシュ音
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE009);
		}
	}
	if (m_move.y > 5.0f)
	{// 上昇した
		CEffect::Create(m_pos, 3, 5);//　テクスチャ　テキスト
	}

	if (bMeshRiver == true)
	{// リバーがある
		if (pMeshRiver != NULL)
		{// NULL以外の場合
			if (m_pos.y < pMeshRiver->Getpos().y)
			{// 川に入ったら
				m_move.x *= 0.7f;		// 遅くなる

				if (m_move.y < -12.0f)
				{// 入った瞬間
					m_move.y *= 0.8f;
					CEffect::Create(m_pos, 7, 0);//　テクスチャ　テキスト
				}
				if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputKeyboard()->GetPress(DIK_D) == true
					|| CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
				{// 入って進んでいる
					CEffect::Create(m_pos, 7, 0);//　テクスチャ　テキスト
				}
			}
		}
	}

	m_pos += m_move;	// 移動量加算

	// ブロックチェック
	bool bBlock = false;	// 当たった
	bool bClimb = false;	// 登っている
	bool bWallJump = false;	// 登っている

	// シーン
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_3);	// 現在　
	CScene *pSceneNext = NULL;								// 次

	while (pScene != NULL)
	{// NULL以外の場合
		pSceneNext = pScene->GetSceneNext();				// 次を保存
		if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
		{// オブジェクトタイプがブロックだったら
			CBlock *pBlock = (CBlock*)pScene;				// ブロックにキャスト
															//bool bLand[4] = { pBlock->Collision(&m_pos, &m_posOld, &m_move, &m_fDownSpeed, m_vtxMax, m_vtxMin) };		// 判定チェック
			pBlock->Collision(&m_pos, &m_posOld, &m_move, &m_fDownSpeed, m_vtxMax, m_vtxMin);		// 判定チェック
			bool *bLand = pBlock->GetHit();

			if (bLand[0] == true)
			{// 上判定
				pFont->SetMessage(CFont::SETTYPE_ADD, "*0\n");
				m_bJump = false;		// ジャンプしない
				bBlock = true;			// 当たった

				m_shadowHeight = pBlock->Getpos().y + pBlock->Getsize(0).y;

				if (m_move.y < -16.0f && m_state == STATE_NONE)
				{// 移動量に応じてダメージを受ける、通常状態だったら
					m_state = STATE_DAMAGE;		// ダメージ状態
					CLife::HitDamage();			// 体力を減らす
				}

				if (pBlock->GetType() == CBlock::COLTYPE_ICE)
				{// 滑るブロック
					m_fDownSpeed = MOVE_DOWNSPEED_ICE;
				}
				else
				{
					m_fDownSpeed = MOVE_DOWNSPEED_NOR;

					if (pBlock->GetType() == CBlock::COLTYPE_SKY)
					{// 空中ブロック
						if (CManager::GetInputKeyboard()->GetPress(DIK_S) || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_DOWN, 0) == true)
						{// （B）を押したら
							m_pos.y -= 1.0f;
							m_posOld.y = m_pos.y;
						}
					}
				}
			}
			else if (bLand[1] == true)
			{// 下判定
				pFont->SetMessage(CFont::SETTYPE_ADD, "*1\n");
				bBlock = true;			// 当たった
			}
			else if (bLand[2] == true)
			{// 左判定
				pFont->SetMessage(CFont::SETTYPE_ADD, "*2\n");
				bBlock = true;			// 当たった

				if (pBlock->GetType() == CBlock::COLTYPE_PUSH)
				{// 押すブロック
					m_pMotion->SetAnimNum(7);	// 押すモーション
					if ((m_SoundCounter % 30) == 0)
					{	// 引き摺る音
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE025);
					}

				}
				else if (pBlock->GetType() == CBlock::COLTYPE_BREAK)
				{// 破壊ブロック
					if (CManager::GetInputKeyboard()->GetPress(DIK_B))
					{// （B）を押したら
						m_pMotion->SetAnimNum(8);		// 破壊モーション
						pBlock->Uninit();				// ブロック破棄
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE032);	// 破壊音
					}
				}
				else if (pBlock->GetType() == CBlock::COLTYPE_FALL)
				{// 倒れるブロック
					if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
					{// ブロック方向に押していたら
						if (D3DX_PI * 0.3f < pBlock->Getrot().z)		// 範囲を指定して回転
							pBlock->Setrot(pBlock->Getrot() + D3DXVECTOR3(0.0f, 0.0f, -0.038f));
						if (pBlock->Getrot().z != 0.0f)					// 倒れたら押すモーション
							m_pMotion->SetAnimNum(7);
						if ((m_SoundCounter % 30) == 0)
						{	// 引き摺る音
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE025);
						}
					}
				}
				else if (pBlock->GetType() == CBlock::COLTYPE_CLIMB)
				{// 登れるブロック
					m_move.y = 0.0f;				// 移動量（Y）を初期化
					m_bJump = false;				// ジャンプしていない
					bClimb = true;					// 登っている
					m_pMotion->SetStop(true);		// モーション停止

					if (m_pMotion->GetAnimNum() != 6)
					{// 登りモーション以外
						m_pMotion->SetAnimNumrot(6, 0);		// 登りモーション
						m_AnimEditCounter = 0;				// モーション編集カウンタ初期化
					}

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE))
					{// （スペース)を押したら
						m_pMotion->SetAnimNumrot(6, (m_pMotion->GetAnimKeyNum() + 1) % m_AnimInfo[m_pMotion->GetAnimNum()].nKeyMax);
					}

					if (CManager::GetInputKeyboard()->GetPress(DIK_W) || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_UP, 0) == true)
					{// （W)を押したら
					 // 上昇、モーションキーを進める
						m_pos.y += 2.0f;
						m_AnimEditCounter++;
						if (m_AnimEditCounter % 5 == 0)
							m_pMotion->SetAnimNumrot(6, (m_pMotion->GetAnimKeyNum() + 1) % m_AnimInfo[m_pMotion->GetAnimNum()].nKeyMax);
						// ツタの音
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE026);
					}
					else if (CManager::GetInputKeyboard()->GetPress(DIK_S) || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_DOWN, 0) == true)
					{// （S)を押したら
					 // 下降、モーションキーを進める
						m_pos.y -= 2.0f;
						m_AnimEditCounter++;
						if (m_AnimEditCounter % 5 == 0)
							m_pMotion->SetAnimNumrot(6, (m_pMotion->GetAnimKeyNum() + 1) % m_AnimInfo[m_pMotion->GetAnimNum()].nKeyMax);
						// ツタの音
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE026);
					}
					else
					{
						m_AnimEditCounter = 0;
					}
				}
				else if (pBlock->GetType() == CBlock::COLTYPE_WALLJUMP)
				{// 壁キックブロック
					bWallJump = true;

					if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
					{// ブロック方向に押していたら
						if (m_pMotion->GetAnimNum() != 6)
						{// 登りモーション以外
							m_pMotion->SetAnimNumrot(6, 0);		// 登りモーション
							m_AnimEditCounter = 0;				// モーション編集カウンタ初期化
							m_pMotion->SetStop(true);			// モーション停止
						}

						m_move.y = 0.0f;			// 移動量（Y）を初期化
						m_pos.y -= 0.2f;			// 壁を滑る
						m_bWallJump = true;			// 壁キックする
					}
				}
			}
			else if (bLand[3] == true)
			{
				pFont->SetMessage(CFont::SETTYPE_ADD, "*3\n");
				bBlock = true;			// 当たった

				if (pBlock->GetType() == CBlock::COLTYPE_PUSH)
				{// 押すブロック
					m_pMotion->SetAnimNum(7);	// 押すモーション
					if ((m_SoundCounter % 30) == 0)
					{	// 引き摺る音
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE025);
					}
				}
				else if (pBlock->GetType() == CBlock::COLTYPE_BREAK)
				{// 破壊ブロック
					if (CManager::GetInputKeyboard()->GetPress(DIK_B))
					{// （B）を押したら
						m_pMotion->SetAnimNum(8);		// 破壊モーション
						pBlock->Uninit();				// ブロック破棄
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE032);	// 破壊音
					}
				}
				else if (pBlock->GetType() == CBlock::COLTYPE_CLIMB)
				{// 登れるブロック
					m_move.y = 0.0f;				// 移動量（Y）を初期化
					m_bJump = false;				// ジャンプしていない
					bClimb = true;					// 登っている
					m_pMotion->SetStop(true);		// モーション停止

					if (m_pMotion->GetAnimNum() != 6)
					{// 登りモーション以外
						m_pMotion->SetAnimNumrot(6, 0);		// 登りモーション
						m_AnimEditCounter = 0;				// モーション編集カウンタ初期化
					}

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE))
					{// （スペース)を押したら
						m_pMotion->SetAnimNumrot(6, (m_pMotion->GetAnimKeyNum() + 1) % m_AnimInfo[m_pMotion->GetAnimNum()].nKeyMax);
					}

					if (CManager::GetInputKeyboard()->GetPress(DIK_W) || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
					{// （W)を押したら
					 // 上昇、モーションキーを進める
						m_pos.y += 2.0f;
						m_AnimEditCounter++;
						if (m_AnimEditCounter % 8 == 0)
							m_pMotion->SetAnimNumrot(6, (m_pMotion->GetAnimKeyNum() + 1) % m_AnimInfo[m_pMotion->GetAnimNum()].nKeyMax);
						// ツタの音
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE026);
					}
					else if (CManager::GetInputKeyboard()->GetPress(DIK_S) || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_DOWN, 0) == true)
					{// （S)を押したら
					 // 下降、モーションキーを進める
						m_pos.y -= 2.0f;
						m_AnimEditCounter++;
						if (m_AnimEditCounter % 8 == 0)
							m_pMotion->SetAnimNumrot(6, (m_pMotion->GetAnimKeyNum() + 1) % m_AnimInfo[m_pMotion->GetAnimNum()].nKeyMax);
						// ツタの音
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE026);
					}
					else
					{
						m_AnimEditCounter = 0;
					}
				}
				else if (pBlock->GetType() == CBlock::COLTYPE_WALLJUMP)
				{// 壁キックブロック
					bWallJump = true;

					if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputXPad()->GetPress(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true)
					{// ブロック方向に押していたら
						if (m_pMotion->GetAnimNum() != 6)
						{// 登りモーション以外
							m_pMotion->SetAnimNumrot(6, 0);		// 登りモーション
							m_AnimEditCounter = 0;				// モーション編集カウンタ初期化
							m_pMotion->SetStop(true);			// モーション停止
						}

						m_move.y = 0.0f;			// 移動量（Y）を初期化
						m_pos.y -= 0.2f;			// 壁を滑る
						m_bWallJump = true;			// 壁キックする
					}
				}
			}
		}
		pScene = pSceneNext;		// 次を設定
	}

	// シーン
	pScene = CScene::GetScene(CScene::PRIORITY_4);		// 現在
	pSceneNext = NULL;									// 次
	while (pScene != NULL)
	{// NULL以外の場合
		pSceneNext = pScene->GetSceneNext();
		if (pScene->GetObjType() == CScene::OBJTYPE_ITEM)
		{//	オブジェクトがアイテムだったら
			CItem *pItem = (CItem*)pScene;
			if (pItem->CollisionItem(&m_pos, &m_move, D3DXVECTOR3(30.0f, 30.0f, 30.0f), D3DXVECTOR3(-30.0f, -30.0f, -30.0f)) == true)
			{//	アイテムに当たったら
				m_bmoveUp = true;		//	動きを早くする
			}
		}
		pScene = pSceneNext;		// 次を設定
	}

	// シーン
	pScene = CScene::GetScene(CScene::PRIORITY_1);		// 現在
	pSceneNext = NULL;									// 次
	while (pScene != NULL)
	{// NULL以外の場合
		pSceneNext = pScene->GetSceneNext();
		if (pScene->GetObjType() == CScene::OBJTYPE_WALL)
		{//	オブジェクトが壁だったら
			CWall *pWall = (CWall*)pScene;
			pWall->Collision(&m_pos, &m_posOld, &m_move, D3DXVECTOR3(30.0f, 30.0f, 30.0f), D3DXVECTOR3(-30.0f, -30.0f, -30.0f));
		}
		pScene = pSceneNext;		// 次を設定
	}

	bool bHitEnemy = false;
	CEnemy *pEnemy = CGame::GetEnemy();

	if (pEnemy != NULL)
	{//	オブジェクトがアイテムだったら
		if (pEnemy->CollisionEnemy(&m_pos, &m_move, D3DXVECTOR3(30.0f, 30.0f, 30.0f), D3DXVECTOR3(-30.0f, -30.0f, -30.0f)) == true)
		{//	敵に当たったら
			bHitEnemy = true;
		}
	}

	if (bHitEnemy == true)
	{
		CLife::HitDamage();
		CLife::HitDamage();
		CLife::HitDamage();
		m_pMotion->SetAnimNum(4);
		m_bMove = false;
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE020);
	}

	if (bBlock == false)
	{// ブロックに当たっていない
		if (m_pMotion->GetAnimNum() != 4)
		{// しぬモーション以外
			m_pMotion->SetStop(false);		// モーション停止
		}

		if (m_pMotion->GetAnimNum() == 6)
		{// 登りモーション
			m_pMotion->SetStop(false);		// モーション停止
			m_pMotion->SetAnimNum(0, 0, 1);		// 待機モーション
		}

		m_bWallJump = false;

		m_nWallJump--;		// 壁キックカウンタ減らす
		if (m_nWallJump == 0)
		{// ０になったら
			m_bJump = true;	// ジャンプしている
		}
	}
	else
	{// 当たっている
		m_nWallJump = 5;	// 壁キックカウンタ設定
	}

	if (bBlock == true && m_bJump == false && m_bWallJump == false && bClimb == false && bWallJump == true && m_pMotion->GetAnimNum() == 6)
	{// 登りモーション
		m_pMotion->SetStop(false);		// モーション停止
		m_pMotion->SetAnimNum(0);		// 待機モーション
	}

	if (m_bWallJumpmove != 0.0f)
	{// 壁キック移動量がある
		if (0.0f < m_bWallJumpmove)
		{// 移動量がある
			m_bWallJumpmove -= 0.13f;		// 減らしていく
		}
		else
		{// ０以下になったら
			m_bWallJumpmove = 0.0f;			// 初期化
		}
	}

	if (m_pos.y < fHeight && bMeshField == true)
	{// メッシュの高さ未満、メッシュ有り
		if (m_move.y < -16.0f && m_state == STATE_NONE)
		{// 移動量に応じてダメージを受ける、通常状態だったら
			m_state = STATE_DAMAGE;		// ダメージ状態
			CLife::HitDamage();			// 体力を減らす
		}

		m_pos.y = fHeight;				// 高さ設定
		m_move.y = 0.0f;				// 移動量（Y）を初期化
		m_fDownSpeed = MOVE_DOWNSPEED_NOR;			// 減速初期化
		m_nWallJump = 5;				// 壁キックカウンタ初期化
		m_bWallJumpmove = 0.0f;			// 壁キック移動量初期化

		m_bJump = false;				// ジャンプしていない

		if (m_pMotion->GetAnimNum() == 6)
		{// 登りモーション
			m_pMotion->SetStop(false);		// モーション停止
			m_pMotion->SetAnimNum(0);		// 待機モーション
		}
	}

	if (m_move.y < 0.0f && m_bJump == true)
	{// 下降している、ジャンプしている
		m_pMotion->SetAnimNum(3);		// 着地モーション
	}

	// カメラに対して移動範囲を指定
	if (m_pos.x < posVDest.x - 350.0f)
	{// 左限界
		m_pos.x = posVDest.x - 350.0f;		// 位置設定
		m_move.x = 0.0f;					// 移動量（X)を初期化
	}
	else if (posVDest.x < m_pos.x)
	{// 右限界
		if (0.0f < m_move.x)
		{// 右に移動している
		 // カメラを動かす
			posVDest.x += m_move.x;			// 視点
			posRDest.x += m_move.x;			// 注視点
		}
	}

	if (m_bEnd == false)
	{
		// カメラ反映
		CManager::GetCamera()->SetPosVDest(posVDest);
		CManager::GetCamera()->SetPosRDest(posRDest);
	}

	if (0.65f < m_bWallJumpmove)
	{// 壁キック移動量チェック
		m_rot.y += ((m_frotDest - m_rot.y) / 5.0f) * ((0.65f - (m_bWallJumpmove / 2.0f)) / 0.65f);		// 向きを調整
	}
	else
	{// 範囲外
		m_rot.y += ((m_frotDest - m_rot.y) / 5.0f);														// 向きを調整
	}

	// 減速処理
	m_move.x -= (m_move.x - 0.0f) * m_fDownSpeed;
	m_move.y -= GRAVITY;
	m_move.z -= (m_move.z - 0.0f) * m_fDownSpeed;

	if (bClimb == true)m_move.y = 0.0f;			// 登っていたら移動量（Y)を初期化


	if (m_bStart == false || (m_bStart == true && m_nCntStart < 60))
	{
		m_pMotion->Update(m_MaxModel, &m_AnimInfo[0], &m_pModel[0], m_ModelInfo[0].pos);		// モーション更新
	}

	CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "Player : 位置　[%.2f, %.2f, %.2f]\n", m_pos.x, m_pos.y, m_pos.z);
	CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "Player : 向き　[%.2f, %.2f, %.2f]\n", m_rot.x, m_rot.y, m_rot.z);
	CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "Player : UpDown　[%.2f]\n", fUpDown);
	CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "Player : fNor　[%.2f]\n", fNor);
	CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "Player : m_bmoveUp　[%d]\n", m_bmoveUp);
	CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "\n");

	if (m_bEnd == false)
	{
		if (m_bStart == false)
		{
			if (m_bFalling == false)
			{
				if (m_pos.y < -150.0f)
				{// 下限設定
					m_pos.y = -150.0f;							// 位置設定
					m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// プレイヤー停止
					m_bMove = false;							// 移動できない
					m_bFalling = true;							// 落下EDに入るためのフラグ
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE039);
				}
			}

			if (m_bMove == false)
			{// 移動できない
				CLife::HitDamage();							// ダメージ受ける
			}

			if (CLife::GetLife() <= 0)
			{// 体力が０
				m_bMove = false;							// 移動できない
				m_pMotion->SetStop(false);					// モーション開始
				m_pMotion->SetAnimNum(4);					// しぬモーション
			}

			if (m_pMotion->GetAnimNum() == 4 && m_pMotion->GetAnimKeyNum() == 2)
			{// キーが２になったら
				m_pMotion->SetStop(true);					// モーション停止
			}
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス

											// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
	{
		if (m_pModel != NULL)
		{
			m_pModel[nCntModel].Draw();
			CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(m_pos.x, m_shadowHeight, m_pos.z));
		}
	}
}