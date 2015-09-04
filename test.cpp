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
    // Input
        size_t nLenInput = 0;
        size_t nLenCmd   = 0;
        size_t nLenParam = 0;

        for( char *p = sInput; *p; p++ )
{
printf( "[%u]: 0x%02X '%c'\n", (unsigned) (p - sInput), *p, *p );
            if( *p == 0x0A || *p == 0x0D )
                *p = 0;

            if( *p == ' ' || *p == 0 )
            {
                nLenInput = strlen( sInput );
                nLenCmd   = p - sInput;
                pParam    = p + 1;
                nLenParam = strlen( pParam );
                *p      = 0;
                break;
            }
}

printf( "Len Input: %u\n", (unsigned) nLenInput );
printf( "Len Comnd: %u\n", (unsigned) nLenCmd );
printf( "Len Param: %u\n", (unsigned) nLenParam );
#endif


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

#if 0
    {
        bitboard_t board;
        printf( "= Knight Moves =\n" );

        //board = BitBoardMovesColorKnight( 0x33 ); // D4
        //board = BitBoardMovesColorKnight( 0x30 ); // A4
        //board = BitBoardMovesColorKnight( 0x70 ); // A8
        //BitBoardPrint( board );
        //printf( "\n" );

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
    }
#endif

#if 0
    {
        // uint32_t aEVAL_LOCATION_KNIGHT[8] =
        bitboard_t board;
        printf( "= Knight = # of Moves/Cell\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );
            board = BitBoardMovesColorKnight( rankfile );

            uint8_t nMoves;
            uint8_t aMoves[ MAX_PIECE_MOVES ];
            BitBoardToRankFileAllMoves( board, nMoves, aMoves, MAX_PIECE_MOVES );

            printf( "# 0x%1X ", nMoves );
            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
        }
    }
#endif

// ===

#if 0
    {
        bitboard_t board;
        printf( "= Bishop = Moves =\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );
            board = BitBoardMovesColorBishop( rankfile );

            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
            BitBoardPrint( board );
            printf( "\n" );
        }
    }
#endif

#if 0
    {
        // uint32_t aEVAL_LOCATION_BISHOP[8] =
        bitboard_t board;
        printf( "= Bishop = # of Moves/Cell\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );
            board = BitBoardMovesColorBishop( rankfile );

            uint8_t aMoves[ MAX_PIECE_MOVES ];
            BitBoardToRankFileAllMoves( board, nMoves, aMoves, MAX_PIECE_MOVES );

            printf( "# 0x%1X ", nMoves );
            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
        }
    }
#endif


#if 0
        printf( "= Bishop = Print each potential move\n" ); // 26

        uint8_t nRankFile = 0x24;
        bitboard_t board = 0;
        board |= BitBoardMovesColorBishop( 0x33 );
        board |= BitBoardMovesColorBishop( 0x34 );

        uint8_t nMoves;
        uint8_t aMoves[ MAX_PIECE_MOVES ];
        BitBoardToRankFileAllMoves( board, nMoves, aMoves, MAX_PIECE_MOVES );

        BitBoardPrint( board );

        printf( "  Moves: %d\n", nMoves );
        for( int iMove = 0; iMove < nMoves; iMove++ )
        {
            uint8_t nRF  = aMoves[ iMove ];
            uint8_t nRow; // Rank
            uint8_t nCol; // File
            RankFileToColRow( nRF, nCol, nRow );

            printf( "  [%2d]: 0x%02X %c%c\n", iMove, aMoves[ iMove ], RankFileToFile( nRF ), RankFileToRank( nRF ) );
        }
#endif

// ==========


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

#if 1
        printf( "= Queen - # Moves for all positions =\n" );
        int maxmoves = 0;

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );

            bitboard_t board = BitBoardMovesColorQueen( rankfile );
            uint8_t moves = bitcount( board );
            printf( "%02X: %d\n", cell, moves );

            if( maxmoves < moves )
                maxmoves = moves ;
        }
        printf( "Max Moves: %d\n", maxmoves );

#endif

#if 1
        printf( "= Queen - Print each potential move =\n" );

        uint8_t nRankFile = 0x24;
        bitboard_t board = BitBoardMovesColorQueen( nRankFile );

        uint8_t nMoves;
        uint8_t aMoves[ 32 ];
        BitBoardToRankFileAllMoves( board, nMoves, aMoves );

        BitBoardPrint( board );

        printf( "  Moves: %d\n", nMoves );
        for( int iMove = 0; iMove < nMoves; iMove++ )
        {
            uint8_t nRF  = aMoves[ iMove ];
            uint8_t nRow; // Rank
            uint8_t nCol; // File
            RankFileToColRow( nRF, nCol, nRow );

            printf( "  [%2d]: 0x%02X %c%c\n", iMove, aMoves[ iMove ], RankFileToFile( nRF ), RankFileToRank( nRF ) );
        }

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

            uint8_t nMoves;
            uint8_t aMoves[ MAX_PIECE_MOVES ];
            BitBoardToRankFileAllMoves( board, nMoves, aMoves, MAX_PIECE_MOVES );

            printf( "# 0x%1X ", nMoves );
            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
        }
    }
#endif

// ===

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

#if 0
        bitboard_t board;

        uint8_t rankfile = 0x00;
        board = BitBoardMakeLocation( rankfile ); // BitBoardMakeLocation // BitBoardMovesColorKing

        uint8_t kingRF = BitBoardToRankFile( board );
        printf( "rank_file: %02X %s %02X kingRF\n", rankfile, (rankfile == kingRF) ? "==" : "!=", kingRF );

        BitBoardPrint( board );
    printf( "\n" );
#endif

#if 1
        bitboard_t board;
        printf( "= King Position to Rank File =\n" );

        for( int cell = 0; cell < 64; cell++ )
        {
            uint8_t col, row, rankfile;
            CellToColRow( cell, col, row );
            rankfile = ColRowToRankFile( col, row );

            printf( "[%d,%d]: 0x%02X %c%c\n", col, row, rankfile, aFILE[ col ], aRANK[ row ] );
            board = BitBoardMakeLocation( rankfile );
            uint8_t kingRF = BitBoardToRankFile( board );
            printf( "rank_file: %02X %s %02X kingRF\n", rankfile, (rankfile == kingRF) ? "==" : "!=", kingRF );

            BitBoardPrint( board );
            printf( "\n" );
        }
#endif

    return 0;
}

