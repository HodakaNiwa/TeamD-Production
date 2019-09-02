//*****************************************************************************
//
//     �T�[�o�[�̏���[server.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "server.h"
#include "serverlib.h"
#include "manager.h"
#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define HINAEVENT_DROP_ITEM (2)

//*****************************************************************************
//    �ÓI�����o�ϐ�
//*****************************************************************************


//*****************************************************************************
//    CServer�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CServer::CServer()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CServer::~CServer()
{

}

//=============================================================================
//    WinSock�̏���������
//=============================================================================
HRESULT CServer::ServerStartUp(void)
{
	WSADATA wsaData;  // winsock�̏��
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	return S_OK;
}

//=============================================================================
//    WinSock�̏I������
//=============================================================================
void CServer::ServerCleanUp(void)
{
	WSACleanup();
}

//=============================================================================
//    ��������
//=============================================================================
CServer *CServer::Create(void)
{
	CServer *pServer = NULL;      // �T�[�o�[�N���X�^�̃|�C���^
	if (pServer == NULL)
	{// ����������ɂȂ��Ă���
		pServer = new CServer;
		if (pServer != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pServer->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pServer;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CServer::Init(void)
{
	// �ϐ����N���A����
	ClearVariable();

	// �\�P�b�g�̍쐬
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;

	// �\�P�b�g�ɖ��̂�t�^
	bind(m_sockServer, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));

	// �N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ���
	listen(m_sockServer, 5);
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		m_hWaitConnectHander[nCntConnect] = 0;
		m_bConnected[nCntConnect] = false;
	}

	// �X���b�h��؂�ւ��Đڑ��ҋ@����������
	m_hWaitConnectAllHander = (HANDLE)_beginthreadex(NULL, 0, WaitConnectAllHandler, this, 0, NULL);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CServer::Uninit(void)
{
	// �}���`�X���b�h�I���̂��߂ɃN���C�A���g�𐶐����j������
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
		// �\�P�b�g(�N���C�A���g)�̊J��
		closesocket(m_sockClient[nCntConnect]);

		// �n���h���̊J��
		if (m_hWaitConnectHander[nCntConnect] != 0)
		{
			CloseHandle(m_hWaitConnectHander[nCntConnect]);
		}
	}

	// �n���h���̊J��
	if (m_hWaitConnectAllHander != 0)
	{
		CloseHandle(m_hWaitConnectAllHander);
	}

	// �\�P�b�g(�T�[�o�[)�̊J��
	closesocket(m_sockServer);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CServer::Update(void)
{
	// ���b�Z�[�W��M����
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == true)
		{// �N���C�A���g�Ɛڑ��ł���
			CDebugProc::Print("%d�ԂƐڑ�����!!\n", nCntConnect);
			RecieveMassageFromClient(nCntConnect);
		}
		else
		{// �N���C�A���g�Ɛڑ��ł��Ă��Ȃ�
			CDebugProc::Print("�T�[�o�[��t��...\n");
		}
	}


	// ���b�Z�[�W��������
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == true)
		{// �N���C�A���g�Ɛڑ��ł���
			ExChangeMessage(nCntConnect);
		}
	}


	// ���b�Z�[�W���M����
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == true)
		{// �N���C�A���g�Ɛڑ��ł���
			SendMessageToClient(nCntConnect);
		}
	}

	CDebugProc::Print("�T�[�o�[�ڑ��l�� : %d�l", m_nNumConnect);
}

//=============================================================================
//    �ڑ��ҋ@����
//=============================================================================
void CServer::WaitConnect(void)
{
	// �N���C�A���g����̐ڑ��v�����󂯕t����
	SOCKET sockClient;
	struct sockaddr_in addrClient;
	int nLenghtAddrClient = 0;
	nLenghtAddrClient = sizeof(addrClient);
	sockClient = accept(m_sockServer, (struct sockaddr *)&addrClient, &nLenghtAddrClient);
	if (sockClient == INVALID_SOCKET)
	{
		return;
	}

	// �J���Ă���ꏊ�ɏ����i�[
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
//    �ڑ��ҋ@����
//=============================================================================
void CServer::WaitConnectAll(void)
{
	// �N���C�A���g����̐ڑ��v�����󂯕t����
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
//    �ڑ��ҋ@�������ĂԊ֐�(�}���`�X���b�h�Ή��̂���)
//=============================================================================
unsigned int WINAPI CServer::WaitConnectHandler(void *pServer)
{
	reinterpret_cast<CServer*>(pServer)->WaitConnect();     // CServer�^�ɃL���X�g���ČĂ�
	return 0;
}

//=============================================================================
//    �ڑ��ҋ@�������ĂԊ֐�(�}���`�X���b�h�Ή��̂���)
//=============================================================================
unsigned int WINAPI CServer::WaitConnectAllHandler(void *pServer)
{
	reinterpret_cast<CServer*>(pServer)->WaitConnectAll();  // CServer�^�ɃL���X�g���ČĂ�
	return 0;
}

//=============================================================================
//    �ϐ����N���A����
//=============================================================================
void CServer::ClearVariable(void)
{
	m_nNumConnect = 0;
	m_hWaitConnectAllHander = 0;
	m_nNumEnemyBulletData = 0;

	for (int nCnt = 0; nCnt < MAX_SERVER_CONNECTING_NUM; nCnt++)
	{
		m_hWaitConnectHander[nCnt] = 0;
		m_nNumPlayerBulletData[nCnt] = 0;
		memset(m_aReceiveMessage[nCnt], 0, MAX_SERVER_DATA);
		memset(m_aSendMessage[nCnt], 0, MAX_SERVER_DATA);
	}

	for (int nCnt = 0; nCnt < MAX_SERVER_CONNECTING_NUM; nCnt++)
	{
		for (int nCntData = 0; nCntData < MAX_SERVER_BULLETDATA; nCntData++)
		{
			ZeroMemory(&m_PlayerBulletData[nCnt][nCntData], sizeof(m_PlayerBulletData[nCnt][nCntData]));
		}
	}
	for (int nCntData = 0; nCntData < MAX_SERVER_BULLETDATA; nCntData++)
	{
		ZeroMemory(&m_EnemyBulletData[nCntData], sizeof(m_EnemyBulletData[nCntData]));
	}
}

//=============================================================================
//    �N���C�A���g���烁�b�Z�[�W���󂯎�鏈��
//=============================================================================
void CServer::RecieveMassageFromClient(int nIdxClient)
{
	// �N���C�A���g����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage[nIdxClient], 0, MAX_SERVER_DATA);
	m_nLengthData[nIdxClient] = recv(m_sockClient[nIdxClient], m_aReceiveMessage[nIdxClient], MAX_SERVER_DATA, 0);
}

//=============================================================================
//    �N���C�A���g�Ƃ̃��b�Z�[�W��������
//=============================================================================
void CServer::ExChangeMessage(int nIdxClient)
{
	// ���b�Z�[�W��\������
	CDebugProc::Print("%s\n��M�o�C�g��[%d]\n", m_aReceiveMessage[nIdxClient], m_nLengthData[nIdxClient]);

	// ���b�Z�[�W����R�}���h��K�p
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
		strcat(m_aSendMessage[nIdxClient], "���b�Z�[�W���M");
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
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_CHARA_DATA) == 0)
	{
		Command_CharaDataClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_GAME_DATA) == 0)
	{
		Command_GameDataClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_TUTO_DATA) == 0)
	{
		Command_TutorialDataClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_NONE) == 0
		&& CServerLib::Memcmp(m_aReceiveMessage[(nIdxClient + 1) % 2], CD_SEND_CLIENT) != 0)
	{
		strcat(m_aSendMessage[nIdxClient], "???");
	}
}

//=============================================================================
//    �N���C�A���g�փ��b�Z�[�W�𑗐M���鏈��
//=============================================================================
void CServer::SendMessageToClient(int nIdxClient)
{
	// �N���C�A���g�փ��b�Z�[�W�𑗐M
	send(m_sockClient[nIdxClient], m_aSendMessage[nIdxClient], strlen(m_aSendMessage[nIdxClient]), 0);
	memset(m_aSendMessage[nIdxClient], 0, MAX_SERVER_DATA);
}

//=============================================================================
//    Hello���b�Z�[�W���N���C�A���g�֑��M���鏈��
//=============================================================================
void CServer::Command_Hello(int nIdxClient)
{
	// �N���C�A���g�ւ̃��b�Z�[�W��ݒ�
	strcat(m_aSendMessage[nIdxClient], "Hello Client");
}

//=============================================================================
//    �N���C�A���g����N���C�A���g�փ��b�Z�[�W���M���鏈��
//=============================================================================
void CServer::Command_Send_Client(int nIdxClient)
{
}

//=============================================================================
//    �N���C�A���g�Ƃ̌�M���I�����鏈��
//=============================================================================
void CServer::Command_DeleteClient(int nIdxClient)
{
	// �\�P�b�g(�N���C�A���g)�̊J��
	closesocket(m_sockClient[nIdxClient]);
	m_bConnected[nIdxClient] = false;
	m_nNumConnect--;
	m_hWaitConnectHander[nIdxClient] = (HANDLE)_beginthreadex(NULL, 0, WaitConnectHandler, this, 0, NULL);
	CDebugProc::ClearString();
	CDebugProc::Print("%d�Ԃ̐ڑ����r�₦�܂����E�E�E\n", nIdxClient);
	CDebugProc::Print("�ڑ���t�J�n\n");
}

//=============================================================================
//    �N���C�A���g����f�[�^��ǂݎ�鏈��
//=============================================================================
void CServer::Command_SendDataClient(int nIdxClient)
{
	float aPlayerPos[3];        // �v���C���[�̍��W
	char aStr[MAX_SERVER_DATA];
	char *pStr;
	pStr = &m_aReceiveMessage[nIdxClient][0];

	// ��M�������b�Z�[�W����f�[�^��ǂݎ��
	pStr= CServerLib::ReadFloat3(pStr, CD_SE_DATA, &aPlayerPos[0]);

	// ��M�������W��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
	strcpy(m_aSendMessage[(nIdxClient + 1) % 2], CServerLib::WriteFloat3(&aPlayerPos[0], aStr));
}

//=============================================================================
//    �N���C�A���g����L�����f�[�^��ǂݎ�鏈��
//=============================================================================
void CServer::Command_CharaDataClient(int nIdxClient)
{
	int nWord = 0;
	int nState = 0;
	int nSelectPlayer = 0;
	int nSelectStageSide = 0;
	int nSelectStageVertical = 0;
	char aStr[MAX_SERVER_DATA];
	char *pStr;
	pStr = &m_aReceiveMessage[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][0];

	// ��M�������b�Z�[�W����f�[�^��ǂݎ��
	nState = CServerLib::ReadInt(pStr, CD_CHARA_DATA);
	nWord = CServerLib::PopString(pStr, CD_CHARA_DATA);
	pStr += nWord;
	nSelectPlayer = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// �f�[�^��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
	strcpy(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nState, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nSelectPlayer, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	if (nIdxClient != 0)
	{
		nSelectStageSide = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nSelectStageVertical = CServerLib::ReadInt(pStr, "");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nSelectStageSide, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nSelectStageVertical, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	if (m_bConnected[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == false)
	{// �܂�������l���ڑ��ł��Ă��Ȃ�
		strcpy(m_aSendMessage[nIdxClient], "???");
	}
}

//=============================================================================
//    �N���C�A���g����Q�[���f�[�^��ǂݎ�鏈��
//=============================================================================
void CServer::Command_GameDataClient(int nIdxClient)
{
	char *pStr;
	pStr = &m_aReceiveMessage[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][0];

	// ��M�������b�Z�[�W����f�[�^��ǂݎ��
	if (CServerLib::Memcmp(pStr, CD_GAME_DATA) == 0)
	{
		// ���o��
		pStr = CServerLib::HeadPutout(pStr, CD_GAME_DATA);

		// �v���C���[�̃f�[�^��ݒ肷��
		pStr = SetGameData_Player(nIdxClient, pStr);

		if (nIdxClient != 0)
		{
			// �Q�[���̃f�[�^��ݒ肷��
			pStr = SetGameData_Game(nIdxClient, pStr);

			// �G�̃f�[�^��ݒ肷��
			pStr = SetGameData_Enemy(nIdxClient, pStr);

			// �G�̒e�f�[�^��ݒ肷��
			pStr = PushBulletData_Enemy(nIdxClient, pStr);
			SetGameData_EnemyBullet(nIdxClient, pStr);

			// �����u���b�N�f�[�^��ݒ肷��
			pStr = SetGameData_Block(nIdxClient, pStr);

			// �A�C�e���̃f�[�^��ݒ肷��
			pStr = SetGameData_Item(nIdxClient, pStr);

			// �X�|�[�������G�̐���ݒ肷��
			pStr = SetGameData_SpawnEnemyCount(nIdxClient, pStr);

			// �}�b�v�C�x���g�̃f�[�^��ݒ肷��
			pStr = SetGameData_MapEvent(nIdxClient, pStr);
		}

		// �v���C���[���m�̒e�f�[�^��ݒ肷��
		pStr = PushBulletData_Player(nIdxClient, pStr);
		SetGameData_PlayerBullet(nIdxClient, pStr);

		if (nIdxClient == 0)
		{
			// �����u���b�N�f�[�^��ݒ肷��
			pStr = SetGameData_Block(nIdxClient, pStr);

			// �����G�f�[�^��ݒ肷��
			pStr = SetGameData_DeleteEnemy(nIdxClient, pStr);

			// ����̏�Ԃ�ݒ肷��
			pStr = SetGameData_ClientState(nIdxClient, pStr);

			// �����A�C�e���f�[�^��ݒ肷��
			pStr = SetGameData_DeleteItem(nIdxClient, pStr);
		}

		// ����̓|�����G�̐���ݒ肷��
		pStr = SetGameData_BreakEnemy(nIdxClient, pStr);

		// �e��������������ݒ肷��
		pStr = SetGameData_HitBullet(nIdxClient, pStr);
	}
	else
	{
		strcpy(m_aSendMessage[nIdxClient], "???");
	}

	if (m_bConnected[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == false)
	{// �܂�������l���ڑ��ł��Ă��Ȃ�
		strcpy(m_aSendMessage[nIdxClient], "???");
	}
}

//=============================================================================
//    �N���C�A���g����`���[�g���A���f�[�^��ǂݎ�鏈��
//=============================================================================
void CServer::Command_TutorialDataClient(int nIdxClient)
{
	char *pStr;
	pStr = &m_aReceiveMessage[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][0];

	// ��M�������b�Z�[�W����f�[�^��ǂݎ��
	if (CServerLib::Memcmp(pStr, CD_TUTO_DATA) == 0)
	{
		// ���o��
		pStr = CServerLib::HeadPutout(pStr, CD_TUTO_DATA);

		// �v���C���[�̃f�[�^��ݒ肷��
		pStr = SetGameData_Player(nIdxClient, pStr);

		if (nIdxClient != 0)
		{
			// �Q�[���̃f�[�^��ݒ肷��
			pStr = SetGameData_Game(nIdxClient, pStr);

			// �G�̃f�[�^��ݒ肷��
			pStr = SetGameData_Enemy(nIdxClient, pStr);

			// �G�̒e�f�[�^��ݒ肷��
			pStr = PushBulletData_Enemy(nIdxClient, pStr);
			SetGameData_EnemyBullet(nIdxClient, pStr);

			// �����u���b�N�f�[�^��ݒ肷��
			pStr = SetGameData_Block(nIdxClient, pStr);
		}

		// �v���C���[���m�̒e�f�[�^��ݒ肷��
		pStr = PushBulletData_Player(nIdxClient, pStr);
		SetGameData_PlayerBullet(nIdxClient, pStr);

		if (nIdxClient == 0)
		{
			// �����u���b�N�f�[�^��ݒ肷��
			pStr = SetGameData_Block(nIdxClient, pStr);

			// �����G�f�[�^��ݒ肷��
			pStr = SetGameData_DeleteEnemy(nIdxClient, pStr);
		}

		// �e��������������ݒ肷��
		pStr = SetGameData_HitBullet(nIdxClient, pStr);
	}
	else
	{
		strcpy(m_aSendMessage[nIdxClient], "???");
	}

	if (m_bConnected[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == false)
	{// �܂�������l���ڑ��ł��Ă��Ȃ�
		strcpy(m_aSendMessage[nIdxClient], "???");
	}
}

//=============================================================================
//    �N���C�A���g�փv���C���[�̃f�[�^��ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_Player(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nNumPlayer = 0;
	float fPlayerPos[3] = { 0.0f ,0.0f, 0.0f };
	float fPlayerRot[3] = { 0.0f ,0.0f, 0.0f };
	int nPlayerState = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// �v���C���[�̐l����ǂݎ��
	nNumPlayer = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcpy(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumPlayer, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (nNumPlayer != 0)
	{// ����̃v���C���[�������Ă���
		pStr = CServerLib::ReadFloat3(pStr, "", &fPlayerPos[0]);
		pStr = CServerLib::ReadFloat3(pStr, "", &fPlayerRot[0]);
		nPlayerState = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// �f�[�^��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fPlayerPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fPlayerRot[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nPlayerState, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}


	// �z�X�g����Ȃ�������
	if (nIdxClient != 0)
	{
		nNumPlayer = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumPlayer, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	return pStr;
}

//=============================================================================
//    �N���C�A���g�փQ�[���̃f�[�^��ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_Game(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nGameState = 0;
	int nMapIdx = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// �f�[�^��ǂݎ��
	nGameState = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	nMapIdx = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// �f�[�^��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nGameState, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nMapIdx, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    �N���C�A���g�֓G�̃f�[�^��ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_Enemy(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumEnemy = 0;
	float fEnemyPos[3] = { 0.0f ,0.0f,0.0f };
	float fEnemyRot[3] = { 0.0f ,0.0f,0.0f };
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// �G�̃f�[�^����ǂݎ��
	nNumEnemy = CServerLib::ReadInt(pStr, "");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumEnemy, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumEnemy == 0) return pStr;

	// �G�̐����f�[�^��ݒ�
	for (int nCntEnemy = 0; nCntEnemy < nNumEnemy; nCntEnemy++)
	{
		// �f�[�^��ǂݎ��
		pStr = CServerLib::ReadFloat3(pStr, "", &fEnemyPos[0]);
		pStr = CServerLib::ReadFloat3(pStr, "", &fEnemyRot[0]);
		nEnemyType = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nEnemyIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// �f�[�^���N���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fEnemyPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fEnemyRot[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nEnemyType, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nEnemyIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcpy(aStr, "\0");
	}

	return pStr;
}

//=============================================================================
//    �N���C�A���g�փA�C�e���̃f�[�^��ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_Item(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumItem = 0;
	float fItemPos[3] = { 0.0f ,0.0f,0.0f };
	int nItemIdx = 0;
	int nItemType = 0;

	// �A�C�e���̃f�[�^����ǂݎ��
	nNumItem = CServerLib::ReadInt(pStr, "");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumItem, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumItem == 0) return pStr;

	// �A�C�e���̐����f�[�^��ݒ�
	for (int nCntEnemy = 0; nCntEnemy < nNumItem; nCntEnemy++)
	{
		// �f�[�^��ǂݎ��
		pStr = CServerLib::ReadFloat3(pStr, "", &fItemPos[0]);
		nItemIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nItemType = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// �f�[�^���N���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fItemPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nItemIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nItemType, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcpy(aStr, "\0");
	}

	return pStr;
}

//=============================================================================
//    �N���C�A���g�փX�|�[�������G�̏���ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_SpawnEnemyCount(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nSpawn = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// �f�[�^��ǂݎ��
	nSpawn = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// �f�[�^��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nSpawn, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    �N���C�A���g�փ}�b�v�C�x���g����ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_MapEvent(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nMapEvent = 0;
	int nNextEvent = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// �f�[�^��ǂݎ��
	nMapEvent = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	nNextEvent = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// �f�[�^��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nMapEvent, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNextEvent, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    �N���C�A���g�֒e��������������ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_HitBullet(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nHitBullet = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// �f�[�^��ǂݎ��
	nHitBullet = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// �f�[�^��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nHitBullet, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    �v���C���[�̒e�����v�b�V�����鏈��
//=============================================================================
char *CServer::PushBulletData_Player(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	float fBulletPos[3] = { 0.0f ,0.0f, 0.0f };
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̐���ݒ�
	m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	if (m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == 0) return pStr;

	// �e�̐������f�[�^���v�b�V��
	for (int nCntData = 0; nCntData < m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM]; nCntData++)
	{
		// �f�[�^�ǂݎ��
		pStr = CServerLib::ReadFloat3(pStr, "", &fBulletPos[0]);
		nBulletIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nBulletType = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// �f�[�^�ݒ�
		m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].pos = D3DXVECTOR3(fBulletPos[0], fBulletPos[1], fBulletPos[2]);
		m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].nIdx = nBulletIdx;
		m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].nType = nBulletType;
	}

	return pStr;
}

//=============================================================================
//    �G�̒e�����v�b�V�����鏈��
//=============================================================================
char *CServer::PushBulletData_Enemy(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	float fBulletPos[3] = { 0.0f ,0.0f,0.0f };
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̐���ݒ�
	m_nNumEnemyBulletData = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	if (m_nNumEnemyBulletData == 0) return pStr;

	// �e�̐������f�[�^���v�b�V��
	for (int nCntData = 0; nCntData < m_nNumEnemyBulletData; nCntData++)
	{
		// �f�[�^�ǂݎ��
		pStr = CServerLib::ReadFloat3(pStr, "", &fBulletPos[0]);
		nBulletIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;
		nBulletType = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// �f�[�^�ݒ�
		m_EnemyBulletData[nCntData].pos = D3DXVECTOR3(fBulletPos[0], fBulletPos[1], fBulletPos[2]);
		m_EnemyBulletData[nCntData].nIdx = nBulletIdx;
		m_EnemyBulletData[nCntData].nType = nBulletType;
	}

	return pStr;
}

//=============================================================================
//    �N���C�A���g�֓G�̒e�f�[�^��ݒ肷�鏈��
//=============================================================================
void CServer::SetGameData_EnemyBullet(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	float fBulletPos[3] = { 0.0f ,0.0f, 0.0f };
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �G�̒e����ݒ�
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(m_nNumEnemyBulletData, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (m_nNumEnemyBulletData == 0) return;

	// �G�̒e�����f�[�^��ݒ�
	for (int nCntData = 0; nCntData < m_nNumEnemyBulletData; nCntData++)
	{
		// �f�[�^���쐬
		fBulletPos[0] = m_EnemyBulletData[nCntData].pos.x;
		fBulletPos[1] = m_EnemyBulletData[nCntData].pos.y;
		fBulletPos[2] = m_EnemyBulletData[nCntData].pos.z;
		nBulletIdx = m_EnemyBulletData[nCntData].nIdx;
		nBulletType = m_EnemyBulletData[nCntData].nType;

		// �f�[�^���N���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fBulletPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nBulletIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nBulletType, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcpy(aStr, "\0");
	}
}

//=============================================================================
//    �N���C�A���g�փv���C���[�̒e�f�[�^��ݒ肷�鏈��
//=============================================================================
void CServer::SetGameData_PlayerBullet(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	float fBulletPos[3] = { 0.0f ,0.0f, 0.0f };
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �v���C���[�̒e����ݒ�
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM], aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM] == 0) return;

	// �v���C���[�̒e�����f�[�^��ݒ�
	for (int nCntData = 0; nCntData < m_nNumPlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM]; nCntData++)
	{
		// �f�[�^���쐬
		fBulletPos[0] = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].pos.x;
		fBulletPos[1] = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].pos.y;
		fBulletPos[2] = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].pos.z;
		nBulletIdx = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].nIdx;
		nBulletType = m_PlayerBulletData[(nIdxClient + 1) % MAX_SERVER_CONNECTING_NUM][nCntData].nType;

		// �f�[�^���N���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteFloat3(&fBulletPos[0], aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nBulletIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nBulletType, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
		strcpy(aStr, "\0");
	}
}

//=============================================================================
//    �N���C�A���g�֏����u���b�N�̃f�[�^��ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_Block(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumDeleteBlock = 0;
	int nDeleteIdx = 0;

	// �����u���b�N�̐����擾
	nNumDeleteBlock = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumDeleteBlock, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (nNumDeleteBlock == 0)return pStr;

	// �����u���b�N�̐������f�[�^��ݒ�
	for (int nCntData = 0; nCntData < nNumDeleteBlock; nCntData++)
	{
		// �f�[�^�ǂݎ��
		nDeleteIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// �f�[�^���N���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nDeleteIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	return pStr;
}

//=============================================================================
//    �N���C�A���g�֏����G�̃f�[�^��ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_DeleteEnemy(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumDeleteEnemy = 0;
	int nDeleteIdx = 0;

	// �����G�̐����擾
	nNumDeleteEnemy = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumDeleteEnemy, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (nNumDeleteEnemy == 0)return pStr;

	// �����G�̐������f�[�^��ݒ�
	for (int nCntData = 0; nCntData < nNumDeleteEnemy; nCntData++)
	{
		// �f�[�^�ǂݎ��
		nDeleteIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// �f�[�^���N���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nDeleteIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	return pStr;
}

//=============================================================================
//    �N���C�A���g�֏����A�C�e���̃f�[�^��ݒ肷�鏈��
//=============================================================================
char *CServer::SetGameData_DeleteItem(int nIdxClient, char *pStr)
{
	char aStr[MAX_SERVER_DATA] = "\0";
	int nWord = 0;
	int nNumDeleteItem = 0;
	int nDeleteIdx = 0;

	// �����A�C�e���̐����擾
	nNumDeleteItem = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumDeleteItem, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");
	if (nNumDeleteItem == 0)return pStr;

	// �����A�C�e���̐������f�[�^��ݒ�
	for (int nCntData = 0; nCntData < nNumDeleteItem; nCntData++)
	{
		// �f�[�^�ǂݎ��
		nDeleteIdx = CServerLib::ReadInt(pStr, "");
		nWord = CServerLib::PopString(pStr, "");
		pStr += nWord;

		// �f�[�^���N���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
		strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nDeleteIdx, aStr));
		strcat(m_aSendMessage[nIdxClient], " ");
	}

	return pStr;
}

//=============================================================================
//    �N���C�A���g�֑���̏�Ԃ𑗐M���鏈��
//=============================================================================
char *CServer::SetGameData_ClientState(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nGameState = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// �f�[�^��ǂݎ��
	nGameState = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;

	// �f�[�^��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nGameState, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    �N���C�A���g�֑���̓|�����G�̐��𑗐M���鏈��
//=============================================================================
char *CServer::SetGameData_BreakEnemy(int nIdxClient, char *pStr)
{
	int nWord = 0;
	int nNumBreak = 0;
	int nScore = 0;
	char aStr[MAX_SERVER_DATA] = "\0";

	// �ʏ�̓G
	nNumBreak = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumBreak, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	// ���b�ԃ^���N
	nNumBreak = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumBreak, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	// ���˖C�^���N
	nNumBreak = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumBreak, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	// �w�r�[�^���N
	nNumBreak = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nNumBreak, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	// ����̃X�R�A
	nScore = CServerLib::ReadInt(pStr, "");
	nWord = CServerLib::PopString(pStr, "");
	pStr += nWord;
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nScore, aStr));
	strcat(m_aSendMessage[nIdxClient], " ");

	return pStr;
}

//=============================================================================
//    �N���C�A���g�֔ԍ��𑗐M���鏈��
//=============================================================================
void CServer::Command_UserIdClient(int nIdxClient)
{
	// �N���C�A���g�ԍ��𑗐M
	char aStr[MAX_SERVER_DATA] = "\0";
	strcpy(m_aSendMessage[nIdxClient], USER_ID);
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nIdxClient, aStr));
}


//*****************************************************************************
//    CClient�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CClient::CClient()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CClient::~CClient()
{

}

//=============================================================================
//    ��������
//=============================================================================
CClient *CClient::Create(char *pSeverAddr)
{
	CClient *pClient = NULL;      // �N���C�A���g�N���X�^�̃|�C���^
	if (pClient == NULL)
	{// ����������ɂȂ��Ă���
		pClient = new CClient;
		if (pClient != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pClient->Init(pSeverAddr)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pClient;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CClient::Init(char *pSeverAddr)
{
	// IP�A�h���X��ݒ�
	if (pSeverAddr == NULL) return E_FAIL;
	SetServerAddr(pSeverAddr);

	// �\�P�b�g�̍쐬
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_aServerAddr);
	m_bConnected = false;

	// �T�[�o�[�֐ڑ�
	connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));
	m_bConnected = true;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CClient::Uninit(void)
{
	// �T�[�o�[�Ƃ̐ڑ����I��
	Command_DeleteClient();

	// �\�P�b�g(�N���C�A���g)�̊J��
	closesocket(m_sockClient);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CClient::Update(void)
{
	if (m_bConnected != true)
	{// �T�[�o�[�Ɛڑ��ł��Ă��Ȃ�
		WaitConnect();
	}
	else
	{// �T�[�o�[�Ɛڑ��ł���
		ExChangeMessage();
	}
}

//=============================================================================
//    �T�[�o�[IP�A�h���X��ݒ肷�鏈��
//=============================================================================
void CClient::SetServerAddr(char *pServerAddr)
{
	strcpy(m_aServerAddr, pServerAddr);
}

//=============================================================================
//    �T�[�o�[�Ƃ̐ڑ��ҋ@����
//=============================================================================
void CClient::WaitConnect(void)
{
	// �\�P�b�g�̍쐬
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_aServerAddr);

	if (connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer)) == SOCKET_ERROR)
	{
		CDebugProc::Print("�ڑ����r�₦�܂����E�E�E\n");
		CDebugProc::Print("�ڑ��J�n\n");
		m_bConnected = false;
	}
	else
	{
		m_bConnected = true;
		m_nLengthData = 0;
	}
}

//=============================================================================
//    �T�[�o�[�Ƃ̃��b�Z�[�W��������
//=============================================================================
void CClient::ExChangeMessage(void)
{
	// �T�[�o�[�֑��郁�b�Z�[�W���쐬
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	if (pKeyboard->GetPress(DIK_RETURN) == true)
	{
		Command_Hello();
	}
	else if (pKeyboard->GetPress(DIK_SPACE) == true)
	{
		Command_Send_Client();
	}
	else if (pKeyboard->GetPress(DIK_1) == true)
	{
		Command_DeleteClient();
	}
	else
	{
		strcat(m_aSendMessage, CD_NONE);
	}

	// �T�[�o�[�փf�[�^(���b�Z�[�W)�𑗐M
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// �T�[�o�[����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	if (m_nLengthData > 1)
	{
		// ���b�Z�[�W��\������
		CDebugProc::Print("%s\n", m_aReceiveMessage);

		// ���b�Z�[�W��\������
		CDebugProc::Print("��M�o�C�g��[%d]\n", m_nLengthData);
	}
}

//=============================================================================
//    Hello���b�Z�[�W���󂯎�鏈��
//=============================================================================
void CClient::Command_Hello(void)
{
	strcat(m_aSendMessage, CD_HELLO);
}

//=============================================================================
//    �ʂ̃N���C�A���g�փ��b�Z�[�W�𑗐M���鏈��
//=============================================================================
void CClient::Command_Send_Client(void)
{
	strcat(m_aSendMessage, CD_SEND_CLIENT);
}

//=============================================================================
//    �T�[�o�[�Ƃ̐ڑ����I�����鏈��
//=============================================================================
void CClient::Command_DeleteClient(void)
{
	strcat(m_aSendMessage, CD_DELETE);
}