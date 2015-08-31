struct StateBitBoard_t
{
    bitboard_t _board[ NUM_PIECES ];
};

struct State_t
{
    StateBitBoard_t _player[ NUM_PLAYERS ];
    int             _flags;
    int             _turn;

    void Init()
    {
        _flags = 0;
        _turn  = 1;

        StateBitBoard_t *pBoard;

        pBoard = &_player[ PLAYER_WHITE ];
        pBoard->_board[ PIECE_EMPTY  ] = BitBoardMakeWhiteSquares();
        pBoard->_board[ PIECE_PAWN   ] = BitBoardInitWhitePawn  ();
        pBoard->_board[ PIECE_ROOK   ] = BitBoardInitWhiteRook  ();
        pBoard->_board[ PIECE_KNIGHT ] = BitBoardInitWhiteKnight();
        pBoard->_board[ PIECE_BISHOP ] = BitBoardInitWhiteBishop();
        pBoard->_board[ PIECE_QUEEN  ] = BitBoardInitWhiteQueen ();
        pBoard->_board[ PIECE_KING   ] = BitBoardInitWhiteKing  ();

        pBoard = &_player[ PLAYER_BLACK ];
        pBoard->_board[ PIECE_EMPTY  ] = BitBoardMakeBlackSquares();
        pBoard->_board[ PIECE_PAWN   ] = BitBoardInitBlackPawn  ();
        pBoard->_board[ PIECE_ROOK   ] = BitBoardInitBlackRook  ();
        pBoard->_board[ PIECE_KNIGHT ] = BitBoardInitBlackKnight();
        pBoard->_board[ PIECE_BISHOP ] = BitBoardInitBlackBishop();
        pBoard->_board[ PIECE_QUEEN  ] = BitBoardInitBlackQueen ();
        pBoard->_board[ PIECE_KING   ] = BitBoardInitBlackKing  ();
    }

    void Dump()
    {
        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            StateBitBoard_t *pBoard = &_player[ iPlayer ];
            
            for( int piece = PIECE_PAWN; piece < NUM_PIECES; piece++ )
            {
                printf( "= %c%c = %08X%08X\n", aPLAYERS[ iPlayer ], aPIECES[ piece ]
                    , (pBoard->_board[ piece ] >> 32) & 0xFFFFFFFF
                    , (pBoard->_board[ piece ]      ) & 0xFFFFFFFF
                );
                BitBoardPrint( pBoard->_board[ piece ], aPIECES[ piece ] );
                printf( "\n" );
            }
        }
    }
};

