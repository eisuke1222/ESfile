//=============================================================================
//
// マップ処理 [map.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "map.h"			// マップ
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "fade.h"			// フェード
#include "texture.h"		// テクスチャ
#include "text.h"			// テキスト
#include "title.h"			// タイトル
#include "game.h"			// ゲーム
#include "effect.h"			// エフェクト
#include "player.h"			// プレイヤー
#include "enemy.h"			// 敵
#include "shadow.h"			// 影

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MC_SKYROT_0			(0.00001f)		// 空回転０
#define	MC_SKYROT_1			(0.00005f)		// 空回転１
#define	MC_AREACOLL_OLD		(0.92f)			// エリア判定前回
#define	MC_AREACOLL_NOW		(0.88f)			// エリア判定現在

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
// コンストラクタ										(public)	*** CMap ***
//=============================================================================
CMap::CMap() : CScene(CScene::PRIORITY_1, CScene::OBJTYPE_MAP)
{

}

//=============================================================================
// デストラクタ										(public)	*** CMap ***
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
// ロード処理										(public)	*** CMap ***
//=============================================================================
HRESULT CMap::Load(void)
{
	return S_OK;
}

//=============================================================================
// アンロード処理										(public)	*** CMap ***
//=============================================================================
void CMap::Unload(void)
{

}

//=============================================================================
// クリエイト										(public)	*** CMap ***
//=============================================================================
CMap *CMap::Create(TYPE type)
{
	CMap *pMap = NULL;			// ポインタ

	if (pMap == NULL)
	{// NULLの場合
		if(type == TYPE_NO_00)			// マップ_00
			pMap = new CMap_00;				// メモリ確保
		else if (type == TYPE_NO_01)	// マップ_01
			pMap = new CMap_01;				// メモリ確保
		else if (type == TYPE_NO_02)	// マップ_02
			pMap = new CMap_02;				// メモリ確保
		else if (type == TYPE_NO_03)	// マップ_03
			pMap = new CMap_03;				// メモリ確保
		else if (type == TYPE_NO_04)	// マップ_04
			pMap = new CMap_04;				// メモリ確保
	}

	if (pMap != NULL)
	{// NULL以外の場合
		pMap->Init();			// 初期化処理
	}

	return pMap;	// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CMap ***
//=============================================================================
void CMap::Uninit(void)
{
	CScene::SetDeath();		// 死亡フラグ
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// コンストラクタ										(public)	*** CMap_00 ***
//=============================================================================
CMap_00::CMap_00() : CMap()
{
	// 初期値設定
	m_pField = NULL;	// フィールド

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		m_pSky[nCount] = NULL;							// NULLへ
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// 回転
	}
}

//=============================================================================
// デストラクタ										(public)	*** CMap_00 ***
//=============================================================================
CMap_00::~CMap_00()
{

}

//=============================================================================
// ロード処理										(public)	*** CMap_00 ***
//=============================================================================
HRESULT CMap_00::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CMap_00 ***
//=============================================================================
void CMap_00::Unload(void)
{

}

//=============================================================================
// 初期化処理										(public)	*** CMap_00 ***
//=============================================================================
HRESULT CMap_00::Init(void)
{
	// メッシュフィールド
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	int nMaxVtx = m_pField->GetMaxVtx();

	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{
		D3DXVECTOR3 pos = m_pField->GetVtx(nCntVtx);

		if (pos.x < -1500.0f || 1500.0f < pos.x || pos.z < -1500.0f || 1500.0f < pos.z)
		{
			pos.y = 80.0f + (rand() % 80);

			m_pField->SetVtx(nCntVtx, pos);
		}
	}
	m_pField->SetVecNor();
	
	// 空１
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// 空２
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(true, false, false, false, true);
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMap_00 ***
//=============================================================================
void CMap_00::Uninit(void)
{
	// フィールド
	if (m_pField != NULL)
	{// NULL以外の場合
		m_pField->Uninit();		// 終了処理
		m_pField = NULL;		// NULLへ
	}

	// 空
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		if (m_pSky[nCount] != NULL)
		{// NULL以外の場合
			m_pSky[nCount]->Uninit();	// 終了処理
			m_pSky[nCount] = NULL;		// NULLへ
		}
	}

	CMap::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMap_00 ***
//=============================================================================
void CMap_00::Update(void)
{
	// 空0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// 回転

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// テクスチャ設定
	}

	// 空1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// 回転

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// テクスチャ設定
	}
}

//=============================================================================
// 描画処理											(public)	*** CMap_00 ***
//=============================================================================
void CMap_00::Draw(void)
{

}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CMap_01::m_nMaxModel	= 0;		// モデル数

LPDIRECT3DTEXTURE9		**CMap_01::m_pTexture	= NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CMap_01::m_pMesh		= NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER			*CMap_01::m_pBuffMat	= NULL;		// マテリアル情報へのポインタ
DWORD					*CMap_01::m_nNumMat		= NULL;		// マテリアル情報の数

CModel::MODEL_INFO		*CMap_01::m_ModelInfo	= NULL;		// モデルインフォ

//=============================================================================
// コンストラクタ										(public)	*** CMap_01 ***
//=============================================================================
CMap_01::CMap_01() : CMap()
{
	// 初期値設定
	m_pModel = NULL;	// モデル

	m_pField = NULL;	// フィールド

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		m_pSky[nCount] = NULL;							// NULLへ
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// 回転
	}

	m_pGrass = NULL;		// 草
}

//=============================================================================
// デストラクタ										(public)	*** CMap_01 ***
//=============================================================================
CMap_01::~CMap_01()
{

}

//=============================================================================
// ロード処理										(public)	*** CMap_01 ***
//=============================================================================
HRESULT CMap_01::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MAP\\titlemap.txt", &m_nMaxModel);		// モデル取得

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
// アンロード処理										(public)	*** CMap_01 ***
//=============================================================================
void CMap_01::Unload(void)
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
// 初期化処理										(public)	*** CMap_01 ***
//=============================================================================
HRESULT CMap_01::Init(void)
{
	// モデル
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULLの場合
		m_pModel = new CModel[m_nMaxModel];	// メモリ確保

		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
		{// モデル数カウント
		 // 初期値設定
			m_pModel[nCntModel].SetInitAll(m_pTexture[nCntModel], m_pMesh[nCntModel], m_pBuffMat[nCntModel], m_nNumMat[nCntModel], m_ModelInfo[nCntModel].pos, m_ModelInfo[nCntModel].rot);
			// 初期化処理
			m_pModel[nCntModel].Init();

			// 親モデルを設定
			if (0 <= m_ModelInfo[nCntModel].nIndex)		// 親以外
				m_pModel[nCntModel].SetParent(&m_pModel[m_ModelInfo[nCntModel].nIndex]);
		}
	}

	// メッシュフィールド
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
	//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	int nMaxVtx = m_pField->GetMaxVtx();	// 頂点数取得

	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{// 頂点カウント
		D3DXVECTOR3 pos = m_pField->GetVtx(nCntVtx);	// 頂点位置取得

		// 範囲計算
		float fRange = sqrtf(powf(pos.x, 2.0f) + powf(pos.z, 2.0f));

		if (1100.0f < fRange)
		{// 山１
			pos.y = 100.0f + (rand() % 10 * 15);
			m_pField->SetVtx(nCntVtx, pos);
		}
		else if (950.0f < fRange)
		{// 山２
			pos.y = 80.0f + (rand() % 10 * 10);
			m_pField->SetVtx(nCntVtx, pos);
		}
		else if (300.0f < fRange)
		{// 家周辺は０へ
			pos.y = 0.0f - (rand() % 15);
			m_pField->SetVtx(nCntVtx, pos);
		}

		if (170.0f < pos.x && pos.x < 690.0f && -160.0f < pos.z && pos.z < 160.0f)
		{// チュートリアル範囲
			pos.y = 0.0f - (rand() % 10);
			m_pField->SetVtx(nCntVtx, pos);
		}
		else if (70.0f < pos.x && pos.x < 790.0f && -260.0f < pos.z && pos.z < 260.0f)
		{// チュートリアル範囲の柵
			pos.y = 0.0f;
			m_pField->SetVtx(nCntVtx, pos);
		}
	}
	m_pField->SetVecNor();

	// 空１
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// 空２
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(true, false, false, false, true);
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	CScene		*pScene		= NULL;		// シーン
	CScene		*pSceneNext	= NULL;		// 次シーン
	CSceneMesh	*pMesh		= NULL;		// メッシュ
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

	// 草
	m_pGrass = new GRASS[m_GRASS_MAX];
	for (int nCntGrass = 0; nCntGrass < m_GRASS_MAX; nCntGrass++)
	{// 最大数カウント
		D3DXVECTOR3 pos;		// 位置

		bool bCheck = false;	// チェック
		do
		{// ループ
			bCheck = false;		// 初期案

			// 位置計算
			pos = D3DXVECTOR3(-250.0f + (float)(rand() % 900), 0.0f, 400.0f - (float)(rand() % 1000));

			// 範囲計算
			float fRange = sqrtf(powf(pos.x, 2.0f) + powf(pos.z, 2.0f));
			if (fRange < 150.0f)
			{// 範囲内だったら
				bCheck = true;	// チェック
			}
		} while (bCheck != false);

		// 初期値設定
		m_pGrass[nCntGrass].nCounter = 0;
		m_pGrass[nCntGrass].pos = pos;
		//m_pGrass[nCntGrass].pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
		m_pGrass[nCntGrass].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
		{// 草カウント
			// 大きさと角度
			D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 14.0f, 0.0f);
			D3DXVECTOR3 rot = m_pGrass[nCntGrass].rot + D3DXVECTOR3(0.0f, (D3DX_PI * 2.0f / (float)(m_GRASS_NUM * 2)) * nCount, 0.0f);

			// 距離と角度
			float fLenght = -5.0f + (float)(rand() % 10);
			float fAngle = -D3DX_PI + ((float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f);

			// 左の草
			D3DXVECTOR3 posLeft = m_pGrass[nCntGrass].pos +
				D3DXVECTOR3(sinf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f)), 7.0f, cosf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f))) +
				(D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght);

			for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
			{// カウント
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_GRASS);
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetInitAll(posLeft, rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Init();
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GRASS));
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->DrawEdit(false, false, true, false, true, 0, D3DCULL_NONE);

				if (pMesh != NULL)
				{// NULL以外の場合
					D3DXVECTOR3 pos = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
					pos.y = pMesh->GetHeight(pos);
					pos.y += 7.0f;
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Setpos(pos);
				}

				m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posLeft;

				D3DXVECTOR3 posVtx[4];
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);
				m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0] = posVtx[0];
				m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1] = posVtx[1];

				m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
				m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;

				posLeft += D3DXVECTOR3(sinf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f), 0.0f, cosf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f));
			}
		}
	}

	// 焚火
	CEffect::Create(CEffect::TYPE_FIRE, CEffect::SHOT_NONE, D3DXVECTOR3(0.0f, 2.0f, -30.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMap_01 ***
//=============================================================================
void CMap_01::Uninit(void)
{
	// モデル
	if (m_pModel != NULL)
	{// NULL以外の場合
		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++) // モデル数カウント
			m_pModel[nCntModel].Uninit();	// 終了処理

		delete[] m_pModel;		// メモリ開放
		m_pModel = NULL;		// NULLへ
	}

	// フィールド
	if (m_pField != NULL)
	{// NULL以外の場合
		m_pField->Uninit();		// 終了処理
		m_pField = NULL;		// NULLへ
	}

	// 空
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		if (m_pSky[nCount] != NULL)
		{// NULL以外の場合
			m_pSky[nCount]->Uninit();	// 終了処理
			m_pSky[nCount] = NULL;		// NULLへ
		}
	}

	// 草
	if (m_pGrass != NULL)
	{// NULL以外の場合
		for (int nCntGrass = 0; nCntGrass < m_GRASS_MAX; nCntGrass++)
		{
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// 草カウント
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Uninit();		// 終了処理
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = NULL;			// NULLへ
				}
			}
		}

		delete[] m_pGrass;	// メモリ開放
		m_pGrass = NULL;		// NULLへ
	}

	CMap::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMap_01 ***
//=============================================================================
void CMap_01::Update(void)
{
	// 空0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// 回転

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// テクスチャ設定
	}

	// 空1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// 回転

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// テクスチャ設定
	}

	CPlayer *pPlayer = CManager::GetTitle()->GetPlayer();
	if (pPlayer != NULL)
	{
		D3DXVECTOR3 posPlayer = pPlayer->Getpos();

		if (m_pGrass != NULL)
		{// NULL以外の場合
			for (int nCntGrass = 0; nCntGrass < m_GRASS_MAX; nCntGrass++)
			{// 最大数カウント
				for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
				{// 草カウント
					for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
					{
						D3DXVECTOR3 posVtx[4];		// 頂点受け取り
						D3DXVECTOR3 posCenter = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);		// 頂点情報取得

						float fRange;
						fRange = sqrtf(powf(posCenter.x - posPlayer.x, 2.0f) + powf(posCenter.z - posPlayer.z, 2.0f));
						if (fRange < 30.0f)
						{// 範囲内だったら
							float fAngle = atan2f(posCenter.x - posPlayer.x, posCenter.z - posPlayer.z);
							D3DXVECTOR3 posCenterNext = m_pGrass[nCntGrass].posCenter[nCount][nCntNum] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							fRange = sqrtf(powf(posCenter.x - posCenterNext.x, 2.0f) + powf(posCenter.z - posCenterNext.z, 2.0f));
							if (fRange < 20.0f)
							{
								posVtx[0] = posVtx[0] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
								posVtx[1] = posVtx[1] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
								m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenterNext;
							}

							m_pGrass[nCntGrass].bFall[nCount][nCntNum] = true;
						}
						else
						{// 範囲外
							if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == true)
							{// 倒れている
								posVtx[0].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].x - posVtx[0].x) * 0.1f;
								posVtx[0].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].z - posVtx[0].z) * 0.1f;
								posVtx[1].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].x - posVtx[1].x) * 0.1f;
								posVtx[1].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].z - posVtx[1].z) * 0.1f;
								m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum]++;

								if (m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] == 20)
								{
									m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
									m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenter;
									m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;
								}
							}
						}

						if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == false)
						{// 倒れていない
							if (m_pGrass->nCounter < 120)
							{// 一定カウントより低い
								posVtx[0].x += sinf(0.0f) * 0.023f;
								posVtx[0].x += cosf(0.0f) * 0.023f;
								posVtx[1].x += sinf(0.0f) * 0.023f;
								posVtx[1].x += cosf(0.0f) * 0.023f;
							}
							else
							{// 一定カウント以上
								posVtx[0].x -= sinf(0.0f) * 0.023f;
								posVtx[0].x -= cosf(0.0f) * 0.023f;
								posVtx[1].x -= sinf(0.0f) * 0.023f;
								posVtx[1].x -= cosf(0.0f) * 0.023f;
							}
						}
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetVtxSize(posVtx);		// 頂点情報設定
					}
				}

				{
					m_pGrass[nCntGrass].nCounter++;		// カウントする

					if (m_pGrass[nCntGrass].nCounter == 240)
					{// 指定したカウントになったら
						m_pGrass[nCntGrass].nCounter = 0;	// カウント初期化
					}
				}
			}
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CMap_01 ***
//=============================================================================
void CMap_01::Draw(void)
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
			//CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(0.0f, 0.1f, 0.0f));
		}
	}
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CMap_02::m_nMaxModel		= 0;		// モデル数

LPDIRECT3DTEXTURE9		**CMap_02::m_pTexture	= NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CMap_02::m_pMesh		= NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER			*CMap_02::m_pBuffMat	= NULL;		// マテリアル情報へのポインタ
DWORD					*CMap_02::m_nNumMat		= NULL;		// マテリアル情報の数

CModel::MODEL_INFO		*CMap_02::m_ModelInfo	= NULL;		// モデルインフォ

//=============================================================================
// コンストラクタ										(public)	*** CMap_02 ***
//=============================================================================
CMap_02::CMap_02() : CMap()
{
	m_nModelWarp = 0;									// モデルワープ数
	m_pModelWarp = NULL;								// モデルワープ

	m_nModelObject = 0;									// モデルオブジェクト数
	m_pModelObject = NULL;								// モデルオブジェクト

	m_pField = NULL;									// フィールド

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		m_pSky[nCount] = NULL;							// NULLへ
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// 回転
	}

	m_nGrassMax = 0;									// 最大草
	m_pGrass = NULL;									// 草

	m_nAreaMax = 0;										// エリア最大数

	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaNum[nCount] = 0;							// エリア数

	m_pArea = NULL;										// エリア

	m_nRouteMax = 0;									// ルート最大数
	m_pRoute = NULL;									// ルート

	m_nPatternMax = 0;									// パターン最大数
	m_pPattern = NULL;									// パターン

	m_pWarp = NULL;										// ワープ

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
		m_pWarpCylinder[nCount] = NULL;					// ワープシリンダー
	m_WarpCylinderUV = D3DXVECTOR2(0.0f, 0.0f);			// ワープシリンダーUV

	m_pGoalCylinder = NULL;								// ゴールシリンダー

	m_bDrawIcon = false;								// アイコン描画
	m_pIcon	= NULL;										// アイコン
}

//=============================================================================
// デストラクタ										(public)	*** CMap_02 ***
//=============================================================================
CMap_02::~CMap_02()
{

}

//=============================================================================
// ロード処理										(public)	*** CMap_02 ***
//=============================================================================
HRESULT CMap_02::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MAP\\gamemap.txt", &m_nMaxModel);		// モデル取得

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
// アンロード処理										(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::Unload(void)
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
// 初期化処理										(public)	*** CMap_02 ***
//=============================================================================
HRESULT CMap_02::Init(void)
{
	// メッシュフィールド
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3600.0f, 0.0f, 3600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(24.0f, 24.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	// 空１
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// 空２
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::Uninit(void)
{
	// モデル
	if (m_pModelWarp != NULL)
	{// NULL以外の場合
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++) // モデル数カウント
			m_pModelWarp[nCntModel].Uninit();	// 終了処理

		delete[] m_pModelWarp;		// メモリ開放
		m_pModelWarp = NULL;		// NULLへ
	}
	if (m_pModelObject != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++) // モデル数カウント
			m_pModelObject[nCntModel].Uninit();	// 終了処理

		delete[] m_pModelObject;	// メモリ開放
		m_pModelObject = NULL;		// NULLへ
	}

	// フィールド
	if (m_pField != NULL)
	{// NULL以外の場合
		m_pField->Uninit();		// 終了処理
		m_pField = NULL;		// NULLへ
	}

	// 空
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		if (m_pSky[nCount] != NULL)
		{// NULL以外の場合
			m_pSky[nCount]->Uninit();	// 終了処理
			m_pSky[nCount] = NULL;		// NULLへ
		}
	}

	// 草
	if (m_pGrass != NULL)
	{// NULL以外の場合
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// 草カウント
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Uninit();		// 終了処理
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = NULL;			// NULLへ
				}
			}
		}

		delete[] m_pGrass;	// メモリ開放
		m_pGrass = NULL;		// NULLへ
	}

	// マップ
	if (m_pArea != NULL)
	{// NULL以外の場合
		delete[] m_pArea;		// 破棄
		m_pArea = NULL;			// NULLへ
	}
	if (m_pRoute != NULL)
	{// NULL以外の場合
		delete[] m_pRoute;		// 破棄
		m_pRoute = NULL;		// NULLへ
	}
	if (m_pPattern != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nPatternMax; nCount++)
		{// エリアカウント
			delete[] m_pPattern[nCount].nPattern;
			m_pPattern[nCount].nPattern = NULL;
		}

		delete[] m_pPattern;	// 破棄
		m_pPattern = NULL;		// NULLへ
	}
	if (m_pWarp != NULL)
	{// NULL以外の場合
		delete[] m_pWarp;		// 破棄
		m_pWarp = NULL;			// NULLへ
	}

	// ワープ
	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
	{// ワープカウント
		if (m_pWarpCylinder[nCount] != NULL)
		{// NULL以外の場合
			m_pWarpCylinder[nCount]->Uninit();
			m_pWarpCylinder[nCount] = NULL;
		}
	}

	// ゴールシリンダー
	if (m_pGoalCylinder != NULL)
	{// NULL以外の場合
		m_pGoalCylinder->Uninit();
		m_pGoalCylinder = NULL;
	}

	// アイコン
	if (m_pIcon != NULL)
	{// NULL以外の場合
		m_pIcon->Uninit();
		m_pIcon = NULL;
	}

	CMap::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::Update(void)
{
	CSound *pSound = CManager::GetSound();

	// 空0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// 回転

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// テクスチャ設定
	}

	// 空1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// 回転

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// テクスチャ設定
	}

	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->Getpos();
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 CameraposR = pCamera->Getpos(1);
	D3DXVECTOR3 Camerarot = pCamera->Getrot();

	if (m_pGrass != NULL)
	{// NULL以外の場合
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{// 最大数カウント
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// 草カウント
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{// ラインカウント
					D3DXVECTOR3 posVtx[4];		// 頂点受け取り
					D3DXVECTOR3 posCenter = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);		// 頂点情報取得

					// カメラ範囲
					float fCameraL = Camerarot.y - (D3DX_PI * 0.5f);
					float fCameraR = Camerarot.y + (D3DX_PI * 0.5f);

					// 範囲調整
					if (fCameraL < -D3DX_PI)
						fCameraL += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraL)
						fCameraL -= D3DX_PI * 2.0f;

					if (fCameraR < -D3DX_PI)
						fCameraR += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraR)
						fCameraR -= D3DX_PI * 2.0f;

					float fCameraAngle = atan2f(posCenter.x - CameraposR.x, posCenter.z - CameraposR.z);

					if (Camerarot.y < (-D3DX_PI * 0.5f) || (D3DX_PI * 0.5f) < Camerarot.y)
					{
						if (fCameraR < fCameraAngle && fCameraAngle < fCameraL)
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}
					else
					{
						if (fCameraL < fCameraAngle && fCameraAngle < fCameraR)
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}

					float fRange;
					fRange = sqrtf(powf(posCenter.x - posPlayer.x, 2.0f) + powf(posCenter.z - posPlayer.z, 2.0f));					
					if (fRange < 850.0f)
					{// 範囲内
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
					}
					else
					{// 範囲外
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
					}

					if (fRange < 30.0f)
					{// 範囲内	
						float fAngle = atan2f(posCenter.x - posPlayer.x, posCenter.z - posPlayer.z);
						D3DXVECTOR3 posCenterNext = m_pGrass[nCntGrass].posCenter[nCount][nCntNum] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
						fRange = sqrtf(powf(posCenter.x - posCenterNext.x, 2.0f) + powf(posCenter.z - posCenterNext.z, 2.0f));
						if (fRange < 20.0f)
						{
							posVtx[0] = posVtx[0] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							posVtx[1] = posVtx[1] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenterNext;
						}

						m_pGrass[nCntGrass].bFall[nCount][nCntNum] = true;
					}
					else
					{// 範囲外
						if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == true)
						{// 倒れる
							posVtx[0].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].x - posVtx[0].x) * 0.1f;
							posVtx[0].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].z - posVtx[0].z) * 0.1f;
							posVtx[1].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].x - posVtx[1].x) * 0.1f;
							posVtx[1].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].z - posVtx[1].z) * 0.1f;
							m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum]++;

							if (m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] == 20)
							{
								m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
								m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenter;
								m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;
							}
						}
					}

					if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == false)
					{// 倒れていない
						if (m_pGrass->nCounter < 120)
						{// 一定カウントより低い
							posVtx[0].x += sinf(0.0f) * 0.023f;
							posVtx[0].x += cosf(0.0f) * 0.023f;
							posVtx[1].x += sinf(0.0f) * 0.023f;
							posVtx[1].x += cosf(0.0f) * 0.023f;
						}
						else
						{// 一定カウント以上
							posVtx[0].x -= sinf(0.0f) * 0.023f;
							posVtx[0].x -= cosf(0.0f) * 0.023f;
							posVtx[1].x -= sinf(0.0f) * 0.023f;
							posVtx[1].x -= cosf(0.0f) * 0.023f;
						}
					}
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetVtxSize(posVtx);		// 頂点情報設定
				}
			}

			{
				m_pGrass[nCntGrass].nCounter++;		// カウントする

				if (m_pGrass[nCntGrass].nCounter == 240)
				{// 一定のカウントになったら
					m_pGrass[nCntGrass].nCounter = 0;	// カウント初期化
				}
			}
		}
	}

	m_WarpCylinderUV.y -= 0.01f;
	for (int nCntWarp = 0; nCntWarp < m_nModelWarp; nCntWarp++)
	{// ワープモデルカウント
		if (m_pWarpCylinder[nCntWarp] != NULL)
		{// NULL以外の場合
			m_pWarpCylinder[nCntWarp]->SetTex(m_WarpCylinderUV);
		}
	}

	if (m_pIcon != NULL)
	{// NULL以外の場合
		D3DXCOLOR col = m_pIcon->Getcol();
		if (m_bDrawIcon == true)
		{// 表示
			if (col.a != 1.0f)
			{// 透明以外
				col.a += 0.05f;
				if (1.0f < col.a) col.a = 1.0f;
			}
		}
		else
		{// 非表示
			if (col.a != 0.0f)
			{// 不透明以外
				col.a -= 0.05f;
				if (col.a < 0.0f) col.a = 0.0f;
			}
		}
		m_pIcon->Setcol(col);
	}
	m_bDrawIcon = false;
}

//=============================================================================
// 描画処理											(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// 計算用マトリックス

	if (1 < m_nPatternMax)
	{
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
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

			if (m_pModelWarp != NULL)
			{// NULL以外の場合
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				m_pModelWarp[nCntModel].Draw();

				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
		}
	}

	for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
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

		if (m_pModelObject != NULL)
		{// NULL以外の場合
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pModelObject[nCntModel].Draw();

			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// 影表示
			//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
		}
	}
}

//=============================================================================
// フィールド設定処理									(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::SetField(D3DXVECTOR3 pos)
{
	// エリア数
	m_nAreaNum[AREA_START]	= 1;
	m_nAreaNum[AREA_END]		= 1;
	m_nAreaNum[AREA_NORMAL]	= rand() % 1 + 2;
	m_nAreaNum[AREA_BATTLE]	= rand() % 1 + 2;
	m_nAreaNum[AREA_ITEM]	= 1;

	// エリア総数計算
	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaMax += m_nAreaNum[nCount];

	// メモリ確保
	if (m_pArea == NULL) m_pArea = new AREA_RANGE[m_nAreaMax];

	// エリア初期化
	for (int nCount = 0; nCount < m_nAreaMax; nCount++)
	{// エリアカウント
		m_pArea[nCount].nArea	= AREA_START;						// エリア
		m_pArea[nCount].fRange	= 0.0f;								// 範囲
		m_pArea[nCount].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心
	}

	// エリア、ルート生成
	{
		// エリア
		{
			int nCntArea = 0;		// エリア番号
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
			{// エリアカウント
				for (int nCntNum = 0; nCntNum < m_nAreaNum[nCount]; nCntNum++)
				{// エリア数カウント
					int nCheckCounter = 0;	// チェックカウント
					bool bCheck = false;	// チェック
					do
					{// ループ
						bCheck = false;

						switch (nCount)
						{// エリア
						case AREA_START:	// スタート
							m_pArea[nCntArea].nArea = AREA_START;		// エリア
							m_pArea[nCntArea].fRange = 280.0f;			// 範囲
							m_pArea[nCntArea].pos	= pos;				// 位置
							break;
						case AREA_END:		// エンド
							m_pArea[nCntArea].nArea = AREA_END;			// エリア
							m_pArea[nCntArea].fRange = 280.0f;			// 範囲

							// 位置
							{
								float fRange = 0.0f;	// 範囲
								do
								{// ループ
									m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
									m_pArea[nCntArea].pos.y = 0.0f;
									m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));

									// 中心からの距離を計算
									fRange = sqrtf(powf(m_pArea[0].pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(m_pArea[0].pos.z - m_pArea[nCntArea].pos.z, 2.0f));
								} while (fRange < 1000.0f);		// 一定距離以上だったら
							}
							break;
						case AREA_NORMAL:	// ノーマル
						case AREA_BATTLE:	// バトル
							// エリア
							if (nCount == AREA_NORMAL)	m_pArea[nCntArea].nArea = AREA_NORMAL;
							if (nCount == AREA_BATTLE)	m_pArea[nCntArea].nArea = AREA_BATTLE;
							m_pArea[nCntArea].fRange = (float)(rand() % 100 + 380);			// 範囲

							// 位置
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							break;
						case AREA_ITEM:		// アイテム
							m_pArea[nCntArea].nArea = AREA_ITEM;			//　エリア
							m_pArea[nCntArea].fRange = (float)(rand() % 50 + 320);			// 範囲

							// 位置
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							break;
						}

						// エリア同士重なっていないか
						for (int nCntCheck = 0; nCntCheck < nCntArea; nCntCheck++)
						{// エリアカウント
							// 中心からの位置計算
							float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - m_pArea[nCntCheck].pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - m_pArea[nCntCheck].pos.z, 2.0f));
							if (fRange < m_pArea[nCntArea].fRange + m_pArea[nCntCheck].fRange + 200.0f)
							{// エリアが重なっていないか
								bCheck = true;		// 重なっている
								nCheckCounter++;	// カウント
								break;
							}
						}

						if (nCheckCounter == 30) break;
					} while (bCheck != false);
					
					if (nCheckCounter != 30)
						nCntArea++;		// エリアカウント
				}
			}
			m_nAreaMax = nCntArea;
		}

		// ルート
		{
			// ルート数
			m_nRouteNum[AREA_START]	= m_nAreaNum[AREA_START];
			m_nRouteNum[AREA_END]	= m_nAreaNum[AREA_END];
			m_nRouteNum[AREA_NORMAL] = m_nAreaNum[AREA_NORMAL];
			m_nRouteNum[AREA_BATTLE] = m_nAreaNum[AREA_BATTLE];
			m_nRouteNum[AREA_ITEM]	= m_nAreaNum[AREA_ITEM];

			// ルート総数計算
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
				m_nRouteMax += m_nRouteNum[nCount];

			// メモリ確保
			if (m_pRoute == NULL) m_pRoute = new ROUTE_RANGE[m_nRouteMax];

			// ルート初期化
			for (int nCount = 0; nCount < m_nRouteMax; nCount++)
			{// ルートカウント
				m_pRoute[nCount].nAreaNumStart = 0;									// スタートエリア番号
				m_pRoute[nCount].nAreaNumEnd = 0;									// エンドエリア番号
				m_pRoute[nCount].posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// スタートエリア位置
				m_pRoute[nCount].posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// エンドエリア位置
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					m_pRoute[nCount].pos[nCntPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ルート頂点位置
				m_pRoute[nCount].fAngle = 0.0f;										// ルート角度
				m_pRoute[nCount].fLenght = 0.0f;										// ルート距離
				m_pRoute[nCount].fRange = 0.0f;										// ルート範囲（道幅）
			}

			int nRoute = 0;		// ルート番号
			for (int nCountArea = 0; nCountArea < m_nAreaMax; nCountArea++)
			{// エリアカウント
				int nAreaNum = nCountArea;		// エリア番号
				float fAreaLenght = 5000.0f;	// 距離

				for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
				{// エリアカウント
					if (nCountArea != nCntAreaCheck && 
						(nCountArea != AREA_START || (nCountArea == AREA_START && nCntAreaCheck != AREA_END)) &&
						(nCountArea != AREA_END || (nCountArea == AREA_END && nCntAreaCheck != AREA_START)))
					{// 同じエリアでない、スタートとエンドを繋がない
						// 中心からの位置計算
						float fRange = sqrtf(powf(m_pArea[nCountArea].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nCountArea].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
						if (fRange < fAreaLenght)
						{// 距離がより近かったら
							nAreaNum = nCntAreaCheck;	// 番号
							fAreaLenght = fRange;		// 距離
						}
					}
				}

				bool bCheck = false;	// チェック
				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ルートカウント
					if ((m_pRoute[nCntRoute].nAreaNumStart == nCountArea && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
						(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nCountArea))
					{// 同じルートがないか
						bCheck = true;	// ある
					}
				}

				if (bCheck == false)
				{// 生成する
					// 範囲（道幅）
					m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

					// 距離
					m_pRoute[nRoute].fLenght = fAreaLenght;

					// 角度
					m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nCountArea].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nCountArea].pos.z);

					// 位置
					m_pRoute[nRoute].nAreaNumStart	= nCountArea;				// スタート番号
					m_pRoute[nRoute].nAreaNumEnd	= nAreaNum;					// エンド番号
					m_pRoute[nRoute].posStart		= m_pArea[nCountArea].pos;	// スタート位置
					m_pRoute[nRoute].posEnd			= m_pArea[nAreaNum].pos;	// エンド位置

					// 中心を計算
					m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

					// 頂点計算
					m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

					// 判定用の頂点計算
					m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

					nRoute++;	// ルートカウント
					if (nRoute == m_nRouteMax) break;
				}
			}

			// 各エリアにルートが何本繋がったか
			int *nRouteCounter = new int[m_nAreaMax];
			for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				nRouteCounter[nCount] = 0;

			for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
			{// ルートカウント
				nRouteCounter[m_pRoute[nCntRoute].nAreaNumStart]++;
				nRouteCounter[m_pRoute[nCntRoute].nAreaNumEnd]++;
			}

			// 末端のエリアを計算
			int nRouteEndNum = 0;
			int *nRouteEnd = new int[m_nAreaMax];
			for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				nRouteEnd[nCount] = 0;

			// 繋がれたルート数
			int nRoute_0 = 0;	
			int nRoute_1 = 0;
			int nRoute_2 = 0;

			for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
			{// ルートカウント
				nRouteEnd[nRouteEndNum] = nCntRoute;	// 番号記憶
				nRouteEndNum++;							// カウント

				if (nRouteCounter[nCntRoute] == 0)
					nRoute_0++;									// ルートが０
				else if (nRouteCounter[nCntRoute] == 1)
					nRoute_1++;									// ルートが１
				else if (1 < nRouteCounter[nCntRoute])
					nRoute_2 += nRouteCounter[nCntRoute] - 1;	// ルートが２
			}

			// パターン数を計算
			int nRoutePattern = nRoute - nRoute_2 + nRoute_0;

			if (nRoute_0 != 0)
			{// ルート
				int nNumber = 0;
				for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
				{// ルートカウント
					if (nRouteCounter[nCntRoute] == 0)
					{// 繋がっていない
						nNumber = nCntRoute;
						break;
					}
				}

				int nAreaNum = nNumber;			// エリア番号
				float fAreaLenght = 5000.0f;	// 距離

				for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
				{// エリアカウント
					if (nNumber != nCntAreaCheck &&
						(nNumber != AREA_START || (nNumber == AREA_START && nCntAreaCheck != AREA_END)) &&
						(nNumber != AREA_END || (nNumber == AREA_END && nCntAreaCheck != AREA_START))) {// 同じエリアでない、スタートとエンドを繋がない
						// 中心からの位置計算
						float fRange = sqrtf(powf(m_pArea[nNumber].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nNumber].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
						if (fRange < fAreaLenght)
						{// 距離がより近かったら
							nAreaNum = nCntAreaCheck;	// 番号
							fAreaLenght = fRange;		// 距離
						}
					}
				}

				bool bCheck = false;	// チェック
				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ルートカウント
					if ((m_pRoute[nCntRoute].nAreaNumStart == nRouteEnd[nCntRoute] && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
						(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nRouteEnd[nCntRoute]))
					{// 同じルートがないか
						bCheck = true;	// ある
					}
				}

				if (bCheck == false)
				{// 生成する
					// 範囲（道幅）
					m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

					// 距離
					m_pRoute[nRoute].fLenght = fAreaLenght;

					// 角度
					m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nNumber].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nNumber].pos.z);

					// 位置
					m_pRoute[nRoute].nAreaNumStart = nNumber;
					m_pRoute[nRoute].nAreaNumEnd = nAreaNum;
					m_pRoute[nRoute].posStart = m_pArea[nNumber].pos;
					m_pRoute[nRoute].posEnd = m_pArea[nAreaNum].pos;

					// 中心を計算
					m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

					// 頂点計算
					m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

					// 判定用の頂点計算
					m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

					nRoute++;	// ルートカウント
					nRoutePattern--;
				}
			}

			m_nRouteMax = nRoute;	// ルート数カウント

			m_nPatternMax = nRoutePattern;		// パターン数を記憶

			// メモリ確保
			if (m_pPattern == NULL)
				m_pPattern = new ROUTE_PATTERN[m_nPatternMax];

			if (m_pPattern != NULL)
			{// NULL以外の場合
				for (int nCntPattert = 0; nCntPattert < m_nPatternMax; nCntPattert++)
				{// パターン数カウント
					// 初期値設定
					m_pPattern[nCntPattert].nMaxPattern = 0;
					m_pPattern[nCntPattert].nPattern = new int[m_nAreaMax];
					m_pPattern[nCntPattert].bStart = false;
					m_pPattern[nCntPattert].bEnd = false;

					if (nCntPattert == 0)
					{// カウントが０だったら
						m_pPattern[nCntPattert].nPattern[0] = m_pRoute[0].nAreaNumStart;
						m_pPattern[nCntPattert].nPattern[1] = m_pRoute[0].nAreaNumEnd;
						m_pPattern[nCntPattert].nMaxPattern = 2;
					}
					else
					{// カウントが０以外だったら
						int nNumber = 0;		// 番号確認
						bool bCheck = false;	// チェック
						// 同じ番号にならないよう調整
						do
						{// ループ
							bCheck = false;		// 初期化
							for (int nCntCheck = 0; nCntCheck < nCntPattert; nCntCheck++)
							{// パターンカウント
								for (int nCntArea = 0; nCntArea < m_pPattern[nCntCheck].nMaxPattern; nCntArea++)
								{// エリアカウント
									if (m_pPattern[nCntCheck].nPattern[nCntArea] == nNumber)
									{// 同じだったら
										nNumber++;		// カウント
										bCheck = true;	// チェック
										break;
									}
								}
								if (bCheck == true) break;
							}
						} while (bCheck != false);

						// 番号につながるルートを検索
						for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
						{// ルートカウント
							if (nNumber == m_pRoute[nCntRoute].nAreaNumStart || nNumber == m_pRoute[nCntRoute].nAreaNumEnd)
							{// 番号が同じ
								m_pPattern[nCntPattert].nPattern[0] = m_pRoute[nCntRoute].nAreaNumStart;
								m_pPattern[nCntPattert].nPattern[1] = m_pRoute[nCntRoute].nAreaNumEnd;
								m_pPattern[nCntPattert].nMaxPattern = 2;
								break;
							}
						}
					}

					for (int nCntPattertNum = 0; nCntPattertNum < m_pPattern[nCntPattert].nMaxPattern; nCntPattertNum++)
					{// パターンにあるエリア数カウント
						for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
						{// ルートカウント
							if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumStart)
							{// スタート番号と同じ
								for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
								{// パターンにあるエリア数カウント
									if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumEnd)
									{// エンド番号と同じ
										break;
									}

									if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
									{// 同じ番号がなかったら
										// 番号記憶
										m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumEnd;
										m_pPattern[nCntPattert].nMaxPattern++;	// パターンのエリア数を増やす
									}
								}
							}
							else if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumEnd)
							{// エンド番号と同じ
								for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
								{// パターンにあるエリア数カウント
									if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumStart)
									{// スタート番号と同じ
										break;
									}

									if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
									{// 同じ番号がなかったら
										// 番号記憶
										m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumStart;
										m_pPattern[nCntPattert].nMaxPattern++;	// パターンのエリア数を増やす
									}
								}
							}
						}
					}
				}
			}

			for (int nCntPattern = 0; nCntPattern < m_nPatternMax; nCntPattern++)
			{// パターン数カウント
				for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattern].nMaxPattern; nCntCheck++)
				{// パターンにあるエリア数カウント
					if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_START)
					{// スタート番号と同じ
						m_pPattern[nCntPattern].bStart = true;		// スタートがある
					}
					else if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_END)
					{// エンド番号と同じ
						m_pPattern[nCntPattern].bEnd = true;		// エンドがある
					}
				}
			}

			if (1 < m_nPatternMax)
			{// パターン数が１より多い
				// メモリ確保
				if (m_pWarp == NULL)
					m_pWarp = new WARP[m_nPatternMax - 1];

				if (m_pWarp != NULL)
				{// NULL以外の場合
					for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
					{// ワープカウント
						if (nCntWarp == 0)
						{// カウントが０だったら
							bool bCheck = false;	// チェック
							do
							{// ループ
								bCheck = false;		// 初期化
								m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// パターン設定

								do
								{// ループ
									m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// パターン設定
								} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// パターンが違うなら抜ける

								if (2 < m_nPatternMax)
								{// パターンが２より多い
									if ((m_pPattern[m_pWarp[nCntWarp].nPattern_1].bStart	== true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd	== true) ||
										(m_pPattern[m_pWarp[nCntWarp].nPattern_1].bEnd		== true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart	== true))
									{// スタートとエンドが結ばれた
										bCheck = true;		// チェック
									}
								}
							} while (bCheck != false);
						}
						else
						{// カウントが１以上だったら
							bool bCheck = false;	// チェック
							do
							{// ループ
								bCheck = false;		// 初期化
								m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// パターン設定

								for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
								{// ワープカウント
									if (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_1 ||
										m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_2)
									{// 同じ番号がある
										bCheck = true;		// チェック
										break;
									}
								}
							} while (bCheck != false);

							do
							{// ループ
								bCheck = false;		// チェック
								do
								{// ループ
									m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// パターン設定
								} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// パターンが違うなら抜ける

								if (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern == 2 &&
									(m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart == true || m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd == true))
								{// パターンのエリアすうが２ + （スタートがある、エンドがある）
									bCheck = true;	// チェック
								}
							} while (bCheck != false);
						}

						bool bCheck = false;	// チェック
						do
						{// ループ
							bCheck = false;		// 初期化
							do
							{// ループ
								m_pWarp[nCntWarp].nArea_1 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_1].nMaxPattern;		// エリア設定
							} while (m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_START ||
								m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_END);		// スタートとエンド以外だったら抜ける
							m_pWarp[nCntWarp].nArea_1 = m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1];		// エリア番号設定

							do
							{
								m_pWarp[nCntWarp].nArea_2 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern;		// エリア設定
							} while (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_START ||
								m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_END);		// スタートとエンド以外だったら抜ける
							m_pWarp[nCntWarp].nArea_2 = m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2];		// エリア番号設定

							for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
							{// ワープカウント
								if (m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_1 ||
									m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_2 ||
									m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_1 ||
									m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_2)
								{// エリアが同じ
									bCheck = true;		// チェック
									break;
								}
							}
						} while (bCheck != false);
					}
				}
			}

			delete[] nRouteCounter;	// 破棄
			delete[] nRouteEnd;		// 破棄
		}
	}

	int nMaxVtx = m_pField->GetMaxVtx();	// 頂点取得

	// 頂点位置設定
	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{
		D3DXVECTOR3 posVtx = m_pField->GetVtx(nCntVtx);

		/*if (posVtx.x < -1500.0f || 1500.0f < posVtx.x || posVtx.z < -1500.0f || 1500.0f < posVtx.z)
		{
		posVtx.y = 80.0f + (rand() % 80);

		m_pField->SetVtx(nCntVtx, posVtx);
		m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		}
		else*/
		{
			posVtx.y = 80.0f + (rand() % 80);			// 位置設定

			// 頂点、色設定
			m_pField->SetVtx(nCntVtx, posVtx);
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

			// エリア
			if (m_pArea != NULL)
			{// NULL以外の場合
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				{// エリアカウント
					// 中心からの位置計算
					float fRange = sqrtf(powf(m_pArea[nCount].pos.x - posVtx.x, 2.0f) + powf(m_pArea[nCount].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[nCount].fRange)
					{// 範囲内だったら
						posVtx.y = 0.0f - (rand() % 15);	// 位置設定

						// 頂点、色設定
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						//if (m_pArea[nCount].nArea == AREA_START || m_pArea[nCount].nArea == AREA_END)
						//	m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

						/*if (m_pWarp != NULL)
						{
							for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
							{
								if (m_pWarp[nCntWarp].nArea_1 == nCount || m_pWarp[nCntWarp].nArea_2 == nCount)
								{
									if (nCntWarp == 0)
										m_pField->SetCol(nCntVtx, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
									else
										m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

									break;
								}
							}
						}*/

						break;
					}					
				}
			}

			// ルート
			if (m_pRoute != NULL)
			{// NULL以外の場合
				for (int nCount = 0; nCount < m_nRouteMax; nCount++)
				{// ルートカウント
					D3DXVECTOR3 vecA, vecC;		// 計算用ベクトル（頂点同士、頂点とPOS）
					int nCheck = 0;				// チェック
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// 位置カウント
						// 外積計算
						vecA = m_pRoute[nCount].pos[nCntPos] - m_pRoute[nCount].pos[(nCntPos + 1) % 4];
						vecC = posVtx - m_pRoute[nCount].pos[(nCntPos + 1) % 4];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
						{// 範囲内か
							nCheck++;	// カウント
						}
					}

					if (nCheck == 4)
					{// 範囲内だったら
						posVtx.y = 0.0f - (rand() % 15);	// 位置設定

						// 頂点、色設定
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}

			// ワープ
			if (m_pWarp != NULL)
			{
				for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
				{
					float fRange;
					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_1].fRange * 0.2f)
					{// 範囲内だったら
						posVtx.y = 0.0f;		// 位置設定

						// 頂点設定
						m_pField->SetVtx(nCntVtx, posVtx);
					}

					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_2].fRange * 0.2f)
					{// 範囲内だったら
						posVtx.y = 0.0f;		// 位置設定

						// 頂点設定
						m_pField->SetVtx(nCntVtx, posVtx);
					}
				}
			}
		}
	}
	m_pField->SetVecNor();	// 法線計算
}

//=============================================================================
// オブジェクト設定処理								(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::SetObject(void)
{
	// ワープ
	if (1 < m_nPatternMax)
	{// ワープが存在する
		// ワープモデル計算
		m_nModelWarp = (m_nPatternMax - 1) * 2;

		// ワープモデル
		if (m_pModelWarp == NULL && m_ModelInfo != NULL)
		{// NULLの場合
			m_pModelWarp = new CModel[m_nModelWarp];	// メモリ確保

			for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
			{// モデル数カウント
				// 初期値設定
				m_pModelWarp[nCntModel].SetInitAll(m_pTexture[0], m_pMesh[0], m_pBuffMat[0], m_nNumMat[0], m_ModelInfo[0].pos, m_ModelInfo[0].rot);
				// 初期化処理
				m_pModelWarp[nCntModel].Init();

				// 親モデルを設定
				m_pModelWarp[nCntModel].SetParent(NULL);

				if (m_pWarpCylinder[nCntModel] == NULL)
				{// NULL以外の場合
					m_pWarpCylinder[nCntModel] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);

					// 初期値設定
					m_pWarpCylinder[nCntModel]->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), 
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 60.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 10.0f));
					// 初期化処理
					m_pWarpCylinder[nCntModel]->Init();
					m_pWarpCylinder[nCntModel]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
					m_pWarpCylinder[nCntModel]->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
				}
			}
		}

		for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
		{// パターンカウント
			if (m_pModelWarp != NULL)
			{// NULL以外の場合
				m_pModelWarp[0 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pModelWarp[1 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);
			}

			if (m_pWarpCylinder[0 + (nCntWarp * 2)] != NULL && m_pWarpCylinder[1 + (nCntWarp * 2)] != NULL)
			{// NULL以外の場合
				m_pWarpCylinder[0 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pWarpCylinder[1 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);

				if (nCntWarp == 0)
				{// １つ目
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
					}
				}
				else if (nCntWarp == 1)
				{// ２つ目
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
					}
				}
			}
		}
	}

	// ゴール
	if (m_pGoalCylinder == NULL)
	{// NULLの場合
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// エリアカウント
			if (nCntArea == AREA_END)
			{// エンド
				m_pGoalCylinder = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GOAL);

				m_pGoalCylinder->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), m_pArea[nCntArea].pos + D3DXVECTOR3(0.0f, -50.0f, 0.0f), 
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 800.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
				m_pGoalCylinder->Init();
				m_pGoalCylinder->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
				m_pGoalCylinder->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
			}
		}
	}

	// アイコン
	if (m_pIcon == NULL)
	{// NULL以外の場合
		m_pIcon = new CScene3D(CScene::PRIORITY_5, CScene::OBJTYPE_MAPICON);

		m_pIcon->SetInitAll(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.125f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		m_pIcon->Init();
		m_pIcon->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
		m_pIcon->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE, D3DCMP_ALWAYS);
		m_pIcon->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	CScene		*pScene		= NULL;		// シーン
	CScene		*pSceneNext	= NULL;		// 次シーン
	CSceneMesh	*pMesh		= NULL;		// メッシュ
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

	int nNumSet = 17;	// エリアセット数
	m_nGrassMax = m_nAreaMax * nNumSet;

	if (m_pGrass == NULL)
	{// NULLの場合
		m_pGrass = new GRASS[m_nGrassMax];

		if (m_pGrass != NULL)
		{// NULL以外の場合
			for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
			{// エリアカウント
				for (int nCntGrass = 0; nCntGrass < nNumSet; nCntGrass++)
				{// 草カウント
					D3DXVECTOR3 pos;		// 位置
					float fAngle = (float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)) * 0.01f;
					float fLenght = (float)(rand() % (int)(m_pArea[nCntArea].fRange - 150.0f) + 100.0f);
					// 位置計算
					pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght;

					int nNum = nCntArea * nNumSet + nCntGrass;

					// 初期値設定
					m_pGrass[nNum].nCounter = 0;
					m_pGrass[nNum].pos = pos;
					m_pGrass[nNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
					{// 草カウント
						// 大きさと角度
						D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 14.0f, 0.0f);
						D3DXVECTOR3 rot = m_pGrass[nNum].rot + D3DXVECTOR3(0.0f, (D3DX_PI * 2.0f / (float)(m_GRASS_NUM * 2)) * nCount, 0.0f);

						// 距離と角度
						float fLenght = -5.0f + (float)(rand() % 10);
						float fAngle = -D3DX_PI + ((float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f);

						// 左の草
						D3DXVECTOR3 posLeft = m_pGrass[nNum].pos +
							D3DXVECTOR3(sinf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f)), 7.0f, cosf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f))) +
							(D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght);

						for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
						{// カウント
							m_pGrass[nNum].pGrass[nCount][nCntNum] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_GRASS);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->SetInitAll(posLeft, rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->Init();
							m_pGrass[nNum].pGrass[nCount][nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GRASS));
							m_pGrass[nNum].pGrass[nCount][nCntNum]->DrawEdit(false, false, true, false, true, 0, D3DCULL_NONE);

							if (pMesh != NULL)
							{// NULL以外の場合
								D3DXVECTOR3 pos = m_pGrass[nNum].pGrass[nCount][nCntNum]->Getpos();
								pos.y = pMesh->GetHeight(pos);
								pos.y += 7.0f;
								m_pGrass[nNum].pGrass[nCount][nCntNum]->Setpos(pos);
							}

							m_pGrass[nNum].posCenter[nCount][nCntNum] = posLeft;

							D3DXVECTOR3 posVtx[4];
							m_pGrass[nNum].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);
							m_pGrass[nNum].posVtx[nCount][nCntNum][0] = posVtx[0];
							m_pGrass[nNum].posVtx[nCount][nCntNum][1] = posVtx[1];

							m_pGrass[nNum].bFall[nCount][nCntNum] = false;
							m_pGrass[nNum].nFallCounter[nCount][nCntNum] = 0;

							posLeft += D3DXVECTOR3(sinf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f), 0.0f, cosf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f));
						}
					}
				}
			}
		}
	}
	
	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// エリアカウント
		if (m_pArea[nCntArea].nArea != AREA_START)
		{
			// 敵
			int nNum = rand() % 3 + 3;
			for (int nCount = 0; nCount < nNum; nCount++)
			{
				float fLenght;
				D3DXVECTOR3 pos;

				fLenght = m_pArea[nCntArea].fRange * 0.5f;
				pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf((float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f))), 0.0f, cosf((float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)))) * fLenght;

				CEnemy::Create(CEnemy::TYPE_N_SLIME, pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			}
		}
	}
}

//=============================================================================
// フィールド衝突判定処理								(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange)
{
	bool bLand = false;
	bool bAreaCheck			= false;	// エリアチェック現在
	bool bAreaCheckOld		= false;	// エリアチェック前回
	bool bRouteCheck		= false;	// ルートチェック現在
	bool bRouteCheckOld		= false;	// ルートチェック前回
	int nAreaCheckNum		= 0;		// エリアチェック番号
	int nRouteCheckNum		= 0;		// ルートチェック番号

	bool bAreaIn			= false;	// エリアの中
	int nAreaInNum			= 0;		// エリアの中番号
	bool bRouteIn			= false;	// ルートの中
	int nRouteInNum			= 0;		// ルートの中番号

	// エリアチェック
	if (bAreaCheck == false || bAreaCheckOld == false)
	{// どちらもFALSE
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// エリアカウント
			// エリアチェック
			bAreaCheck		= false;
			bAreaCheckOld	= false;
			nAreaCheckNum	= nCntArea;

			float fRange;	// 範囲
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPosOld->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPosOld->z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// 範囲内か
				bAreaCheckOld	= true;			// 範囲内
				bAreaIn			= true;			// エリア内にいる
				nAreaInNum		= nCntArea;		// エリア内の番号
			}

			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (m_pArea[nCntArea].fRange * MC_AREACOLL_NOW <= fRange)
			{// 範囲外か
				bAreaCheck = true;		// 範囲内
			}

			// どちらもTRUEなら処理を抜ける
			if (bAreaCheck == true && bAreaCheckOld == true) break;
		}
	}

	if (bRouteCheck == false || bRouteCheckOld == false)
	{// どちらもFALSE
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ルートカウント
			// ルートチェック
			bRouteCheck		= false;
			bRouteCheckOld	= false;
			nRouteCheckNum	= nCntRoute;

			int nHit	= 0;	// 現在ヒット数
			int nHitOld = 0;	// 前回ヒット数

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// 位置カウント
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// 外積計算
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = *pPos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// 範囲内か
					nHit++;		// カウント
				}

				// 外積計算
				vecAOld = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecCOld = *pPosOld - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecAOld.z * vecCOld.x) - (vecAOld.x * vecCOld.z)) <= 0)
				{// 範囲内か
					nHitOld++;	// カウント
				}
			}

			if (nHit != 4)
			{// 範囲外か
				bRouteCheck = true;			// 範囲外
			}
			if (nHitOld == 4)
			{// 範囲内か
				bRouteCheckOld	= true;			// 範囲内
				bRouteIn		= true;			// ルート内にいる
				nRouteInNum		= nCntRoute;	// ルート内の番号
			}

			// どちらもTRUEなら処理を抜ける
			if (bRouteCheck == true && bRouteCheckOld == true) break;
		}
	}

	bool bArea = false;		// エリア判定

	// エリア判定
	if (bAreaCheck == true && bAreaCheckOld == true)
	{// どちらもTRUE
		float fRouteAngle		= 0.0f;		// ルート角度
		float fRouteAngleRange	= 0.0f;		// ルート角度範囲
		float fRouteAngleL		= 0.0f;		// ルート角度L
		float fRouteAngleR		= 0.0f;		// ルート角度R
		bool bRoute				= false;	// ルートにいる

		// エリア中心からプレイヤーの角度計算
		float fAngle = atan2f(pPos->x - m_pArea[nAreaCheckNum].pos.x, pPos->z - m_pArea[nAreaCheckNum].pos.z);

		if (bRouteIn == true)
		{// ルート内にいる
			D3DXVECTOR3 posLei;
			if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posStart)
			{// スタートと同じだったら
				posLei = m_pRoute[nRouteInNum].posCol[0];	// レイ用位置

				bool bCheck = false;	// チェック
				while (true)
				{// ループ
					// 
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{
						break;
					}
					else
					{
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle)) * 1.0f;
					}
				}
			}
			else if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posEnd)
			{// エンドと同じだったら
				posLei = m_pRoute[nRouteInNum].posCol[2];	// レイ用位置

				bool bCheck = false;	// チェック
				while (true)
				{// ループ
					// 中心からの位置計算
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{// 範囲外
						break;
					}
					else
					{// 範囲内
						// レイを進める
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle + D3DX_PI), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle + D3DX_PI)) * 1.0f;
					}
				}
			}

			// ルート角度計算
			fRouteAngle = atan2f(m_pRoute[nRouteInNum].posCenter.x - m_pArea[nAreaCheckNum].pos.x, m_pRoute[nRouteInNum].posCenter.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = atan2f(posLei.x - m_pArea[nAreaCheckNum].pos.x, posLei.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = fRouteAngle - fRouteAngleRange;
			fRouteAngleRange = fabsf(fRouteAngleRange);

			if (fRouteAngleRange < -D3DX_PI)
				fRouteAngleRange += D3DX_PI * 2.0f;
			if (D3DX_PI < fRouteAngleRange)
				fRouteAngleRange -= D3DX_PI * 2.0f;

			fRouteAngleRange = fabsf(fRouteAngleRange);

			fRouteAngleL = fRouteAngle - fRouteAngleRange;
			fRouteAngleR = fRouteAngle + fRouteAngleRange;

			if (fRouteAngleL < fAngle && fAngle < fRouteAngleR)
			{
				bRoute = true;
			}
			else if (fRouteAngleL < -D3DX_PI)
			{
				if (fRouteAngleL < fAngle + (-D3DX_PI * 2.0f) && fAngle + (-D3DX_PI * 2.0f)  < fRouteAngleR)
				{
					bRoute = true;
				}
			}
			else if (D3DX_PI < fRouteAngleR)
			{
				if (fRouteAngleL < fAngle + (D3DX_PI * 2.0f) && fAngle + (D3DX_PI * 2.0f)  < fRouteAngleR)
				{
					bRoute = true;
				}
			}
		}

		if (bRouteIn == false || (bRouteIn == true && bRoute == false))
		{// ルート内にいない、（ルート内にいる + ルート範囲外）
			pPos->x = m_pArea[nAreaCheckNum].pos.x + sinf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);
			pPos->z = m_pArea[nAreaCheckNum].pos.z + cosf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);

			pMove->x = 0.0f;
			pMove->z = 0.0f;
			bArea = true;	// エリア判定
			bLand = true;
		}
	}

	// ルート判定
	if (bArea == false && bAreaIn == false)
	{// エリア判定していない + エリアにいない
		if (bRouteCheck == true && bRouteCheckOld == true)
		{// どちらもTRUE
			// センター計算
			float fAngleCenter	= 0.0f;
			float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteCheckNum].posStart.x - m_pRoute[nRouteCheckNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteCheckNum].posStart.z - m_pRoute[nRouteCheckNum].pos[0].z, 2.0f));

			// 外積計算
			D3DXVECTOR3 vecALR, vecCLR;
			vecALR = m_pRoute[nRouteCheckNum].posStart - m_pRoute[nRouteCheckNum].posEnd;
			vecCLR = *pPos - m_pRoute[nRouteCheckNum].posEnd;

			// ルートの右側か左側か
			if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle - (D3DX_PI * 0.5f);	// 左側
			else
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle + (D3DX_PI * 0.5f);	// 右側

			// センター計算
			D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

			// センターへの角度計算
			float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

			int nHit = 0;	// ヒット数

			while (1)
			{
				nHit = 0;	// ヒット数
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
				{// 位置カウント
					// 外積計算
					D3DXVECTOR3 vecA, vecC;
					vecA = m_pRoute[nRouteCheckNum].posCol[nCntPos] - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
					vecC = *pPos - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

					if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
					{// 範囲内
						nHit++;
					}
				}

				if (nHit == 4)
				{// 範囲内
					bLand = true;
					break;		// 処理を抜ける
				}
				else
				{// 範囲外
					// 位置を更新（近づける）
					pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
					pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
				}
			}

			pMove->x = 0.0f;
			pMove->z = 0.0f;
		}
	}

	// その他の判定（抜けた場合）
	if (bArea == false && bAreaIn == false)
	{// エリア判定していない + エリアにいない
		if (bRouteIn == false)
		{// ルートにいない
			if (bRouteCheck == true && bRouteCheckOld == false)
			{// 現在が範囲外 + 前回が範囲外
				int nRouteNum = 0;		// ルート番号

				for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
				{// ルートカウント
					int nHit = 0;	// ヒット数

					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// 位置カウント
						// 外積計算
						D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
						vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// 範囲内
							nHit++;		// カウント
						}
					}

					if (nHit == 4)
					{// 範囲内
						nRouteNum = nCntRoute;		// 番号を記憶
						break;
					}
				}

				// センター計算
				float fAngleCenter	= 0.0f;
				float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteNum].posStart.x - m_pRoute[nRouteNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteNum].posStart.z - m_pRoute[nRouteNum].pos[0].z, 2.0f));

				// 外積計算
				D3DXVECTOR3 vecALR, vecCLR;
				vecALR = m_pRoute[nRouteNum].posStart - m_pRoute[nRouteNum].posEnd;
				vecCLR = *pPos - m_pRoute[nRouteNum].posEnd;

				// ルートの右側か左側か
				if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
					fAngleCenter = m_pRoute[nRouteNum].fAngle - (D3DX_PI * 0.5f);	// 左側
				else
					fAngleCenter = m_pRoute[nRouteNum].fAngle + (D3DX_PI * 0.5f);	// 右側

				// センター計算
				D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

				// センターへの角度計算
				float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

				int nHit = 0;	// ヒット数

				while (1)
				{
					nHit = 0;	// ヒット数
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// 位置カウント
						// 外積計算
						D3DXVECTOR3 vecA, vecC;
						vecA = m_pRoute[nRouteNum].posCol[nCntPos] - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// 範囲内
							nHit++;
						}
					}

					if (nHit == 4)
					{// 範囲内
						bLand = true;
						break;		// 処理を抜ける
					}
					else
					{// 範囲外
						// 位置を更新（近づける）
						pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
						pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
					}
				}

				pMove->x = 0.0f;
				pMove->z = 0.0f;
			}
		}
	}

	return bLand;
}

//=============================================================================
// エリアルート内にいるか判定処理						(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::CollisionInOut(D3DXVECTOR3 pos)
{
	bool bLand = false;

	if (bLand == false)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// エリアカウント
			float fRange;	// 範囲
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pos.z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// 範囲内か
				bLand = true;
				break;
			}
		}
	}

	if (bLand == false)
	{
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ルートカウント
			int nHit = 0;	// 現在ヒット数

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// 位置カウント
				D3DXVECTOR3 vecA, vecC;
				// 外積計算
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = pos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// 範囲内か
					nHit++;		// カウント
				}
			}

			if (nHit == 4)
			{// 範囲内か
				bLand = true;
				break;
			}
		}
	}

	return bLand;
}

//=============================================================================
// フィールドワープ衝突判定処理						(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp)
{
	int nWarpNow = 0;
	int nWarpNext = 0;
	bool bUseWarp = false;;

	for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
	{
		float fRange;
		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNow = m_pWarp[nCntWarp].nArea_1;
			nWarpNext = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}

		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNext = m_pWarp[nCntWarp].nArea_1;
			nWarpNow = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}
	}

	if (bUseWarp == true)
	{
		*pPosWarp = m_pArea[nWarpNext].pos;

		if (m_pIcon != NULL)
		{// NULL以外の場合
			m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nWarpNow].pos.x, 100.0f, m_pArea[nWarpNow].pos.z));
			m_bDrawIcon = true;			// アイコン描画
		}
	}

	return bUseWarp;
}

//=============================================================================
// フィールドゴール衝突判定処理						(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::CollisionGoal(D3DXVECTOR3 *pPos)
{
	bool bUseGoal = false;;

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{
		if (nCntArea == AREA_END)
		{
			float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (fRange < 35.0f)
			{
				bUseGoal = true;

				if (m_pIcon != NULL)
				{// NULL以外の場合
					m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nCntArea].pos.x, 100.0f, m_pArea[nCntArea].pos.z));
					m_bDrawIcon = true;			// アイコン描画
				}

				break;
			}
		}
	}

	return bUseGoal;
}

//=============================================================================
// フィールドカメラ衝突判定処理						(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	bool bAreaIn	= false;	// エリアの中
	int nAreaInNum	= 0;		// エリアの中番号
	bool bRouteIn	= false;	// ルートの中
	int nRouteInNum = 0;		// ルートの中番号
	bool bAreaInV	= false;	// 視点ルートの中
	bool bRouteInV	= false;	// 視点ルートの中

	// エリアチェック
	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// エリアカウント
		float fRange;	// 範囲
		fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - posR.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - posR.z, 2.0f));
		if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
		{// 範囲内か
			bAreaIn		= true;			// エリア内にいる
			nAreaInNum	= nCntArea;		// エリア内の番号
			break;
		}
	}

	// ルートチェック
	for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
	{// ルートカウント
		int nHit = 0;	// 現在ヒット数
		int nHitV = 0;	// 現在ヒット数

		for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
		{// 位置カウント
			D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
			// 外積計算
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posR - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// 範囲内か
				nHit++;		// カウント
			}

			// 外積計算
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posV - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// 範囲内か
				nHitV++;		// カウント
			}
		}

		if (nHit == 4)
		{// 範囲内か
			bRouteIn	= true;			// 範囲外
			nRouteInNum = nCntRoute;	// エリア内の番号

			if (nHitV == 4)
			{// 範囲内か
				bRouteInV = true;			// 範囲外
			}

			break;
		}


	}

	bool bArea = false;		// エリア判定

	// エリア判定
	if (bAreaIn == true)
	{// エリア内にいる
		// 距離を計算
		float fRange = sqrtf(powf(m_pArea[nAreaInNum].pos.x - posV.x, 2.0f) + powf(m_pArea[nAreaInNum].pos.z - posV.z, 2.0f));
		if (fRange < m_pArea[nAreaInNum].fRange)
		{// 範囲内
			return true;
		}
		else
		{// 範囲外
			if (bRouteInV == true)
			{// 視点がルート内にいる
				return true;
			}
			else
			{// 視点がルート内にいない
				return false;
			}
		}
	}

	// ルート判定
	if (bArea == false && bAreaIn == false)
	{// エリア判定していない + エリアにいない
		if (bRouteIn == true)
		{// ルート内にいる
			int nHit = 0;	// 現在ヒット数

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// 位置カウント
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// 外積計算
				vecA = m_pRoute[nRouteInNum].posCol[nCntPos] - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = posV - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// 範囲内か
					nHit++;		// カウント
				}
			}

			if (nHit == 4)
			{// 範囲内か
				return true;
			}
			else
			{// 範囲外
				return false;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CMap_03::m_nMaxModel		= 0;		// モデル数

LPDIRECT3DTEXTURE9		**CMap_03::m_pTexture	= NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CMap_03::m_pMesh		= NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER			*CMap_03::m_pBuffMat	= NULL;		// マテリアル情報へのポインタ
DWORD					*CMap_03::m_nNumMat		= NULL;		// マテリアル情報の数

CModel::MODEL_INFO		*CMap_03::m_ModelInfo	= NULL;		// モデルインフォ

//=============================================================================
// コンストラクタ										(public)	*** CMap_03 ***
//=============================================================================
CMap_03::CMap_03() : CMap()
{
	m_nModelWarp = 0;									// モデルワープ数
	m_pModelWarp = NULL;								// モデルワープ

	m_nModelObject = 0;									// モデルオブジェクト数
	m_pModelObject = NULL;								// モデルオブジェクト

	m_pField = NULL;									// フィールド

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		m_pSky[nCount] = NULL;							// NULLへ
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// 回転
	}

	m_nGrassMax = 0;									// 最大草
	m_pGrass = NULL;									// 草

	m_nAreaMax = 0;										// エリア最大数

	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaNum[nCount] = 0;							// エリア数

	m_pArea = NULL;										// エリア

	m_nRouteMax = 0;									// ルート最大数
	m_pRoute = NULL;									// ルート

	m_nPatternMax = 0;									// パターン最大数
	m_pPattern = NULL;									// パターン

	m_pWarp = NULL;										// ワープ

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
		m_pWarpCylinder[nCount] = NULL;					// ワープシリンダー
	m_WarpCylinderUV = D3DXVECTOR2(0.0f, 0.0f);			// ワープシリンダーUV

	m_pGoalCylinder = NULL;								// ゴールシリンダー

	m_bDrawIcon = false;								// アイコン描画
	m_pIcon = NULL;										// アイコン
}

//=============================================================================
// デストラクタ										(public)	*** CMap_03 ***
//=============================================================================
CMap_03::~CMap_03()
{

}

//=============================================================================
// ロード処理										(public)	*** CMap_03 ***
//=============================================================================
HRESULT CMap_03::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MAP\\gamemap.txt", &m_nMaxModel);		// モデル取得

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
// アンロード処理										(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::Unload(void)
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
// 初期化処理										(public)	*** CMap_03 ***
//=============================================================================
HRESULT CMap_03::Init(void)
{
	// メッシュフィールド
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3600.0f, 0.0f, 3600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(24.0f, 24.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	// 空１
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// 空２
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::Uninit(void)
{
	// モデル
	if (m_pModelWarp != NULL)
	{// NULL以外の場合
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++) // モデル数カウント
			m_pModelWarp[nCntModel].Uninit();	// 終了処理

		delete[] m_pModelWarp;		// メモリ開放
		m_pModelWarp = NULL;		// NULLへ
	}
	if (m_pModelObject != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++) // モデル数カウント
			m_pModelObject[nCntModel].Uninit();	// 終了処理

		delete[] m_pModelObject;	// メモリ開放
		m_pModelObject = NULL;		// NULLへ
	}

	// フィールド
	if (m_pField != NULL)
	{// NULL以外の場合
		m_pField->Uninit();		// 終了処理
		m_pField = NULL;		// NULLへ
	}

	// 空
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		if (m_pSky[nCount] != NULL)
		{// NULL以外の場合
			m_pSky[nCount]->Uninit();	// 終了処理
			m_pSky[nCount] = NULL;		// NULLへ
		}
	}

	// 草
	if (m_pGrass != NULL)
	{// NULL以外の場合
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// 草カウント
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Uninit();		// 終了処理
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = NULL;			// NULLへ
				}
			}
		}

		delete[] m_pGrass;	// メモリ開放
		m_pGrass = NULL;		// NULLへ
	}

	// マップ
	if (m_pArea != NULL)
	{// NULL以外の場合
		delete[] m_pArea;		// 破棄
		m_pArea = NULL;			// NULLへ
	}
	if (m_pRoute != NULL)
	{// NULL以外の場合
		delete[] m_pRoute;		// 破棄
		m_pRoute = NULL;		// NULLへ
	}
	if (m_pPattern != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nPatternMax; nCount++)
		{// エリアカウント
			delete[] m_pPattern[nCount].nPattern;
			m_pPattern[nCount].nPattern = NULL;
		}

		delete[] m_pPattern;	// 破棄
		m_pPattern = NULL;		// NULLへ
	}
	if (m_pWarp != NULL)
	{// NULL以外の場合
		delete[] m_pWarp;		// 破棄
		m_pWarp = NULL;			// NULLへ
	}

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
	{
		if (m_pWarpCylinder[nCount] != NULL)
		{
			m_pWarpCylinder[nCount]->Uninit();
			m_pWarpCylinder[nCount] = NULL;
		}
	}

	// ゴールシリンダー
	if (m_pGoalCylinder != NULL)
	{// NULL以外の場合
		m_pGoalCylinder->Uninit();
		m_pGoalCylinder = NULL;
	}

	// アイコン
	if (m_pIcon != NULL)
	{// NULL以外の場合
		m_pIcon->Uninit();
		m_pIcon = NULL;
	}

	CMap::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::Update(void)
{
	// 空0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// 回転

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// テクスチャ設定
	}

	// 空1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// 回転

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// テクスチャ設定
	}

	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->Getpos();
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 CameraposR = pCamera->Getpos(1);
	D3DXVECTOR3 Camerarot = pCamera->Getrot();

	if (m_pGrass != NULL)
	{// NULL以外の場合
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{// 最大数カウント
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// 草カウント
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{// ラインカウント
					D3DXVECTOR3 posVtx[4];		// 頂点受け取り
					D3DXVECTOR3 posCenter = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);		// 頂点情報取得

																							// カメラ範囲
					float fCameraL = Camerarot.y - (D3DX_PI * 0.5f);
					float fCameraR = Camerarot.y + (D3DX_PI * 0.5f);

					// 範囲調整
					if (fCameraL < -D3DX_PI)
						fCameraL += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraL)
						fCameraL -= D3DX_PI * 2.0f;

					if (fCameraR < -D3DX_PI)
						fCameraR += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraR)
						fCameraR -= D3DX_PI * 2.0f;

					float fCameraAngle = atan2f(posCenter.x - CameraposR.x, posCenter.z - CameraposR.z);

					if (Camerarot.y < (-D3DX_PI * 0.5f) || (D3DX_PI * 0.5f) < Camerarot.y)
					{
						if (fCameraR < fCameraAngle && fCameraAngle < fCameraL)
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}
					else
					{
						if (fCameraL < fCameraAngle && fCameraAngle < fCameraR)
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}

					float fRange;
					fRange = sqrtf(powf(posCenter.x - posPlayer.x, 2.0f) + powf(posCenter.z - posPlayer.z, 2.0f));
					if (fRange < 850.0f)
					{
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
					}
					else
					{
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
					}

					bool bCheck = false;
					if (fRange < 30.0f)
					{
						bCheck = true;
					}

					if (bCheck == true)
					{
						float fAngle = atan2f(posCenter.x - posPlayer.x, posCenter.z - posPlayer.z);
						D3DXVECTOR3 posCenterNext = m_pGrass[nCntGrass].posCenter[nCount][nCntNum] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
						fRange = sqrtf(powf(posCenter.x - posCenterNext.x, 2.0f) + powf(posCenter.z - posCenterNext.z, 2.0f));
						if (fRange < 20.0f)
						{
							posVtx[0] = posVtx[0] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							posVtx[1] = posVtx[1] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenterNext;
						}

						m_pGrass[nCntGrass].bFall[nCount][nCntNum] = true;
					}
					else
					{
						if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == true)
						{
							posVtx[0].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].x - posVtx[0].x) * 0.1f;
							posVtx[0].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].z - posVtx[0].z) * 0.1f;
							posVtx[1].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].x - posVtx[1].x) * 0.1f;
							posVtx[1].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].z - posVtx[1].z) * 0.1f;
							m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum]++;

							if (m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] == 20)
							{
								m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
								m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenter;
								m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;
							}
						}
					}

					if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == false)
					{
						if (m_pGrass->nCounter < 120)
						{// 一定カウントより低い
							posVtx[0].x += sinf(0.0f) * 0.023f;
							posVtx[0].x += cosf(0.0f) * 0.023f;
							posVtx[1].x += sinf(0.0f) * 0.023f;
							posVtx[1].x += cosf(0.0f) * 0.023f;
						}
						else
						{// 一定カウント以上
							posVtx[0].x -= sinf(0.0f) * 0.023f;
							posVtx[0].x -= cosf(0.0f) * 0.023f;
							posVtx[1].x -= sinf(0.0f) * 0.023f;
							posVtx[1].x -= cosf(0.0f) * 0.023f;
						}
					}
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetVtxSize(posVtx);		// 頂点情報設定
				}
			}

			{
				m_pGrass[nCntGrass].nCounter++;		// カウントする

				if (m_pGrass[nCntGrass].nCounter == 240)
				{// 指定したカウントになったら
					m_pGrass[nCntGrass].nCounter = 0;	// カウント初期化
				}
			}
		}
	}

	m_WarpCylinderUV.y -= 0.01f;
	for (int nCntWarp = 0; nCntWarp < m_nModelWarp; nCntWarp++)
	{
		if (m_pWarpCylinder[nCntWarp] != NULL)
		{
			m_pWarpCylinder[nCntWarp]->SetTex(m_WarpCylinderUV);
		}
	}

	if (m_pIcon != NULL)
	{
		D3DXCOLOR col = m_pIcon->Getcol();
		if (m_bDrawIcon == true)
		{
			if (col.a != 1.0f)
			{
				col.a += 0.05f;
				if (1.0f < col.a) col.a = 1.0f;
			}
		}
		else
		{
			if (col.a != 0.0f)
			{
				col.a -= 0.05f;
				if (col.a < 0.0f) col.a = 0.0f;
			}
		}
		m_pIcon->Setcol(col);
	}
	m_bDrawIcon = false;
}

//=============================================================================
// 描画処理											(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// 計算用マトリックス

	if (1 < m_nPatternMax)
	{
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
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

			if (m_pModelWarp != NULL)
			{// NULL以外の場合
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				m_pModelWarp[nCntModel].Draw();

				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

				// 影表示
				//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
			}
		}
	}

	for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
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

		if (m_pModelObject != NULL)
		{// NULL以外の場合
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pModelObject[nCntModel].Draw();

			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// 影表示
			//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
		}
	}
}

//=============================================================================
// フィールド設定処理									(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::SetField(D3DXVECTOR3 pos)
{
	// エリア数
	m_nAreaNum[AREA_START]	= 1;
	m_nAreaNum[AREA_END]	= 1;
	m_nAreaNum[AREA_NORMAL]	= 0;
	m_nAreaNum[AREA_BATTLE]	= 1;
	m_nAreaNum[AREA_ITEM]	= 0;

	// エリア総数計算
	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaMax += m_nAreaNum[nCount];

	// メモリ確保
	if (m_pArea == NULL) m_pArea = new AREA_RANGE[m_nAreaMax];

	// エリア初期化
	for (int nCount = 0; nCount < m_nAreaMax; nCount++)
	{// エリアカウント
		m_pArea[nCount].nArea	= AREA_START;						// エリア
		m_pArea[nCount].fRange	= 0.0f;								// 範囲
		m_pArea[nCount].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心
	}

	// エリア、ルート生成
	{
		// エリア
		{
			int nCntArea = 0;		// エリア番号
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
			{// エリアカウント
				for (int nCntNum = 0; nCntNum < m_nAreaNum[nCount]; nCntNum++)
				{// エリア数カウント
					int nCheckCounter = 0;	// チェックカウント
					bool bCheck = false;	// チェック
					do
					{// ループ
						bCheck = false;

						switch (nCount)
						{// エリア
						case AREA_START:	// スタート
							m_pArea[nCntArea].nArea = AREA_START;		// エリア
							m_pArea[nCntArea].fRange = 280.0f;			// 範囲
							m_pArea[nCntArea].pos	= pos;				// 位置
							break;
						case AREA_END:		// エンド
							m_pArea[nCntArea].nArea = AREA_END;			// エリア
							m_pArea[nCntArea].fRange = 280.0f;			// 範囲

							// 位置
							{
								bool bRange = false;
								do
								{// ループ
									m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
									m_pArea[nCntArea].pos.y = 0.0f;
									m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));

									// 中心からの距離を計算
									float fRange = sqrtf(powf(m_pArea[0].pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(m_pArea[0].pos.z - m_pArea[nCntArea].pos.z, 2.0f));

									if (fRange < 1000.0f)
									{
										bCheck = true;
									}

									if (-1000.0f < m_pArea[nCntArea].pos.x && m_pArea[nCntArea].pos.x < 1000.0 &&
										-1000.0f < m_pArea[nCntArea].pos.z && m_pArea[nCntArea].pos.z < 1000.0f)
									{
										bCheck = true;
									}
								} while (bRange != false);
							}
							break;
						case AREA_NORMAL:	// ノーマル
						case AREA_BATTLE:	// バトル
							// エリア
							if (nCount == AREA_NORMAL)	m_pArea[nCntArea].nArea = AREA_NORMAL;
							if (nCount == AREA_BATTLE)	m_pArea[nCntArea].nArea = AREA_BATTLE;
							m_pArea[nCntArea].fRange = (float)(rand() % 100 + 580);			// 範囲

							// 位置
							m_pArea[nCntArea].pos.x = (float)((rand() % ((900 - (int)m_pArea[nCntArea].fRange) * 2)) - (900 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((900 - (int)m_pArea[nCntArea].fRange) * 2)) - (900 - (int)m_pArea[nCntArea].fRange));
							break;
						case AREA_ITEM:		// アイテム
							m_pArea[nCntArea].nArea = AREA_ITEM;			//　エリア
							m_pArea[nCntArea].fRange = (float)(rand() % 50 + 320);			// 範囲

							// 位置
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							break;
						}

						// エリア同士重なっていないか
						for (int nCntCheck = 0; nCntCheck < nCntArea; nCntCheck++)
						{// エリアカウント
							// 中心からの位置計算
							float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - m_pArea[nCntCheck].pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - m_pArea[nCntCheck].pos.z, 2.0f));
							if (fRange < m_pArea[nCntArea].fRange + m_pArea[nCntCheck].fRange + 100.0f)
							{// エリアが重なっていないか
								bCheck = true;		// 重なっている
								nCheckCounter++;	// カウント
								break;
							}
						}

						if (nCheckCounter == 30) break;
					} while (bCheck != false);
					
					if (nCheckCounter != 30)
						nCntArea++;		// エリアカウント
				}
			}
			m_nAreaMax = nCntArea;
		}

		// ルート
		{
			// ルート数
			m_nRouteNum[AREA_START]	= m_nAreaNum[AREA_START];
			m_nRouteNum[AREA_END]	= m_nAreaNum[AREA_END];
			m_nRouteNum[AREA_NORMAL] = m_nAreaNum[AREA_NORMAL];
			m_nRouteNum[AREA_BATTLE] = m_nAreaNum[AREA_BATTLE];
			m_nRouteNum[AREA_ITEM]	= m_nAreaNum[AREA_ITEM];

			// ルート総数計算
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
				m_nRouteMax += m_nRouteNum[nCount];

			// メモリ確保
			if (m_pRoute == NULL) m_pRoute = new ROUTE_RANGE[m_nRouteMax];

			// ルート初期化
			for (int nCount = 0; nCount < m_nRouteMax; nCount++)
			{// ルートカウント
				m_pRoute[nCount].nAreaNumStart = 0;									// スタートエリア番号
				m_pRoute[nCount].nAreaNumEnd = 0;									// エンドエリア番号
				m_pRoute[nCount].posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// スタートエリア位置
				m_pRoute[nCount].posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// エンドエリア位置
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					m_pRoute[nCount].pos[nCntPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ルート頂点位置
				m_pRoute[nCount].fAngle = 0.0f;										// ルート角度
				m_pRoute[nCount].fLenght = 0.0f;										// ルート距離
				m_pRoute[nCount].fRange = 0.0f;										// ルート範囲（道幅）
			}

			int nRoute = 0;		// ルート番号
			for (int nCountArea = 0; nCountArea < m_nAreaMax; nCountArea++)
			{// エリアカウント
				int nAreaNum = nCountArea;		// エリア番号
				float fAreaLenght = 5000.0f;	// 距離

				for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
				{// エリアカウント
					if (nCountArea != nCntAreaCheck && 
						(nCountArea != AREA_START || (nCountArea == AREA_START && nCntAreaCheck != AREA_END)) &&
						(nCountArea != AREA_END || (nCountArea == AREA_END && nCntAreaCheck != AREA_START)))
					{// 同じエリアでない、スタートとエンドを繋がない
						// 中心からの位置計算
						float fRange = sqrtf(powf(m_pArea[nCountArea].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nCountArea].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
						if (fRange < fAreaLenght)
						{// 距離がより近かったら
							nAreaNum = nCntAreaCheck;	// 番号
							fAreaLenght = fRange;		// 距離
						}
					}
				}

				bool bCheck = false;	// チェック
				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ルートカウント
					if ((m_pRoute[nCntRoute].nAreaNumStart == nCountArea && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
						(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nCountArea))
					{// 同じルートがないか
						bCheck = true;	// ある
					}
				}

				if (bCheck == false)
				{// 生成する
					// 範囲（道幅）
					m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

					// 距離
					m_pRoute[nRoute].fLenght = fAreaLenght;

					// 角度
					m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nCountArea].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nCountArea].pos.z);

					// 位置
					m_pRoute[nRoute].nAreaNumStart	= nCountArea;				// スタート番号
					m_pRoute[nRoute].nAreaNumEnd	= nAreaNum;					// エンド番号
					m_pRoute[nRoute].posStart		= m_pArea[nCountArea].pos;	// スタート位置
					m_pRoute[nRoute].posEnd			= m_pArea[nAreaNum].pos;	// エンド位置

					// 中心を計算
					m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

					// 頂点計算
					m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

					// 判定用の頂点計算
					m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

					nRoute++;	// ルートカウント
					if (nRoute == m_nRouteMax) break;
				}
			}

			// 各エリアにルートが何本繋がったか
			int *nRouteCounter = new int[m_nAreaMax];
			for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				nRouteCounter[nCount] = 0;

			for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
			{// ルートカウント
				nRouteCounter[m_pRoute[nCntRoute].nAreaNumStart]++;
				nRouteCounter[m_pRoute[nCntRoute].nAreaNumEnd]++;
			}

			// 末端のエリアを計算
			int nRouteEndNum = 0;
			int *nRouteEnd = new int[m_nAreaMax];
			for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				nRouteEnd[nCount] = 0;

			// 繋がれたルート数
			int nRoute_0 = 0;	
			int nRoute_1 = 0;
			int nRoute_2 = 0;

			for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
			{// ルートカウント
				nRouteEnd[nRouteEndNum] = nCntRoute;	// 番号記憶
				nRouteEndNum++;							// カウント

				if (nRouteCounter[nCntRoute] == 0)
					nRoute_0++;									// ルートが０
				else if (nRouteCounter[nCntRoute] == 1)
					nRoute_1++;									// ルートが１
				else if (1 < nRouteCounter[nCntRoute])
					nRoute_2 += nRouteCounter[nCntRoute] - 1;	// ルートが２
			}

			// パターン数を計算
			int nRoutePattern = nRoute - nRoute_2 + nRoute_0;

			if (nRoute_0 != 0)
			{// ルート
				int nNumber = 0;
				for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
				{// ルートカウント
					if (nRouteCounter[nCntRoute] == 0)
					{// 繋がっていない
						nNumber = nCntRoute;
						break;
					}
				}

				int nAreaNum = nNumber;			// エリア番号
				float fAreaLenght = 5000.0f;	// 距離

				for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
				{// エリアカウント
					if (nNumber != nCntAreaCheck &&
						(nNumber != AREA_START || (nNumber == AREA_START && nCntAreaCheck != AREA_END)) &&
						(nNumber != AREA_END || (nNumber == AREA_END && nCntAreaCheck != AREA_START))) {// 同じエリアでない、スタートとエンドを繋がない
						// 中心からの位置計算
						float fRange = sqrtf(powf(m_pArea[nNumber].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nNumber].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
						if (fRange < fAreaLenght)
						{// 距離がより近かったら
							nAreaNum = nCntAreaCheck;	// 番号
							fAreaLenght = fRange;		// 距離
						}
					}
				}

				bool bCheck = false;	// チェック
				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ルートカウント
					if ((m_pRoute[nCntRoute].nAreaNumStart == nRouteEnd[nCntRoute] && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
						(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nRouteEnd[nCntRoute]))
					{// 同じルートがないか
						bCheck = true;	// ある
					}
				}

				if (bCheck == false)
				{// 生成する
					// 範囲（道幅）
					m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

					// 距離
					m_pRoute[nRoute].fLenght = fAreaLenght;

					// 角度
					m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nNumber].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nNumber].pos.z);

					// 位置
					m_pRoute[nRoute].nAreaNumStart = nNumber;
					m_pRoute[nRoute].nAreaNumEnd = nAreaNum;
					m_pRoute[nRoute].posStart = m_pArea[nNumber].pos;
					m_pRoute[nRoute].posEnd = m_pArea[nAreaNum].pos;

					// 中心を計算
					m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

					// 頂点計算
					m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

					// 判定用の頂点計算
					m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

					nRoute++;	// ルートカウント
					nRoutePattern--;
				}
			}

			m_nRouteMax = nRoute;	// ルート数カウント

			m_nPatternMax = nRoutePattern;		// パターン数を記憶

			// メモリ確保
			if (m_pPattern == NULL)
				m_pPattern = new ROUTE_PATTERN[m_nPatternMax];

			if (m_pPattern != NULL)
			{// NULL以外の場合
				for (int nCntPattert = 0; nCntPattert < m_nPatternMax; nCntPattert++)
				{// パターン数カウント
					// 初期値設定
					m_pPattern[nCntPattert].nMaxPattern = 0;
					m_pPattern[nCntPattert].nPattern = new int[m_nAreaMax];
					m_pPattern[nCntPattert].bStart = false;
					m_pPattern[nCntPattert].bEnd = false;

					if (nCntPattert == 0)
					{// カウントが０だったら
						m_pPattern[nCntPattert].nPattern[0] = m_pRoute[0].nAreaNumStart;
						m_pPattern[nCntPattert].nPattern[1] = m_pRoute[0].nAreaNumEnd;
						m_pPattern[nCntPattert].nMaxPattern = 2;
					}
					else
					{// カウントが０以外だったら
						int nNumber = 0;		// 番号確認
						bool bCheck = false;	// チェック
						// 同じ番号にならないよう調整
						do
						{// ループ
							bCheck = false;		// 初期化
							for (int nCntCheck = 0; nCntCheck < nCntPattert; nCntCheck++)
							{// パターンカウント
								for (int nCntArea = 0; nCntArea < m_pPattern[nCntCheck].nMaxPattern; nCntArea++)
								{// エリアカウント
									if (m_pPattern[nCntCheck].nPattern[nCntArea] == nNumber)
									{// 同じだったら
										nNumber++;		// カウント
										bCheck = true;	// チェック
										break;
									}
								}
								if (bCheck == true) break;
							}
						} while (bCheck != false);

						// 番号につながるルートを検索
						for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
						{// ルートカウント
							if (nNumber == m_pRoute[nCntRoute].nAreaNumStart || nNumber == m_pRoute[nCntRoute].nAreaNumEnd)
							{// 番号が同じ
								m_pPattern[nCntPattert].nPattern[0] = m_pRoute[nCntRoute].nAreaNumStart;
								m_pPattern[nCntPattert].nPattern[1] = m_pRoute[nCntRoute].nAreaNumEnd;
								m_pPattern[nCntPattert].nMaxPattern = 2;
								break;
							}
						}
					}

					for (int nCntPattertNum = 0; nCntPattertNum < m_pPattern[nCntPattert].nMaxPattern; nCntPattertNum++)
					{// パターンにあるエリア数カウント
						for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
						{// ルートカウント
							if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumStart)
							{// スタート番号と同じ
								for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
								{// パターンにあるエリア数カウント
									if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumEnd)
									{// エンド番号と同じ
										break;
									}

									if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
									{// 同じ番号がなかったら
										// 番号記憶
										m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumEnd;
										m_pPattern[nCntPattert].nMaxPattern++;	// パターンのエリア数を増やす
									}
								}
							}
							else if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumEnd)
							{// エンド番号と同じ
								for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
								{// パターンにあるエリア数カウント
									if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumStart)
									{// スタート番号と同じ
										break;
									}

									if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
									{// 同じ番号がなかったら
										// 番号記憶
										m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumStart;
										m_pPattern[nCntPattert].nMaxPattern++;	// パターンのエリア数を増やす
									}
								}
							}
						}
					}
				}
			}

			for (int nCntPattern = 0; nCntPattern < m_nPatternMax; nCntPattern++)
			{// パターン数カウント
				for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattern].nMaxPattern; nCntCheck++)
				{// パターンにあるエリア数カウント
					if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_START)
					{// スタート番号と同じ
						m_pPattern[nCntPattern].bStart = true;		// スタートがある
					}
					else if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_END)
					{// エンド番号と同じ
						m_pPattern[nCntPattern].bEnd = true;		// エンドがある
					}
				}
			}

			if (1 < m_nPatternMax)
			{// パターン数が１より多い
				// メモリ確保
				if (m_pWarp == NULL)
					m_pWarp = new WARP[m_nPatternMax - 1];

				if (m_pWarp != NULL)
				{// NULL以外の場合
					for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
					{// ワープカウント
						if (nCntWarp == 0)
						{// カウントが０だったら
							bool bCheck = false;	// チェック
							do
							{// ループ
								bCheck = false;		// 初期化
								m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// パターン設定

								do
								{// ループ
									m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// パターン設定
								} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// パターンが違うなら抜ける

								if (2 < m_nPatternMax)
								{// パターンが２より多い
									if ((m_pPattern[m_pWarp[nCntWarp].nPattern_1].bStart	== true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd	== true) ||
										(m_pPattern[m_pWarp[nCntWarp].nPattern_1].bEnd		== true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart	== true))
									{// スタートとエンドが結ばれた
										bCheck = true;		// チェック
									}
								}
							} while (bCheck != false);
						}
						else
						{// カウントが１以上だったら
							bool bCheck = false;	// チェック
							do
							{// ループ
								bCheck = false;		// 初期化
								m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// パターン設定

								for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
								{// ワープカウント
									if (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_1 ||
										m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_2)
									{// 同じ番号がある
										bCheck = true;		// チェック
										break;
									}
								}
							} while (bCheck != false);

							do
							{// ループ
								bCheck = false;		// チェック
								do
								{// ループ
									m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// パターン設定
								} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// パターンが違うなら抜ける

								if (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern == 2 &&
									(m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart == true || m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd == true))
								{// パターンのエリアすうが２ + （スタートがある、エンドがある）
									bCheck = true;	// チェック
								}
							} while (bCheck != false);
						}

						bool bCheck = false;	// チェック
						do
						{// ループ
							bCheck = false;		// 初期化
							do
							{// ループ
								m_pWarp[nCntWarp].nArea_1 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_1].nMaxPattern;		// エリア設定
							} while (m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_START ||
								m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_END);		// スタートとエンド以外だったら抜ける
							m_pWarp[nCntWarp].nArea_1 = m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1];		// エリア番号設定

							do
							{
								m_pWarp[nCntWarp].nArea_2 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern;		// エリア設定
							} while (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_START ||
								m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_END);		// スタートとエンド以外だったら抜ける
							m_pWarp[nCntWarp].nArea_2 = m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2];		// エリア番号設定

							for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
							{// ワープカウント
								if (m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_1 ||
									m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_2 ||
									m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_1 ||
									m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_2)
								{// エリアが同じ
									bCheck = true;		// チェック
									break;
								}
							}
						} while (bCheck != false);
					}
				}
			}

			delete[] nRouteCounter;	// 破棄
			delete[] nRouteEnd;		// 破棄
		}
	}

	int nMaxVtx = m_pField->GetMaxVtx();	// 頂点取得

	// 頂点位置設定
	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{
		D3DXVECTOR3 posVtx = m_pField->GetVtx(nCntVtx);

		/*if (posVtx.x < -1500.0f || 1500.0f < posVtx.x || posVtx.z < -1500.0f || 1500.0f < posVtx.z)
		{
		posVtx.y = 80.0f + (rand() % 80);

		m_pField->SetVtx(nCntVtx, posVtx);
		m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		}
		else*/
		{
			posVtx.y = 80.0f + (rand() % 80);			// 位置設定

			// 頂点、色設定
			m_pField->SetVtx(nCntVtx, posVtx);
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

			// エリア
			if (m_pArea != NULL)
			{// NULL以外の場合
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				{// エリアカウント
					// 中心からの位置計算
					float fRange = sqrtf(powf(m_pArea[nCount].pos.x - posVtx.x, 2.0f) + powf(m_pArea[nCount].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[nCount].fRange)
					{// 範囲内だったら
						posVtx.y = 0.0f - (rand() % 15);	// 位置設定

						// 頂点、色設定
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						//if (m_pArea[nCount].nArea == AREA_START || m_pArea[nCount].nArea == AREA_END)
						//	m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

						/*if (m_pWarp != NULL)
						{
							for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
							{
								if (m_pWarp[nCntWarp].nArea_1 == nCount || m_pWarp[nCntWarp].nArea_2 == nCount)
								{
									if (nCntWarp == 0)
										m_pField->SetCol(nCntVtx, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
									else
										m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

									break;
								}
							}
						}*/

						break;
					}					
				}
			}

			// ルート
			if (m_pRoute != NULL)
			{// NULL以外の場合
				for (int nCount = 0; nCount < m_nRouteMax; nCount++)
				{// ルートカウント
					D3DXVECTOR3 vecA, vecC;		// 計算用ベクトル（頂点同士、頂点とPOS）
					int nCheck = 0;				// チェック
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// 位置カウント
						// 外積計算
						vecA = m_pRoute[nCount].pos[nCntPos] - m_pRoute[nCount].pos[(nCntPos + 1) % 4];
						vecC = posVtx - m_pRoute[nCount].pos[(nCntPos + 1) % 4];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
						{// 範囲内か
							nCheck++;	// カウント
						}
					}

					if (nCheck == 4)
					{// 範囲内だったら
						posVtx.y = 0.0f - (rand() % 15);	// 位置設定

						// 頂点、色設定
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}

			// ワープ
			if (m_pWarp != NULL)
			{
				for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
				{
					float fRange;
					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_1].fRange * 0.2f)
					{// 範囲内だったら
						posVtx.y = 0.0f;		// 位置設定

						// 頂点設定
						m_pField->SetVtx(nCntVtx, posVtx);
					}

					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_2].fRange * 0.2f)
					{// 範囲内だったら
						posVtx.y = 0.0f;		// 位置設定

						// 頂点設定
						m_pField->SetVtx(nCntVtx, posVtx);
					}
				}
			}
		}
	}
	m_pField->SetVecNor();	// 法線計算
}

//=============================================================================
// オブジェクト設定処理								(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::SetObject(void)
{
	if (1 < m_nPatternMax)
	{// ワープが存在する
		// ワープモデル計算
		m_nModelWarp = (m_nPatternMax - 1) * 2;

		// ワープモデル
		if (m_pModelWarp == NULL && m_ModelInfo != NULL)
		{// NULLの場合
			m_pModelWarp = new CModel[m_nModelWarp];	// メモリ確保

			for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
			{// モデル数カウント
				// 初期値設定
				m_pModelWarp[nCntModel].SetInitAll(m_pTexture[0], m_pMesh[0], m_pBuffMat[0], m_nNumMat[0], m_ModelInfo[0].pos, m_ModelInfo[0].rot);
				// 初期化処理
				m_pModelWarp[nCntModel].Init();

				// 親モデルを設定
				m_pModelWarp[nCntModel].SetParent(NULL);

				if (m_pWarpCylinder[nCntModel] == NULL)
				{// NULL以外の場合
					m_pWarpCylinder[nCntModel] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);

					// 初期値設定
					m_pWarpCylinder[nCntModel]->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), 
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 60.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 10.0f));
					// 初期化処理
					m_pWarpCylinder[nCntModel]->Init();
					m_pWarpCylinder[nCntModel]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
					m_pWarpCylinder[nCntModel]->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
				}
			}
		}

		for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
		{// パターンカウント
			if (m_pModelWarp != NULL)
			{// NULL以外の場合
				m_pModelWarp[0 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pModelWarp[1 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);
			}

			if (m_pWarpCylinder[0 + (nCntWarp * 2)] != NULL && m_pWarpCylinder[1 + (nCntWarp * 2)] != NULL)
			{// NULL以外の場合
				m_pWarpCylinder[0 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pWarpCylinder[1 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);

				if (nCntWarp == 0)
				{
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
					}
				}
				else if (nCntWarp == 1)
				{
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
					}
				}
			}
		}
	}

	if (m_pGoalCylinder == NULL)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{
			if (nCntArea == AREA_END)
			{
				m_pGoalCylinder = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GOAL);

				m_pGoalCylinder->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), m_pArea[nCntArea].pos + D3DXVECTOR3(0.0f, -50.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 800.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(1.0f, 1.0f));
				m_pGoalCylinder->Init();
				m_pGoalCylinder->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
				m_pGoalCylinder->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
			}
		}
	}

	if (m_pIcon == NULL)
	{// NULL以外の場合
		m_pIcon = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_MAPICON);

		m_pIcon->SetInitAll(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.125f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		m_pIcon->Init();
		m_pIcon->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
		m_pIcon->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE, D3DCMP_NOTEQUAL);
		m_pIcon->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	CScene		*pScene		= NULL;		// シーン
	CScene		*pSceneNext	= NULL;		// 次シーン
	CSceneMesh	*pMesh		= NULL;		// メッシュ
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

	int nNumSet = 20;
	m_nGrassMax = m_nAreaMax * nNumSet;

	if (m_pGrass == NULL)
	{
		m_pGrass = new GRASS[m_nGrassMax];

		if (m_pGrass != NULL)
		{
			for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
			{
				for (int nCntGrass = 0; nCntGrass < nNumSet; nCntGrass++)
				{
					D3DXVECTOR3 pos;		// 位置
					float fAngle = (float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)) * 0.01f;
					float fLenght = (float)(rand() % (int)(m_pArea[nCntArea].fRange - 150.0f) + 100.0f);
					// 位置計算
					pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght;

					int nNum = nCntArea * nNumSet + nCntGrass;

					// 初期値設定
					m_pGrass[nNum].nCounter = 0;
					m_pGrass[nNum].pos = pos;
					m_pGrass[nNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
					{// 草カウント
						D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 14.0f, 0.0f);
						D3DXVECTOR3 rot = m_pGrass[nNum].rot + D3DXVECTOR3(0.0f, (D3DX_PI * 2.0f / (float)(m_GRASS_NUM * 2)) * nCount, 0.0f);

						float fAngle = -D3DX_PI + ((float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f);
						float fLenght = -5.0f + (float)(rand() % 10);

						D3DXVECTOR3 posLeft = m_pGrass[nNum].pos +
							D3DXVECTOR3(sinf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f)), 7.0f, cosf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f))) +
							(D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght);

						for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
						{
							m_pGrass[nNum].pGrass[nCount][nCntNum] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_GRASS);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->SetInitAll(posLeft, rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->Init();
							m_pGrass[nNum].pGrass[nCount][nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GRASS));
							m_pGrass[nNum].pGrass[nCount][nCntNum]->DrawEdit(false, false, true, false, true, 0, D3DCULL_NONE);

							if (pMesh != NULL)
							{
								D3DXVECTOR3 pos = m_pGrass[nNum].pGrass[nCount][nCntNum]->Getpos();
								pos.y = pMesh->GetHeight(pos);
								pos.y += 7.0f;
								m_pGrass[nNum].pGrass[nCount][nCntNum]->Setpos(pos);
							}

							m_pGrass[nNum].posCenter[nCount][nCntNum] = posLeft;

							D3DXVECTOR3 posVtx[4];
							m_pGrass[nNum].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);
							m_pGrass[nNum].posVtx[nCount][nCntNum][0] = posVtx[0];
							m_pGrass[nNum].posVtx[nCount][nCntNum][1] = posVtx[1];

							m_pGrass[nNum].bFall[nCount][nCntNum] = false;
							m_pGrass[nNum].nFallCounter[nCount][nCntNum] = 0;

							posLeft += D3DXVECTOR3(sinf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f), 0.0f, cosf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f));
						}
					}
				}
			}
		}
	}

	/*if (m_pModelObject == NULL)
	{
		m_nModelObject = 500;
		m_pModelObject = new CModel[m_nModelObject];

		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
		{
			// 初期値設定
			m_pModelObject[nCntModel].SetInitAll(m_pTexture[1], m_pMesh[1], m_pBuffMat[1], m_nNumMat[1], m_ModelInfo[1].pos, m_ModelInfo[1].rot);
			// 初期化処理
			m_pModelObject[nCntModel].Init();

			// 親モデルを設定
			m_pModelObject[nCntModel].SetParent(NULL);

			D3DXVECTOR3 pos;
			bool bCheck = false;
			do
			{
				bCheck = false;

				pos.x = -2000.0f + (float)(rand() % (2000 * 2));
				pos.y = 0.0f;
				pos.z = -2000.0f + (float)(rand() % (2000 * 2));

				if (bCheck == false)
				{
					for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
					{
						float fRange = sqrtf(powf(pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(pos.z - m_pArea[nCntArea].pos.z, 2.0f));
						if (fRange < m_pArea[nCntArea].fRange)
						{
							bCheck = true;
							break;
						}
					}
				}
				if (bCheck == false)
				{
					for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
					{
						int nCheck = 0;
						for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
						{
							D3DXVECTOR3 vecA, vecC;
							vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % 4];
							vecC = pos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % 4];

							if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
							{// 範囲内か
								nCheck++;	// カウント
							}
						}

						if (nCheck == 4)
						{// 範囲内だったら
							bCheck = true;
							break;
						}
					}
				}
			} while (bCheck != false);

			if (pMesh != NULL)
			{
				pos.y = pMesh->GetHeight(pos);
			}

			m_pModelObject[nCntModel].Setpos(pos);
		}
	}*/

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// エリアカウント
		if (m_pArea[nCntArea].nArea != AREA_START)
		{
			// 敵
//			CEnemy::Create(CEnemy::TYPE_N_SLIME, m_pArea[nCntArea].pos + D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
//			break;

			int nNum = rand() % 3 + 3;
			for (int nCount = 0; nCount < nNum; nCount++)
			{
				float fLenght;
				D3DXVECTOR3 pos;

				fLenght = m_pArea[nCntArea].fRange * 0.5f;
				pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf((float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f))), 0.0f, cosf((float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)))) * fLenght;

				CEnemy::Create(CEnemy::TYPE_N_SLIME, pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			}
		}
	}
}

//=============================================================================
// フィールド衝突判定処理								(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange)
{
	bool bLand = false;
	bool bAreaCheck			= false;	// エリアチェック現在
	bool bAreaCheckOld		= false;	// エリアチェック前回
	bool bRouteCheck		= false;	// ルートチェック現在
	bool bRouteCheckOld		= false;	// ルートチェック前回
	int nAreaCheckNum		= 0;		// エリアチェック番号
	int nRouteCheckNum		= 0;		// ルートチェック番号

	bool bAreaIn			= false;	// エリアの中
	int nAreaInNum			= 0;		// エリアの中番号
	bool bRouteIn			= false;	// ルートの中
	int nRouteInNum			= 0;		// ルートの中番号

	// エリアチェック
	if (bAreaCheck == false || bAreaCheckOld == false)
	{// どちらもFALSE
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// エリアカウント
			// エリアチェック
			bAreaCheck		= false;
			bAreaCheckOld	= false;
			nAreaCheckNum	= nCntArea;

			float fRange;	// 範囲
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPosOld->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPosOld->z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// 範囲内か
				bAreaCheckOld	= true;			// 範囲内
				bAreaIn			= true;			// エリア内にいる
				nAreaInNum		= nCntArea;		// エリア内の番号
			}

			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (m_pArea[nCntArea].fRange * MC_AREACOLL_NOW <= fRange)
			{// 範囲外か
				bAreaCheck = true;		// 範囲内
			}

			// どちらもTRUEなら処理を抜ける
			if (bAreaCheck == true && bAreaCheckOld == true) break;
		}
	}

	if (bRouteCheck == false || bRouteCheckOld == false)
	{// どちらもFALSE
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ルートカウント
			// ルートチェック
			bRouteCheck		= false;
			bRouteCheckOld	= false;
			nRouteCheckNum	= nCntRoute;

			int nHit	= 0;	// 現在ヒット数
			int nHitOld = 0;	// 前回ヒット数

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// 位置カウント
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// 外積計算
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = *pPos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// 範囲内か
					nHit++;		// カウント
				}

				// 外積計算
				vecAOld = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecCOld = *pPosOld - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecAOld.z * vecCOld.x) - (vecAOld.x * vecCOld.z)) <= 0)
				{// 範囲内か
					nHitOld++;	// カウント
				}
			}

			if (nHit != 4)
			{// 範囲外か
				bRouteCheck = true;			// 範囲外
			}
			if (nHitOld == 4)
			{// 範囲内か
				bRouteCheckOld	= true;			// 範囲内
				bRouteIn		= true;			// ルート内にいる
				nRouteInNum		= nCntRoute;	// ルート内の番号
			}

			// どちらもTRUEなら処理を抜ける
			if (bRouteCheck == true && bRouteCheckOld == true) break;
		}
	}

	bool bArea = false;		// エリア判定

	// エリア判定
	if (bAreaCheck == true && bAreaCheckOld == true)
	{// どちらもTRUE
		float fRouteAngle		= 0.0f;		// ルート角度
		float fRouteAngleRange	= 0.0f;		// ルート角度範囲
		float fRouteAngleL		= 0.0f;		// ルート角度L
		float fRouteAngleR		= 0.0f;		// ルート角度R
		bool bRoute				= false;	// ルートにいる

		// エリア中心からプレイヤーの角度計算
		float fAngle = atan2f(pPos->x - m_pArea[nAreaCheckNum].pos.x, pPos->z - m_pArea[nAreaCheckNum].pos.z);

		if (bRouteIn == true)
		{// ルート内にいる
			D3DXVECTOR3 posLei;
			if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posStart)
			{// スタートと同じだったら
				posLei = m_pRoute[nRouteInNum].posCol[0];	// レイ用位置

				bool bCheck = false;	// チェック
				while (true)
				{// ループ
					// 
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{
						break;
					}
					else
					{
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle)) * 1.0f;
					}
				}
			}
			else if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posEnd)
			{// エンドと同じだったら
				posLei = m_pRoute[nRouteInNum].posCol[2];	// レイ用位置

				bool bCheck = false;	// チェック
				while (true)
				{// ループ
					// 中心からの位置計算
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{// 範囲外
						break;
					}
					else
					{// 範囲内
						// レイを進める
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle + D3DX_PI), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle + D3DX_PI)) * 1.0f;
					}
				}
			}

			// ルート角度計算
			fRouteAngle = atan2f(m_pRoute[nRouteInNum].posCenter.x - m_pArea[nAreaCheckNum].pos.x, m_pRoute[nRouteInNum].posCenter.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = atan2f(posLei.x - m_pArea[nAreaCheckNum].pos.x, posLei.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = fRouteAngle - fRouteAngleRange;
			fRouteAngleRange = fabsf(fRouteAngleRange);

			if (fRouteAngleRange < -D3DX_PI)
				fRouteAngleRange += D3DX_PI * 2.0f;
			if (D3DX_PI < fRouteAngleRange)
				fRouteAngleRange -= D3DX_PI * 2.0f;

			fRouteAngleRange = fabsf(fRouteAngleRange);

			fRouteAngleL = fRouteAngle - fRouteAngleRange;
			fRouteAngleR = fRouteAngle + fRouteAngleRange;

			if (fRouteAngleL < fAngle && fAngle < fRouteAngleR)
			{
				bRoute = true;
			}
		}

		if (bRouteIn == false || (bRouteIn == true && bRoute == false))
		{// ルート内にいない、（ルート内にいる + ルート範囲外）
			pPos->x = m_pArea[nAreaCheckNum].pos.x + sinf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);
			pPos->z = m_pArea[nAreaCheckNum].pos.z + cosf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);

			pMove->x = 0.0f;
			pMove->z = 0.0f;
			bArea = true;	// エリア判定
			bLand = true;
		}
	}

	// ルート判定
	if (bArea == false && bAreaIn == false)
	{// エリア判定していない + エリアにいない
		if (bRouteCheck == true && bRouteCheckOld == true)
		{// どちらもTRUE
			// センター計算
			float fAngleCenter	= 0.0f;
			float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteCheckNum].posStart.x - m_pRoute[nRouteCheckNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteCheckNum].posStart.z - m_pRoute[nRouteCheckNum].pos[0].z, 2.0f));

			// 外積計算
			D3DXVECTOR3 vecALR, vecCLR;
			vecALR = m_pRoute[nRouteCheckNum].posStart - m_pRoute[nRouteCheckNum].posEnd;
			vecCLR = *pPos - m_pRoute[nRouteCheckNum].posEnd;

			// ルートの右側か左側か
			if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle - (D3DX_PI * 0.5f);	// 左側
			else
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle + (D3DX_PI * 0.5f);	// 右側

			// センター計算
			D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

			// センターへの角度計算
			float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

			int nHit = 0;	// ヒット数

			while (1)
			{
				nHit = 0;	// ヒット数
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
				{// 位置カウント
					// 外積計算
					D3DXVECTOR3 vecA, vecC;
					vecA = m_pRoute[nRouteCheckNum].posCol[nCntPos] - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
					vecC = *pPos - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

					if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
					{// 範囲内
						nHit++;
					}
				}

				if (nHit == 4)
				{// 範囲内
					bLand = true;
					break;		// 処理を抜ける
				}
				else
				{// 範囲外
					// 位置を更新（近づける）
					pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
					pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
				}
			}

			pMove->x = 0.0f;
			pMove->z = 0.0f;
		}
	}

	// その他の判定（抜けた場合）
	if (bArea == false && bAreaIn == false)
	{// エリア判定していない + エリアにいない
		if (bRouteIn == false)
		{// ルートにいない
			if (bRouteCheck == true && bRouteCheckOld == false)
			{// 現在が範囲外 + 前回が範囲外
				int nRouteNum = 0;		// ルート番号

				for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
				{// ルートカウント
					int nHit = 0;	// ヒット数

					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// 位置カウント
						// 外積計算
						D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
						vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// 範囲内
							nHit++;		// カウント
						}
					}

					if (nHit == 4)
					{// 範囲内
						nRouteNum = nCntRoute;		// 番号を記憶
						break;
					}
				}

				// センター計算
				float fAngleCenter	= 0.0f;
				float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteNum].posStart.x - m_pRoute[nRouteNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteNum].posStart.z - m_pRoute[nRouteNum].pos[0].z, 2.0f));

				// 外積計算
				D3DXVECTOR3 vecALR, vecCLR;
				vecALR = m_pRoute[nRouteNum].posStart - m_pRoute[nRouteNum].posEnd;
				vecCLR = *pPos - m_pRoute[nRouteNum].posEnd;

				// ルートの右側か左側か
				if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
					fAngleCenter = m_pRoute[nRouteNum].fAngle - (D3DX_PI * 0.5f);	// 左側
				else
					fAngleCenter = m_pRoute[nRouteNum].fAngle + (D3DX_PI * 0.5f);	// 右側

				// センター計算
				D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

				// センターへの角度計算
				float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

				int nHit = 0;	// ヒット数

				while (1)
				{
					nHit = 0;	// ヒット数
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// 位置カウント
						// 外積計算
						D3DXVECTOR3 vecA, vecC;
						vecA = m_pRoute[nRouteNum].posCol[nCntPos] - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// 範囲内
							nHit++;
						}
					}

					if (nHit == 4)
					{// 範囲内
						bLand = true;
						break;		// 処理を抜ける
					}
					else
					{// 範囲外
						// 位置を更新（近づける）
						pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
						pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
					}
				}

				pMove->x = 0.0f;
				pMove->z = 0.0f;
			}
		}
	}

	return bLand;
}

//=============================================================================
// エリアルート内にいるか判定処理						(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::CollisionInOut(D3DXVECTOR3 pos)
{
	bool bLand = false;

	if (bLand == false)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// エリアカウント
			float fRange;	// 範囲
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pos.z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// 範囲内か
				bLand = true;
				break;
			}
		}
	}

	if (bLand == false)
	{
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ルートカウント
			int nHit = 0;	// 現在ヒット数

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// 位置カウント
				D3DXVECTOR3 vecA, vecC;
				// 外積計算
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = pos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// 範囲内か
					nHit++;		// カウント
				}
			}

			if (nHit == 4)
			{// 範囲内か
				bLand = true;
				break;
			}
		}
	}

	return bLand;
}

//=============================================================================
// フィールドワープ衝突判定処理						(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp)
{
	int nWarpNow = 0;
	int nWarpNext = 0;
	bool bUseWarp = false;

	for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
	{
		float fRange;
		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNow = m_pWarp[nCntWarp].nArea_1;
			nWarpNext = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}

		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNext = m_pWarp[nCntWarp].nArea_1;
			nWarpNow = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}
	}

	if (bUseWarp == true)
	{
		*pPosWarp = m_pArea[nWarpNext].pos;

		if (m_pIcon != NULL)
		{// NULL以外の場合
			m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nWarpNow].pos.x, 100.0f, m_pArea[nWarpNow].pos.z));
			m_bDrawIcon = true;			// アイコン描画
		}
	}

	return bUseWarp;
}

//=============================================================================
// フィールドゴール衝突判定処理						(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::CollisionGoal(D3DXVECTOR3 *pPos)
{
	bool bUseGoal = false;;

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{
		if (nCntArea == AREA_END)
		{
			float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (fRange < 35.0f)
			{
				bUseGoal = true;

				if (m_pIcon != NULL)
				{// NULL以外の場合
					m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nCntArea].pos.x, 100.0f, m_pArea[nCntArea].pos.z));
					m_bDrawIcon = true;			// アイコン描画
				}

				break;
			}
		}
	}

	return bUseGoal;
}

//=============================================================================
// フィールドカメラ衝突判定処理						(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	bool bAreaIn	= false;	// エリアの中
	int nAreaInNum	= 0;		// エリアの中番号
	bool bRouteIn	= false;	// ルートの中
	int nRouteInNum = 0;		// ルートの中番号
	bool bRouteInV	= false;	// 視点ルートの中

	// エリアチェック
	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// エリアカウント
		float fRange;	// 範囲
		fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - posR.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - posR.z, 2.0f));
		if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
		{// 範囲内か
			bAreaIn		= true;			// エリア内にいる
			nAreaInNum	= nCntArea;		// エリア内の番号
			break;
		}
	}

	// ルートチェック
	for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
	{// ルートカウント
		int nHit = 0;	// 現在ヒット数
		int nHitV = 0;	// 現在ヒット数

		for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
		{// 位置カウント
			D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
			// 外積計算
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posR - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// 範囲内か
				nHit++;		// カウント
			}

			// 外積計算
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posV - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// 範囲内か
				nHitV++;		// カウント
			}
		}

		if (nHit == 4)
		{// 範囲内か
			bRouteIn	= true;			// 範囲外
			nRouteInNum = nCntRoute;	// エリア内の番号

			if (nHitV == 4)
			{// 範囲内か
				bRouteInV = true;			// 範囲外
			}

			break;
		}


	}

	bool bArea = false;		// エリア判定

	// エリア判定
	if (bAreaIn == true)
	{// エリア内にいる
		// 距離を計算
		float fRange = sqrtf(powf(m_pArea[nAreaInNum].pos.x - posV.x, 2.0f) + powf(m_pArea[nAreaInNum].pos.z - posV.z, 2.0f));
		if (fRange < m_pArea[nAreaInNum].fRange)
		{// 範囲内
			return true;
		}
		else
		{// 範囲外
			if (bRouteInV == true)
			{// 視点がルート内にいる
				return true;
			}
			else
			{// 視点がルート内にいない
				return false;
			}
		}
	}

	// ルート判定
	if (bArea == false && bAreaIn == false)
	{// エリア判定していない + エリアにいない
		if (bRouteIn == true)
		{// ルート内にいる
			int nHit = 0;	// 現在ヒット数

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// 位置カウント
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// 外積計算
				vecA = m_pRoute[nRouteInNum].posCol[nCntPos] - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = posV - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// 範囲内か
					nHit++;		// カウント
				}
			}

			if (nHit == 4)
			{// 範囲内か
				return true;
			}
			else
			{// 範囲外
				return false;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CMap_04::m_nMaxModel		= 0;		// モデル数

LPDIRECT3DTEXTURE9		**CMap_04::m_pTexture	= NULL;		// テクスチャ情報へのポインタ
LPD3DXMESH				*CMap_04::m_pMesh		= NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER			*CMap_04::m_pBuffMat	= NULL;		// マテリアル情報へのポインタ
DWORD					*CMap_04::m_nNumMat		= NULL;		// マテリアル情報の数

CModel::MODEL_INFO		*CMap_04::m_ModelInfo	= NULL;		// モデルインフォ

//=============================================================================
// コンストラクタ										(public)	*** CMap_04 ***
//=============================================================================
CMap_04::CMap_04() : CMap()
{
	m_nModelWarp = 0;									// モデルワープ数
	m_pModelWarp = NULL;								// モデルワープ

	m_nModelObject = 0;									// モデルオブジェクト数
	m_pModelObject = NULL;								// モデルオブジェクト

	m_pField = NULL;									// フィールド

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		m_pSky[nCount] = NULL;							// NULLへ
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// 回転
	}

	m_nGrassMax = 0;									// 最大草
	m_pGrass = NULL;									// 草

	m_nAreaMax = 0;										// エリア最大数

	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaNum[nCount] = 0;							// エリア数

	m_pArea = NULL;										// エリア

	m_nRouteMax = 0;									// ルート最大数
	m_pRoute = NULL;									// ルート

	m_nPatternMax = 0;									// パターン最大数
	m_pPattern = NULL;									// パターン

	m_pWarp = NULL;										// ワープ

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
		m_pWarpCylinder[nCount] = NULL;					// ワープシリンダー
	m_WarpCylinderUV = D3DXVECTOR2(0.0f, 0.0f);		// ワープシリンダーUV

	m_bGoal = false;									// ゴールできるか
	m_pGoalCylinder = NULL;								// ゴールシリンダー

	m_bDrawIcon = false;								// アイコン描画
	m_pIcon = NULL;										// アイコン
}

//=============================================================================
// デストラクタ										(public)	*** CMap_04 ***
//=============================================================================
CMap_04::~CMap_04()
{

}

//=============================================================================
// ロード処理										(public)	*** CMap_04 ***
//=============================================================================
HRESULT CMap_04::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MAP\\gamemap.txt", &m_nMaxModel);		// モデル取得

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
// アンロード処理										(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::Unload(void)
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
// 初期化処理										(public)	*** CMap_04 ***
//=============================================================================
HRESULT CMap_04::Init(void)
{
	// メッシュフィールド
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3600.0f, 0.0f, 3600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(24.0f, 24.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	// 空１
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// 空２
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::Uninit(void)
{
	// モデル
	if (m_pModelWarp != NULL)
	{// NULL以外の場合
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++) // モデル数カウント
			m_pModelWarp[nCntModel].Uninit();	// 終了処理

		delete[] m_pModelWarp;		// メモリ開放
		m_pModelWarp = NULL;		// NULLへ
	}
	if (m_pModelObject != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++) // モデル数カウント
			m_pModelObject[nCntModel].Uninit();	// 終了処理

		delete[] m_pModelObject;	// メモリ開放
		m_pModelObject = NULL;		// NULLへ
	}

	// フィールド
	if (m_pField != NULL)
	{// NULL以外の場合
		m_pField->Uninit();		// 終了処理
		m_pField = NULL;		// NULLへ
	}

	// 空
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// 最大数カウント
		if (m_pSky[nCount] != NULL)
		{// NULL以外の場合
			m_pSky[nCount]->Uninit();	// 終了処理
			m_pSky[nCount] = NULL;		// NULLへ
		}
	}

	// 草
	if (m_pGrass != NULL)
	{// NULL以外の場合
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// 草カウント
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Uninit();		// 終了処理
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = NULL;			// NULLへ
				}
			}
		}

		delete[] m_pGrass;	// メモリ開放
		m_pGrass = NULL;		// NULLへ
	}

	// マップ
	if (m_pArea != NULL)
	{// NULL以外の場合
		delete[] m_pArea;		// 破棄
		m_pArea = NULL;			// NULLへ
	}
	if (m_pRoute != NULL)
	{// NULL以外の場合
		delete[] m_pRoute;		// 破棄
		m_pRoute = NULL;		// NULLへ
	}
	if (m_pPattern != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nPatternMax; nCount++)
		{// エリアカウント
			delete[] m_pPattern[nCount].nPattern;
			m_pPattern[nCount].nPattern = NULL;
		}

		delete[] m_pPattern;	// 破棄
		m_pPattern = NULL;		// NULLへ
	}
	if (m_pWarp != NULL)
	{// NULL以外の場合
		delete[] m_pWarp;		// 破棄
		m_pWarp = NULL;			// NULLへ
	}

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
	{
		if (m_pWarpCylinder[nCount] != NULL)
		{
			m_pWarpCylinder[nCount]->Uninit();
			m_pWarpCylinder[nCount] = NULL;
		}
	}

	// ゴールシリンダー
	if (m_pGoalCylinder != NULL)
	{// NULL以外の場合
		m_pGoalCylinder->Uninit();
		m_pGoalCylinder = NULL;
	}

	// アイコン
	if (m_pIcon != NULL)
	{// NULL以外の場合
		m_pIcon->Uninit();
		m_pIcon = NULL;
	}

	CMap::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::Update(void)
{
	// 空0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// 回転

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// テクスチャ設定
	}

	// 空1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL以外の場合
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// 回転

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// テクスチャ設定
	}

	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->Getpos();
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 CameraposR = pCamera->Getpos(1);
	D3DXVECTOR3 Camerarot = pCamera->Getrot();

	if (m_pGrass != NULL)
	{// NULL以外の場合
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{// 最大数カウント
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// 草カウント
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{// ラインカウント
					D3DXVECTOR3 posVtx[4];		// 頂点受け取り
					D3DXVECTOR3 posCenter = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);		// 頂点情報取得

																							// カメラ範囲
					float fCameraL = Camerarot.y - (D3DX_PI * 0.5f);
					float fCameraR = Camerarot.y + (D3DX_PI * 0.5f);

					// 範囲調整
					if (fCameraL < -D3DX_PI)
						fCameraL += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraL)
						fCameraL -= D3DX_PI * 2.0f;

					if (fCameraR < -D3DX_PI)
						fCameraR += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraR)
						fCameraR -= D3DX_PI * 2.0f;

					float fCameraAngle = atan2f(posCenter.x - CameraposR.x, posCenter.z - CameraposR.z);

					if (Camerarot.y < (-D3DX_PI * 0.5f) || (D3DX_PI * 0.5f) < Camerarot.y)
					{
						if (fCameraR < fCameraAngle && fCameraAngle < fCameraL)
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}
					else
					{
						if (fCameraL < fCameraAngle && fCameraAngle < fCameraR)
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// 範囲チェック
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}

					float fRange;
					fRange = sqrtf(powf(posCenter.x - posPlayer.x, 2.0f) + powf(posCenter.z - posPlayer.z, 2.0f));
					if (fRange < 850.0f)
					{
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
					}
					else
					{
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
					}

					bool bCheck = false;
					if (fRange < 30.0f)
					{
						bCheck = true;
					}

					if (bCheck == true)
					{
						float fAngle = atan2f(posCenter.x - posPlayer.x, posCenter.z - posPlayer.z);
						D3DXVECTOR3 posCenterNext = m_pGrass[nCntGrass].posCenter[nCount][nCntNum] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
						fRange = sqrtf(powf(posCenter.x - posCenterNext.x, 2.0f) + powf(posCenter.z - posCenterNext.z, 2.0f));
						if (fRange < 20.0f)
						{
							posVtx[0] = posVtx[0] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							posVtx[1] = posVtx[1] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenterNext;
						}

						m_pGrass[nCntGrass].bFall[nCount][nCntNum] = true;
					}
					else
					{
						if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == true)
						{
							posVtx[0].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].x - posVtx[0].x) * 0.1f;
							posVtx[0].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].z - posVtx[0].z) * 0.1f;
							posVtx[1].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].x - posVtx[1].x) * 0.1f;
							posVtx[1].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].z - posVtx[1].z) * 0.1f;
							m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum]++;

							if (m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] == 20)
							{
								m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
								m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenter;
								m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;
							}
						}
					}

					if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == false)
					{
						if (m_pGrass->nCounter < 120)
						{// 一定カウントより低い
							posVtx[0].x += sinf(0.0f) * 0.023f;
							posVtx[0].x += cosf(0.0f) * 0.023f;
							posVtx[1].x += sinf(0.0f) * 0.023f;
							posVtx[1].x += cosf(0.0f) * 0.023f;
						}
						else
						{// 一定カウント以上
							posVtx[0].x -= sinf(0.0f) * 0.023f;
							posVtx[0].x -= cosf(0.0f) * 0.023f;
							posVtx[1].x -= sinf(0.0f) * 0.023f;
							posVtx[1].x -= cosf(0.0f) * 0.023f;
						}
					}
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetVtxSize(posVtx);		// 頂点情報設定
				}
			}

			{
				m_pGrass[nCntGrass].nCounter++;		// カウントする

				if (m_pGrass[nCntGrass].nCounter == 240)
				{// 指定したカウントになったら
					m_pGrass[nCntGrass].nCounter = 0;	// カウント初期化
				}
			}
		}
	}

	m_WarpCylinderUV.y -= 0.01f;
	for (int nCntWarp = 0; nCntWarp < m_nModelWarp; nCntWarp++)
	{
		if (m_pWarpCylinder[nCntWarp] != NULL)
		{
			m_pWarpCylinder[nCntWarp]->SetTex(m_WarpCylinderUV);
		}
	}

	if (m_bGoal == true)
	{
		if (m_pGoalCylinder != NULL)
		{
			D3DXVECTOR3 size = m_pGoalCylinder->Getsize();
			if (size.y != 800.0f)
			{
				size.y += 5.0f;
				if (800.0f < size.y) size.y = 800.0f;
			}
			m_pGoalCylinder->SetStart(m_pGoalCylinder->Getpos(), m_pGoalCylinder->Getrot(), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
		}

		if (m_pIcon != NULL)
		{
			D3DXCOLOR col = m_pIcon->Getcol();
			if (m_bDrawIcon == true)
			{
				if (col.a != 1.0f)
				{
					col.a += 0.05f;
					if (1.0f < col.a) col.a = 1.0f;
				}
			}
			else
			{
				if (col.a != 0.0f)
				{
					col.a -= 0.05f;
					if (col.a < 0.0f) col.a = 0.0f;
				}
			}
			m_pIcon->Setcol(col);
		}
		m_bDrawIcon = false;
	}
}

//=============================================================================
// 描画処理											(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// 計算用マトリックス

	if (1 < m_nPatternMax)
	{
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
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

			if (m_pModelWarp != NULL)
			{// NULL以外の場合
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				m_pModelWarp[nCntModel].Draw();

				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

				// 影表示
				//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
			}
		}
	}

	for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
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

		if (m_pModelObject != NULL)
		{// NULL以外の場合
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pModelObject[nCntModel].Draw();

			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// 影表示
			//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
		}
	}
}

//=============================================================================
// フィールド設定処理									(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::SetField(D3DXVECTOR3 pos)
{
	// エリア数
	m_nAreaNum[AREA_START]	= 0;
	m_nAreaNum[AREA_END]	= 0;
	m_nAreaNum[AREA_NORMAL]	= 0;
	m_nAreaNum[AREA_BATTLE]	= 1;
	m_nAreaNum[AREA_ITEM]	= 0;

	// エリア総数計算
	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaMax += m_nAreaNum[nCount];

	// メモリ確保
	if (m_pArea == NULL) m_pArea = new AREA_RANGE[m_nAreaMax];

	// エリア初期化
	for (int nCount = 0; nCount < m_nAreaMax; nCount++)
	{// エリアカウント
		m_pArea[nCount].nArea	= AREA_START;						// エリア
		m_pArea[nCount].fRange	= 0.0f;								// 範囲
		m_pArea[nCount].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心
	}

	// エリア、ルート生成
	{
		// エリア
		{
			int nCntArea = 0;		// エリア番号
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
			{// エリアカウント
				for (int nCntNum = 0; nCntNum < m_nAreaNum[nCount]; nCntNum++)
				{// エリア数カウント
					int nCheckCounter = 0;	// チェックカウント
					bool bCheck = false;	// チェック
					do
					{// ループ
						bCheck = false;

						switch (nCount)
						{// エリア
						case AREA_START:	// スタート
							m_pArea[nCntArea].nArea = AREA_START;		// エリア
							m_pArea[nCntArea].fRange = 280.0f;			// 範囲
							m_pArea[nCntArea].pos	= pos;				// 位置
							break;
						case AREA_END:		// エンド
							m_pArea[nCntArea].nArea = AREA_END;			// エリア
							m_pArea[nCntArea].fRange = 280.0f;			// 範囲

							// 位置
							{
								float fRange = 0.0f;	// 範囲
								do
								{// ループ
									m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
									m_pArea[nCntArea].pos.y = 0.0f;
									m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));

									// 中心からの距離を計算
									fRange = sqrtf(powf(m_pArea[0].pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(m_pArea[0].pos.z - m_pArea[nCntArea].pos.z, 2.0f));
								} while (fRange < 1400.0f &&
									-700.0f < m_pArea[nCntArea].pos.x && m_pArea[nCntArea].pos.x < 700.0f &&
									-700.0f < m_pArea[nCntArea].pos.z && m_pArea[nCntArea].pos.z < 700.0f);		// 一定距離以上だったら
							}
							break;
						case AREA_NORMAL:	// ノーマル
						case AREA_BATTLE:	// バトル
							// エリア
							if (nCount == AREA_NORMAL)	m_pArea[nCntArea].nArea = AREA_NORMAL;
							if (nCount == AREA_BATTLE)	m_pArea[nCntArea].nArea = AREA_BATTLE;
							/*m_pArea[nCntArea].fRange = (float)(rand() % 100 + 580);			// 範囲

							// 位置
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));*/

							m_pArea[nCntArea].fRange = 800.0f;			// 範囲

							// 位置
							m_pArea[nCntArea].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

							break;
						case AREA_ITEM:		// アイテム
							m_pArea[nCntArea].nArea = AREA_ITEM;			//　エリア
							m_pArea[nCntArea].fRange = (float)(rand() % 50 + 320);			// 範囲

							// 位置
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							break;
						}

						// エリア同士重なっていないか
						for (int nCntCheck = 0; nCntCheck < nCntArea; nCntCheck++)
						{// エリアカウント
							// 中心からの位置計算
							float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - m_pArea[nCntCheck].pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - m_pArea[nCntCheck].pos.z, 2.0f));
							if (fRange < m_pArea[nCntArea].fRange + m_pArea[nCntCheck].fRange + 100.0f)
							{// エリアが重なっていないか
								bCheck = true;		// 重なっている
								nCheckCounter++;	// カウント
								break;
							}
						}

						if (nCheckCounter == 30) break;
					} while (bCheck != false);
					
					if (nCheckCounter != 30)
						nCntArea++;		// エリアカウント
				}
			}
			m_nAreaMax = nCntArea;
		}

		if (1 < m_nAreaMax)
		{
			// ルート
			{
				// ルート数
				m_nRouteNum[AREA_START] = m_nAreaNum[AREA_START];
				m_nRouteNum[AREA_END] = m_nAreaNum[AREA_END];
				m_nRouteNum[AREA_NORMAL] = m_nAreaNum[AREA_NORMAL];
				m_nRouteNum[AREA_BATTLE] = m_nAreaNum[AREA_BATTLE];
				m_nRouteNum[AREA_ITEM] = m_nAreaNum[AREA_ITEM];

				// ルート総数計算
				for (int nCount = 0; nCount < AREA_MAX; nCount++)
					m_nRouteMax += m_nRouteNum[nCount];

				// メモリ確保
				if (m_pRoute == NULL) m_pRoute = new ROUTE_RANGE[m_nRouteMax];

				// ルート初期化
				for (int nCount = 0; nCount < m_nRouteMax; nCount++)
				{// ルートカウント
					m_pRoute[nCount].nAreaNumStart = 0;									// スタートエリア番号
					m_pRoute[nCount].nAreaNumEnd = 0;									// エンドエリア番号
					m_pRoute[nCount].posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// スタートエリア位置
					m_pRoute[nCount].posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// エンドエリア位置
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
						m_pRoute[nCount].pos[nCntPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ルート頂点位置
					m_pRoute[nCount].fAngle = 0.0f;										// ルート角度
					m_pRoute[nCount].fLenght = 0.0f;										// ルート距離
					m_pRoute[nCount].fRange = 0.0f;										// ルート範囲（道幅）
				}

				int nRoute = 0;		// ルート番号
				for (int nCountArea = 0; nCountArea < m_nAreaMax; nCountArea++)
				{// エリアカウント
					int nAreaNum = nCountArea;		// エリア番号
					float fAreaLenght = 5000.0f;	// 距離

					for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
					{// エリアカウント
						if (nCountArea != nCntAreaCheck &&
							(nCountArea != AREA_START || (nCountArea == AREA_START && nCntAreaCheck != AREA_END)) &&
							(nCountArea != AREA_END || (nCountArea == AREA_END && nCntAreaCheck != AREA_START)))
						{// 同じエリアでない、スタートとエンドを繋がない
							// 中心からの位置計算
							float fRange = sqrtf(powf(m_pArea[nCountArea].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nCountArea].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
							if (fRange < fAreaLenght)
							{// 距離がより近かったら
								nAreaNum = nCntAreaCheck;	// 番号
								fAreaLenght = fRange;		// 距離
							}
						}
					}

					bool bCheck = false;	// チェック
					for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
					{// ルートカウント
						if ((m_pRoute[nCntRoute].nAreaNumStart == nCountArea && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
							(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nCountArea))
						{// 同じルートがないか
							bCheck = true;	// ある
						}
					}

					if (bCheck == false)
					{// 生成する
						// 範囲（道幅）
						m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

						// 距離
						m_pRoute[nRoute].fLenght = fAreaLenght;

						// 角度
						m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nCountArea].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nCountArea].pos.z);

						// 位置
						m_pRoute[nRoute].nAreaNumStart = nCountArea;				// スタート番号
						m_pRoute[nRoute].nAreaNumEnd = nAreaNum;					// エンド番号
						m_pRoute[nRoute].posStart = m_pArea[nCountArea].pos;	// スタート位置
						m_pRoute[nRoute].posEnd = m_pArea[nAreaNum].pos;	// エンド位置

						// 中心を計算
						m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

						// 頂点計算
						m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

						// 判定用の頂点計算
						m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

						nRoute++;	// ルートカウント
						if (nRoute == m_nRouteMax) break;
					}
				}

				// 各エリアにルートが何本繋がったか
				int *nRouteCounter = new int[m_nAreaMax];
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
					nRouteCounter[nCount] = 0;

				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ルートカウント
					nRouteCounter[m_pRoute[nCntRoute].nAreaNumStart]++;
					nRouteCounter[m_pRoute[nCntRoute].nAreaNumEnd]++;
				}

				// 末端のエリアを計算
				int nRouteEndNum = 0;
				int *nRouteEnd = new int[m_nAreaMax];
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
					nRouteEnd[nCount] = 0;

				// 繋がれたルート数
				int nRoute_0 = 0;
				int nRoute_1 = 0;
				int nRoute_2 = 0;

				for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
				{// ルートカウント
					nRouteEnd[nRouteEndNum] = nCntRoute;	// 番号記憶
					nRouteEndNum++;							// カウント

					if (nRouteCounter[nCntRoute] == 0)
						nRoute_0++;									// ルートが０
					else if (nRouteCounter[nCntRoute] == 1)
						nRoute_1++;									// ルートが１
					else if (1 < nRouteCounter[nCntRoute])
						nRoute_2 += nRouteCounter[nCntRoute] - 1;	// ルートが２
				}

				// パターン数を計算
				int nRoutePattern = nRoute - nRoute_2 + nRoute_0;

				if (nRoute_0 != 0)
				{// ルート
					int nNumber = 0;
					for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
					{// ルートカウント
						if (nRouteCounter[nCntRoute] == 0)
						{// 繋がっていない
							nNumber = nCntRoute;
							break;
						}
					}

					int nAreaNum = nNumber;			// エリア番号
					float fAreaLenght = 5000.0f;	// 距離

					for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
					{// エリアカウント
						if (nNumber != nCntAreaCheck &&
							(nNumber != AREA_START || (nNumber == AREA_START && nCntAreaCheck != AREA_END)) &&
							(nNumber != AREA_END || (nNumber == AREA_END && nCntAreaCheck != AREA_START))) {// 同じエリアでない、スタートとエンドを繋がない
							// 中心からの位置計算
							float fRange = sqrtf(powf(m_pArea[nNumber].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nNumber].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
							if (fRange < fAreaLenght)
							{// 距離がより近かったら
								nAreaNum = nCntAreaCheck;	// 番号
								fAreaLenght = fRange;		// 距離
							}
						}
					}

					bool bCheck = false;	// チェック
					for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
					{// ルートカウント
						if ((m_pRoute[nCntRoute].nAreaNumStart == nRouteEnd[nCntRoute] && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
							(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nRouteEnd[nCntRoute]))
						{// 同じルートがないか
							bCheck = true;	// ある
						}
					}

					if (bCheck == false)
					{// 生成する
						// 範囲（道幅）
						m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

						// 距離
						m_pRoute[nRoute].fLenght = fAreaLenght;

						// 角度
						m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nNumber].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nNumber].pos.z);

						// 位置
						m_pRoute[nRoute].nAreaNumStart = nNumber;
						m_pRoute[nRoute].nAreaNumEnd = nAreaNum;
						m_pRoute[nRoute].posStart = m_pArea[nNumber].pos;
						m_pRoute[nRoute].posEnd = m_pArea[nAreaNum].pos;

						// 中心を計算
						m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

						// 頂点計算
						m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

						// 判定用の頂点計算
						m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

						nRoute++;	// ルートカウント
						nRoutePattern--;
					}
				}

				m_nRouteMax = nRoute;	// ルート数カウント

				m_nPatternMax = nRoutePattern;		// パターン数を記憶

				// メモリ確保
				if (m_pPattern == NULL)
					m_pPattern = new ROUTE_PATTERN[m_nPatternMax];

				if (m_pPattern != NULL)
				{// NULL以外の場合
					for (int nCntPattert = 0; nCntPattert < m_nPatternMax; nCntPattert++)
					{// パターン数カウント
						// 初期値設定
						m_pPattern[nCntPattert].nMaxPattern = 0;
						m_pPattern[nCntPattert].nPattern = new int[m_nAreaMax];
						m_pPattern[nCntPattert].bStart = false;
						m_pPattern[nCntPattert].bEnd = false;

						if (nCntPattert == 0)
						{// カウントが０だったら
							m_pPattern[nCntPattert].nPattern[0] = m_pRoute[0].nAreaNumStart;
							m_pPattern[nCntPattert].nPattern[1] = m_pRoute[0].nAreaNumEnd;
							m_pPattern[nCntPattert].nMaxPattern = 2;
						}
						else
						{// カウントが０以外だったら
							int nNumber = 0;		// 番号確認
							bool bCheck = false;	// チェック
							// 同じ番号にならないよう調整
							do
							{// ループ
								bCheck = false;		// 初期化
								for (int nCntCheck = 0; nCntCheck < nCntPattert; nCntCheck++)
								{// パターンカウント
									for (int nCntArea = 0; nCntArea < m_pPattern[nCntCheck].nMaxPattern; nCntArea++)
									{// エリアカウント
										if (m_pPattern[nCntCheck].nPattern[nCntArea] == nNumber)
										{// 同じだったら
											nNumber++;		// カウント
											bCheck = true;	// チェック
											break;
										}
									}
									if (bCheck == true) break;
								}
							} while (bCheck != false);

							// 番号につながるルートを検索
							for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
							{// ルートカウント
								if (nNumber == m_pRoute[nCntRoute].nAreaNumStart || nNumber == m_pRoute[nCntRoute].nAreaNumEnd)
								{// 番号が同じ
									m_pPattern[nCntPattert].nPattern[0] = m_pRoute[nCntRoute].nAreaNumStart;
									m_pPattern[nCntPattert].nPattern[1] = m_pRoute[nCntRoute].nAreaNumEnd;
									m_pPattern[nCntPattert].nMaxPattern = 2;
									break;
								}
							}
						}

						for (int nCntPattertNum = 0; nCntPattertNum < m_pPattern[nCntPattert].nMaxPattern; nCntPattertNum++)
						{// パターンにあるエリア数カウント
							for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
							{// ルートカウント
								if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumStart)
								{// スタート番号と同じ
									for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
									{// パターンにあるエリア数カウント
										if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumEnd)
										{// エンド番号と同じ
											break;
										}

										if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
										{// 同じ番号がなかったら
											// 番号記憶
											m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumEnd;
											m_pPattern[nCntPattert].nMaxPattern++;	// パターンのエリア数を増やす
										}
									}
								}
								else if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumEnd)
								{// エンド番号と同じ
									for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
									{// パターンにあるエリア数カウント
										if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumStart)
										{// スタート番号と同じ
											break;
										}

										if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
										{// 同じ番号がなかったら
											// 番号記憶
											m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumStart;
											m_pPattern[nCntPattert].nMaxPattern++;	// パターンのエリア数を増やす
										}
									}
								}
							}
						}
					}
				}

				for (int nCntPattern = 0; nCntPattern < m_nPatternMax; nCntPattern++)
				{// パターン数カウント
					for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattern].nMaxPattern; nCntCheck++)
					{// パターンにあるエリア数カウント
						if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_START)
						{// スタート番号と同じ
							m_pPattern[nCntPattern].bStart = true;		// スタートがある
						}
						else if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_END)
						{// エンド番号と同じ
							m_pPattern[nCntPattern].bEnd = true;		// エンドがある
						}
					}
				}

				if (1 < m_nPatternMax)
				{// パターン数が１より多い
					// メモリ確保
					if (m_pWarp == NULL)
						m_pWarp = new WARP[m_nPatternMax - 1];

					if (m_pWarp != NULL)
					{// NULL以外の場合
						for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
						{// ワープカウント
							if (nCntWarp == 0)
							{// カウントが０だったら
								bool bCheck = false;	// チェック
								do
								{// ループ
									bCheck = false;		// 初期化
									m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// パターン設定

									do
									{// ループ
										m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// パターン設定
									} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// パターンが違うなら抜ける

									if (2 < m_nPatternMax)
									{// パターンが２より多い
										if ((m_pPattern[m_pWarp[nCntWarp].nPattern_1].bStart == true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd == true) ||
											(m_pPattern[m_pWarp[nCntWarp].nPattern_1].bEnd == true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart == true))
										{// スタートとエンドが結ばれた
											bCheck = true;		// チェック
										}
									}
								} while (bCheck != false);
							}
							else
							{// カウントが１以上だったら
								bool bCheck = false;	// チェック
								do
								{// ループ
									bCheck = false;		// 初期化
									m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// パターン設定

									for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
									{// ワープカウント
										if (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_1 ||
											m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_2)
										{// 同じ番号がある
											bCheck = true;		// チェック
											break;
										}
									}
								} while (bCheck != false);

								do
								{// ループ
									bCheck = false;		// チェック
									do
									{// ループ
										m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// パターン設定
									} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// パターンが違うなら抜ける

									if (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern == 2 &&
										(m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart == true || m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd == true))
									{// パターンのエリアすうが２ + （スタートがある、エンドがある）
										bCheck = true;	// チェック
									}
								} while (bCheck != false);
							}

							bool bCheck = false;	// チェック
							do
							{// ループ
								bCheck = false;		// 初期化
								do
								{// ループ
									m_pWarp[nCntWarp].nArea_1 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_1].nMaxPattern;		// エリア設定
								} while (m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_START ||
									m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_END);		// スタートとエンド以外だったら抜ける
								m_pWarp[nCntWarp].nArea_1 = m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1];		// エリア番号設定

								do
								{
									m_pWarp[nCntWarp].nArea_2 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern;		// エリア設定
								} while (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_START ||
									m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_END);		// スタートとエンド以外だったら抜ける
								m_pWarp[nCntWarp].nArea_2 = m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2];		// エリア番号設定

								for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
								{// ワープカウント
									if (m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_1 ||
										m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_2 ||
										m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_1 ||
										m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_2)
									{// エリアが同じ
										bCheck = true;		// チェック
										break;
									}
								}
							} while (bCheck != false);
						}
					}
				}

				delete[] nRouteCounter;	// 破棄
				delete[] nRouteEnd;		// 破棄
			}
		}
	}

	int nMaxVtx = m_pField->GetMaxVtx();	// 頂点取得

	// 頂点位置設定
	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{
		D3DXVECTOR3 posVtx = m_pField->GetVtx(nCntVtx);

		/*if (posVtx.x < -1500.0f || 1500.0f < posVtx.x || posVtx.z < -1500.0f || 1500.0f < posVtx.z)
		{
		posVtx.y = 80.0f + (rand() % 80);

		m_pField->SetVtx(nCntVtx, posVtx);
		m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		}
		else*/
		{
			posVtx.y = 80.0f + (rand() % 80);			// 位置設定

			// 頂点、色設定
			m_pField->SetVtx(nCntVtx, posVtx);
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

			// エリア
			if (m_pArea != NULL)
			{// NULL以外の場合
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				{// エリアカウント
					// 中心からの位置計算
					float fRange = sqrtf(powf(m_pArea[nCount].pos.x - posVtx.x, 2.0f) + powf(m_pArea[nCount].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[nCount].fRange)
					{// 範囲内だったら
						posVtx.y = 0.0f - (rand() % 15);	// 位置設定

						// 頂点、色設定
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						//if (m_pArea[nCount].nArea == AREA_START || m_pArea[nCount].nArea == AREA_END)
						//	m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

						/*if (m_pWarp != NULL)
						{
							for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
							{
								if (m_pWarp[nCntWarp].nArea_1 == nCount || m_pWarp[nCntWarp].nArea_2 == nCount)
								{
									if (nCntWarp == 0)
										m_pField->SetCol(nCntVtx, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
									else
										m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

									break;
								}
							}
						}*/

						break;
					}					
				}
			}

			// ルート
			if (m_pRoute != NULL)
			{// NULL以外の場合
				for (int nCount = 0; nCount < m_nRouteMax; nCount++)
				{// ルートカウント
					D3DXVECTOR3 vecA, vecC;		// 計算用ベクトル（頂点同士、頂点とPOS）
					int nCheck = 0;				// チェック
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// 位置カウント
						// 外積計算
						vecA = m_pRoute[nCount].pos[nCntPos] - m_pRoute[nCount].pos[(nCntPos + 1) % 4];
						vecC = posVtx - m_pRoute[nCount].pos[(nCntPos + 1) % 4];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
						{// 範囲内か
							nCheck++;	// カウント
						}
					}

					if (nCheck == 4)
					{// 範囲内だったら
						posVtx.y = 0.0f - (rand() % 15);	// 位置設定

						// 頂点、色設定
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}

			// ワープ
			if (m_pWarp != NULL)
			{
				for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
				{
					float fRange;
					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_1].fRange * 0.2f)
					{// 範囲内だったら
						posVtx.y = 0.0f;		// 位置設定

						// 頂点設定
						m_pField->SetVtx(nCntVtx, posVtx);
					}

					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_2].fRange * 0.2f)
					{// 範囲内だったら
						posVtx.y = 0.0f;		// 位置設定

						// 頂点設定
						m_pField->SetVtx(nCntVtx, posVtx);
					}
				}
			}
		}
	}
	m_pField->SetVecNor();	// 法線計算
}

//=============================================================================
// オブジェクト設定処理								(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::SetObject(void)
{
	if (1 < m_nPatternMax)
	{// ワープが存在する
		// ワープモデル計算
		m_nModelWarp = (m_nPatternMax - 1) * 2;

		// ワープモデル
		if (m_pModelWarp == NULL && m_ModelInfo != NULL)
		{// NULLの場合
			m_pModelWarp = new CModel[m_nModelWarp];	// メモリ確保

			for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
			{// モデル数カウント
				// 初期値設定
				m_pModelWarp[nCntModel].SetInitAll(m_pTexture[0], m_pMesh[0], m_pBuffMat[0], m_nNumMat[0], m_ModelInfo[0].pos, m_ModelInfo[0].rot);
				// 初期化処理
				m_pModelWarp[nCntModel].Init();

				// 親モデルを設定
				m_pModelWarp[nCntModel].SetParent(NULL);

				if (m_pWarpCylinder[nCntModel] == NULL)
				{// NULL以外の場合
					m_pWarpCylinder[nCntModel] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);

					// 初期値設定
					m_pWarpCylinder[nCntModel]->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), 
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 60.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 10.0f));
					// 初期化処理
					m_pWarpCylinder[nCntModel]->Init();
					m_pWarpCylinder[nCntModel]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
					m_pWarpCylinder[nCntModel]->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
				}
			}
		}

		for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
		{// パターンカウント
			if (m_pModelWarp != NULL)
			{// NULL以外の場合
				m_pModelWarp[0 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pModelWarp[1 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);
			}

			if (m_pWarpCylinder[0 + (nCntWarp * 2)] != NULL && m_pWarpCylinder[1 + (nCntWarp * 2)] != NULL)
			{// NULL以外の場合
				m_pWarpCylinder[0 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pWarpCylinder[1 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);

				if (nCntWarp == 0)
				{
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
					}
				}
				else if (nCntWarp == 1)
				{
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
					}
				}
			}
		}
	}

	if (m_pGoalCylinder == NULL)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{
			m_pGoalCylinder = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GOAL);

			m_pGoalCylinder->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), m_pArea[nCntArea].pos + D3DXVECTOR3(0.0f, -50.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
			m_pGoalCylinder->Init();
			m_pGoalCylinder->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
			m_pGoalCylinder->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
		}
	}

	if (m_pIcon == NULL)
	{// NULL以外の場合
		m_pIcon = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_MAPICON);

		m_pIcon->SetInitAll(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.125f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		m_pIcon->Init();
		m_pIcon->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
		m_pIcon->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE, D3DCMP_NOTEQUAL);
		m_pIcon->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	CScene		*pScene		= NULL;		// シーン
	CScene		*pSceneNext	= NULL;		// 次シーン
	CSceneMesh	*pMesh		= NULL;		// メッシュ
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

	int nNumSet = 28;
	m_nGrassMax = m_nAreaMax * nNumSet;

	if (m_pGrass == NULL)
	{
		m_pGrass = new GRASS[m_nGrassMax];

		if (m_pGrass != NULL)
		{
			for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
			{
				for (int nCntGrass = 0; nCntGrass < nNumSet; nCntGrass++)
				{
					D3DXVECTOR3 pos;		// 位置
					float fAngle = (float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)) * 0.01f;
					float fLenght = (float)(rand() % (int)(m_pArea[nCntArea].fRange - 150.0f) + 100.0f);
					// 位置計算
					pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght;

					int nNum = nCntArea * nNumSet + nCntGrass;

					// 初期値設定
					m_pGrass[nNum].nCounter = 0;
					m_pGrass[nNum].pos = pos;
					m_pGrass[nNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
					{// 草カウント
						D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 14.0f, 0.0f);
						D3DXVECTOR3 rot = m_pGrass[nNum].rot + D3DXVECTOR3(0.0f, (D3DX_PI * 2.0f / (float)(m_GRASS_NUM * 2)) * nCount, 0.0f);

						float fAngle = -D3DX_PI + ((float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f);
						float fLenght = -5.0f + (float)(rand() % 10);

						D3DXVECTOR3 posLeft = m_pGrass[nNum].pos +
							D3DXVECTOR3(sinf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f)), 7.0f, cosf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f))) +
							(D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght);

						for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
						{
							m_pGrass[nNum].pGrass[nCount][nCntNum] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_GRASS);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->SetInitAll(posLeft, rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->Init();
							m_pGrass[nNum].pGrass[nCount][nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GRASS));
							m_pGrass[nNum].pGrass[nCount][nCntNum]->DrawEdit(false, false, true, false, true, 0, D3DCULL_NONE);

							if (pMesh != NULL)
							{
								D3DXVECTOR3 pos = m_pGrass[nNum].pGrass[nCount][nCntNum]->Getpos();
								pos.y = pMesh->GetHeight(pos);
								pos.y += 7.0f;
								m_pGrass[nNum].pGrass[nCount][nCntNum]->Setpos(pos);
							}

							m_pGrass[nNum].posCenter[nCount][nCntNum] = posLeft;

							D3DXVECTOR3 posVtx[4];
							m_pGrass[nNum].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);
							m_pGrass[nNum].posVtx[nCount][nCntNum][0] = posVtx[0];
							m_pGrass[nNum].posVtx[nCount][nCntNum][1] = posVtx[1];

							m_pGrass[nNum].bFall[nCount][nCntNum] = false;
							m_pGrass[nNum].nFallCounter[nCount][nCntNum] = 0;

							posLeft += D3DXVECTOR3(sinf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f), 0.0f, cosf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f));
						}
					}
				}
			}
		}
	}

	/*if (m_pModelObject == NULL)
	{
		m_nModelObject = 500;
		m_pModelObject = new CModel[m_nModelObject];

		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
		{
			// 初期値設定
			m_pModelObject[nCntModel].SetInitAll(m_pTexture[1], m_pMesh[1], m_pBuffMat[1], m_nNumMat[1], m_ModelInfo[1].pos, m_ModelInfo[1].rot);
			// 初期化処理
			m_pModelObject[nCntModel].Init();

			// 親モデルを設定
			m_pModelObject[nCntModel].SetParent(NULL);

			D3DXVECTOR3 pos;
			bool bCheck = false;
			do
			{
				bCheck = false;

				pos.x = -2000.0f + (float)(rand() % (2000 * 2));
				pos.y = 0.0f;
				pos.z = -2000.0f + (float)(rand() % (2000 * 2));

				if (bCheck == false)
				{
					for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
					{
						float fRange = sqrtf(powf(pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(pos.z - m_pArea[nCntArea].pos.z, 2.0f));
						if (fRange < m_pArea[nCntArea].fRange)
						{
							bCheck = true;
							break;
						}
					}
				}
				if (bCheck == false)
				{
					for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
					{
						int nCheck = 0;
						for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
						{
							D3DXVECTOR3 vecA, vecC;
							vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % 4];
							vecC = pos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % 4];

							if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
							{// 範囲内か
								nCheck++;	// カウント
							}
						}

						if (nCheck == 4)
						{// 範囲内だったら
							bCheck = true;
							break;
						}
					}
				}
			} while (bCheck != false);

			if (pMesh != NULL)
			{
				pos.y = pMesh->GetHeight(pos);
			}

			m_pModelObject[nCntModel].Setpos(pos);
		}
	}*/

	CGame *pGame = CManager::GetGame();
	CGame::GAME game = CGame::GAME_TUTORIAL;
	if (pGame != NULL)
	{
		game = pGame->GetGame();
	}

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// エリアカウント
		//if (m_pArea[nCntArea].nArea == AREA_START)
		{
			// 敵
			if (game == CGame::GAME_PLAY_B)
				CEnemy::Create(CEnemy::TYPE_B_SLIME, m_pArea[nCntArea].pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			break;
		}
	}
}

//=============================================================================
// フィールド衝突判定処理								(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange)
{
	bool bLand = false;
	bool bAreaCheck			= false;	// エリアチェック現在
	bool bAreaCheckOld		= false;	// エリアチェック前回
	bool bRouteCheck		= false;	// ルートチェック現在
	bool bRouteCheckOld		= false;	// ルートチェック前回
	int nAreaCheckNum		= 0;		// エリアチェック番号
	int nRouteCheckNum		= 0;		// ルートチェック番号

	bool bAreaIn			= false;	// エリアの中
	int nAreaInNum			= 0;		// エリアの中番号
	bool bRouteIn			= false;	// ルートの中
	int nRouteInNum			= 0;		// ルートの中番号

	// エリアチェック
	if (bAreaCheck == false || bAreaCheckOld == false)
	{// どちらもFALSE
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// エリアカウント
			// エリアチェック
			bAreaCheck		= false;
			bAreaCheckOld	= false;
			nAreaCheckNum	= nCntArea;

			float fRange;	// 範囲
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPosOld->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPosOld->z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// 範囲内か
				bAreaCheckOld	= true;			// 範囲内
				bAreaIn			= true;			// エリア内にいる
				nAreaInNum		= nCntArea;		// エリア内の番号
			}

			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (m_pArea[nCntArea].fRange * MC_AREACOLL_NOW <= fRange)
			{// 範囲外か
				bAreaCheck = true;		// 範囲内
			}

			// どちらもTRUEなら処理を抜ける
			if (bAreaCheck == true && bAreaCheckOld == true) break;
		}
	}

	if (bRouteCheck == false || bRouteCheckOld == false)
	{// どちらもFALSE
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ルートカウント
			// ルートチェック
			bRouteCheck		= false;
			bRouteCheckOld	= false;
			nRouteCheckNum	= nCntRoute;

			int nHit	= 0;	// 現在ヒット数
			int nHitOld = 0;	// 前回ヒット数

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// 位置カウント
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// 外積計算
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = *pPos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// 範囲内か
					nHit++;		// カウント
				}

				// 外積計算
				vecAOld = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecCOld = *pPosOld - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecAOld.z * vecCOld.x) - (vecAOld.x * vecCOld.z)) <= 0)
				{// 範囲内か
					nHitOld++;	// カウント
				}
			}

			if (nHit != 4)
			{// 範囲外か
				bRouteCheck = true;			// 範囲外
			}
			if (nHitOld == 4)
			{// 範囲内か
				bRouteCheckOld	= true;			// 範囲内
				bRouteIn		= true;			// ルート内にいる
				nRouteInNum		= nCntRoute;	// ルート内の番号
			}

			// どちらもTRUEなら処理を抜ける
			if (bRouteCheck == true && bRouteCheckOld == true) break;
		}
	}

	bool bArea = false;		// エリア判定

	// エリア判定
	if (bAreaCheck == true && bAreaCheckOld == true)
	{// どちらもTRUE
		float fRouteAngle		= 0.0f;		// ルート角度
		float fRouteAngleRange	= 0.0f;		// ルート角度範囲
		float fRouteAngleL		= 0.0f;		// ルート角度L
		float fRouteAngleR		= 0.0f;		// ルート角度R
		bool bRoute				= false;	// ルートにいる

		// エリア中心からプレイヤーの角度計算
		float fAngle = atan2f(pPos->x - m_pArea[nAreaCheckNum].pos.x, pPos->z - m_pArea[nAreaCheckNum].pos.z);

		if (bRouteIn == true)
		{// ルート内にいる
			D3DXVECTOR3 posLei;
			if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posStart)
			{// スタートと同じだったら
				posLei = m_pRoute[nRouteInNum].posCol[0];	// レイ用位置

				bool bCheck = false;	// チェック
				while (true)
				{// ループ
					// 
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{
						break;
					}
					else
					{
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle)) * 1.0f;
					}
				}
			}
			else if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posEnd)
			{// エンドと同じだったら
				posLei = m_pRoute[nRouteInNum].posCol[2];	// レイ用位置

				bool bCheck = false;	// チェック
				while (true)
				{// ループ
					// 中心からの位置計算
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{// 範囲外
						break;
					}
					else
					{// 範囲内
						// レイを進める
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle + D3DX_PI), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle + D3DX_PI)) * 1.0f;
					}
				}
			}

			// ルート角度計算
			fRouteAngle = atan2f(m_pRoute[nRouteInNum].posCenter.x - m_pArea[nAreaCheckNum].pos.x, m_pRoute[nRouteInNum].posCenter.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = atan2f(posLei.x - m_pArea[nAreaCheckNum].pos.x, posLei.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = fRouteAngle - fRouteAngleRange;
			fRouteAngleRange = fabsf(fRouteAngleRange);

			if (fRouteAngleRange < -D3DX_PI)
				fRouteAngleRange += D3DX_PI * 2.0f;
			if (D3DX_PI < fRouteAngleRange)
				fRouteAngleRange -= D3DX_PI * 2.0f;

			fRouteAngleRange = fabsf(fRouteAngleRange);

			fRouteAngleL = fRouteAngle - fRouteAngleRange;
			fRouteAngleR = fRouteAngle + fRouteAngleRange;

			if (fRouteAngleL < fAngle && fAngle < fRouteAngleR)
			{
				bRoute = true;
			}
		}

		if (bRouteIn == false || (bRouteIn == true && bRoute == false))
		{// ルート内にいない、（ルート内にいる + ルート範囲外）
			pPos->x = m_pArea[nAreaCheckNum].pos.x + sinf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);
			pPos->z = m_pArea[nAreaCheckNum].pos.z + cosf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);

			pMove->x = 0.0f;
			pMove->z = 0.0f;
			bArea = true;	// エリア判定
			bLand = true;
		}
	}

	// ルート判定
	if (bArea == false && bAreaIn == false)
	{// エリア判定していない + エリアにいない
		if (bRouteCheck == true && bRouteCheckOld == true)
		{// どちらもTRUE
			// センター計算
			float fAngleCenter	= 0.0f;
			float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteCheckNum].posStart.x - m_pRoute[nRouteCheckNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteCheckNum].posStart.z - m_pRoute[nRouteCheckNum].pos[0].z, 2.0f));

			// 外積計算
			D3DXVECTOR3 vecALR, vecCLR;
			vecALR = m_pRoute[nRouteCheckNum].posStart - m_pRoute[nRouteCheckNum].posEnd;
			vecCLR = *pPos - m_pRoute[nRouteCheckNum].posEnd;

			// ルートの右側か左側か
			if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle - (D3DX_PI * 0.5f);	// 左側
			else
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle + (D3DX_PI * 0.5f);	// 右側

			// センター計算
			D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

			// センターへの角度計算
			float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

			int nHit = 0;	// ヒット数

			while (1)
			{
				nHit = 0;	// ヒット数
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
				{// 位置カウント
					// 外積計算
					D3DXVECTOR3 vecA, vecC;
					vecA = m_pRoute[nRouteCheckNum].posCol[nCntPos] - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
					vecC = *pPos - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

					if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
					{// 範囲内
						nHit++;
					}
				}

				if (nHit == 4)
				{// 範囲内
					bLand = true;
					break;		// 処理を抜ける
				}
				else
				{// 範囲外
					// 位置を更新（近づける）
					pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
					pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
				}
			}

			pMove->x = 0.0f;
			pMove->z = 0.0f;
		}
	}

	// その他の判定（抜けた場合）
	if (bArea == false && bAreaIn == false)
	{// エリア判定していない + エリアにいない
		if (bRouteIn == false)
		{// ルートにいない
			if (bRouteCheck == true && bRouteCheckOld == false)
			{// 現在が範囲外 + 前回が範囲外
				int nRouteNum = 0;		// ルート番号

				for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
				{// ルートカウント
					int nHit = 0;	// ヒット数

					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// 位置カウント
						// 外積計算
						D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
						vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// 範囲内
							nHit++;		// カウント
						}
					}

					if (nHit == 4)
					{// 範囲内
						nRouteNum = nCntRoute;		// 番号を記憶
						break;
					}
				}

				// センター計算
				float fAngleCenter	= 0.0f;
				float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteNum].posStart.x - m_pRoute[nRouteNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteNum].posStart.z - m_pRoute[nRouteNum].pos[0].z, 2.0f));

				// 外積計算
				D3DXVECTOR3 vecALR, vecCLR;
				vecALR = m_pRoute[nRouteNum].posStart - m_pRoute[nRouteNum].posEnd;
				vecCLR = *pPos - m_pRoute[nRouteNum].posEnd;

				// ルートの右側か左側か
				if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
					fAngleCenter = m_pRoute[nRouteNum].fAngle - (D3DX_PI * 0.5f);	// 左側
				else
					fAngleCenter = m_pRoute[nRouteNum].fAngle + (D3DX_PI * 0.5f);	// 右側

				// センター計算
				D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

				// センターへの角度計算
				float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

				int nHit = 0;	// ヒット数

				while (1)
				{
					nHit = 0;	// ヒット数
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// 位置カウント
						// 外積計算
						D3DXVECTOR3 vecA, vecC;
						vecA = m_pRoute[nRouteNum].posCol[nCntPos] - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// 範囲内
							nHit++;
						}
					}

					if (nHit == 4)
					{// 範囲内
						bLand = true;
						break;		// 処理を抜ける
					}
					else
					{// 範囲外
						// 位置を更新（近づける）
						pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
						pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
					}
				}

				pMove->x = 0.0f;
				pMove->z = 0.0f;
			}
		}
	}

	return bLand;
}

//=============================================================================
// エリアルート内にいるか判定処理						(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::CollisionInOut(D3DXVECTOR3 pos)
{
	bool bLand = false;

	if (bLand == false)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// エリアカウント
			float fRange;	// 範囲
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pos.z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// 範囲内か
				bLand = true;
				break;
			}
		}
	}

	if (bLand == false)
	{
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ルートカウント
			int nHit = 0;	// 現在ヒット数

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// 位置カウント
				D3DXVECTOR3 vecA, vecC;
				// 外積計算
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = pos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// 範囲内か
					nHit++;		// カウント
				}
			}

			if (nHit == 4)
			{// 範囲内か
				bLand = true;
				break;
			}
		}
	}

	return bLand;
}

//=============================================================================
// フィールドワープ衝突判定処理						(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp)
{
	int nWarpNow = 0;
	int nWarpNext = 0;
	bool bUseWarp = false;

	for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
	{
		float fRange;
		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNow = m_pWarp[nCntWarp].nArea_1;
			nWarpNext = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}

		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNext = m_pWarp[nCntWarp].nArea_1;
			nWarpNow = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}
	}

	if (bUseWarp == true)
	{
		*pPosWarp = m_pArea[nWarpNext].pos;

		if (m_pIcon != NULL)
		{// NULL以外の場合
			m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nWarpNow].pos.x, 100.0f, m_pArea[nWarpNow].pos.z));
			m_bDrawIcon = true;			// アイコン描画
		}
	}

	return bUseWarp;
}

//=============================================================================
// フィールドゴール衝突判定処理						(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::CollisionGoal(D3DXVECTOR3 *pPos)
{
	bool bUseGoal = false;;

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{
		float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			bUseGoal = true;

			if (m_pIcon != NULL)
			{// NULL以外の場合
				m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nCntArea].pos.x, 100.0f, m_pArea[nCntArea].pos.z));
				m_bDrawIcon = true;			// アイコン描画
			}

			break;
		}
	}

	return bUseGoal;
}

//=============================================================================
// フィールドカメラ衝突判定処理						(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	bool bAreaIn	= false;	// エリアの中
	int nAreaInNum	= 0;		// エリアの中番号
	bool bRouteIn	= false;	// ルートの中
	int nRouteInNum = 0;		// ルートの中番号
	bool bRouteInV	= false;	// 視点ルートの中

	// エリアチェック
	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// エリアカウント
		float fRange;	// 範囲
		fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - posR.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - posR.z, 2.0f));
		if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
		{// 範囲内か
			bAreaIn		= true;			// エリア内にいる
			nAreaInNum	= nCntArea;		// エリア内の番号
			break;
		}
	}

	// ルートチェック
	for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
	{// ルートカウント
		int nHit = 0;	// 現在ヒット数
		int nHitV = 0;	// 現在ヒット数

		for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
		{// 位置カウント
			D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
			// 外積計算
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posR - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// 範囲内か
				nHit++;		// カウント
			}

			// 外積計算
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posV - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// 範囲内か
				nHitV++;		// カウント
			}
		}

		if (nHit == 4)
		{// 範囲内か
			bRouteIn	= true;			// 範囲外
			nRouteInNum = nCntRoute;	// エリア内の番号

			if (nHitV == 4)
			{// 範囲内か
				bRouteInV = true;			// 範囲外
			}

			break;
		}


	}

	bool bArea = false;		// エリア判定

	// エリア判定
	if (bAreaIn == true)
	{// エリア内にいる
		// 距離を計算
		float fRange = sqrtf(powf(m_pArea[nAreaInNum].pos.x - posV.x, 2.0f) + powf(m_pArea[nAreaInNum].pos.z - posV.z, 2.0f));
		if (fRange < m_pArea[nAreaInNum].fRange)
		{// 範囲内
			return true;
		}
		else
		{// 範囲外
			if (bRouteInV == true)
			{// 視点がルート内にいる
				return true;
			}
			else
			{// 視点がルート内にいない
				return false;
			}
		}
	}

	// ルート判定
	if (bArea == false && bAreaIn == false)
	{// エリア判定していない + エリアにいない
		if (bRouteIn == true)
		{// ルート内にいる
			int nHit = 0;	// 現在ヒット数

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// 位置カウント
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// 外積計算
				vecA = m_pRoute[nRouteInNum].posCol[nCntPos] - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = posV - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// 範囲内か
					nHit++;		// カウント
				}
			}

			if (nHit == 4)
			{// 範囲内か
				return true;
			}
			else
			{// 範囲外
				return false;
			}
		}
	}

	return false;
}