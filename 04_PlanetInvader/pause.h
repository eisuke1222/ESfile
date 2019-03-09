//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_MAX_BG		(4)			// ポーズの最大数
#define PAUSE_MAX_SELECT	(3)			// ポーズの最大数
#define PAUSE_SIZE_X		(160)		// 大きさ（X)
#define PAUSE_SIZE_Y		(50)		// 大きさ（Y)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPause : public CScene
{// ポーズ（親：CScene）
public:
	CPause();									// コンストラクタ
	~CPause();									// デストラクタ

	typedef enum
	{
		TEXTURE_PAUSE_BG = 0,
		TEXTURE_PAUSE,
		TEXTURE_TITLE,
		TEXTURE_MAX
	}TEXTURE;

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CPause *Create(void);				// 生成

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURE_MAX];		// テクスチャ情報へのポインタ
	CScene2D *m_apScene2D[PAUSE_MAX_BG];					// シーン2D
	CScene2D *m_apScene2DSelect[PAUSE_MAX_SELECT];			// シーン2D

	int m_nSelectNum;							// 選択番号
	int m_nCntTimer;							// 時間カウンタ

protected:

};

#endif