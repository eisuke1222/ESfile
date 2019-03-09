//=============================================================================
//
// シーンメッシュオービット処理 [scenemeshorbit.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENEMESHOBRBIT_H_
#define _SCENEMESHOBRBIT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneMeshOrbit : public CScene
{// シーンメッシュオービット（親：CScene）
public:
	const static int m_MAXOFFSET = 2;						// オフセット数

	CSceneMeshOrbit(int nPriority, CScene::OBJTYPE objType);// コンストラクタ
	~CSceneMeshOrbit();										// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

															// クリエイト
	static CSceneMeshOrbit *Create(D3DXVECTOR3 OffSet_0, D3DXVECTOR3 OffSet_1, D3DXVECTOR3 col_rgb, float fMaxcol_a, float fMincol_a, int nMaxVtx, LPDIRECT3DTEXTURE9 pTexture = NULL);

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	void SetMtxParent(D3DXMATRIX *pMtx)						// マトリックス設定
		{ m_pMtxParent = pMtx; }

	void Setcol(D3DXVECTOR3 col_rgb)						// 色設定
		{ m_col_rgb = col_rgb; }

private:
	LPDIRECT3DTEXTURE9			m_pTexture;					// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					// 頂点バッファ情報へのポインタ
	D3DXMATRIX					*m_pMtxParent;				// 親のマトリックスへのポインタ

	int							m_nMaxVtx;					// 最大頂点数

	D3DXVECTOR3					*m_aPosVtx;					// 頂点位置
	D3DXVECTOR3					m_aOffSet[m_MAXOFFSET];		// オフセット位置
	D3DXVECTOR3					m_col_rgb;					// 色
	float						m_fMaxcol_a;				// 最大透明度
	float						m_fMincol_a;				// 最小透明度

protected:

};

#endif