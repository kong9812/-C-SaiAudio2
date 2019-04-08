#ifndef _SAI_DELAY_H_
#define _SAI_DELAY_H_

#include "libTool.h"
#include "LoadSoundClass.h"
#include "SaiAudio2.h"
#include <xapobase.h>
#pragma comment(lib,"xapobase.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DELAY_TIME_DEFAULT		(1.0f)
#define DELAY_VOLUME_DEFAULT	(0.5f)

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct	// SAI��I �x������(only for 2ch)
{
	WAV_FILE		wavFile;		// wav�t�@�C��
	float			delayTime;		// �x���̕b��
	float			volume;			// �c���̉���

}SAI_APO_DELAY;

//*****************************************************************************
// SAI_DELAY_APO (only for 2ch)
//*****************************************************************************
class __declspec(uuid("{80D4BED4-7605-4E4C-B29C-5479C375C599}"))SAI_DELAY_APO : public CXAPOParametersBase
{
public:
	SAI_DELAY_APO(SAI_VOICE_TOOL *svt);	// �������p
	~SAI_DELAY_APO();					// �I���p

	// LockForProcess�̃I�[�o�[���C�h
	STDMETHOD(LockForProcess)
		(UINT32 inputLockedParameterCount,
			const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pInputLockedParameters,
			UINT32 outputLockedParameterCount,
			const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pOutputLockedParameters)
		override;

	// Process�̃I�[�o�[���C�h
	STDMETHOD_(void, Process)
		(UINT32 inputProcessParameterCount,
			const XAPO_PROCESS_BUFFER_PARAMETERS * pInputProcessParameters,
			UINT32 outputProcessParameterCount,
			XAPO_PROCESS_BUFFER_PARAMETERS * pOutputProcessParameters,
			BOOL isEnabled)
		override;

	// OnSetParameters�̃I�[�o�[���C�g
	virtual void OnSetParameters
	(const void* pParameters, UINT32 ParameterByteSize);

private:
	static XAPO_REGISTRATION_PROPERTIES _regProps;

	// �t�H�[�}�b�g
	WAVEFORMATEX inFormat;
	WAVEFORMATEX outFormat;

	// �G�R�[�p
	float	*backupBuf;
	int		delaySample;
	int		readPos;
	int		writePos;
	bool	play;

	// WAV�t�@�C��
	WAV_FILE	wavFmt;

	// �p�����[�^
	SAI_APO_DELAY apoParameter[3];
};

#endif