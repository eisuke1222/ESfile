//=============================================================================
//
// シーン2D処理 [scene2d.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

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
class CScene2D : public CScene
{// シーン2D（親：CScene）
public:
	CScene2D(int nPriority, CScene::OBJTYPE objType);			// コンストラクタ
	~CScene2D();												// デストラクタ

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)				// テクスチャ反映
		{ m_pTexture = pTexture; }
	
	HRESULT Init(void);											// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

																// 描画編集
	void DrawEdit(bool bAdd, bool bSubtract, bool bATest, int nATest = 0)
	{
		m_bDrawAdd		= bAdd;									// 加算合成
		m_bDrawSubtract	= bSubtract;							// 減産合成
		m_bDrawATest	= bATest;								// αテスト
		m_nATest		= nATest;								// αテスト数値
	}

																// 初期値設定
	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV)
	{
		m_pos			= pos;									// 位置							
		m_size			= size;									// 大きさ
		m_col			= col;									// 色
		m_TexUV			= TexUV;								// UV
	}

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }			// 位置設定
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }			// 位置取得
	void Setrot(float rot)			{ m_rot = rot; }			// 向き設定
	float Getrot(void)				{ return m_rot; }			// 向き取得
	void Setsize(D3DXVECTOR3 size)	{ m_size = size; }			// 大きさ設定
	D3DXVECTOR3 Getsize(void)		{ return m_size; }			// 大きさ取得
	void Setcol(D3DXCOLOR col)		{ m_col = col; }			// 色設定
	D3DXCOLOR Getcol(void)			{ return m_col; }			// 色取得
	void SetTexUV(D3DXVECTOR2 TexUV){ m_TexUV = TexUV; }		// 色設定
	D3DXVECTOR2 GetTexUV(void)		{ return m_TexUV; }			// 色取得

																// アニメーション情報設定
	void SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;							// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// 頂点バッファ情報へのポインタ

	D3DXVECTOR3				m_pos;								// 位置
	float					m_rot;								// 向き
	D3DXVECTOR3				m_size;								// 大きさ
	D3DXCOLOR				m_col;								// 色
	D3DXVECTOR2				m_TexUV;							// UV

	bool					m_bDrawAdd;							// 加算合成
	bool					m_bDrawSubtract;					// 減産合成
	bool					m_bDrawATest;						// αテスト
	int						m_nATest;							// αテスト数値

protected:

};

class CScene2D_Preset : public CScene2D
{// シーン2Dプリセット（親：CScene2D）
public:
	typedef enum
	{// 種類
		TYPE_NONE = 0,	// 何もない
		TYPE_COL,		// 色
		TYPE_SIZE,		// 大きさ
		TYPE_FLASING,	// 点滅
		TYPE_MAX		// 最大数
	}TYPE;

	CScene2D_Preset(int nPriority, CScene::OBJTYPE objType);	// コンストラクタ
	~CScene2D_Preset();											// デストラクタ

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)				// テクスチャ反映
		{ CScene2D::BindTexture(pTexture); }

	HRESULT Init(void);											// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	void SetcolType(int nMaxFrame, D3DXCOLOR colDest)			// 色設定
	{ for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++) 
		{ if (m_nType[nCntType] == TYPE_NONE) 
			{ m_nType[nCntType] = TYPE_COL; m_nCntFrame[nCntType] = 0; m_nMaxFrame[nCntType] = nMaxFrame; m_colDest_1[nCntType] = colDest;  break; }
		}
	}
	void SetSizeType(int nMaxFrame, D3DXVECTOR3 sizeDest)		// 大きさ設定
	{ for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++) 
		{ if (m_nType[nCntType] == TYPE_NONE) 
			{ m_nType[nCntType] = TYPE_SIZE; m_nCntFrame[nCntType] = 0; m_nMaxFrame[nCntType] = nMaxFrame; m_sizeDest[nCntType] = sizeDest;  break; }
		}
	}

private:
	void Preset(void);											// プリセット

	int						m_nType[TYPE_MAX];					// 種類				
	int						m_nCntFrame[TYPE_MAX];				// フレームカウント
	int						m_nMaxFrame[TYPE_MAX];				// フレームの最大値
	D3DXCOLOR				m_colDest_1[TYPE_MAX];				// 目標の色1
	D3DXCOLOR				m_colDest_2[TYPE_MAX];				// 目標の色2
	D3DXVECTOR3				m_sizeDest[TYPE_MAX];				// 目標の大きさ

protected:

};

#endif