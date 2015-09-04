enum Pieces_e
{
    PIECE_PAWN  , // 0
    PIECE_ROOK  , // 1
    PIECE_KNIGHT, // 2
    PIECE_BISHOP, // 3
    PIECE_QUEEN , // 4
    PIECE_KING  , // 5
    NUM_PIECES  ,
    PIECE_EMPTY=6,// Optimization: switch from very last byte (=7) to save 1 byte in aPIECES; since we need +1 for NULL terminator
};

// 16 chars including the NULL, the '?' is PIECE_NOT_USED=7
const char aPIECES[] = "PRNBQK ?prnbqk "; // Uppercase = White, Lowercase = Black

enum Player_e
{
    PLAYER_WHITE,
    PLAYER_BLACK,
    NUM_PLAYERS
};

const char aPLAYERS[] = "WB "; // Player 3 is to display empty cell

// http://www.stmintz.com/ccc/index.php?id=424966
const int MAX_POSITION_MOVES = 256; // Theoritical max moves from any position is ~218

// Technically multiple queens could cover every square on the board
// bump to 64 when needed
// See: BitBoardToRankFileAllMoves()
const int MAX_PIECE_MOVES = 32;
