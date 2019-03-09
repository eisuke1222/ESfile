//=============================================================================
//
// 数字処理 [number.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene2d.h"	// シーン2D
#include "scene3d.h"	// シーン3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER2D_SIZE_X	(30)		// 大きさ（X)
#define NUMBER2D_SIZE_Y	(30)		// 大きさ（Y)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber
{// 数字
public:
	CNumber();									// コンストラクタ
	~CNumber();									// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャ情報へのポインタ

};

class CNumber2D : public CScene2D, CNumber
{// 数字2D（親：CScene2D）
public:
	CNumber2D();								// コンストラクタ
	~CNumber2D();								// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);		// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:

protected:

};

class CNumber3D : public CScene3D, CNumber
{// 数字3D（親：CScene3D）
public:
	CNumber3D();								// コンストラクタ
	~CNumber3D();								// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, CScene3D::SCENE3DTYPE screenType);				// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:

protected:

};

#endif