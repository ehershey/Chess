/*

    WBWBWBWB
    BWBWBWBW
    WBWBWBWB
    BWBWBWBW
    WBWBWBWB
    BWBWBWBW
    WBWBWBWB
    BWBWBWBW

3 Choices for Bitboards: (We use the first one)

  63 62 61 60 59 58 57 56   a8 b8 c8 d8 e8 f8 g8 h8
  55 54 53 52 51 50 49 48   a7 b7 c7 d7 e7 f7 g7 h7
  47 46 45 44 43 42 41 40   a6 b6 c6 d6 e6 f6 g6 h6
  39 38 37 36 35 34 33 32   a5 b5 c5 d5 e5 f5 g5 h5
  31 30 29 28 27 26 25 24   a4 b4 c4 d4 e4 f4 g4 h4
  23 22 21 20 19 18 17 16   a3 b3 c3 d3 e3 f3 g3 h3
  15 14 13 12 11 10  9  8   a2 b2 c2 d2 e2 f2 g2 h2
   7  6  5  4  3  2  1  0   a1 b1 c1 d1 e1 f1 g1 h1  Rank 1 .. 8

                            File A..H

  56 57 58 59 60 61 62 63   a8 b8 c8 d8 e8 f8 g8 h8
  48 49 50 51 52 53 54 55   a7 b7 c7 d7 e7 f7 g7 h7
  40 41 42 43 44 45 46 47   a6 b6 c6 d6 e6 f6 g6 h6
  32 33 34 35 36 37 38 39   a5 b5 c5 d5 e5 f5 g5 h5
  24 25 26 27 28 29 30 31   a4 b4 c4 d4 e4 f4 g4 h4
  16 17 18 19 20 21 22 23   a3 b3 c3 d3 e3 f3 g3 h3
   8  9 10 11 12 13 14 15   a2 b2 c2 d2 e2 f2 g2 h2
   0  1  2  3  4  5  6  7   a1 b1 c1 d1 e1 f1 g1 h1  Rank 1 .. 8

                            File A..H

   0  1  2  3  4  5  6  7   a8 b8 c8 d8 e8 f8 g8 h8
   8  9 10 11 12 13 14 15   a7 b7 c7 d7 e7 f7 g7 h7
  16 17 18 19 20 21 22 23   a6 b6 c6 d6 e6 f6 g6 h6
  24 25 26 27 28 29 30 31   a5 b5 c5 d5 e5 f5 g5 h5
  32 33 34 35 36 37 38 39   a4 b4 c4 d4 e4 f4 g4 h4
  40 41 42 43 44 45 46 47   a3 b3 c3 d3 e3 f3 g3 h3
  48 49 50 51 52 53 54 55   a2 b2 c2 d2 e2 f2 g2 h2
  56 57 58 59 60 61 62 63   a1 b1 c1 d1 e1 f1 g1 h1  Rank 1 .. 8

                            File A..H
  Useful links:

    * http://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/physical.html
    * http://pages.cs.wisc.edu/~psilord/blog/data/chess-pages/nonsliding.html
    * http://www.dummies.com/how-to/content/naming-ranks-and-files-in-chess.html
*/


// i -> x,y
void CellToColRow( const int cell, int& col_, int& row_ )
{
    col_ = cell & 7;
    row_ = cell / 8;
}


uint8_t ColRowToRankFile( const int col, const int row )
{
    uint8_t rankfile = (row << 4) + col;
    return rankfile;
}


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

const char aFILE[8] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' }; // which Col
const char aRANK[8] = { '1', '2', '3', '4', '5', '6', '7', '8' }; // which Row

/** @param col

    Col:  0    1    2    3    4    5    6    7
    Mask: 0x80 0x40 0x20 0x10 0x08 0x04 0x02 0x01
    File: A    B    C    D    E    F    G    H

    A = 0x8080808080808080
    B = 0x4040404040404040
    C = 0x2020202020202020
    D = 0x1010101010101010
    E = 0x0808080808080808
    F = 0x0404040404040404
    G = 0x0202020202020202
    H = 0x0101010101010101

*/

// Map File to BitMask: // 1 << (8 - col)
const uint8_t aColMask[8] = {
     0x80 // 1
    ,0x40 // 2
    ,0x20 // 3
    ,0x10 // 4
    ,0x08 // 5
    ,0x04 // 6
    ,0x02 // 7
    ,0x01 // 8
};


bitboard_t BitBoardMakeFile( uint8_t col )
{
    bitboard_t t = (bitboard_t) aColMask[ col ]; // 1 << (8 - col)
    bitboard_t board = 0
        | t << 56
        | t << 48
        | t << 40
        | t << 32
        | t << 24
        | t << 16
        | t <<  8
        | t <<  0;
    return board;
}


/** @param row

    7 = 0xFF00000000000000
    6 = 0x00FF000000000000
    5 = 0x0000FF0000000000
    4 = 0x000000FF00000000
    3 = 0x00000000FF000000
    2 = 0x0000000000FF0000
    1 = 0x000000000000FF00
    0 = 0x00000000000000FF
*/
bitboard_t BitBoardMakeRank( uint8_t row )
{
    bitboard_t rank  = 0xFF; // rank
    bitboard_t board = rank << (8 * row);
    return board;
}


/** @param rankfile bit-packed rank and file
    00 = a1
    77 = h8
*/
bitboard_t BitBoardMakeLocation( uint8_t rankfile )
{
    uint8_t row = (rankfile >> 4) & 7; // rank
    uint8_t col = (rankfile >> 0) & 7; // file

    bitboard_t board = ((bitboard_t) aColMask[ col ]) << (8 * row);
    return board;
}


/** @param rankfile
    Makes diagonal: \
*/
bitboard_t BitBoardMakeDiagonalLeft( uint8_t rankfile )
{
    uint8_t row  = (rankfile >> 4) & 7; // rank
    uint8_t col  = (rankfile >> 0) & 7; // file
    uint8_t diag = row + col;
    bitboard_t board = 0x8040201008040201UL;

    // col + row = Left Diagonal = Total 14 diagonals
    // 789ABCDE
    // 6789ABCD
    // 56789ABC
    // 456789AB
    // 3456789A
    // 23456789
    // 12345678
    // 01234567
    //
    // ABCDEFGH File
    switch( diag )
    {
        case 0x0: board = 0x0000000000000080UL; break; // >> 7*8 // same as Right, but bytes mirrored
        case 0x1: board = 0x0000000000008040UL; break; // >> 6*8
        case 0x2: board = 0x0000000000804020UL; break; // >> 5*8
        case 0x3: board = 0x0000000080402010UL; break; // >> 4*8
        case 0x4: board = 0x0000008040201008UL; break; // >> 3*8
        case 0x5: board = 0x0000804020100804UL; break; // >> 2*8
        case 0x6: board = 0x0080402010080402UL; break; // >> 1*8
        case 0x7: board = 0x8040201008040201UL; break; // Above is >> 8*, Below is << 8*
        case 0x8: board = 0x4020100804020100UL; break; // << 1*8
        case 0x9: board = 0x2010080402010000UL; break; // << 2*8
        case 0xA: board = 0x1008040201000000UL; break; // << 3*8
        case 0xB: board = 0x0804020100000000UL; break; // << 4*8
        case 0xC: board = 0x0402010000000000UL; break; // << 5*8
        case 0xD: board = 0x0201000000000000UL; break; // << 6*8
        case 0xE: board = 0x0100000000000000UL; break; // << 7*8
    }
    return board;
}


/** @param rankfile
    Make diagonal: /
*/
bitboard_t BitBoardMakeDiagonalRight( uint8_t rankfile )
{
    uint8_t row  = (rankfile >> 4) & 7; // rank
    uint8_t col  = (rankfile >> 0) & 7; // file
    uint8_t diag = (7 - row) + col;
    bitboard_t board = 0x0102040810204080UL;

    // Total of 14 diagonals
    // 01234567   7 -> 0
    // 12345678   6 -> 1
    // 23456789   5 -> 2
    // 3456789A   4 -> 3
    // 456789AB   3 -> 4
    // 56789ABC   2 -> 5
    // 6789ABCD   1 -> 6
    // 789ABCDE   0 -> 7 rank
    //
    // ABCDEFGH File
    switch( diag )
    {
        case 0x0: board = 0x8000000000000000UL; break; // << 7*8
        case 0x1: board = 0x4080000000000000UL; break; // << 6*8
        case 0x2: board = 0x2040800000000000UL; break; // << 5*8
        case 0x3: board = 0x1020408000000000UL; break; // << 4*8
        case 0x4: board = 0x0810204080000000UL; break; // << 3*8
        case 0x5: board = 0x0408102040800000UL; break; // << 2*8
        case 0x6: board = 0x0204081020408000UL; break; // << 1*8
        case 0x7: board = 0x0102040810204080UL; break; // Above is << *8, Below is >> *8
        case 0x8: board = 0x0001020408102040UL; break; // >> 1*8
        case 0x9: board = 0x0000010204081020UL; break; // >> 2*8
        case 0xA: board = 0x0000000102040810UL; break; // >> 3*8
        case 0xB: board = 0x0000000001020408UL; break; // >> 4*8
        case 0xC: board = 0x0000000000010204UL; break; // >> 5*8
        case 0xD: board = 0x0000000000000102UL; break; // >> 6*8
        case 0xE: board = 0x0000000000000001UL; break; // >> 7*8
    }

    return board;
}


bitboard_t BitBoardMovesBlackPawn( uint8_t rankfile )
{
    uint8_t row = (rankfile >> 4) & 7; // rank
    uint8_t col = (rankfile >> 0) & 7; // file

    bitboard_t origin = BitBoardMakeLocation( rankfile );
    bitboard_t board  = origin >> 8;

    if( row == 6 ) // can move 2 tiles if haven't moved
        board |= (board >> 8); // Optimization: Don't need: & rank8

    return board & ~origin;
}


bitboard_t BitBoardMovesWhitePawn( uint8_t rankfile )
{
    uint8_t row = (rankfile >> 4) & 7; // rank
    uint8_t col = (rankfile >> 0) & 7; // file

    bitboard_t origin = BitBoardMakeLocation( rankfile );
    bitboard_t board  = origin << 8;

    if( row == 1 ) // can move 2 tiles if haven't moved
        board |= (board << 8);

    return board & ~origin;
}


bitboard_t BitBoardMovesColorRook( uint8_t rankfile )
{
    uint8_t row = (rankfile >> 4) & 7; // rank
    uint8_t col = (rankfile >> 0) & 7; // file

    bitboard_t origin = BitBoardMakeLocation( rankfile );
    bitboard_t file   = BitBoardMakeFile( col );
    bitboard_t rank   = BitBoardMakeRank( row );

    bitboard_t board = file | rank;
    return board & ~origin;
}


bitboard_t BitBoardMovesColorKnight( uint8_t rankfile )
{
    /* 8 possible moves

      .C.B.
      D. .A
      . x .
      E. .H
      .F.G.

    */
    bitboard_t origin = BitBoardMakeLocation( rankfile );

    bitboard_t fileA  = ~BitBoardMakeFile( 0 );
    bitboard_t fileB  = ~BitBoardMakeFile( 1 );
    bitboard_t fileG  = ~BitBoardMakeFile( 6 );
    bitboard_t fileH  = ~BitBoardMakeFile( 7 );

    bitboard_t left2  = (origin & fileA & fileB) << 2;
    bitboard_t right2 = (origin & fileG & fileH) >> 2;

    bitboard_t above2 = origin << 16;
    bitboard_t above1 = origin <<  8;
    bitboard_t below1 = origin >>  8;
    bitboard_t below2 = origin >> 16;

    bitboard_t A = (right2        ) << 8; // this is simply <<  6
    bitboard_t B = (above2 & fileH) >> 1; // this is simply << 15
    bitboard_t C = (above2 & fileA) << 1; // this is simply << 17
    bitboard_t D = (left2         ) << 8; // this is simply << 10
    bitboard_t E = (left2         ) >> 8; // this is simply >>  6
    bitboard_t F = (below2 & fileA) << 1; // this is simply >> 15
    bitboard_t G = (below2 & fileH) >> 1; // this is simply >> 17
    bitboard_t H = (right2        ) >> 8; // this is simply >> 10

    bitboard_t board = A | B | C | D | E | F | G | H;
    return board & ~origin;
}


bitboard_t BitBoardMovesColorBishop( uint8_t rankfile )
{
    uint8_t row = (rankfile >> 4) & 7; // rank
    uint8_t col = (rankfile >> 0) & 7; // file

    bitboard_t origin = BitBoardMakeLocation( rankfile );
    bitboard_t diagL  = BitBoardMakeDiagonalLeft ( rankfile );
    bitboard_t diagR  = BitBoardMakeDiagonalRight( rankfile );

    bitboard_t board = diagL | diagR;
    return board & ~origin;
}


bitboard_t BitBoardMovesColorQueen( uint8_t rankfile )
{
    uint8_t row = (rankfile >> 4) & 7; // rank
    uint8_t col = (rankfile >> 0) & 7; // file

    bitboard_t origin = BitBoardMakeLocation( rankfile );
    bitboard_t file   = BitBoardMakeFile( col );
    bitboard_t rank   = BitBoardMakeRank( row );
    bitboard_t diagL  = BitBoardMakeDiagonalLeft ( rankfile );
    bitboard_t diagR  = BitBoardMakeDiagonalRight( rankfile );

    bitboard_t board = file | rank | diagL | diagR;
    return board & ~origin; // don't include current position
}


bitboard_t BitBoardMovesColorKing( uint8_t rankfile )
{
    bitboard_t fileA  = ~BitBoardMakeFile( 0 );
    bitboard_t fileH  = ~BitBoardMakeFile( 7 );

    bitboard_t origin = BitBoardMakeLocation( rankfile );
    bitboard_t left   = (origin & fileA) << 1;
    bitboard_t right  = (origin & fileH) >> 1;

    bitboard_t above  = origin << 8;
    bitboard_t below  = origin >> 8;

    bitboard_t ul     = left << 8;
    bitboard_t bl     = left >> 8;

    bitboard_t ur     = right << 8;
    bitboard_t br     = right >> 8;

    bitboard_t board = 0
        | ul | above | ur
        |left|   0   | right
        | bl | below | br;

    return board & ~origin;
}


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
