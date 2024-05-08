#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
#include<fstream>
#define FILENAME_LENGTH 256
// function�ŧi
int* initial_x_u1_p1(int L);
int* initial_x_u1_n1(int L);
int findLastNegativeOne(int result[], int length);
void showSequence(int array[], int length);
int Num_con_one(int result[], int length);
int Undercount(int result[], int length, int num_con_one);
long int C(int n, int r);
void JUMP(int change[], int length_change);
int countOnes(int sequence[], int length);
void Generate_c(int change[], int L, int c[]);
void xcorr_1(int c[], int L, int ACF[]);
int* xcorr_2(int c[], int L);
int level(int n);
int** basis_Data(int match_layer);
int* flipArray(int array[], int length);
int Section_index(int ACF[],int match_layer,int factorials[],int** basis_data,int L);
int* createArray(int size) ;
int* create_1_Array(int size);
int** createMatrix(int rows, int cols);
int*** create3DArray(int dim1, int dim2, int dim3);
void putDataIntoArrays(int*** sequence_log, int*** ACF_log,int* num_log , int section_index, int index, int L, int* c, int* ACF);
void change_0_1(int *p, int *q, int length);
int*** re_create3DArray(int*** array,int n, int dim1, int dim2, int dim3);
void writeArrayToFile(const char *filename, int **array, int rows, int cols);
void readArrayFromFile(const char *filename, int **array, int rows, int cols);

int main(int argc, const char * argv[]) {
    // 
//    clock_t start_time = clock();
    
    printf("�w���]�w���:\n");
    int L=42;                                       //�]�w��
    int match_layer=5;                              //�]�w�t��h��
    printf("L=%d,match_layer=%d\n",L,match_layer);
    int total_section=level(match_layer+1);        //�w���`�@total_section�Ӥ���(section:0~total_section-1)
    printf("total_section=%d\n",total_section);
    int prespace=200000;							//log���w���Ŷ�   *********************************�ثe����t�Z�Ӥj *********************************
	printf("prespace=%d\n",prespace); 
//    int top1=2*C(L-1,L/2)/total_section;
    int new_section=1;
    int* num_log=NULL;
    int* index_log=NULL;
    int*** sequence_log=NULL;
//    int*** ACF_log=NULL;
//    int prespace_log[total_section];
    num_log=createArray(total_section);							//�����Usection�����`��
	index_log=create_1_Array(total_section);                   //�����Usection�����ƶq �A-1��ܨS�Ψ� 
    sequence_log=create3DArray(total_section,1,L);       //�w�����t��m�ǦC�Ŷ�for�t��
//    ACF_log=create3DArray(total_section,1,L-1);          //�w�����t��mACF�Ŷ�for�t��
    //prespace_log==createArray(total_section);				//�O���֦h���O���� 
//    for (int i = 0; i < total_section; i++) {
//        prespace_log[i] = prespace;
//    }
//	showSequence(prespace_log,total_section);//============================================test

    int factorials[]={0};                          //�w���p��u���h�v�H�ƨϥ�
    for(int i=0;i<match_layer+1;i++){
        factorials[i]=level(i+1);
//        printf("%d",factorials[i]);
//        printf("\n");
    }
    int** basis_data=basis_Data(match_layer);       //�w���p��u�U�h�򩳡v�H�ƨϥ�
    for(int i=0;i<match_layer;i++){
        for(int j=0;j<i+2;j++){
//            printf("%d",basis_data[i][j]);
        }
//        printf("\n");
    }
    
    int length_change = L-1;
    int* change = NULL;
    for (int loop=0;loop<2;loop++){
        if(loop==0){
            change = initial_x_u1_p1(L);
        }
        if(loop==1){
            change = initial_x_u1_n1(L);
        }
        // �˵���l��change
//        printf("initail change:\n");
//        showSequence(change, L-1);
        
        long int top=C(L-1,countOnes(change,length_change))-1; //�w�]JUMP������W������
        printf("start with sequence num: %ld\n",top);
        long int Jumpping[3]={top/4, top/2, top*(3/4)};
        for (long int i=0;i<top;i++){
//            �d��JUMP�i�ױ�
            if (i==Jumpping[2]){
                printf("Jumpping 25%%\n");
            }
            if(i==Jumpping[0]){
                printf("\nJumpping 50%%\n");
            }
            if(i==Jumpping[1]){
                printf("\nJumpping 75%%\n");
            }
            int ACF[L-1],c[L];
            JUMP(change,length_change);
			Generate_c(change, L, c);
			xcorr_1(c, L, ACF);
            //================================================================������� 
            if (ACF[1]>2 || ACF[1]<-2) continue;
			else if (ACF[2]>3 || ACF[2]<-3) continue;
			else if (ACF[3]>4 || ACF[3]<-4) continue;
			else if (ACF[4]>4 || ACF[4]<-4) continue;
			else if (ACF[5]>5 || ACF[5]<-5) continue;
			else if (ACF[6]>5 || ACF[6]<-5) continue;
			else if (ACF[7]>5 || ACF[7]<-5) continue;
			else if (ACF[8]>5 || ACF[8]<-5) continue;
			else if (ACF[9]>7 || ACF[9]<-7) continue;
			else if (ACF[10]>7 || ACF[10]<-7) continue;
			else if (ACF[11]>7 || ACF[11]<-7) continue;
			
			else if (ACF[L-7]>7 || ACF[L-7]<-7) continue;
			else if (ACF[L-8]>7 || ACF[L-8]<-7) continue;
			else if (ACF[L-9]>7 || ACF[L-9]<-7) continue;
			else if (ACF[L-10]>7 || ACF[L-10]<-7) continue;
			else if (ACF[L-11]>7 || ACF[L-11]<-7) continue;
			else if (ACF[L-12]>7 || ACF[L-12]<-7) continue;

            //================================================================������� 
            
            int section_index=Section_index(ACF, match_layer, factorials,basis_data,L-1);
            if (section_index < 0 || section_index >= total_section) {
			    printf("Error: Invalid section_index %d\n", section_index);
			    continue;
			}
            int index = index_log[section_index];
            num_log[section_index] = num_log[section_index] + 1;     
            index_log[section_index] = index_log[section_index] + 1;
			if(index==-1){      //����s�϶� 
				new_section+=1;
				sequence_log=re_create3DArray(sequence_log,section_index,total_section,prespace,L);
				index_log[section_index] = index_log[section_index] + 1;
				index=0;
			}
			else if(index==prespace){  //�϶����F�A���scsv 
				printf("full%d ",section_index+1);
			    char filename[FILENAME_LENGTH];
			    snprintf(filename, FILENAME_LENGTH, "sequence_data%d.csv", section_index+1);
			    writeArrayToFile(filename, (int **)sequence_log[section_index], prespace, L);
				for(int j=0;j<prespace;j++){
					for(int k=0;k<L;k++){
						sequence_log[section_index][j][k] = 0;
					}
				}
				index_log[section_index]=1;
				index=0;
			}
			for(int k=0;k<L;k++){
				sequence_log[section_index][index][k] = c[k];
			}

			//sequence_log[section_index][index] = c;

        } 
        printf("finish part_%d\n",loop);
	}
	//=================================================================test
//    for (int i = 0; i < total_section; i++) {
//    	for (int j = 0; j < num_log[i]; j++) {
//        	for (int k = 0; k < L-1; k++) {
//            	printf("%d ", sequence_log[i][j][k]);
//        	}
//    		printf("\n");
//    	}
//    printf("\n");
//    }
	for (int i = 0; i < total_section; i++){
		printf("%d ", num_log[i]);
	}
	printf("\n");
	//==============================================================test
	printf("useful section: %d\n ",new_section);
	//==============================================�����s�Jcsv ==================================== 
	char filename[FILENAME_LENGTH];
	for (int n = 0; n < total_section; n++){
		if(index_log[n]==-1) continue;
		printf("%d ", n);
	    snprintf(filename, FILENAME_LENGTH, "sequence_data%d.csv", n+1);
	    writeArrayToFile(filename, (int **)sequence_log[n], index_log[n], L);		
	}
	printf("\n");
		// Free the allocated memory
//    for (int i = 0; i < total_section; i++) {
//        for (int j = 0; j < index_log[i]; j++) {
//            free(sequence_log[i][j]);
//        }
//        free(sequence_log[i]);
//    }
//    free(sequence_log);
//    free(change);
	//=================================================================================================�t�� 
	// ��L�ܼƫŧi
    int z = 8, zmax=L/2;
    int temp_max_p[L], temp_max_q[L];
    int** pair_sequence= createMatrix(3000, L);
    int index_pair=0;
    for (int n = 0; n < total_section / 2; n++) {
    	if(num_log[n]>0 && num_log[total_section - n - 1]>0){
    		printf("section: %d\n", n);
			printf("sequences: %d\n", num_log[n]);	
		}
		else continue; 
		char filename1[256],filename2[256];
	    snprintf(filename1, sizeof(filename1), "sequence_data%d.csv", n+1);
	    snprintf(filename2, sizeof(filename2), "sequence_data%d.csv", total_section - n );
		int** sequence_log1=createMatrix(num_log[n], L);
		int** sequence_log2=createMatrix(num_log[total_section - n - 1], L);
    	readArrayFromFile(filename1, sequence_log1, num_log[n], L);
    	readArrayFromFile(filename2, sequence_log2, num_log[total_section - n - 1], L);
    	int** ACF_log1=createMatrix(num_log[n], L-1);
		int** ACF_log2=createMatrix(num_log[total_section - n - 1], L-1);
    	for(int i = 0; i < num_log[n]; i++){
    		ACF_log1[i] = xcorr_2(sequence_log1[i],L);
		}
    	for(int i = 0; i < num_log[total_section - n - 1]; i++){
    		ACF_log2[i] = xcorr_2(sequence_log2[i],L);
		}
        for (int i = 0; i < num_log[n]; i++) {
            for (int j = 0; j < num_log[total_section - n - 1]; j++) {
                int flag = 0;
//                int j_loop = 1;
//                for (int k = 0; k < match_layer; k++) {
//                    if (ACF_log[n][i][k] + ACF_log[total_section - n - 1][j][k] == 0) {
//						flag++;
//                    }
//                    else break;
//                }

                // if�W����pass�ܤ֦�match_layer��flag
                for (int k = 0; k < zmax; k++) {
                    if (ACF_log1[i][k] + ACF_log2[j][k] == 0){
                    	if(ACF_log1[i][L - k-2] + ACF_log2[j][L - k-2] == 0){
                    		flag++;
						}
						else{
							break;
						}
                    } 
					else {
                        break;
                    }
                }
                // ��� 
                if (flag == z) {
                    int *p = sequence_log1[i];
                    int *q = sequence_log2[j];
                    change_0_1(p, q, L);
                    // ��X���G
                    showSequence(p,L);
                    showSequence(q,L);
                    printf("z: %d\n", flag);
					pair_sequence[index_pair]=p;
					pair_sequence[index_pair+1]=q;
					index_pair+=2;   
                } 
				else if (flag > z) {
                    int *p = sequence_log1[i];
                    int *q = sequence_log2[j];
                    index_pair=0;
                    change_0_1(p, q, L);
                    // ��X���G
                    showSequence(p,L);
                    showSequence(q,L);
                    printf("z: %d\n", flag);
					pair_sequence[index_pair]=p;
					pair_sequence[index_pair+1]=q;
					index_pair+=2;
                    z=flag;
                }
            }  
        }      
    }          
	printf("pairs: %d\n",index_pair);
	printf("Finish\n");


	//ofstream file("sequence_data.csv");
	ofstream outFile; 
	outFile.open("Z_13.csv", ios::out); 
	for (int i = 0; i < index_pair; i++)
	{	
		for (int j = 0; j < L; j++)
		{
			outFile << pair_sequence[i][j]<<',' ;
		}
		outFile << endl;
	}
	outFile.close(); 




//    free(prespace_log);
    free(pair_sequence);

    return 0;
}

// ========================  function  ========================================
// ��l��x�ǦC for����L�o��u1=1
int* initial_x_u1_p1(int L) {
    int length=L-1;
    int* array = (int*)malloc(length * sizeof(int));  // �ϥΰʺA���t�O����
    if (array == NULL) {
        // �B�z���s���t���Ѫ����p
        fprintf(stderr, "Memory allocation failed.\n");
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
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= L/2-1; i++) {
        array[i] = 0;
    }
    for(int i = 0; i <= L/2-1; i++){
        array[i + L/2] = 1;
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
long int C(int n, int r) {
    if (n < r || r < 0)
        return 0;  

    if (n == r || r == 0)
        return 1;  

    return C(n - 1, r) + C(n - 1, r - 1); 
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

// ���ͧǦC�A�Q��!(xor)
void Generate_c(int change[], int L, int c[]) {
    c[0] = 0;
    for (int i = 0; i < L; i++) {
        c[i + 1] = !(change[i] ^ c[i]);
    }
}

// �p��ACF���u�O�du1~u(L-1)
void xcorr_1(int c[], int L, int ACF[]) {
    int u = L - 1;
    for (int i = 0; i < u; i++) {
        int ones = 0;
        for (int j = 0; j < L - 1 - i; j++) {
            int cor = !(c[j] ^ c[j + i + 1]);
            ones = ones + 2 * cor - 1;
        }
        ACF[i] = ones;
    }
}
//�p��ACF���u�O�du1~u(L-1)
int* xcorr_2(int c[], int L){
    int* ACF = (int*)malloc(L * sizeof(int));
    if (ACF == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    int u=L-1;
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
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    for (int layer = 0; layer < match_layer; layer++) {
        int sequenceLength = layer + 2;

        sequences[layer] = (int*)malloc(sequenceLength * sizeof(int));
        if (sequences[layer] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");

            for (int i = 0; i < layer; i++) {
                free(sequences[i]);
            }
            free(sequences);

            exit(EXIT_FAILURE);
        }
        // ��l��
        for (int i = 0; i < sequenceLength; i++) {
            sequences[layer][i] = -(layer+1)+(i)*2;
        }
    }
    return sequences;
}


//����ǦC
int* flipArray(int array[], int length) {
    int* flippedArray = (int*)malloc(length * sizeof(int));

    if (flippedArray == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // ?��l??�������f�Ǧs?��s??��
    for (int i = 0; i < length; i++) {
        flippedArray[i] = array[length - 1 - i];
    }
    

    return flippedArray;
}

//�p��ACF�����������index
int Section_index(int ACF[],int match_layer,int factorials[],int** basis_data,int length_ACF){
    int section_index=0;
    int basis_index[match_layer];
    int* flippedACF = flipArray(ACF,length_ACF);
    for (int i=0;i<match_layer;i++){
        for(int j=0;j<length_ACF;j++){
            if(flippedACF[i]==basis_data[i][j]){
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
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    return array;
}

//�إߤ@��-1�ǦC
int* create_1_Array(int size) {
    int* array = (int*)malloc(size * sizeof(int));
    if (array == NULL) {

        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        array[i] = -1;
    }
    return array;
}


// �إߤG���}�C
int** createMatrix(int rows, int cols) {
    int** matrix = (int**)calloc(rows, sizeof(int*));

    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for rows.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)calloc(cols, sizeof(int));

        if (matrix[i] == NULL) {
            // �p�G���t���ѡA���񤧫e���t���O����
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }

            free(matrix);

            fprintf(stderr, "Memory allocation failed for cols.\n");
            exit(EXIT_FAILURE);
        }
    }

    return matrix;
}


// �إߤT���}�C
int*** create3DArray(int dim1, int dim2, int dim3) {
    int*** array = (int***)calloc(dim1, sizeof(int**));

    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed for dim1.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dim1; i++) {
        array[i] = (int**)calloc(dim2, sizeof(int*));

        if (array[i] == NULL) {
            // �p�G���t���ѡA���񤧫e���t���O����
            for (int j = 0; j < i; j++) {
                free(array[j]);
            }

            free(array);

            fprintf(stderr, "Memory allocation failed for dim2.\n");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < dim2; j++) {
            array[i][j] = (int*)calloc(dim3, sizeof(int));

            if (array[i][j] == NULL) {
                // �p�G���t���ѡA���񤧫e���t���O����
                for (int k = 0; k < j; k++) {
                    free(array[i][k]);
                }

                free(array[i]);
                free(array);

                fprintf(stderr, "Memory allocation failed for dim3.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    return array;
}


// �ק�T���}�C
int*** re_create3DArray(int*** array,int n, int dim1, int dim2, int dim3) {

    	array[n] = (int**)malloc(dim2* sizeof(int*));
        for (int j = 0; j < dim2; j++) {  
            array[n][j] = (int*)malloc(dim3* sizeof(int));
            // ��l�Ʒs�W�������� 0�A�u�w��s�W�������i���l��
//            for (int k = 0; k < dim3; k++) {  // �ץ�����
//                array[n][j][k] = 0;
//            }
        } 

    return array;
}


// �ഫ0��-1
void change_0_1(int *p, int *q, int length) {     
    for (int i = 0; i < length; i++) {
        if (p[i] == 0) {
            p[i] = -1;
        }
        if (q[i] == 0) {
            q[i] = -1;
        }
    }
}
//csv�g�J 
void writeArrayToFile(const char *filename, int **array, int rows, int cols) {
    FILE *file = fopen(filename, "a");
    
    if (file != NULL) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                fprintf(file, "%d", array[i][j]);
                if (j < cols - 1) {
                    fprintf(file, ",");
                }
            }

            fprintf(file, "\n");
        }

        fclose(file);
    } else {
        printf("Error opening file: %s\n", filename);
    }
}
//csvŪ�� 
void readArrayFromFile(const char *filename, int **array, int rows, int cols) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (fscanf(file, "%d", &array[i][j]) != 1) {
                    printf("Error reading file: %s\n", filename);
                    fclose(file);
                    return;
                }

                if (j < cols - 1) {
                    fgetc(file); 
                }
            }

            fgetc(file);
        }

        fclose(file);
    } else {
        printf("Error opening file: %s\n", filename);
    }
}
