#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// 定义WAV文件头部结构
struct WAVHeader {
    char chunkID[4];        // 文件标识，通常为"RIFF"
    uint32_t chunkSize;     // 文件大小
    char format[4];         // 文件格式，“WAVE”
    char subchunk1ID[4];    // 子块标识，“fmt”
    uint32_t subchunk1Size; // 子块大小
    uint16_t audioFormat;   // 音频格式，1为PCM格式
    uint16_t numChannels;   // 声道数，1:单声道，2:双声道
    uint32_t sampleRate;    // 采样率
    uint32_t byteRate;      // 每秒的字节数
    uint16_t blockAlign;    // 块对齐
    uint16_t bitsPerSample; // 采样深度
    char subchunk2ID[4];    // 子块标识，“data”
    uint32_t subchunk2Size; // 子块大小
};

// 音频数据结构
struct WAVData {
    struct WAVHeader header;  // 音频头部数据
    uint8_t* sample;          // 音频数据
    float* floatsample;

};

// 读取wav格式文件
struct WAVData* audioread(const char* filename);
// 写入wav格式文件
void audiowrite(const char* filename, struct WAVData* audio_data);


struct WAVData* audioread(const char* filename) {
    // 打开文件
    FILE* inputFile = fopen(filename, "rb");
    if (inputFile == NULL) {
        perror("文件代开失败!");
        return NULL;
    }

    // 读取文件头部信息
    struct WAVHeader header;
    //fread(&header, sizeof(struct WAVHeader), 1, inputFile);
    fread(header.chunkID, 4, 1, inputFile); // “RIFF”
    fread(&header.chunkSize, sizeof(uint32_t), 1, inputFile);
    fread(header.format, 4, 1, inputFile); // "WAVE"
    fread(header.subchunk1ID, 4, 1, inputFile);

    // 判断wave后面是不是fmt字段，不是的话跳过
    if (!((header.subchunk1ID[0] == 'f')
        && (header.subchunk1ID[1] == 'm')
        && (header.subchunk1ID[2] == 't')))
    {
        //printf("Chunck ID: %s detected\n", header.subchunk1ID);

        //printf("Non-Data Chunk detected\n");
        //fseek(inputFile, -4, SEEK_CUR);
        char c[5]="";
        int ChunkLen;
        do {
            fread(&ChunkLen, 4, 1, inputFile);//读出来 长度
            fseek(inputFile, ChunkLen, SEEK_CUR);//直接跳过
            fread(c, 4, 1, inputFile);
            /*fread(&c1, 1, 1, inputFile);
            fread(&c2, 1, 1, inputFile);
            fread(&c3, 1, 1, inputFile);
            fread(&c4, 1, 1, inputFile);*/

            //printf("Chunck ID: %s detected\n", c);
            //printf("ChunkLen: %d\n", ChunkLen);

        } while (!((c[0] == 'f') && (c[1] == 'm') && (c[2] == 't') && (c[3] == ' ')));
    }
    header.subchunk1ID[0] = 'f';
    header.subchunk1ID[1] = 'm';
    header.subchunk1ID[2] = 't';
    header.subchunk1ID[3] = ' ';

    fread(&header.subchunk1Size, 4, 1, inputFile);
    fread(&header.audioFormat, 2, 1, inputFile);
    fread(&header.numChannels, 2, 1, inputFile);
    fread(&header.sampleRate, 4, 1, inputFile);
    fread(&header.byteRate, 4, 1, inputFile);
    fread(&header.blockAlign, 2, 1, inputFile);
    fread(&header.bitsPerSample, 2, 1, inputFile);

    //printf("subchunk1size: %d\n", header.subchunk1Size);

    // 移动到fmt结束的位置
    if (header.subchunk1Size > 16) {
        fseek(inputFile, header.subchunk1Size - 16, SEEK_CUR);
    }
    header.subchunk1Size = 16;

    fread(header.subchunk2ID, 4, 1, inputFile);
    // 判断wave后面是不是'data'字段，不是的话跳过
    if (!((header.subchunk2ID[0] == 'd')
        && (header.subchunk2ID[1] == 'a')
        && (header.subchunk2ID[2] == 't')
        && (header.subchunk2ID[3] == 'a')))
    {
        //printf("Chunck ID: %s detected\n", header.subchunk2ID);

        //printf("Non-Data Chunk detected\n");
        //fseek(inputFile, -4, SEEK_CUR);
        char c[5]="";
        int ChunkLen;
        do {
            fread(&ChunkLen, 4, 1, inputFile);//读出来 长度
            fseek(inputFile, ChunkLen, SEEK_CUR);//直接跳过
            fread(c, 4, 1, inputFile);
            /*fread(&c1, 1, 1, inputFile);
            fread(&c2, 1, 1, inputFile);
            fread(&c3, 1, 1, inputFile);
            fread(&c4, 1, 1, inputFile);*/

            //printf("Chunck ID: %s detected\n", c);
            //printf("ChunkLen: %d\n", ChunkLen);

        } while (!((c[0] == 'd') && (c[1] == 'a') && (c[2] == 't') && (c[3] == 'a')));
    }
    header.subchunk2ID[0] = 'd';
    header.subchunk2ID[1] = 'a';
    header.subchunk2ID[2] = 't';
    header.subchunk2ID[3] = 'a';

    fread(&header.subchunk2Size, 4, 1, inputFile);

    // 验证文件格式
    if (strncmp(header.chunkID, "RIFF", 4) != 0 || strncmp(header.format, "WAVE", 4) != 0) {
        perror("无效文件!");
        fclose(inputFile);
        return NULL;
    }

    // 输出wav文件信息
    printf("音频格式: %hu\n", header.audioFormat);
    printf("声道数: %hu\n", header.numChannels);
    printf("采样率: %hu\n", header.sampleRate);
    printf("采样深度: %hu\n", header.bitsPerSample);
    printf("子块大小: %hu\n", header.subchunk2Size);
    //printf("文件格式：%s\n", header.format);
    //printf("子块标识：%s\n", header.subchunk1ID);
    //printf("子块大小：%hu\n", header.subchunk1Size);

    struct WAVData* audio_data = NULL;

    // 读取wav文件数据
    audio_data = (struct WAVData*)malloc(sizeof(struct WAVData));
    if (audio_data == NULL) {
        perror("内存申请失败!");
        return NULL;
    }
    audio_data->header = header;
    if (header.audioFormat == 3) { // IEEE float data
        audio_data->floatsample = (float*)malloc(header.subchunk2Size);
        if (audio_data->floatsample == NULL) {
            perror("内存申请失败!");
            free(audio_data);
            return NULL;
        }
        fread(audio_data->floatsample, header.subchunk2Size, 1, inputFile);
        audio_data->header.audioFormat = 3;
    }
    else { // int data
        audio_data->sample = (uint8_t*)malloc(header.subchunk2Size);
        if (audio_data->sample == NULL) {
            perror("内存申请失败!");
            free(audio_data);
            return NULL;
        }
        fread(audio_data->sample, header.subchunk2Size, 1, inputFile);
        audio_data->header.audioFormat = 1;
    }

    fclose(inputFile);
    return audio_data;
    
}

void audiowrite(const char* filename, struct WAVData* audio_data) {
    // 打开输出文件
    FILE* outputFile = fopen(filename, "wb");
    if (outputFile == NULL) {
        perror("文件打开失败!");
        return;
    }

    // 写入文件
    fwrite(&audio_data->header, sizeof(struct WAVHeader), 1, outputFile);

    if (audio_data->header.audioFormat == 3) { // IEEE float data
        fwrite(audio_data->floatsample, audio_data->header.subchunk2Size, 1, outputFile);

    }
    else { // int data
        fwrite(audio_data->sample, audio_data->header.subchunk2Size, 1, outputFile);

    }

    // 关闭文件
    fclose(outputFile);
}