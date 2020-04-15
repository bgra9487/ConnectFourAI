#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>

using namespace std;

typedef struct node{
    int value;
    short column;
} node;

void print_board(int8_t** board){ // debugging tool for me
    for(int8_t i=5;i>=0;i--){
        for(int8_t j=0;j<7;j++){
            printf("%c ", (board[i][j]==0) ? '.' : ((board[i][j]==1) ? 'r' : 'y'));
        }
        printf("\n");
    }
}

void update(int count, int8_t* arr){ // helper function for num_in_row
    if(count == 2){
        arr[0]++;
    }
    else if(count == 3){
        arr[1]++;
    }
    else if(count >= 4){
        arr[2]++;
    }
}

void print_data(node* inp,int8_t depth,int8_t player){
    printf("VALUE: %d,  COLUMN: %d, DEPTH: %d, PLAYER: %d\n",inp->value,inp->column,depth,player);
}

int8_t boards_equal(int8_t** board1, int8_t** board2){
    for(int8_t i = 0;i<6;i++){
        for(int8_t j=0;j<7;j++){
            if(board1[i][j]!=board2[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

int max(int a, int b){ // self explanatory
    return a >= b ? a : b;
}

int min(int a, int b){ // self explanatory
    return a <= b ? a : b;
}

int8_t* num_in_row(int8_t** board,int8_t player){// 1 for red, -1 for yellow
    int count = 0;
    int temp = 0;
    int i;
    int j;
    int indx_i;
    int indx_j;

    int8_t* ret = (int8_t*)calloc(4,sizeof(int8_t));

    for(i = 0; i < 6;i++){
        j = 0;
        while(j < 7){
            temp = 0;
            while(j < 7 && board[i][j] == player){
                j++;
                temp++;
                ret[3]++;
            }
            update(temp,ret);
            j++;

        }
    }

    for(i = 0;i<7;i++){ // vertical search
        j = 0;
        while(j < 6){
            temp = 0;
            while(j < 6 && board[j][i]==player){
                j++;
                temp++;
            }
            update(temp,ret);
            j++;

        }
    }

    // up-right diagonal search
    i = 6;
    j = 0;
    while(i >= 0){
        indx_i = i;
        indx_j = 0;
        while(indx_i < 6){
            temp = 0;
            while(indx_i < 6 && board[indx_i][indx_j]==player){
                temp++;
                indx_i++;
                indx_j++;
            }
            update(temp,ret);
            indx_i++;
            indx_j++;

        }
        i--;
    }
    i=0;
    j=1;
    while(j<7){
        indx_i = 0;
        indx_j = j;
        while(indx_j < 7){
            temp = 0;
            while(indx_j < 7 && board[indx_i][indx_j]==player){
                temp++;
                indx_i++;
                indx_j++;
            }
            update(temp,ret);
            indx_i++;
            indx_j++;

        }
        j++;
    }

    i = 6; // up - left diagonal search
    j = 6;
    while(i >= 0){
        indx_i = i;
        indx_j = j;
        while(indx_i < 6){
            temp = 0;
            while(indx_i < 6 && board[indx_i][indx_j] == player){
                temp++;
                indx_i++;
                indx_j--;
            }
            update(temp,ret);
            indx_i++;
            indx_j--;

        }
        i--;
    }


    i = 0;
    j = 5;
    while(j >= 0){
        indx_i = i;
        indx_j = j;
        while(indx_j >= 0){
            temp = 0;
            while(indx_j >= 0 && board[indx_i][indx_j]==player){
                temp++;
                indx_i++;
                indx_j--;
            }
            update(temp,ret);
            indx_i++;
            indx_j--;

        }
        j--;
    }
    return ret;
}

int heuristic(int8_t** board, int8_t player){
    int middle_count = 0;
    int op_player = -player;

    for(int i = 0;i<6;i++){
        if(board[i][3]==player){
            middle_count++;
        }
    }

    int good_4 = 0;
    int good_2 = 0;
    int good_3 = 0;

    for(int i = 0;i<6;i++){
        for(int j = 0; j<4;j++){
            int player_count = 0;
            int broke = 0;
            for(int k=0;k<4;k++){
                if(board[i][j+k]==op_player){
                    broke = 1;
                    break;
                }
                else if(board[i][j+k]==player){
                    player_count++;
                }
            }
            if(!broke){
                if(player_count == 2){
                    good_2++;
                }
                else if(player_count == 3){
                    good_3++;
                }
            }
        }
    }


    for(int i=0;i<3;i++){
        for(int j = 0;j<7;j++){
            int player_count = 0;
            int broke = 0;
            for(int k=0;k<4;k++){
                if(board[i+k][j]==op_player){
                    broke = 1;
                    break;
                }
                else if(board[i+k][j]==player){
                    player_count++;
                }
            }
            if(!broke){
                if(player_count == 2){
                    good_2++;
                }
                else if(player_count == 3){
                    good_3++;
                }
            }
        }
    }

    for(int i= 0;i<3;i++){
        for(int j = 0;j<4;j++){
            int player_count = 0;
            int broke = 0;
            for(int k=0;k<4;k++){
                if(board[i+k][j+k]==op_player){
                    broke = 1;
                    break;
                }
                else if(board[i+k][j+k]==player){
                    player_count++;
                }
            }
            if(!broke){
                if(player_count == 2){
                    good_2++;
                }
                else if(player_count == 3){
                    good_3++;
                }
            }
        }
    }

    for(int i=5;i>=3;i--){
        for(int j=0;j<=3;j++){
            int player_count = 0;
            int broke = 0;
            for(int k=0;k<4;k++){
                if(board[i-k][j+k]==op_player){
                    broke = 1;
                    break;
                }
                else if(board[i-k][j+k]==player){
                    player_count++;
                }
            }
            if(!broke){
                if(player_count == 2){
                    good_2++;
                }
                else if(player_count == 3){
                    good_3++;
                }
            }
        }
    }

    int good_5 = 0;

    for(int i = 0;i<6;i++){
        for(int j = 0; j<3;j++){
            int player_count = 0;
            int broke = 0;
            for(int k=0;k<5;k++){
                if(board[i][j+k]==op_player){
                    broke = 1;
                    break;
                }
                else if(board[i][j+k]==player){
                    player_count++;
                }
            }
            if(!broke && player_count == 3){
                good_5++;
            }
        }
    }

    for(int i= 0;i<2;i++){
        for(int j = 0;j<3;j++){
            int player_count = 0;
            int broke = 0;
            for(int k=0;k<5;k++){
                if(board[i+k][j+k]==op_player){
                    broke = 1;
                    break;
                }
                else if(board[i+k][j+k]==player){
                    player_count++;
                }
            }
            if(!broke && player_count == 3){
                good_5++;
            }
        }
    }

    for(int i=5;i>=4;i--){
        for(int j=0;j<=2;j++){
            int player_count = 0;
            int broke = 0;
            for(int k=0;k<5;k++){
                if(board[i-k][j+k]==op_player){
                    broke = 1;
                    break;
                }
                else if(board[i-k][j+k]==player){
                    player_count++;
                }
            }
            if(!broke && player_count == 3){
                good_5++;
            }
        }
    }

    return 2*middle_count + 4*good_2 + 16*(good_3-good_5) + 100*good_5;
}

int evaluation(int8_t** board){// evaluation function as outlined in assignment
    return heuristic(board,1)-heuristic(board,-1);
}

int utility(int8_t** board){ // return 10000 if red wins, else if y wins return -10000, else return evaluation]
    int8_t* r_counts = num_in_row(board,1);
    int8_t* y_counts = num_in_row(board,-1);
    int ret;
    if(r_counts[2]){
        ret = 10000;
    }
    else if(y_counts[2]){
        ret = -10000;
    }
    ret = evaluation(board);
    free(r_counts);
    free(y_counts);
    return ret;
}

int8_t can_drop(int8_t** board, int8_t column){//return 1 if can drop, 0 if can't drop
    return board[5][column]==0;
}

int8_t is_draw(int8_t** board){
    for(int i=0;i<7;i++){
        if(can_drop(board,i)){
            return 0;
        }
    }
    return 1;
}

int8_t** drop(int8_t** board, int8_t column, int8_t player){ // create a new board and drop a counter into it
    int8_t** output = (int8_t**)malloc(sizeof(int8_t*)*6);
    for(int8_t i = 0;i<6;i++){
        output[i] = (int8_t*)malloc(sizeof(int8_t)*7);
        for(int8_t j=0;j<7;j++){
            output[i][j] = board[i][j];
        }
    }

    if(column == -1){
        return output;
    }

    for(int8_t i = 0;i<6;i++){
        if(output[i][column]==0){
            output[i][column] = player;
            break;
        }
    }
    return output;
}

void free_board(int8_t** board){
    for(int i=0;i<6;i++){
        free(board[i]);
    }
    free(board);
}

int8_t is_winning_move(int8_t** board, int8_t column, int8_t player){
    // first, check where the item is dropped...
    int found_spot;
    for(int i = 0;i<6;i++){
        if(board[i][column]==0){
            found_spot = i;
            break;
        }
    }
    // horizontal check for winner
    int h_count = 1;
    for(int j = column+1;j<7;j++){
        if(board[found_spot][j]==player){
            h_count++;
        }
        else{
            break;
        }
    }
    for(int j = column-1;j>=0;j--){
        if(board[found_spot][j]==player){
            h_count++;
        }
        else{
            break;
        }
    }
    if(h_count >= 4){
        return 1;
    }

    int v_count = 1;
    for(int i = found_spot-1;i>=0;i--){
        if(board[i][column]==player){
            v_count++;
        }
        else{
            break;
        }
    }
    if(v_count == 4){
        return 1;
    }

    int d_count1 = 1;
    int tempi = found_spot + 1;
    int tempj = column + 1;

    while(tempi < 6 && tempj < 7){
        if(board[tempi][tempj]==player){
            d_count1++;
        }
        else{
            break;
        }
        tempi++;
        tempj++;
    }

    tempi = found_spot -1;
    tempj = column - 1;

    while(tempi >= 0 && tempj >= 0){
        if(board[tempi][tempj]==player){
            d_count1++;
        }
        else{
            break;
        }
        tempi--;
        tempj--;
    }

    if(d_count1 >= 4){
        return 1;
    }

    int d_count2 = 1;
    tempi = found_spot+1;
    tempj = column-1;
    while(tempi < 6 && tempj >= 0){
        if(board[tempi][tempj]==player){
            d_count2++;
        }
        else{
            break;
        }
        tempi++;
        tempj--;
    }

    tempi = found_spot-1;
    tempj = column+1;
    while(tempi >= 0 && tempj < 7){
        if(board[tempi][tempj]==player){
            d_count2++;
        }
        else{
            break;
        }
        tempi--;
        tempj++;
    }

    if(d_count2 >= 4){
        return 1;
    }
    //printf("column: %d, diagonal count1 %d, diagonal count2 %d\n",column,d_count1,d_count2);
    return 0;
}

int8_t can_win(int8_t** board, int8_t player){
    int column_set[7] = {3,4,2,1,5,0,6};
    for(int i = 0;i<7;i++){
        if(can_drop(board,column_set[i])){
            if(is_winning_move(board,column_set[i],player)){
                return column_set[i];
            }
        }
    }
    return -1;
}

int8_t about_to_lose(int8_t** board, int8_t player){ // return column
    return can_win(board,-player);
}

int8_t bad_move(int8_t** board, int8_t player){ // return true / false
    return about_to_lose(board,player)!=-1;
}

node* negamax(int8_t** board, int8_t depth, int alpha, int beta, int8_t colour){
    node* output = (node*)calloc(1,sizeof(node));
    int8_t column_set[7] = {3,4,2,1,5,0,6};
    int util = utility(board);
    if(util==10000 || util == -10000){
        output->value = colour*util;
        output->column = -1;
        return output;
    }

    if(is_draw(board)){
        output->value = 0;
        output->column = -1;
        return output;
    }

    if(depth == 0){
        output->value = colour*evaluation(board);
        output->column = -1;
        return output;
    }

    output->value = INT_MIN+1;

    int8_t recurred = 0;

    for(int i = 0;i<7;i++){
        if(can_drop(board,column_set[i])){
            int8_t** new_board = drop(board,column_set[i],colour);

            if(bad_move(new_board,colour)){
                free_board(new_board);
                continue;
            }

            recurred = 1;
            node* recur = negamax(new_board,depth-1,-beta,-alpha,-colour);
            recur->value *= -1;

            if(recur->value > output->value){
                output->value = recur->value;
                output->column = column_set[i];
            }

            alpha = max(alpha,output->value);

            free_board(new_board);
            free(recur);

            if(alpha>=beta){
                break;
            }
        }
    }

    if(!recurred){ // all moves are crap at this point
        for(int8_t i = 0;i<7;i++){
            if(can_drop(board,column_set[i])){
                output->value = -10000;
                output->column = -1;
                break;
            }
        }
    }
    return output;
}

int main(int argc, char** argv){
    char* state = argv[1];
    char player = argv[2][0];

    int8_t** board = (int8_t**)malloc(sizeof(int8_t*)*6);

    for(int i = 0;i<6;i++){
        board[i] = (int8_t*)malloc(sizeof(int8_t)*7);
    }

    char* line = strtok(state,","); // process the board state
    int8_t i = 0;
    int token_count = 0;
    while(line != NULL){
        for(int j=0;j<7;j++){
            if(line[j] != '.'){
                token_count++;
            }
            board[i][j] = (line[j]=='r') ? 1 : ((line[j]=='y') ? -1 : 0);
        }
        line = strtok(NULL,",");
        i++;
    }


    /* any code here is just run once for great program strength */
    int hardcode_success = 0;

    if(token_count <= 4){
        string filename = to_string(token_count)+".txt";
        ifstream file(to_string(token_count)+".txt");
        string fline;
        while(getline(file,fline)){
            int8_t** temp = (int8_t**)malloc(sizeof(int8_t*)*6);
            for(int8_t j = 0;j<6;j++){
                temp[j] = (int8_t*)malloc(sizeof(int8_t)*7);
            }

            char* temp_state = (char*)fline.c_str();
            i = 0;
            strtok(temp_state,",");
            while(temp_state != NULL && i < 6){
                for(int8_t j=0;j<7;j++){
                    temp[i][j] = (temp_state[j]=='r') ? 1 : ((temp_state[j]=='y') ? -1 : 0);
                }
                temp_state = strtok(NULL,",");
                i++;
            }
            if(boards_equal(board,temp)){
                printf("%s\n",temp_state);
                hardcode_success = 1;
            }
            free(temp);
        }
    }

    if(!hardcode_success){
        int8_t colour = (player == 'r' ? 1 : -1);
        int8_t check_win = can_win(board,colour);
        int8_t check_lose = about_to_lose(board,colour);
        if(check_win != -1){ // can't win
            printf("%d\n",check_win);
        }
        if(check_lose != -1){ // about to lose
            printf("%d\n",check_lose);
        }
        else{
            node* alpha = negamax(board,max(8,(token_count)/2),INT_MIN+1,INT_MAX,colour);

            if(alpha->column == -1){
                sleep(1);
            }
            else{
                printf("%d\n",alpha->column);
            }
            free(alpha);
        }
    }

    free_board(board);
    //printf("It took me %d clicks (%f seconds).\n",(int)t,(float)((float)t)/CLOCKS_PER_SEC);

    return 0;
}
