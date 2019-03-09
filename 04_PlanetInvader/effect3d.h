//=============================================================================
//
// エフェクト3D処理 [effect3d.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "model3d.h"	// モデル3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffect3D : public CModel3D
{// オブジェクト（親：CModel3D）
public:
	CEffect3D();									// コンストラクタ
	~CEffect3D();									// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード
	static CEffect3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 rotAdd, D3DXVECTOR3 move);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 rotAdd, D3DXVECTOR3 move);					// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;		// テクスチャ情報へのポインタ
	static LPD3DXMESH			m_pMesh;			// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat;			// マテリアル情報へのポインタ
	static DWORD				m_nNumMat;			// マテリアル情報の数

	int			m_nID;								// 親ID
	int			m_nCntTimer;						// 時間カウンタ
	D3DXVECTOR3 m_rotAdd;							// 向き
	D3DXVECTOR3 m_move;								// 移動量
	bool		m_bStop;							// 止まる
protected:

};

#endif