#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 定義 CSV 讀取選項
struct CSVReadingOptions {
    char delimiter;
    int hasHeaderRow;
};
// 定義 CSV 寫入選項
struct CSVWritingOptions {
    char delimiter;
};
// 定義 CSV 數據結構
struct CSVData {
    char ***data;
    size_t rows;
    size_t cols;
};

// 函數聲明
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

// 定義 CSV 文件路徑的
#define ACF_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/ACF_log/section_index_%d.csv"
#define sequence_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/sequence_log/section_index_%d.csv"
#define pair_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/pair/Z_%d.csv"
//#define pair_BASE_PATH "/Users/ray/Downloads/L%d/Z_%d.csv"

// 主函數
int main(int argc, const char *argv[]) {
    
    int L = 38;
    
    
    //單次搜尋一個Z
    int Z =13;
    // 呼叫 sequence_readCSV 函數讀取指定路徑的 CSV 檔案
    struct CSVData pair_data = pair_readCSV(options_read, L, Z);
    
//    // 顯示資料筆數
    printf("pair_data總共有 %zu 筆資料\n", pair_data.rows);
    processPairs(pair_data,L,0);
    return 0;
}


//計算交互的SZCP的Z
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
    // 配對過程
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
            //狀況一
            int situation=1;
            cross_xcorr_2(a, c, CCF_1, L);
            cross_xcorr_2(b, d, CCF_2, L);
            //狀況二
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
                        //狀況三
                        situation=3;
                        cross_xcorr_2(a, e, CCF_1, L);
                        cross_xcorr_2(b, f, CCF_2, L);
                        cross_xcorr_2(c, e, CCF_3, L);
                        cross_xcorr_2(d, f, CCF_4, L);
                        //狀況四
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

//計算cross correlation
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

//讀取pair資料
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
    // 預先分配足夠的記憶體
    size_t maxRows = 1000; // 根據實際情況調整
    result.data = (char ***)malloc(maxRows * sizeof(char *));
    if (result.data == NULL) {
        perror("Memory allocation failed for result.data");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        // 分割行
        char *token;
        char *rest = line;
        size_t colCount = 0;

        // 預先分配足夠的記憶體
        result.data[result.rows] = (char **)malloc(L * sizeof(char *));
        if (result.data[result.rows] == NULL) {
            perror("Memory allocation failed for result.data[result.rows]");
            exit(EXIT_FAILURE);
        }

        while ((token = strtok_r(rest, &options.delimiter, &rest))) {
            // 增加列
            result.data[result.rows][colCount] = strdup(token);
            colCount++;
        }

        // 如果是第一行，設置列數
        if (result.rows == 0) {
            result.cols = colCount;
        }

        // 增加行
        result.rows++;

        // 檢查是否需要擴展記憶體
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














//執行show出zone跟sequence還有把想要的sequence存進csv
void processAndSavePairs(int L, int total_section, int flag, size_t num_zmax, size_t* zmax_m0) {
    // 設置緩存大小，可以根據實際情況調整
    size_t bufferSize = 40000; // 假設緩存大小為 40000 個元素
    size_t bufferCount = 0;

    // 動態分配緩存空間
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
            // 根據儲存的 index 進行 show, 計算 Z, 並儲存進相對應的 Z 長度 CSV =========================
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

            // 將數據複製到 buffer 中
            buffer[bufferCount] = malloc(L * sizeof(char*));
            buffer[bufferCount + 1] = malloc(L * sizeof(char*));
            for (size_t k = 0; k < L; k++) {
                buffer[bufferCount][k] = strdup(sequence_i_data.data[i][k]);
                buffer[bufferCount + 1][k] = strdup(sequence_j_data.data[j][k]);
            }
            bufferCount += 2;

            // 如果緩存達到一定大小，或者達到某個觸發條件，就寫入文件
            if (bufferCount >= bufferSize) {
                writeToPairFile(L, flag, buffer, bufferCount);
                bufferCount = 0; // 重置緩存計數
            }

            // 釋放 sequence_i_data 內存
            for (size_t k = 0; k < sequence_i_data.rows; k++) {
                for (size_t l = 0; l < sequence_i_data.cols; l++) {
                    free(sequence_i_data.data[k][l]);
                }
                free(sequence_i_data.data[k]);
            }
            free(sequence_i_data.data);

            // 釋放 sequence_j_data 內存
            for (size_t k = 0; k < sequence_j_data.rows; k++) {
                for (size_t l = 0; l < sequence_j_data.cols; l++) {
                    free(sequence_j_data.data[k][l]);
                }
                free(sequence_j_data.data[k]);
            }
            free(sequence_j_data.data);
        }
    }

    // 寫入文件，處理剩餘的緩存
    if (bufferCount > 0) {
        writeToPairFile(L, flag, buffer, bufferCount);
    }

    // 釋放緩存空間
    for (size_t i = 0; i < bufferSize; i++) {
        free(buffer[i]);
    }
    free(buffer);
}


// 新增的函式，將緩存中的數據寫入文件
void writeToPairFile(int L, int flag, char*** buffer, size_t bufferSize) {
    struct CSVWritingOptions options_write = {','};

    // 打開pair路徑
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

    // 關閉文件
    fclose(file);
}

//讀取ＡＣＦ資料
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
    // 預先分配足夠的記憶體
    size_t maxRows = 1000; // 根據實際情況調整
    result.data = (char ***)malloc(maxRows * sizeof(char *));
    if (result.data == NULL) {
        perror("Memory allocation failed for result.data");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        // 分割行
        char *token;
        char *rest = line;
        size_t colCount = 0;

        // 預先分配足夠的記憶體
        result.data[result.rows] = (char **)malloc(L * sizeof(char *));
        if (result.data[result.rows] == NULL) {
            perror("Memory allocation failed for result.data[result.rows]");
            exit(EXIT_FAILURE);
        }

        while ((token = strtok_r(rest, &options.delimiter, &rest))) {
            // 增加列
            result.data[result.rows][colCount] = strdup(token);
            colCount++;
        }

        // 如果是第一行，設置列數
        if (result.rows == 0) {
            result.cols = colCount;
        }

        // 增加行
        result.rows++;

        // 檢查是否需要擴展記憶體
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

//讀取sequence資料
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
    // 預先分配足夠的記憶體
    size_t maxRows = 1000; // 根據實際情況調整
    result.data = (char ***)malloc(maxRows * sizeof(char *));
    if (result.data == NULL) {
        perror("Memory allocation failed for result.data");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        // 分割行
        char *token;
        char *rest = line;
        size_t colCount = 0;

        // 預先分配足夠的記憶體
        result.data[result.rows] = (char **)malloc(L * sizeof(char *));
        if (result.data[result.rows] == NULL) {
            perror("Memory allocation failed for result.data[result.rows]");
            exit(EXIT_FAILURE);
        }

        while ((token = strtok_r(rest, &options.delimiter, &rest))) {
            // 增加列
            result.data[result.rows][colCount] = strdup(token);
            colCount++;
        }

        // 如果是第一行，設置列數
        if (result.rows == 0) {
            result.cols = colCount;
        }

        // 增加行
        result.rows++;

        // 檢查是否需要擴展記憶體
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

//建立一維序列
size_t* createArray(size_t size){
    size_t* array = (size_t*)calloc(size , sizeof(int));
    if (array == NULL) {
        // 處理分配失敗的情況
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return array;
}

// level 函數
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

