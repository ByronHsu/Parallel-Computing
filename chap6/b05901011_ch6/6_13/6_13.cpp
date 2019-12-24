#include <bits/stdc++.h>
using namespace std;

#include "mpi.h"

#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) (BLOCK_HIGH(id,p,n)-BLOCK_LOW(id,p,n)+1)
#define BLOCK_OWNER(j,p,n) (((p)*((j)+1)-1)/(n))

using namespace std;

const int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

void update(int id, int p, char **board, char **newBoard, int n, int m, int iter){
    int start = BLOCK_LOW(id, p, n);
    int end = BLOCK_HIGH(id, p, n);
    MPI_Status status;

    if(id) MPI_Send(board[start], m, MPI_CHAR, id - 1, 1, MPI_COMM_WORLD);
    if(id + 1 != p) MPI_Send(board[end], m, MPI_CHAR, id + 1, 2, MPI_COMM_WORLD);

    if(id) MPI_Recv(board[start - 1], m, MPI_CHAR, id - 1, 2, MPI_COMM_WORLD, &status);
    if(id + 1 != p) MPI_Recv(board[end + 1], m, MPI_CHAR, id + 1, 1, MPI_COMM_WORLD, &status);

    for(int i = start; i <= end; i++){
        for(int j = 0; j < m; j++){
            int live = 0;
            for(int d = 0; d < 8; d++){
                int x = i + dx[d];
                int y = j + dy[d];
                if(x >= 0 && x < n && y >= 0 && y < m){
                    if(board[x][y] == '1') live += 1;
                }
            }
            if(board[i][j] == '1'){
                if(live == 2 || live == 3) newBoard[i][j] = '1';
                else newBoard[i][j] = '0';
            }else{
                if(live == 3) newBoard[i][j] = '1';
                else newBoard[i][j] = '0';
            }
        }
    }

    for(int i = start; i <= end; i++)
        memcpy(board[i], newBoard[i], m);
}

void solve(int p, int id, char **argv){
    int n, m;
    int iter = atoi(argv[2]);
    int outputIter = atoi(argv[3]);
    char *boardMemory, *newBoardMemory;
    char **board, **newBoard;
    int *blockSize;
    int *displs;

    FILE *file;
    if(id == 0){
        file = fopen(argv[1], "r");
        assert(fscanf(file, "%d%d", &n, &m) == 2);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    boardMemory = (char *) malloc(n * m + 5);
    newBoardMemory = (char *) malloc(n * m + 5);
    board = (char **) malloc(n * sizeof(char *));
    newBoard = (char **) malloc(n * sizeof(char *));

    for(int i = 0; i < n; i++){
        board[i] = &boardMemory[i * m];
        newBoard[i] = &newBoardMemory[i * m];
    }
    
    if(id == 0){
        blockSize = (int *) malloc(p * 4);
        displs = (int *) malloc(p * 4);
        int offset = 0;
        for(int i = 0; i < p; i++){
            displs[i] = offset;
            blockSize[i] = BLOCK_SIZE(i, p, n) * m;
            offset += blockSize[i];
        }
        for(int i = 0; i < n; i++)
            assert(fscanf(file, "%s", newBoard[i]) == 1);
        fclose(file);
    }

    int start = BLOCK_LOW(id, p, n);
    int end = BLOCK_HIGH(id, p, n);
    MPI_Scatterv(newBoardMemory, blockSize, displs, MPI_CHAR, board[start], (end - start + 1) * m, MPI_CHAR, 0, MPI_COMM_WORLD);
    for(int T = 0; T < iter; T++){
        update(id, p, board, newBoard, n, m, iter);
        if((T + 1) % outputIter == 0) {
            MPI_Gatherv(board[start], (end - start + 1) * m, MPI_CHAR, newBoardMemory, blockSize, displs, MPI_CHAR, 0, MPI_COMM_WORLD);
            if(id == 0) {
                printf("iteration: %d\n", T + 1);
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++)
                        printf("%c", newBoard[i][j]);
                    printf("\n");
                }
            }
        }
    }
}

int main(int argc, char** argv){
    int id, p;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    double time = -MPI_Wtime();
    double maxTime;
    solve(p, id, argv);
    time += MPI_Wtime();
    MPI_Reduce(&time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if(id == 0){
        printf("Execution Time: %.9f seconds\n", maxTime);
    }
    MPI_Finalize();
    return 0; 
}