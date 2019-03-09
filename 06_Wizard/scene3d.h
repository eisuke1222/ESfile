//=============================================================================
//
// シーン3D処理 [scene3d.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

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
class CScene3D : public CScene
{// シーン3D（親：CScene）
public:
	typedef enum
	{// シーン3Dタイプ
		SCENE3DTYPE_NORMAL = 0,		// 通常
		SCENE3DTYPE_BILLBOARD,		// ビルボード
		SCENE3DTYPE_MAX				// 最大数
	}SCENE3DTYPE;

	CScene3D(int nPriority, CScene::OBJTYPE objType);	// コンストラクタ
	~CScene3D();										// デストラクタ

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)		// テクスチャ反映
		{ m_pTexture = pTexture; }		

	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

	// 描画編集
	void DrawEdit(bool bAdd, bool bSubtract, bool bATest, bool bZWrite, bool bLight, int nATest = 0, int nCullMode = 3, DWORD dCmpValue = D3DCMP_LESSEQUAL)
	{
		m_bDrawAdd		= bAdd;					// 加算合成
		m_bDrawSubtract	= bSubtract;			// 減産合成
		m_bDrawATest	= bATest;				// αテスト
		m_bDrawZWrite	= bZWrite;				// Zライト
		m_bDrawLight	= bLight;				// ライト影響
		m_nATest		= nATest;				// αテスト数値
		m_nCullMode		= nCullMode;			// カリング
		m_dCmpValue		= dCmpValue;			// Zライト設定
	}

	// 初期値設定
	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
	{
		m_pos			= pos;					// 位置
		m_rot			= rot;					// 向き
		m_size			= size;					// 大きさ
		m_col			= col;					// 色
		m_TexUV			= TexUV;				// テクスチャUV
		m_scene3dType	= scene3dType;			// シーン3Dタイプ
	}

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// 位置取得
	void Setrot(D3DXVECTOR3 rot)	{ m_rot = rot; }	// 向き設定
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// 向き取得
	void Setsize(D3DXVECTOR3 size)	{ m_size = size; }	// 大きさ設定
	D3DXVECTOR3 Getsize(void)		{ return m_size; }	// 大きさ取得
	void Setcol(D3DXCOLOR col)		{ m_col = col; }	// 色設定
	D3DXCOLOR Getcol(void)			{ return m_col; }	// 色取得

	void SetVtxSize(D3DXVECTOR3 *VtxSize);				// 頂点位置、大きさ調整
	void GetVtxSize(D3DXVECTOR3 *VtxSize);				// 頂点位置、大きさ取得

	void SetBillRot(float rot);							// ビルボード回転

														// アニメーション情報設定
	void SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV);		
	D3DXVECTOR2 GetTex(void)		{ return m_TexUV; }	// UV取得

	D3DXVECTOR3 GetVtx(int nNum);						// 頂点
	D3DXVECTOR3 GetNor(int nNum);						// 法線

	float GetHeight(D3DXVECTOR3 pos);					// 高さ取得

	void SetDraw(bool bDraw)	{ m_bDraw = bDraw; }	// 描画設定
	bool GetDraw(void)			{return m_bDraw; }		// 描画取得

private:
	LPDIRECT3DTEXTURE9		m_pTexture;					// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファ情報へのポインタ

	D3DXMATRIX				m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3				m_pos;						// 位置
	D3DXVECTOR3 			m_rot;						// 向き
	D3DXVECTOR3 			m_size;						// 大きさ
	D3DXCOLOR				m_col;						// 色
	D3DXVECTOR2 			m_TexUV;					// UV
							
	bool					m_bVtxSize;					// 頂点位置、大きさ調整した
							
	bool					m_bDraw;					// 描画

	bool					m_bDrawAdd;					// 加算合成
	bool					m_bDrawSubtract;			// 減産合成
	bool					m_bDrawATest;				// αテスト
	bool					m_bDrawZWrite;				// Zライト
	bool					m_bDrawLight;				// ライト影響
	int						m_nATest;					// αテスト数値
	int						m_nCullMode;				// カリング
	DWORD					m_dCmpValue;				// Zライト設定
							
	SCENE3DTYPE 			m_scene3dType;				// シーン3Dタイプ

protected:

};

#endif