//=============================================================================
//
// アイテム処理 [item.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene3d.h"		// シーン
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEM_MAXTYPE (4)	// アイテムの種類最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CItem : public CScene3D
{// シーン3D（親：CScene）
public:
	typedef enum
	{
		TYPE_SPEEDUP = 0,		//	スピードUPアイテム
		TYPE_LIFEUP,			//	回復アイテム
		TYPE_SCOREUP,			//	スコア加算アイテム
		TYPE_SCOREUP2,			//	スコア加算2アイテム
		TYPE_MAX
	}ITEMTYPE;

	CItem();															// コンストラクタ
	~CItem();															// デストラクタ
																		// 初期化処理
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int itemType);
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理
																		// 生成
	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int itemType);
	static HRESULT Load(void);											// テクスチャの読み込み
	static void Unload(void);													// 破棄
																				// 当たり判定
	bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin);
private:
	static LPDIRECT3DTEXTURE9		m_pTexture[ITEM_MAXTYPE];					// テクスチャ情報へのポインタ
	ITEMTYPE				m_ItemType;									// アイテムの種類
	D3DXMATRIX				m_mtxWorld;									// ワールドマトリックス
	D3DXVECTOR2				m_TexUV;									// U
	CPlayer					*m_pPlayer;									// プレイヤーのポインタ型

	int m_nCntAnim, m_nPatternAnim;										//アニメーションカウンター・パターン
	bool m_Anim;														//アニメーションパターンが足されるか引かれるか
protected:

};

#endif