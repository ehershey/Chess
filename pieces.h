enum Pieces_e
{
    PIECE_PAWN   , // 0
    PIECE_ROOK   , // 1
    PIECE_KNIGHT , // 2
    PIECE_BISHOP , // 3
    PIECE_QUEEN  , // 4
    PIECE_KING   , // 5
    PIECE_EMPTY  ,// Optimization: switch from very last byte (=7) to save 1 byte in aPIECES; since we need +1 for NULL terminator
    PIECE_UNKNOWN,

    NUM_PIECES   = PIECE_EMPTY,
    PIECE_MASK   = 7,

// Aliases
    WHITE_PAWN   = PIECE_PAWN  ,
    WHITE_ROOK   = PIECE_ROOK  ,
    WHITE_KNGIHT = PIECE_KNIGHT,
    WHITE_BISHOP = PIECE_BISHOP,
    WHITE_QUEEN  = PIECE_QUEEN ,
    WHITE_KING   = PIECE_KING  ,
    WHITE_EMPTY  = PIECE_EMPTY ,

    BLACK_PAWN   = PIECE_PAWN   + 8,
    BLACK_ROOK   = PIECE_ROOK   + 8,
    BLACK_KNGIHT = PIECE_KNIGHT + 8,
    BLACK_BISHOP = PIECE_BISHOP + 8,
    BLACK_QUEEN  = PIECE_QUEEN  + 8,
    BLACK_KING   = PIECE_KING   + 8,
    BLACK_EMPTY  = PIECE_EMPTY  + 8,
};

// 16 chars including the NULL, the '?' is PIECE_UKNOWN, the end NULLis BLACK_PIECE_UNKNOWN
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
