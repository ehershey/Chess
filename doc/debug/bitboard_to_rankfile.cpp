uint8_t BitBoardToRankFile( const bitboard_t board )
{
    // Optimization: Find First Set Bit Position (63=MSB, 0=LSB)
    // C++: GCC: __builtin_clz
    //      MS : #include <intrin.h> // _BitScanReverse( &DWORD, val ); _BitScanReverse64( &output, val );
    //      Clang: http://clang.llvm.org/doxygen/lzcntintrin_8h_source.html
    // Asm: x86: BSR    Bit Scan Reverse
    //      PPC: cntlz  Count Leading Zeroes

    // http://stackoverflow.com/questions/671815/what-is-the-fastest-most-efficient-way-to-find-the-highest-set-bit-msb-in-an-i
    // http://stackoverflow.com/questions/355967/how-to-use-msvc-intrinsics-to-get-the-equivalent-of-this-gcc-code

    if( 0 == board )
        return 0xFF; // Invalid !

    bitboard_t search = board;
    uint8_t nRankFile = 0;

printf( "DEBUG: b: %08X %08X\n", (uint32_t) (board >> 32), (uint32_t) board );

    while( !(search & 0xFF) ) // nRowMask = 0xFF
    {
        search >>= 8;
        nRankFile += 0x10; // rank
    }
printf( "DEBUG: RF: %02X\n", nRankFile );
printf( "DEBUG: b : %08X\n", (uint32_t) search );

    for( int x = 0; x < 8; x++ )
    {
printf( "DEBUG: [%d] %02X: %d\n", x, aColMask[ x], (uint8_t) (search & aColMask[x]) );
        if( search & aColMask[ x ] )
        {
printf( "DEBUG: Found @ %d\n", x );
            return (nRankFile + x);
        }
    }

    return 0xFF; // WTF?! Invalid!
}

