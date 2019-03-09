//=============================================================================
//
// ストーリーフォント処理 [storyfont.h]
// Author : 佐藤安純　Sato_Aumi
//
//=============================================================================
#ifndef _STORYFONT_H_
#define _STORYFONT_H_

#include "main.h"

//=============================================================================
// 状態の設定
//=============================================================================
typedef enum
{
	// クエスト1
	TYPESTORYFONT_01 = 0,	//クエスト１-依頼シーン
	TYPESTORYFONT_02,		//クエスト１-報告シーンランクA
	TYPESTORYFONT_03,		//クエスト１-報告シーンランクB&C
	TYPESTORYFONT_04,		//クエスト１-報告シーンランクD
	TYPESTORYFONT_05,		//クエスト１-ランクAシーン
	TYPESTORYFONT_06,		//クエスト１-ランクB&Cシーン
	TYPESTORYFONT_07,		//クエスト１-ランクDシーン

	// クエスト２
	TYPESTORYFONT_11,		//クエスト2-依頼シーン
	TYPESTORYFONT_12,		//クエスト2-報告シーンAランク
	TYPESTORYFONT_13,		//クエスト2-報告シーンB&Cランク
	TYPESTORYFONT_14,		//クエスト2-報告シーンDランク
	TYPESTORYFONT_15,		//クエスト2-ランクＡシーン
	TYPESTORYFONT_16,		//クエスト2-ランクB&Cシーン
	TYPESTORYFONT_17,		//クエスト2-ランクDシーン

	// クエスト３
	TYPESTORYFONT_21,		//クエスト3-依頼シーン
	TYPESTORYFONT_22,		//クエスト3-報告シーンAランク
	TYPESTORYFONT_23,		//クエスト3-報告シーンB&Cランク
	TYPESTORYFONT_24,		//クエスト3-報告シーンDランク
	TYPESTORYFONT_25,		//クエスト3-ランクAシーン
	TYPESTORYFONT_26,		//クエスト3-ランクB&Cシーン
	TYPESTORYFONT_27,		//クエスト3-ランクDシーン
	TYPESTORYFONT_MAX,
}TYPESTORYFONT;

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	TYPESTORYFONT	type;			//フォントのタイプ
	bool			bUse;			//使用しているかどうか
}StoryFont;

typedef struct
{
	char			Line[256];		//格納する文字列
	int				nTex;			//テクスチャの番号
	int				nIdx;			//ポリゴンのIDを設定
	bool			bUse;			//使用しているかどうか
}StoryLine;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitStoryFont(void);
void UninitStoryFont(void);
void UpdateStoryFont(void);
void DrawStoryFont(void);

void SetStoryFont(TYPESTORYFONT type);
bool GetStoryFont(void);

void DeleteStoryFont(void);

#endif