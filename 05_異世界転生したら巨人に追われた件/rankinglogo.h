//=============================================================================
//
// ランキングロゴ処理 [rankinglogo.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _RANKINGLOGO_H_
#define _RANKINGLOGO_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene2d.h"	// シーン2D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_RANK_LOGO	(6)		// ロゴの最大数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRnkingLogo : public CScene2D
{// ランキングロゴ（親：CScene2D）
public:
	CRnkingLogo();								// コンストラクタ
	~CRnkingLogo();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CRnkingLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTexNum);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);					// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	float m_fColA;								// 透明度の調整

	//	静的メンバ変数
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_RANK_LOGO];		// テクスチャ情報へのポインタ
	D3DXVECTOR3 m_size;							// サイズの大きさ


protected:

};

#endif