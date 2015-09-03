
uint32_t aEVAL_CENTER_DISTANCE[ 8 ] =
{   // ABCDEFGH
     0x01233210 // 8
    ,0x12344321 // 7
    ,0x23455432 // 6
    ,0x34566543 // 5
    ,0x34566543 // 4
    ,0x23455432 // 3
    ,0x12344321 // 2
    ,0x01233210 // 1
};

// no aEVAL_LOCATION_ROOK because no positional advantage

// Weighted strength of Knight's position
uint32_t aEVAL_LOCATION_KNIGHT[ 8 ] = 
#if 0
{   // ABCDEFGH
     0x12344321 // 8
    ,0x24788742 // 7
    ,0x37DEED73 // 6
    ,0x48EFFE84 // 5
    ,0x48EFFE84 // 4
    ,0x37DEED73 // 3
    ,0x24688642 // 2
    ,0x12344321 // 1
};
#else
// Simple: number of reachable cells from a given location
{   // ABCDEFGH
     0x23444432 // 8
    ,0x34677643 // 7 0x34666643
    ,0x46899864 // 6 4688888864
    ,0x579AA975 // 5 0x46888864
    ,0x579AA975 // 4 0x46888864
    ,0x46899864 // 3 0x46888864
    ,0x34677643 // 2 0x34666643
    ,0x23444432 // 1
};
#endif

uint32_t aEVAL_LOCATION_BISHOP[ 8 ] =
{   // ABCDEFGH
     0x77777777 // 8
    ,0x79999997 // 7
    ,0x79BBBB97 // 6
    ,0x79BDDB97 // 5
    ,0x79BDDB97 // 4
    ,0x79BBBB97 // 3
    ,0x79999997 // 2
    ,0x77777777 // 1
};

// Simple: number of reachable cells from a given location
uint32_t aEVAL_LOCATION_QUEEN[8] =
{   // ABCDEFGH
     0x55555555 // 8
    ,0x57777775 // 7
    ,0x57999975 // 6
    ,0x579BB975 // 5
    ,0x579BB975 // 4
    ,0x57999975 // 3
    ,0x57777775 // 2
    ,0x55555555// 1
};

// aEVAL_LOCATION_KING_BEGGAME[ 8 ] = // Early
// aEVAL_LOCATION_KING_MIDGAME[ 8 ] = // Middle
// aEVAL_LOCATION_KING_ENDGAME[ 8 ] = // Late
