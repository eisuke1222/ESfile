//=============================================================================
//
// タイトルフレーム処理 [titleframe.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _TITLEFRAME_H_
#define _TITLEFRAME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene2d.h"	// シーン2D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FRAME		(2)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitleFrame : public CScene2D
{// タイトルフレーム（親：CScene2D）
public:
	CTitleFrame();								// コンストラクタ
	~CTitleFrame();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CTitleFrame *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float rot, int nTex);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float rot);					// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	D3DXVECTOR3 m_movesize;						// サイズの大きさ変更
	D3DXVECTOR3 m_msize;						// サイズの大きさ変更

												// 静的メンバ変数
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_FRAME];		// テクスチャ情報へのポインタ


protected:

};

#endif