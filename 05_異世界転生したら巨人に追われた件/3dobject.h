//=============================================================================
//
// 3Dobject処理 [3dobject.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _3DOBJECT_H_
#define _3DOBJECT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene3d.h"	// シーン
#include "player.h"		// プレイヤー

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class C3Dobject : public CScene3D
{// 3Dオブジェクト（親：CScene）
public:
	typedef enum
	{// 種類
		TYPE_001 = 0,	// 種類１
		TYPE_002,		// 種類２
		TYPE_003,		// 種類３
		TYPE_004,		// 種類４
		TYPE_005,		// 種類５
		TYPE_006,		// 種類６
		TYPE_007,		// 種類７
		TYPE_008,		// 種類８
		TYPE_009,		// 種類９
		TYPE_MAX
	}OBJECTTYPE;

	C3Dobject();														// コンストラクタ
	~C3Dobject();														// デストラクタ
																		// 初期化処理
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType);
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理
																		// 生成
	static C3Dobject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType);
	static HRESULT Load(void);											// テクスチャの読み込み
	static void Unload(void);											// 破棄

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TYPE_MAX];					// テクスチャ情報へのポインタ
	OBJECTTYPE					m_Type;									// アイテムの種類
	D3DXMATRIX					m_mtxWorld;								// ワールドマトリックス
	D3DXVECTOR2					m_TexUV;								// U

protected:

};

#endif