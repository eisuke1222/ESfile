//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAUGE_MAX_TEXTURE	(2)			// テクスチャ数
#define GAUGE_SIZE_X		(80)		// 大きさ（X)
#define GAUGE_SIZE_Y		(20)		// 大きさ（Y)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;			// シーン2D

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGauge : public CScene
{// ゲージ（親：CScene）
public:
	CGauge();									// コンストラクタ
	~CGauge();									// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CGauge *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nGaugeNum);		// 生成

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	static void SetReset(int nPlayerNum, bool bReset) { m_bResetGauge[nPlayerNum] = bReset; }

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;		// テクスチャ情報へのポインタ
	CScene2D *m_apScene2D[GAUGE_MAX_TEXTURE];	// シーン2D

	int m_nGaugeNum;							// ゲージ番号

	static bool m_bResetGauge[MAX_PLAYER];					// リセット
	D3DXVECTOR3 m_pos[MAX_PLAYER][GAUGE_MAX_TEXTURE];		// 位置
	D3DXVECTOR3 m_size[MAX_PLAYER][GAUGE_MAX_TEXTURE];		// 大きさ

protected:

};

#endif