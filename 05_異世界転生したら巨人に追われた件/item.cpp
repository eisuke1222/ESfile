//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"		// シーン3D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "font.h"			// フォント
#include "camera.h"			// カメラ
#include "item.h"			// アイテム
#include "itemicon.h"		// アイテムのアイコン
#include "life.h"			// 体力表示
#include "score.h"			// スコア
#include "effect.h"			// エフェクト
#include "sound.h"			// サウンド

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CItem::m_pTexture[ITEM_MAXTYPE] = {};

//=============================================================================
// コンストラクタ								(public)	*** CItem ***
//=============================================================================
CItem::CItem() : CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_ITEM)
{
	m_ItemType = {};
}

//=============================================================================
// デストラクタ									(public)	*** CItem ***
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
//	アイテムの生成
//=============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int itemType)
{
	CItem *pItem = {};

	if (pItem == NULL)
	{//	アイテムの生成
		pItem = new CItem;
		if (pItem != NULL)
		{//アイテムの初期化
			pItem->Init(pos, rot, size, col, TexUV, itemType);
		}
	}
	return pItem;
}

//=============================================================================
// 初期化処理									(public)	*** CItem ***
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int itemType)
{
	// 初期値設定
	CScene3D::SetInitAll(pos, rot, size, col, TexUV, CScene3D::SCENE3DTYPE_BILLBOARD);
	// 初期化処理
	CScene3D::Init();
	// テクスチャ設定
	CScene3D::BindTexture(m_pTexture[itemType]);

	m_ItemType = (ITEMTYPE)itemType;	// テクスチャ種類

	m_nCntAnim = 0;			// アニメーションカウント
	m_nPatternAnim = 0;		// アニメーションパターン
	m_Anim = false;			// アニメーション
	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CItem ***
//=============================================================================
void CItem::Uninit(void)
{
	CScene3D::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CItem ***
//=============================================================================
void CItem::Update(void)
{
	D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// 視点取得
	D3DXVECTOR3 pos = CScene3D::Getpos();		// 位置取得
	D3DXVECTOR3 size = CScene3D::Getsize();		// 大きさ取得

	/* アニメーション処理 */
	m_nCntAnim++;
	if (m_nCntAnim % 10 == 0)
	{// 一定フレームになったら
		if (m_Anim == false)
		{// アニメーションしない
			m_nPatternAnim++;		// パターンカウント

			if (m_nPatternAnim >= 5)
			{// パターンが一定以上
				m_Anim = true;		// アニメーションする
			}
		}
		else
		{// アニメーションする
			m_nPatternAnim--;		// パターンカウント
			if (m_nPatternAnim <= 0)
			{
				m_Anim = false;
			}
		}
	}

	// テクスチャ設定
	SetTex(5, m_nPatternAnim, D3DXVECTOR2(0.2f, 0.25f), D3DXVECTOR2(1.0f, 1.0f));

	if (pos.x + size.x < posVDest.x - 500.0f)
	{// 一定以上離れたら
		Uninit();		// 終了処理
	}

}

//=============================================================================
// 描画処理										(public)	*** CItem ***
//=============================================================================
void CItem::Draw(void)
{
	CScene3D::Draw();	// 描画処理
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CItem::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item\\speedup.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item\\lifeup.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item\\Treasure000.png", &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\item\\Treasure002.png", &m_pTexture[3]);

	return S_OK;
}
//=============================================================================
// アンロード処理								(public)	*** CItem ***
//=============================================================================
void CItem::Unload(void)
{
	for (int nCount = 0; nCount < ITEM_MAXTYPE; nCount++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
//==============================================================================================================================
// アイテムのあたり判定の作成
//==============================================================================================================================
bool  CItem::CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	CSound *pSound = CManager::GetSound();

	bool bHit = false;
	D3DXVECTOR3 pos = CScene3D::Getpos();
	D3DXVECTOR3 size = CScene3D::Getsize();
	if (pos.x + (size.x / 2) >= pPos->x + sizeMin.x &&		// 上
		pos.x - (size.x / 2) <= pPos->x + sizeMax.x &&		// 下
		pos.z + (size.z / 2) >= pPos->z + sizeMin.z &&		// 右
		pos.z - (size.z / 2) <= pPos->z + sizeMax.z &&		// 左
		pos.y + (size.y/ 2) >= pPos->y + sizeMin.y &&		// 手前
		pos.y - (size.y*1.5) <= pPos->y + sizeMax.y)			// 奥
	{// アイテムにぶつかった場合

		if (m_ItemType == TYPE_SPEEDUP)
		{//	スピードアイテムだったら
			bHit = true;
			Uninit();
			CEffect::Create(pos, 3, 6);//　テクスチャ　テキスト
			CItemicon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), TYPE_SPEEDUP);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE004);
		}
		else if (m_ItemType == TYPE_LIFEUP)
		{//	回復アイテムだったら
			CLife::Recovery();
			Uninit();
			CEffect::Create(pos, 3, 8);//　テクスチャ　テキスト
			CItemicon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), TYPE_LIFEUP);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE007);

		}
		else if (m_ItemType == TYPE_SCOREUP)
		{//	スコア加算アイテムだったら
			CScore::AddScore(200);
			Uninit();
			CEffect::Create(pos, 3, 7);//　テクスチャ　テキスト
			CItemicon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), TYPE_SCOREUP);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE006);
		}
		else if (m_ItemType == TYPE_SCOREUP2)
		{//	スコア加算アイテムだったら
			CScore::AddScore(500);
			Uninit();
			CEffect::Create(pos, 3, 7);//　テクスチャ　テキスト
			CItemicon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), TYPE_SCOREUP2);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE006);
		}
	}

	return bHit;
}