/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    // Initialize the library
    etherlib_init(quickQuitHandler);

    // Visit every block between the first and the most recent
    forEveryBlock(visitBlock, nullptr, 0, 100000, 500);

    // Clean up
    etherlib_cleanup();
    return 0;
}

//----------------------------------------------------------------
// For each block
bool visitBlock(CBlock& block, void* data) {
    // Visit each tranaction and show it seperately
    cerr << block.blockNumber << "\t" << block.transactions.size() << "\r";
    cerr.flush();
    for (auto trans : block.transactions)
        visitTransaction(trans, data);
    return true;
}

//----------------------------------------------------------------
// For each transaction in the block
bool visitTransaction(CTransaction& trans, void* data) {
    // Simply print the transaction to the screen
    cout << trans << endl;
    return true;
}
