/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();  // not every command needs a node
    acctlib_init(quickQuitHandler);

    ENTER("chifra");

    int r = EXIT_FAILURE;
    COptions options;
    if (!options.prepareArguments(argc, argv)) {
        RETURN(r);

    } else if (options.parseArguments(options.commandLines[0])) {
        r = options.handle_commands();

    }
    
    acctlib_cleanup();
    EXIT_NOMSG(r);
}
