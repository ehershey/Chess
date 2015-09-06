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
//  uint8_t    iEnemy    ; // OptimizatioN: iPlayer^1

    // Location
    uint8_t    iSrcRF    ;
    uint8_t    iDstRF    ;

    uint8_t    iPlayerSrc; // which player
    uint8_t    iPlayerDst; // which player

    // Piece
    uint8_t    iPieceSrc ; // (ColorPieces_e) // 8*iPlayerColor
    uint8_t    iPieceDst ; // (ColorPieces_e) // 8*iPlayerColor
//  uint8_t    iEnemySrc ;
//  uint8_t    iEnemyDst ;

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
    ,STATE_CHECKMATE         = (1 << 2)
    ,STATE_STALEMATE         = (1 << 3)
    ,STATE_CAN_CASTLE_Q_SIDE = (1 << 4) // TODO: do we need same shift as STATE_CAN_CASTLE_Q_SIDE == MOVE_CASTLED_Q_SIDE ?
    ,STATE_CAN_CASTLE_K_SIDE = (1 << 5) // TODO: do we need same shift as STATE_CAN_CASTLE_K_SIDE == MOVE_CASTLED_K_SIDE ?

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
    // Move                        ; //      96  112 bytes
    uint8_t         _bFlags        ; // +1   97  113
    uint8_t         _bMoveType     ; // +1   98  114  4-bites: Piece move 4-bits: Type of move or capture
    uint8_t         _iSrcRF        ; // +1   99  115  From RankFile (0xROWCOL)
    uint8_t         _iDstRF        ; // +1  100  116  Unto RankFile (0xROWCOL)
    uint8_t         _bPawnsMoved[2]; // +2  102  118  8-bits: Bitflags if pawn[col] has moved
    // Children                    ;
    uint16_t        _iParent       ; // +2  104  120  i-node of parent
    uint16_t        _iFirstChild   ; // +2  106  122  i-node of first child
    uint8_t         _nChildren     ; // +1  107  123  total children
    // Eval & Search               ;
    uint8_t         _nDepth        ; // +1  108  124
     int16_t        _nEval         ; // +2  110  126
    uint8_t         _nBestMoveRF   ; // +1  111  127
    // Unused                      ; // ============  Current depth when searching
    uint8_t         _iGamePhase    ; //     112  128  0x00=early game, 0x80=mid-game, 0xFF=end-game

    void AddPiece( int iPlayer, int iPiece, uint8_t nDstRF )
    {
        StateBitBoard_t *pState = &_player[ iPlayer ];
        pState->AddPiece( iPiece, nDstRF );
    }

    bool Capture( const Move_t& move )
    {
        bool bValid = false;

        switch (move.iPieceSrc & PIECE_MASK)
        {
            case PIECE_PAWN  : bValid = CapturePawn  ( move ); break;
            case PIECE_ROOK  : bValid = CaptureRook  ( move ); break;
            case PIECE_KNIGHT: bValid = CaptureKnight( move ); break;
            case PIECE_BISHOP: bValid = CaptureBishop( move ); break;
            case PIECE_QUEEN : bValid = CaptureQueen ( move ); break;
            case PIECE_KING  : bValid = CaptureKing  ( move ); break;
            default:
                printf( "ERROR: Invalid Capture with unknown piece!\n" );
        }

//        _bMoveType &= ~MOVE_PIECE_MASK;
//        _bMoveType |=  move.iPieceSrc;

        return bValid;
    }

    bool CaptureBishop( const Move_t& move )
    {
        bool bValid = false;

        _bMoveType &= ~MOVE_FLAGS_MASK;
        _bMoveType |= MOVE_CAPTURE_ENEMY;

        return bValid;
    }

    bool CaptureKing( const Move_t& move )
    {
        bool bValid = false;

        _bMoveType &= ~MOVE_FLAGS_MASK;
        _bMoveType |= MOVE_CAPTURE_ENEMY;

        return bValid;
    }

    bool CaptureKnight( const Move_t& move )
    {
        bool bValid = false;

        _bMoveType &= ~MOVE_FLAGS_MASK;
        _bMoveType |= MOVE_CAPTURE_ENEMY;

        return bValid;
    }

    bool CapturePawn( const Move_t& move )
    {
        bool bValid = false;

        DoCapture( move );

        _bMoveType &= ~MOVE_FLAGS_MASK;
        _bMoveType |= MOVE_CAPTURE_ENEMY;

        return bValid;
    }

    bool CaptureRook( const Move_t& move )
    {
        bool bValid = false;

        // Check if enemy is PLAYER_WHITE
        // still can castle, Check if iDstRF == A1, then can't castle Q side
        // still can castle, Check if iDstRF == H1, then can't castle K side
        // Check if enemy is PLAYER_BLACK
        // still can castle, Check if iDstRF == A8, then can't castle Q side
        // still can castle, Check if iDstRF == H8, then can't castle K side

        _bMoveType &= ~MOVE_FLAGS_MASK;
        _bMoveType |= MOVE_CAPTURE_ENEMY;

        return bValid;
    }

    bool CaptureQueen( const Move_t& move )
    {
        bool bValid = false;

        _bMoveType &= ~MOVE_FLAGS_MASK;
        _bMoveType |= MOVE_CAPTURE_ENEMY;

        return bValid;
    }

    void Clear()
    {
        StateBitBoard_t *pState;

        pState = &_player[ PLAYER_WHITE ];
        pState->Zero();

        pState = &_player[ PLAYER_BLACK ];
        pState->Zero();
    }

    void DelPiece( uint8_t nDstRF )
    {
        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            StateBitBoard_t *pState = &_player[ iPlayer ];
            pState->DelPiece( nDstRF );
        }
    }

    void DoCapture( const Move_t move )
    {
        _iSrcRF = move.iSrcRF;
        _iDstRF = move.iDstRF;

        _player[ move.iPlayer   ]._aBoards[ move.iPieceSrc & PIECE_MASK ] &= ~move.bBoardSrc;
        _player[ move.iPlayer   ]._aBoards[ move.iPieceSrc & PIECE_MASK ] |=  move.bBoardDst;

        _player[ move.iPlayer^1 ]._aBoards[ move.iPieceDst & PIECE_MASK ] &= ~move.bBoardDst;

        // IsCheck()?
    }

    // Remove piece from old location, place piece onto new location
    void DoMove( const Move_t& move )
    {
        _iSrcRF = move.iSrcRF;
        _iDstRF = move.iDstRF;

        bitboard_t boardOld = BitBoardMakeLocation( move.iSrcRF );
        bitboard_t boardNew = BitBoardMakeLocation( move.iDstRF );

        _player[ move.iPlayer ]._aBoards[ move.iPieceSrc & PIECE_MASK ] &= ~boardOld;
        _player[ move.iPlayer ]._aBoards[ move.iPieceSrc & PIECE_MASK ] |=  boardNew;

        // IsCheck()?
    }

    void Dump()
    {
        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            StateBitBoard_t *pState = &_player[ iPlayer ];
            pState->Dump( iPlayer );
        }
    }

    float Eval()
    {
        _nEval = 0;

        return _nEval;
    }

    void Init()
    {
printf( "INFO: Boards[]: %u bytes\n", (uint32_t) sizeof( _player ) );
printf( "INFO: State   : %u bytes\n", (uint32_t) sizeof( *this    ) );

        Reset();

//printf( "State.Flags: %08X\n", _bFlags );

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

    bitboard_t GetAllPieces()
    {
        bitboard_t board = 0;

        for( int iPlayer = 0; iPlayer < NUM_PLAYERS; iPlayer++ )
            board |= GetPlayerAllPieces( iPlayer );

        return board;
    }

inline uint8_t GetColorEnemy () { return ~_bFlags &  STATE_WHICH_PLAYER; }
inline uint8_t GetColorPlayer() { return  _bFlags &  STATE_WHICH_PLAYER; }

    bitboard_t GetPlayerAllPieces( int iPlayer )
    {
        return _player[ iPlayer ].GetBoardAllPieces();
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

    /**
        @param nKingRF
            0xRankFile      - Use specified king position
            INVALID_MOVE_RF - Use current king position
    */
    bool IsCheck( uint8_t nKingRF ) // = INVALID_MOVE_RF
    {
        bool    bIsCheck    = false;
        uint8_t iPlayer     = GetColorPlayer();
        uint8_t iEnemy      = GetColorEnemy ();
        uint8_t bPawnsMoved = _bPawnsMoved[ iEnemy ];

        bitboard_t origin = (nKingRF == INVALID_MOVE_RF)
            ? _player[ iPlayer ]._aBoards[ PIECE_KING ]
            : BitBoardMakeLocation( nKingRF )
            ;

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
                case PIECE_PAWN  :
// TODO: FIXME: Replace with BitBoardAttacksWhitePawn( nKingRF, bPawnsMoved )
                    if (iEnemy == PLAYER_WHITE)
                        movesPotential = BitBoardMovesWhitePawn( nKingRF, bPawnsMoved );
                    else
                        movesPotential = BitBoardMovesBlackPawn( nKingRF, bPawnsMoved );
                    break;
                case PIECE_ROOK  : movesPotential = BitBoardMovesColorRook  ( nKingRF ); break;
                case PIECE_KNIGHT: movesPotential = BitBoardMovesColorKnight( nKingRF ); break;
                case PIECE_BISHOP: movesPotential = BitBoardMovesColorBishop( nKingRF ); break;
                case PIECE_QUEEN : movesPotential = BitBoardMovesColorQueen ( nKingRF ); break;

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

        if( bIsCheck )
        {
            uint8_t nMoves;
            uint8_t aMoves[ MAX_PIECE_MOVES ];
            BitBoardToRankFileAllMoves( origin, nMoves, aMoves, MAX_PIECE_MOVES );
            // IsCheckmate()?
        }

        return bIsCheck;
    }

    bool IsCheckPassInto( const Move_t& move )
    {
        bool bIsCheck = false;

        State_t nextState = *this;

        nextState.DoMove( move );
        if ( nextState.IsCheck( INVALID_MOVE_RF ) )
            bIsCheck = true;

        return bIsCheck;
    }


    // Test trivial potential moves
    bool IsValidMove( const Move_t& move )
    {
        bool       bValid          = false;
        uint8_t    bPawnsMoved     = _bPawnsMoved[ move.iPlayer ];
        bitboard_t bBoardPotential = 0; // potential moves
        bitboard_t bBoardCastle    = move.bBoardDst;

        // If dest is same color as player mark invalid
        // If new state IsCheck() not a valid move
        // If dest is enemy color
        // Check Line-of-Sight from source to destination

        if ((move.iPieceSrc & PIECE_MASK) == PIECE_EMPTY)
            return bValid;

        switch( move.iPieceSrc )
        {
            case WHITE_PAWN  : bBoardPotential = BitBoardMovesWhitePawn( move.iSrcRF, bPawnsMoved ); break;
            case BLACK_PAWN  : bBoardPotential = BitBoardMovesBlackPawn( move.iSrcRF, bPawnsMoved ); break;
            case WHITE_ROOK  :
            case BLACK_ROOK  : bBoardPotential = BitBoardMovesColorRook  ( move.iSrcRF ); break;
            case WHITE_KNIGHT:
            case BLACK_KNIGHT: bBoardPotential = BitBoardMovesColorKnight( move.iSrcRF ); break;
            case WHITE_BISHOP:
            case BLACK_BISHOP: bBoardPotential = BitBoardMovesColorBishop( move.iSrcRF ); break;
            case WHITE_QUEEN :
            case BLACK_QUEEN : bBoardPotential = BitBoardMovesColorQueen ( move.iSrcRF ); break;
            case WHITE_KING  : bBoardPotential = BitBoardMovesColorKing  ( move.iSrcRF );
                if (_bFlags & STATE_CAN_CASTLE_Q_SIDE) bBoardCastle |= BitBoardMakeLocation( _C1 );
                if (_bFlags & STATE_CAN_CASTLE_K_SIDE) bBoardCastle |= BitBoardMakeLocation( _G1 );
                bValid = bBoardPotential & bBoardCastle ? 1 : 0;
                return bValid;
                break;
            case BLACK_KING  : bBoardPotential = BitBoardMovesColorKing  ( move.iSrcRF );
                if (_bFlags & STATE_CAN_CASTLE_Q_SIDE) bBoardCastle |= BitBoardMakeLocation( _C8 );
                if (_bFlags & STATE_CAN_CASTLE_K_SIDE) bBoardCastle |= BitBoardMakeLocation( _G8 );
                bValid = bBoardPotential & bBoardCastle ? 1 : 0;
                return bValid;
                break;
        }

        bValid = bBoardPotential & move.bBoardDst ? 1 : 0;

        if ( IsCheckPassInto( move ) )
            return bValid;

        bValid = true;
        return bValid;
    }

    Move_t MakeMove( uint8_t fromRankFile, uint8_t toRankFile )
    {
        Move_t move;

        move.iPlayer = GetColorPlayer();

        move.iSrcRF = fromRankFile;
        move.iDstRF = toRankFile  ;

        move.bBoardSrc = BitBoardMakeLocation( fromRankFile );
        move.bBoardDst = BitBoardMakeLocation( toRankFile   );

        bitboard_t aBoardPieces[ NUM_PLAYERS ];
        /*      */ aBoardPieces[ PLAYER_WHITE ] = _player[ PLAYER_WHITE ].GetBoardAllPieces();
        /*      */ aBoardPieces[ PLAYER_BLACK ] = _player[ PLAYER_BLACK ].GetBoardAllPieces();

        move.iPlayerSrc = move.iPlayer; // Default square owner is player
        move.iPlayerDst = move.iPlayer; // Default square owner is player

        for( int iPlayer = PLAYER_WHITE; iPlayer < NUM_PLAYERS; iPlayer++ )
        {
            if (aBoardPieces[ iPlayer ] & move.bBoardSrc)
                move.iPlayerSrc = iPlayer;

            if (aBoardPieces[ iPlayer ] & move.bBoardDst)
                move.iPlayerDst = iPlayer;
        }

        move.iPieceSrc = _player[ move.iPlayerSrc ].GetPiece( fromRankFile ) + (8 * move.iPlayerSrc);
        move.iPieceDst = _player[ move.iPlayerDst ].GetPiece( toRankFile   ) + (8 * move.iPlayerDst);

        return move;
    }

    bool Move( const Move_t& move )
    {
        bool bValid = false;

        // Trivial reject moves
        if ( !IsValidMove( move ) )
        {
            printf( "ERROR: Invalid potential move\n" );
            return bValid;
        }

        int iPiece = move.iPieceSrc & PIECE_MASK;
        switch ( iPiece )
        {
            case PIECE_PAWN  : bValid = MovePawn  ( move ); break;
            case PIECE_ROOK  : bValid = MoveRook  ( move ); break;
            case PIECE_KNIGHT: bValid = MoveKnight( move ); break;
            case PIECE_BISHOP: bValid = MoveBishop( move ); break;
            case PIECE_QUEEN : bValid = MoveQueen ( move ); break;
            case PIECE_KING  : bValid = MoveKing  ( move ); break;
            default:
                printf( "ERROR: Invalid Move with unknown piece!\n" );
        }

        if ( bValid )
        {
            _bMoveType &= ~MOVE_PIECE_MASK;
            _bMoveType |=  iPiece;
        }

        return bValid;
    }

    bool MoveBishop( const Move_t& move )
    {
        bool bValid = false;

        if ( IsCheckPassInto( move ) )
            return bValid;

        bValid = true;
        DoMove( move );

        _bMoveType &= ~MOVE_FLAGS_MASK;
        // Optimziation: _bMoveType |= MOVE_NORMAL

        return bValid;
    }

    bool MoveKing( const Move_t& move )
    {
        bool bValid = false;

        int bCanCastleQ = _bFlags & STATE_CAN_CASTLE_Q_SIDE;
        int bCanCastleK = _bFlags & STATE_CAN_CASTLE_K_SIDE;
        int bCanCastle  = _bFlags & STATE_CAN_CASTLE_MASK  ;

//printf( ">Move() State.Flags: %08X\n", _bFlags );
//printf( ">Move() KING  Castle: %s\n", bCanCastle ? "Yes" : "NO " );
            // Input: e1g1, e1c1, e8g8, e8c8
            if ( bCanCastle )
            {
                Castle_t castle;
                castle.bWhichSide = 0;
                castle.nNewKingRF = move.iDstRF;

                if (move.iPlayer == PLAYER_WHITE)
                {
                    if (move.iSrcRF == _E1)
                    {
                        if (bCanCastleQ && (move.iDstRF == _C1))
                        {
                            castle.bWhichSide = MOVE_CASTLED_Q_SIDE;
                            castle.nNewKingRF = _C1;
                            castle.nNewRookRF = _D1;
                            castle.nOldRookRF = _A1;
                        }
                        else
                        if (bCanCastleK && (move.iDstRF == _G1))
                        {
                            castle.bWhichSide = MOVE_CASTLED_K_SIDE;
                            castle.nNewKingRF = _G1;
                            castle.nNewRookRF = _F1;
                            castle.nOldRookRF = _H1;
                        }
                    }
                }
                else
                {
                    if (move.iSrcRF == _E8)
                    {
                        if (bCanCastleQ && (move.iDstRF == _C8))
                        {
                            castle.bWhichSide = MOVE_CASTLED_Q_SIDE;
                            castle.nNewKingRF = _C8;
                            castle.nNewRookRF = _D8;
                            castle.nOldRookRF = _A8;
                        }
                        else
                        if (bCanCastleK && (move.iDstRF == _G8))
                        {
                            castle.bWhichSide = MOVE_CASTLED_K_SIDE;
                            castle.nNewKingRF = _G8;
                            castle.nNewRookRF = _F8;
                            castle.nOldRookRF = _H8;
                        }
                    }
                }

                if ( castle.bWhichSide ) // Attempt to castle?
                {
                    // Verify no other pieces between king and rook
                    bitboard_t boardAll     = GetAllPieces();
                    bitboard_t boardNewKing = BitBoardMakeLocation( castle.nNewKingRF );
                    bitboard_t boardNewRook = BitBoardMakeLocation( castle.nNewRookRF );
                    bitboard_t boardOldKing = BitBoardMakeLocation( move.iSrcRF );
                    bitboard_t boardOldRook = BitBoardMakeLocation( castle.nOldRookRF );

                    if (boardAll & boardNewKing) // if (iPieceDst == PIECE_EMPTY)
                        return false;

                    if (boardAll & boardNewRook)
                        return false;

                    // Verify Player's own rook exists in the correct location
                    if (! (_player[ move.iPlayer ]._aBoards[ PIECE_ROOK ] & boardOldRook))
                        return false;

                    bool bIsCheck0         = IsCheck( castle.nNewKingRF ); // FIXME: Verify enemy
                    bool bIsCheck1         = IsCheck( castle.nNewRookRF );
                    bool bPassThroughCheck = bIsCheck0 | bIsCheck1;
                    if ( bPassThroughCheck )
                        return false;

                    // _bFlags |=  STATE_GAME_MID;

                    Move_t moveKing = MakeMove( move.iSrcRF      , castle.nNewKingRF );
                    Move_t moveRook = MakeMove( castle.nOldRookRF, castle.nNewRookRF );

                    DoMove( moveKing ); // Remove old king, place new king
                    DoMove( moveRook ); // Remove old rook, place new rook

                    bValid = true;
                }

                // King moved, can't castle anymore
                SetCastledFlags( castle.bWhichSide ); // Optimization, 0=none, else
            } // can castle
            else
            {
                bitboard_t boardPotential = BitBoardMovesColorKing( move.iSrcRF );

                if ( boardPotential & move.bBoardDst )
                {
                    bool bPassIntoCheck = IsCheck( move.iDstRF );
                    if ( bPassIntoCheck ) // Can't move into check
                        return false;

                    DoMove( move );

                    _bMoveType &= ~MOVE_FLAGS_MASK;
                    // Optimziation: _bMoveType |= MOVE_NORMAL

                    bValid = true;
                }
            }

        return bValid;
    }

    bool MoveKnight( const Move_t& move )
    {
        bool bValid = false;

        if ( IsCheckPassInto( move ) )
            return bValid;

        bValid = true;
        DoMove( move );

        _bMoveType &= ~MOVE_FLAGS_MASK;
        // Optimziation: _bMoveType |= MOVE_NORMAL

        return bValid;
    }

    bool MovePawn( const Move_t& move )
    {
        bool bValid = false;

        if ( IsCheckPassInto( move ) )
            return bValid;

        // if move.iEnemyDst == PIECE_EMPTY already done in MoveOrCapture()
        bValid = true;
        DoMove( move );

        _bMoveType &= ~MOVE_FLAGS_MASK;
        // Optimziation: _bMoveType |= MOVE_NORMAL

        int iSrcRank = move.iSrcRF >> 4;
        int iDstRank = move.iDstRF >> 4;
        int iSrcFile = move.iSrcRF &  7;

        int nDistance = (iDstRank - iSrcRank);
        if ((nDistance == 2) || (nDistance == -2))
            _bMoveType |= MOVE_PAWN_DOUBLE;

        _bPawnsMoved[ move.iPlayer ] |= (1 << iSrcFile);

        return bValid;
    }

    bool MoveRook( const Move_t& move )
    {
        bool bValid = false;

        int bCanCastleQ = _bFlags & STATE_CAN_CASTLE_Q_SIDE;
        int bCanCastleK = _bFlags & STATE_CAN_CASTLE_K_SIDE;
        int bCanCastle  = _bFlags & STATE_CAN_CASTLE_MASK  ;

        if ( IsCheckPassInto( move ) )
            return bValid;

        if (move.iPlayerSrc == PLAYER_WHITE)
        {
            if (bCanCastleQ && (move.iSrcRF == _A1))
                _bFlags &= ~STATE_CAN_CASTLE_Q_SIDE; // mark can't castle

            if (bCanCastleK && (move.iSrcRF == _A8))
                _bFlags &= ~STATE_CAN_CASTLE_K_SIDE; // mark can't castle
        }
        else // PLAYER_BLACK
        {
            if (bCanCastleQ && (move.iSrcRF == _A8))
                _bFlags &= ~STATE_CAN_CASTLE_Q_SIDE; // mark can't castle

            if (bCanCastleK && (move.iSrcRF == _H8))
                _bFlags &= ~STATE_CAN_CASTLE_K_SIDE; // mark can't castle
        }

        bValid = true;
        DoMove( move );

        _bMoveType &= ~MOVE_FLAGS_MASK;
        // Optimziation: _bMoveType |= MOVE_NORMAL

        return bValid;
    }

    bool MoveQueen( const Move_t& move )
    {
        bool bValid = false;

        if ( IsCheckPassInto( move ) )
            return bValid;

        bValid = true;
        DoMove( move );

        _bMoveType &= ~MOVE_FLAGS_MASK;
        // Optimziation: _bMoveType |= MOVE_NORMAL

        return bValid;
    }

    bool MoveOrCapture( uint8_t fromRankFile, uint8_t toRankFile )
    {
        bool   bValid = false;
        Move_t move   = MakeMove( fromRankFile, toRankFile );

        if (fromRankFile == toRankFile)
            return bValid;

        if ((move.iPieceSrc  & PIECE_MASK) == PIECE_EMPTY)
            return bValid;

        // if dst square is empty, then have move, else capture
        if ((move.iPieceDst & PIECE_MASK) == PIECE_EMPTY)
            bValid = Move( move );
        else
        {
            if (move.iPlayerSrc == move.iPlayerDst)
                return bValid; // Can't attack same color!

             bValid = Capture( move );
        }
        return bValid;
    }

    void PrintCompactBoard( bool bPrintRankFile = true )
    {
        RawBoard_t board;
        _GetRawBoard( board );
        board.PrintCompact( bPrintRankFile );
    }

    void PrintPrettyBoard( bool bPrintRankFile = true )
    {
        RawBoard_t board;
        _GetRawBoard( board );
        board.PrintPretty( bPrintRankFile );
    }

    void Reset()
    {
        // Move
        _bFlags         = 0 | STATE_CAN_CASTLE_Q_SIDE | STATE_CAN_CASTLE_K_SIDE;
        _bMoveType      = 0;
        _iSrcRF         = 0;
        _iDstRF         = 0;
        _bPawnsMoved[0] = 0;
        _bPawnsMoved[1] = 0;

        // Children
        _iParent     = 0;
        _iFirstChild = 1;
        _nChildren   = 0;

        // Eval & Search
        _nDepth      = 0;
        _nEval       = 0;
        _nBestMoveRF = INVALID_MOVE_RF;

        _iGamePhase  = 0;
    }

    void Search()
    {
    }

    /**
        @param bWhichCastleSide
            0 = none
            MOVE_CASTLED_Q_SIDE
            MOVE_CASTLED_K_SIDE
    */
    void SetCastledFlags( int bWhichCastleSide )
    {
        _bFlags     &= ~STATE_CAN_CASTLE_MASK;
        _bMoveType  |= bWhichCastleSide      ;
    }

inline void    TogglePlayer  () {         _bFlags ^= STATE_WHICH_PLAYER; }

    void Zero()
    {
        memset( this, 0, sizeof( *this ) );
    }
};
