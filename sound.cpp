//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************
typedef struct
{
	const char *Filename;	// �t�@�C����
	bool Loop;				// ���[�v���邩�ǂ���
} PARAM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD Buffersize, DWORD Bufferoffset);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IXAudio2 *XAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *MasteringVoice = NULL;			// �}�X�^�[�{�C�X
IXAudio2SourceVoice *SourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
BYTE *DataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
DWORD SizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

// �e���f�ނ̃p�����[�^
PARAM Param[SOUND_LABEL_MAX] =
{
	{"data/BGM/bgm_GAME_MODE.wav", true},					// BGM0
	{"data/BGM/bgm_BOSS_MODE.wav", true},					// BGM1
	{"data/SE/shot000.wav", false},							// �e���ˉ�
	{"data/SE/explosion000.wav", false},					// ������
	{"data/SE/se_maoudamashii_explosion05.wav", false},		// �������Q
	{"data/SE/se_Enter�I��.wav", false},					// �G���^�[�I��
	{"data/SE/se_mission_false.wav", false},				// �G���^�[�I��
	{"data/SE/se_mission_true.wav", false},					// �G���^�[�I��
	{"data/SE/���b�N��.wav", false},						// ���b�N��
	{"data/SE/�L�����Z����.wav", false},					// �L�����Z����
	{"data/SE/�G�l�~�[������.wav", false},					// �G�l�~�[�̔�����
	
};

//=============================================================================
// ������
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&XAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();
		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = XAudio2->CreateMasteringVoice(&MasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(XAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			XAudio2->Release();
			XAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int CntSound = 0; CntSound < SOUND_LABEL_MAX; CntSound++)
	{
		HANDLE hFile;
		DWORD ChunkSize = 0;
		DWORD ChunkPosition = 0;
		DWORD Filetype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(Param[CntSound].Filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &ChunkSize, &ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &Filetype, sizeof(DWORD), ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if(Filetype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &ChunkSize, &ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, ChunkSize, ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &SizeAudio[CntSound], &ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		DataAudio[CntSound] = (BYTE*)malloc(SizeAudio[CntSound]);
		hr = ReadChunkData(hFile, DataAudio[CntSound], SizeAudio[CntSound], ChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = XAudio2->CreateSourceVoice(&SourceVoice[CntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = SizeAudio[CntSound];
		buffer.pAudioData = DataAudio[CntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = 0;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		SourceVoice[CntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	// �ꎞ��~
	for(int CntSound = 0; CntSound < SOUND_LABEL_MAX; CntSound++)
	{
		if(SourceVoice[CntSound])
		{
			// �ꎞ��~
			SourceVoice[CntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			SourceVoice[CntSound]->DestroyVoice();
			SourceVoice[CntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(DataAudio[CntSound]);
			DataAudio[CntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	MasteringVoice->DestroyVoice();
	MasteringVoice = NULL;
	
	if(XAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		XAudio2->Release();
		XAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(��~)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = SizeAudio[label];
	buffer.pAudioData = DataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = 0;

	// ��Ԏ擾
	SourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		SourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		SourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	SourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	SourceVoice[label]->Start(0);

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	SourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		SourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		SourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void StopSound(void)
{
	// �ꎞ��~
	for(int CntSound = 0; CntSound < SOUND_LABEL_MAX; CntSound++)
	{
		if(SourceVoice[CntSound])
		{
			// �ꎞ��~
			SourceVoice[CntSound]->Stop(0);
		}
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD Read;
	DWORD ChunkType;
	DWORD ChunkDataSize;
	DWORD RIFFDataSize = 0;
	DWORD FileType;
	DWORD BytesRead = 0;
	DWORD Offset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &ChunkType, sizeof(DWORD), &Read, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &ChunkDataSize, sizeof(DWORD), &Read, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(ChunkType)
		{
		case 'FFIR':
			RIFFDataSize  = ChunkDataSize;
			ChunkDataSize = 4;
			if(ReadFile(hFile, &FileType, sizeof(DWORD), &Read, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, ChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		Offset += sizeof(DWORD) * 2;
		if(ChunkType == format)
		{
			*pChunkSize         = ChunkDataSize;
			*pChunkDataPosition = Offset;

			return S_OK;
		}

		Offset += ChunkDataSize;
		if(BytesRead >= RIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD Buffersize, DWORD Bufferoffset)
{
	DWORD Read;
	
	if(SetFilePointer(hFile, Bufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, Buffersize, &Read, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

