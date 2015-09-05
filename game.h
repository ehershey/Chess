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

    bool MoveOrCapture( uint8_t nSrcRF, uint8_t nDstRF )
    {
        return _aMoves[ _nMoves ].MoveOrCapture( nSrcRF, nDstRF );
    }

    void NextTurn()
    {
        _nMoves++;
        _aMoves[ _nMoves ] = _aMoves[ _nMoves-1 ];
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
        _aMoves[ _nMoves ].CompactPrintBoard();
    }

    void PrintBoardPretty()
    {
        _aMoves[ _nMoves ].PrettyPrintBoard();
    }
};

const int MAX_THREADS = 8;

const int MAX_POOL_MOVES = 4096*64; // Max is 5949~6349
uint32_t nMovePool  [ MAX_THREADS ];
State_t *aMovePool  [ MAX_THREADS ]; // Array of pointers to move pools
