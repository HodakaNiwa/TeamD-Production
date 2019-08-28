//*****************************************************************************
//
//     ひなあられの処理[hinaarare.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "hinaarare.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "boxCollider.h"
#include "basemode.h"
#include "player.h"
#include "effectManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define HINAARARE_DOWN_SPEED    (-20.0f)   // ひなあられが落ちるスピード
#define HINAARARE_MAX_LIFE      (120)      // ひなあられの寿命の最大値
#define HINAARARE_EFFECT_IDX    (13)       // ひなあられが使うエフェクトの番号

//*****************************************************************************
//    静的メンバ変数
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CHinaarare::CHinaarare(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	// 各種値のクリア
	m_nLife = 0;
	m_pModel = NULL;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CHinaarare::~CHinaarare()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CHinaarare *CHinaarare::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CHinaarare *pHinaarare = NULL;      // ひなあられクラス型のポインタ
	if (pHinaarare == NULL)
	{// メモリが空になっている
		pHinaarare = new CHinaarare(nPriority);
		if (pHinaarare != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pHinaarare->SetPos(pos);                                     // 座標
			pHinaarare->SetRot(rot);                                     // 向き
			pHinaarare->BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // モデル情報割り当て

			// 当たり判定用箱モデルを作成
			pHinaarare->SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
			pHinaarare->CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

			if (FAILED(pHinaarare->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pHinaarare;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CHinaarare::Init(void)
{
	// 寿命を設定
	m_nLife = HINAARARE_MAX_LIFE;

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CHinaarare::Uninit(void)
{
	// モデルの破棄
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CHinaarare::Update(void)
{
	// 値の取得
	D3DXVECTOR3 pos = GetPos();

	// 下に落とす
	pos.y += HINAARARE_DOWN_SPEED;

	// エフェクトを出す
	SetEffect(pos);

	// 値の設定
	SetPos(pos);

	// 寿命を減らす
	m_nLife--;
	if (m_nLife <= 0 || pos.y <= 0.0f)
	{// 寿命がなくなる OR 地面につく
		Uninit();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CHinaarare::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// ワールドマトリックスの設定
	SetMtxWorld(pDevice);

	// モデルの描画
	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	// 3Dオブジェクトの描画
	CObject3D::Draw();
}

//=============================================================================
// ワールドマトリックスの計算処理
//=============================================================================
void CHinaarare::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxWorld, mtxRot; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 回転行列を作成(D3DXMatrixRotationYawPitchRoll参照)
	D3DXVECTOR3 rot = GetRot();
	float fSinPitch = sinf(rot.x);
	float fCosPitch = cosf(rot.x);
	float fSinYaw = sinf(rot.y);
	float fCosYaw = cosf(rot.y);
	float fSinRoll = sinf(rot.z);
	float fCosRoll = cosf(rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// 大きさと回転を反映する
	mtxWorld._11 = mtxRot._11;
	mtxWorld._12 = mtxRot._12;
	mtxWorld._13 = mtxRot._13;
	mtxWorld._21 = mtxRot._21;
	mtxWorld._22 = mtxRot._22;
	mtxWorld._23 = mtxRot._23;
	mtxWorld._31 = mtxRot._31;
	mtxWorld._32 = mtxRot._32;
	mtxWorld._33 = mtxRot._33;

	// オフセット位置を反映
	D3DXVECTOR3 pos = GetPos();
	mtxWorld._41 = pos.x;
	mtxWorld._42 = pos.y;
	mtxWorld._43 = pos.z;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CObject3D::SetMtxWorld(mtxWorld);
}

//=============================================================================
//    当たり判定用箱モデルを作成する処理
//=============================================================================
void CHinaarare::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, bReturnFlag);
	SetBoxCollider(pBoxCollider);
}

//=============================================================================
//    モデル情報設定処理
//=============================================================================
void CHinaarare::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, pMesh, pBuffMat,
		nNumMat, pTexture);
}

//=============================================================================
//    オブジェクトに当たったときの処理
//=============================================================================
void CHinaarare::Hit(CScene *pScene)
{
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{// プレイヤーにヒットした
		Uninit();
	}
}

//=============================================================================
//    エフェクトを出す処理
//=============================================================================
void CHinaarare::SetEffect(D3DXVECTOR3 pos)
{
	// エフェクト管轄クラスを取得
	if (CManager::GetBaseMode() == NULL) { return; }
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager == NULL) { return; }

	// エフェクトを生成
	pEffectManager->SetEffect(pos, INITIALIZE_D3DXVECTOR3, HINAARARE_EFFECT_IDX);
}