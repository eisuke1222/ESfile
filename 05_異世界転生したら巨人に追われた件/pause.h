//*****************************************************************************
//
//				ポーズの処理[pause.h]
//				Auther : Shun Yokomichi
//
//*****************************************************************************
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;				// シーン2D
class CPauseBg;				// ポーズの背景
class CPress;				//　ボタン

//*****************************************************************************
//		マクロ定義
//*****************************************************************************
#define POLYGON_PAUSE	(3)		// ポーズのtexture数
#define POLYGON_PRESS	(5)

//*****************************************************************************
//		クラス定義(ポーズ)
//*****************************************************************************
class CPause
{
public:
	CPause();
	~CPause();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 UV);		// 初期化
	void Uninit(void);					// 終了
	void Update();						// 更新
	void Draw(void);					// 描画
	
	// 静的メンバ関数
	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 UV);	// 生成
	static HRESULT Load(void);				// 読み込み
	static void Unload(void);				// texture破棄

private:
	int m_nNum;								// 番号
	float m_fColA;							//透明度
	CScene2D *m_apScene2D[POLYGON_PAUSE];
	CPauseBg *m_pPauseBg;
	CPress   *m_pPress[POLYGON_PRESS];

	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[POLYGON_PAUSE];
};

#endif

