//=============================================================================
//
// テクスチャアニメーション処理 [texanim.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TEXANIM_H_
#define _TEXANIM_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene2d.h"	// シーン2D
#include "scene3d.h"	// シーン3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTexAnim : public CScene
{// テクスチャアニメーション（親：CScene）
public:
	typedef enum
	{// 種類
		TYPE_2D = 0,	// 2D
		TYPE_3D,		// 3D
		TYPE_3D_BILL,	// 3D_Bill
		TYPE_MAX		// 最大数
	}TYPE;

	CTexAnim();												// コンストラクタ
	virtual ~CTexAnim();									// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

															// クリエイト
	static CTexAnim *Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR2 texUV, D3DXCOLOR col, LPDIRECT3DTEXTURE9 pTexture);

	virtual HRESULT Init(void) = 0;							// 初期化処理
	virtual void Uninit(void);								// 終了処理
	virtual void Update(void) = 0;							// 更新処理
	virtual void Draw(void) = 0;							// 描画処理

	// アニメーション設定
	void SetAnim(int nMaxAnim, int nMaxLine, int nSpped = 0, int nPattern = 0, bool bLoop = false, bool bAutoAnim = false)
	{
		m_bAutoAnim = bAutoAnim;	// 自動アニメーション
		m_bLoop		= bLoop;		// ループ
		m_nMaxAnim	= nMaxAnim;		// アニメーション数
		m_nMaxLine	= nMaxLine;		// ライン数
		m_nCntAnim	= 0;			// アニメーションカウント	
		m_nSpeed	= nSpped;		// アニメーション速度
		m_nPattern	= nPattern;		// パターン数
	}

private:

protected:
	int					m_nMax;								// 最大数
	D3DXVECTOR3			m_pos;								// 位置
	D3DXVECTOR3			m_move;								// 移動量
	D3DXVECTOR3			m_size;								// 大きさ
	D3DXVECTOR2			m_texUV;							// UV			
	D3DXCOLOR			m_col;								// 色

	LPDIRECT3DTEXTURE9	m_pTexture;					
	bool				m_bAutoAnim;						// 自動アニメーション
	bool				m_bLoop;							// ループ
	int					m_nMaxAnim;							// アニメーション数
	int					m_nMaxLine;							// ライン数
	int					m_nCntAnim;							// アニメーションカウント	
	int					m_nSpeed;							// アニメーション速度
	int					m_nPattern;							// パターン数

	bool				m_bBillBoard;						// ビルボードか
};

class CAnim2D : public CTexAnim
{// アニメーション2D（親：CTexAnim）
public:
	CAnim2D();												// コンストラクタ
	~CAnim2D();												// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

private:
	CScene2D			*m_pAnim2D;							// アニメーション2D

protected:

};

class CAnim3D : public CTexAnim
{// アニメーション3D（親：CTexAnim）
public:
	CAnim3D();												// コンストラクタ
	~CAnim3D();												// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

private:
	CScene3D			*m_pAnim3D;							// アニメーション3D

protected:

};

#endif