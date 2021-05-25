/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

bool inputFilter(const string_q& input, const COptions* opt);
//-----------------------------------------------------------------------
bool acct_Display(CTraverser* trav, void* data1) {
    COptions* opt = (COptions*)data1;

    trav->nProcessed++;
    if (opt->freshenOnly)
        return true;

    if (inputFilter(trav->trans.input, opt)) {
        if (opt->accounting) {
            blknum_t next = trav->index < opt->apps.size() - 1 ? opt->apps[trav->index + 1].blk : NOPOS;
            process_reconciliation(opt, trav->trans, opt->prevStatements, next, opt->tokens);
        }
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trav->trans;
        opt->firstOut = false;
    }

    prog_Log(trav, data1);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool acct_Pre(CTraverser* trav, void* data1) {
    COptions* opt = (COptions*)data1;

    if (opt->apps.size() > 0 && opt->first_record != 0) {
        CReconciliation eth;
        eth.blockNumber = opt->apps[0].blk - 1;
        eth.endBal = getBalanceAt(opt->accountedFor, opt->apps[0].blk - 1);
        opt->prevStatements[opt->accountedFor + "_eth"] = eth;
    }
    return true;
}

//-----------------------------------------------------------------------
bool inputFilter(const string_q& input, const COptions* opt) {
    if (opt->fourbytes.empty())
        return true;
    if (opt->freshenOnly)
        return true;
    for (auto four : opt->fourbytes)
        if (startsWith(input, four))
            return true;
    return false;
}
