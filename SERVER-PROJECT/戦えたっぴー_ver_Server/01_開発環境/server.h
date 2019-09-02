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
#define MAX_SERVER_BULLETDATA      (1000)  // サーバーが管理できる弾のデータ量

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

// メッセージ読み取り用
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
	void ClearVariable(void);

	void RecieveMassageFromClient(int nIdxClient);
	void ExChangeMessage(int nIdxClient);
	void SendMessageToClient(int nIdxClient);

	void Command_Hello(int nIdxClient);
	void Command_Send_Client(int nIdxClient);
	void Command_DeleteClient(int nIdxClient);
	void Command_SendDataClient(int nIdxClient);
	void Command_CharaDataClient(int nIdxClient);
	void Command_GameDataClient(int nIdxClient);
	void Command_TutorialDataClient(int nIdxClient);
	char *SetGameData_Player(int nIdxClient, char *pStr);
	char *SetGameData_Game(int nIdxClient, char *pStr);
	char *SetGameData_Enemy(int nIdxClient, char *pStr);
	char *SetGameData_Block(int nIdxClient, char *pStr);
	char *SetGameData_Item(int nIdxClient, char *pStr);
	char *SetGameData_SpawnEnemyCount(int nIdxClient, char *pStr);
	char *SetGameData_MapEvent(int nIdxClient, char *pStr);
	char *SetGameData_DeleteEnemy(int nIdxClient, char *pStr);
	char *SetGameData_DeleteItem(int nIdxClient, char *pStr);
	char *PushBulletData_Player(int nIdxClient, char *pStr);
	char *PushBulletData_Enemy(int nIdxClient, char *pStr);
	char *SetGameData_ClientState(int nIdxClient, char *pStr);
	char *SetGameData_BreakEnemy(int nIdxClient, char *pStr);
	char *SetGameData_HitBullet(int nIdxClient, char *pStr);
	void SetGameData_EnemyBullet(int nIdxClient, char *pStr);
	void SetGameData_PlayerBullet(int nIdxClient, char *pStr);
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

	// 弾の管理に必要
	typedef struct
	{
		D3DXVECTOR3 pos;
		int nIdx;
		int nType;
	}BULLET_DATA;
	int m_nNumPlayerBulletData[MAX_SERVER_CONNECTING_NUM];
	int m_nNumEnemyBulletData;
	BULLET_DATA m_PlayerBulletData[MAX_SERVER_CONNECTING_NUM][MAX_SERVER_BULLETDATA];
	BULLET_DATA m_EnemyBulletData[MAX_SERVER_BULLETDATA];
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

	HRESULT Init(char *pSeverAddr);
	void Uninit(void);
	void Update(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void SetServerAddr(char *pServerAddr);
	void WaitConnect(void);
	void ExChangeMessage(void);
	void Command_Hello(void);
	void Command_Send_Client(void);
	void Command_DeleteClient(void);

	char m_aServerAddr[64];                   // サーバーのIPアドレス
	struct sockaddr_in m_addrServer;          // ソケットアドレス(サーバー)の情報
	SOCKET m_sockClient;                      // ソケット(クライアント用)
	char m_aReceiveMessage[MAX_SERVER_DATA];  // 受信データ
	char m_aSendMessage[MAX_SERVER_DATA];     // 送信データ
	int m_nLengthData;                        // 受信データのサイズ
	bool m_bConnected;                        // サーバーに接続したかどうか
};

#endif
