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

//printf( "raw: %s\n", fen );

        while (*fen == ' ')
            fen++;

        State_t    state;
        state.ClearBoard();
        state.ResetFlags();

/*
    https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

FEN: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
*/
        bitboard_t origin = 0x8000000000000000ull;
//int debug = 0;
        // SECTION_BOARD
        while (*fen && (*fen != ' '))
        {
//printf( "[%2d]: '%c'\n", debug++, *fen );
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
//printf( "Piece: %c  Player: %d\n", aPIECES[ iPiece ], iPlayer );
                state._player[ iPlayer ]._aBoards[ iPiece & PIECE_MASK ] |= origin;
                origin >>= 1;
            }

            fen++;
        }

        if( origin )
        {
//printf( "org0: %08X\n", (uint32_t) (origin >> 32) );
//printf( "org1: %08X\n", (uint32_t) (origin >>  0) );
            printf( "Invalid FEN input: bad board state\n" );
            return;
        }

        while (*fen == ' ')
            fen++;

        // SECTION_COLOR_TO_PLAY_NEXT
        while (*fen && (*fen != ' '))
        {
//printf( "C2PN: %c\n", *fen );
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
        state._bFlags &= ~(STATE_CAN_CASTLE_W | STATE_CAN_CASTLE_B);
        while (*fen && (*fen != ' '))
        {
//printf( "CASTLE: %c\n", *fen );
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
//printf( "EP: %c\n", *fen );
            switch( *fen )
            {
                case '-': break; // no pawn moved
                default: // FIXME: last EP location
                    printf( "FIXME: FEN: Ignoring last E.P location\n" );
                    break;
            }
            fen++;
        }

        // Check if white pawns on home row
        // Check if black pawns on home row

        while (*fen == ' ')
            fen++;

        // SECTION_NUM_HALF_MOVES_LAST_CAPTURE_OR_PAWN_MOVE
        while (*fen && (*fen != ' '))
        {
//printf( "HALF: %c\n", *fen );
            // FIXME: Half Moves
            fen++;
        }

        while (*fen == ' ')
            fen++;

        // SECTION_MOVE_NUM
//printf( "MOVE: %s\n", fen );

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


/*
  Who   - Who can read/write?
  When  - When can the data be read or written?
        - When does the caller need the results?
        - When do other processes need the results?
  What  - What data really needs to be read / written ?
        - What are the contraints?
          - Latency?
          - Throughput?
          - To data?
          - To tranformation?
          - To ordering?
          - To global guarantees?
          - To local guarantees?
  How   - How is the data stored? Accessed?
          How will the data be transformed? -> What are the operations?
  Why   - Why do you need to read/write the data?
  Where -

Per Thread Queue?
  - Insert
  - Delete
  - Search


Atomic integers:
   push_count = 0
   pop_count  = 0

Current count = push_count - pop_count

Insert (Append at End)
Delete (Node can be anywhere)
Search (needle), return sub-list


Insert Thread:
   Insert count > 0?
   - No, Wait, goto 1
   - Yes
     Add to source
     Atomic push_count++

Delete Thread:
   Delete count > 0?
   - No, Wait, goto 1
   - Yes
     Deref id
     Mark source deleted
     Atomic pop_count++

Search Thread:
   Search Command
   Deref id
   Deleted?
   - Yes, Skip
   - No, Match criteria?
     - No, goto skip
     - Yes, Append to result

Now is not meaningfull in concurrent system

Reference:
  http://cellperformance.beyond3d.com/articles/public/concurrency_rabit_hole.pdf


Queue Operations:
  IsEmpty()
  Push()
  Pop()

Move Queue
1. Single Queue
   Thread 0 writes
   Thread N reads

2. Queue per Core
   Thread 0 pushes job X onto core Y
*/

const int MAX_THREADS = 8;
const int MAX_POOL_MOVES = 4096*MAX_SEARCH_MOVES; // Max is 5949~6349

#ifdef _OPENMP
    omp_lock_t aLockPool[ MAX_THREADS ];
#endif // OPENMP

uint32_t       nMovePool  [ MAX_THREADS ];
SearchState_t *aMovePool  [ MAX_THREADS ]; // Array of pointers to move pools

#ifdef _OPENMP
void PushState( int iThread, SearchState_t search )
{
    search._state._nDepth      = 0;
    search._state._iParent     = 0;
    search._state._iFirstChild = 1;
    search._state._nChildren   = 1; // TODO: FIXME: SearchState _nMoves

    omp_set_lock  ( &aLockPool[ iThread ] );

    // #pragma OMP atomic
        int iMove = ++nMovePool[ iThread ];
                      aMovePool[ iThread ][ iMove ] = search;

    omp_unset_lock( &aLockPool[ iThread ] );
}
#endif // OPENMP

