//*****************************************************************************
//
//     サーバーの処理[server.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "server.h"
#include "serverlib.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数
//*****************************************************************************


//*****************************************************************************
//    CServerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CServer::CServer()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CServer::~CServer()
{

}

//=============================================================================
//    WinSockの初期化処理
//=============================================================================
HRESULT CServer::ServerStartUp(void)
{
	WSADATA wsaData;  // winsockの情報
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	return S_OK;
}

//=============================================================================
//    WinSockの終了処理
//=============================================================================
void CServer::ServerCleanUp(void)
{
	WSACleanup();
}

//=============================================================================
//    生成処理
//=============================================================================
CServer *CServer::Create(void)
{
	CServer *pServer = NULL;      // サーバークラス型のポインタ
	if (pServer == NULL)
	{// メモリが空になっている
		pServer = new CServer;
		if (pServer != NULL)
		{// インスタンスを生成できた
			if (FAILED(pServer->Init()))
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

	return pServer;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CServer::Init(void)
{
	// 接続人数初期化
	m_nNumConnect = 0;

	// ハンドルを初期化
	m_hWaitConnectAllHander = 0;

	// ソケットの作成
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	// ソケットの設定
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;

	// ソケットに名称を付与
	bind(m_sockServer, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));

	// クライアントからの接続要求を待てる状態にする
	listen(m_sockServer, 5);
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		m_hWaitConnectHander[nCntConnect] = 0;
		m_bConnected[nCntConnect] = false;
	}

	// スレッドを切り替えて接続待機処理をする
	m_hWaitConnectAllHander = (HANDLE)_beginthreadex(NULL, 0, WaitConnectAllHandler, this, 0, NULL);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CServer::Uninit(void)
{
	// マルチスレッド終了のためにクライアントを生成し破棄する
	CClient *pClient[MAX_SERVER_CONNECTING_NUM] = {};
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM - m_nNumConnect; nCntConnect++)
	{
		pClient[nCntConnect] = CClient::Create("127.0.1.1");
	}
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM - m_nNumConnect; nCntConnect++)
	{
		if (pClient[nCntConnect] != NULL)
		{
			pClient[nCntConnect]->Uninit();
			delete pClient[nCntConnect];
			pClient[nCntConnect] = NULL;
		}
	}

	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		// ソケット(クライアント)の開放
		closesocket(m_sockClient[nCntConnect]);

		// ハンドルの開放
		if (m_hWaitConnectHander[nCntConnect] != 0)
		{
			CloseHandle(m_hWaitConnectHander[nCntConnect]);
		}
	}

	// ハンドルの開放
	if (m_hWaitConnectAllHander != 0)
	{
		CloseHandle(m_hWaitConnectAllHander);
	}

	// ソケット(サーバー)の開放
	closesocket(m_sockServer);
}

//=============================================================================
//    更新処理
//=============================================================================
void CServer::Update(void)
{
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == true)
		{// クライアントと接続できた
			CDebugProc::Print(1, "%d番と接続完了!!\n", nCntConnect);
			ExChangeMessage(nCntConnect);
		}
		else
		{// クライアントと接続できていない
			CDebugProc::Print(1, "サーバー受付中...\n");
		}
	}

	CDebugProc::Print(1, "サーバー接続人数 : %d人", m_nNumConnect);
}

//=============================================================================
//    接続待機処理
//=============================================================================
void CServer::WaitConnect(void)
{
	// クライアントからの接続要求を受け付ける
	SOCKET sockClient;
	struct sockaddr_in addrClient;
	int nLenghtAddrClient = 0;
	nLenghtAddrClient = sizeof(addrClient);
	sockClient = accept(m_sockServer, (struct sockaddr *)&addrClient, &nLenghtAddrClient);
	if (sockClient == INVALID_SOCKET)
	{
		return;
	}

	// 開いている場所に情報を格納
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == false)
		{
			m_sockClient[nCntConnect] = sockClient;
			m_addrClient[nCntConnect] = addrClient;
			m_nLengthAddrClient[nCntConnect] = nLenghtAddrClient;
			memset(m_aSendMessage[nCntConnect], 0, sizeof(m_aSendMessage[nCntConnect]));
			m_bConnected[nCntConnect] = true;
			m_nNumConnect++;
			break;
		}
	}

	return;
}

//=============================================================================
//    接続待機処理
//=============================================================================
void CServer::WaitConnectAll(void)
{
	// クライアントからの接続要求を受け付ける
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		m_nLengthAddrClient[nCntConnect] = sizeof(m_addrClient);
		m_sockClient[nCntConnect] = accept(m_sockServer, (struct sockaddr *)&m_addrClient[nCntConnect], &m_nLengthAddrClient[nCntConnect]);
		if (m_sockClient[nCntConnect] == INVALID_SOCKET)
		{
			return;
		}
		memset(m_aSendMessage[nCntConnect], 0, sizeof(m_aSendMessage[nCntConnect]));
		m_bConnected[nCntConnect] = true;
		m_nNumConnect++;
	}

	return;
}

//=============================================================================
//    接続待機処理を呼ぶ関数(マルチスレッド対応のため)
//=============================================================================
unsigned int WINAPI CServer::WaitConnectHandler(void *pServer)
{
	reinterpret_cast<CServer*>(pServer)->WaitConnect();     // CServer型にキャストして呼ぶ
	return 0;
}

//=============================================================================
//    接続待機処理を呼ぶ関数(マルチスレッド対応のため)
//=============================================================================
unsigned int WINAPI CServer::WaitConnectAllHandler(void *pServer)
{
	reinterpret_cast<CServer*>(pServer)->WaitConnectAll();  // CServer型にキャストして呼ぶ
	return 0;
}

//=============================================================================
//    クライアントとのメッセージ交換処理
//=============================================================================
void CServer::ExChangeMessage(int nIdxClient)
{
	// クライアントからデータ(メッセージ)を受信
	memset(m_aReceiveMessage[nIdxClient], 0, sizeof(m_aReceiveMessage[nIdxClient]));
	m_nLengthData[nIdxClient] = recv(m_sockClient[nIdxClient], m_aReceiveMessage[nIdxClient], MAX_SERVER_DATA, 0);

	// メッセージを表示する
	CDebugProc::Print(1, "%s\n受信バイト数[%d]\n", m_aReceiveMessage[nIdxClient], m_nLengthData[nIdxClient]);

	// メッセージからコマンドを適用
	if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_HELLO) == 0)
	{
		Command_Hello(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_DELETE) == 0)
	{
		Command_DeleteClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_SEND_CLIENT) == 0)
	{
		Command_Send_Client(nIdxClient);
		strcat(m_aSendMessage[nIdxClient], "メッセージ送信");
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_SE_DATA) == 0)
	{
		Command_SendDataClient(nIdxClient);

		strcat(m_aSendMessage[nIdxClient], " ");
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_USERID) == 0)
	{
		Command_UserIdClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_NONE) == 0
		&& CServerLib::Memcmp(m_aReceiveMessage[(nIdxClient + 1) % 2], CD_SEND_CLIENT) != 0)
	{
		strcat(m_aSendMessage[nIdxClient], "???");
	}

	// クライアントへメッセージを送信
	send(m_sockClient[nIdxClient], m_aSendMessage[nIdxClient], strlen(m_aSendMessage[nIdxClient]), 0);
	memset(m_aSendMessage[nIdxClient], 0, sizeof(m_aSendMessage[nIdxClient]));
}

//=============================================================================
//    Helloメッセージをクライアントへ送信する処理
//=============================================================================
void CServer::Command_Hello(int nIdxClient)
{
	// クライアントへのメッセージを設定
	strcat(m_aSendMessage[nIdxClient], "Hello Client");
}

//=============================================================================
//    クライアントからクライアントへメッセージ送信する処理
//=============================================================================
void CServer::Command_Send_Client(int nIdxClient)
{
}

//=============================================================================
//    クライアントとの交信を終了する処理
//=============================================================================
void CServer::Command_DeleteClient(int nIdxClient)
{
	// ソケット(クライアント)の開放
	closesocket(m_sockClient[nIdxClient]);
	m_bConnected[nIdxClient] = false;
	m_nNumConnect--;
	m_hWaitConnectHander[nIdxClient] = (HANDLE)_beginthreadex(NULL, 0, WaitConnectHandler, this, 0, NULL);
	CDebugProc::ClearString();
	CDebugProc::Print(1, "%d番の接続が途絶えました・・・\n", nIdxClient);
	CDebugProc::Print(1, "接続受付開始\n");
}

//=============================================================================
//    クライアントからデータを読み取る処理
//=============================================================================
void CServer::Command_SendDataClient(int nIdxClient)
{
	float aPlayerPos[3];        // プレイヤーの座標
	char aStr[MAX_SERVER_DATA];
	char *pStr;
	pStr = &m_aReceiveMessage[nIdxClient][0];

	// 受信したメッセージからデータを読み取り
	pStr= CServerLib::ReadFloat3(pStr, CD_SE_DATA, &aPlayerPos[0]);

	// 受信した座標を別クライアントへ送信するためのメッセージを作成
	strcpy(m_aSendMessage[(nIdxClient + 1) % 2], CServerLib::WriteFloat3(&aPlayerPos[0], aStr));
}

//=============================================================================
//    クライアントからキャラデータを読み取る処理
//=============================================================================
void CServer::Command_CharaDataClient(int nIdxClient)
{

}

//=============================================================================
//    クライアントからゲームデータを読み取る処理
//=============================================================================
void CServer::Command_GameDataClient(int nIdxClient)
{

}

//=============================================================================
//    クライアントへ番号を送信する処理
//=============================================================================
void CServer::Command_UserIdClient(int nIdxClient)
{
	// クライアント番号を送信
	char aStr[MAX_SERVER_DATA];
	strcpy(m_aSendMessage[nIdxClient], USER_ID);
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nIdxClient, aStr));
}


//*****************************************************************************
//    CClientの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CClient::CClient()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CClient::~CClient()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CClient *CClient::Create(char *pSeverAddr)
{
	CClient *pClient = NULL;      // クライアントクラス型のポインタ
	if (pClient == NULL)
	{// メモリが空になっている
		pClient = new CClient;
		if (pClient != NULL)
		{// インスタンスを生成できた
			if (FAILED(pClient->Init(pSeverAddr)))
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

	return pClient;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CClient::Init(char *pSeverAddr)
{
	// クライアントID初期化
	m_nClientId = 0;

	// ハンドルを初期化
	m_hWaitConnectHander = 0;

	// データクリア
	memset(m_aData, 0, MAX_SERVER_DATA);
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);

	// IPアドレスを設定
	if (pSeverAddr == NULL) return E_FAIL;
	SetServerAddr(pSeverAddr);

	// スレッドを切り替えて接続待機処理をする
	m_hWaitConnectHander = (HANDLE)_beginthreadex(NULL, 0, WaitConnectHandler, this, 0, NULL);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CClient::Uninit(void)
{
	// サーバーとの接続を終了
	Command_DeleteClient();

	// ハンドルの開放
	ReleaseHandller();

	// ソケット(クライアント)の開放
	closesocket(m_sockClient);
}

//=============================================================================
//    更新処理
//=============================================================================
void CClient::Update(void)
{
	if (m_bConnected == true)
	{// サーバーと接続できた
		ExChangeMessage();
	}
	else
	{// サーバーと接続できていない
		CDebugProc::Print(1, "サーバーが見つかりません・・・\n");
	}

	// データクリア
	memset(m_aData, 0, MAX_SERVER_DATA);
}

//=============================================================================
//    マルチスレッドハンドルを開放する処理
//=============================================================================
void CClient::ReleaseHandller()
{
	// ハンドルの開放
	if (m_hWaitConnectHander != 0)
	{
		CloseHandle(m_hWaitConnectHander);
	}
}

//=============================================================================
//    接続待機処理を呼ぶ関数(マルチスレッド対応のため)
//=============================================================================
unsigned int WINAPI CClient::WaitConnectHandler(void *pClient)
{
	reinterpret_cast<CClient*>(pClient)->WaitConnect();     // CClient型にキャストして呼ぶ
	return 0;
}

//=============================================================================
//    サーバーIPアドレスを設定する処理
//=============================================================================
void CClient::SetServerAddr(char *pServerAddr)
{
	strcpy(m_aServerAddr, pServerAddr);
}

//=============================================================================
//    サーバーとの接続待機処理
//=============================================================================
void CClient::WaitConnect(void)
{
	// ソケットの作成
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	// ソケットの設定
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_aServerAddr);

	if (connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer)) == SOCKET_ERROR)
	{// サーバーと接続失敗
		CDebugProc::Print(1, "接続が途絶えました・・・\n");
		CDebugProc::Print(1, "接続開始\n");
		m_bConnected = false;
	}
	else
	{// サーバーと接続成功
		m_nLengthData = 0;

		// クライアントIDを取得する
		SetClientUserId();
		m_bConnected = true;
	}
}

//=============================================================================
//   自分のクライアント番号をサーバーから受信する処理
//=============================================================================
void CClient::SetClientUserId(void)
{
	m_nClientId = 0;  // ユーザーID初期化

	// サーバーへデータ(メッセージ)を送信
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	strcpy(m_aSendMessage, CD_USERID);
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// サーバーからデータ(メッセージ)を受信
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	// メッセージをクライアント番号に変換
	m_nClientId = CServerLib::ReadInt(m_aReceiveMessage, USER_ID);
}

//=============================================================================
//    サーバーとのメッセージ交換処理
//=============================================================================
void CClient::ExChangeMessage(void)
{
	// サーバーへ送るメッセージを作成
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));

	// 画面によって処理わけ
	if (CManager::GetMode() == CManager::MODE_CHARASELECT)
	{// キャラ選択画面だった
		Command_CharaClient();
	}
	else if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム画面だった
		Command_GameClient();
	}

	// サーバーへデータ(メッセージ)を送信
	CDebugProc::Print(1, "送ったメッセージ : %s\n", m_aSendMessage);
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// サーバーからデータ(メッセージ)を受信
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	if (m_nLengthData > 1)
	{
		// メッセージを表示する
		CDebugProc::Print(1, "受け取ったメッセージ : %s\n", m_aReceiveMessage);

		// メッセージを表示する
		CDebugProc::Print(1, "受信バイト数[%d]\n", m_nLengthData);
	}
}

//=============================================================================
//    データをバイナリ化する処理
//=============================================================================
void CClient::Print(char *fmt, ...)
{
	va_list list;              // 可変長引数(... の引数を受け取ることができる)
	char aStr[MAX_SERVER_DATA];// 文字列格納用変数

	va_start(list, fmt);       // 可変長引数リストを初期化(fmt以下の引数を登録)
	vsprintf(aStr, fmt, list); // 文字列を標準出力にのっとって書き出し
	va_end(list);              // 引数リストをクリア
	strcat(m_aData, aStr);     // 格納した文字列をメンバ変数の後ろにくっつける
}

//=============================================================================
//    Helloメッセージを受け取る処理
//=============================================================================
void CClient::Command_Hello(void)
{
	strcat(m_aSendMessage, CD_HELLO);
}

//=============================================================================
//    別のクライアントへメッセージを送信する処理
//=============================================================================
void CClient::Command_Send_Client(void)
{
	strcat(m_aSendMessage, CD_SEND_CLIENT);
}

//=============================================================================
//    クライアントへメッセージを送信する処理(キャラ選択画面の時)
//=============================================================================
void CClient::Command_CharaClient(void)
{
	strcpy(m_aSendMessage, CD_CHARA_DATA);
	strcat(m_aSendMessage, m_aData);
}

//=============================================================================
//    クライアントへメッセージを送信する処理(ゲーム画面の時)
//=============================================================================
void CClient::Command_GameClient(void)
{
	strcpy(m_aSendMessage, CD_GAME_DATA);
	strcat(m_aSendMessage, m_aData);
}

//=============================================================================
//    サーバーとの接続を終了する処理
//=============================================================================
void CClient::Command_DeleteClient(void)
{
	// サーバーへ送るメッセージを作成
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	strcpy(m_aSendMessage, CD_DELETE);

	// サーバーへデータ(メッセージ)を送信
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// サーバーからデータ(メッセージ)を受信
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);
}


//=============================================================================
//    クライアントIDを取得する処理
//=============================================================================
int CClient::GetClientId(void)
{
	return m_nClientId;
}

//=============================================================================
//    クライアントIDを取得する処理
//=============================================================================
char *CClient::GetReceiveMessage(void)
{
	return m_aReceiveMessage;
}

//=============================================================================
//    受信バイト数を取得する処理
//=============================================================================
int CClient::GetLengthData(void)
{
	return m_nLengthData;
}