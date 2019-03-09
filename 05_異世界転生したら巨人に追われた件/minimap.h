//=============================================================================
//
// ミニマップ処理 [minimap.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include	"scene2d.h"		// シーン2D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MINI	(2)		// 最大数

//*****************************************************************************
// クラス定義	(CScene2Dの派生)
//*****************************************************************************
class CMinimap : public CScene2D
{
public:
	typedef enum
	{
		TYPE_NONE = 0,		// 何もない状態
		TYPE_PLAYER,		// プレイヤー
		TYPE_ENEMY,			// 敵
		TYPE_MAX
	}TYPE;		// 画像のタイプ

	CMinimap();									// コンストラクタ
	~CMinimap();								// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type, int nTex);		// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	static CMinimap *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type, int nTex);		// 生成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_MINI];		// テクスチャ情報へのポインタ
	TYPE m_Type;
	float m_fPosX;
};

#endif // !_MINIMAP_H_

