#ifndef _SAIAUDIO2_H_
#define _SAIAUDIO2_H_

#include "libTool.h"
#include "LoadSoundClass.h"
#include <xaudio2.h>
#pragma comment (lib, "xaudio2.lib")

// �}�N����`
#define SAI_INIT_SUCCEEDED					(3)
#define SAI_DEFAULT_VOLUME					(1.0f)
#define SAI_LOOP_CNT_NOPE					(XAUDIO2_NO_LOOP_REGION)
#define SAI_LOOP_CNT_INFINITE				(XAUDIO2_LOOP_INFINITE)

//�\���̐錾
typedef struct			// SaiVoiceTool (SVT)
{
	IXAudio2SourceVoice *sourceVoice;		// �ύX���Ȃ��ł�������! DONT CHANGE ANYTHING! 
	WAV_FILE			wavFmt;				// �ύX���Ȃ��ł�������! DONT CHANGE ANYTHING! 
	bool				isPlay;				// �ύX���Ȃ��ł�������! DONT CHANGE ANYTHING! 
	int					loopCnt;			// ���[�v�p�J�E���^�[
}SAI_SVT_BASE;

typedef struct			// SaiVoiceTool (SVT)
{
	float				freqPich;			// �s�[�`
	
}SAI_SVT_SECONDARY;

typedef struct			// SaiVoiceTool (SVT)
{
	SAI_SVT_BASE		svtBase;			// SVT�̃x�[�X����
	SAI_SVT_SECONDARY	svtSecondary;		// SVT�̊�{�p�����[�^

}SAI_VOICE_TOOL;

// �N���X�錾
class SAI_AUDIO2_INTERFACE
{
public:
	SAI_AUDIO2_INTERFACE(void);		// ������
	~SAI_AUDIO2_INTERFACE(void);	// �I������

	// ���[�h�T�E���h�֐�
	IXAudio2SourceVoice *LoadXAudio2Voice(const char *path, SAI_VOICE_TOOL *svt);

	// �������̌���(True:���� False:���s)
	bool InitCheck();

	// �}�X�^�[�{�C�X���擾
	IXAudio2MasteringVoice *GetMasterVoice(void);

	// �Đ�
	HRESULT PlayVoice(SAI_VOICE_TOOL *svt);

	// �Đ�
	HRESULT StopVoice(SAI_VOICE_TOOL *svt);

	// �ꎞ��~
	HRESULT PauseVoice(SAI_VOICE_TOOL *svt);

	// (frequency ratio)�s�[�`
	HRESULT SetPitch(SAI_VOICE_TOOL *svt);
	float	GetPitch(SAI_VOICE_TOOL *svt);

private:
	SAI_LOAD_WAV			*wavFile;					// wav�t�@�C���̓ǂݍ��ݗp�N���X

	IXAudio2MasteringVoice	*masteringVoice = NULL;		// �}�X�^�[�{�C�X
	IXAudio2				*xAudio2		= NULL;		// XAudio2�̃C���^�[�t�F�C�X
	
	int						initCheck		= 0;		// �������̌���
	float					*channelVolume	= NULL;		// �{�����[��
};

#endif