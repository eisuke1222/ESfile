//=============================================================================
//
// 背景処理 [bg.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_SIZE_X		(10)		// 大きさ（X)
#define BG_SIZE_Y		(10)		// 大きさ（Y)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBg : public CScene
{// 背景（親：CScene）
public:
	CBg();										// コンストラクタ
	~CBg();										// デストラクタ

	typedef enum
	{// テクスチャ
		TEXTURE_TUTORIAL = 0,
		TEXTURE_GAME,
		TEXTURE_MAX
	}TEXTURE;

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CBg *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nSetNum, int nTexNum);		// 生成

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURE_MAX];		// テクスチャ情報へのポインタ
	CScene2D *m_apScene2D[TEXTURE_MAX];			// シーン2D

	D3DXVECTOR2 m_posUV[TEXTURE_MAX];			// 位置
	static int m_nSetNum;						// セット数


protected:

};

#endif