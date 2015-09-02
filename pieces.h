enum Pieces_e
{
    PIECE_EMPTY , // 0
    PIECE_PAWN  , // 1
    PIECE_ROOK  , // 2
    PIECE_KNIGHT, // 3
    PIECE_BISHOP, // 4
    PIECE_QUEEN , // 5
    PIECE_KING  , // 6
    NUM_PIECES
};

const char aPIECES[] = " PRNBQK? prnbqk?"; // Uppercase = White, Lowercase = Black

enum Player_e
{
    PLAYER_WHITE,
    PLAYER_BLACK,
    NUM_PLAYERS
};

const char aPLAYERS[] = "WB "; // Player 3 is to display empty cell

// http://www.stmintz.com/ccc/index.php?id=424966
const int MAX_MOVES = 256; // Theoritical max moves from any position is ~218
