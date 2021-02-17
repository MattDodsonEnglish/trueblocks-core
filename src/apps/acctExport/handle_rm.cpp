/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

const char* STR_NOTFOUND = "Monitor [{ADDRESS}] not found";
const char* STR_DELETED = "Monitor [{ADDRESS}] was deleted but not removed";
const char* STR_UNDELETED = "Monitor [{ADDRESS}] was undeleted";
const char* STR_REMOVED = "Monitor [{ADDRESS}] was permentantly removed";
const char* STR_DELETEFIRST = "Monitor [{ADDRESS}] must be deleted before it can be removed";

//------------------------------------------------------------------------------------------------
bool COptions::handle_rm(const CAddressArray& addrs) {
    CStringArray results;
    for (auto monitor : allMonitors) {
        if (!monitor.exists()) {
            results.push_back(monitor.Format(STR_NOTFOUND));
            LOG_WARN(monitor.Format(STR_NOTFOUND));
        } else {
            if (crudCommand == "remove") {
                if (monitor.isDeleted()) {
                    monitor.removeMonitor();
                    results.push_back(monitor.Format(STR_REMOVED));
                } else {
                    results.push_back(monitor.Format(STR_DELETEFIRST));
                }
            } else {
                monitor.isDeleted() ? monitor.undeleteMonitor() : monitor.deleteMonitor();
                results.push_back(monitor.Format(monitor.isDeleted() ? STR_DELETED : STR_UNDELETED));
            }
            LOG_INFO(results[results.size() - 1]);
        }
    }

    if (isApiMode()) {
        expContext().exportFmt = JSON1;
        cout << exportPreamble("", "");
        string_q msg;
        bool first = true;
        for (auto remove : results) {
            if (!first)
                msg += ",";
            msg += ("\"" + remove + "\"");
            first = false;
        }
        if (msg.empty())
            msg = "{ \"msg\": \"nothing was removed\" }";
        cout << msg;
        cout << exportPostamble(errors, "") << endl;
    }

    return false;  // do not continue
}
