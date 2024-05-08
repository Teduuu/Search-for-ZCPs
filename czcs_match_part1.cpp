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
struct CSVWritingOptions options_write = {','};
void sequence_appendCSV(struct CSVWritingOptions options, struct CSVData sequence_i_data, int m0, int L, size_t* zmax_m0, int i,int flag);
void processAndSavePairs(int L, int total_section,int flag,size_t num_zmax,size_t* zmax_m0);
void writeToPairFile(int L, int flag, char*** buffer, size_t bufferSize);
int level(int n);
size_t* createArray(size_t size);

// �w�q CSV �����|��
#define ACF_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/ACF_log/section_index_%d.csv"
#define sequence_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/sequence_log/section_index_%d.csv"
#define pair_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/pair/Z_%d.csv"


// �D���
int main(int argc, const char *argv[]) {
    int L = 38;
    const int match_layer = 10;
    int total_section = level(match_layer + 1);
    const int prespace=100000; //���@�ʸU����m
    size_t* zmax_m0=createArray(prespace/10000);
    size_t* zmax_m1=createArray(prespace/1000);
    size_t* zmax_m2=createArray(prespace/100);
    size_t* zmax_m3=createArray(prespace/10);
    size_t* zmax_m4=createArray(prespace);
    int m0=-3;int m1=-3;int m2=-3;int m3=-3;int m4=-3;
    int zmax=L/2;
    for (int section_index = 0; section_index < total_section/2; section_index++) {
        printf("processing section:%d\n",section_index);
        // �c�� CSV �����|
        char ACF_i_filePath[256];
        char ACF_j_filePath[256];
        sprintf(ACF_i_filePath, ACF_BASE_PATH, L, section_index);
        sprintf(ACF_j_filePath, ACF_BASE_PATH, L, total_section-1-section_index);
        
        
        // �ˬd���|�O�_�s�b
        if (access(ACF_i_filePath, F_OK) != -1 && access(ACF_j_filePath, F_OK) != -1) {
            // Ū�� ACF sequence CSV �ƾ�
            struct CSVReadingOptions options_read = {','};
            struct CSVData ACF_i_data = ACF_readCSV(options_read, L, section_index);
            struct CSVData ACF_j_data = ACF_readCSV(options_read, L, total_section-1-section_index);
            //�i��t����x�s���t�諸�ǦC�թһݭnindex�]section_index,i,j�^===================================
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
                            m0=m0+3;                        //�U�O�|�x�s�۷��(m+1)/3��pair
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
            // ���� ACF ���s
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
    
    // check �ƾ�
    int num_zmax=(m0+1)/3;
    int num_zmax_m1=(m1+1)/3;
    int num_zmax_m2=(m2+1)/3;
    int num_zmax_m3=(m3+1)/3;
    int num_zmax_m4=(m4+1)/3;
    printf("zmax-4��%d��\n",num_zmax_m4);
    printf("zmax-3��%d��\n",num_zmax_m3);
    printf("zmax-2��%d��\n",num_zmax_m2);
    printf("zmax-1��%d��\n",num_zmax_m1);
    printf("zmax��%d��\n",num_zmax);
    
    
    processAndSavePairs(L,total_section,L/2,num_zmax, zmax_m0);
    processAndSavePairs(L,total_section,L/2-1,num_zmax_m1, zmax_m1);
    processAndSavePairs(L,total_section,L/2-2,num_zmax_m2, zmax_m2);
    processAndSavePairs(L,total_section,L/2-3,num_zmax_m3, zmax_m3);
    processAndSavePairs(L,total_section,L/2-4,num_zmax_m4, zmax_m4);

    return 0;
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
        // ����w�s�Ŷ�
//        for (size_t i = 0; i < bufferSize; i++) {
//            free(buffer[i]);
//        }
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
    size_t maxRows = 10000; // �ھڹ�ڱ��p�վ�
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

