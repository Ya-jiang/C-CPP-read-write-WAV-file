#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// ����WAV�ļ�ͷ���ṹ
struct WAVHeader {
    char chunkID[4];        // �ļ���ʶ��ͨ��Ϊ"RIFF"
    uint32_t chunkSize;     // �ļ���С
    char format[4];         // �ļ���ʽ����WAVE��
    char subchunk1ID[4];    // �ӿ��ʶ����fmt��
    uint32_t subchunk1Size; // �ӿ��С
    uint16_t audioFormat;   // ��Ƶ��ʽ��1ΪPCM��ʽ
    uint16_t numChannels;   // ��������1:��������2:˫����
    uint32_t sampleRate;    // ������
    uint32_t byteRate;      // ÿ����ֽ���
    uint16_t blockAlign;    // �����
    uint16_t bitsPerSample; // �������
    char subchunk2ID[4];    // �ӿ��ʶ����data��
    uint32_t subchunk2Size; // �ӿ��С
};

// ��Ƶ���ݽṹ
struct WAVData {
    struct WAVHeader header;  // ��Ƶͷ������
    uint8_t* sample;          // ��Ƶ����
    float* floatsample;

};

// ��ȡwav��ʽ�ļ�
struct WAVData* audioread(const char* filename);
// д��wav��ʽ�ļ�
void audiowrite(const char* filename, struct WAVData* audio_data);


struct WAVData* audioread(const char* filename) {
    // ���ļ�
    FILE* inputFile = fopen(filename, "rb");
    if (inputFile == NULL) {
        perror("�ļ�����ʧ��!");
        return NULL;
    }

    // ��ȡ�ļ�ͷ����Ϣ
    struct WAVHeader header;
    //fread(&header, sizeof(struct WAVHeader), 1, inputFile);
    fread(header.chunkID, 4, 1, inputFile); // ��RIFF��
    fread(&header.chunkSize, sizeof(uint32_t), 1, inputFile);
    fread(header.format, 4, 1, inputFile); // "WAVE"
    fread(header.subchunk1ID, 4, 1, inputFile);

    // �ж�wave�����ǲ���fmt�ֶΣ����ǵĻ�����
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
            fread(&ChunkLen, 4, 1, inputFile);//������ ����
            fseek(inputFile, ChunkLen, SEEK_CUR);//ֱ������
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

    // �ƶ���fmt������λ��
    if (header.subchunk1Size > 16) {
        fseek(inputFile, header.subchunk1Size - 16, SEEK_CUR);
    }
    header.subchunk1Size = 16;

    fread(header.subchunk2ID, 4, 1, inputFile);
    // �ж�wave�����ǲ���'data'�ֶΣ����ǵĻ�����
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
            fread(&ChunkLen, 4, 1, inputFile);//������ ����
            fseek(inputFile, ChunkLen, SEEK_CUR);//ֱ������
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

    // ��֤�ļ���ʽ
    if (strncmp(header.chunkID, "RIFF", 4) != 0 || strncmp(header.format, "WAVE", 4) != 0) {
        perror("��Ч�ļ�!");
        fclose(inputFile);
        return NULL;
    }

    // ���wav�ļ���Ϣ
    printf("��Ƶ��ʽ: %hu\n", header.audioFormat);
    printf("������: %hu\n", header.numChannels);
    printf("������: %hu\n", header.sampleRate);
    printf("�������: %hu\n", header.bitsPerSample);
    printf("�ӿ��С: %hu\n", header.subchunk2Size);
    //printf("�ļ���ʽ��%s\n", header.format);
    //printf("�ӿ��ʶ��%s\n", header.subchunk1ID);
    //printf("�ӿ��С��%hu\n", header.subchunk1Size);

    struct WAVData* audio_data = NULL;

    // ��ȡwav�ļ�����
    audio_data = (struct WAVData*)malloc(sizeof(struct WAVData));
    if (audio_data == NULL) {
        perror("�ڴ�����ʧ��!");
        return NULL;
    }
    audio_data->header = header;
    if (header.audioFormat == 3) { // IEEE float data
        audio_data->floatsample = (float*)malloc(header.subchunk2Size);
        if (audio_data->floatsample == NULL) {
            perror("�ڴ�����ʧ��!");
            free(audio_data);
            return NULL;
        }
        fread(audio_data->floatsample, header.subchunk2Size, 1, inputFile);
        audio_data->header.audioFormat = 3;
    }
    else { // int data
        audio_data->sample = (uint8_t*)malloc(header.subchunk2Size);
        if (audio_data->sample == NULL) {
            perror("�ڴ�����ʧ��!");
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
    // ������ļ�
    FILE* outputFile = fopen(filename, "wb");
    if (outputFile == NULL) {
        perror("�ļ���ʧ��!");
        return;
    }

    // д���ļ�
    fwrite(&audio_data->header, sizeof(struct WAVHeader), 1, outputFile);

    if (audio_data->header.audioFormat == 3) { // IEEE float data
        fwrite(audio_data->floatsample, audio_data->header.subchunk2Size, 1, outputFile);

    }
    else { // int data
        fwrite(audio_data->sample, audio_data->header.subchunk2Size, 1, outputFile);

    }

    // �ر��ļ�
    fclose(outputFile);
}