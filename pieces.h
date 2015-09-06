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

    NUM_PIECES          = PIECE_EMPTY,
    PIECE_MASK          = 7,
    PIECE_PLAYER_OFFSET = 8, // piece / offset -> which player (color)

// Aliases
    WHITE_PAWN   = PIECE_PAWN   + (0 * PIECE_PLAYER_OFFSET),
    WHITE_ROOK   = PIECE_ROOK   + (0 * PIECE_PLAYER_OFFSET),
    WHITE_KNGIHT = PIECE_KNIGHT + (0 * PIECE_PLAYER_OFFSET),
    WHITE_BISHOP = PIECE_BISHOP + (0 * PIECE_PLAYER_OFFSET),
    WHITE_QUEEN  = PIECE_QUEEN  + (0 * PIECE_PLAYER_OFFSET),
    WHITE_KING   = PIECE_KING   + (0 * PIECE_PLAYER_OFFSET),
    WHITE_EMPTY  = PIECE_EMPTY  + (0 * PIECE_PLAYER_OFFSET),

    BLACK_PAWN   = PIECE_PAWN   + (1 * PIECE_PLAYER_OFFSET),
    BLACK_ROOK   = PIECE_ROOK   + (1 * PIECE_PLAYER_OFFSET),
    BLACK_KNGIHT = PIECE_KNIGHT + (1 * PIECE_PLAYER_OFFSET),
    BLACK_BISHOP = PIECE_BISHOP + (1 * PIECE_PLAYER_OFFSET),
    BLACK_QUEEN  = PIECE_QUEEN  + (1 * PIECE_PLAYER_OFFSET),
    BLACK_KING   = PIECE_KING   + (1 * PIECE_PLAYER_OFFSET),
    BLACK_EMPTY  = PIECE_EMPTY  + (1 * PIECE_PLAYER_OFFSET),

    _PAD_BITS    = 0xFF
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
