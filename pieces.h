enum Pieces_e
{
    PIECE_PAWN  , // 0
    PIECE_ROOK  , // 1
    PIECE_KNIGHT, // 2
    PIECE_BISHOP, // 3
    PIECE_QUEEN , // 4
    PIECE_KING  , // 5
    NUM_PIECES  ,
    PIECE_EMPTY , // 7
};

const char aPIECES[] = "PRNBQK? prnbqk? "; // Uppercase = White, Lowercase = Black

enum Player_e
{
    PLAYER_WHITE,
    PLAYER_BLACK,
    NUM_PLAYERS
};

const char aPLAYERS[] = "WB "; // Player 3 is to display empty cell

// http://www.stmintz.com/ccc/index.php?id=424966
const int MAX_POSITION_MOVES = 256; // Theoritical max moves from any position is ~218
