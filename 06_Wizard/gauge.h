//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

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
class CGauge : public CScene
{// ゲージ（親：CScene）
public:
	typedef enum
	{// 種類
		TYPE_2D = 0,	// 2D
		TYPE_3D,		// 3D
		TYPE_MAX		// 最大数
	}TYPE;

	typedef enum
	{// ゲージ
		GAUGE_BG = 0,	// 背景
		GAUGE_BAR,		// バー
		GAUGE_MAX		// 最大数
	}GAUGE;

	CGauge();												// コンストラクタ
	virtual ~CGauge();										// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

															// クリエイト
	static CGauge *Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);	

	virtual HRESULT Init(void) = 0;							// 初期化処理
	virtual void Uninit(void);								// 終了処理
	virtual void Update(void) = 0;							// 更新処理
	virtual void Draw(void) = 0;							// 描画処理

															// バー設定
	virtual void SetBar(D3DXVECTOR3 pos, int nNow, int nMax) = 0;	

private:

protected:
	D3DXVECTOR3 m_pos;										// 位置
	D3DXVECTOR3 m_size;										// 大きさ
	D3DXCOLOR	m_col;										// 色

};

class CGauge2D : public CGauge
{// ゲージ2D（親：CGauge）
public:
	CGauge2D();												// コンストラクタ
	~CGauge2D();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	void SetBar(D3DXVECTOR3 pos, int nNow, int nMax);		// バー設定

private:
	CScene2D	*m_pGause2D[TYPE_MAX];						// ゲージ2D

protected:

};

class CGauge3D : public CGauge
{// ゲージ3D（親：CGauge）
public:
	CGauge3D();												// コンストラクタ
	~CGauge3D();											// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	void SetBar(D3DXVECTOR3 pos, int nNow, int nMax);		// バー設定

private:
	CScene3D	*m_pGause3D[TYPE_MAX];						// ゲージ3D

protected:

};

#endif