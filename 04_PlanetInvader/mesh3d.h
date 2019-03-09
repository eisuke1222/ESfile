//=============================================================================
//
// メッシュ3D処理 [mesh3D.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MESH3D_H_
#define _MESH3D_H_

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
class CMesh3D : public CScene
{// メッシュ3D（親：CScene）
public:
	CMesh3D(int nPriority);						// コンストラクタ
	CMesh3D(int nPriority, int niD);			// コンストラクタ
	~CMesh3D();									// デストラクタ
	
	typedef enum
	{// メッシュタイプ
		MESHTYPE_FIELD = 0,			// フィールド
		MESHTYPE_WALL,				// ウォール
		MESHTYPE_CYLINDER,			// シリンダー
		MESHTYPE_MAX
	}MESHTYPE;

	typedef enum
	{// テクスチャタイプ
		MESHTEX_ALL = 0,			// 全面
		MESHTEX_NUMSIZE,			// 分割
		MESHTEX_MAX
	}MESHTEX;

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }		// テクスチャ反映

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	void SetInitAll(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);		// 初期値設定

	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void) { return m_pos; }		// 位置取得

	void SetTex(D3DXVECTOR2 TexMoveUV);				// アニメーション情報設定

private:

	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファ情報へのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;			// インデックス情報へのポインタ
	D3DXMATRIX	m_mtxWorld;						// ワールドマトリックス
	int m_nMaxVtx;								// 頂点の最大数
	int m_nMaxIdx;								// インデックスの最大数
	int m_nMaxPolygon;							// ポリゴンの最大数

	MESHTYPE m_meshType;						// メッシュタイプ
	MESHTEX m_meshTex;							// テクスチャタイプ
	D3DXVECTOR3 m_num;							// 枚数
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR2 m_TexUV;						// UV

protected:

};

#endif