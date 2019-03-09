//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene3d.h"	// シーン3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EXPLOSION_SIZE_X	(30)		// 大きさ（X)
#define EXPLOSION_SIZE_Y	(30)		// 大きさ（Y)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CExplosion : public CScene3D
{// 爆発（親：CScene3D）
public:
	typedef enum
	{// 爆発タイプ
		EXPLOSIONTYPE_NORMAL = 0,
		EXPLOSIONTYPE_DAMAGE,
		EXPLOSIONTYPE_MAX
	}EXPLOSIONTYPE;

	CExplosion();								// コンストラクタ
	~CExplosion();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, EXPLOSIONTYPE explosionType, int nShotIdx);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, EXPLOSIONTYPE explosionType, int nShotIdx);					// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャ情報へのポインタ

	int m_nAnimCounter;							// アニメーションカウンタ
	int m_AnimPattern;							// アニメーションパターン

	EXPLOSIONTYPE m_explosionType;				// 爆発タイプ
	int m_nShotIdx;								// 撃った人ID

protected:

};

#endif