//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "block.h"			// ブロック
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "game.h"			// ゲーム
#include "input.h"			// 入力
#include "font.h"			// フォント
#include "camera.h"			// カメラ
#include "meshField.h"		// メッシュフィールド
#include "shadow.h"			// 影

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME_0	"data\\MODEL\\Object\\box.x"		// 通常ブロック
#define MODEL_NAME_1	"data\\MODEL\\Object\\PBrock.x"		// 壊れるブロック
#define MODEL_NAME_2	"data\\MODEL\\Object\\rock.x"		// 木
#define MODEL_NAME_3	"data\\MODEL\\Object\\obj_tree.x"	// 背景用木
#define MODEL_NAME_4	"data\\MODEL\\Object\\Airblock.x"	// 空中床(正式採用視野)
#define MODEL_NAME_5	"data\\MODEL\\Object\\Airblock2.x"	// 空中床
#define MODEL_NAME_6	"data\\MODEL\\Object\\Slipblock.x"	// 滑る床(調整中)
#define MODEL_NAME_7	"data\\MODEL\\Object\\log.x"		// 丸太(調整中)
#define MODEL_NAME_8	"data\\MODEL\\Object\\seesaw.x"		// シーソー(調整中)
#define MODEL_NAME_9	"data\\MODEL\\Object\\ivy.x"		// ツタ
#define MODEL_NAME_10	"data\\MODEL\\Object\\obj_ivy.x"	// 背景用ツタ
#define MODEL_NAME_11	"data\\MODEL\\Object\\box3.x"		// 通常ブロック4つ分
#define MODEL_NAME_12	"data\\MODEL\\Object\\Wood.x"		// 背景木2
#define MODEL_NAME_13	"data\\MODEL\\Object\\Wood2.x"		// 背景木3
#define MODEL_NAME_14	"data\\MODEL\\Object\\Wood3.x"		// 背景木4
#define MODEL_NAME_15	"data\\MODEL\\Object\\Wood4.x"		// 背景木5
#define MODEL_NAME_16	"data\\MODEL\\Object\\grass.x"		// 草
#define MODEL_NAME_17	"data\\MODEL\\Object\\goal.x"		// ゴール
//#define MODEL_NAME_11	"data\\MODEL\\Object\\stone.x"		// 岩(?)
//#define MODEL_NAME_12	"data\\MODEL\\Object\\rock.x"		// 岩
//#define MODEL_NAME_13	"data\\MODEL\\Object\\rock2.x"		// 岩2
//#define MODEL_NAME_14	"data\\MODEL\\Object\\rock3.x"		// 岩3
//#define MODEL_NAME_15	"data\\MODEL\\Object\\rock4.x"		// 岩4
//#define MODEL_NAME_16	"data\\MODEL\\Object\\rock5.x"		// 岩5

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CBlock::m_pTexture[MODELTYPE_MAX] = {};	// テクスチャ情報へのポインタ
LPD3DXMESH				CBlock::m_pMesh[MODELTYPE_MAX] = {};		// メッシュ情報へのポインタ
LPD3DXBUFFER			CBlock::m_pBuffMat[MODELTYPE_MAX] = {};		// マテリアル情報へのポインタ
DWORD					CBlock::m_nNumMat[MODELTYPE_MAX] = {};		// マテリアル情報の数

//=============================================================================
// コンストラクタ									(public)	*** CBlock ***
//=============================================================================
CBlock::CBlock() : CSceneX(CScene::PRIORITY_3, CScene::OBJTYPE_BLOCK)
{
	m_modelType = MODELTYPE_BOX_R;					// モデル種類
	m_colType = COLTYPE_NONE;						// 衝突種類
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量

	// 衝突場所
	for (int nCount = 0; nCount < HIT_MAX; nCount++) m_bHit[nCount] = false;

	m_shadowHeight = 0.0f;							// 影高さ
}

//=============================================================================
// デストラクタ										(public)	*** CBlock ***
//=============================================================================
CBlock::~CBlock()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CBlock::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	char cName[256];		// モデル名

	for (int nCntParnet = 0; nCntParnet < MODELTYPE_MAX; nCntParnet++)
	{// モデル種類カウント
		switch (nCntParnet)
		{// カウント番号
		case MODELTYPE_BOX_R:		// 岩
			wsprintf(cName, MODEL_NAME_0);
			break;
		case MODELTYPE_BOX_W_1:		// 木箱１
			wsprintf(cName, MODEL_NAME_2);
			break;
		case MODELTYPE_BOX_W_2:		// 木箱２
			wsprintf(cName, MODEL_NAME_3);
			break;
		case MODELTYPE_SKY:			// 空中
			wsprintf(cName, MODEL_NAME_4);
			break;
		case MODELTYPE_ICE:			// 氷ブロック
			wsprintf(cName, MODEL_NAME_6);
			break;
		case MODELTYPE_LOG:			// 丸太
			wsprintf(cName, MODEL_NAME_7);
			break;
		case MODELTYPE_SEESAW:		// シーソー
			wsprintf(cName, MODEL_NAME_8);
			break;
		case MODELTYPE_IVY:			// つた１
			wsprintf(cName, MODEL_NAME_9);
			break;
		case MODELTYPE_IVY_2:		// つた２
			wsprintf(cName, MODEL_NAME_10);
			break;
		case MODELTYPE_BOX_W_3:		// 木箱３
			wsprintf(cName, MODEL_NAME_11);
			break;
		case MODELTYPE_TREE:		// 木１
			wsprintf(cName, MODEL_NAME_12);
			break;
		case MODELTYPE_TREE2:		// 木２
			wsprintf(cName, MODEL_NAME_13);
			break;
		case MODELTYPE_TREE3:		// 木３
			wsprintf(cName, MODEL_NAME_14);
			break;
		case MODELTYPE_TREE4:		// 木４
			wsprintf(cName, MODEL_NAME_15);
			break;
		case MODELTYPE_GRASS:		// 草
			wsprintf(cName, MODEL_NAME_16);
			break;
		case MODELTYPE_GOAL:		// ゴール
			wsprintf(cName, MODEL_NAME_17);
			break;
		case MODELTYPE_PBROCK:		// 破壊可能ブロック
			wsprintf(cName, MODEL_NAME_1);
			break;
		default:
			wsprintf(cName, MODEL_NAME_0);
			break;
		}

		// Xファイルの読み込み
		D3DXLoadMeshFromX(cName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCntParnet], NULL, &m_nNumMat[nCntParnet], &m_pMesh[nCntParnet]);

		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCntParnet]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCntParnet]];
		m_pTexture[nCntParnet] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntParnet]];

		for (DWORD tex = 0; tex < m_nNumMat[nCntParnet]; tex++)
		{// マテリアル数カウント
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
					//MessageBox(NULL, "テクスチャの読み込みに失敗しました(block)", NULL, MB_OK);
				}
			}
		}

		delete[] matDef;
		matDef = NULL;
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CBlock::Unload(void)
{
	for (int nCntParnet = 0; nCntParnet < MODELTYPE_MAX; nCntParnet++)
	{// モデル種類カウント
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
}

//=============================================================================
// 確保処理											(public)	*** CBlock ***
//=============================================================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, MODELTYPE modelType, COLTYPE coltype)
{
	CBlock *pBlock = NULL;			// ポインタ

	if (pBlock == NULL)
	{// NULLの場合
		pBlock = new CBlock;			// メモリ確保

		if (pBlock != NULL)
		{// NULL以外の場合
			pBlock->Init(pos, modelType, coltype);		// 初期化処理
		}
	}

	return pBlock;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CBlock ***
//=============================================================================
HRESULT CBlock::Init(D3DXVECTOR3 pos, MODELTYPE modelType, COLTYPE coltype)
{
	// 初期値設定
	CSceneX::SetInitAll(m_pTexture[modelType], m_pMesh[modelType], m_pBuffMat[modelType], m_nNumMat[modelType], pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// 初期化処理
	CSceneX::Init();

	// 位置設定
	CSceneX::Setpos(CSceneX::Getpos() + D3DXVECTOR3(0.0f, -CSceneX::Getsize(1).y, 0.0f));

	if (coltype == COLTYPE_FALL)
	{// 倒れる判定
		CSceneX::Setrot(CSceneX::Getrot() + D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));	// 回転
	}
	else if (coltype == COLTYPE_SEESAW)
	{// シーソー判定
		CSceneX::Setrot(D3DXVECTOR3(0.0f, 0.0f, 0.5f));		// 初期角度
	}
	else if (coltype == COLTYPE_NONE)
	{// 判定なし
		if (modelType == MODELTYPE_BOX_W_2 || modelType == MODELTYPE_IVY_2 || (MODELTYPE_TREE <= modelType && modelType <= MODELTYPE_GRASS))
		{// 背景モデル
			CSceneX::Setrot(D3DXVECTOR3(0.0f, (-314.0f + (float)(rand() % 628)) * 0.01f, 0.0f));	// 角度設定
		}
	}

	m_modelType = modelType;	// モデル種類
	m_colType = coltype;		// 衝突種類

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CBlock ***
//=============================================================================
void CBlock::Uninit(void)
{
	CSceneX::Uninit();			// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CBlock ***
//=============================================================================
void CBlock::Update(void)
{
	D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// 視点取得
	CManager::MODE pMode = CManager::GetMode();						// モード取得
	
	// 衝突場所の初期化
	for (int nCount = 0; nCount < HIT_MAX; nCount++) m_bHit[nCount] = false;

	D3DXVECTOR3 pos = CSceneX::Getpos();		// 位置取得
	D3DXVECTOR3 rot = CSceneX::Getrot();		// 角度取得
	D3DXVECTOR3 sizeMax = CSceneX::Getsize(0);	// 大きさ取得（最大）
	D3DXVECTOR3 sizeMin = CSceneX::Getsize(1);	// 大きさ取得（最小）

	int nMeshNum = 0;		// メッシュフィールド番号
	bool bMesh = false;		// メッシュフィールド有無

	if (pMode == CManager::MODE_GAME)
	{// ゲームモードのみ
		for (int nCount = 0; nCount < CGame::m_MeshFieldMax; nCount++)
		{// フィールドカウント
			if (CGame::GetMeshField(nCount) != NULL)
			{// NULL以外の場合
				if (CGame::GetMeshField(nCount)->ColInMesh(pos + sizeMax))
				{// 判定内にある
					nMeshNum = nCount;	// メッシュ番号
					bMesh = true;		// メッシュあり
					break;
				}
				else if (CGame::GetMeshField(nCount)->ColInMesh(pos))
				{// 判定内にある
					nMeshNum = nCount;	// メッシュ番号
					bMesh = true;		// メッシュあり
					break;
				}
				else if (CGame::GetMeshField(nCount)->ColInMesh(pos + sizeMin))
				{// 判定内にある
					nMeshNum = nCount;	// メッシュ番号
					bMesh = true;		// メッシュあり
					break;
				}
			}
		}

		// フィールド高さ取得
		float fHeight = CGame::GetMeshField(nMeshNum)->GetHeight(pos);

		// 位置を記憶する
		m_posOld = pos;

		// 移動量加算
		pos += m_move;

		// 判定チェック
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_3);		// シーン取得
		CScene *pSceneNext = NULL;									// 次シーン

		while (pScene != NULL)
		{// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();		// 次シーンを記憶
			if (pScene != this && pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// 自分以外、ブロックの場合
				CBlock *pBlock = (CBlock*)pScene;		// ブロックにキャスト
				if (pBlock->OtherBlockCollision(&pos, &m_posOld, &m_move, CSceneX::Getsize(0), CSceneX::Getsize(1)))
				{// 判定チェック
					if (m_move.x != 0.0f)	// 移動量が０じゃない
						pBlock->Setpos(pBlock->Getpos() + m_move);	// 位置
				}
			}
			pScene = pSceneNext;			// 次シーン
		}

		if (m_colType == COLTYPE_FALL)
		{// 倒れる
			if (rot.z <= D3DX_PI * 0.3f)
			{// 一定角度以下
				if (0.0f < rot.z)
				{// ０以上
					rot.z += -0.03f;		// 角度加算
				}
				else
				{// その他
					rot.z = 0.0f;			// 角度設定
				}
			}
			else
			{// その他
				if (rot.z < D3DX_PI * 0.5f)
				{// 一定角度以下
					rot.z += 0.003f;		// 角度加算
				}
				else
				{// その他
					rot.z = D3DX_PI * 0.5f;	// 角度設定
				}
			}
		}

		if (pos.y + sizeMin.y < fHeight && bMesh == true)
		{// 地面より下に行ったら
			pos.y = fHeight - sizeMin.y;	// 高さ調整
			m_move.y = 0.0f;				// 移動量を消す
		}

		if ((fHeight < pos.y + sizeMin.y || bMesh == false) && m_colType == COLTYPE_PUSH)
		{// 重力
			m_move.y -= 0.7f;
		}
	}

	// 移動量（X）を０へ
	m_move.x = 0.0f;

	// 位置設定
	CSceneX::Setpos(pos);
	// 角度設定
	CSceneX::Setrot(rot);

	if (pos.y < -100.0f || pos.x + sizeMax.x < posVDest.x - 800.0f)
	{// 範囲チェック
		Uninit();			// 終了処理
	}
}

//=============================================================================
// 描画処理											(public)	*** CBlock ***
//=============================================================================
void CBlock::Draw(void)
{
	CSceneX::Draw();		// 描画処理

	// 影
	CShadow::SetShadow(CSceneX::GetmtxWorld(), m_pMesh[m_modelType], m_nNumMat[m_modelType], D3DXVECTOR3(CSceneX::Getpos().x, m_shadowHeight + 1.0f, CSceneX::Getpos().y));
}

//=============================================================================
// 当たり判定処理											(public)	*** CBlock ***
//=============================================================================
void CBlock::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float *fDownSpeed, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	// 衝突場所の初期化
	for (int nCount = 0; nCount < HIT_MAX; nCount++) m_bHit[nCount] = false;

	D3DXVECTOR3 BlockPos = CSceneX::Getpos();			// 位置取得
	D3DXVECTOR3 BlockSizeMax = CSceneX::Getsize(0);		// 大きさ取得（最大）
	D3DXVECTOR3 BlockSizeMin = CSceneX::Getsize(1);		// 大きさ取得（最小）

	if (m_colType != COLTYPE_NONE)
	{// 判定なし以外
		if (m_colType != COLTYPE_SEESAW && m_colType != COLTYPE_CLIMB && m_colType != COLTYPE_FALL && m_colType != COLTYPE_WALLJUMP)
		{// 指定の判定以外
			if ((BlockPos.x + BlockSizeMin.x) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.x))
			{// X範囲確認
				if ((pos->y + sizeMin.y) <= (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMax.y) <= (posOld->y + sizeMin.y))
				{// 上からの当たり判定
					m_bHit[0] = true;		// 上当たった

					// 位置調整、移動量（Y）を０へ
					pos->y = BlockPos.y + BlockSizeMax.y + (-sizeMin.y);
					move->y = 0.0f;

					switch (m_colType)
					{// 衝突種類
					case COLTYPE_BOX:		// 矩形
					case COLTYPE_PUSH:		// 押す
					case COLTYPE_BREAK:		// 破壊
					case COLTYPE_SKY:		// 空中
					case COLTYPE_ICE:		// 氷
						break;
					}
				}
				else if ((BlockPos.y + BlockSizeMin.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (BlockPos.y + BlockSizeMin.y))
				{// 下からの当たり判定
					if (m_colType != COLTYPE_SKY)
					{// 空中以外
						m_bHit[1] = true;	// 下当たった

						// 位置調整、移動量（Y）を０へ
						pos->y = BlockPos.y + BlockSizeMin.y + (-sizeMax.y);
						move->y = 0.0f;
					}
				}
			}
			if ((pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y))
			{// Y範囲確認
				if ((BlockPos.x + BlockSizeMin.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (BlockPos.x + BlockSizeMin.x))
				{// 左からの当たり判定
					m_bHit[2] = true;		// 左当たった

					if (m_colType != COLTYPE_SKY)
					{// 空中以外
						// 位置調整
						pos->x = BlockPos.x + BlockSizeMin.x + (-sizeMax.x);
					}

					// 移動量（X）を調整
					move->x -= move->x / 8;

					switch (m_colType)
					{// 衝突種類
					case COLTYPE_BOX:		// 矩形
						break;
					case COLTYPE_PUSH:		// 押す
						pos->x -= 0.1f;		// 進める
						m_move.x = move->x;	// 移動量設定
						break;
					case COLTYPE_BREAK:		// 破壊
					case COLTYPE_SKY:		// 空中
					case COLTYPE_ICE:		// 氷
						break;
					}
				}
				else if ((pos->x + sizeMin.x) <= (BlockPos.x + BlockSizeMax.x) && (BlockPos.x + BlockSizeMax.x) <= (posOld->x + sizeMin.x))
				{// 右からの当たり判定
					m_bHit[3] = true;		// 右当たった

					if (m_colType != COLTYPE_SKY)
					{// 空中以外
						// 位置調整
						pos->x = BlockPos.x + BlockSizeMax.x + (-sizeMin.x);
					}

					// 移動量（X）を調整
					move->x -= move->x / 8;

					switch (m_colType)
					{// 衝突種類
					case COLTYPE_BOX:		// 矩形
						break;
					case COLTYPE_PUSH:		// 押す
						pos->x += 0.1f;		// 進める
						m_move.x = move->x;	// 移動量設定
						break;
					case COLTYPE_BREAK:		// 破壊
					case COLTYPE_SKY:		// 空中
					case COLTYPE_ICE:		// 氷
						break;
					}
				}
			}
		}
		else
		{// その他
			if (m_colType == COLTYPE_FALL)
			{// 倒れる
				if ((BlockPos.x + BlockSizeMin.x) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.x))
				{// X範囲確認
					if ((pos->y + sizeMin.y) <= (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMax.y) <= (posOld->y + sizeMin.y))
					{// 上からの当たり判定
						m_bHit[0] = true;	// 上当たった

						// 位置調整、移動量（X）を０へ
						pos->y = BlockPos.y + BlockSizeMax.y + (-sizeMin.y);
						move->y = 0.0f;
					}
					else if ((BlockPos.y + BlockSizeMin.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (BlockPos.y + BlockSizeMin.y))
					{// 下からの当たり判定
						if (m_colType != COLTYPE_SKY)
						{// 空中以外
							m_bHit[1] = true;		// 下当たった

							// 位置調整、移動量（X）を０へ
							pos->y = BlockPos.y + BlockSizeMin.y + (-sizeMax.y);
							move->y = 0.0f;
						}
					}
				}
				if (CSceneX::Getrot().z == 0.0f)
				{// 角度が０だったら
					if ((pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y))
					{// Y範囲確認
						if ((BlockPos.x + BlockSizeMin.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (BlockPos.x + BlockSizeMin.x))
						{// 左からの当たり判定
							m_bHit[2] = true;		// 左当たった 

							// 位置調整、移動量（X）を調整
							pos->x = BlockPos.x + BlockSizeMin.x + (-sizeMax.x);
							move->x /= 2;
						}
						else if ((pos->x + sizeMin.x) <= (BlockPos.x + BlockSizeMax.x) && (BlockPos.x + BlockSizeMax.x) <= (posOld->x + sizeMin.x))
						{// 右からの当たり判定
							m_bHit[3] = true;		// 右当たった

							// 位置調整、移動量（X）を調整
							pos->x = BlockPos.x + BlockSizeMax.x + (-sizeMin.x);
							move->x /= 2;
						}
					}
				}
				else
				{// その他
					if ((pos->y + sizeMax.y) < (BlockPos.y + BlockSizeMax.y) + 100.0f)
					{// Y範囲確認
						if ((BlockPos.x + BlockSizeMin.x - BlockSizeMax.y) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (BlockPos.x + BlockSizeMin.x - BlockSizeMax.y))
						{// 左からの当たり判定
							m_bHit[2] = true;		// 左当たった

							// 位置調整、移動量（X）を調整
							pos->x = BlockPos.x + BlockSizeMin.x - BlockSizeMax.y + (-sizeMax.x);
							move->x /= 2;
						}
						else if ((pos->x + sizeMin.x) <= (BlockPos.x + BlockSizeMax.x) && (BlockPos.x + BlockSizeMax.x) <= (posOld->x + sizeMin.x))
						{// 右からの当たり判定
							m_bHit[3] = true;		// 右当たった
						}
					}
				}
			}
			else if (m_colType == COLTYPE_SEESAW)
			{// シーソー
				CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_TEST);		// フォント取得

				D3DXVECTOR3 VtxPos[6];		// シーソー頂点位置計算用
				float frot;					// 中心から頂点への角度
				frot = -atan2f(BlockSizeMin.x + BlockPos.x, 0.0f);			// 角度計算

				// 頂点位置計算（左２点）
				VtxPos[0].x = BlockPos.x + sinf(frot + CSceneX::Getrot().z) * ((-BlockSizeMin.x + BlockPos.x - BlockPos.x)* 1.0f);
				VtxPos[0].y = cosf(frot + CSceneX::Getrot().z) * -((-BlockSizeMin.x + BlockPos.x - BlockPos.x) * 1.0f);
				VtxPos[0].z = BlockSizeMax.z;
				VtxPos[3].x = BlockPos.x + sinf(frot + CSceneX::Getrot().z) * ((-BlockSizeMin.x + BlockPos.x - BlockPos.x) * 1.0f);
				VtxPos[3].y = cosf(frot + CSceneX::Getrot().z) * -((-BlockSizeMin.x + BlockPos.x - BlockPos.x) * 1.0f);
				VtxPos[3].z = BlockSizeMin.z;

				// 頂点位置計算（中央２点）
				VtxPos[1].x = BlockPos.x;
				VtxPos[1].y = 0.0f;
				VtxPos[1].z = BlockSizeMax.z;
				VtxPos[4].x = BlockPos.x;
				VtxPos[4].y = 0.0f;
				VtxPos[4].z = BlockSizeMin.z;

				frot = -atan2f(BlockSizeMax.x - BlockPos.x, 0.0f);			// 角度計算

				// 頂点位置計算（右２点）
				VtxPos[2].x = BlockPos.x + sinf(frot + CSceneX::Getrot().z) * (BlockSizeMax.x + BlockPos.x - BlockPos.x) * 1.0f;
				VtxPos[2].y = cosf(frot + CSceneX::Getrot().z) * -(BlockSizeMax.x + BlockPos.x - BlockPos.x) * 1.0f;
				VtxPos[2].z = BlockSizeMax.z;
				VtxPos[5].x = BlockPos.x + sinf(frot + CSceneX::Getrot().z) * ((BlockSizeMax.x + BlockPos.x - BlockPos.x) * 1.0f);
				VtxPos[5].y = cosf(frot + CSceneX::Getrot().z) * -(BlockSizeMax.x + BlockPos.x - BlockPos.x) * 1.0f;
				VtxPos[5].z = BlockSizeMin.z;

				// フォント文字設定
				pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", (BlockSizeMax.x + BlockPos.x - BlockPos.x));
				pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", (-BlockSizeMin.x + BlockPos.x - BlockPos.x));

				pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f, %.2f, %.2f\n", VtxPos[0].x, VtxPos[0].y, VtxPos[0].z);
				pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f, %.2f, %.2f\n\n", VtxPos[3].x, VtxPos[3].y, VtxPos[3].z);

				pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f, %.2f, %.2f\n", VtxPos[1].x, VtxPos[1].y, VtxPos[1].z);
				pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f, %.2f, %.2f\n\n", VtxPos[4].x, VtxPos[4].y, VtxPos[4].z);

				pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f, %.2f, %.2f\n", VtxPos[2].x, VtxPos[2].y, VtxPos[2].z);
				pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f, %.2f, %.2f\n\n", VtxPos[5].x, VtxPos[5].y, VtxPos[5].z);

				D3DXVECTOR3 vecA, vecC;		// ベクトル計算用

				// ベクトル計算
				vecA = VtxPos[0] - VtxPos[3];
				vecC = *pos - VtxPos[3];

				if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
				{// 範囲内（右側）にいることを確認
				 // ベクトル計算
					vecA = VtxPos[5] - VtxPos[2];
					vecC = *pos - VtxPos[2];

					if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
					{// 範囲内（右側）にいることを確認
					 // ベクトル計算
						vecA = VtxPos[1] - VtxPos[4];
						vecC = *pos - VtxPos[4];

						//pFont->SetMessage(CFont::SETTYPE_ADD, "true\n");

						if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
						{// 範囲内（右側）にいることを確認
						 //pFont->SetMessage(CFont::SETTYPE_ADD, "right\n");

							D3DXVECTOR3 nor, vec[2];	// 内積計算用

							//
							vec[0] = VtxPos[1] - VtxPos[4];
							vec[1] = VtxPos[5] - VtxPos[4];

							// 内積計算
							D3DXVec3Cross(&nor, &vec[0], &vec[1]);
							// 正規化
							D3DXVec3Normalize(&nor, &nor);

							// ベクトル計算
							D3DXVECTOR3 vecP;
							float fHeight, fHeightOld;

							// 高さ計算
							vecP = *pos - VtxPos[4];
							fHeight = (((nor.x * vecP.x) + (nor.z * vecP.z)) / -nor.y);

							vecP = *posOld - VtxPos[4];
							fHeightOld = (((nor.x * vecP.x) + (nor.z * vecP.z)) / -nor.y);

							if ((pos->y + sizeMin.y) < (fHeight + BlockSizeMax.y + BlockPos.y/*- sizeMin.y*/) /*&& (fHeightOld - sizeMin.y) <= (posOld->y + sizeMin.y)*/)
							{// 上からの当たり判定
								m_bHit[0] = true;		// 上当たった

								// 位置調整、移動量（Y）を０へ
								pos->y = fHeight + (-sizeMin.y) + BlockSizeMax.y + BlockPos.y;
								move->y = 0.0f;

								//pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", fHeight);

								if (0.0f < CSceneX::Getrot().z)
								{// 一定の角度以下
									// 乗っている位置と中心から距離の比率を計算
									float fDistance = (pos->x - BlockPos.x) / ((BlockPos.x + BlockSizeMin.x) - BlockPos.x);

									// 角度設定
									CSceneX::Setrot(CSceneX::Getrot() + D3DXVECTOR3(0.0f, 0.0f, -0.008f * -fDistance));
								}
							}
						}
						else
						{
							//pFont->SetMessage(CFont::SETTYPE_ADD, "left\n");

							// ベクトル計算
							D3DXVECTOR3 vecP = *pos - VtxPos[1];
							float fHeight;

							D3DXVECTOR3 nor, vec[2];	// 内積計算用

							//
							vec[0] = VtxPos[4] - VtxPos[1];
							vec[1] = VtxPos[0] - VtxPos[1];

							// 内積計算
							D3DXVec3Cross(&nor, &vec[0], &vec[1]);
							// 正規化
							D3DXVec3Normalize(&nor, &nor);

							// 高さ計算
							fHeight = (((nor.x * vecP.x) + (nor.z * vecP.z)) / -nor.y);

							if ((pos->y + sizeMin.y) < (fHeight + BlockSizeMax.y + BlockPos.y)/* && (fHeight) <= (posOld->y + sizeMin.y)*/)
							{// 上からの当たり判定
								m_bHit[0] = true;		// 上当たった

								// 位置調整、移動量（Y）を０へ
								pos->y = fHeight + (-sizeMin.y) + BlockSizeMax.y + BlockPos.y;
								move->y = 0.0f;

								//pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", fHeight);

								/*if (CSceneX::Getrot().z < 0.3f)
								{
								float fDistance = (pos->x - BlockPos.x) / ((BlockPos.x + BlockSizeMin.x) - BlockPos.x);

								CSceneX::Setrot(CSceneX::Getrot() + D3DXVECTOR3(0.0f, 0.0f, 0.005f * fDistance));
								}*/
							}
						}
					}
				}

				if (m_bHit[0] == false && m_bHit[1] == false)
				{// 上、下に当たっていない
					D3DXVECTOR3 RL;

					if (pos->x < BlockPos.x) RL = VtxPos[3];
					else RL = VtxPos[5];

					if ((pos->y + sizeMin.y) < (RL.y + BlockSizeMax.y + BlockPos.y))
					{// Y範囲確認
						if ((VtxPos[3].x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (VtxPos[3].x))
						{// 左からの当たり判定
							m_bHit[2] = true;	// 左当たった

							// 位置調整、移動量（X）を０へ
							pos->x = VtxPos[3].x + (-sizeMax.x);
							move->x = 0.0f;
						}
						else if ((pos->x + sizeMin.x) <= (VtxPos[5].x) && (VtxPos[5].x) <= (posOld->x + sizeMin.x))
						{// 右からの当たり判定
							m_bHit[3] = true;	// 右当たった

							// 位置調整、移動量（X）を０へ
							pos->x = VtxPos[5].x + (-sizeMin.x);
							move->x = 0.0f;
						}
					}
				}
			}
			else if (m_colType == COLTYPE_CLIMB)
			{// 登る
				if ((BlockPos.x + BlockSizeMin.x) <= (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) <= (BlockPos.x + BlockSizeMax.x))
				{// X範囲確認
					if ((pos->y + sizeMin.y) <= (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMax.y) <= (posOld->y + sizeMin.y))
					{// 上からの当たり判定
						m_bHit[0] = true;		// 上当たった

						// 位置調整、移動量（Y）を０へ
						pos->y = BlockPos.y + BlockSizeMax.y + (-sizeMin.y);
						move->y = 0.0f;

					}
					else if ((BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) < (BlockPos.y + BlockSizeMin.y))
					{// 下からの当たり判定
						m_bHit[1] = true;		// 下当たった

						// 位置調整、移動量（Y）を０へ
						pos->y = BlockPos.y + BlockSizeMin.y + (-sizeMax.y);
						move->y = 0.0f;
					}
				}

				if ((pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMin.y) <= (pos->y + sizeMax.y))
				{// Y範囲確認
					if ((BlockPos.x + BlockSizeMin.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (BlockPos.x + BlockSizeMin.x))
					{// 左からの当たり判定
						m_bHit[2] = true;		// 左当たった

						// 位置調整、移動量（X）を０へ
						pos->x = BlockPos.x + BlockSizeMin.x + (-sizeMax.x);
						move->x = 0.0f;
					}
					else if ((pos->x + sizeMin.x) <= (BlockPos.x + BlockSizeMax.x) && (BlockPos.x + BlockSizeMax.x) <= (posOld->x + sizeMin.x))
					{// 右からの当たり判定
						m_bHit[3] = true;		// 右当たった

						// 位置調整、移動量（X）を０へ
						pos->x = BlockPos.x + BlockSizeMax.x + (-sizeMin.x);
						move->x = 0.0f;
					}
				}
			}
			else if (m_colType == COLTYPE_WALLJUMP)
			{// 壁キック
				if ((BlockPos.x + BlockSizeMin.x) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.x))
				{// X範囲確認
					if ((pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMax.y) < (posOld->y + sizeMin.y))
					{// 上からの当たり判定
						m_bHit[0] = true;		// 上当たった

						// 位置調整、移動量（Y）を０へ
						pos->y = BlockPos.y + BlockSizeMax.y + (-sizeMin.y + 0.0001f);
						move->y = 0.0f;

					}
					else if ((BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) < (BlockPos.y + BlockSizeMin.y))
					{// 下からの当たり判定
						m_bHit[1] = true;		// 下当たった

						// 位置調整、移動量（Y）を０へ
						pos->y = BlockPos.y + BlockSizeMin.y + (-sizeMax.y);
						move->y = 0.0f;
					}
				}

				if ((pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMin.y) <= (pos->y + sizeMax.y))
				{// Y範囲確認
					if ((BlockPos.x + BlockSizeMin.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (BlockPos.x + BlockSizeMin.x))
					{// 左からの当たり判定
						m_bHit[2] = true;		// 左当たった

						// 位置調整、移動量（X）を０へ
						pos->x = BlockPos.x + BlockSizeMin.x + (-sizeMax.x);
						move->x = 0.0f;
					}
					else if ((pos->x + sizeMin.x) <= (BlockPos.x + BlockSizeMax.x) && (BlockPos.x + BlockSizeMax.x) <= (posOld->x + sizeMin.x))
					{// 右からの当たり判定
						m_bHit[3] = true;		// 右当たった

						// 位置調整、移動量（X）を０へ
						pos->x = BlockPos.x + BlockSizeMax.x + (-sizeMin.x);
						move->x = 0.0f;
					}
				}
			}
		}
	}
}

//=============================================================================
// 当たり判定処理											(public)	*** CBlock ***
//=============================================================================
bool CBlock::OtherBlockCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	bool bLand = false;		// 当たったか

	D3DXVECTOR3 BlockPos = CSceneX::Getpos();
	D3DXVECTOR3 BlockSizeMax = CSceneX::Getsize(0);
	D3DXVECTOR3 BlockSizeMin = CSceneX::Getsize(1);

	if (m_colType != COLTYPE_NONE)
	{
		if ((BlockPos.x + BlockSizeMin.x) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.x))
		{// X範囲確認
			if ((pos->y + sizeMin.y) <= (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMax.y) <= (posOld->y + sizeMin.y))
			{// 上からの当たり判定
				//bLand = true;

				// 位置調整、移動量（Y）を０へ
				pos->y = BlockPos.y + BlockSizeMax.y + (-sizeMin.y) + 0.1f;
				move->y = 0.0f;
			}
			else if ((BlockPos.y + BlockSizeMin.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (BlockPos.y + BlockSizeMin.y))
			{// 下からの当たり判定
				bLand = true;

				// 位置調整、移動量（Y）を０へ
				pos->y = BlockPos.y + BlockSizeMin.y + (-sizeMax.y) - 0.1f;
				move->y = 0.0f;
			}
		}

		if ((pos->y + sizeMin.y) <= (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMin.y) <= (pos->y + sizeMax.y))
		{// Y範囲確認
			if ((BlockPos.x + BlockSizeMin.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (BlockPos.x + BlockSizeMin.x))
			{// 左からの当たり判定
			 //bLand = true;

				// 位置調整、移動量（X）を０へ
				pos->x = BlockPos.x + BlockSizeMin.x + (-sizeMax.x);
				move->x = 0.0f;
			}
			else if ((pos->x + sizeMin.x) <= (BlockPos.x + BlockSizeMax.x) && (BlockPos.x + BlockSizeMax.x) <= (posOld->x + sizeMin.x))
			{// 右からの当たり判定
			 //bLand = true;
				// 位置調整、移動量（X）を０へ
				pos->x = BlockPos.x + BlockSizeMax.x + (-sizeMin.x);
				move->x = 0.0f;
			}
		}
	}

	return bLand;
}