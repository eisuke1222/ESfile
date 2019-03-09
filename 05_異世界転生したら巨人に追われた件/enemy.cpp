//=============================================================================
//
// 敵(巨人)処理 [player.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "enemy.h"			// プレイヤー
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "game.h"			// ゲーム
#include "input.h"			// 入力
#include "model.h"			// モデル
#include "motion.h"			// モーション
#include "camera.h"			// カメラ
#include "meshField.h"		// メッシュフィールド
#include "block.h"			// ブロック
#include "item.h"			// アイテム
#include "effect.h"			// エフェクト
#include "shadow.h"			// 影

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ANIM_NAME		"data\\TEXT\\giantmodeldata.txt"		// モーションデータ
#define MOVE_SPEED		(0.95f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CEnemy::m_MaxModel = 0;				// モデル数
int						CEnemy::m_MaxAnim = 0;				// モーション数

LPDIRECT3DTEXTURE9		**CEnemy::m_pTexture = NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CEnemy::m_pMesh = NULL;			// メッシュ情報へのポインタ
LPD3DXBUFFER			*CEnemy::m_pBuffMat = NULL;			// マテリアル情報へのポインタ
DWORD					*CEnemy::m_nNumMat = NULL;			// マテリアル情報の数

CEnemy::MODEL_INFO		*CEnemy::m_ModelInfo = NULL;		// モデル
CMotion::ANIM_INFO		*CEnemy::m_AnimInfo = NULL;			// モーション

//=============================================================================
// コンストラクタ									(public)	*** CPlayer ***
//=============================================================================
CEnemy::CEnemy() : CScene(CScene::PRIORITY_3, CScene::OBJTYPE_ENEMY)
{
	m_bStart = true;										// スタート
	m_nCntStart = 0;										// スタートカウント
		
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ（最大）
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ（最小）
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 角度
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 移動量
	m_frotDest = 0.0f;										// 目標角度
	m_bJump = false;										// ジャンプ
	m_bSlip = false;										// 滑る
	m_fDownSpeed = 0.2f;									// 減速速度
	m_nWallJump = 5;										// 壁キックカウント
	m_bWallJump = false;									// 壁キック
	m_bWallJumpmove = 0.0f;									// 壁キック移動量
	m_bmoveUp = false;										// 加速
	m_nMoveCounter = 0;										// 移動量カウント

	m_shadowHeight = 0.0f;									// 影高さ
}

//=============================================================================
// デストラクタ										(public)	*** CPlayer ***
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CEnemy::Load(void)
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
					//MessageBox(NULL, "テクスチャの読み込みに失敗しました(ememy)", NULL, MB_OK);
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
void CEnemy::Unload(void)
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
// 確保処理											(public)	*** CEnemy ***
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = NULL;			// ポインタ

	if (pEnemy == NULL)
	{// NULLの場合
		pEnemy = new CEnemy;			// メモリ確保
	}

	if (pEnemy != NULL)
	{// NULL以外の場合
		pEnemy->m_pModel = new CModel[m_MaxModel];		// メモリ確保

		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// カウント
			// 初期値設定
			pEnemy->m_pModel[nCntModel].SetInitAll(m_pTexture[nCntModel], m_pMesh[nCntModel], m_pBuffMat[nCntModel], m_nNumMat[nCntModel], m_ModelInfo[nCntModel].pos, m_ModelInfo[nCntModel].rot);
			// 初期化処理
			pEnemy->m_pModel[nCntModel].Init();

			if (0 <= m_ModelInfo[nCntModel].nIndex)
			{// 子の場合
				// 親を設定
				pEnemy->m_pModel[nCntModel].SetParent(&pEnemy->m_pModel[m_ModelInfo[nCntModel].nIndex]);
			}
		}

		pEnemy->m_pos = pos;			// 位置
		pEnemy->m_rot = rot;			// 角度
		pEnemy->m_frotDest = rot.y;		// 目標角度
		pEnemy->Init();					// 初期化処理
	}

	return pEnemy;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CEnemy ***
//=============================================================================
HRESULT CEnemy::Init()
{
	// モーション
	if (m_pMotion == NULL)
	{// NULLの場合
		m_pMotion = new CMotion;	// メモリ確保
	}

	// 判定設定
	m_vtxMax = D3DXVECTOR3(50.0f, 1000.0f, 50.0f);
	m_vtxMin = D3DXVECTOR3(-50.0f, 0.0f, -50.0f);

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Uninit(void)
{
	// モデル
	if (m_pModel != NULL)
	{// NULL以外の場合
		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// カウント
			m_pModel[nCntModel].Uninit();		// 終了処理
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

	CGame::SetEnemy();			// NULLへ

	CScene::SetDeath();			// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Update(void)
{
	bool bMeshField = false;					// メッシュフィールド有無
	CMeshField *pMeshField = NULL;				// メッシュフィールドポインタ
	CSound *pSound = CManager::GetSound();		// サウンド取得

	for (int nCount = 0; nCount < CGame::m_MeshFieldMax; nCount++)
	{// フィールドカウンタ
		pMeshField = CGame::GetMeshField(nCount);	// メッシュフィールド取得

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

	float fHeight = -500.0f;		// 高さ
	float fUpDown = 0.0f;			// 速度

	if (bMeshField == true)
	{// メッシュフィールドがある
		fHeight = pMeshField->GetHeight(m_pos);		// 高さ取得
	}
	m_shadowHeight = fHeight;		// 影高さ設定

	m_posOld = m_pos;		// 位置保存

	if (m_bStart == true)
	{// スタート
		if (m_pos.x < -300.0f)
		{// 一定位置以下のみ
			if (m_pos.x < -420.0f)
			{// 一定位置以下のみ
				m_pos.y = 300.0f;	// 高さ
			}
			else
			{// その他
				if (m_pos.y == 300.0f)
				{// 一定位置になったら
					if (m_pMotion->GetAnimNum() != 3)
					{// 歩くモーション以外だったら
						m_pMotion->SetAnimNum(3);	//	歩くモーション
					}
				}
				m_pos.y -= 20.0f;					// 重力
			}

			// 移動処理
			m_frotDest = -D3DX_PI * 0.5f;
			m_move.x += sinf(-m_frotDest) * ((0.8f - m_bWallJumpmove) + (0.8f * fUpDown));
			m_move.z += cosf(-m_frotDest) * ((0.8f - m_bWallJumpmove) + (0.8f * fUpDown));
		}
		else
		{// その他
			m_nCntStart++;

			if (m_nCntStart == 180)
			{// スタートカウント
				m_bStart = false;		// スタート終わり
				m_nCntStart = 0;		// スタートカウント
			}
		}
	}
	else
	{// その他
		// 移動処理
		m_frotDest = -D3DX_PI * 0.5f;
		m_move.x += sinf(-m_frotDest) * ((MOVE_SPEED - m_bWallJumpmove) + (MOVE_SPEED * fUpDown));
		m_move.z += cosf(-m_frotDest) * ((MOVE_SPEED - m_bWallJumpmove) + (MOVE_SPEED * fUpDown));

		m_pMotion->SetAnimNum(1);	//	歩くモーション
	}

	m_pos += m_move;		// 移動量加算

	bool bBlock = false;	// ブロックに当たったか

	CScene *pScene = CScene::GetScene(CScene::PRIORITY_3);		// シーン取得
	CScene *pSceneNext = NULL;									// 次シーン

	while (pScene != NULL)
	{// NULL以外の場合
		pSceneNext = pScene->GetSceneNext();					// 次シーンを記憶
		if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
		{//	ブロックの場合
			CBlock *pBlock = (CBlock*)pScene;					// ブロックにキャスト
			pBlock->Collision(&m_pos, &m_posOld, &m_move, &m_fDownSpeed, m_vtxMax, m_vtxMin);
			bool *bLand = pBlock->GetHit();			// 衝突位置の取得
			for (int nCount = 0; nCount < 4; nCount++)
			{// カウント
				if (bLand[nCount] == true)
				{// 衝突していたら
					bBlock = true;	// ブロックに当たった

					D3DXVECTOR3 posblock = pBlock->Getpos();		// 位置取得
					CEffect::Create(posblock, 5, 1);//　テクスチャ　テキスト
					CEffect::Create(posblock, 2, 3);//　テクスチャ　テキスト
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE032);	// サウンド
					pBlock->Uninit();		// ブロック終了処理
				}
			}
		}
		pScene = pSceneNext;		// 次シーン
	}

	if (m_pos.y < fHeight)
	{// 地面より低くなったら
		m_pos.y = fHeight;			// 高さ調整
		m_move.y = 0.0f;			// 移動量（Y）を０へ
		m_fDownSpeed = 0.2f;		// 減速速度を初期化
		m_nWallJump = 5;			// 壁キックカウント初期化
		m_bWallJumpmove = 0.0f;		// 壁キック移動量初期化
		m_bJump = false;			// ジャンプしていない
	}


	if (0.65f < m_bWallJumpmove)
	{// 壁キック移動量が一定以下
		// 角度調整
		m_rot.y += ((m_frotDest - m_rot.y) / 5.0f) * ((0.65f - (m_bWallJumpmove / 2.0f)) / 0.65f);
	}
	else
	{// その他
		// 角度調整
		m_rot.y += ((m_frotDest - m_rot.y) / 5.0f);
	}

	// 減速処理
	m_move.x -= (m_move.x - 0.0f) * 0.2f;
	//m_move.y -= 0.7f;
	m_move.z -= (m_move.z - 0.0f) * 0.2f;

	if (m_bStart == false || (m_bStart == true && m_nCntStart < 60))
	{// スタート状態でない、スタート状態 && 一定カウント以下
		m_pMotion->Update(m_MaxModel, &m_AnimInfo[0], &m_pModel[0], m_ModelInfo[0].pos);		// モーション更新
	}

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_TEST);		// フォント取得

	// 文字設定
	pFont->SetMessage(CFont::SETTYPE_ADD, "Enemy : 位置　[%.2f, %.2f, %.2f]\n", m_pos.x, m_pos.y, m_pos.z);
	pFont->SetMessage(CFont::SETTYPE_ADD, "Enemy : fHeight　[%.2f]\n", fHeight);
	pFont->SetMessage(CFont::SETTYPE_ADD, "Enemy : bMesh　[%d]\n", bMeshField);

	if (m_pos.x >= 12260)
	{// ゴールに近づいたら
		m_move.y -= 0.02f;	// 減速
	}
	if (m_pos.x >= 12780)
	{//	敵を消す処理
		CEffect::Create(m_pos + D3DXVECTOR3(0.0f, 80.0f, 0.0f), 3, 10);//　テクスチャ　テキスト
		CEffect::Create(m_pos + D3DXVECTOR3(0.0f, 80.0f, 0.0f), 6, 9);//　テクスチャ　テキスト
		Uninit();
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE028);
	}
}

//=============================================================================
// 描画処理											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxRot, mtxTrans, matScale;			// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//拡大縮小行列の作成
	D3DXMatrixScaling(&matScale, m_fScaleX, m_fScaleY, m_fScaleZ);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &matScale);

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
//=============================================================================
//	敵の当たり判定									(public)	*** CEnemy ***
//=============================================================================
bool CEnemy::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	bool bHit = false;		// 衝突

	// 判定チェック
	if (m_pos.x + (m_vtxMax.x / 2) > pPos->x + sizeMin.x && //　右
		m_pos.x + (m_vtxMin.x / 2) < pPos->x + sizeMax.x && //　左
		m_pos.z + (m_vtxMax.z / 2) > pPos->z + sizeMin.z && //　奥
		m_pos.z + (m_vtxMin.z / 2) < pPos->z + sizeMax.z && //　手前
		m_pos.y + (m_vtxMax.y / 2) > pPos->y + sizeMin.y && //　奥
		m_pos.y + (m_vtxMin.y / 2) < pPos->y + sizeMax.y)	//　手前
	{
		bHit = true;			// 当たった

		// 移動量調整
		pMove->x += 200.0f;		
		pMove->y += 10.0f;
	}

	return bHit;		// 値を返す
}