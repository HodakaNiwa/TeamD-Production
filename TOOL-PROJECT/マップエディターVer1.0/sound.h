//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:	//誰からもアクセス可能
	//*************************************
	// サウンドファイル
	//*************************************
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// BGM0 (OP)
		SOUND_LABEL_BGM001,			// BGM1	(TUTORIAL)
		SOUND_LABEL_BGM002,			// BGM2	(GAME)
		SOUND_LABEL_BGM003,			// BGM3	(CLEAR)
		SOUND_LABEL_BGM004,			// BGM4	(GAMEOVER)
		SOUND_LABEL_BGM005,			// BGM5	(RANKING)
		SOUND_LABEL_SE_BULLET,		// 死亡
		SOUND_LABEL_SE_SHIT,		// 死亡
		SOUND_LABEL_SE_DEATH,		// 死亡
		SOUND_LABEL_SE_PAUSE,		//ポーズ
		SOUND_LABEL_SE_PAUSESELECT,	//ポーズセレクト
		SOUND_LABEL_SE_PAUSECURSOR,	//ポーズカーソル
		SOUND_LABEL_SE_PAUSECANCEL,	//ポーズキャンセル
		SOUND_LABEL_SE_ALARM,		// 死亡
		SOUND_LABEL_SE_GET,			// アイテム獲得
		SOUND_LABEL_SE_FEVER,		//フィーバー
		SOUND_LABEL_SE_CANSEL,		//キャンセル
		SOUND_LABEL_SE_KETTEI,		//決定
		SOUND_LABEL_SE_SLIDE,		//決定
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	CSound();
	~CSound();
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
private:	//自分だけがアクセス可能
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/bgm000.wav", -1 },		// BGM0 (OP)
		{ "data/BGM/bgm001.wav", -1 },		// BGM1	(TUTORIAL)
		{ "data/BGM/bgm002.wav", -1 },		// BGM2	(GAME)
		{ "data/BGM/bgm003.wav", 0 },		// BGM3	(CLEAR)
		{ "data/BGM/bgm004.wav", 0 },		// BGM4	(GAMEOVER)
		{ "data/BGM/bgm005.wav", -1 },		// BGM5	(RANKING)
		{ "data/SE/bullet.wav", 0 },			// 死亡
		{ "data/SE/fall.wav", 0 },			// 死亡
		{ "data/SE/death.wav", 0 },			// 死亡
		{ "data/SE/pause.wav", 0 },			// ポーズ
		{ "data/SE/pauseselect.wav", 0 },	// ポーズ
		{ "data/SE/cursor.wav", 0 },		// ポーズ
		{ "data/SE/cancel.wav", 0 },		// ポーズ
		{ "data/SE/alarm.wav", 0 },			// 警報
		{ "data/SE/get.wav", 0 },			// アイテム獲得
		{ "data/SE/fever.wav", 0 },			// フィーバー
		{ "data/SE/back.wav", 0 },			// キャンセル
		{ "data/SE/kettei.wav", 0 },		// 決定
		{ "data/SE/tutorial_advance.wav", 0 },		// 決定
	};
};
#endif
