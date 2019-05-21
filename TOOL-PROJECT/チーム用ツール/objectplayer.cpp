//=============================================================================
//
// オブジェクトプレイヤーの処理 [objectplayer.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "objectplayer.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "debugproc.h"
#include "scene3D.h"
#include "meshfield.h"
#include "model.h"
#include "editor.h"
#include "object.h"
#include "loadfilefunction.h"
#include "map.h"
#include "modelcreate.h"
#include "object.h"
//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CObject *CObjectPlayer::m_pObject = NULL;	//オブジェクトのポインタ情報

//=============================================================================
// プレイヤーのコンストラクタ
//=============================================================================
CObjectPlayer::CObjectPlayer() : CObject(3,OBJTYPE_OBJECT)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置の初期値
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期値
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//大きさ
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量の初期値
	m_pObject = NULL;								//オブジェクトのポインタを初期化
	m_pCreateObject = NULL;
	m_nSeletObject = 0;
	m_fSpeed = 50.0f;
}
//=============================================================================
// プレイヤーのデストラクタ
//=============================================================================
CObjectPlayer::~CObjectPlayer()
{

}

//=============================================================================
//プレイヤーの生成
//=============================================================================
CObjectPlayer *CObjectPlayer::Create(D3DXVECTOR3 pos)
{
	CObjectPlayer *pObjectPlayer = NULL;					//プレイヤーのポインタ
	if (pObjectPlayer == NULL)
	{
		pObjectPlayer = new CObjectPlayer;					//シーンの動的確保
		pObjectPlayer->m_pos = pos;							//位置の設置処理
		pObjectPlayer->Init();								//初期化処理
	}
	return pObjectPlayer;									//値を返す
}

//=============================================================================
// プレイヤーの初期化処理
//=============================================================================
HRESULT CObjectPlayer::Init(void)
{
	m_nSeletObject = 0;

	if (m_pObject == NULL)
	{
		CMap *pMap = NULL;
		if (pMap == NULL)
		{
			m_pObject = CObject::Create(m_pos,m_rot,m_scale,true,true);
			if (m_pObject != NULL)
			{
				m_pObject->BindX(pMap->GetCModelCreate()->GetMesh(m_nSeletObject), pMap->GetCModelCreate()->GetBuffMat(m_nSeletObject), pMap->GetCModelCreate()->GetNumMat(m_nSeletObject));
				m_pObject->SetObjType(OBJTYPE_PLAYER);
			}
		}
	}
	SetObjType(OBJTYPE_PLAYER);
	return S_OK;
}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void CObjectPlayer::Uninit(void)
{
	//for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	//{//モデルの数だけ繰り返し
	//	if (m_apModel[nCntModel] != NULL)
	//	{
	//		//終了処理
	//		m_apModel[nCntModel]->Uninit();
	//		delete m_apModel[nCntModel];
	//		m_apModel[nCntModel] = NULL;
	//	}
	//}

	//if (m_pObject != NULL)
	//{
	//	m_pObject->Uninit();
	//	delete m_pObject;
	//	m_pObject = NULL;
	//}


	Release();
}

//=============================================================================
// プレイヤーの更新処理
//=============================================================================
void CObjectPlayer::Update(void)
{
	//移動処理
	Move();

	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

#ifdef _DEBUG
	CDebugProc::Print(1, "モデルの位置  : x[%.1f],y[%.1f],z[%.1f]\n", m_pos.x, m_pos.y, m_pos.z);
#endif
}

//=============================================================================
// プレイヤーの描画処理
//=============================================================================
void CObjectPlayer::Draw(void)
{
	////レンダリングの取得
	//CRenderer *pRenderer;
	//pRenderer = CManager::GetRenderer();

	////デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice;
	//pDevice = pRenderer->GetDevice();

	//D3DXMATRIX mtxRot, mtxTrans, mtxScale;					//計算用マトリックス

	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&m_mtxWorld);

	//// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot,
	//	m_rot.y, m_rot.x, m_rot.z);

	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//// 大きさの反映
	//D3DXMatrixScaling(&mtxScale,
	//	m_scale.x, m_scale.y, m_scale.z);

	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y,
	//	m_rot.x,
	//	m_rot.z);

	//D3DXMatrixMultiply(&m_mtxWorld,
	//	&m_mtxWorld,
	//	&mtxRot);

	//// 移動を反映
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//D3DXMATRIX	mtxShadow;		//シャドウマトリックス
	//D3DXPLANE	planeField;		//平面情報
	//D3DXVECTOR4 vecLight;		//ライト情報
	//D3DXVECTOR3	pos, normal;	//位置、法線

	////減算合成の設定
	//pDevice->SetRenderState(D3DRS_BLENDOP,
	//	D3DBLENDOP_REVSUBTRACT);
	//pDevice->SetRenderState(D3DRS_SRCBLEND,
	//	D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND,
	//	D3DBLEND_ONE);

	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	////シャドウマトリックスを初期化
	//D3DXMatrixIdentity(&mtxShadow);

	////ライトの設定
	//vecLight = D3DXVECTOR4(0.25f, 0.87f, -0.44f, 0.0f);

	////位置の設定
	//pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	////法線の設定
	//normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	////位置と法線から平面を作成する
	//D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	////平面の射影
	//D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	////ワールドマトリックスと掛け合わせる
	//D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	//// ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	////元の設定に戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP,
	//	D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND,
	//	D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND,
	//	D3DBLEND_INVSRCALPHA);
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//// マテリアルデータへのポインタを取得
	//CMap *pMap = NULL;
	//if (pMap == NULL)
	//{
	//	D3DXMATERIAL *pMat = (D3DXMATERIAL*)pMap->GetCModelCreate()->GetBuffMat(m_nSeletObject)->GetBufferPointer();
	//	pMat->MatD3D.Diffuse.a = 0.4f;
	//}
	if (m_pObject != NULL)
	{
		m_pObject->Draw();
	}
	if (m_pCreateObject != NULL)
	{
		m_pCreateObject->Draw();
	}
}

//=============================================================================
// オブジェクトの選択取得
//=============================================================================
int CObjectPlayer::GetSelectObject(void)
{
	return m_nSeletObject;
}

//=============================================================================
// オブジェクトの移動スピード取得
//=============================================================================
float CObjectPlayer::GetSpeed(void)
{
	return m_fSpeed;
}

//=============================================================================
// オブジェクトの移動スピード設置処理
//=============================================================================
void CObjectPlayer::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

//=============================================================================
// オブジェクトの選択設置処理
//=============================================================================
void CObjectPlayer::SetSelectObject(int nSelect)
{
	//m_nSeletObject = nSelect;

	CMap *pMap = NULL;	//マップのポインタ
	if (pMap == NULL)
	{
		m_nNumModel = pMap->GetCModelCreate()->GetNumModel();
		//m_nSeletObject = (m_nSeletObject + nSelect) % m_nNumModel;
		if (nSelect < m_nNumModel && nSelect >= 0)
		{
			m_nSeletObject = nSelect;
			m_pObject->BindX(pMap->GetCModelCreate()->GetMesh(m_nSeletObject), pMap->GetCModelCreate()->GetBuffMat(m_nSeletObject), pMap->GetCModelCreate()->GetNumMat(m_nSeletObject));
		}
	}

}

//=============================================================================
// オブジェクトの情報取得
//=============================================================================
CObject *CObjectPlayer::GetObjectPointer(void)
{
	return m_pObject;
}
//=============================================================================
// プレイヤーの移動処理
//=============================================================================
void CObjectPlayer::Move(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//カメラの向き取得
	D3DXVECTOR3 cameraRot = pCamera->GetRot();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 pos = m_pObject->GetPos();
	D3DXVECTOR3 rot = m_pObject->GetRot();
	D3DXVECTOR3 size = m_pObject->GetSize();
	bool bHit = m_pObject->GetHit();
	bool bDestroy = m_pObject->GetDestroy();

	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		CMap *pMap = NULL;	//マップのポインタ
		if (pMap == NULL)
		{
			m_nNumModel = pMap->GetCModelCreate()->GetNumModel();
			m_nSeletObject = (m_nSeletObject + (m_nNumModel - 1)) % m_nNumModel;
			m_pObject->BindX(pMap->GetCModelCreate()->GetMesh(m_nSeletObject), pMap->GetCModelCreate()->GetBuffMat(m_nSeletObject), pMap->GetCModelCreate()->GetNumMat(m_nSeletObject));
		}
	}
	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		CMap *pMap = NULL;	//マップのポインタ
		if (pMap == NULL)
		{
			m_nNumModel = pMap->GetCModelCreate()->GetNumModel();
			m_nSeletObject = (m_nSeletObject + 1) % m_nNumModel;
			m_pObject->BindX(pMap->GetCModelCreate()->GetMesh(m_nSeletObject), pMap->GetCModelCreate()->GetBuffMat(m_nSeletObject), pMap->GetCModelCreate()->GetNumMat(m_nSeletObject));
		}
	}
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CMap *pMap = NULL;
		if (pMap == NULL)
		{
			m_pCreateObject = CObject::Create(pos, rot, size,bHit,bDestroy);
			m_pCreateObject->BindX(pMap->GetCModelCreate()->GetMesh(m_nSeletObject), pMap->GetCModelCreate()->GetBuffMat(m_nSeletObject), pMap->GetCModelCreate()->GetNumMat(m_nSeletObject));
			m_pCreateObject->SetType(m_nSeletObject);
		}
	}

	//if (pInputKeyboard->GetTrigger(DIK_A) == true)
	//{// キーボードの[A]キーが押された（左移動）
	//	if (pInputKeyboard->GetTrigger(DIK_W) == true)
	//	{// キーボードの[W]キーが押された（左上移動）
	//		m_pos.x += -sinf(cameraRot.y + D3DX_PI * 0.75f) * 50.0f;
	//		m_pos.z += -cosf(cameraRot.y + D3DX_PI * 0.75f) * 50.0f;
	//	}
	//	else if (pInputKeyboard->GetTrigger(DIK_S) == true)
	//	{// キーボードの[S]キーが押された（左下移動）
	//		m_pos.x += -sinf(cameraRot.y + D3DX_PI * 0.25f) * 50.0f;
	//		m_pos.z += -cosf(cameraRot.y + D3DX_PI * 0.25f) * 50.0f;
	//	}
	//	else
	//	{// キーボードの[A]キーが押された（左移動）
	//		m_pos.x += -sinf(cameraRot.y + D3DX_PI / 2) * 50.0f;
	//		m_pos.z += -cosf(cameraRot.y + D3DX_PI / 2) * 50.0f;
	//	}
	//}
	//else if (pInputKeyboard->GetTrigger(DIK_D) == true)
	//{// キーボードの[D]キーが押された（右移動）
	//	if (pInputKeyboard->GetTrigger(DIK_W) == true)
	//	{// キーボードの[W]キーが押された（右上移動）
	//		m_pos.x += -sinf(cameraRot.y + D3DX_PI * -0.75f) * 50.0f;
	//		m_pos.z += -cosf(cameraRot.y + D3DX_PI * -0.75f) * 50.0f;
	//	}
	//	else if (pInputKeyboard->GetTrigger(DIK_S) == true)
	//	{// キーボードの[S]キーが押された（右下移動）
	//		m_pos.x += -sinf(cameraRot.y + D3DX_PI * -0.25f) * 50.0f;
	//		m_pos.z += -cosf(cameraRot.y + D3DX_PI * -0.25f) * 50.0f;
	//	}
	//	else
	//	{// キーボードの[D]キーが押された（右移動）
	//		m_pos.x += -sinf(cameraRot.y + D3DX_PI / -2) * 50.0f;
	//		m_pos.z += -cosf(cameraRot.y + D3DX_PI / -2) * 50.0f;
	//	}
	//}
	//else if (pInputKeyboard->GetTrigger(DIK_W) == true)
	//{//キーボードの[W]キーが押された（前移動）
	//	m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1)*50.0f;
	//	m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1)*50.0f;
	//}
	//else if (pInputKeyboard->GetTrigger(DIK_S) == true)
	//{//キーボードの[S]キーが押された（後移動）
	//	m_pos.x += -sinf(cameraRot.y + D3DX_PI * 0)*50.0f;
	//	m_pos.z += -cosf(cameraRot.y + D3DX_PI * 0)*50.0f;
	//}

	if (pInputKeyboard->GetTrigger(DIK_F) == true)
	{//
		rot.y += 0.785f;
	}
	if (pInputKeyboard->GetTrigger(DIK_G) == true)
	{//
		rot.y -= 0.785f;
	}
	if (pInputKeyboard->GetTrigger(DIK_W) == true)
	{
		pos.z += m_fSpeed;
	}
	else if (pInputKeyboard->GetTrigger(DIK_S) == true)
	{
		pos.z -= m_fSpeed;
	}
	else if (pInputKeyboard->GetTrigger(DIK_A) == true)
	{
		pos.x -= m_fSpeed;
	}
	else if(pInputKeyboard->GetTrigger(DIK_D) == true)
	{
		pos.x += m_fSpeed;
	}
	//位置の設置処理
	m_pObject->SetPos(pos);
	m_pObject->SetRot(rot);
	//m_pObject->SetRot(m_rot);
	//重力
	//m_move.y -= cosf(D3DX_PI * 0) * 0.5f;

	//移動加算処理
	//m_pos.y += m_move.y;

#ifdef _DEBUG
	CDebugProc::Print(1,"%d\n", m_nSeletObject);
	CDebugProc::Print(1, "%f\n", rot.y);
	CDebugProc::Print(1, "種類の切り替え[1,2] モデルの設置[3] モデルの保存[F2]\n");
#endif
}