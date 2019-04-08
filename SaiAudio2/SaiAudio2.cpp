#include "SaiAudio2.h"

// ������
SAI_AUDIO2_INTERFACE::SAI_AUDIO2_INTERFACE()
{
	// COM���C�u�����̏�����
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		// ����+1
		initCheck++;

		// XAudio2�I�u�W�F�N�g�̍쐬
		if (SUCCEEDED(XAudio2Create(&xAudio2)))
		{
			// ����+1
			initCheck++;

			// �}�X�^�[�{�C�X�̍쐬
			if (SUCCEEDED(xAudio2->CreateMasteringVoice(&masteringVoice)))
			{
				// ����+1
				initCheck++;

				// �t�@�C���̓ǂݍ��݃N���X
				wavFile = new LOAD_WAV;
			}
		}
	}
}


// �I������
SAI_AUDIO2_INTERFACE::~SAI_AUDIO2_INTERFACE()
{
	// COM���C�u�����̏I������
	CoUninitialize();

	// �}�X�^�[�{�C�X
	if (masteringVoice != NULL)
	{
		masteringVoice->DestroyVoice();
		masteringVoice = NULL;
	}

	// �C���^�[�t�F�[�X
	if (xAudio2 != NULL)
	{
		xAudio2->Release();
		xAudio2 = NULL;
	}

	// �{�����[���̃��������
	free(channelVolume);

	// �t�@�C���̓ǂݍ��݃N���X
	delete wavFile;
}

// �������̌���
bool SAI_AUDIO2_INTERFACE::InitCheck()
{
	// ���������� = �O���SUCCEEDED
	if (initCheck != SAI_INIT_SUCCEEDED)
	{
		return false;
	}

	return true;
}

// �Đ�
HRESULT SAI_AUDIO2_INTERFACE::PlayVoice(SAI_VOICE_TOOL *voiceTool)
{
	// �{�C�X�ƍĐ���Ԃ̃`�F�b�N
	if ((voiceTool->sourceVoice != NULL)&&(!voiceTool->isPlay))
	{
		// �Đ�
		voiceTool->sourceVoice->Start();

		return S_OK;
	}

	return S_FALSE;
}

// ��~
HRESULT SAI_AUDIO2_INTERFACE::StopVoice(SAI_VOICE_TOOL *voiceTool)
{
	// �{�C�X�ƍĐ���Ԃ̃`�F�b�N
	if ((voiceTool->sourceVoice != NULL) && (voiceTool->isPlay))
	{
		// ��~
		voiceTool->sourceVoice->Stop();

		return S_OK;
	}

	return S_FALSE;
}

// �ꎞ��~
HRESULT SAI_AUDIO2_INTERFACE::PauseVoice(SAI_VOICE_TOOL *voiceTool)
{
	// �{�C�X�ƍĐ���Ԃ̃`�F�b�N
	if ((voiceTool->sourceVoice != NULL) && (voiceTool->isPlay))
	{
		// �ꎞ��~
		voiceTool->sourceVoice->Stop(XAUDIO2_PLAY_TAILS);

		return S_OK;
	}

	return S_FALSE;
}

//==============================================================
// ���[�h�T�E���h
//==============================================================
IXAudio2SourceVoice *SAI_AUDIO2_INTERFACE::LoadXAudio2Voice(char *path, SAI_VOICE_TOOL *voiceTool)
{
	IXAudio2SourceVoice *sourceVoice = NULL;	// �\�[�X�{�C�X
	WAVEFORMATEX		pcm;					// WAVEFORMATEX�\����
	XAUDIO2_BUFFER		buffer;					// �o�b�t�@

	{// ���[�J���ϐ��̏�����

		// �\���̂̏�����
		memset(&pcm, 0, sizeof(WAVEFORMATEX));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// wav�t�@�C���̓ǂݍ���(WAVEFORMATEX�\����)
		pcm = wavFile->LoadWavFile(path, voiceTool->wav);

		// �o�b�t�@�̐ݒ�
		buffer.pAudioData	= (BYTE*)voiceTool->wav->data.waveData;
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes	= voiceTool->wav->data.waveSize;
		buffer.LoopCount	= voiceTool->loopCnt;
	}

	// �\�[�X�{�C�X�̍쐬
	if (FAILED(xAudio2->CreateSourceVoice(&sourceVoice, &pcm, NULL, XAUDIO2_MAX_FREQ_RATIO)))
	{
		return NULL;
	}

	// �o�b�t�@���o
	if (FAILED(sourceVoice->SubmitSourceBuffer(&buffer)))
	{
		return NULL;
	}

	{// �{�����[���̏�����
		// �������m��[�`�����l����]
		channelVolume = (float *)malloc(voiceTool->wav->fmt.fmtChannel * sizeof(float));
		for (int i = 0; i < voiceTool->wav->fmt.fmtChannel; i++)
		{
			channelVolume[i] = SAI_DEFAULT_VOLUME;
		}
	}

	return sourceVoice;
}
