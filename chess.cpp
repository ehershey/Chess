#include <stdio.h>  // printf
#include <stdint.h> // uint64_t
#include <string.h> // memcpy

// BEGIN OMP
    #include <omp.h>
// END OMP

#include "utility.h"
#include "bitboard.h"
#include "pieces.h"
#include "rawboard.h"
#include "state.h"
#include "game.h"
#include "eval.h"
#include "search.h"

    bool gbAnsiOutput = false;

// BEGIN OMP
    int gnThreadsMaximum = 0 ;
    int gnThreadsActive  = 0 ; // 0 = auto detect; > 0 manual # of threads

void StartupCommandLine( const int nArg, const char *aArg[] )
{
#ifdef WIN32
    gbAnsiOutput = false;
#else
    gbAnsiOutput = true;
#endif

    for( int iArg = 1; iArg < nArg; iArg++ )
    {
        if (strcmp( aArg[ iArg ], "-ansi" ) == 0)
            gbAnsiOutput = true;
    }
}

void StartupMulticore()
{
// BEGIN OMP
    gnThreadsMaximum = omp_get_num_procs();

    if(!gnThreadsActive) // user didn't specify how many threads to use, default to all of them
        gnThreadsActive = gnThreadsMaximum;
    else
        omp_set_num_threads( gnThreadsActive );

    printf( "Using: %d / %d cores\n", gnThreadsActive, gnThreadsMaximum );

    for( int iCore = 0; iCore < gnThreadsActive; iCore++ )
    {
        nMovePool[ iCore ] = 0;
        aMovePool[ iCore ] = new State_t[ MAX_POOL_MOVES ];
    }

    size_t nMemState = sizeof( State_t[ MAX_POOL_MOVES ] );
    printf( "Mem/Core: %u bytes (%u KB)\n", (uint32_t) nMemState, (uint32_t) nMemState/1024 );
}

void ShutdownMulticore()
{
    for( int iCore = 0; iCore < gnThreadsActive; iCore++ )
    {
        delete [] aMovePool[ iCore ];
    }
}
// END OMP

int INVALID_LOCATION = -1;

int GetInputLocation( char *pText, size_t nLen )
{
    char *p   = pText;
    int   nRF = INVALID_LOCATION;

    if (nLen < 2)
        return nRF;

    bool bUpper  = (p[0] >= 'A') && (p[0] <= 'H');
    bool bLower  = (p[0] >= 'a') && (p[0] <= 'h');
    bool bNumber = (p[1] >= '0') && (p[1] <= '7');

    if ((nLen > 1) && (bUpper || bLower) && bNumber)
    {
        nRF = (((p[1] - 1) & 7) << 4) + ((p[0] - 1) & 7);
printf( "RankFile: 0x%02X\n", nRF );
    }

    return nRF;
}


/** Reads input string into arguments
    @Note: Replaces arg seperator (space) with NULL
*/
void GetInputArguments( char *sInput, int nMaxCmds,
    int& nCmds_, char *aCmds_[], int aLens_[] )
{
    char *start = sInput;
    int   iCmds = 0;

    aCmds_[ iCmds ] = sInput;
    aLens_[ iCmds ] = 0;

    for( char *end = sInput; *end && (iCmds < nMaxCmds); end++ )
    {
        if( *end == ' ' || *end == 0xA || *end == 0xD )
        {
            aLens_[ iCmds ] = end - start;
            iCmds++;
            aCmds_[ iCmds ] = start = end + 1;
            *end = 0;
        }
    }

    nCmds_ = iCmds;
}

int main( const int nArg, const char *aArg[] )
{
    StartupCommandLine( nArg, aArg );
    StartupMulticore();

    ChessGame_t game;
    game.Reset();

    bool    bQuit = false;
    int     iNextPlayer = 0;
    int     nSrcRF;
    int     nDstRF;
    int     iSrcPiece;
    int     iDstPiece;

    const int MAX_COMMANDS = 4;
    int     nCmds;
    char   *aCmds[ MAX_COMMANDS ];
    int     aLens[ MAX_COMMANDS ]; // length of each command
    bool    bBadCommand;

(void) iSrcPiece;
(void) iDstPiece;

    // spin-lock
    // wait for command
    while( !bQuit )
    {
        // if (gbBoardValid)
        /*         */ game.Print( gbAnsiOutput );
        iNextPlayer = game.PlayerToPlayNext();
        printf( "%c>", aPLAYERS[ iNextPlayer ] );

        fflush( stdin );
        char   sInput[ 32 ];
        fgets( sInput, sizeof( sInput ), stdin );

        GetInputArguments( sInput, MAX_COMMANDS, nCmds, aCmds, aLens );

        bBadCommand = false;
        if( aLens[0] == 1 )
        {
            switch( aCmds[0][0] )
            {
                case 'e':
                    // <Piece>Location
                    printf( "Edit: %s\n", aCmds[1] );

                    for( int iPiece = 0; iPiece < (int) sizeof( aPIECES ); iPiece++ )
                    {
                        if( aPIECES[ iPiece ] == aCmds[1][0] )
                            iSrcPiece = iPiece;
                    }

                    iDstPiece = iSrcPiece & 7;
                    if ((iDstPiece >= PIECE_PAWN ) && (iDstPiece <= PIECE_KING))
                    {
                        int color = PLAYER_WHITE;
                        if( iSrcPiece > NUM_PIECES)
                            color = PLAYER_BLACK;

                        nDstRF = GetInputLocation( aCmds[1]+1, aLens[1]-1 );
                        if (nDstRF == INVALID_LOCATION)
                            printf( "Error. Invalid location\n" );
                        else
                        {
                            game.Edit( color, iDstPiece, nDstRF );
                        }
                    }
                    else
                            printf( "Error. Invalid piece. Valid: PRNBQKprnbqk\n" );
                    break;

                case 'm':
                    printf( "Param: %s\n", aCmds[1] );
                    nSrcRF = GetInputLocation( aCmds[1]+0, aLens[1]-0 );
                    nDstRF = GetInputLocation( aCmds[1]+2, aLens[1]-2 );
                    if ((nSrcRF == INVALID_LOCATION) || (nDstRF == INVALID_LOCATION))
                        printf( "Error. Invalid location\n" );
                    else
                    {
                        //iSrcPiece = BoardGetPiece( nSrcRF );
                        //iDstPiece = BoardGetPiece( nDsrRF );
                        //bool bDirectCaptures  = // Player x Enemy
                        //bool bIndirectCapture = // pawn: en-passant
                        if( game.MoveOrCapture( nSrcRF, nDstRF ) )
                            game.NextTurn();
                    }
                    break;

                case 'q':
                    printf( "Quiting...\n" );
                    // TODO: Send STOP SHUTDOWN to search threads
                    bQuit = true;
                    break;

                default:
                    bBadCommand = true;
                    break;
            }
        }
        else
        if( aLens[0] == 2 )
        {
            if( strcmp( aCmds[0], "ng" ) == 0 )
                game.Init();
            else
                bBadCommand = true;
        }
        else
        {
            if( strcmp( aCmds[0], "quit" ) == 0 )
                bQuit = true;
            else
            if( strcmp( aCmds[0], "random") == 0 )
                 ; // ignore xboard command
            else
            if( strcmp( aCmds[0], "new" ) == 0 )
                game.Init();
            else
                 bBadCommand = true;
        }

        if( bBadCommand )
            printf( "Ignored command: %s\n", aCmds[0] );
    }; // while bGameRunning

    ShutdownMulticore();

    return 0;
}

