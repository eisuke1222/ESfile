//=============================================================================
//
// 雲処理 [cloud.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _CLOUD_H_
#define _CLOUD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene3d.h"	// シーン3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLOUD_SIZE_X	(30)		// 大きさ（X)
#define CLOUD_SIZE_Y	(30)		// 大きさ（Y)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCloud : public CScene3D
{// 雲（親：CScene3D）
public:
	CCloud();										// コンストラクタ
	~CCloud();										// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード
	static CCloud *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType);	// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType);			// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャ情報へのポインタ

	D3DXVECTOR3 m_pos;								// 位置

protected:

};

#endif