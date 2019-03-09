//*****************************************************************************
//
//				ライフの処理[life.h]
//				Auther : Shun Yokomichi
//
//*****************************************************************************
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
//		マクロ定義
//*****************************************************************************
#define		MAX_LIFE		(3)		// ライフの最大数

//*****************************************************************************
//		クラス定義(ライフ)
//*****************************************************************************
class CLife : public CScene
{
public:
	CLife();
	~CLife();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//	静的メンバ関数
	static HRESULT Load(void);	//	読み込む関数(テクスチャ)
	static void Unload(void);	// 開放する関数(テクスチャ)
	static CLife * Create();
	static void HitDamage();		// ダメージ用
	static void Recovery();			// 回復用
	static int GetLife() { int nLife = 0; for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++) { if (m_bUse[nCntLife]) { nLife++; } } return nLife; };			// 回復用

private:
	static int m_nLife;

	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャへのポインタ
	static bool m_bUse[MAX_LIFE];
	CScene2D *m_apScene[MAX_LIFE];		// CScene2Dのポインタ

};

#endif
