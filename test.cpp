#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <iostream> 


using namespace std;


typedef uint64_t board_t;
typedef uint16_t row_t;

static const board_t ROW_MASK = 0xFFFFULL;
static const board_t COL_MASK = 0x000F000F000F000FULL;

static row_t row_left_table [65536];
static row_t row_right_table[65536];
static board_t col_up_table[65536];
static board_t col_down_table[65536];
//static float heur_score_table[65536];
//static float score_table[65536];

static inline board_t
transpose(board_t board) {
  board_t temp = board & 0xf0f00f0ff0f00f0fULL;
  board_t temp1 = board & 0x0000f0f00000f0f0ULL;
  board_t temp2 = board & 0x0f0f00000f0f0000ULL;
  temp = temp | (temp1 << 12) | (temp2 >> 12);
  board_t temp0 = temp & 0xff00ff0000ff00ffULL;
  temp1 = temp & 0x00ff00ff00000000ULL;
  temp2 = temp & 0x00000000ff00ff00ULL;
  return temp0 | (temp1 >> 24) | (temp2 << 24);
}

static inline void print_board(board_t board) {
    int i,j;
    for(i=0; i<4; i++) {
        for(j=0; j<4; j++) {
            uint8_t powerVal = (board) & 0x000000000000000fULL;
            printf("%6u", (powerVal == 0) ? 0 : 1 << powerVal);
            board >>= 4;
        }
        printf("\n");
    }
    printf("\n");
}

static inline board_t execute_move_0(board_t board) {
    board_t ret = board;
    board_t t = transpose(board);
    ret ^= col_up_table[(t >>  0) & ROW_MASK] <<  0;
    ret ^= col_up_table[(t >> 16) & ROW_MASK] <<  4;
    ret ^= col_up_table[(t >> 32) & ROW_MASK] <<  8;
    ret ^= col_up_table[(t >> 48) & ROW_MASK] << 12;
    return ret;
}

static inline board_t execute_move_1(board_t board) {
    board_t ret = board;
    board_t t = transpose(board);
    ret ^= col_down_table[(t >>  0) & ROW_MASK] <<  0;
    ret ^= col_down_table[(t >> 16) & ROW_MASK] <<  4;
    ret ^= col_down_table[(t >> 32) & ROW_MASK] <<  8;
    ret ^= col_down_table[(t >> 48) & ROW_MASK] << 12;
    return ret;
}

static inline board_t execute_move_2(board_t board) {
    board_t ret = board;
    ret = board_t(row_left_table[(board >>  0) & ROW_MASK]) <<  0;
    print_board(row_left_table[(board >>  0) & ROW_MASK]);
    ret += board_t(row_left_table[(board >> 16) & ROW_MASK]) << 16;
    print_board(row_left_table[(board >>  16) & ROW_MASK]);
    ret += board_t(row_left_table[(board >> 32) & ROW_MASK]) << 32;
    print_board(row_left_table[(board >>  32) & ROW_MASK]);
    ret += board_t(row_left_table[(board >> 48) & ROW_MASK]) << 48;
    print_board(row_left_table[(board >>  48) & ROW_MASK]);
    return ret;
}

static inline board_t execute_move_3(board_t board) {
    board_t ret = board;
    ret ^= board_t(row_right_table[(board >>  0) & ROW_MASK]) <<  0;
    ret ^= board_t(row_right_table[(board >> 16) & ROW_MASK]) << 16;
    ret ^= board_t(row_right_table[(board >> 32) & ROW_MASK]) << 32;
    ret ^= board_t(row_right_table[(board >> 48) & ROW_MASK]) << 48;
    return ret;
}

static inline board_t execute_move(int move, board_t board) {
    switch(move) {
    case 0: // up
        return execute_move_0(board);
    case 1: // down
        return execute_move_1(board);
    case 2: // left
        return execute_move_2(board);
    case 3: // right
        return execute_move_3(board);
    default:
        return ~0ULL;
    }
}

static inline board_t unpack_col(row_t row) {
    board_t tmp = row;
    return (tmp | (tmp << 12ULL) | (tmp << 24ULL) | (tmp << 36ULL)) & COL_MASK;
}

static inline row_t reverse_row(row_t row) {
    return (row >> 12) | ((row >> 4) & 0x00F0) | ((row << 4) & 0x0F00) | (row << 12);
}

void init_tables() {
    for (unsigned row = 0; row < 65536; ++row) {
        unsigned line[4] = {
                (row >>  0) & 0xf,
                (row >>  4) & 0xf,
                (row >>  8) & 0xf,
                (row >> 12) & 0xf
        };


        // execute a move to the left
        for (int i = 0; i < 3; ++i) {
            int j;
            for (j = i + 1; j < 4; ++j) {
                if (line[j] != 0) break;
            }
            if (j == 4) break; // no more tiles to the right

            if (line[i] == 0) {
                line[i] = line[j];
                line[j] = 0;
                i--; // retry this entry
            } else if (line[i] == line[j]) {
                if(line[i] != 0xf) {
                    /* Pretend that 32768 + 32768 = 32768 (representational limit). */
                    line[i]++;
                }
                line[j] = 0;
            }
        }

        row_t result = (line[0] <<  0) |
                       (line[1] <<  4) |
                       (line[2] <<  8) |
                       (line[3] << 12);
        row_t rev_result = reverse_row(result);
        unsigned rev_row = reverse_row(row);

        cout << "row = " << row << " rev = " << rev_row << endl;

        row_left_table [    row] =                row  ^                result;
        row_right_table[rev_row] =            rev_row  ^            rev_result;
        col_up_table   [    row] = unpack_col(    row) ^ unpack_col(    result);
        col_down_table [rev_row] = unpack_col(rev_row) ^ unpack_col(rev_result);
    }
}

int main() {
	board_t board = 0x1002000000001001ULL;

	init_tables();

	//print_board(board);

    // board = transpose(board);
    // //print_board(board);


	board = execute_move(2, board);
	printf("move 2\n");
	print_board(board);

	board = execute_move(3, board);
	// printf("move 3\n");
	// print_board(board);

	board = execute_move(1, board);
	// printf("move 1\n");
	// print_board(board);

	board = execute_move(0, board);
	// printf("move 0\n");
	// print_board(board);
 //    unsigned row = 0;
 //    unsigned rev = 0;
	// for (row = 0; row < 65536; ++row) {
	// 	rev = (row >> 12) | ((row >> 4) & 0x00F0)  | ((row << 4) & 0x0F00) | (row << 12);
	// 	if (rev > 65535) cout << "row = " << row << " rev = " << rev << endl;
	// }

	return 0;
}