enum Pieces_e
{
    PIECE_EMPTY ,
    PIECE_PAWN  , 
    PIECE_ROOK  ,
    PIECE_KNIGHT,
    PIECE_BISHOP,
    PIECE_QUEEN ,
    PIECE_KING  ,
    NUM_PIECES
};

const char aPIECES[] = " PRNBQK";

enum Player_e
{
    PLAYER_WHITE,
    PLAYER_BLACK,
    NUM_PLAYERS
};

const char aPLAYERS[] = "WB "; // Player 3 is to display empty cell
