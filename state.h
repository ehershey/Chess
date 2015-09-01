// Pretty Print
    char CELL_W  [] = "\x1B[30;47m"; // Black on White
    char CELL_B  [] = "\x1B[37;40m"; // White on Black
    char CELL_EOL[] = "\x1B[0m";
    // Version 1: char[8x8*2] // 128 bytes = 2 bytes/cell
    // Version 2: char[8*8]   //  64 bytes = 1 byte/cell -- encode black as 8+piece
    // Version 3: int32[8]    //  32 bytes = 1 nibble/cell
    const int  CELLS_SIZE = 64;

struct PrettyPrintBoard_t
{
    char _cells[ CELLS_SIZE ]; // Version 2: 1 byte/cell
    //uint32_t _cells[ 8 ];    // Version 3: 1 nibble/cell // 8 rows of 4 bits/piece

    void Init()
    {
        memset( _cells, PIECE_EMPTY, sizeof( _cells ) );
    }

    void Print( bool bPrintRankFile = true )
    {
        char *p = _cells;

        for( int iCell = 0; iCell < 64; iCell++ )
        {
            int  iPiece  = *p & 7;
            int  iPlayer = *p / 8 + 2*(iPiece == PIECE_EMPTY); // if piece empty, don't display player

            char cPlayer = aPLAYERS[ iPlayer ];
            char cPiece  = aPIECES [ iPiece  ];

            if (bPrintRankFile && ((iCell & 7) == 0))
                printf( "%c ", aRANK[ 7 - iCell/8 ] );

            printf( "%s%c%c"
                , (iCell + ((iCell/8)) & 1) ? CELL_B : CELL_W
                , cPlayer
                , cPiece
            );

            if (iCell && ((iCell & 7) == 7))
                printf( "%s\n", CELL_EOL );

            p++;
        }

        if (bPrintRankFile )
        {
             printf( "  " );
             for( int x = 0; x < 8; x++ )
               printf( "%c ", aFILE[ x ] );
        }
    }
};


struct StateBitBoard_t
{
    bitboard_t _aBoards[ NUM_PIECES ];

    void Dump( int iPlayer )
    {
        for( int iPiece = PIECE_PAWN; iPiece < NUM_PIECES; iPiece++ )
        {
            bitboard_t board  = _aBoards[ iPiece ];
            uint32_t   b1     = board >> 32;
            uint32_t   b0     = board >>  0;

            char       player = aPLAYERS[ iPlayer ];
            char       piece  = aPIECES [ iPiece  ];

            printf( "= %c%c = %08X%08X\n", player, piece, b1, b0 );
            BitBoardPrint( board, piece );
            printf( "\n" );
        }
    }

    void BitBoardToPrettyPrint( int iPlayer, PrettyPrintBoard_t *board_ )
    {
        for( int iPiece = PIECE_PAWN; iPiece < NUM_PIECES; iPiece++ )
        {
            bitboard_t bits = _aBoards[ iPiece ];
            bitboard_t mask = 0x8000000000000000UL;

            // Enumerate though all bits, filling in the board
            char *p = board_->_cells;

            for( int iCell = 0; iCell < 64; iCell++, p++, mask >>= 1 )
                if( bits & mask )
                    *p = iPiece + (8*iPlayer);
        }
    }

    bitboard_t GetBoardAllPieces()
    {
        bitboard_t board = 0;

        for( int iPiece = PIECE_PAWN; iPiece < NUM_PIECES; iPiece++ )
            board  |= _aBoards[ iPiece ];

        return board;
    }

};

struct State_t
{
    StateBitBoard_t _player[ NUM_PLAYERS ];
    uint16_t        _turn ;
    uint8_t         _flags;
    uint8_t         _from ; // ROWCOL
    uint8_t         _to   ; // ROWCOL

    void Init()
    {
        _flags = 0;
        _turn  = 1;
        _from  = 0;
        _to    = 0;

        StateBitBoard_t *pState;

        pState = &_player[ PLAYER_WHITE ];
        pState->_aBoards[ PIECE_EMPTY  ] = BitBoardMakeWhiteSquares();
        pState->_aBoards[ PIECE_PAWN   ] = BitBoardInitWhitePawn  ();
        pState->_aBoards[ PIECE_ROOK   ] = BitBoardInitWhiteRook  ();
        pState->_aBoards[ PIECE_KNIGHT ] = BitBoardInitWhiteKnight();
        pState->_aBoards[ PIECE_BISHOP ] = BitBoardInitWhiteBishop();
        pState->_aBoards[ PIECE_QUEEN  ] = BitBoardInitWhiteQueen ();
        pState->_aBoards[ PIECE_KING   ] = BitBoardInitWhiteKing  ();

        pState = &_player[ PLAYER_BLACK ];
        pState->_aBoards[ PIECE_EMPTY  ] = BitBoardMakeBlackSquares();
        pState->_aBoards[ PIECE_PAWN   ] = BitBoardInitBlackPawn  ();
        pState->_aBoards[ PIECE_ROOK   ] = BitBoardInitBlackRook  ();
        pState->_aBoards[ PIECE_KNIGHT ] = BitBoardInitBlackKnight();
        pState->_aBoards[ PIECE_BISHOP ] = BitBoardInitBlackBishop();
        pState->_aBoards[ PIECE_QUEEN  ] = BitBoardInitBlackQueen ();
        pState->_aBoards[ PIECE_KING   ] = BitBoardInitBlackKing  ();
    }

    void Dump()
    {
        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            StateBitBoard_t *pState = &_player[ iPlayer ];
            pState->Dump( iPlayer );
        }
    }

    void PrettyPrintBoard( bool bPrintRankFile = true )
    {
        PrettyPrintBoard_t board;
        board.Init();

        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            StateBitBoard_t *pState = &_player[ iPlayer ];
            pState->BitBoardToPrettyPrint( iPlayer, &board );
        }

        board.Print( bPrintRankFile );

    }
};

