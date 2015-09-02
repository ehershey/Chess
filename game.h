// http://chess.stackexchange.com/questions/4113/longest-chess-game-possible-maximum-moves
const int MAX_GAME_MOVES = 4096; // Max is 5949~6349

struct ChessGame_t
{
    int     _nMoves;
    State_t _aMoves[ MAX_GAME_MOVES ];
};

const int MAX_THREADS = 8;

uint32_t nMovePool[ MAX_THREADS ];
State_t *aMovePool[ MAX_THREADS ]; // Array of points to move pools

