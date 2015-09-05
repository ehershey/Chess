struct StateBitBoard_t
{
    bitboard_t _aBoards[ NUM_PIECES ];

    void Zero()
    {
        memset( _aBoards, 0, sizeof( _aBoards );
    }

    void AddPiece( int iPiece, uint8_t nDstRF )
    {
        bitboard_t& board  = _aBoards[ iPiece ];
        bitboard_t  origin = BitBoardMakeLocation( nDstRF );
        board |= origin;
    }

    void DelPiece( uint8_t nDstRF )
    {
        bitboard_t  origin = BitBoardMakeLocation( nDstRF );

        for( int iPiece = PIECE_PAWN; iPiece < NUM_PIECES; iPiece++ )
        {
            bitboard_t& board  = _aBoards[ iPiece ];
            board &= ~origin;
        }
    }

    void Dump( int iPlayer )
    {
        for( int iPiece = PIECE_PAWN; iPiece < NUM_PIECES; iPiece++ )
        {
            bitboard_t board  = _aBoards[ iPiece ];
            uint32_t   b1     = board >> 32;
            uint32_t   b0     = board >>  0;

            char       player = aPLAYERS[ iPlayer ];
            char       piece  = aPIECES [ iPiece  ];

            printf( "= %c%c = %08X%08X\n", player, piece, b1, b0 );
            BitBoardPrint( board, piece );
            printf( "\n" );
        }
    }

    // Merge all the bitboards into a single raw board
    // Empty cells are NOT touched so it is safe to call this with both players
    void BitBoardsToRawBoard( int iPlayer, RawBoard_t *board_ )
    {
        for( int iPiece = PIECE_PAWN; iPiece < NUM_PIECES; iPiece++ )
        {
            bitboard_t bits = _aBoards[ iPiece ];
            bitboard_t mask = 0x8000000000000000ull;

            // Enumerate though all bits, filling in the board
            char *p = board_->_cells;

            for( int iCell = 0; iCell < 64; iCell++, p++, mask >>= 1 )
                if( bits & mask )
                    *p = iPiece + (8*iPlayer);
        }
    }

    bitboard_t GetBoardAllPieces()
    {
        bitboard_t board = 0;

        for( int iPiece = PIECE_PAWN; iPiece < NUM_PIECES; iPiece++ )
            board  |= _aBoards[ iPiece ];

        return board;
    }

    uint8_t GetPiece( const uint8_t rankfile )
    {
        bitboard_t board = BitBoardMakeLocation( rankfile );

        for( int iPiece = PIECE_PAWN; iPiece < NUM_PIECES; iPiece++ )
            if( _aBoards[ iPiece ] & board )
                return iPiece;

        return PIECE_EMPTY;
    }
};


struct Castle_t
{
    uint8_t bWhichSide; // 0=no castle, or assume non-zero: STATE_CAN_CASTLE_Q_SIDE STATE_CAN_CASTLE_K_SIDE
    uint8_t nNewKingRF; // must test if square empty
    uint8_t nNewRookRF; // must test if square empty
    uint8_t nOldRookRF;
};

struct Move_t
{
    uint8_t    iPlayer   ;
    uint8_t    iEnemy    ;

    // Location
    uint8_t    nSrcRF    ;
    uint8_t    nDstRF    ;

    uint8_t    iPlayerSrc; // which player
    uint8_t    iPlayerDst; // which player

    // Piece
    uint8_t    iPieceSrc ;
    uint8_t    iPieceDst ;
    uint8_t    iEnemySrc ;
    uint8_t    iEnemyDst ;

    // Board Location Mask
    bitboard_t bBoardSrc;
    bitboard_t bBoardDst;
};

/*
TODO:
    Also, do not allow the en passant flag when the en
    passant capture is not legal!  Setting this flag
    willy nilly after each double pawn push means that
    the position after 1.d4 d5 2.Nf3 comes out different
    from 1.Nf3 d5 2.d4.

    Subject: PGN upddate and revisions
    https://groups.google.com/forum/#!topic/rec.games.chess.computer/FtUoFFfNXfQ
*/


enum StateFlags_e
{
     STATE_WHICH_PLAYER      = (1 << 0) // 0=White 1=Black // Optimization: Could encode in _bMoveType, and use 4-bit of Piece type
    ,STATE_CHECK             = (1 << 1)
//    ,STATE_CHECKMATE         = (1 << 2)
//    ,STATE_STALEMATE         = (1 << 3)
    ,STATE_CAN_CASTLE_Q_SIDE = (1 << 3)
    ,STATE_CAN_CASTLE_K_SIDE = (1 << 4)

    ,STATE_CAN_CASTLE_MASK   = STATE_CAN_CASTLE_Q_SIDE | STATE_CAN_CASTLE_K_SIDE
};

enum MoveFlags_e
{
    // 4-bits: which piece was moved
     MOVE_PIECE_MASK        = 0x0F
    ,MOVE_PIECE_SHIFT       = 0

    // 15 different states
    // http://chessprogramming.wikispaces.com/Encoding+Moves
    // 4-bit code: promotion capture special1 special0
    ,MOVE_FLAGS_MASK        = 0xF0
    ,MOVE_FLAGS_SHIFT       = 4
    ,MOVE_NORMAL            = (0 << MOVE_FLAGS_SHIFT) // special0
    ,MOVE_NORMAL_MASK       = (1 << MOVE_FLAGS_SHIFT) // special0 = 0 move without capture
    ,MOVE_PAWN_DOUBLE       = (1 << MOVE_FLAGS_SHIFT) // special0 = 1 pawn moves 2 squares
    ,MOVE_CASTLED_Q_SIDE    = (2 << MOVE_FLAGS_SHIFT) // special1 = Castle
    ,MOVE_CASTLED_K_SIDE    = (3 << MOVE_FLAGS_SHIFT)
    ,MOVE_CAPTURE_ENEMY     = (4 << MOVE_FLAGS_SHIFT) // Capture
    ,MOVE_CAPTURE_EP        = (5 << MOVE_FLAGS_SHIFT)
    ,MOVE_PROMOTE_KNIGHT    = (8 << MOVE_FLAGS_SHIFT) // Promote
    ,MOVE_PROMOTE_BISHOP    = (9 << MOVE_FLAGS_SHIFT) // special0 = piece type
    ,MOVE_PROMOTE_ROOK      = (10<< MOVE_FLAGS_SHIFT) // special1 = piece type
    ,MOVE_PROMOTE_QUEEN     = (11<< MOVE_FLAGS_SHIFT) // special1 = piece type

    ,MOVE_HAS_CASTLED_MASK  = MOVE_CASTLED_Q_SIDE | MOVE_CASTLED_K_SIDE
};

struct State_t
{
    StateBitBoard_t _player[ NUM_PLAYERS ]; // 96 bytes if not storing board[ PIECE_EMPTY ]
    //                            //      96  112 bytes
    uint8_t         _bFlags     ; // +1   97  113
    uint8_t         _bMoveType  ; // +1   98  114  4-bites: Piece move 4-bits: Type of move or capture
    uint16_t        _iParent    ; // +2  100  116  i-node of parent
    uint16_t        _iFirstChild; // +2  102  118  i-node of first child
    uint8_t         _nChildren  ; // +1  103  119  total children
    uint8_t         _nBestMoveRF; // +1  104  120
     int16_t        _nEval      ; // +2  106  122
    uint8_t         _iFrom      ; // +1  107  123  RankFile (0xROWCOL)
    uint8_t         _iTo        ; // +1  108  124  RankFile (0xROWCOL)
    uint8_t         _bPawnsMoved; // +1  109  125  8-bits: Bitflags if pawn[col] has moved
    uint8_t         _iGamePhase ; // +1  110  126  0x00=early game, 0x80=mid-game, 0xFF=end-game
    uint8_t         _pad1       ; // ============
    uint8_t         _pad2       ; //     112  128

    void Zero()
    {
        memset( this, 0, sizeof( *this ) );
    }

    void Clear()
    {
        pState = &_player[ PLAYER_WHITE ];
        pState->Zero();

        pState = &_player[ PLAYER_BLACK ];
        pState->Zero();
    }

    void Init()
    {
printf( "INFO: Boards[]: %u bytes\n", (uint32_t) sizeof( _player ) );
printf( "INFO: State   : %u bytes\n", (uint32_t) sizeof( *this    ) );

        _nEval  = 0;
        _bFlags = 0 | STATE_CAN_CASTLE_Q_SIDE | STATE_CAN_CASTLE_K_SIDE;
        _iFrom  = 0;
        _iTo    = 0;

        _bPawnsMoved = 0;
        _iParent     = 0;
        _iFirstChild = 1;
        _nChildren   = 0;

        StateBitBoard_t *pState;

        pState = &_player[ PLAYER_WHITE ];
        // use board[ PIECE_EMPTY ] as Current Best Search ?
        //pState->_aBoards[ PIECE_EMPTY  ] = 0; // BitBoardMakeWhiteSquares();
        pState->_aBoards[ PIECE_PAWN   ] = BitBoardInitWhitePawn  ();
        pState->_aBoards[ PIECE_ROOK   ] = BitBoardInitWhiteRook  ();
        pState->_aBoards[ PIECE_KNIGHT ] = BitBoardInitWhiteKnight();
        pState->_aBoards[ PIECE_BISHOP ] = BitBoardInitWhiteBishop();
        pState->_aBoards[ PIECE_QUEEN  ] = BitBoardInitWhiteQueen ();
        pState->_aBoards[ PIECE_KING   ] = BitBoardInitWhiteKing  ();

        pState = &_player[ PLAYER_BLACK ];
        //pState->_aBoards[ PIECE_EMPTY  ] = 0; // BitBoardMakeBlackSquares();
        pState->_aBoards[ PIECE_PAWN   ] = BitBoardInitBlackPawn  ();
        pState->_aBoards[ PIECE_ROOK   ] = BitBoardInitBlackRook  ();
        pState->_aBoards[ PIECE_KNIGHT ] = BitBoardInitBlackKnight();
        pState->_aBoards[ PIECE_BISHOP ] = BitBoardInitBlackBishop();
        pState->_aBoards[ PIECE_QUEEN  ] = BitBoardInitBlackQueen ();
        pState->_aBoards[ PIECE_KING   ] = BitBoardInitBlackKing  ();
    }

    void Dump()
    {
        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            StateBitBoard_t *pState = &_player[ iPlayer ];
            pState->Dump( iPlayer );
        }
    }

    void AddPiece( int iPlayer, int iPiece, uint8_t nDstRF )
    {
        StateBitBoard_t *pState = &_player[ iPlayer ];
        pState->AddPiece( iPiece, nDstRF );
    }

    void DelPiece( uint8_t nDstRF )
    {
        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            StateBitBoard_t *pState = &_player[ iPlayer ];
            pState->DelPiece( nDstRF );
        }
    }

    void _GetRawBoard( RawBoard_t& board )
    {
        board.Init();

        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            StateBitBoard_t *pState = &_player[ iPlayer ];
            pState->BitBoardsToRawBoard( iPlayer, &board );
        }
    }

    void CompactPrintBoard( bool bPrintRankFile = true )
    {
        RawBoard_t board;
        _GetRawBoard( board );
        board.PrintCompact( bPrintRankFile );
    }

    void PrettyPrintBoard( bool bPrintRankFile = true )
    {
        RawBoard_t board;
        _GetRawBoard( board );
        board.PrintPretty( bPrintRankFile );
    }

inline uint8_t GetColorPlayer() { return  _bFlags &  STATE_WHICH_PLAYER; }
inline uint8_t GetColorEnemy () { return ~_bFlags &  STATE_WHICH_PLAYER; }
inline void    TogglePlayer  () {         _bFlags ^= STATE_WHICH_PLAYER; }

    bitboard_t GetPlayerAllPieces( int iPlayer )
    {
        return _player[ iPlayer ].GetBoardAllPieces();
    }

    bitboard_t GetAllPieces()
    {
        bitboard_t board = 0;

        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
            board |= GetPlayerAllPieces( iPlayer );

        return board;
    }

    bool IsCheck( uint8_t nKingRF ) // = 0xFF )
    {
        uint8_t iEnemy  = GetColorEnemy ();

//        StateBitBoard_t *pStateUs   = &_player[ iPlayer ];
//        StateBitBoard_t *pStateThem = &_player[ iEnemy  ];

        //bitboard_t origin           = _player[ iPlayer ]._aBoards[ PIECE_KING ];
        //uint8_t    kingRankFile     = BitBoardToRankFile( origin );
        bitboard_t origin             = BitBoardMakeLocation( nKingRF );

        // From the King's location
        // see if any of the enemy's pieces have Line-of-Sight to us

        // Broad  Phase: quickly cull out if piece can't even potentially move to king
        //               if (boardPiece & origin) can potentially see us
        for( int iPiece = PIECE_QUEEN; iPiece > PIECE_PAWN; iPiece-- )
        {
            bitboard_t movesPotential = 0;
            bitboard_t movesValid     = 0;
            bitboard_t piecesEnemy    = _player[ iEnemy ]._aBoards[ iPiece ];

(void) movesValid;

            switch( iPiece )
            {
                case PIECE_QUEEN : movesPotential = BitBoardMovesColorQueen ( nKingRF ); break;
                case PIECE_BISHOP: movesPotential = BitBoardMovesColorBishop( nKingRF ); break;
                case PIECE_KNIGHT: movesPotential = BitBoardMovesColorKnight( nKingRF ); break;
                case PIECE_ROOK  : movesPotential = BitBoardMovesColorRook  ( nKingRF ); break;
                case PIECE_PAWN  :
                    if( iEnemy == PLAYER_BLACK )
                        movesPotential = BitBoardMovesWhitePawn( nKingRF );
                    else
                        movesPotential = BitBoardMovesBlackPawn( nKingRF );
                    break;

                // Any enemy pieces lay on the potential moves lines?
                if (movesPotential & origin)
                {
                    if (piecesEnemy & movesPotential)
                    {
                        // Narrow Phase: walk from Enemy to King checking LoS
                    }
                }
                else
                {
                   ; // can't even move to us
                }
            }
        } // for piece

        return false; // FIXME:
    }

    bool IsValidMove( uint8_t fromRankFile, uint8_t toRankFile )
    {
        bitboard_t origin = BitBoardMakeLocation( fromRankFile );
        bitboard_t dest   = BitBoardMakeLocation( toRankFile );

        // get the piece type
        uint8_t iPlayer    = GetColorPlayer();
        uint8_t iEnemy     = GetColorEnemy ();

        uint8_t iPieceSrc  = _player[ iPlayer ].GetPiece( fromRankFile );
        uint8_t iPieceDst  = _player[ iPlayer ].GetPiece( toRankFile );
        uint8_t iEnemyDst  = _player[ iEnemy  ].GetPiece( toRankFile ); // check for capture

        if( iPieceSrc == PIECE_EMPTY )
            return false;

(void) iPieceDst;
(void) iEnemyDst;
(void) origin;
(void) dest;

        // If dest is same color as player mark invalid

        // If new state IsCheck() not a valid move

        // If dest is enemy color

        // Except if King (this color) captures Rook (this color)
        // And King hasn't moved
        // And Rook hasn't moved

        // Check Line-of-Sight from piece

        return false; // FIXME:
    }

    bool Move( uint8_t fromRankFile, uint8_t toRankFile )
    {
        bool    bValid    = false;

        uint8_t iPlayer   = GetColorPlayer();
        uint8_t iPieceSrc = _player[ iPlayer ].GetPiece( fromRankFile );

        int bCanCastleQ = _bFlags & STATE_CAN_CASTLE_Q_SIDE;
        int bCanCastleK = _bFlags & STATE_CAN_CASTLE_K_SIDE;
        int bCanCastle  = _bFlags & STATE_CAN_CASTLE_MASK  ;

/*
    switch( move.iPieceSrc )
    {
    }
*/

        if( iPieceSrc == PIECE_ROOK )
        {
            if( iPlayer == PLAYER_WHITE )
            {
                if( bCanCastleQ && (fromRankFile == 0x00) )
                    _bFlags &= ~STATE_CAN_CASTLE_Q_SIDE; // mark can't castle

                if( bCanCastleK && (fromRankFile == 0x07) )
                    _bFlags &= ~STATE_CAN_CASTLE_K_SIDE; // mark can't castle
            }
            else // PLAYER_BLACK
            {
                if( bCanCastleQ && (fromRankFile == 0x70) )
                    _bFlags &= ~STATE_CAN_CASTLE_Q_SIDE; // mark can't castle

                if( bCanCastleK && (fromRankFile == 0x77) )
                    _bFlags &= ~STATE_CAN_CASTLE_K_SIDE; // mark can't castle
            }
        }
        else
        if( iPieceSrc == PIECE_KING )
        {
            // Input: e1g1, e1c1, e8g8, e8c8
            if( bCanCastle )
            {
                Castle_t castle;
                castle.bWhichSide = 0;
                castle.nNewKingRF = toRankFile;

                if ((iPlayer == PLAYER_WHITE) && (fromRankFile == _E1))
                {
                    if (bCanCastleQ && (toRankFile == _C1))
                    {
                        castle.bWhichSide = MOVE_CASTLED_Q_SIDE;
                        castle.nNewKingRF = _C1;
                        castle.nNewRookRF = _D1;
                        castle.nOldRookRF = _A1;
                    }
                    else
                    if (bCanCastleK && (toRankFile == _G1))
                    {
                        castle.bWhichSide = MOVE_CASTLED_K_SIDE;
                        castle.nNewKingRF = _G1;
                        castle.nNewRookRF = _F1;
                        castle.nOldRookRF = _H1;
                    }
                }
                else
                if ((iPlayer == PLAYER_BLACK) && (fromRankFile == _E8))
                {
                    if (bCanCastleQ && (toRankFile == _C8))
                    {
                        castle.bWhichSide = MOVE_CASTLED_Q_SIDE;
                        castle.nNewKingRF = _C8;
                        castle.nNewRookRF = _D8;
                        castle.nOldRookRF = _A8;
                    }
                    else
                    if (bCanCastleK && (toRankFile == _G8))
                    {
                        castle.bWhichSide = MOVE_CASTLED_K_SIDE;
                        castle.nNewKingRF = _G8;
                        castle.nNewRookRF = _F8;
                        castle.nOldRookRF = _H8;
                    }
                }

                if( castle.bWhichSide ) // Attempt to castle?
                {
                    // Verify no other pieces between king and rook
                    bitboard_t boardAll     = GetAllPieces();
                    bitboard_t boardNewKing = BitBoardMakeLocation( castle.nNewKingRF );
                    bitboard_t boardNewRook = BitBoardMakeLocation( castle.nNewRookRF );
                    bitboard_t boardOldKing = BitBoardMakeLocation( fromRankFile );
                    bitboard_t boardOldRook = BitBoardMakeLocation( castle.nOldRookRF );

                    if (boardAll & boardNewKing) // if (iPieceDst == PIECE_EMPTY)
                        return false;

                    if( boardAll & boardNewRook)
                        return false;

                    // Verify Player's own rook exists in the right location
                    if( ! (_player[ iPlayer ]._aBoards[ PIECE_ROOK ] & boardOldRook) )
                        return false;

                    bool bIsCheck0         = IsCheck( castle.nNewKingRF );
                    bool bIsCheck1         = IsCheck( castle.nNewRookRF );
                    bool bPassThroughCheck = bIsCheck0 | bIsCheck1;

                    if( bPassThroughCheck )
                        return false;

                    // _bFlags |=  STATE_GAME_MID;

//                    Move_t moveKing = MakeMove( fromRankFile     , castle.nNewKingRF );
//                    Move_t moveRook = MakeMove( castle.nOldRookRF, castle.nNewRookRF );
//
//                    DoMove( moveKing );
//                    DoMove( moveRook );

                    // Remove old king, old rook
                    _player[ iPlayer ]._aBoards[ PIECE_KING ] &= ~boardOldKing;
                    _player[ iPlayer ]._aBoards[ PIECE_ROOK ] &= ~boardOldRook;

                    // Place new king, new rook
                    _player[ iPlayer ]._aBoards[ PIECE_KING ] |= boardNewKing;
                    _player[ iPlayer ]._aBoards[ PIECE_ROOK ] |= boardNewRook;

                    bValid = true;
                }

                // King moved, can't castle anymore
                SetCastledFlags( castle.bWhichSide ); // Optimization, 0=none, else
            } // can castle
            else
            {
                bitboard_t boardPotential = BitBoardMovesColorKing( fromRankFile );
                bitboard_t boardNewKing   = BitBoardMakeLocation  ( toRankFile   );

                if( boardPotential & boardNewKing )
                {
                    Move_t move = MakeMove( fromRankFile, toRankFile );
                    DoMove( move );
                    bValid = true;
                }
            }
        }

        return bValid;
    }

    bool Capture( uint8_t fromRankFile, uint8_t toRankFile )
    {
        bool bValid = true;

(void) fromRankFile;
(void) toRankFile ;

        return bValid;
    }

    Move_t MakeMove( uint8_t fromRankFile, uint8_t toRankFile )
    {
        uint8_t iPlayer = GetColorPlayer();
        uint8_t iEnemy  = GetColorEnemy ();

        Move_t move;

        move.iPlayer = iPlayer;
        move.iEnemy  = iEnemy ;

        move.nSrcRF = fromRankFile;
        move.nDstRF = toRankFile  ;

        move.iPieceSrc = _player[ iPlayer ].GetPiece( fromRankFile );
        move.iPieceDst = _player[ iPlayer ].GetPiece( toRankFile   );
        move.iEnemySrc = _player[ iEnemy  ].GetPiece( fromRankFile );
        move.iEnemyDst = _player[ iEnemy  ].GetPiece( toRankFile   ); // check for capture

        move.bBoardSrc = BitBoardMakeLocation( fromRankFile );
        move.bBoardDst = BitBoardMakeLocation( toRankFile   );

        return move;
    }

    // Remove piece from old location, place piece onto new location
    void DoMove( const Move_t& move )
    {
        bitboard_t boardOld = BitBoardMakeLocation( move.nSrcRF );
        bitboard_t boardNew = BitBoardMakeLocation( move.nDstRF );

        _player[ move.iPlayer ]._aBoards[ move.iPieceSrc ] &= ~boardOld;
        _player[ move.iPlayer ]._aBoards[ move.iPieceSrc ] |=  boardNew;
    }

    bool MoveOrCapture( uint8_t fromRankFile, uint8_t toRankFile )
    {
        bool   bValid = false;
        Move_t move   = MakeMove( fromRankFile, toRankFile );

        // Move should already be verified:
        // iPieceSrc != PIECE_EMPTY
        // iPieceDst != PIECE_EMPTY
        if (move.iPieceSrc == PIECE_EMPTY)
            return bValid;

        if (move.iPieceDst != PIECE_EMPTY)
            return bValid; // Can't attack same color!

        // if dst square is empty, the IsMove()
        if (move.iEnemyDst == PIECE_EMPTY)
            bValid = Move( fromRankFile, toRankFile );
        else
            bValid = Capture( fromRankFile, toRankFile );

        return bValid;
    }

    float Eval()
    {
        _nEval = 0;

        return _nEval;
    }

    void SetCastledFlags( int bWhichCastleSide )
    {
        _bFlags     &= ~STATE_CAN_CASTLE_MASK;
        _bMoveType  |=  bWhichCastleSide     ;
    }
};

