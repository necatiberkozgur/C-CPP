#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** createmap(FILE*, int, int);


void find_treasure(FILE*, int**, int**, int, int, int, int, int);


int main(int argc, char* argv[]) {
    FILE* mapfile;                                  /*Treasure map which will be given by user*/
    FILE* keyfile;                                  /*Key map which will be given by user*/
    FILE* out;                                      /*Output files to print results*/
    int** map;                                      /*Treasure Map in type of Pointer-to-Pointer which represents 2D Array*/
    int** key_matrix;                               /*Key Matrix in type of Pointer-to-Pointer which represents 2D Array*/
    int rows, cols, size_key;                       /*Numerical values to represent matrices and to use later*/
    char* ptr;                                      /*Char pointer to use in strtol() fx to convert command line args to int*/
    char* tok;                                      /*Char pointer to use in strtok() fx to extract ROWSxCOLS from args[1]*/

    tok = strtok(argv[1], "x");
    rows = (int) strtol(tok, &ptr, 10);
    tok = strtok(NULL, "x");
    cols = (int) strtol(tok, &ptr, 10);             /*Rows and Columns assigned*/

    size_key = (int) strtol(argv[2], &ptr, 10);     /*Key Matrice Rows and Columns assigned*/

    mapfile = fopen(argv[3], "r");
    keyfile = fopen(argv[4], "r");
    out = fopen(argv[5], "w");                      /*I/O Files opened*/

    map = createmap(mapfile, rows, cols);                               /*Creating a treasure map*/
    key_matrix = createmap(keyfile, size_key, size_key);
    find_treasure(out, map, key_matrix, rows, cols, size_key, 0, 0);

    fclose(mapfile);
    fclose(keyfile);
    fclose(out);
    free(key_matrix);
    free(map);
    return 0;
}


int** createmap(FILE* fl, int row, int col){
    int** arr;
    char* line;
    char* ptr;
    int row_count = 0,
        col_count = 0;
    line = malloc((2 * col) * sizeof(char));
    arr = malloc(row * sizeof(int*));
    for(row_count; row_count<row; row_count++){
        *(arr+row_count) = malloc(col * sizeof(int));
    }
    col_count = 1;
    row_count = 1;
    while(fscanf(fl, "%s", line) != EOF) {
        *(*(arr+(row_count-1))+(col_count-1)) = (int) strtol(line, &ptr, 10);
        col_count++;

        if(col_count % (col+1) == 0){
            row_count++;
            col_count = 1;
        }

    }
    free(line);
    return arr;
}


void find_treasure(FILE* write,int** map, int** key, int row_map, int col_map, int size_key,
                   int x_start, int y_start){
    int x_temp, y_temp, direction=0, key_x = 0, key_y = 0;
    x_temp = x_start;
    y_temp = y_start;
    for(y_temp, key_y; y_temp < y_start + size_key; y_temp++, key_y++){
        for(x_temp, key_x = 0; x_temp < x_start + size_key; x_temp++, key_x++){

            direction = direction + map[y_temp][x_temp]*key[key_y][key_x];

        }
        x_temp = x_start;
    }
    fprintf(write, "%d;%d ; %d\n",  y_start+(size_key/2), x_start+(size_key/2), direction);


    if(direction<0){                        /* In case of modulo operation with negative numbers*/
        while(direction<0){
            direction = direction + 5;
        }
    }


    direction = direction%5;

    if(direction==0){
        return;
    }
    if(direction == 1){
        if(y_start>0){
            find_treasure(write, map, key, row_map, col_map, size_key, x_start, y_start-size_key);
        }
        else{
            find_treasure(write, map, key, row_map, col_map, size_key, x_start, y_start+size_key);
        }
    }
    if(direction == 2){
        if(y_start + size_key <row_map){
            find_treasure(write, map, key, row_map, col_map, size_key, x_start, y_start+size_key);
        }
        else{
            find_treasure(write, map, key, row_map, col_map, size_key, x_start, y_start-size_key);
        }
    }
    if(direction == 3){
        if(x_start + size_key <col_map){
            find_treasure(write, map, key, row_map, col_map, size_key, x_start+size_key, y_start);
        }
        else{
            find_treasure(write, map, key, row_map, col_map, size_key, x_start-size_key, y_start);
        }
    }

    if(direction == 4){
        if(x_start>0){
            find_treasure(write, map, key, row_map, col_map, size_key, x_start-size_key, y_start);
        }
        else{
            find_treasure(write, map, key, row_map, col_map, size_key, x_start+size_key, y_start);
        }
    }
}