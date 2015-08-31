/*

  63 62 61 60 59 58 57 56   WBWBWBWB
  55 54 53 52 51 50 49 48   BWBWBWBW
  47 46 45 44 43 42 41 40   WBWBWBWB
  39 38 37 36 35 34 33 32   BWBWBWBW
  31 30 29 28 27 26 25 24   WBWBWBWB
  23 22 21 20 19 18 17 16   BWBWBWBW
  15 14 13 12 11 10  9  8   WBWBWBWB
   7  6  5  4  3  2  1  0   BWBWBWBW

*/

typedef uint64_t bitboard_t;

bitboard_t BitBoardMakeWhiteSquares () { bitboard_t board = 0xAA55AA55AA55AA55UL; return board; }
bitboard_t BitBoardMakeBlackSquares () { bitboard_t board = 0x55AA55AA55AA55AAUL; return board; }
bitboard_t BitBoardMakeBlackInit    () { bitboard_t board = 0xFFFF000000000000UL; return board; }
bitboard_t BitBoardMakeWhiteInit    () { bitboard_t board = 0x000000000000FFFFUL; return board; }

bitboard_t BitBoardInitBlackPawn    () { bitboard_t board = 0x00FF000000000000UL; return board; }
bitboard_t BitBoardInitWhitePawn    () { bitboard_t board = 0x000000000000FF00UL; return board; }
bitboard_t BitBoardInitBlackRook    () { bitboard_t board = 0x8100000000000000UL; return board; }
bitboard_t BitBoardInitWhiteRook    () { bitboard_t board = 0x0000000000000081UL; return board; }
bitboard_t BitBoardInitBlackKnight  () { bitboard_t board = 0x4200000000000000UL; return board; }
bitboard_t BitBoardInitWhiteKnight  () { bitboard_t board = 0x0000000000000042UL; return board; }
bitboard_t BitBoardInitBlackBishop  () { bitboard_t board = 0x2400000000000000UL; return board; }
bitboard_t BitBoardInitWhiteBishop  () { bitboard_t board = 0x0000000000000024UL; return board; }
bitboard_t BitBoardInitBlackQueen   () { bitboard_t board = 0x1000000000000000UL; return board; }
bitboard_t BitBoardInitWhiteQueen   () { bitboard_t board = 0x0000000000000010UL; return board; }
bitboard_t BitBoardInitBlackKing    () { bitboard_t board = 0x0800000000000000UL; return board; }
bitboard_t BitBoardInitWhiteKing    () { bitboard_t board = 0x0000000000000008UL; return board; }

const char aFILE [8] = { '1', '2', '3', '4', '5', '6', '7', '8' }; // Col
const char aROW  [8] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' }; // Row

void BitBoardPrint( const bitboard_t board, const char solidPiece = 'X', const bool bPrintRankFile = true )
{
    static char aPieces[2] = { '.', '?' }; // Empty, Solid
    aPieces[ 1 ] = solidPiece;

    const int  BOARD_SIZE = 11*9 + 1; // BOARD_MARK is largest

    const char BOARD_BARE[ BOARD_SIZE ] =
        "????????\n"
        "????????\n"
        "????????\n"
        "????????\n"
        "????????\n"
        "????????\n"
        "????????\n"
        "????????\n";

    const char BOARD_MARK[ BOARD_SIZE ] =
        "8 ????????\n"
        "7 ????????\n"
        "6 ????????\n"
        "5 ????????\n"
        "4 ????????\n"
        "3 ????????\n"
        "2 ????????\n"
        "1 ????????\n"
        "  ABCDEFGH\n";

    const char *aBOARDS[2] = { BOARD_BARE, BOARD_MARK };

    static char      aCells[ BOARD_SIZE ];
    /* */  char *p = aCells + 2*bPrintRankFile;

    memcpy( aCells, aBOARDS[ bPrintRankFile ], BOARD_SIZE );

    bitboard_t temp = board;
    for( int y = 7; y >= 0; y-- )
    {
        temp = board >> (8 * y);

        for( int x = 7; x >= 0; x-- )
            *p++ = aPieces[ ((temp >> x) & 1) ];

        if( bPrintRankFile )
            p += 3;
    }

    printf( "%s", aCells );
}
