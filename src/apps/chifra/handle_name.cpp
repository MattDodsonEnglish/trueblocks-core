/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_name(void) {

    // name mode does not require a running node
    nodeNotRequired();

    ostringstream os;
    os << "ethName " << tool_flags << " ; ";
    if (isTestMode())
        cout << os.str() << endl;
    else
        if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings

    return true;
}
