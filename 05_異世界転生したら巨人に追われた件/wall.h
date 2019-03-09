//=============================================================================
//
// 壁処理 [wall.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene3d.h"		// シーン
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CWall : public CScene3D
{// シーン3D（親：CScene）
public:
	CWall();														// コンストラクタ
	~CWall();														// デストラクタ
																		
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理
																		// 生成
	static CWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
	static HRESULT Load(void);											// テクスチャの読み込み
	static void Unload(void);											// 破棄

	void Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMode, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;							// テクスチャ情報へのポインタ

protected:

};

#endif