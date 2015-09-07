// http://chess.stackexchange.com/questions/4113/longest-chess-game-possible-maximum-moves
const int MAX_GAME_MOVES = 4096; // Max is 5949~6349

struct ChessGame_t
{
    int     _nMoves;
    State_t _aMoves[ MAX_GAME_MOVES ];

    void Reset()
    {
        _nMoves = 0;
        _aMoves[ _nMoves ].Zero();
    }

    void Init()
    {
        _nMoves = 0;
        _aMoves[ _nMoves ].Init();
    }

    void Clear()
    {
        _aMoves[ _nMoves ].ClearBoard();
    }

    void Edit( int iPlayer, int iPiece, uint8_t nDstRF )
    {
        _aMoves[ _nMoves ].DelPiece( nDstRF );
        _aMoves[ _nMoves ].AddPiece( iPlayer, iPiece, nDstRF );
    }

    void InputFEN( const char *fen )
    {
        if((strncmp( fen, "FEN", 3) == 0)
        || (strncmp( fen, "fen", 3) == 0))
            fen += 3;

        while (*fen == ' ')
            fen++;

        State_t    state;
        state.ClearBoard();
        state.ResetFlags();

        // https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
        // FEN: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
        bitboard_t origin = 0x8000000000000000ull;

        // SECTION_BOARD
        while (*fen && (*fen != ' '))
        {
            int iPiece = PIECE_UNKNOWN;
            switch( *fen )
            {
                // And this is why C++  sucks ... can't case PIECE[ WHITE_PAWN ], when we _already_ have a const char PIECES[...] = "PRNBQK";
                case 'P': iPiece = WHITE_PAWN  ; break; // aPIECES[ WHITE_PAWN   ]
                case 'R': iPiece = WHITE_ROOK  ; break; // aPIECES[ WHITE_ROOK   ]
                case 'N': iPiece = WHITE_KNIGHT; break; // aPIECES[ WHITE_KNIGHT ]
                case 'B': iPiece = WHITE_BISHOP; break; // aPIECES[ WHITE_BISHOP ]
                case 'Q': iPiece = WHITE_QUEEN ; break; // aPIECES[ WHITE_QUEEN  ]
                case 'K': iPiece = WHITE_KING  ; break; // aPIECES[ WHITE_KING   ]

                case 'p': iPiece = BLACK_PAWN  ; break; // aPIECES[ BLACK_PAWN   ]
                case 'r': iPiece = BLACK_ROOK  ; break; // aPIECES[ BLACK_ROOK   ]
                case 'n': iPiece = BLACK_KNIGHT; break; // aPIECES[ BLACK_KNIGHT ]
                case 'b': iPiece = BLACK_BISHOP; break; // aPIECES[ BLACK_BISHOP ]
                case 'q': iPiece = BLACK_QUEEN ; break; // aPIECES[ BLACK_QUEEN  ]
                case 'k': iPiece = BLACK_KING  ; break; // aPIECES[ BLACK_KING   ]

                // Skip this many squares for the next input
                case '1': origin >>= 1; break;
                case '2': origin >>= 2; break;
                case '3': origin >>= 3; break;
                case '4': origin >>= 4; break;
                case '5': origin >>= 5; break;
                case '6': origin >>= 6; break;
                case '7': origin >>= 7; break;
                case '8': origin >>= 8; break;
                case '/': break; // NOP

                default:
                    printf( "Invalid FEN input: bad piece for board setup\n" );
                    return;
            }

            if (iPiece != PIECE_UNKNOWN)
            {
                int iPlayer = iPiece / 8;
                state._player[ iPlayer ]._aBoards[ iPiece & PIECE_MASK ] |= origin;
                origin >>= 1;
            }

            fen++;
        }

        if( origin )
        {
            printf( "Invalid FEN input: bad board state\n" );
            return;
        }

        while (*fen == ' ')
            fen++;

        // SECTION_COLOR_TO_PLAY_NEXT
        while (*fen && (*fen != ' '))
        {
            switch( *fen )
            {
                case 'w': state.SetColorPlayer( PLAYER_WHITE ); break;
                case 'b': state.SetColorPlayer( PLAYER_BLACK ); break;
                default:
                    printf( "Invalid FEN input: bad color to play next\n" );
                    return;
            }
            fen++;
        }

        while (*fen == ' ')
            fen++;

        // SECTION_CASTLE_INFO
        state._bFlags &= ~STATE_CAN_CASTLE_MASK;
        while (*fen && (*fen != ' '))
        {
            switch( *fen )
            {
                case 'K': state._bFlags |= STATE_CAN_CASTLE_WK_SIDE; break;
                case 'Q': state._bFlags |= STATE_CAN_CASTLE_WQ_SIDE; break;
                case 'k': state._bFlags |= STATE_CAN_CASTLE_BK_SIDE; break;
                case 'q': state._bFlags |= STATE_CAN_CASTLE_BQ_SIDE; break;
                default:
                    printf( "Invalid FEN input: bad castle information\n" );
                    return;
            }
            fen++;
        }

        while (*fen == ' ')
            fen++;

        // SECTION_EP
        // iPlayer = OwnerOfPawn( nRF )
        // TODO: set _bPawnsMoved[ iPlayer ] true
        while (*fen && (*fen != ' '))
        {
            switch( *fen )
            {
                case '-': break; // no pawn moved
            }
            fen++;
        }

        // Check if white pawns on home row
        // Check if black pawns on home row

        while (*fen && (*fen != ' '))
        {
            fen++;
        }

        // SECTION_NUM_HALF_MOVES_LAST_CAPTURE_OR_PAWN_MOVE
        while (*fen && (*fen != ' '))
        {
            fen++;
        }

        while (*fen == ' ')
            fen++;

        // SECTION_MOVE_NUM
        state._iParent = 0; // signal is first move in game

        _nMoves = atoi( fen );
        if (_nMoves < 1)
        {
            printf( "Invalid FEN input: bad move number\n" );
            return;
        }

        _nMoves--;
        _aMoves[ _nMoves ] = state;
    }

    bool MoveOrCapture( uint8_t nSrcRF, uint8_t nDstRF )
    {
        _aMoves[ _nMoves+1 ] = _aMoves[ _nMoves ];
        return _aMoves[ _nMoves+1 ].MoveOrCapture( nSrcRF, nDstRF );
    }

    void NextTurn()
    {
        _nMoves++;
        _aMoves[ _nMoves ].TogglePlayer();
//printf( "Moves[].Flags: %02X\n", _aMoves[ _nMoves]._bFlags );
    }

    int PlayerToPlayNext()
    {
//printf( "Moves: %d\n", _nMoves );
        return _aMoves[ _nMoves ].GetColorPlayer();
    }

    void Print( bool bAnsiOutput )
    {
        if( bAnsiOutput )
            PrintBoardPretty();
        else
            PrintBoardCompact();
    }

    void PrintBoardCompact()
    {
        _aMoves[ _nMoves ].PrintCompactBoard();
    }

    void PrintBoardPretty()
    {
        _aMoves[ _nMoves ].PrintPrettyBoard();
    }
};

const int MAX_THREADS = 8;

const int MAX_POOL_MOVES = 4096*64; // Max is 5949~6349
uint32_t nMovePool  [ MAX_THREADS ];
State_t *aMovePool  [ MAX_THREADS ]; // Array of pointers to move pools
