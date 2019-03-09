//=============================================================================
//
// アイテムの取得時表記処理 [itemicon.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _ITEMICON_H_
#define _ITEMICON_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"			// メイン
#include "scene3d.h"		// シーン
#include "player.h"			// プレイヤー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEMICON_MAXTYPE (4)	// アイテムの種類最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CItemicon : public CScene3D
{// シーン3D（親：CScene）
public:
	typedef enum
	{
		ITEMICON_SPEEDUP = 0,		//	スピードUPアイテム
		ITEMICON_LIFEUP,			//	回復アイテム
		ITEMICON_SCOREUP,			//	スコア加算アイテム
		ITEMICON_SCOREUP2,			//	スコア加算2アイテム
		ITEMICON_MAX
	}ITEMICONTYPE;

	CItemicon();														// コンストラクタ
	~CItemicon();														// デストラクタ
																		// 初期化処理
	HRESULT Init(D3DXVECTOR3 pos,D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int itemType);
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理
																		// 生成
	static CItemicon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int itemType);
	static HRESULT Load(void);											// テクスチャの読み込み
	static void Unload(void);													// 破棄
private:
	static LPDIRECT3DTEXTURE9		m_pTexture[ITEMICON_MAXTYPE];				// テクスチャ情報へのポインタ
	ITEMICONTYPE			m_ItemIconType;								// アイテムの種類
	D3DXMATRIX				m_mtxWorld;									// ワールドマトリックス
	D3DXVECTOR2				m_TexUV;									// UV
	D3DXVECTOR3				m_move;										// 動き
	D3DXVECTOR3				m_posold;									// 過去の位置

protected:

};

#endif