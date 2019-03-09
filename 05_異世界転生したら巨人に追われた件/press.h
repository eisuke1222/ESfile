//*****************************************************************************
//
//				プレスボタンの処理[press.h]
//				Auther : Shun Yokomichi
//
//*****************************************************************************
#ifndef _PRESS_H_
#define _PRESS_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"		// シーン2D

//*****************************************************************************
//		マクロ定義
//*****************************************************************************
#define MAX_TEX_PRESS	(10)		// 最大数

//*****************************************************************************
//		クラス定義(ポリゴン)
//*****************************************************************************
class CPress : public CScene2D
{
public:
	typedef enum
	{
		TYPE_NONE = 0,		// 何もない状態
		TYPE_TRANSPARENCE,	// 不透明から透明(自動)
		TYPE_OPAQUE,		// 透明から不透明(自動)
		TYPE_MANUAL,		// 強制的に透明にする4
		TYPE_MAX			// 総数
	}TYPE;

	CPress();
	~CPress();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nTex, float rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetType(TYPE type)
	{
		m_type = type;
	}

	void SetTex(int nTex)
	{
		CScene2D::BindTexture(m_pTexture[nTex]);
	}

	// 静的メンバ関数
	static CPress * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nTex, float rot);
	static HRESULT Load(void);	// 読み込む関数(テクスチャ)
	static void Unload(void);	// 開放する関数(テクスチャ)

private:
	float m_fColA;		// 透明度保管用
	TYPE m_type;		// 透明度のタイプ管理用

						// 静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEX_PRESS];
};

#endif
