//*****************************************************************************
//
//     サーバーの処理[server.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SERVER_H_
#define _SERVER_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MAX_SERVER_CONNECTING_NUM  (2)     // サーバーにアクセスできる最大人数
#define MAX_SERVER_DATA            (4096)  // サーバーが管理できるデータ量

// サーバーのコマンド
#define CD_NONE                "CD_NONE"
#define CD_HELLO               "CD_HELLO"
#define CD_SEND_CLIENT         "CD_SEND_CLIENT"
#define CD_DELETE              "CD_DELETE"
#define CD_SE_DATA             "CD_SE_DATA"
#define CD_CHARA_DATA          "CD_CHARA_DATA"
#define CD_TUTO_DATA           "CD_TUTO_DATA"
#define CD_GAME_DATA           "CD_GAME_DATA"
#define CD_USERID              "CD_USERID"

// クライアントID読み取り用
#define USER_ID                "USER_ID"

//*****************************************************************************
//     サーバークラスの定義
//*****************************************************************************
class CServer
{
public:    // 誰でもアクセス可能
	CServer();
	~CServer();

	static HRESULT ServerStartUp(void);
	static void ServerCleanUp(void);
	static CServer *Create(void);
	static unsigned int WINAPI WaitConnectAllHandler(void *pServer);
	static unsigned int WINAPI WaitConnectHandler(void *pServer);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void WaitConnect(void);
	void WaitConnectAll(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void ExChangeMessage(int nIdxClient);
	void Command_Hello(int nIdxClient);
	void Command_Send_Client(int nIdxClient);
	void Command_DeleteClient(int nIdxClient);
	void Command_SendDataClient(int nIdxClient);
	void Command_CharaDataClient(int nIdxClient);
	void Command_GameDataClient(int nIdxClient);
	void Command_UserIdClient(int nIdxClient);

	int m_nNumConnect;                                                     // 現在接続されているクライアントの人数
	struct sockaddr_in m_addrServer;                                       // ソケットアドレス(サーバー)の情報
	struct sockaddr_in m_addrClient[MAX_SERVER_CONNECTING_NUM];            // ソケットアドレス(クライアント)の情報
	SOCKET m_sockServer;                                                   // ソケット(サーバー用)
	SOCKET m_sockClient[MAX_SERVER_CONNECTING_NUM];                        // ソケット(クライアント用)
	int m_nLengthAddrClient[MAX_SERVER_CONNECTING_NUM];                    // ソケットアドレス(クライアント)のサイズ
	char m_aReceiveMessage[MAX_SERVER_CONNECTING_NUM][MAX_SERVER_DATA];    // 受信データ
	char m_aSendMessage[MAX_SERVER_CONNECTING_NUM][MAX_SERVER_DATA];       // 送信データ
	int m_nLengthData[MAX_SERVER_CONNECTING_NUM];                          // 受信データのサイズ
	bool m_bConnected[MAX_SERVER_CONNECTING_NUM];                          // クライアントと接続したかどうか

	// マルチスレッドプログラムに使用
	HANDLE m_hWaitConnectAllHander;
	HANDLE m_hWaitConnectHander[MAX_SERVER_CONNECTING_NUM];
};

//*****************************************************************************
//    クライアントクラスの定義
//*****************************************************************************
class CClient
{
public:    // 誰でもアクセス可能
	CClient();
	~CClient();

	static CClient *Create(char *pSeverAddr);
	static unsigned int WINAPI WaitConnectHandler(void *pClient);

	HRESULT Init(char *pSeverAddr);
	void Uninit(void);
	void Update(void);
	void Print(char *fmt, ...);

	bool GetConnected(void);
	int GetClientId(void);
	char *GetReceiveMessage(void);
	int GetLengthData(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void SetServerAddr(char *pServerAddr);
	void SetClientUserId(void);
	void WaitConnect(void);
	void ExChangeMessage(void);
	void Command_Hello(void);
	void Command_Send_Client(void);
	void Command_CharaClient(void);
	void Command_TutorialClient(void);
	void Command_GameClient(void);
	void Command_DeleteClient(void);

	void ReleaseHandller();

	int m_nClientId;                          // 自分のクライアント番号(プレイヤーの番号と紐づけ)
	char m_aServerAddr[64];                   // サーバーのIPアドレス
	struct sockaddr_in m_addrServer;          // ソケットアドレス(サーバー)の情報
	SOCKET m_sockClient;                      // ソケット(クライアント用)
	char m_aReceiveMessage[MAX_SERVER_DATA];  // 受信データ
	char m_aSendMessage[MAX_SERVER_DATA];     // 送信データ
	int m_nLengthData;                        // 受信データのサイズ
	bool m_bConnected;                        // サーバーに接続したかどうか
	char m_aData[MAX_SERVER_DATA];            // 送信させる各種データ(各種データをこれにメッセージ化する)

	// マルチスレッドプログラムに使用
	HANDLE m_hWaitConnectHander;
};

#endif
