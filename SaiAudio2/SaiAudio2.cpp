#include "SaiAudio2.h"

// ������
SAI_AUDIO2_INTERFACE::SAI_AUDIO2_INTERFACE(void)
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

				// �N���Xnew
				wavFile = new SAI_LOAD_WAV;
			}
		}
	}
}


// �I������
SAI_AUDIO2_INTERFACE::~SAI_AUDIO2_INTERFACE(void)
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

	// �N���Xdelete
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

// �}�X�^�[�{�C�X���擾
IXAudio2MasteringVoice *SAI_AUDIO2_INTERFACE::GetMasterVoice(void)
{
	return masteringVoice;
}

//==============================================================
// ���[�h�T�E���h
//==============================================================
IXAudio2SourceVoice *SAI_AUDIO2_INTERFACE::LoadXAudio2Voice(const char *path, SAI_VOICE_TOOL *svt)
{
	IXAudio2SourceVoice *sourceVoice = NULL;	// �\�[�X�{�C�X
	WAVEFORMATEX		pcm;					// WAVEFORMATEX�\����
	XAUDIO2_BUFFER		buffer;					// �o�b�t�@

	{// ���[�J���ϐ��̏�����

		// �\���̂̏�����
		memset(&pcm, 0, sizeof(WAVEFORMATEX));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// wav�t�@�C���̓ǂݍ���(WAVEFORMATEX�\����)
		pcm = wavFile->LoadWavFile(path, &svt->svtBase.wavFmt);

		// �o�b�t�@�̐ݒ�
		buffer.pAudioData	= (BYTE *)svt->svtBase.wavFmt.data.waveData;
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes	= svt->svtBase.wavFmt.data.waveSize;
		buffer.LoopCount	= svt->svtBase.loopCnt;
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
		channelVolume = (float *)malloc(svt->svtBase.wavFmt.fmt.fmtChannel * sizeof(float));
		for (int i = 0; i < svt->svtBase.wavFmt.fmt.fmtChannel; i++)
		{
			channelVolume[i] = SAI_DEFAULT_VOLUME;
		}
	}

	return sourceVoice;
}

// �Đ�
HRESULT SAI_AUDIO2_INTERFACE::PlayVoice(SAI_VOICE_TOOL *svt)
{
	// �{�C�X�ƍĐ���Ԃ̃`�F�b�N
	if ((svt->svtBase.sourceVoice != NULL) && (!svt->svtBase.isPlay))
	{
		// �Đ�
		svt->svtBase.sourceVoice->Start();

		return S_OK;
	}

	return S_FALSE;
}

// ��~
HRESULT SAI_AUDIO2_INTERFACE::StopVoice(SAI_VOICE_TOOL *svt)
{
	// �{�C�X�ƍĐ���Ԃ̃`�F�b�N
	if ((svt->svtBase.sourceVoice != NULL) && (svt->svtBase.isPlay))
	{
		// ��~
		svt->svtBase.sourceVoice->Stop();

		return S_OK;
	}

	return S_FALSE;
}

// �ꎞ��~
HRESULT SAI_AUDIO2_INTERFACE::PauseVoice(SAI_VOICE_TOOL *svt)
{
	// �{�C�X�ƍĐ���Ԃ̃`�F�b�N
	if ((svt->svtBase.sourceVoice != NULL) && (svt->svtBase.isPlay))
	{
		// �ꎞ��~
		svt->svtBase.sourceVoice->Stop(XAUDIO2_PLAY_TAILS);

		return S_OK;
	}

	return S_FALSE;
}

// (frequency ratio)�s�[�`
HRESULT SAI_AUDIO2_INTERFACE::SetPitch(SAI_VOICE_TOOL *svt)
{
	// �{�C�X�ƃp�����[�^�̃`�F�b�N
	if ((svt->svtBase.sourceVoice != NULL) && 
		(svt->svtSecondary.freqPich > XAUDIO2_MIN_FREQ_RATIO) &&
		(svt->svtSecondary.freqPich < XAUDIO2_MAX_FREQ_RATIO))
	{
		// �s�[�`��ݒ�
		return svt->svtBase.sourceVoice->SetFrequencyRatio(svt->svtSecondary.freqPich);
	}
	return S_FALSE;
}
float SAI_AUDIO2_INTERFACE::GetPitch(SAI_VOICE_TOOL *svt)
{
	float pitch = 0;

	// �{�C�X�̃`�F�b�N
	if (svt->svtBase.sourceVoice != NULL)
	{
		// �s�[�`���擾
		svt->svtBase.sourceVoice->GetFrequencyRatio(&pitch);

		return pitch;
	}

	return pitch;
}