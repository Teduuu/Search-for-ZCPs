#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define sequence_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/sequence_log/section_index_%d.csv"
#define ACF_BASE_PATH "/Users/ray/Desktop/final_project_L/L_%d/ACF_log/section_index_%d.csv"
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

// function�ŧi
int* initial_x_u1_p1(int L);
int* initial_x_u1_n1(int L);
int* initial_x_u1_p3(int L);
int* initial_x_u1_n3(int L);
int findLastNegativeOne(int result[], int length);
void showSequence(int array[], int length);
int Num_con_one(int result[], int length);
int Undercount(int result[], int length, int num_con_one);
int C(int n, int r);
void JUMP(int change[], int length_change);
int countOnes(int sequence[], int length);
int* Generate_c(int change[], int L);
void con_Generate_c(int change[],int c[] ,int L);
int* xcorr_2(int c[], int L);
void con_xcorr_2(int c[],int ACF[], int L);
int level(int n);
int** basis_Data(int match_layer);
int Section_index(int ACF[],int match_layer,int factorials[],int** basis_data,int L);
int* createArray(int size);
int** createMatrix(int rows, int cols);
int*** create3DArray(int dim1, int dim2, int dim3);
//for write csv
void sequence_appendCSV(struct CSVWritingOptions options, int*** data, int numData, int* num_log,int L, int section_index);
void ACF_appendCSV(struct CSVWritingOptions options, int*** data, int numData,int* num_log, int L, int section_index);



int main(int argc, const char * argv[]) {
    struct CSVWritingOptions options_write = {','};
    printf("�w���]�w���:\n");
    int L=42;                                            //�]�w��
    const int match_layer=10;                             //�]�w�t��h��
    printf("L=%d,match_layer=%d\n",L,match_layer);
    int total_section=level(match_layer+1);              //�w���`�@total_section�Ӥ���(section:0~total_section-1)
    printf("total_section=%d\n",total_section);
    //    int top1=2*C(L-1,L/2)/total_section;
    int* factorials=createArray(match_layer+1);    //�w���p��u���h�v�H�ƨϥ�
    for(int i=0;i<match_layer+1;i++){
        factorials[i]=level(i+1);
        //        printf("%d",factorials[i]);
        //        printf("\n");
    }
    
    int** basis_data=basis_Data(match_layer);           //�w���p��u�U�h�򩳡v�H�ƨϥ�
    for(int i=0;i<match_layer;i++){
        for(int j=0;j<i+2;j++){
            //            printf("%d",basis_data[i][j]);
        }
        //        printf("\n");
    }
    
    int numData = 1;  // �]�m�榸�̤j�ƾڼƶq
    int*** cData=create3DArray(total_section,numData, L);
    int*** ACFData=create3DArray(total_section,numData, L-1);

    int* num_log=createArray(total_section);
    
    
    int length_change = L-1;
    int* change = NULL;
    for (int loop=0;loop<2;loop++){
        if(loop==0){
            change = initial_x_u1_p1(L);
        }
        if(loop==1){
            change = initial_x_u1_n1(L);
        }
        if(loop==2){
            change = initial_x_u1_p3(L);
        }
        if(loop==3){
            change = initial_x_u1_n3(L);
        }
        // �˵���l��change
        //        printf("initail change:\n");
        //        showSequence(change, L-1);
        JUMP(change,length_change);
        int* c=Generate_c(change,L);
        int* ACF=xcorr_2(c,L);
        
        int top=C(L-1,countOnes(change,length_change))-2; //�w�]JUMP������W������
        top=top/500;
        printf("part%d�ݭnJUMP%d��\n",loop,top);
        int Jumpping[3]={top/4, top/2, top*(3/4)};
        for (int i=0;i<top;i++){
            // �d��JUMP�i�ױ�
            if (i==Jumpping[2]){
                printf("Jumpping 25%%\n");
            }
            if(i==Jumpping[0]){
                printf("Jumpping 50%%\n");
            }
            if(i==Jumpping[1]){
                printf("Jumpping 75%%\n");
            }

            //
            JUMP(change,length_change);
//            showSequence(change, length_change);
            con_Generate_c(change,c,L);
            //����z�����================================================
            int c_loop=1;
            if(countOnes(c,L)>0.75*L || countOnes(c,L)<0.25*L){
                c_loop=0;
            }
            if(c_loop!=1){
                continue;
            }
            
//            showSequence(c, L);
            con_xcorr_2(c,ACF,L);
//            showSequence(ACF, L-1);
            
            
            //����z�����================================================
            int i_loop=1;
            for(int w=0;w<=L-10;w++){
                if(abs(ACF[w])>6){
                    i_loop=0;
                    break;
                }
            }
            if(i_loop!=1){
                continue;
            }
            
            //============================================================
            
            int section_index=Section_index(ACF, match_layer, factorials,basis_data,L-1);
            if (section_index >= 0 && section_index < total_section) {
                //��c��ACF�s�i�����w
                int index=num_log[section_index];
                for (int w=0;w<L;w++){
                    cData[section_index][index][w]=c[w];
                }
                for (int w=0;w<L-1;w++){
                    ACFData[section_index][index][w]=ACF[w];
                }
                num_log[section_index]=num_log[section_index]+1;
                if(index==numData-1){
                    sequence_appendCSV(options_write, cData, numData, num_log,L,section_index);
                    ACF_appendCSV(options_write, ACFData, numData, num_log, L, section_index);
                    num_log[section_index]=0;
                    printf("section %d is full\n",section_index);
                }
            }
            else {
                // �B�z section_index ���X�k�����p
                printf("Error: Invalid section_index %d\n", section_index);
            }
            // Directly put data into arrays
        }
        printf("finish part_%d\n",loop);
    }

    
    
//    for(int i=0;i<total_section;i++){
//        printf("%d",num_log[i]);
//    }
    
    //��S��icsv���ɩ�
    for(int i=0;i<total_section;i++){
        if(num_log[i]!=0){
//            printf("section %d �ɩ� %d �Ө�L_%d\n",i,num_log[i],L);
            sequence_appendCSV(options_write, cData, num_log[i],num_log ,L,i);
            ACF_appendCSV(options_write, ACFData, num_log[i],num_log ,L, i );
        }
    }
    
//    for(int i=0;i<total_section;i++){
//        printf("%d",num_log[i]);
//    }
//    
    
    //  Free the allocated memory
    for (int i = 0; i < match_layer; i++) {
        free(basis_data[i]);
    }
    free(basis_data);
    return 0;
}



// ========================  function  for csv  ========================================
void sequence_appendCSV(struct CSVWritingOptions options, int*** data, int numData,int* num_log, int L, int section_index) {
    char filePath[256];
    sprintf(filePath, sequence_BASE_PATH, L, section_index);
    FILE *file = fopen(filePath, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < numData; i++) {
        for (size_t j = 0; j < L; j++) {
            fprintf(file, "%d", data[section_index][i][j]);
            if (j < L-1) {
                fprintf(file, "%c", options.delimiter);
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void ACF_appendCSV(struct CSVWritingOptions options, int*** data, int numData,int* num_log, int L, int section_index) {
    char filePath[256];
    sprintf(filePath, ACF_BASE_PATH, L, section_index);
    FILE *file = fopen(filePath, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < numData; i++) {
        for (size_t j = 0; j < L-1; j++) {
            fprintf(file, "%d", data[section_index][i][j]);
            if (j < L-2) {
                fprintf(file, "%c", options.delimiter);
            }
        }
        fprintf(file, "\n");
    }
//    num_log[section_index]=0;
    fclose(file);
}



// ========================  function  for search  ========================================
// ��l��x�ǦC for����L�o��u1=1
int* initial_x_u1_p1(int L) {
    int length=L-1;
    int* array = (int*)malloc(length * sizeof(int));  // �ϥΰʺA���t�O����
    if (array == NULL) {
        // �B�z���s���t���Ѫ����p
        fprintf(stderr, "initial_x_u1_p1 Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < L/2-1; i++) {
        array[i] = 0;
    }
    for(int i = 0; i <= L/2-1; i++){
        array[i + L/2-1] = 1;
    }
    return array;
}

// ��l��x�ǦC for����L�o��u1=-1
int* initial_x_u1_n1(int L) {
    int length=L-1;
    int* array = (int*)malloc(length * sizeof(int));  // �ϥΰʺA���t�O����
    if (array == NULL) {
        // �B�z���s���t���Ѫ����p
        fprintf(stderr, "initial_x_u1_n1 Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < L/2; i++) {
        array[i] = 0;
    }
    for(int i = 0; i < L/2; i++){
        array[i + L/2] = 1;
    }
    return array;
}

// ��l��x�ǦC for����L�o��u1=3
int* initial_x_u1_p3(int L) {
    int length=L-1;
    int* array = (int*)malloc(length * sizeof(int));  // �ϥΰʺA���t�O����
    if (array == NULL) {
        // �B�z���s���t���Ѫ����p
        fprintf(stderr, "initial_x_u1_p3 Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < L/2-2; i++) {
        array[i] = 0;
    }
    for(int i = 0; i <= L/2+1; i++){
        array[i + L/2-2] = 1;
    }
    return array;
}

// ��l��x�ǦC for����L�o��u1=-3
int* initial_x_u1_n3(int L) {
    int length=L-1;
    int* array = (int*)malloc(length * sizeof(int));  // �ϥΰʺA���t�O����
    if (array == NULL) {
        // �B�z���s���t���Ѫ����p
        fprintf(stderr, "initial_x_u1_n3 Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < L/2+1; i++) {
        array[i] = 0;
    }
    for(int i = 0; i < L/2-1; i++){
        array[i + L/2+1] = 1;
    }
    return array;
}

// ��̥k��0����m
int findLastNegativeOne(int result[], int length) {
    int last_index = -1;  // ��l�Ƭ��@�ӵL�Ī���
    for (int i = length - 1; i >= 0; i--) {
        if (result[i] == 0) {
            last_index = i;
            break;
        }
    }
    return last_index;
}
// for��K�ˬd�ǦC ,if length�]�w�W�X�쥻�ǦC�������|���0
void showSequence(int array[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}


//�ˬd�̥k�䦳�s��X��0
int Num_con_one(int result[], int length) {
    int num_con_one = 0;
    for (int i = length - 1; i >= 0; i--) {
        if (result[i] == 0) {
            num_con_one++;
        }
        else{
            break;
        }
    }
    return num_con_one;
}

//�o��q�k��ƨӲ�(num_con_one+1)��0����m
int Undercount(int result[], int length,int num_con_one){
    int undercount=0;
    for (int i = length-num_con_one-1; i >= 0; i--) {
        if (result[i] == 0) {
            undercount=i;
            break;
        }
    }
    return undercount;
}

//�p��ƦC�զXnchoosek
int C(int n,int r){
    if(n<r||r<0)
        return -1;
    if(n==r||r==0)
        return 1;
    return C(n-1,r)+C(n-1,r-1);
}

// JUMP
void JUMP (int change[],int length_change){
    if (change[length_change-1]!=0){
        int last_neg_one_index=findLastNegativeOne(change,length_change+1);
        change[last_neg_one_index]=1;
        change[last_neg_one_index+1]=0;
    }
    else {
        if(change[length_change-3]==0 && change[length_change-2]==1 && change[length_change-1]==0){
            change[length_change-3]=1  ;
            change[length_change-2]=0 ;
            change[length_change-1]=0 ;
        }
        else{
            int num_con_one= Num_con_one(change, length_change);
            int undercount=Undercount(change, length_change, num_con_one);
            change[undercount]=1;
            for (int i=0;i<=num_con_one;i++){
                change[undercount+1+i]=0;
            }
            if (length_change-(num_con_one-1)>undercount+2+num_con_one){
                for (int i=0;i<num_con_one;i++){
                    change[length_change-1-i]=1;
                }
            }
            else{
                for (int i=undercount+2+num_con_one;i<length_change;i++){
                    change[i]=1;
                }
            }
        }
    }
}

//�p��ǦC�����h�֭�1
int countOnes(int sequence[], int length) {
    int onesCount = 0;
    for (int i = 0; i < length; i++) {
        if (sequence[i] == 1) {
            onesCount++;
        }
    }
    return onesCount;
}

//���ͧǦC�A�Q��!(xor)
int* Generate_c(int change[], int L){
    int* c = (int*)calloc(L ,sizeof(int));
    if (c == NULL) {
        fprintf(stderr, "Generate_c Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    c[0] = 0;
    for (int i = 0; i < L; i++) {
        c[i + 1] = !(change[i] ^ c[i]);
    }
    return c;
}
//���ͧǦC�A�Q��!(xor)
void con_Generate_c(int change[],int c[] ,int L){
    for (int i = 0; i < L; i++) {
        c[i + 1] = !(change[i] ^ c[i]);
    }
}

//�p��ACF���u�O�du1~u(L-1)
int* xcorr_2(int c[], int L){
    int u=L-1;
    int* ACF = (int*)malloc(u * sizeof(int));
    if (ACF == NULL) {
        fprintf(stderr, "xcorr_2 Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i=0;i<u;i++){
        int ones=0;
        for (int j=0;j<L-1-i;j++){
            int cor=!(c[j]^c[j+i+1]);
            ones=ones+2*cor-1;
        }
        ACF[i]=ones;
    }
    return ACF;
}

void con_xcorr_2(int c[],int ACF[], int L){
    int u=L-1;
    for (int i=0;i<u;i++){
        int ones=0;
        for (int j=0;j<L-1-i;j++){
            int cor=!(c[j]^c[j+i+1]);
            ones=ones+2*cor-1;
        }
        ACF[i]=ones;
    }
}

//�p�ⶥ�h(n)�I
int level(int n){
    int sum=1;
    //    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        sum=sum*i;
    }
    return sum;
}


//�إ�basis_data��
int** basis_Data(int match_layer) {
    int** sequences = (int**)malloc(match_layer * sizeof(int*));
    if (sequences == NULL) {
        fprintf(stderr, "basis_Data_1 Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    
    for (int layer = 0; layer < match_layer; layer++) {
        int sequenceLength = layer + 2;
        sequences[layer] = (int*)malloc(sequenceLength * sizeof(int));
        if (sequences[layer] == NULL) {
            fprintf(stderr, "basis_Data_2 Memory allocation failed.\n");
            
            for (int i = 0; i < layer; i++) {
                free(sequences[i]);
            }
            free(sequences);
            
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < sequenceLength; i++) {
            sequences[layer][i] = -(layer+1)+(i)*2;
        }
    }
    return sequences;
}

//�p��ACF�����������index
int Section_index(int ACF[],int match_layer,int factorials[],int** basis_data,int length_ACF){
    int section_index=0;
    int basis_index[match_layer];
    for (int i=0;i<match_layer;i++){
        for(int j=0;j<length_ACF;j++){
            if(ACF[length_ACF-1-i]==basis_data[i][j]){
                basis_index[i]=j;
                break;
            }
        }
    }
    for(int i=0;i<match_layer;i++){
        section_index=section_index+(basis_index[i]*factorials[match_layer])/factorials[i+1];
    }
    return section_index;
}

//�إߤ@���ǦC
int* createArray(int size) {
    int* array = (int*)calloc(size, sizeof(int));
    if (array == NULL) {
        // �B�z���t���Ѫ����p
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

//    // ��l�ƧǦC��
//    for (int i = 0; i < size; i++) {
//        array[i] = 0;
//    }
    return array;
}

// �إߤG���}�C
int** createMatrix(int rows, int cols) {
    int** matrix = (int**)calloc(rows , sizeof(int*));
    if (matrix == NULL) {
        // �B�z���t���Ѫ����p
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)calloc(cols ,sizeof(int));
    }
//    for (int i = 0; i < rows; i++) {
//        for(int j=0;j<cols;j++){
//            matrix[i][j]= 0;
//        }
//    }
    return matrix;
}

// �إߤT���}�C
int*** create3DArray(int dim1, int dim2, int dim3) {
    int*** array = (int***)calloc(dim1 , sizeof(int**));
    if (array == NULL) {
        // �B�z���t���Ѫ����p
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < dim1; i++) {
        array[i] = (int**)calloc(dim2 , sizeof(int*));
        for (int j = 0; j < dim2; j++) {
            array[i][j] = (int*)calloc(dim3 , sizeof(int));
        }
    }
    return array;
}

