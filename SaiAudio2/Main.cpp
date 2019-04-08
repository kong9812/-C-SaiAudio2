#include "Main.h"

// �O���[�o���ϐ�
SAI_AUDIO2_INTERFACE	*saiAudio2;		// SaiAudio2�̃C���^�[�t�F�[�X
SAI_VOICE_TOOL			testSoundSVT;	// �e�X�g�T�E���h��SVT

// �v���g�^�C�v�錾
void Init(void);		// ������
void Uninit(void);		// �I������

// ���C���֐�
int main(void)
{
	// ������
	Init();

	// �Đ�
	saiAudio2->PlayVoice(&testSoundSVT);

	// �L�[����������i��
	getchar();

	// �I������
	Uninit();
}

// ������
void Init(void)
{
	// �C���^�[�t�F�[�X�쐬
	saiAudio2 = new SAI_AUDIO2_INTERFACE;

	// �������̌���
	if (saiAudio2->InitCheck())
	{
		// �ǂݍ���
		testSoundSVT.sourceVoice = saiAudio2->LoadXAudio2Voice(SOUND_PATH, &testSoundSVT);
	}
}

// �I������
void Uninit(void)
{
	// �C���^�[�t�F�[�X���
	delete saiAudio2;
}