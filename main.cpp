#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "wav_read.h"

int main() {
    // 批量测试读写wav音频
    char infiles[100] = ""; // 输入文件夹，包含“i.wav”格式命名的wav音频
    char outfilename[100] = ""; // 写入wav的文件夹

    for (int i = 0; i < 27; i++) {
        char temp[100];
        sprintf(temp, "%s%d.wav", infiles,i);
        printf("%s\n", temp);

        struct WAVData* audio_data = audioread(temp); // 读wav
        if (audio_data == NULL) {
            perror("文件打开失败!");
            return 1;
        }
        strcpy(temp, "");
        sprintf(temp, "%s%d.wav", outfilename, i);
        audiowrite(temp, audio_data); // 写wav
    }

    
    return 0;
}
