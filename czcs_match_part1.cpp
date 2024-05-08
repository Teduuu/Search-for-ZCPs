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
struct CSVWritingOptions options_write = {','};
void sequence_appendCSV(struct CSVWritingOptions options, struct CSVData sequence_i_data, int m0, int L, size_t* zmax_m0, int i,int flag);
void processAndSavePairs(int L, int total_section,int flag,size_t num_zmax,size_t* zmax_m0);
void writeToPairFile(int L, int flag, char*** buffer, size_t bufferSize);
int level(int n);
size_t* createArray(size_t size);

// 定義 CSV 文件路徑的
#define ACF_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/ACF_log/section_index_%d.csv"
#define sequence_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/sequence_log/section_index_%d.csv"
#define pair_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/pair/Z_%d.csv"


// 主函數
int main(int argc, const char *argv[]) {
    int L = 38;
    const int match_layer = 10;
    int total_section = level(match_layer + 1);
    const int prespace=100000; //給一百萬的位置
    size_t* zmax_m0=createArray(prespace/10000);
    size_t* zmax_m1=createArray(prespace/1000);
    size_t* zmax_m2=createArray(prespace/100);
    size_t* zmax_m3=createArray(prespace/10);
    size_t* zmax_m4=createArray(prespace);
    int m0=-3;int m1=-3;int m2=-3;int m3=-3;int m4=-3;
    int zmax=L/2;
    for (int section_index = 0; section_index < total_section/2; section_index++) {
        printf("processing section:%d\n",section_index);
        // 構建 CSV 文件路徑
        char ACF_i_filePath[256];
        char ACF_j_filePath[256];
        sprintf(ACF_i_filePath, ACF_BASE_PATH, L, section_index);
        sprintf(ACF_j_filePath, ACF_BASE_PATH, L, total_section-1-section_index);
        
        
        // 檢查路徑是否存在
        if (access(ACF_i_filePath, F_OK) != -1 && access(ACF_j_filePath, F_OK) != -1) {
            // 讀取 ACF sequence CSV 數據
            struct CSVReadingOptions options_read = {','};
            struct CSVData ACF_i_data = ACF_readCSV(options_read, L, section_index);
            struct CSVData ACF_j_data = ACF_readCSV(options_read, L, total_section-1-section_index);
            //進行配對並儲存有配對的序列組所需要index（section_index,i,j）===================================
            for(size_t i=0;i< ACF_i_data.rows;i++){
                for(size_t j=0;j<ACF_j_data.rows;j++){
                    int flag=0;
                    for(int k=0;k<zmax;k++){
                        if (atoi(ACF_i_data.data[i][k]) + atoi(ACF_j_data.data[j][k]) == 0){
                            if(atoi(ACF_i_data.data[i][L-2-k]) + atoi(ACF_j_data.data[j][L-2-k]) == 0){
                                flag++;
                            }
                            else{
                                break;
                            }
                        }
                        else{
                            break;
                        }
                    }
                    if (flag>=zmax-4){
                        if(flag==zmax){
                            m0=m0+3;                        //各別會儲存相當於(m+1)/3組pair
                            zmax_m0[m0]=section_index;
                            zmax_m0[m0+1]=i;
                            zmax_m0[m0+2]=j;
                        }
                        else if(flag==zmax-1){
                            m1=m1+3;
                            zmax_m1[m1]=section_index;
                            zmax_m1[m1+1]=i;
                            zmax_m1[m1+2]=j;
                        }
                        else if(flag==zmax-2){
                            m2=m2+3;
                            zmax_m2[m2]=section_index;
                            zmax_m2[m2+1]=i;
                            zmax_m2[m2+2]=j;
                        }
                        else if(flag==zmax-3){
                            m3=m3+3;
                            zmax_m3[m3]=section_index;
                            zmax_m3[m3+1]=i;
                            zmax_m3[m3+2]=j;
                        }
                        else if(flag==zmax-4){
                            m4=m4+3;
                            zmax_m4[m4]=section_index;
                            zmax_m4[m4+1]=i;
                            zmax_m4[m4+2]=j;
                        }
                    }
                }
            }
            //======================================================================
            // 釋放 ACF 內存
            for (size_t i = 0; i < ACF_i_data.rows; i++) {
                for (size_t j = 0; j < ACF_i_data.cols; j++) {
                    free(ACF_i_data.data[i][j]);
                }
                free(ACF_i_data.data[i]);
            }
            free(ACF_i_data.data);
            
            for (size_t i = 0; i < ACF_j_data.rows; i++) {
                for (size_t j = 0; j < ACF_j_data.cols; j++) {
                    free(ACF_j_data.data[i][j]);
                }
                free(ACF_j_data.data[i]);
            }
            free(ACF_j_data.data);
        }
        else {
//            printf("Path does not exist for section %d\n", section_index);
            continue;
        }
    }
    
    // check 數據
    int num_zmax=(m0+1)/3;
    int num_zmax_m1=(m1+1)/3;
    int num_zmax_m2=(m2+1)/3;
    int num_zmax_m3=(m3+1)/3;
    int num_zmax_m4=(m4+1)/3;
    printf("zmax-4有%d組\n",num_zmax_m4);
    printf("zmax-3有%d組\n",num_zmax_m3);
    printf("zmax-2有%d組\n",num_zmax_m2);
    printf("zmax-1有%d組\n",num_zmax_m1);
    printf("zmax有%d組\n",num_zmax);
    
    
    processAndSavePairs(L,total_section,L/2,num_zmax, zmax_m0);
    processAndSavePairs(L,total_section,L/2-1,num_zmax_m1, zmax_m1);
    processAndSavePairs(L,total_section,L/2-2,num_zmax_m2, zmax_m2);
    processAndSavePairs(L,total_section,L/2-3,num_zmax_m3, zmax_m3);
    processAndSavePairs(L,total_section,L/2-4,num_zmax_m4, zmax_m4);

    return 0;
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
        // 釋放緩存空間
//        for (size_t i = 0; i < bufferSize; i++) {
//            free(buffer[i]);
//        }
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
    size_t maxRows = 10000; // 根據實際情況調整
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

