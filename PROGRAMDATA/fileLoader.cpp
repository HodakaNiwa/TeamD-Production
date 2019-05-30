//*****************************************************************************
//
//     �t�@�C���ǂݍ��݂̏���[fileLoader.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "fileLoader.h"
#include "functionlib.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CFileLoader::CFileLoader(char *pLoadFileName)
{
	// �e��l�̃N���A
	m_pFile = NULL;                          // �t�@�C���|�C���^
	strcpy(m_aLoadFileName, pLoadFileName);  // �ǂݍ��ރt�@�C����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CFileLoader::~CFileLoader()
{

}

//=============================================================================
//    ��������
//=============================================================================
CFileLoader *CFileLoader::Create(char *pLoadFileName)
{
	CFileLoader *pFileLoader = NULL;	// �t�@�C���ǂݍ��݃N���X�ւ̃|�C���^
	if (pFileLoader == NULL)
	{
		pFileLoader = new CFileLoader(pLoadFileName);
		if (pFileLoader != NULL)
		{
			// ����������
			if (FAILED(pFileLoader->Init()))
			{
				delete pFileLoader;
				return NULL;
			}
		}
	}

	return pFileLoader;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CFileLoader::Init(void)
{
	// �t�@�C�����J��
	if (m_pFile == NULL)
	{
		m_pFile = fopen(m_aLoadFileName, "r");
		if (m_pFile == NULL)
		{// �t�@�C���|�C���^�擾�Ɏ��s
			return E_FAIL;
		}
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CFileLoader::Uninit(void)
{
	// �t�@�C�������
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
	}
}

//=============================================================================
//    �t�@�C�����當������擾���鏈��
//=============================================================================
char *CFileLoader::GetString(char *pSrc)
{
	// �t�@�C������L���ȕ�������擾����
	if (m_pFile != NULL)
	{
		strcpy(pSrc, CFunctionLib::ReadLine(m_pFile, pSrc));
	}

	return pSrc;
}

//=============================================================================
//    �ǂݍ��ރt�@�C������ݒ肷��
//=============================================================================
void CFileLoader::SetLoadfileName(char *pFileName)
{
	strcpy(m_aLoadFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ރt�@�C�������擾����
//=============================================================================
char *CFileLoader::GetLoadfileName(void)
{
	return m_aLoadFileName;
}