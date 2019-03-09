//=============================================================================
//
// メッシュシリンダー処理 [meshcylinder.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "mesh3d.h"		// メッシュ3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHCYLINDER_MAX_TEXTURE	(3)			// テクスチャ最大数
#define MESHCYLINDER_SIZE_X			(30)		// 大きさ（X)
#define MESHCYLINDER_SIZE_Y			(30)		// 大きさ（Y)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshCylinder : public CMesh3D
{// メッシュシリンダー（親：CMesh3D）
public:
	CMeshCylinder();							// コンストラクタ
	~CMeshCylinder();							// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CMeshCylinder *Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nTexNum, D3DXVECTOR2 TexMoveUV, MESHTEX meshTex);		// 生成

	HRESULT Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV, MESHTEX meshTex);				// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MESHCYLINDER_MAX_TEXTURE];		// テクスチャ情報へのポインタ

	D3DXVECTOR2 m_posUV;						// 位置
	D3DXVECTOR2 m_moveUV;						// 移動量

protected:

};

#endif