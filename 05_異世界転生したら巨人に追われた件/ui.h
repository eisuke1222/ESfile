//*****************************************************************************
//
//				ポリゴンの処理[polygon.h]
//				Auther : Shun Yokomichi
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
//		マクロ定義
//*****************************************************************************
#define MAX_POLYGON			(5)		// Textureの最大数

//*****************************************************************************
//		クラス定義(ポリゴン)
//*****************************************************************************
class CUiPolygon : public CScene2D
{
public:
	CUiPolygon();
	~CUiPolygon();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nTexNum);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CUiPolygon * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nTexNum);
	static HRESULT Load(void);	//	読み込む関数(テクスチャ)
	static void Unload(void);	// 開放する関数(テクスチャ)
	static void SetA(float colA) { m_colA = colA; };

private:
	int m_nTexNum;

	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_POLYGON];
	static float m_colA;
};

#endif
