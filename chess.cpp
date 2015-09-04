#include <stdio.h>  // printf
#include <stdint.h> // uint64_t
#include <string.h> // memcpy

// BEGIN OMP
    #include <omp.h>
// END OMP

#include "utility.h"
#include "bitboard.h"
#include "pieces.h"
#include "state.h"
#include "game.h"
#include "eval.h"

// BEGIN OMP
int gnThreadsMaximum = 0 ;
int gnThreadsActive  = 0 ; // 0 = auto detect; > 0 manual # of threads

void InitMulticore()
{
// BEGIN OMP
    gnThreadsMaximum = omp_get_num_procs();

    if(!gnThreadsActive) // user didn't specify how many threads to use, default to all of them
        gnThreadsActive = gnThreadsMaximum;
    else
        omp_set_num_threads( gnThreadsActive );

    printf( "Using: %d / %d cores\n", gnThreadsActive, gnThreadsMaximum );

    for( int iCore = 0; iCore < gnThreadsActive; iCore++ )
    {
        nMovePool[ iCore ] = 0;
        aMovePool[ iCore ] = new State_t[ MAX_POOL_MOVES ];
    }

    size_t nMemState = sizeof( State_t[ MAX_POOL_MOVES ] );
    printf( "Mem/Core: %u bytes (%u KB)\n", (uint32_t) nMemState, (uint32_t) nMemState/1024 );
}
// END OMP

int main( const int nArg, const char *aArg[] )
{
    (void) nArg;
    (void) aArg;

    InitMulticore();

    State_t state;
    state.Init();

#if 0
    //state.Dump();
    state.PrettyPrintBoard();
#endif

#if 1
    {
        /*
            uint32_t aEVAL_LOCATION_QUEEN[ 8 ] =
            {   // ABCDEFGH
                 0x // 8
                ,0x // 7
                ,0x // 6
                ,0x // 5
                ,0x // 4
                ,0x // 3
                ,0x // 2
                ,0x // 1
            };
        */
        bitboard_t board;
        printf( "= Queen = # of Moves/Cell\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );
            board = BitBoardMovesColorQueen( rankfile );

            const int SIZE = 64;
            uint8_t nMoves;
            uint8_t aMoves[ SIZE ];
            BitBoardToRankFileAllMoves( board, nMoves, aMoves, SIZE );

            printf( "# 0x%1X ", nMoves );
            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
        }
    }
#endif

    // spin-lock
    // wait for command

    return 0;
}

