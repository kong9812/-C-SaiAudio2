#include "LoadSoundClass.h"

//=============================================================================
// サウンドの読み込み
//=============================================================================
bool LOAD_WAV::OpenSound(HWND hWnd, char fileName[], char soundPath[])
{
	OPENFILENAME ofn;
	char path[CHAR_MAX];
	WIN32_FIND_DATA fileData;

	// カレントディレクトリを取り出す
	GetCurrentDirectory(MAX_PATH, oldPath);

	path[0] = '\0';
	ofn.hwndOwner = hWnd;
	memset(&ofn, 0, sizeof(OPENFILENAME));  //構造体を0でクリア
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrInitialDir = oldPath;
	ofn.lpstrFilter = "sound file(*.wav)\0*.wav\0\0";
	ofn.lpstrFile = path;
	ofn.nMaxFile = sizeof(path);
	ofn.lpstrDefExt = "wav";

	if (GetOpenFileName(&ofn) != TRUE)
	{
		return FALSE;
	}

	strcpy(soundPath, path);

	// ファイル名を取り出す
	FindFirstFile(ofn.lpstrFile, &fileData);

	// ファイル名
	fileName = fileData.cFileName;

	// カレントディレクトリを設定する
	SetCurrentDirectory(oldPath);

	return TRUE;
}

//*****************************************************************************
// WAVファイルの読み込む(WAVEFORMATEX構造体)
//*****************************************************************************
WAVEFORMATEX LOAD_WAV::LoadWavFile(const char *path, WAV_FILE  *wavfile)
{
	WAVEFORMATEX	pcm;				// WAVEFORMATEX構造体
	FILE			*fp;				// ファイル構造体							
	int				chunkFlag = NULL;	// チャンクを探すループのフラグ

	{// ローカル変数の初期化
		memset(&pcm, 0, sizeof(WAVEFORMATEX));
		memset(&fp, 0, sizeof(FILE));
	}
	// ファイルの準備
	fopen_s(&fp, path, "rb");

	// RIFFの読み込み
	fread(&wavfile->riff, sizeof(RIFF_CHUNK), 1, fp);

	// FMTとDATAの読み込み
	while (chunkFlag != FLAG_CHUNK_END)
	{
		char	chunk[CHUNK_SIZE];
		long	size = 0;

		// チャンクとサイズの読み込み
		fread(&chunk, sizeof(chunk), 1, fp);
		fread(&size, sizeof(size), 1, fp);

		// fmtチャンク
		if (memcmp(chunk, CHUNK_FMT, CHUNK_SIZE) == CHUNK_SAME)
		{
			// チャンクとサイズを設定
			memcpy(wavfile->fmt.fmtChunk, chunk, CHUNK_SIZE);
			wavfile->fmt.fmtSize = size;

			// フォーマットIDから読み込み
			fread(&wavfile->fmt.fmtFormatTag,
				sizeof(FMT_CHUNK) - 8, 1, fp);

			// フラグ処理
			chunkFlag += FLAG_CHUNK_FMT;

			continue;
		}

		// dataチャンク
		if (memcmp(chunk, CHUNK_DATA, CHUNK_SIZE) == CHUNK_SAME)
		{
			// チャンクとサイズを設定
			memcpy(wavfile->data.dataChunk, CHUNK_DATA, CHUNK_SIZE);
			wavfile->data.waveSize = size;

			// メモリ確保
			wavfile->data.waveData = (short *)malloc(wavfile->data.waveSize);

			// 波形の読み込み
			fread(wavfile->data.waveData, wavfile->data.waveSize, 1, fp);

			// フラグ処理
			chunkFlag += FLAG_CHUNK_DATA;

			continue;
		}

		// 他のチャンク
		else
		{
			// size分をskip
			fseek(fp, size, SEEK_CUR);
		}
	}

	/* wav構造体の初期化 */
	pcm.cbSize = 0;
	pcm.nChannels = wavfile->fmt.fmtChannel;
	pcm.wBitsPerSample = wavfile->fmt.fmtBitPerSample;
	pcm.nSamplesPerSec = wavfile->fmt.fmtSampleRate;
	pcm.wFormatTag = WAVE_FORMAT_PCM;
	pcm.nBlockAlign = (wavfile->fmt.fmtChannel*wavfile->fmt.fmtBitPerSample) / 8;
	pcm.nAvgBytesPerSec = wavfile->fmt.fmtSampleRate*pcm.nBlockAlign;

	fclose(fp);

	return pcm;
}
