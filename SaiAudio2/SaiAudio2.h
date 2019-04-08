#ifndef _SAIAUDIO2_H_
#define _SAIAUDIO2_H_

#include "libTool.h"
#include "LoadSoundClass.h"
#include <xaudio2.h>

// �}�N����`
#define SAI_INIT_SUCCEEDED					(3)
#define SAI_DEFAULT_VOLUME					(1.0f)
#define SAI_LOOP_CNT_NOPE					(XAUDIO2_NO_LOOP_REGION)
#define SAI_LOOP_CNT_LOOP					(XAUDIO2_LOOP_INFINITE)

//�\���̐錾
typedef struct
{
	IXAudio2SourceVoice *sourceVoice;
	WAV_FILE			*wav;
	int					loopCnt;
	bool				isPlay;
}SAI_VOICE_TOOL;

// �N���X�錾
class SAI_AUDIO2_INTERFACE
{
public:
	SAI_AUDIO2_INTERFACE();		// ������
	~SAI_AUDIO2_INTERFACE();	// �I������

	// ���[�h�T�E���h�֐�
	IXAudio2SourceVoice *LoadXAudio2Voice(char *path, SAI_VOICE_TOOL *voiceTool);

	// �������̌���(True:���� False:���s)
	bool InitCheck();

	// �Đ�
	HRESULT PlayVoice(SAI_VOICE_TOOL *voiceTool);

	// �Đ�
	HRESULT StopVoice(SAI_VOICE_TOOL *voiceTool);

	// �ꎞ��~
	HRESULT PauseVoice(SAI_VOICE_TOOL *voiceTool);

private:
	// �t�@�C���̓ǂݍ��݃N���X
	LOAD_WAV *wavFile;;

	IXAudio2MasteringVoice	*masteringVoice = NULL;		// �}�X�^�[�{�C�X
	IXAudio2				*xAudio2		= NULL;		// XAudio2�̃C���^�[�t�F�C�X
	
	int						initCheck		= 0;		// �������̌���
	float					*channelVolume	= NULL;		// �{�����[��
};

#endif