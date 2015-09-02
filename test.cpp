#include <stdio.h>  // printf
#include <stdint.h> // uint64_t
#include <string.h> // memcpy

#include "utility.h"
#include "bitboard.h"
#include "pieces.h"
#include "state.h"
#include "game.h"

int main()
{

#if 0
    make_bitcount8();

//    for( int i = 0; i < 256; i++ )
//    {
//        int a = bitcount8( i );
//        int b = popcount64( i );
//            printf( "[%02X]: %d != %d\n", i, a, b );
//    }
#endif

#if 0
    bitboard_t board;
        uint8_t col, row, rankfile = 0x30; // A4
        RankFileToColRow( rankfile, col, row );
        printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
    board = BitBoardMovesColorKnight( rankfile ); 
    BitBoardPrint( board );
    int moves = bitcount( board );
    printf( "%d max potential moves\n", moves );
#endif

#if 0
    bitboard_t board;
    board = BitBoardMakeWhiteSquares();
    printf( "White Squares\n" );
    BitBoardPrint( board );
    printf( "\n" );

    board = BitBoardMakeBlackSquares();
    printf( "Black Squares\n" );
    BitBoardPrint( board );
    printf( "\n" );
#endif

#if 0
    State_t state;
    state.Init();
    state.Dump();

    state.PrettyPrintBoard();
#endif

#if 0
    for( int cell = 0; cell < 64; cell++ )
    {
        //uint8_t col      = cell & 7;
        //uint8_t row      = cell / 8;
        //uint8_t rankfile = (row << 4) + col;

        int col, row, rankfile;
        CellToColRow( cell, col, row );
        rankfile = ColRowToRankFile( col, row );

        printf( "[%d,%d]: 0x%02X %c%c -> %d\n", col, row, rankfile, aFILE[ col ], aRANK[ row ], (row + col) );
    }
#endif

#if 0
    bitboard_t board;
    printf( "= White Pawn Moves =\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );

            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
            board = BitBoardMovesWhitePawn( rankfile );
            BitBoardPrint( board );
            printf( "\n" );
        }
#endif

#if 0
    {
        bitboard_t board;
        printf( "= Black Pawn Moves =\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );

            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
            board = BitBoardMovesBlackPawn( rankfile );
            BitBoardPrint( board );
            printf( "\n" );
        }

    }
#endif

#if 0
        bitboard_t board;
        printf( "= Rook Moves =\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );

            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
            board = BitBoardMovesColorRook( rankfile );
            BitBoardPrint( board );
            printf( "\n" );
        }
#endif

#if 1
    bitboard_t board;
    printf( "= Knight Moves =\n" );

        //board = BitBoardMovesColorKnight( 0x33 ); // D4
        //board = BitBoardMovesColorKnight( 0x30 ); // A4
        //board = BitBoardMovesColorKnight( 0x70 ); // A8
        //BitBoardPrint( board );
        //printf( "\n" );

//if(1)
        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );

            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
            board = BitBoardMovesColorKnight( rankfile );
            BitBoardPrint( board );
            printf( "\n" );
        }
#endif

#if 0
    bitboard_t board;
    printf( "= Bishop Moves =\n" );

        BitBoardPrint( board );
        printf( "\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );

            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
            board = BitBoardMovesColorBishop( rankfile );
            BitBoardPrint( board );
            printf( "\n" );
        }
#endif

#if 0
    bitboard_t board;
    printf( "= Queen Moves =\n" );

        board = BitBoardMovesColorQueen( 0x00 );
        BitBoardPrint( board );
        printf( "\n" );

        board = BitBoardMovesColorQueen( 0x07 );
        BitBoardPrint( board );
        printf( "\n" );

        board = BitBoardMovesColorQueen( 0x70 );
        BitBoardPrint( board );
        printf( "\n" );

        board = BitBoardMovesColorQueen( 0x77 );
        BitBoardPrint( board );
        printf( "\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );

            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
            board = BitBoardMovesColorQueen( rankfile );

            BitBoardPrint( board );
            printf( "\n" );
        }
#endif

#if 0
    {
        bitboard_t board;
        printf( "= King Moves =\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );

            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
            board = BitBoardMovesColorKing( rankfile );
            uint8_t kingRF = BitBoardToRankFile( board );
            printf( "rank_file: %02X %s %02X kingRF\n", rankfile, (rankfile == kingRF) ? "==" : "!=", kingRF );

            BitBoardPrint( board );
            printf( "\n" );
        }
    }
#endif

    return 0;
}

