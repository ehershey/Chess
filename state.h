const char CELL_W  [] = "\x1B[30;47m"; // Black on White
const char CELL_B  [] = "\x1B[37;40m"; // White on Black
const char CELL_EOL[] = "\x1B[0m";
const int  FANCY  = 64 * 2;

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

    void MakePrettyBoard( int iPlayer, char board[ FANCY ] )
    {
        for( int iPiece = PIECE_PAWN; iPiece < NUM_PIECES; iPiece++ )
        {
            bitboard_t bits = _aBoards[ iPiece ];
            bitboard_t temp ;

            char       player = aPLAYERS[ iPlayer ];
            char       piece  = aPIECES [ iPiece  ];

            // Enumerate though all bits, filling in the board            
            char *p = board;

            for( int y = 7; y >= 0; y-- )
            {
                temp = bits >> (8 * y);
                for( int x = 7; x >= 0; x-- )
                {
                    if( (temp >> x) & 1 )
                    {
                        *p++ = player;
                        *p++ = piece ;
                    }
                    else
                        p += 2;
                }
            }
        }
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

    char* PrettyPrintBoard()
    {
        static char board[ FANCY ];
        memset( board, ' ', FANCY );

        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            StateBitBoard_t *pState = &_player[ iPlayer ];
            pState->MakePrettyBoard( iPlayer, board );
        }

        for( int cell = 0; cell < 64; cell++ )
        {
            printf( "%s%c%c"
                , (cell + (cell/8)) & 1 ? CELL_B : CELL_W
                , board[ cell*2 + 0 ]
                , board[ cell*2 + 1 ]
            );
            if( cell && ((cell & 7) == 7) )
                printf( "%s\n", CELL_EOL );
        }

        return board;
    }
};

