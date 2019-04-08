#include "Main.h"

// �O���[�o���ϐ�
SAI_AUDIO2_INTERFACE	*saiAudio2;		// SaiAudio2�̃C���^�[�t�F�[�X
SAI_EFFECT_INTERFACE	*saiEffect;		// SaiAudio2�̃G�t�F�N�g�C���^�[�t�F�[�X
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

	// �s�[�`
	saiAudio2->SetPitch(&testSoundSVT);

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

	// �e�X�g�T�E���h��SVT�̏�����
	testSoundSVT.svtBase.loopCnt		= SAI_LOOP_CNT_INFINITE;

	// �������̌���
	if (saiAudio2->InitCheck())
	{
		// �ǂݍ���
		testSoundSVT.svtBase.sourceVoice = saiAudio2->LoadXAudio2Voice(SOUND_PATH, &testSoundSVT);
	}

	// �e�X�g�p(Get)
	testSoundSVT.svtSecondary.freqPich = saiAudio2->GetPitch(&testSoundSVT);
	// �e�X�g�p(Set)
	testSoundSVT.svtSecondary.freqPich = 1.5f;

	// �G�t�F�N�g�C���^�[�t�F�[�X�̍쐬
	saiEffect = new SAI_EFFECT_INTERFACE(&testSoundSVT);

}

// �I������
void Uninit(void)
{
	// �C���^�[�t�F�[�X���
	delete saiAudio2;
	delete saiEffect;
}