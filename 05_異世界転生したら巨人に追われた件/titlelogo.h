//=============================================================================
//
// タイトルロゴ処理 [titlelogo.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene2d.h"	// シーン2D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LOGO	(2)		// 最大数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitleLogo : public CScene2D
{// タイトルロゴ（親：CScene2D）
public:
	typedef enum
	{
		TYPE_NONE = 0,	// 何もない
		TYPE_LOGO,		// タイトルロゴ
		TYPE_CREDIT,	// クレジット
		TYPE_MAX		// 総数
	}TYPE;

	CTitleLogo();								// コンストラクタ
	~CTitleLogo();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CTitleLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, int nTex);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);					// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	static int GetPattern(void) { return m_Pattern; };
	static void SetPattern(int nPattern) { m_Pattern = nPattern; };

	static int GetData(void) { return m_nData; };
	static void SetGetData(int nPattern) { m_Pattern = m_nData; };

private:
	static int m_Pattern;
	static int m_nData;
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_LOGO];		// テクスチャ情報へのポインタ
	D3DXVECTOR3 m_size;							// サイズの大きさ
	int m_nTime;
	float m_fColA;								// 透明度
	TYPE m_type;								// タイプ

protected:

};

#endif