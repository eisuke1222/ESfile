//=============================================================================
//
// マークの処理 [mark.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "mark.h"
#include "shadow.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_NAME	"data\\TEXTURE\\mark.png"		//読みこむテクスチャファイル名
#define MAX_MARK		(4)								//壁の最大数
#define CHANGE_MOVE		(0.3f)							//変化量
#define CHANGE_COUNTER	(40)							//変化量カウンター

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexMark(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMark = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMark = NULL;	// テクスチャへのポインタ
D3DXVECTOR2				g_texMark;				//テクスチャ
Mark					g_Mark[MAX_MARK];		//壁の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitMark(void)
{
	//変数宣言
	int nCntMark;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  TEXTURE_NAME,
						      &g_pTextureMark);

	//変数の初期化
	for (nCntMark = 0; nCntMark < MAX_MARK; nCntMark++)
	{
		g_Mark[nCntMark].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//中心座標
		g_Mark[nCntMark].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_Mark[nCntMark].Oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の位置
		g_Mark[nCntMark].ChangeMove = 0.0f;							//変化量
		g_Mark[nCntMark].nCntMove = 0;								//変化量カウンター
		g_Mark[nCntMark].bUse = false;								//使用しているかどうか
	}

	// 頂点情報の作成
	MakeVertexMark(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMark(void)
{
	//テクスチャの開放
	if (g_pTextureMark != NULL)
	{
		g_pTextureMark->Release();
		g_pTextureMark = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMark != NULL)
	{
		g_pVtxBuffMark->Release();
		g_pVtxBuffMark = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMark(void)
{
	//変数宣言
	int nCntMark;

	for (nCntMark = 0; nCntMark < MAX_MARK; nCntMark++)
	{
		if (g_Mark[nCntMark].bUse == true)
		{
			//カウンターの加算
			g_Mark[nCntMark].nCntMove++;

			if ((g_Mark[nCntMark].nCntMove % CHANGE_COUNTER) == 0)
			{//一定時間が経過したら
				g_Mark[nCntMark].ChangeMove *= -1;	//符号を変える
			}

			//移動量の更新
			g_Mark[nCntMark].move.y = g_Mark[nCntMark].ChangeMove;

			//位置の更新
			g_Mark[nCntMark].pos += g_Mark[nCntMark].move;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMark(void)
{
	//変数宣言
	int nCntMark;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX		  mtxRot, mtxTrans;			// 計算用マトリックス
	D3DXMATRIX		  mtxView;					//ビューマトリクス

	//アルファテスト処理を有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntMark = 0; nCntMark < MAX_MARK; nCntMark++)
	{
		if (g_Mark[nCntMark].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Mark[nCntMark].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//逆行列を設定
			g_Mark[nCntMark].mtxWorld._11 = mtxView._11;
			g_Mark[nCntMark].mtxWorld._12 = mtxView._21;
			g_Mark[nCntMark].mtxWorld._13 = mtxView._31;
			g_Mark[nCntMark].mtxWorld._21 = mtxView._12;
			g_Mark[nCntMark].mtxWorld._22 = mtxView._22;
			g_Mark[nCntMark].mtxWorld._23 = mtxView._32;
			g_Mark[nCntMark].mtxWorld._31 = mtxView._13;
			g_Mark[nCntMark].mtxWorld._32 = mtxView._23;
			g_Mark[nCntMark].mtxWorld._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Mark[nCntMark].pos.x, g_Mark[nCntMark].pos.y, g_Mark[nCntMark].pos.z);
			D3DXMatrixMultiply(&g_Mark[nCntMark].mtxWorld, &g_Mark[nCntMark].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Mark[nCntMark].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMark, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMark);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntMark * 4,
				2);
		}
	}

	//アルファテスト処理を無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMark(LPDIRECT3DDEVICE9 pDevice)
{
	//変数宣言
	int nCntMark;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MARK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMark,
		NULL);

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMark->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMark = 0; nCntMark < MAX_MARK; nCntMark++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-13.0f, 26.0f, 00.0f);
		pVtx[1].pos = D3DXVECTOR3(13.0f, 26.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-13.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(13.0f, 0.0f, 0.0f);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMark->Unlock();
}

//=============================================================================
// 設定情報
//=============================================================================
int SetMark(D3DXVECTOR3 pos)
{
	//変数宣言
	int nCntMark;
	int nNumMark = 0;

	for (nCntMark = 0; nCntMark < MAX_MARK; nCntMark++)
	{
		if (g_Mark[nCntMark].bUse == false)
		{
			g_Mark[nCntMark].pos = pos;
			g_Mark[nCntMark].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Mark[nCntMark].ChangeMove = 0.1f;
			g_Mark[nCntMark].nCntMove = 0;
			g_Mark[nCntMark].bUse = true;
			nNumMark = nCntMark;
			break;
		}
	}
	
	return nNumMark;
}

//=============================================================================
// マークを消す
//=============================================================================
void DeleteMark(void)
{
	//変数宣言
	int nCntMark;

	for (nCntMark = 0; nCntMark < MAX_MARK; nCntMark++)
	{
		if (g_Mark[nCntMark].bUse == true)
		{
			g_Mark[nCntMark].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//中心座標
			g_Mark[nCntMark].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
			g_Mark[nCntMark].Oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の位置
			g_Mark[nCntMark].ChangeMove = 0.0f;							//変化量
			g_Mark[nCntMark].nCntMove = 0;								//変化量カウンター
			g_Mark[nCntMark].bUse = false;								//使用しているかどうか
		}
	}
}
void DeleteMark(int nIdxMark)
{
	if (g_Mark[nIdxMark].bUse == true)
	{
		g_Mark[nIdxMark].bUse = false;		//使用していない状態にする
	}
}