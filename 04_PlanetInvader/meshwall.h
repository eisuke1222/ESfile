//=============================================================================
//
// メッシュウォール処理 [meshwall.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "mesh3d.h"		// メッシュ3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHWALL_SIZE_X		(30)		// 大きさ（X)
#define MESHWALL_SIZE_Y		(30)		// 大きさ（Y)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshWall : public CMesh3D
{// メッシュウォール（親：CMesh3D）
public:
	CMeshWall();								// コンストラクタ
	~CMeshWall();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CMeshWall *Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);	// 生成

	HRESULT Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);				// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャ情報へのポインタ

protected:

};

#endif