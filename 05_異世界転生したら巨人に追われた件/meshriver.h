//=============================================================================
//
// メッシュリバー処理 [meshriver.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MESHRIVER_H_
#define _MESHRIVER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "Scenemesh.h"	// シーンメッシュ

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshRiver : public CSceneMesh
{// メッシュフィールド（親：CSceneMesh）
public:
	typedef enum
	{
		TEXTURE_DORO = 0,
		TEXTURE_RIVER,
		TEXTURE_MAX
	}TEXTURE;

	CMeshRiver();									// コンストラクタ
	~CMeshRiver();									// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード
	static CMeshRiver *Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nNum);		// 生成

	HRESULT Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);					// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURE_MAX];			// テクスチャ情報へのポインタ
	float m_Sinrot;

protected:

};

#endif