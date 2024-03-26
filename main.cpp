#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "wav_read.h"

int main() {
    // �������Զ�дwav��Ƶ
    char infiles[100] = ""; // �����ļ��У�������i.wav����ʽ������wav��Ƶ
    char outfilename[100] = ""; // д��wav���ļ���

    for (int i = 0; i < 27; i++) {
        char temp[100];
        sprintf(temp, "%s%d.wav", infiles,i);
        printf("%s\n", temp);

        struct WAVData* audio_data = audioread(temp); // ��wav
        if (audio_data == NULL) {
            perror("�ļ���ʧ��!");
            return 1;
        }
        strcpy(temp, "");
        sprintf(temp, "%s%d.wav", outfilename, i);
        audiowrite(temp, audio_data); // дwav
    }

    
    return 0;
}
