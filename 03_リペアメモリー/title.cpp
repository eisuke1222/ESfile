//=============================================================================
//
// タイトル画面 [title.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "title.h"
#include "main.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "meshcylinder.h"
#include "meshwall.h"
#include "meshfield.h"
#include "shadow.h"
#include "xobject.h"
#include "start.h"
#include "fade.h"
#include "flontpolygon.h"
#include "titleanim.h"

//=============================================================================
// マクロ定義
//=============================================================================


//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureTitle = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;		//頂点バッファへのポインタ
int g_nCntTime;										//エフェクトのカウンター
int g_nCntTimeTitle;								//タイトル時間のカウンター

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitTitle(void)
{
	//変数宣言
	int nIndx = 0;

	//変数の初期化
	g_nCntTime = 0;
	g_nCntTimeTitle = 0;

	//関数呼び出し
	InitCamera();		//カメラ
	InitMeshCylinder();	//空
	InitMeshWall();		//壁
	InitMeshField();	//地面
	InitShadow();		//影
	InitXObject();		//オブジェクト
	InitStart();		//スタートボタン
	InitFlontPolygon();	//フロントポリゴン

	//カメラ
	SetCamera(D3DXVECTOR3(0.0f, 550.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), -2000.0f, CAMERATYPE_TITLE);

	//空
	SetMeshCylinder();

	//壁
	SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//地面
	SetMeshField(FIELDTEXT_FIELD_1, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_2, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_3, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_4, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_5, FIELDTEXTURE_PARK);
	SetMeshField(FIELDTEXT_FIELD_6, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_7, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_8, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_9, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_OTHER_1, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_OTHER_2, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_OTHER_3, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_OTHER_4, FIELDTEXTURE_FIELD);
	SetMeshField(FIELDTEXT_FIELD_WALL_1, FIELDTEXTURE_WALLTOP2);
	SetMeshField(FIELDTEXT_FIELD_WALL_2, FIELDTEXTURE_WALLTOP2);
	SetMeshField(FIELDTEXT_FIELD_WALL_3, FIELDTEXTURE_WALLTOP);
	SetMeshField(FIELDTEXT_FIELD_WALL_4, FIELDTEXTURE_WALLTOP);
	SetMeshField(FIELDTEXT_FIELD_HUNSUI, FIELDTEXTURE_HUNSUI);

	//オブジェクト
	SetXObject(TEXTTYPE_SEARCH_HOUSE_0);
	SetXObject(TEXTTYPE_SEARCH_HOUSE_1);
	SetXObject(TEXTTYPE_SEARCH_HOUSE_2);
	SetXObject(TEXTTYPE_SEARCH_HOUSE_3);
	SetXObject(TEXTTYPE_SEARCH_HOUSE_4);
	SetXObject(TEXTTYPE_SEARCH_HOUSE_5);
	SetXObject(TEXTTYPE_SEARCH_HOUSE_6);
	SetXObject(TEXTTYPE_SEARCH_HOUSE_7);
	SetXObject(TEXTTYPE_SEARCH_HOUSE_8);

	//タイトル


	//ボタンのセット
	SetEnter(D3DXVECTOR3(630.0f, 650.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	SetTitleAnim();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitTitle(void)
{
	//関数呼び出し	
	UninitCamera();			//カメラ
	UninitMeshCylinder();	//空
	UninitMeshWall();		//壁
	UninitMeshField();		//地面
	UninitShadow();			//影
	UninitXObject();		//オブジェクト
	UninitStart();			//スタートボタン
	UninitFlontPolygon();	//フロントポリゴン

	DeleteTitleAnim();
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateTitle(void)
{
	//時間を加算する
	g_nCntTime++;
	g_nCntTimeTitle++;

	TitleAnim pTitleAnim = GetTitleAnim();

	//フェード情報を取得する
	FADE fade;
	fade = GetFade();

	//関数呼び出し
	UpdateCamera();			//カメラ
	UpdateMeshCylinder();	//空
	UpdateMeshWall();		//壁
	UpdateMeshField();		//地面
	UpdateShadow();			//影
	UpdateXObject();		//オブジェクト
	UpdateStart();			//スタートボタン
	UpdateFlontPolygon();	//フロントポリゴン
	
	//決定キーが押されたかどうか
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
	{
		if (fade != FADE_OUT)		//フェードアウトが実行されていないなら
		{
			if (pTitleAnim.nCntAnimNum != 118)
			{
				SetTitleAnimNum();
			}
			else
			{
				//スタートボタンの設定
				SetPush(STARTSTRACT_ENTER);

				//フェードモード設定
				SetFade(MODE_STORY);
				PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
			}
		}
	}
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawTitle(void)
{
	//関数呼び出し
	SetUpCamera();		//カメラ
	DrawMeshCylinder();	//空
	DrawMeshWall();		//壁
	DrawMeshField();	//地面
	DrawShadow();		//影
	DrawXObject();		//オブジェクト
	DrawStart();		//スタートボタン
	DrawFlontPolygon();	//フロントポリゴン
}