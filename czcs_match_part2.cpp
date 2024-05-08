#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// �w�q CSV Ū���ﶵ
struct CSVReadingOptions {
    char delimiter;
    int hasHeaderRow;
};
// �w�q CSV �g�J�ﶵ
struct CSVWritingOptions {
    char delimiter;
};
// �w�q CSV �ƾڵ��c
struct CSVData {
    char ***data;
    size_t rows;
    size_t cols;
};

// ����n��
struct CSVData ACF_readCSV(struct CSVReadingOptions options, int L, int section_index);
struct CSVData sequence_readCSV(struct CSVReadingOptions options, int L, int section_index);
struct CSVData pair_readCSV(struct CSVReadingOptions options, int L, int flag);
struct CSVWritingOptions options_write = {','};
struct CSVReadingOptions options_read = {','};
void sequence_appendCSV(struct CSVWritingOptions options, struct CSVData sequence_i_data, int m0, int L, size_t* zmax_m0, int i,int flag);
void processAndSavePairs(int L, int total_section,int flag,size_t num_zmax,size_t* zmax_m0);
void writeToPairFile(int L, int flag, char*** buffer, size_t bufferSize);
int level(int n);
size_t* createArray(size_t size);
void cross_xcorr_2(int c[], int d[], size_t ACF[],int L);
int cross_SZCP_z(size_t* CCF_1,size_t* CCF_2,int L);
int findMinimum_2(int a, int b);
int findMinimum_3(int a, int b, int c);
int findMinimum_4(int a, int b, int c,int d);
void processPairs(struct CSVData pair_data, int L,int target);
void printArray(const char *name, int array[], size_t size);

// �w�q CSV �����|��
#define ACF_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/ACF_log/section_index_%d.csv"
#define sequence_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/sequence_log/section_index_%d.csv"
#define pair_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/pair/Z_%d.csv"
//#define pair_BASE_PATH "/Users/ray/Downloads/L%d/Z_%d.csv"

// �D���
int main(int argc, const char *argv[]) {
    
    int L = 38;
    
    
    //�榸�j�M�@��Z
    int Z =13;
    // �I�s sequence_readCSV ���Ū�����w���|�� CSV �ɮ�
    struct CSVData pair_data = pair_readCSV(options_read, L, Z);
    
//    // ��ܸ�Ƶ���
    printf("pair_data�`�@�� %zu �����\n", pair_data.rows);
    processPairs(pair_data,L,0);
    return 0;
}


//�p��椬��SZCP��Z
int cross_SZCP_z(size_t* CCF_1,size_t* CCF_2,int L){
    int z=0;
    if(CCF_1[0]+CCF_2[0]==0){
        for(size_t i=1;i<=L/2;i++){
            if(CCF_1[i]+CCF_2[i]==0){
                if(CCF_1[L-i]+CCF_2[L-i]==0){
                    z++;
                }
                else{
                    break;
                }
            }
            else{
                break;
            }
        }
    }
    return z;
}



void processPairs(struct CSVData pair_data, int L,int target) {
    int a[L], b[L], c[L], d[L], e[L], f[L], g[L], h[L];
    size_t CCF_1[L], CCF_2[L], CCF_3[L], CCF_4[L], CCF_5[L], CCF_6[L];
    // �t��L�{
    for (size_t loop_1 = 0; loop_1 + 1 < pair_data.rows-6; loop_1+=2) {
        if(loop_1%10==0){
            printf("loop_1 :%zu\n",loop_1);
        }
        for (size_t j = 0; j < pair_data.cols; j++) {
            a[j] = atoi(pair_data.data[loop_1][j]);
        }
        for (size_t j = 0; j < pair_data.cols; j++) {
            b[j] = atoi(pair_data.data[loop_1 + 1][j]);
        }
        
        for (size_t loop_2 = 2; loop_1 + loop_2 + 1 < pair_data.rows-4 ; loop_2+=2) {
            for (size_t j = 0; j < pair_data.cols; j++) {
                c[j] = atoi(pair_data.data[loop_1 + loop_2][j]);
            }
            for (size_t j = 0; j < pair_data.cols; j++) {
                d[j] = atoi(pair_data.data[loop_1 + loop_2 + 1][j]);
            }
            //���p�@
            int situation=1;
            cross_xcorr_2(a, c, CCF_1, L);
            cross_xcorr_2(b, d, CCF_2, L);
            //���p�G
            if(cross_SZCP_z(CCF_1, CCF_2, L) == 0){
                situation=2;
                cross_xcorr_2(a, d, CCF_1, L);
                cross_xcorr_2(b, c, CCF_2, L);
            }
            if (cross_SZCP_z(CCF_1, CCF_2, L) > 0) {
//                printf("situation:%d\n",situation);
                int flag = cross_SZCP_z(CCF_1, CCF_2, L);
                for (size_t loop_3 = 2; loop_1 + loop_2 + loop_3 + 1 < pair_data.rows-2; loop_3+=2) {
                    for (size_t j = 0; j < pair_data.cols; j++) {
                        e[j] = atoi(pair_data.data[loop_1 + loop_2 + loop_3 ][j]);
                    }
                    for (size_t j = 0; j < pair_data.cols; j++) {
                        f[j] = atoi(pair_data.data[loop_1 + loop_2 + loop_3 + 1][j]);
                    }
                    if(situation==1||situation==3||situation==4){
                        //���p�T
                        situation=3;
                        cross_xcorr_2(a, e, CCF_1, L);
                        cross_xcorr_2(b, f, CCF_2, L);
                        cross_xcorr_2(c, e, CCF_3, L);
                        cross_xcorr_2(d, f, CCF_4, L);
                        //���p�|
                        if(cross_SZCP_z(CCF_1, CCF_2, L) == 0 || cross_SZCP_z(CCF_3, CCF_4, L) == 0){
                            situation=4;
                            cross_xcorr_2(a, f, CCF_1, L);
                            cross_xcorr_2(b, e, CCF_2, L);
                            cross_xcorr_2(c, f, CCF_3, L);
                            cross_xcorr_2(d, e, CCF_4, L);
                        }
                    }
                    if(situation==2||situation==5||situation==6){
                        situation=5;
                        cross_xcorr_2(a, e, CCF_1, L);
                        cross_xcorr_2(b, f, CCF_2, L);
                        cross_xcorr_2(d, e, CCF_3, L);
                        cross_xcorr_2(c, f, CCF_4, L);
                        if(cross_SZCP_z(CCF_1, CCF_2, L) == 0 || cross_SZCP_z(CCF_3, CCF_4, L) == 0){
                            situation=6;
                            cross_xcorr_2(a, f, CCF_1, L);
                            cross_xcorr_2(b, e, CCF_2, L);
                            cross_xcorr_2(d, f, CCF_3, L);
                            cross_xcorr_2(c, e, CCF_4, L);
                        }
                    }
                    if (cross_SZCP_z(CCF_1, CCF_2, L) > 0 && cross_SZCP_z(CCF_3, CCF_4, L) > 0) {
//                        printf("situation:%d\n",situation);
                        flag = findMinimum_3(cross_SZCP_z(CCF_1, CCF_2, L), cross_SZCP_z(CCF_3, CCF_4, L), flag);
                        for (size_t loop_4 = 2; loop_1 + loop_2 + loop_3 + loop_4 + 1 < pair_data.rows ; loop_4+=2) {
                            for (size_t j = 0; j < pair_data.cols; j++) {
                                g[j] = atoi(pair_data.data[loop_1 + loop_2 + loop_3 + loop_4 ][j]);
                            }
                            for (size_t j = 0; j < pair_data.cols; j++) {
                                h[j] = atoi(pair_data.data[loop_1 + loop_2 + loop_3 + loop_4 + 1][j]);
                            }
                            if(situation==3||situation==7||situation==8){
                                situation=7;
                                cross_xcorr_2(a, g, CCF_1, L);
                                cross_xcorr_2(b, h, CCF_2, L);
                                cross_xcorr_2(c, g, CCF_3, L);
                                cross_xcorr_2(d, h, CCF_4, L);
                                cross_xcorr_2(e, g, CCF_5, L);
                                cross_xcorr_2(f, h, CCF_6, L);
                                if(cross_SZCP_z(CCF_1, CCF_2, L) == 0 || cross_SZCP_z(CCF_3, CCF_4, L) == 0 || cross_SZCP_z(CCF_5, CCF_6, L) == 0){
                                    situation=8;
                                    cross_xcorr_2(a, h, CCF_1, L);
                                    cross_xcorr_2(b, g, CCF_2, L);
                                    cross_xcorr_2(c, h, CCF_3, L);
                                    cross_xcorr_2(d, g, CCF_4, L);
                                    cross_xcorr_2(e, h, CCF_5, L);
                                    cross_xcorr_2(f, g, CCF_6, L);
                                }
                            }
                            if(situation==4||situation==9||situation==10){
                                situation=9;
                                cross_xcorr_2(a, g, CCF_1, L);
                                cross_xcorr_2(b, h, CCF_2, L);
                                cross_xcorr_2(c, g, CCF_3, L);
                                cross_xcorr_2(d, h, CCF_4, L);
                                cross_xcorr_2(f, g, CCF_5, L);
                                cross_xcorr_2(e, h, CCF_6, L);
                                if(cross_SZCP_z(CCF_1, CCF_2, L) == 0 || cross_SZCP_z(CCF_3, CCF_4, L) == 0 || cross_SZCP_z(CCF_5, CCF_6, L) == 0){
                                    situation=10;
                                    cross_xcorr_2(a, h, CCF_1, L);
                                    cross_xcorr_2(b, g, CCF_2, L);
                                    cross_xcorr_2(c, h, CCF_3, L);
                                    cross_xcorr_2(d, g, CCF_4, L);
                                    cross_xcorr_2(f, h, CCF_5, L);
                                    cross_xcorr_2(e, g, CCF_6, L);
                                }
                            }
                            if(situation==5||situation==11||situation==12){
                                situation=11;
                                cross_xcorr_2(a, g, CCF_1, L);
                                cross_xcorr_2(b, h, CCF_2, L);
                                cross_xcorr_2(d, g, CCF_3, L);
                                cross_xcorr_2(c, h, CCF_4, L);
                                cross_xcorr_2(e, g, CCF_5, L);
                                cross_xcorr_2(f, h, CCF_6, L);
                                if(cross_SZCP_z(CCF_1, CCF_2, L) == 0 || cross_SZCP_z(CCF_3, CCF_4, L) == 0 || cross_SZCP_z(CCF_5, CCF_6, L) == 0){
                                    situation=12;
                                    cross_xcorr_2(a, h, CCF_1, L);
                                    cross_xcorr_2(b, g, CCF_2, L);
                                    cross_xcorr_2(d, h, CCF_3, L);
                                    cross_xcorr_2(c, g, CCF_4, L);
                                    cross_xcorr_2(e, h, CCF_5, L);
                                    cross_xcorr_2(f, g, CCF_6, L);
                                }
                            }
                            if(situation==6||situation==13||situation==14){
                                situation=13;
                                cross_xcorr_2(a, g, CCF_1, L);
                                cross_xcorr_2(b, h, CCF_2, L);
                                cross_xcorr_2(d, g, CCF_3, L);
                                cross_xcorr_2(c, h, CCF_4, L);
                                cross_xcorr_2(f, g, CCF_5, L);
                                cross_xcorr_2(e, h, CCF_6, L);
                                if(cross_SZCP_z(CCF_1, CCF_2, L) == 0 || cross_SZCP_z(CCF_3, CCF_4, L) == 0 || cross_SZCP_z(CCF_5, CCF_6, L) == 0){
                                    situation=14;
                                    cross_xcorr_2(a, h, CCF_1, L);
                                    cross_xcorr_2(b, g, CCF_2, L);
                                    cross_xcorr_2(d, h, CCF_3, L);
                                    cross_xcorr_2(c, g, CCF_4, L);
                                    cross_xcorr_2(f, h, CCF_5, L);
                                    cross_xcorr_2(e, g, CCF_6, L);
                                }
                            }
                            if (cross_SZCP_z(CCF_1, CCF_2, L) > 0 && cross_SZCP_z(CCF_3, CCF_4, L) > 0 && cross_SZCP_z(CCF_5, CCF_6, L) > 0) {
                                flag = findMinimum_4(cross_SZCP_z(CCF_1, CCF_2, L), cross_SZCP_z(CCF_3, CCF_4, L), cross_SZCP_z(CCF_5, CCF_6, L), flag);
                                if(flag>target){
                                    printf("L=%d , zone=%d\n", L, flag);
                                    printf("%zu,%zu,%zu,%zu\n",loop_1,loop_1+loop_2,loop_1+loop_2+loop_3,loop_1+loop_2+loop_3+loop_4);
                                    if(situation==7){
                                        printArray("a", a, L);
                                        printArray("b", b, L);
                                        printArray("c", c, L);
                                        printArray("d", d, L);
                                        printArray("e", e, L);
                                        printArray("f", f, L);
                                        printArray("g", g, L);
                                        printArray("h", h, L);
                                    }
                                    if(situation==8){
                                        printArray("a", a, L);
                                        printArray("b", b, L);
                                        printArray("c", c, L);
                                        printArray("d", d, L);
                                        printArray("e", e, L);
                                        printArray("f", f, L);
                                        printArray("g", h, L);
                                        printArray("h", g, L);
                                    }
                                    if(situation==9){
                                        printArray("a", a, L);
                                        printArray("b", b, L);
                                        printArray("c", c, L);
                                        printArray("d", d, L);
                                        printArray("e", f, L);
                                        printArray("f", e, L);
                                        printArray("g", g, L);
                                        printArray("h", h, L);
                                    }
                                    if(situation==10){
                                        printArray("a", a, L);
                                        printArray("b", b, L);
                                        printArray("c", c, L);
                                        printArray("d", d, L);
                                        printArray("e", f, L);
                                        printArray("f", e, L);
                                        printArray("g", h, L);
                                        printArray("h", g, L);
                                    }
                                    if(situation==11){
                                        printArray("a", a, L);
                                        printArray("b", b, L);
                                        printArray("c", d, L);
                                        printArray("d", c, L);
                                        printArray("e", e, L);
                                        printArray("f", f, L);
                                        printArray("g", g, L);
                                        printArray("h", h, L);
                                    }
                                    if(situation==12){
                                        printArray("a", a, L);
                                        printArray("b", b, L);
                                        printArray("c", d, L);
                                        printArray("d", c, L);
                                        printArray("e", e, L);
                                        printArray("f", f, L);
                                        printArray("g", h, L);
                                        printArray("h", g, L);
                                    }
                                    if(situation==13){
                                        printArray("a", a, L);
                                        printArray("b", b, L);
                                        printArray("c", d, L);
                                        printArray("d", c, L);
                                        printArray("e", f, L);
                                        printArray("f", e, L);
                                        printArray("g", g, L);
                                        printArray("h", h, L);
                                    }
                                    if(situation==14){
                                        printArray("a", a, L);
                                        printArray("b", b, L);
                                        printArray("c", d, L);
                                        printArray("d", c, L);
                                        printArray("e", f, L);
                                        printArray("f", e, L);
                                        printArray("g", h, L);
                                        printArray("h", g, L);
                                    }
                                }
                            }
                            else{
                                continue;
                            }
                        }
                    }
                    else{
                        continue;
                    }
                }
            }
            else{
                continue;
            }
        }
    }
}

void printArray(const char *name, int array[], size_t size) {
    printf("%s = [ ", name);
    for (size_t i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("]\n");
}

//�p��cross correlation
void cross_xcorr_2(int c[], int d[], size_t ACF[],int L){
    for (int i=0;i<L;i++){
        int ones=0;
        for (int j=0;j<L-i;j++){
            int cor=!(c[j]^d[j+i]);
            ones=ones+2*cor-1;
        }
        ACF[i]=ones;
    }
}

//Ū��pair���
struct CSVData pair_readCSV(struct CSVReadingOptions options, int L, int flag) {
    char filePath[256];
    sprintf(filePath, pair_BASE_PATH, L, flag);
    struct CSVData result;
    result.data = NULL;
    result.rows = 0;
    result.cols = 0;

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening sequence file");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    // �w�����t�������O����
    size_t maxRows = 1000; // �ھڹ�ڱ��p�վ�
    result.data = (char ***)malloc(maxRows * sizeof(char *));
    if (result.data == NULL) {
        perror("Memory allocation failed for result.data");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        // ���Φ�
        char *token;
        char *rest = line;
        size_t colCount = 0;

        // �w�����t�������O����
        result.data[result.rows] = (char **)malloc(L * sizeof(char *));
        if (result.data[result.rows] == NULL) {
            perror("Memory allocation failed for result.data[result.rows]");
            exit(EXIT_FAILURE);
        }

        while ((token = strtok_r(rest, &options.delimiter, &rest))) {
            // �W�[�C
            result.data[result.rows][colCount] = strdup(token);
            colCount++;
        }

        // �p�G�O�Ĥ@��A�]�m�C��
        if (result.rows == 0) {
            result.cols = colCount;
        }

        // �W�[��
        result.rows++;

        // �ˬd�O�_�ݭn�X�i�O����
        if (result.rows >= maxRows) {
            maxRows *= 2;
            result.data = (char ***)realloc(result.data, maxRows * sizeof(char *));
            if (result.data == NULL) {
                perror("Memory reallocation failed for result.data");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(file);
    return result;
}














//����show�Xzone��sequence�٦���Q�n��sequence�s�icsv
void processAndSavePairs(int L, int total_section, int flag, size_t num_zmax, size_t* zmax_m0) {
    // �]�m�w�s�j�p�A�i�H�ھڹ�ڱ��p�վ�
    size_t bufferSize = 40000; // ���]�w�s�j�p�� 40000 �Ӥ���
    size_t bufferCount = 0;

    // �ʺA���t�w�s�Ŷ�
    char*** buffer = (char***)malloc(bufferSize * sizeof(char**));
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed for buffer.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t count = 0; count < num_zmax; count++) {
        int section_index = (int)zmax_m0[count * 3];
        int i = (int)zmax_m0[1 + count * 3];
        int j = (int)zmax_m0[2 + count * 3];
        char sequence_i_filePath[256];
        char sequence_j_filePath[256];
        sprintf(sequence_i_filePath, sequence_BASE_PATH, L, section_index);
        sprintf(sequence_j_filePath, sequence_BASE_PATH, L, total_section - 1 - section_index);

        if (access(sequence_i_filePath, F_OK) != -1 && access(sequence_j_filePath, F_OK) != -1) {
            // �ھ��x�s�� index �i�� show, �p�� Z, ���x�s�i�۹����� Z ���� CSV =========================
            struct CSVReadingOptions options_read = {','};
            struct CSVData sequence_i_data = sequence_readCSV(options_read, L, section_index);
            struct CSVData sequence_j_data = sequence_readCSV(options_read, L, total_section - 1 - section_index);

            if (flag > L/2 - 2) {
                printf("zone=%d\n", flag);
                for (size_t k = 0; k < sequence_i_data.cols; k++) {
                    printf("%s\t", sequence_i_data.data[i][k]);
                }
                for (size_t k = 0; k < sequence_j_data.cols; k++) {
                    printf("%s\t", sequence_j_data.data[j][k]);
                }
            }

            // �N�ƾڽƻs�� buffer ��
            buffer[bufferCount] = malloc(L * sizeof(char*));
            buffer[bufferCount + 1] = malloc(L * sizeof(char*));
            for (size_t k = 0; k < L; k++) {
                buffer[bufferCount][k] = strdup(sequence_i_data.data[i][k]);
                buffer[bufferCount + 1][k] = strdup(sequence_j_data.data[j][k]);
            }
            bufferCount += 2;

            // �p�G�w�s�F��@�w�j�p�A�Ϊ̹F��Y��Ĳ�o����A�N�g�J���
            if (bufferCount >= bufferSize) {
                writeToPairFile(L, flag, buffer, bufferCount);
                bufferCount = 0; // ���m�w�s�p��
            }

            // ���� sequence_i_data ���s
            for (size_t k = 0; k < sequence_i_data.rows; k++) {
                for (size_t l = 0; l < sequence_i_data.cols; l++) {
                    free(sequence_i_data.data[k][l]);
                }
                free(sequence_i_data.data[k]);
            }
            free(sequence_i_data.data);

            // ���� sequence_j_data ���s
            for (size_t k = 0; k < sequence_j_data.rows; k++) {
                for (size_t l = 0; l < sequence_j_data.cols; l++) {
                    free(sequence_j_data.data[k][l]);
                }
                free(sequence_j_data.data[k]);
            }
            free(sequence_j_data.data);
        }
    }

    // �g�J���A�B�z�Ѿl���w�s
    if (bufferCount > 0) {
        writeToPairFile(L, flag, buffer, bufferCount);
    }

    // ����w�s�Ŷ�
    for (size_t i = 0; i < bufferSize; i++) {
        free(buffer[i]);
    }
    free(buffer);
}


// �s�W���禡�A�N�w�s�����ƾڼg�J���
void writeToPairFile(int L, int flag, char*** buffer, size_t bufferSize) {
    struct CSVWritingOptions options_write = {','};

    // ���}pair���|
    char filePath[256];
    sprintf(filePath, pair_BASE_PATH, L, flag);
    FILE *file = fopen(filePath, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (size_t i = 0; i < bufferSize; i++) {
        for (size_t k = 0; k < L; k++) {
            fprintf(file, "%s", buffer[i][k]);
            if (k < L - 1) {
                fprintf(file, "%c", options_write.delimiter);
            }
        }
        fprintf(file, "");
    }

    // �������
    fclose(file);
}

//Ū���ϢѢԸ��
struct CSVData ACF_readCSV(struct CSVReadingOptions options, int L, int section_index) {
    char filePath[256];
    sprintf(filePath, ACF_BASE_PATH, L, section_index);
    struct CSVData result;
    result.data = NULL;
    result.rows = 0;
    result.cols = 0;

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening ACF file");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    // �w�����t�������O����
    size_t maxRows = 1000; // �ھڹ�ڱ��p�վ�
    result.data = (char ***)malloc(maxRows * sizeof(char *));
    if (result.data == NULL) {
        perror("Memory allocation failed for result.data");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        // ���Φ�
        char *token;
        char *rest = line;
        size_t colCount = 0;

        // �w�����t�������O����
        result.data[result.rows] = (char **)malloc(L * sizeof(char *));
        if (result.data[result.rows] == NULL) {
            perror("Memory allocation failed for result.data[result.rows]");
            exit(EXIT_FAILURE);
        }

        while ((token = strtok_r(rest, &options.delimiter, &rest))) {
            // �W�[�C
            result.data[result.rows][colCount] = strdup(token);
            colCount++;
        }

        // �p�G�O�Ĥ@��A�]�m�C��
        if (result.rows == 0) {
            result.cols = colCount;
        }

        // �W�[��
        result.rows++;

        // �ˬd�O�_�ݭn�X�i�O����
        if (result.rows >= maxRows) {
            maxRows *= 2;
            result.data = (char ***)realloc(result.data, maxRows * sizeof(char *));
            if (result.data == NULL) {
                perror("Memory reallocation failed for result.data");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(file);
    return result;
}

//Ū��sequence���
struct CSVData sequence_readCSV(struct CSVReadingOptions options, int L, int section_index) {
    char filePath[256];
    sprintf(filePath, sequence_BASE_PATH, L, section_index);
    struct CSVData result;
    result.data = NULL;
    result.rows = 0;
    result.cols = 0;

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening sequence file");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    // �w�����t�������O����
    size_t maxRows = 1000; // �ھڹ�ڱ��p�վ�
    result.data = (char ***)malloc(maxRows * sizeof(char *));
    if (result.data == NULL) {
        perror("Memory allocation failed for result.data");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        // ���Φ�
        char *token;
        char *rest = line;
        size_t colCount = 0;

        // �w�����t�������O����
        result.data[result.rows] = (char **)malloc(L * sizeof(char *));
        if (result.data[result.rows] == NULL) {
            perror("Memory allocation failed for result.data[result.rows]");
            exit(EXIT_FAILURE);
        }

        while ((token = strtok_r(rest, &options.delimiter, &rest))) {
            // �W�[�C
            result.data[result.rows][colCount] = strdup(token);
            colCount++;
        }

        // �p�G�O�Ĥ@��A�]�m�C��
        if (result.rows == 0) {
            result.cols = colCount;
        }

        // �W�[��
        result.rows++;

        // �ˬd�O�_�ݭn�X�i�O����
        if (result.rows >= maxRows) {
            maxRows *= 2;
            result.data = (char ***)realloc(result.data, maxRows * sizeof(char *));
            if (result.data == NULL) {
                perror("Memory reallocation failed for result.data");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(file);
    return result;
}

//�إߤ@���ǦC
size_t* createArray(size_t size){
    size_t* array = (size_t*)calloc(size , sizeof(int));
    if (array == NULL) {
        // �B�z���t���Ѫ����p
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return array;
}

// level ���
int level(int n){
    int sum=1;
    //    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        sum=sum*i;
    }
    return sum;
}

int findMinimum_2(int a, int b) {
    int min = a;
    if (b < min) {
        min = b;
    }
    return min;
}

int findMinimum_3(int a, int b, int c) {
    int min = a;
    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }
    return min;
}
int findMinimum_4(int a, int b, int c,int d) {
    int min = a;
    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }
    if (d < min) {
        min = d;
    }
    return min;
}

