//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "item.h"
#include "input.h"
#include "camera.h"
#include "itemCnt.h"
#include "flontpolygon.h"
#include "itemfont.h"
#include "xplayer.h"
#include "storyfont.h"
#include "mark.h"
#include "sound.h"
#include "effect.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_XFAILE		(4)								//読みこむｘファイルの最大数
#define MAX_XOBJECT		(10)							//ｘファイルの最大使用数
#define ITEM_SPEED		(2.0f)							//モデルの移動量
#define MAX_ITEM		(5)								//モデルの最大数
#define MAX_TEXTURE		(3)								//テクスチャの最大数
#define ITEM_CIRCLE		(60.0f)							//アイテムの範囲設定

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Item				g_Item[MAX_XFAILE][MAX_XOBJECT];	//モデルの情報
int					g_nGetItem;							//アイテムの総数

//=============================================================================
// 初期化処理
//=============================================================================
void InitItem(void)
{
	//変数宣言
	char XFileName[MAX_XFAILE][48];
	int nCntXFile;
	int nCntItem;
	int		nNumVtx = 0;	//頂点数
	DWORD	sizeFVF;	//頂点フォーマットのサイズ
	BYTE *	pVtxBuff;	//頂点バッファへのポインタ

	//ファイル名の読みこみ
//	wsprintf(&XFileName[0][0], "data\\MODEL\\Map\\box000.x");	//アイテム
//	wsprintf(&XFileName[1][0], "data\\MODEL\\Map\\kanban.x");	//車

	wsprintf(&XFileName[0][0], "data\\MODEL\\Map\\kanban.x");	//歯車1-看板
	wsprintf(&XFileName[1][0], "data\\MODEL\\Map\\car.x");		//歯車2-車
	wsprintf(&XFileName[2][0], "data\\MODEL\\Map\\ueki_pink.x");//歯車3-植木鉢
	wsprintf(&XFileName[3][0], "data\\MODEL\\Map\\denwabox.x");	//歯車4-電話ボックス

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntXFile = 0; nCntXFile < MAX_XFAILE; nCntXFile++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(&XFileName[nCntXFile][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Item[nCntXFile][0].pBuffMatItem,
			NULL,
			&g_Item[nCntXFile][0].nNumMatItem,
			&g_Item[nCntXFile][0].pMeshItem);

		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)g_Item[nCntXFile][0].pBuffMatItem->GetBufferPointer();
		matDef = new D3DMATERIAL9[g_Item[nCntXFile][0].nNumMatItem];
		g_Item[nCntXFile][0].pTextureItem = new LPDIRECT3DTEXTURE9[g_Item[nCntXFile][0].nNumMatItem];

		for (DWORD tex = 0; tex < g_Item[nCntXFile][0].nNumMatItem; tex++)
		{
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			g_Item[nCntXFile][0].pTextureItem[tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&g_Item[nCntXFile][0].pTextureItem[tex])))
				{
					MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}
	}

	//変数の初期化
	g_nGetItem = 0;

	// 構造体の初期設定
	for (nCntXFile = 0; nCntXFile < MAX_XFAILE; nCntXFile++)
	{
		g_Item[nCntXFile][0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntXFile][0].OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntXFile][0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntXFile][0].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntXFile][0].CircleMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntXFile][0].CircleMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntXFile][0].vtxMinItem = D3DXVECTOR3(1000.0f, 0.0f, 1000.0f);
		g_Item[nCntXFile][0].vtxMaxItem = D3DXVECTOR3(-1000.0f, 0.0f, -1000.0f);
		g_Item[nCntXFile][0].nIdxMark = 0;
		g_Item[nCntXFile][0].nIdxFlontPolygon[0] = 0;
		g_Item[nCntXFile][0].nIdxFlontPolygon[1] = 0;
		g_Item[nCntXFile][0].type = ITEMTYPE_MAX;
		g_Item[nCntXFile][0].nCntTimer = 0;
		g_Item[nCntXFile][0].bInPlayer = false;
		g_Item[nCntXFile][0].bUse = false;

		//頂点数を取得
		nNumVtx = g_Item[nCntXFile][0].pMeshItem->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Item[nCntXFile][0].pMeshItem->GetFVF());

		//頂点バッファをロック
		g_Item[nCntXFile][0].pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

			//最大値を比較
			if (g_Item[nCntXFile][0].vtxMaxItem.x < vtx.x)
			{
				g_Item[nCntXFile][0].vtxMaxItem.x = vtx.x;
			}
			if (g_Item[nCntXFile][0].vtxMaxItem.z < vtx.z)
			{
				g_Item[nCntXFile][0].vtxMaxItem.z = vtx.z;
			}

			//最小値を比較
			if (g_Item[nCntXFile][0].vtxMinItem.x > vtx.x)
			{
				g_Item[nCntXFile][0].vtxMinItem.x = vtx.x;
			}
			if (g_Item[nCntXFile][0].vtxMinItem.z > vtx.z)
			{
				g_Item[nCntXFile][0].vtxMinItem.z = vtx.z;
			}

			//サイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		//バッファをアンロック
		g_Item[nCntXFile][0].pMeshItem->UnlockVertexBuffer();
	}

	for (nCntXFile = 0; nCntXFile < MAX_XFAILE; nCntXFile++)
	{
		for (nCntItem = 1; nCntItem < MAX_XOBJECT; nCntItem++)
		{
			g_Item[nCntXFile][nCntItem] = g_Item[nCntXFile][0];
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	//変数宣言
	int	nCntXFile;

	for (nCntXFile = 0; nCntXFile < MAX_XFAILE; nCntXFile++)
	{
		// メッシュの開放
		if (g_Item[nCntXFile][0].pMeshItem != NULL)
		{
			g_Item[nCntXFile][0].pMeshItem->Release();
			g_Item[nCntXFile][0].pMeshItem = NULL;
		}

		// マテリアルの開放
		if (g_Item[nCntXFile][0].pBuffMatItem != NULL)
		{
			g_Item[nCntXFile][0].pBuffMatItem->Release();
			g_Item[nCntXFile][0].pBuffMatItem = NULL;
		}
	}

	for (nCntXFile = 0; nCntXFile < MAX_XFAILE; nCntXFile++)
	{
		for (DWORD nTex = 0; nTex < g_Item[nCntXFile][0].nNumMatItem; nTex++)
		{
			//テクスチャの開放
			if (g_Item[nCntXFile][0].pTextureItem[nTex] != NULL)
			{
				g_Item[nCntXFile][0].pTextureItem[nTex]->Release();
				g_Item[nCntXFile][0].pTextureItem[nTex] = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	//変数宣言
	int	nCntXFile;
	int nCntItem;

	for (nCntXFile = 0; nCntXFile < MAX_XFAILE; nCntXFile++)
	{
		for (nCntItem = 0; nCntItem < MAX_XOBJECT; nCntItem++)
		{
			if (g_Item[nCntXFile][nCntItem].bUse == true)
			{
				g_Item[nCntXFile][nCntItem].nCntTimer++;

				if (g_Item[nCntXFile][nCntItem].nCntTimer % 20 == 0)
				{
					SetRainEffect(g_Item[nCntXFile][nCntItem].pos, EFFECTTYPE_RESURRECTIONWATER_1, 2, 30, 0, 0);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号	/噴水
				}
			}

			if (g_Item[nCntXFile][nCntItem].bUse == false && g_Item[nCntXFile][nCntItem].nIdxFlontPolygon[0] != g_Item[nCntXFile][nCntItem].nIdxFlontPolygon[1])
			{
				if (GetStoryFont() == false)
				{
					if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
					{
						DeleteFlontPolygon(g_Item[nCntXFile][nCntItem].nIdxFlontPolygon[0]);	//ポリゴンを消す
						DeleteFlontPolygon(g_Item[nCntXFile][nCntItem].nIdxFlontPolygon[1]);
						DeleteItemFont();		//フォントを消す
					}
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数宣言
	int				nCntXFile;
	int				nCntItem;
	D3DXMATRIX		mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9	matDef;				//現在のマテリアル保存用
	D3DXMATERIAL	*pMat;				//マテリアルデータへのポインタ

	for (nCntXFile = 0; nCntXFile < MAX_XFAILE; nCntXFile++)
	{
		for (nCntItem = 0; nCntItem < MAX_XOBJECT; nCntItem++)
		{
			if (g_Item[nCntXFile][nCntItem].bUse == true)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Item[nCntXFile][nCntItem].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCntXFile][nCntItem].rot.y, g_Item[nCntXFile][nCntItem].rot.x, g_Item[nCntXFile][nCntItem].rot.z);
				D3DXMatrixMultiply(&g_Item[nCntXFile][nCntItem].mtxWorld, &g_Item[nCntXFile][nCntItem].mtxWorld, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTrans, g_Item[nCntXFile][nCntItem].pos.x, g_Item[nCntXFile][nCntItem].pos.y, g_Item[nCntXFile][nCntItem].pos.z);
				D3DXMatrixMultiply(&g_Item[nCntXFile][nCntItem].mtxWorld, &g_Item[nCntXFile][nCntItem].mtxWorld, &mtxTrans);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCntXFile][nCntItem].mtxWorld);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Item[nCntXFile][nCntItem].pBuffMatItem->GetBufferPointer();

				// テクスチャデータへのポインタを取得
				for (int nCntMat = 0; nCntMat < (int)g_Item[nCntXFile][nCntItem].nNumMatItem; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャの設定
					pDevice->SetTexture(0, g_Item[nCntXFile][nCntItem].pTextureItem[nCntMat]);

					// モデル(パーツ)の描画
					g_Item[nCntXFile][nCntItem].pMeshItem->DrawSubset(nCntMat);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}

}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollsionItem(D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, D3DXVECTOR3 * pPos, D3DXVECTOR3 * pOldPos)
{
	//変数宣言
	int	nCntXFile;
	int nCntItem;
	bool bLand = false;

	XPLAYER *pXPlayer = GetXPlayer();

	for (nCntXFile = 0; nCntXFile < MAX_XFAILE; nCntXFile++)
	{
		for (nCntItem = 0; nCntItem < MAX_XOBJECT; nCntItem++)
		{
			if (g_Item[nCntXFile][nCntItem].bUse == true)
			{
				if (pPos->x + vtxMax.x >= g_Item[nCntXFile][nCntItem].pos.x + g_Item[nCntXFile][nCntItem].vtxMinItem.x &&
					pPos->x + vtxMin.x <= g_Item[nCntXFile][nCntItem].pos.x + g_Item[nCntXFile][nCntItem].vtxMaxItem.x &&
					pPos->z + vtxMax.z >= g_Item[nCntXFile][nCntItem].pos.z + g_Item[nCntXFile][nCntItem].vtxMinItem.z &&
					pPos->z + vtxMin.z <= g_Item[nCntXFile][nCntItem].pos.z + g_Item[nCntXFile][nCntItem].vtxMaxItem.z)
				{//ｘの範囲判定
						DeleteItem(nCntItem, g_Item[nCntXFile][nCntItem].type);		//アイテムを消す
						DeleteMark(g_Item[nCntXFile][nCntItem].nIdxMark);			//マークを消す

						pXPlayer->nAnimnow = PLAYERANIM_NEUTRAL;
						pXPlayer->nAnimkeynow = 0;

						bLand = true;

						AddScore(1500);

						PlaySound(SOUND_LABEL_SE_PICKUP);
				}

				/*if (pPos->x + vtxMax.x > g_Item[nCntXFile][nCntItem].pos.x + g_Item[nCntXFile][nCntItem].vtxMinItem.x &&
					pPos->x + vtxMin.x < g_Item[nCntXFile][nCntItem].pos.x + g_Item[nCntXFile][nCntItem].vtxMaxItem.x)
				{//ｘの範囲判定
					if (pOldPos->z + vtxMin.z >= g_Item[nCntXFile][nCntItem].pos.z + g_Item[nCntXFile][nCntItem].vtxMaxItem.z &&
						g_Item[nCntXFile][nCntItem].pos.z + g_Item[nCntXFile][nCntItem].vtxMaxItem.z >= pPos->z + vtxMin.z)
					{//奥面
						DeleteItem(nCntItem, g_Item[nCntXFile][nCntItem].type);	//アイテムを消す
						DeleteMark(g_Item[nCntXFile][nCntItem].nIdxMark);			//マークを消す

						pXPlayer->nAnimnow = PLAYERANIM_NEUTRAL;
						pXPlayer->nAnimkeynow = 0;

						bLand = true;
					}
					else if (pOldPos->z + vtxMax.z <= g_Item[nCntXFile][nCntItem].pos.z + g_Item[nCntXFile][nCntItem].vtxMinItem.z &&
						g_Item[nCntXFile][nCntItem].pos.z + g_Item[nCntXFile][nCntItem].vtxMinItem.z <= pPos->z + vtxMax.z)
					{//手前の面
						DeleteItem(nCntItem, g_Item[nCntXFile][nCntItem].type);	//アイテムを消す
						DeleteMark(g_Item[nCntXFile][nCntItem].nIdxMark);			//マークを消す

						pXPlayer->nAnimnow = PLAYERANIM_NEUTRAL;
						pXPlayer->nAnimkeynow = 0;

						bLand = true;
					}
				}

				if (pPos->z + vtxMax.z < g_Item[nCntXFile][nCntItem].pos.z + g_Item[nCntXFile][nCntItem].vtxMaxItem.z &&
					pPos->z + vtxMax.z > g_Item[nCntXFile][nCntItem].pos.z + g_Item[nCntXFile][nCntItem].vtxMinItem.z)
				{//zの範囲判定
					if (pOldPos->x + vtxMin.x >= g_Item[nCntXFile][nCntItem].pos.x + g_Item[nCntXFile][nCntItem].vtxMaxItem.x &&
						g_Item[nCntXFile][nCntItem].pos.x + g_Item[nCntXFile][nCntItem].vtxMaxItem.x >= pPos->x + vtxMin.x)
					{//右側の面
						DeleteItem(nCntItem, g_Item[nCntXFile][nCntItem].type);	//アイテムを消す
						DeleteMark(g_Item[nCntXFile][nCntItem].nIdxMark);			//マークを消す

						pXPlayer->nAnimnow = PLAYERANIM_NEUTRAL;
						pXPlayer->nAnimkeynow = 0;

						bLand = true;
					}
					else if (pOldPos->x + vtxMax.x <= g_Item[nCntXFile][nCntItem].pos.x + g_Item[nCntXFile][nCntItem].vtxMinItem.x &&
						g_Item[nCntXFile][nCntItem].pos.x + g_Item[nCntXFile][nCntItem].vtxMinItem.x <= pPos->x + vtxMax.x)
					{//左側の面
						DeleteItem(nCntItem, g_Item[nCntXFile][nCntItem].type);		//アイテムを消す
						DeleteMark(g_Item[nCntXFile][nCntItem].nIdxMark);				//マークを消す

						pXPlayer->nAnimnow = PLAYERANIM_NEUTRAL;
						pXPlayer->nAnimkeynow = 0;

						bLand = true;
					}
				}*/

				//外側の範囲判定
				if (pPos->x + vtxMax.x > g_Item[nCntXFile][nCntItem].CircleMin.x &&
					pPos->x + vtxMin.x < g_Item[nCntXFile][nCntItem].CircleMax.x &&
					pPos->z + vtxMax.z < g_Item[nCntXFile][nCntItem].CircleMax.z &&
					pPos->z + vtxMax.z > g_Item[nCntXFile][nCntItem].CircleMin.z)
				{//ｘとｚの範囲判定
					if (g_Item[nCntXFile][nCntItem].bInPlayer == false)
					{//アイテムの範囲内に入ったら
						g_Item[nCntXFile][nCntItem].nIdxMark = SetMark(D3DXVECTOR3(g_Item[nCntXFile][nCntItem].pos.x, 50.0f, g_Item[nCntXFile][nCntItem].pos.z));	//マークを出す
						g_Item[nCntXFile][nCntItem].bInPlayer = true;																								//範囲内に入っている状態にする
					}
				}
				else
				{
					if (g_Item[nCntXFile][nCntItem].bInPlayer == true)
					{
						DeleteMark(g_Item[nCntXFile][nCntItem].nIdxMark);		//マークを消す
						g_Item[nCntXFile][nCntItem].bInPlayer = false;			//範囲内に入っていない状態にする
					}
				}
			}
		}
	}

	return bLand;
}

//=============================================================================
// モデルの設定
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ITEMTYPE type)
{
	//変数宣言
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_XOBJECT; nCntItem++)
	{
		if (g_Item[type][nCntItem].bUse == false)
		{
			g_Item[type][nCntItem].pos = pos;								//中心座標
			g_Item[type][nCntItem].OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の位置
			g_Item[type][nCntItem].rot = rot;								//向き
			g_Item[type][nCntItem].type = type;								//アイテムのタイプ
			g_Item[type][nCntItem].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//回転量
			g_Item[type][nCntItem].nIdxMark = 0;							//マークの番号
			g_Item[type][nCntItem].nIdxFlontPolygon[0] = 0;					//フロントポリゴンの使用番号
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = 0;					//フロントポリゴンの使用番号
			g_Item[type][nCntItem].nCntTimer = 0;					//
			g_Item[type][nCntItem].bInPlayer = false;						//範囲内に入っていない状態にする
			g_Item[type][nCntItem].bUse = true;								//使用している状態にする

			//範囲設定（Max）
			g_Item[type][nCntItem].CircleMax = g_Item[type][nCntItem].pos + g_Item[type][nCntItem].vtxMaxItem;
			g_Item[type][nCntItem].CircleMax += D3DXVECTOR3(30.0f, 0.0f, 30.0f);

			//範囲設定（Min）
			g_Item[type][nCntItem].CircleMin = g_Item[type][nCntItem].pos + g_Item[type][nCntItem].vtxMinItem;
			g_Item[type][nCntItem].CircleMin -= D3DXVECTOR3(30.0f, 0.0f, 30.0f);

			//アイテムの総数を増やす
			g_nGetItem++;

			//影の設定
			/*g_Item[nCntXFile].nIdxshadow = SetShadow(D3DXVECTOR3(g_Item[nCntXFile].pos.x, 0.0f, g_Item[nCntXFile].pos.z),
																  g_Item[nCntXFile].rot, 15.0f, 15.0f);	*/
			break;
		}
	}
}

//=============================================================================
// アイテムの消去
//=============================================================================
void DeleteItem(int nCntItem, ITEMTYPE type)
{
	if (g_Item[type][nCntItem].bUse == true)
	{
		g_Item[type][nCntItem].bUse = false;	//消去
		AddItemCnt(1);
		g_Item[type][nCntItem].nIdxFlontPolygon[0] = SetFlontPolygon(FLONTPOLYGONTEXTURE_FONT_BG, D3DXVECTOR3(640.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, 180.0f);	//UIのセット

		if (g_Item[type][nCntItem].type == ITEMTYPE_GIAR_00)
		{
			SetItemFont("部品を手に入れた！", TYPEITEMFONT_ITEM);														//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_GEAR_0, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
		else if (g_Item[type][nCntItem].type == ITEMTYPE_GIAR_01)
		{
			SetItemFont("部品を手に入れた！", TYPEITEMFONT_ITEM);														//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_GEAR_1, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
		else if (g_Item[type][nCntItem].type == ITEMTYPE_GIAR_02)
		{
			SetItemFont("部品を手に入れた！", TYPEITEMFONT_ITEM);														//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_GEAR_2, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
		else if (g_Item[type][nCntItem].type == ITEMTYPE_GIAR_03)
		{
			SetItemFont("部品を手に入れた！", TYPEITEMFONT_ITEM);														//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_GEAR_3, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
		else if (g_Item[type][nCntItem].type == ITEMTYPE_WATCH_00)
		{
			SetItemFont("時計の枠を手に入れた！", TYPEITEMFONT_ITEM);													//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_CLOCK_0, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
		else if (g_Item[type][nCntItem].type == ITEMTYPE_WATCH_01)
		{
			SetItemFont("チェーンを手に入れた！", TYPEITEMFONT_ITEM);													//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_CLOCK_1, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
		else if (g_Item[type][nCntItem].type == ITEMTYPE_WATCH_02)
		{
			SetItemFont("受皿を手に入れた！", TYPEITEMFONT_ITEM);														//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_CLOCK_2, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
		else if (g_Item[type][nCntItem].type == ITEMTYPE_ORUGORU_00)
		{
			SetItemFont("オルゴール１を手に入れた！", TYPEITEMFONT_ITEM);														//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_MUSICBOX_0, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
		else if (g_Item[type][nCntItem].type == ITEMTYPE_ORUGORU_01)
		{
			SetItemFont("オルゴール２を手に入れた！", TYPEITEMFONT_ITEM);														//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_MUSICBOX_1, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
		else if (g_Item[type][nCntItem].type == ITEMTYPE_ORUGORU_02)
		{
			SetItemFont("木箱を手に入れた！", TYPEITEMFONT_ITEM);														//文字の設定
			g_Item[type][nCntItem].nIdxFlontPolygon[1] = SetFlontPolygon(FLONTPOLYGONTEXTURE_MUSICBOX_2, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130.0f, 130.0f);	//アイテムのアイコン設定
		}
	}
}

//=============================================================================
// 全てのアイテムを消す
//=============================================================================
void AllDeleteItem(void)
{
	//変数宣言
	int nCntXFile;
	int nCntItem;

	for (nCntXFile = 0; nCntXFile < MAX_XFAILE; nCntXFile++)
	{
		for (nCntItem = 0; nCntItem < MAX_XOBJECT; nCntItem++)
		{
			g_Item[nCntXFile][nCntItem].bUse = false;		//アイテムを消す
		}
	}

	g_nGetItem = 0;

	//アイテムカウンターをリセットする
	ResetItemCnt();
}

//=============================================================================
// アイテムの総数を取得
//=============================================================================
int GetItem(void)
{
	return g_nGetItem;
}