struct StateBitBoard_t
{
    bitboard_t _aBoards[ NUM_PIECES ];

    void Zero()
    {
        memset( _aBoards, 0, sizeof( _aBoards ) );
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

