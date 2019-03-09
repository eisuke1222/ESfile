//=============================================================================
//
// ポリゴン処理 [flontpolygon.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "flontpolygon.h"
#include "xobject.h"
#include "time.h"
#include "main.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_FLONTPOLYGON	(64)			//読みこむテクスチャの最大数
#define TITLEANIM_UV_U		(0.1f)
#define TITLEANIM_UV_V		(0.08333f)

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureFlontPolygon[FLONTPOLYGONTEXTURE_MAX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFlontPolygon = NULL;			//頂点バッファへのポインタ
FlontPolygon			g_FlontPolygon[MAX_FLONTPOLYGON];		//情報設定
int						g_nMaxFlontPolygon;
int						g_nPointerIdx;
bool					g_bDeletePartsTexture;

//=============================================================================
// スタートボタン初期化処理
//=============================================================================
void InitFlontPolygon(void)
{
	//変数宣言
//	int		nCntTex;
	int     nCntFlontPolygon;

	g_nPointerIdx = 99;
	g_bDeletePartsTexture = false;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数の初期化
	for (nCntFlontPolygon = 0; nCntFlontPolygon < MAX_FLONTPOLYGON; nCntFlontPolygon++)
	{
		g_FlontPolygon[nCntFlontPolygon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//中心座標
		g_FlontPolygon[nCntFlontPolygon].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
		g_FlontPolygon[nCntFlontPolygon].fHeight = 0.0f;								//高さ
		g_FlontPolygon[nCntFlontPolygon].fWidth = 0.0f;								//幅
		g_FlontPolygon[nCntFlontPolygon].nCntAnim = 0;								//アニメーションカウント
		g_FlontPolygon[nCntFlontPolygon].nCntPattrn= 0;								//アニメーションパターン
		g_FlontPolygon[nCntFlontPolygon].nTexNum = 0;								//テクスチャの番号
		g_FlontPolygon[nCntFlontPolygon].nCntPolygon = 0;							//使用番号
		g_FlontPolygon[nCntFlontPolygon].bUse = false;								//使用していない状態にする
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game_over.png",	&g_pTextureFlontPolygon[0]);	//ゲームオーバーロゴ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\UI2.png",		&g_pTextureFlontPolygon[1]);	//フォント背景
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player3.png",	&g_pTextureFlontPolygon[2]);	//主人公顔画像
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy02.png",	&g_pTextureFlontPolygon[3]);	//顔画像
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy04.png",	&g_pTextureFlontPolygon[4]);	//顔画像
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy03.png",	&g_pTextureFlontPolygon[5]);	//顔画像
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item000.png",	&g_pTextureFlontPolygon[6]);	//歯車１
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item002.png",	&g_pTextureFlontPolygon[7]);	//歯車２
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item002.png",	&g_pTextureFlontPolygon[8]);	//歯車３
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item003.png",	&g_pTextureFlontPolygon[9]);	//歯車４
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\watch00.png",	&g_pTextureFlontPolygon[10]);	//時計１（枠）
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\watch02.png",	&g_pTextureFlontPolygon[11]);	//時計２（チェーン）
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\watch02.png",	&g_pTextureFlontPolygon[12]);	//時計３（受皿）
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\orugoru00.png",	&g_pTextureFlontPolygon[13]);	//オルゴール１
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\orugoru01.png",	&g_pTextureFlontPolygon[14]);	//オルゴール２
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\orugoru02.png",	&g_pTextureFlontPolygon[15]);	//オルゴール３（木箱）
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sen.png",		&g_pTextureFlontPolygon[16]);	//数字の線

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\paper.png", &g_pTextureFlontPolygon[17]);		//メニュー枠
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item000.png", &g_pTextureFlontPolygon[18]);		//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item001.png", &g_pTextureFlontPolygon[19]);		//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item002.png", &g_pTextureFlontPolygon[20]);		//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item003.png", &g_pTextureFlontPolygon[21]);		//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item000.png", &g_pTextureFlontPolygon[22]);		//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item001.png", &g_pTextureFlontPolygon[23]);		//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item002.png", &g_pTextureFlontPolygon[24]);		//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item003.png", &g_pTextureFlontPolygon[25]);		//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\zenmai000.png", &g_pTextureFlontPolygon[26]);	//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\orugoru001.png", &g_pTextureFlontPolygon[27]);	//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\orugoru000.png", &g_pTextureFlontPolygon[28]);	//部品
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\parts000.png", &g_pTextureFlontPolygon[29]);		//部品大きさ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\parts001.png", &g_pTextureFlontPolygon[30]);		//部品大きさ

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pazzlein.png", &g_pTextureFlontPolygon[31]);		//文字
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pazzlestart.png", &g_pTextureFlontPolygon[32]);	//文字
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pazzleend.png", &g_pTextureFlontPolygon[33]);	//文字

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stamp_A.png", &g_pTextureFlontPolygon[34]);		//スタンプA
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stamp_B.png", &g_pTextureFlontPolygon[35]);		//スタンプB
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stamp_C.png", &g_pTextureFlontPolygon[36]);		//スタンプC
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stamp_D.png", &g_pTextureFlontPolygon[37]);		//スタンプD

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\parts_Oclock.jpg", &g_pTextureFlontPolygon[38]);		//探索パーツQ1
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\parts_MusicBox.jpg", &g_pTextureFlontPolygon[39]);	//探索パーツQ2
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\parts_doll.jpg", &g_pTextureFlontPolygon[40]);		//探索パーツQ3

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pointer.png", &g_pTextureFlontPolygon[41]);		//ポインター

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\rank_A.png", &g_pTextureFlontPolygon[42]);		//ランクA
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\rank_B.png", &g_pTextureFlontPolygon[43]);		//ランクB
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\rank_C.png", &g_pTextureFlontPolygon[44]);		//ランクC
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\rank_D.png", &g_pTextureFlontPolygon[45]);		//ランクD

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\result000.png", &g_pTextureFlontPolygon[46]);		//

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial000.png", &g_pTextureFlontPolygon[47]);		//
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial001.png", &g_pTextureFlontPolygon[48]);		//
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial002.png", &g_pTextureFlontPolygon[49]);		//

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FLONTPOLYGON,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffFlontPolygon,
								NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFlontPolygon->Lock(0, 0, (void**)&pVtx, 0);
	
	for (nCntFlontPolygon = 0; nCntFlontPolygon < MAX_FLONTPOLYGON; nCntFlontPolygon++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(250.0f, 100.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(750.0f, 100.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(250.0f, 200.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(750.0f, 200.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFlontPolygon->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitFlontPolygon(void)
{
	//変数宣言
	int nCntTex;

	//テクスチャの破棄
	for (nCntTex = 0; nCntTex < FLONTPOLYGONTEXTURE_MAX; nCntTex++)
	{
		if (g_pTextureFlontPolygon[nCntTex] != NULL)
		{
			g_pTextureFlontPolygon[nCntTex]->Release();
			g_pTextureFlontPolygon[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffFlontPolygon != NULL)
	{
		g_pVtxBuffFlontPolygon->Release();
		g_pVtxBuffFlontPolygon = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateFlontPolygon(void)
{
	//変数宣言
	int nCntFlontPolygon;

	MODE mode = GetMode();

	PAZZLENUM PazzleNum = *GetPazzleNum();
	bool bChangeMoveObj = false;
	DIMOUSESTATE2 pMouse = GetMousePointer();
	//POINT po;
	DIJOYSTATE pStick = GetStick();

	Time *pTime = GetTime();

	int nPazzle = 0;

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFlontPolygon->Lock(0, 0, (void**)&pVtx, 0);
	
	for (nCntFlontPolygon = 0; nCntFlontPolygon < MAX_FLONTPOLYGON; nCntFlontPolygon++)
	{
		if (g_FlontPolygon[nCntFlontPolygon].bUse == true)
		{
			if (g_FlontPolygon[nCntFlontPolygon].nTexNum >= FLONTPOLYGONTEXTURE_MENU_1 &&
				g_FlontPolygon[nCntFlontPolygon].nTexNum <= FLONTPOLYGONTEXTURE_MENU_MB2)
			{
				if (PazzleNum.modelNum[nPazzle] > 0)
				{
					if (/*GetMouseTrigger(0) == true && */ (GetKeyboardTrigger(DIK_LSHIFT) == true || GetKeyboardTrigger(DIK_RSHIFT) == true || GetButtonTrigger(0) == true) && bChangeMoveObj == false && g_FlontPolygon[g_nPointerIdx].bUse == true &&
						g_FlontPolygon[nCntFlontPolygon].pos.x - (g_FlontPolygon[nCntFlontPolygon].fWidth / 2) < g_FlontPolygon[g_nPointerIdx].pos.x && g_FlontPolygon[nCntFlontPolygon].pos.x + (g_FlontPolygon[nCntFlontPolygon].fWidth / 2) > g_FlontPolygon[g_nPointerIdx].pos.x &&
						g_FlontPolygon[nCntFlontPolygon].pos.y - (g_FlontPolygon[nCntFlontPolygon].fHeight / 2) < g_FlontPolygon[g_nPointerIdx].pos.y && g_FlontPolygon[nCntFlontPolygon].pos.y + (g_FlontPolygon[nCntFlontPolygon].fHeight / 2) > g_FlontPolygon[g_nPointerIdx].pos.y)
					{
						SetMovePazzelObj(nPazzle);
						bChangeMoveObj = true;
					}
				}
				else
				{
					g_FlontPolygon[nCntFlontPolygon].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				}
	
				nPazzle++;
			}
			else if (g_FlontPolygon[nCntFlontPolygon].nTexNum >= FLONTPOLYGONTEXTURE_PAZZLE_IN &&
				g_FlontPolygon[nCntFlontPolygon].nTexNum <= FLONTPOLYGONTEXTURE_PAZZLE_END)
			{
				g_FlontPolygon[nCntFlontPolygon].pos.x -= 9.0f;

				if (g_FlontPolygon[nCntFlontPolygon].pos.x < -SCREEN_WIDTH)
				{
					DeleteFlontPolygon(nCntFlontPolygon);
				}
				if (g_FlontPolygon[nCntFlontPolygon].pos.x <= SCREEN_WIDTH / 4 && g_FlontPolygon[nCntFlontPolygon].nTexNum == FLONTPOLYGONTEXTURE_PAZZLE_START)
				{
					SetTimeState(TIMESTATE_NORMAL);
				}
			}
			else if (g_FlontPolygon[nCntFlontPolygon].nTexNum >= FLONTPOLYGONTEXTURE_QUESTSTAMP_A &&
				g_FlontPolygon[nCntFlontPolygon].nTexNum <= FLONTPOLYGONTEXTURE_QUESTSTAMP_D)
			{
				if (mode == MODE_GAMECLEAR)
				{
					if (g_FlontPolygon[nCntFlontPolygon].fWidth <= 300 &&
						g_FlontPolygon[nCntFlontPolygon].fHeight <= 300)
					{
						g_FlontPolygon[nCntFlontPolygon].fWidth = 300;
						g_FlontPolygon[nCntFlontPolygon].fHeight = 300;

					}
					else
					{
						g_FlontPolygon[nCntFlontPolygon].fWidth -= 300 / 4;
						g_FlontPolygon[nCntFlontPolygon].fHeight -= 300 / 4;
					}
				}
				else
				{
					if (g_FlontPolygon[nCntFlontPolygon].fWidth <= 500 &&
						g_FlontPolygon[nCntFlontPolygon].fHeight <= 500)
					{
						g_FlontPolygon[nCntFlontPolygon].fWidth = 500;
						g_FlontPolygon[nCntFlontPolygon].fHeight = 500;

					}
					else
					{
						g_FlontPolygon[nCntFlontPolygon].fWidth -= 500 / 4;
						g_FlontPolygon[nCntFlontPolygon].fHeight -= 500 / 4;
					}
				}
			}
			else if (g_FlontPolygon[nCntFlontPolygon].nTexNum == FLONTPOLYGONTEXTURE_QUESTCLEAR)
			{
				if (g_FlontPolygon[nCntFlontPolygon].col.a < 0.5f)
				{
					g_FlontPolygon[nCntFlontPolygon].col.a += 0.015f;
				}
				else
				{
					g_FlontPolygon[nCntFlontPolygon].col.a = 0.5f;
				}
			}
			else if (g_FlontPolygon[nCntFlontPolygon].nTexNum >= FLONTPOLYGONTEXTURE_PARTS_WIN_CLOCK &&
				g_FlontPolygon[nCntFlontPolygon].nTexNum <= FLONTPOLYGONTEXTURE_PARTS_WIN_DOLL)
			{
				if (g_FlontPolygon[nCntFlontPolygon].fWidth >= 900 &&
					g_FlontPolygon[nCntFlontPolygon].fHeight >= 600)
				{
					g_FlontPolygon[nCntFlontPolygon].fWidth = 900;
					g_FlontPolygon[nCntFlontPolygon].fHeight = 600;

					if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
					{
						DeleteFlontPolygon(nCntFlontPolygon);
					}
				}
				else
				{
					g_FlontPolygon[nCntFlontPolygon].fWidth += 900 / 12;
					g_FlontPolygon[nCntFlontPolygon].fHeight += 600 / 12;
				}
			}
			else if (g_FlontPolygon[nCntFlontPolygon].nTexNum == FLONTPOLYGONTEXTURE_POINTER)
			{
				if (pTime->TimeState == TIMESTATE_NORMAL)
				{
					if (GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 0 || pStick.rgdwPOV[0] == 4500 || pStick.rgdwPOV[0] == 31500 || pStick.lY < -300)
					{
						g_FlontPolygon[nCntFlontPolygon].pos.y -= 6.0f;
					}
					if (GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 18000 || pStick.rgdwPOV[0] == 22500 || pStick.rgdwPOV[0] == 13500 || pStick.lY > 300)
					{
						g_FlontPolygon[nCntFlontPolygon].pos.y += 6.0f;
					}
					if (GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 27000 || pStick.rgdwPOV[0] == 31500 || pStick.rgdwPOV[0] == 22500 || pStick.lX < -300)
					{
						g_FlontPolygon[nCntFlontPolygon].pos.x -= 6.0f;
					}
					if (GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 9000 || pStick.rgdwPOV[0] == 13500 || pStick.rgdwPOV[0] == 4500 || pStick.lX > 300)
					{
						g_FlontPolygon[nCntFlontPolygon].pos.x += 6.0f;
					}
					{
						/*if (g_FlontPolygon[nCntFlontPolygon].pos.x < 50.0f)
						{
							g_FlontPolygon[nCntFlontPolygon].pos.x = 50.0f;
						}*/

						if (g_FlontPolygon[nCntFlontPolygon].pos.x < (SCREEN_WIDTH - 240) + 50.0f)
						{
							g_FlontPolygon[nCntFlontPolygon].pos.x = (SCREEN_WIDTH - 240) + 50.0f;
						}

						if (g_FlontPolygon[nCntFlontPolygon].pos.x > SCREEN_WIDTH - 50.0f)
						{
							g_FlontPolygon[nCntFlontPolygon].pos.x = SCREEN_WIDTH - 50.0f;
						}

						/*if (g_FlontPolygon[nCntFlontPolygon].pos.y < 50.0f)
						{
							g_FlontPolygon[nCntFlontPolygon].pos.y = 50.0f;
						}*/

						if (g_FlontPolygon[nCntFlontPolygon].pos.y < 200.0f)
						{
							g_FlontPolygon[nCntFlontPolygon].pos.y = 200.0f;
						}

						if (g_FlontPolygon[nCntFlontPolygon].pos.y > SCREEN_HEIGHT - 50.0f)
						{
							g_FlontPolygon[nCntFlontPolygon].pos.y = SCREEN_HEIGHT - 50.0f;
						}
					}
				}
			}
			else if (g_FlontPolygon[nCntFlontPolygon].nTexNum >= FLONTPOLYGONTEXTURE_CLEAR_RANK_A &&
				g_FlontPolygon[nCntFlontPolygon].nTexNum <= FLONTPOLYGONTEXTURE_CLEAR_RANK_D)
			{
				if (g_FlontPolygon[nCntFlontPolygon].fWidth <= 1000 &&
					g_FlontPolygon[nCntFlontPolygon].fHeight <= 400)
				{
					g_FlontPolygon[nCntFlontPolygon].fWidth = 1000;
					g_FlontPolygon[nCntFlontPolygon].fHeight = 400;

				}
				else
				{
					g_FlontPolygon[nCntFlontPolygon].fWidth -= 1000 / 4;
					g_FlontPolygon[nCntFlontPolygon].fHeight -= 400 / 4;
				}
			}

			//頂点座標の設定
			pVtx[nCntFlontPolygon * 4].pos = D3DXVECTOR3(g_FlontPolygon[nCntFlontPolygon].pos.x - (g_FlontPolygon[nCntFlontPolygon].fWidth / 2), g_FlontPolygon[nCntFlontPolygon].pos.y - (g_FlontPolygon[nCntFlontPolygon].fHeight / 2), 0.0f);
			pVtx[nCntFlontPolygon * 4 + 1].pos = D3DXVECTOR3(g_FlontPolygon[nCntFlontPolygon].pos.x + (g_FlontPolygon[nCntFlontPolygon].fWidth / 2), g_FlontPolygon[nCntFlontPolygon].pos.y - (g_FlontPolygon[nCntFlontPolygon].fHeight / 2), 0.0f);
			pVtx[nCntFlontPolygon * 4 + 2].pos = D3DXVECTOR3(g_FlontPolygon[nCntFlontPolygon].pos.x - (g_FlontPolygon[nCntFlontPolygon].fWidth / 2), g_FlontPolygon[nCntFlontPolygon].pos.y + (g_FlontPolygon[nCntFlontPolygon].fHeight / 2), 0.0f);
			pVtx[nCntFlontPolygon * 4 + 3].pos = D3DXVECTOR3(g_FlontPolygon[nCntFlontPolygon].pos.x + (g_FlontPolygon[nCntFlontPolygon].fWidth / 2), g_FlontPolygon[nCntFlontPolygon].pos.y + (g_FlontPolygon[nCntFlontPolygon].fHeight / 2), 0.0f);
			//頂点カラーの設定
			pVtx[nCntFlontPolygon * 4].col = g_FlontPolygon[nCntFlontPolygon].col;
			pVtx[nCntFlontPolygon * 4 + 1].col = g_FlontPolygon[nCntFlontPolygon].col;
			pVtx[nCntFlontPolygon * 4 + 2].col = g_FlontPolygon[nCntFlontPolygon].col;
			pVtx[nCntFlontPolygon * 4 + 3].col = g_FlontPolygon[nCntFlontPolygon].col;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFlontPolygon->Unlock();
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawFlontPolygon(void)
{
	//変数宣言
	int nCntFlontPolygon;

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFlontPolygon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntFlontPolygon = 0; nCntFlontPolygon < MAX_FLONTPOLYGON; nCntFlontPolygon++)
	{
		if (g_FlontPolygon[nCntFlontPolygon].bUse == true)
		{
			//テクスチャの設定
			if (g_FlontPolygon[nCntFlontPolygon].nTexNum < FLONTPOLYGONTEXTURE_MAX)
			{
				pDevice->SetTexture(0, g_pTextureFlontPolygon[g_FlontPolygon[nCntFlontPolygon].nTexNum]);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								   nCntFlontPolygon * 4,	//開始する頂点のインデックス
								   2);						//プリミティブの数
		}
	}
}

//=============================================================================
// 情報設定
//=============================================================================
int SetFlontPolygon(int nTexNum, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	//変数宣言
	int nCntFlontPolygon;
	int nNumIndx = 0;
	float fHalfWidth = 0;		//幅
	float fHalfHight = 0;		//高さ

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFlontPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntFlontPolygon = 0; nCntFlontPolygon < MAX_FLONTPOLYGON; nCntFlontPolygon++)
	{
		if (g_FlontPolygon[nCntFlontPolygon].bUse == false)
		{
			g_FlontPolygon[nCntFlontPolygon].pos = pos;				//中心座標
			g_FlontPolygon[nCntFlontPolygon].fHeight = fHeight;		//高さ
			g_FlontPolygon[nCntFlontPolygon].fWidth = fWidth;		//幅
			g_FlontPolygon[nCntFlontPolygon].col = col;				//色
			g_FlontPolygon[nCntFlontPolygon].nCntAnim = 0;			//アニメーションカウント
			g_FlontPolygon[nCntFlontPolygon].nCntPattrn = 0;		//アニメーションパターン

			//幅と高さを設定する
			fHalfWidth = g_FlontPolygon[nCntFlontPolygon].fWidth * 0.5f;
			fHalfHight = g_FlontPolygon[nCntFlontPolygon].fHeight * 0.5f;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fHalfWidth, pos.y - fHalfHight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fHalfWidth, pos.y - fHalfHight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fHalfWidth, pos.y + fHalfHight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fHalfWidth, pos.y + fHalfHight, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
		
			g_FlontPolygon[nCntFlontPolygon].nTexNum = nTexNum;		//テクスチャの番号

			if (g_FlontPolygon[nCntFlontPolygon].nTexNum == FLONTPOLYGONTEXTURE_PARTS_WIN_CLOCK ||
				g_FlontPolygon[nCntFlontPolygon].nTexNum == FLONTPOLYGONTEXTURE_PARTS_WIN_MUSICBOX ||
				g_FlontPolygon[nCntFlontPolygon].nTexNum == FLONTPOLYGONTEXTURE_PARTS_WIN_DOLL)
			{
				g_bDeletePartsTexture = true;
			}

			if (g_FlontPolygon[nCntFlontPolygon].nTexNum == FLONTPOLYGONTEXTURE_POINTER)
			{
				g_nPointerIdx = nCntFlontPolygon;
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			
			g_FlontPolygon[nCntFlontPolygon].bUse = true;			//使用している状態にする
			nNumIndx = nCntFlontPolygon;
			break;
		}

		//該当の位置まで進める
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffFlontPolygon->Unlock();

	return nNumIndx;
}

//=============================================================================
// ポリゴンをすべて消す
//=============================================================================
void DeleteAllFlontPolygon(void)
{
	//変数宣言
	int nCntFlontPolygon;

	for (nCntFlontPolygon = 0; nCntFlontPolygon < MAX_FLONTPOLYGON; nCntFlontPolygon++)
	{
		if (g_FlontPolygon[nCntFlontPolygon].bUse == true)
		{
			g_FlontPolygon[nCntFlontPolygon].bUse = false;		//使用していない状態にする

			if (g_FlontPolygon[nCntFlontPolygon].nTexNum == FLONTPOLYGONTEXTURE_POINTER)
			{
				g_nPointerIdx = 99;
			}
		}
	}

	g_bDeletePartsTexture = false;
}

//=============================================================================
// 特定のポリゴンを消す
//=============================================================================
void DeleteFlontPolygon(int nIndx)
{
	if (g_FlontPolygon[nIndx].bUse == true)
	{
		if (g_FlontPolygon[nIndx].nTexNum == FLONTPOLYGONTEXTURE_PARTS_WIN_CLOCK ||
			g_FlontPolygon[nIndx].nTexNum == FLONTPOLYGONTEXTURE_PARTS_WIN_MUSICBOX ||
			g_FlontPolygon[nIndx].nTexNum == FLONTPOLYGONTEXTURE_PARTS_WIN_DOLL)
		{
			g_bDeletePartsTexture = false;
		}

		if (g_FlontPolygon[nIndx].nTexNum == FLONTPOLYGONTEXTURE_POINTER)
		{
			g_nPointerIdx = 99;
		}

		g_FlontPolygon[nIndx].bUse = false;		//使用していない状態にする
	}
}
void DeleteFlontPolygonA(int nTexNum)
{
	int nCntFlontPolygon;

	for (nCntFlontPolygon = 0; nCntFlontPolygon < MAX_FLONTPOLYGON; nCntFlontPolygon++)
	{
		if (g_FlontPolygon[nCntFlontPolygon].bUse == true && g_FlontPolygon[nCntFlontPolygon].nTexNum == nTexNum)
		{
			g_FlontPolygon[nCntFlontPolygon].bUse = false;
			break;
		}
	}
}

//=============================================================================
// 情報を取得
//=============================================================================
FlontPolygon * GetFlontPolygon(void)
{
	return &g_FlontPolygon[0];
}

//=============================================================================
// 情報を取得
//=============================================================================
bool DeletePartsTexture(void)
{
	return g_bDeletePartsTexture;
}