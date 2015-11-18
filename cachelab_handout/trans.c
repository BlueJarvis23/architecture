/* 
 *Team: A01073629 -- DMarshall
 *      A01107224 -- AWallentine
 *
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, l;
    int tmp=0;
    int tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    if( M == 32 && N == 32 ){ // Blocked Matrix Transpose -- 8 because b of cache is 5, 32 bytes = 8 ints
        for (i = 0; i < N; i+=8) {
            for (j = 0; j < M; j+=8) {
                for(k = i; k < i + 8; ++k){
                    if(i == j) tmp = A[k][k];
                    for(l = j; l < j + 8; ++l){
                        if(k != l) B[l][k] = A[k][l];
                    }
                    if(i == j) B[k][k] = tmp;
                }
            }
        }    
    }

    if( M == 64 && N == 64 ){
        for (i = 0; i < N; i+=8) {
            for (j = 0; j < M; j+=8) {
                for(k = 0; k < 8; ++k){
                    tmp = A[j+k][i];
                    tmp1 = A[j+k][i+1];
                    tmp2 = A[j+k][i+2];
                    tmp3 = A[j+k][i+3];
                    if(!k){
                        tmp4 = A[j+k][i+4];
                        tmp5 = A[j+k][i+5];
                        tmp6 = A[j+k][i+6];
                        tmp7 = A[j+k][i+7];
                    }
                    B[i][j+k] = tmp;
                    B[i][j+k+64] = tmp1;
                    B[i][j+k+128] = tmp2;
                    B[i][j+k+192] = tmp3;
                }
                for(k=7; k>0; --k){
                    tmp = A[j+k][i+4];
                    tmp1 = A[j+k][i+4+1];
                    tmp2 = A[j+k][i+4+2];
                    tmp3 = A[j+k][i+4+3];
                    B[i+4][j+k] = tmp;
                    B[i+4][j+k+64] = tmp1;
                    B[i+4][j+k+128] = tmp2;
                    B[i+4][j+k+192] = tmp3;
                }
                B[i+4][j] = tmp4;
                B[i+4][j+64] = tmp5;
                B[i+4][j+128] = tmp6;
                B[i+4][j+192] = tmp7;
            }
        }    
    }

    if( M == 61 && N==67 ){
        for (i = 0; i < N; i+=8) {
            for (j = 0; j < M; j+=8) {
                for(k = j; k < j + 8 && k < M; ++k){
                    for(l = i; l < i + 8 && l < N; ++l){
                        B[k][l] = A[l][k];
                    }
                }
            }
        }    
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 
char trans_32_desc[] = "Trans for 32 x 32";
void trans_32(int M, int N, int A[N][M], int B[M][N]){
    int i, j, k, l;
    int tmp=0;
    for (i = 0; i < N; i+=8) {
        for (j = 0; j < M; j+=8) {
            for(k = i; k < i + 8; ++k){
                if(i == j) tmp = A[k][k];
                for(l = j; l < j + 8; ++l){
                    if(k != l) B[l][k] = A[k][l];
                }
                if(i == j) B[k][k] = tmp;
            }
        }
    }    
}

char trans_64_desc[] = "Trans for 64 x 64";
void trans_64(int M, int N, int A[N][M], int B[M][N]){
    int i, j, k;
    int tmp, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    for (i = 0; i < N; i+=8) {
        for (j = 0; j < M; j+=8) {
            for(k = 0; k < 8; ++k){
                tmp = A[j+k][i];
                tmp1 = A[j+k][i+1];
                tmp2 = A[j+k][i+2];
                tmp3 = A[j+k][i+3];
                if(!k){
                    tmp4 = A[j+k][i+4];
                    tmp5 = A[j+k][i+5];
                    tmp6 = A[j+k][i+6];
                    tmp7 = A[j+k][i+7];
                }
                B[i][j+k] = tmp;
                B[i][j+k+64] = tmp1;
                B[i][j+k+128] = tmp2;
                B[i][j+k+192] = tmp3;
            }
            for(k=7; k>0; --k){
                tmp = A[j+k][i+4];
                tmp1 = A[j+k][i+4+1];
                tmp2 = A[j+k][i+4+2];
                tmp3 = A[j+k][i+4+3];
                B[i+4][j+k] = tmp;
                B[i+4][j+k+64] = tmp1;
                B[i+4][j+k+128] = tmp2;
                B[i+4][j+k+192] = tmp3;
            }
            B[i+4][j] = tmp4;
            B[i+4][j+64] = tmp5;
            B[i+4][j+128] = tmp6;
            B[i+4][j+192] = tmp7;
        }
    }    
}

char trans_61x67_desc[] = "Trans for 61 x 67";
void trans_61x67(int M, int N, int A[N][M], int B[M][N]){
    int i, j, k, l;
    for (i = 0; i < N; i+=8) {
        for (j = 0; j < M; j+=8) {
            for(k = j; k < j + 8 && k < M; ++k){
                for(l = i; l < i + 8 && l < N; ++l){
                    B[k][l] = A[l][k];
                }
            }
        }
    }    
}

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 
    /* Register any additional transpose functions */
    registerTransFunction(trans_32, trans_32_desc); 
    registerTransFunction(trans_64, trans_64_desc); 
    registerTransFunction(trans_61x67, trans_61x67_desc); 

    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

