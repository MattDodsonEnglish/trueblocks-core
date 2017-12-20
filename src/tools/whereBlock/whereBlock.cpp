/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    etherlib_init("binary");

    setNoTracing(true); // we never need to trace in this app

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (!options.alone) {
            SFString cachePath = blockCachePath("");
            if (isTestMode())
                cachePath = "--";
            cout << cYellow << "\nReport on block locations:" << cOff << (verbose ? "" : "\n  (cache folder: " + cachePath + ")") << "\n";
        }

        SFString list = options.getBlockNumList();
        while (!list.empty()) {
            blknum_t bn = toLongU(nextTokenClear(list, '|'));
            CFilename fileName(getBinaryFilename1(bn));
            bool exists = fileExists(fileName.getFullPath());

            if (options.alone) {
                // When running in 'alone' mode, only report items in the cache
                if (exists)
                    cout << fileName.getFullPath() << "\n";

            } else {

                SFString path = (verbose ? fileName.getFullPath() : fileName.relativePath(getStorageRoot()));
                SFString vers = getVersionFromClient();
                if (isTestMode() && verbose)
                    path = "--";
                if (isTestMode())
                    vers = "--";
                SFString fallback = getenv("FALLBACK");
                bool running_node = isNodeRunning();

                cout << "\tblock " << cTeal << padLeft(asStringU(bn),9) << cOff << " ";
                     if (exists)            cout << "found at cache:  " << cTeal << path << cOff << "\n";
                else if (running_node)      cout << "found at node:   " << vers << "\n";
                else if (!fallback.empty()) cout << "found at remote: " << fallback << "\n";
                else                        cout << "was not found\n";
            }
        }
    }
    return 0;
}
