#include <stdio.h>  // printf
#include <stdint.h> // uint64_t
#include <string.h> // memcpy

#include "bitboard.h"
#include "pieces.h"
#include "state.h"

int main()
{
    bitboard_t board;

    board = BitBoardMakeWhiteSquares();
    printf( "White Squares\n" );
    BitBoardPrint( board );
    printf( "\n" );

    board = BitBoardMakeBlackSquares();
    printf( "Black Squares\n" );
    BitBoardPrint( board );
    printf( "\n" );

    State_t state;
    state.Init();
    state.Dump();

    return 0;
}

