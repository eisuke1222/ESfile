//=============================================================================
//
// シーンメッシュスキン処理 [scenemeshskin.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENEMESHSKIN_H_
#define _SCENEMESHSKIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		MAX_VTX	(6)
#define		MAX_OFFSET	(2)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneMeshSkin : public CScene
{// シーンメッシュスキン（親：CScene）
public:
	CSceneMeshSkin(int nPriority, CScene::OBJTYPE objType);		// コンストラクタ
	~CSceneMeshSkin();											// デストラクタ

	static HRESULT Load(void);									// ロード
	static void Unload(void);									// アンロード

																// クリエイト
	static CSceneMeshSkin *Create(D3DXVECTOR3 OffSet_0, D3DXVECTOR3 OffSet_1);

	HRESULT Init(void);											// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	void SetMtxParent(D3DXMATRIX *pMtx) { m_pMtxParent = pMtx; }// マトリックス設定

private:
	LPDIRECT3DTEXTURE9				m_pTexture;					// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;					// 頂点バッファ情報へのポインタ
	D3DXMATRIX						*m_pMtxParent;				// 親のマトリックスへのポインタ

	D3DXMATRIX						m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3						m_aPosVtx[MAX_VTX];			// 頂点位置
	D3DXVECTOR3						m_aOffSet[MAX_OFFSET];		// オフセット位置

protected:

};

#endif