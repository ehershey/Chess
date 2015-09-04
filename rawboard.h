 // Pretty Print
    char CELL_W  [] = "\x1B[30;47m"; // Black on White
    char CELL_B  [] = "\x1B[37;40m"; // White on Black
    char CELL_EOL[] = "\x1B[0m";
    // Version 1: char[8x8*2] // 128 bytes = 2 bytes/cell
    // Version 2: char[8*8]   //  64 bytes = 1 byte/cell -- encode black as 8+piece
    // Version 3: int32[8]    //  32 bytes = 1 nibble/cell
    const int  CELLS_SIZE = 64;

struct RawBoard_t
{
//    enum 
//    {
//        CELLS_SIZE = 64
//    };

    char _cells[ CELLS_SIZE ]; // Version 2: 1 byte/cell
    //uint32_t _cells[ 8 ];    // Version 3: 1 nibble/cell // 8 rows of 4 bits/piece

    void Init()
    {
        memset( _cells, PIECE_EMPTY, sizeof( _cells ) );
    }

    void PrintFile( int nSpacer = 1 )
    {
        printf( "  " );
        for( int x = 0; x < 8; x++ )
            printf( "%c%*s", aFILE[ x ], nSpacer, "" );
        printf( "\n" );
    }

    void PrintCompact( bool bPrintRankFile = true )
    {
        char *p = _cells;

        for( int iCell = 0; iCell < 64; iCell++, p++ )
        {
            char iCol    = iCell & 7;
            char iRow    = iCell / 8;

            int  iPiece  = *p & 0xF;
            char cPiece  = aPIECES [ iPiece  ];
            bool bOdd    = ((iCell + iRow) & 1);

            if (bPrintRankFile && (iCol == 0))
                printf( "%c ", aRANK[ 7 - iCell/8 ] );

            if( (iPiece & 7) == PIECE_EMPTY )
                printf( "%c", " ."[ bOdd ] );
            else
                printf( "%c", cPiece );

            if (iCell && (iCol == 7))
                printf( "\n" );
        }

        if ( bPrintRankFile )
            PrintFile( 0 );
    }

    void PrintPretty( bool bPrintRankFile = true )
    {
        char *p = _cells;

        for( int iCell = 0; iCell < 64; iCell++, p++ )
        {
            char iCol    = iCell & 7;
            char iRow    = iCell / 8;

            int  iPiece  = *p & 7;
            int  iPlayer = *p / 8 + 2*(iPiece == PIECE_EMPTY); // if piece empty, don't display player -> player 3 "blank"

            char cPlayer = aPLAYERS[ iPlayer ];
            char cPiece  = aPIECES [ iPiece  ];

            if (bPrintRankFile && (iCol == 0))
                printf( "%c ", aRANK[ 7 - iCell/8 ] );

            printf( "%s%c%c"
                , ((iCell + iRow) & 1) ? CELL_B : CELL_W
                , cPlayer
                , cPiece
            );

            if (iCell && (iCol == 7))
                printf( "%s\n", CELL_EOL );
        }

        if ( bPrintRankFile )
            PrintFile();
    }
};

