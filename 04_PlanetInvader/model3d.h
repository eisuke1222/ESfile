//=============================================================================
//
// モデル3D処理 [model3D.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MODEL3D_H_
#define _MODEL3D_H_

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
class CModel3D : public CScene
{// モデル3D（親：CScene）
public:
	CModel3D(int nPriority);					// コンストラクタ
	CModel3D(int nPriority, int nID);			// コンストラクタ
	~CModel3D();								// デストラクタ
	
	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	void SetInitAll(LPDIRECT3DTEXTURE9 *pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	{// 初期値設定
		m_pTexture	= pTexture;			// テクスチャ
		m_pMesh		= pMesh;			// メッシュ
		m_pBuffMat	= pBuffMat;			// マテリアルバッファ
		m_nNumMat	= nNumMat;			// マテリアル数
		m_pos		= pos;				// 位置
		m_rot		= rot;				// 向き
	}

	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void) { return m_pos; }		// 位置取得
	void Setrot(D3DXVECTOR3 rot) { m_rot = rot; }	// 向き設定
	D3DXVECTOR3 Getrot(void) { return m_rot; }		// 向き取得

	D3DXVECTOR3 Getsize(int nNum) { if (nNum == 0) return m_vtxMax;	else return m_vtxMin; }		// 大きさ取得
	void Setcol(D3DXCOLOR col) { m_col = col; m_bcolChange = true; }							// 色設定

private:

	LPDIRECT3DTEXTURE9	*m_pTexture = NULL;				// テクスチャ情報へのポインタ
	LPD3DXMESH			m_pMesh = NULL;					// メッシュ情報へのポインタ
	LPD3DXBUFFER		m_pBuffMat = NULL;				// マテリアル情報へのポインタ
	DWORD				m_nNumMat = NULL;				// マテリアル情報の数
	D3DXMATRIX			m_mtxWorld;						// ワールドマトリックス

	D3DXVECTOR3			m_vtxMin;						// 頂点の最小値
	D3DXVECTOR3			m_vtxMax;						// 頂点の最大値

	D3DXVECTOR3			m_pos;							// 位置
	D3DXVECTOR3			m_rot;							// 向き
	D3DXCOLOR			m_col;							// 色
	bool				m_bcolChange;					// 色変更

protected:

};

#endif