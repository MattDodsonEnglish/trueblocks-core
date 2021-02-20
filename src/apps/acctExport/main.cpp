/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool removeAbi(const string_q& path, void* data);
int main(int argc, const char* argv[]) {
    pinlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        // clang-format off
        options.className =
            (options.count ? GETRUNTIME_CLASS(CMonitorCount)->m_ClassName
             : options.appearances
                 ? GETRUNTIME_CLASS(CAppearance)->m_ClassName
                 : (options.traces
                        ? GETRUNTIME_CLASS(CTrace)->m_ClassName
                        : (options.receipts
                               ? GETRUNTIME_CLASS(CReceipt)->m_ClassName
                               : (options.logs ? GETRUNTIME_CLASS(CLogEntry)->m_ClassName
                                               : (options.statements ? GETRUNTIME_CLASS(CReconciliation)->m_ClassName
                                                                     : GETRUNTIME_CLASS(CTransaction)->m_ClassName)))));
        // clang-format on

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], options.className);

        if (options.loadAllAppearances()) {
            if (options.count) {
                bool first = true;
                for (auto cnt : options.counts) {
                    if (shouldQuit())
                        break;
                    cout << ((isJson() && !first) ? ", " : "");
                    cout << cnt.Format() << endl;
                    first = false;
                }

            } else if (options.appearances) {
                options.handle_appearances();

            } else if (options.receipts) {
                options.handle_receipts();

            } else if (options.traces) {
                options.handle_traces();

            } else if (options.logs) {
                options.handle_logs();

            } else if (options.statements) {
                options.handle_statements();

            } else {
                ASSERT(accounting);
                // for (int i = 0 ; i < 100 ; i++) {
                //    forEveryFileInFolder(getCachePath("abis/"), removeAbi, nullptr);
                options.handle_accounting();
                //}
            }
        }

        once = false;
    }

    ostringstream os;
    os << ", \"start\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.exportRange.first)) << endl;
    os << ", \"end\": " << (isTestMode() ? "\"0xdeadbeef\"" : uint_2_Str(options.exportRange.second)) << endl;
    if (!options.count && options.allMonitors.size() == 1) {
        options.getNamedAccount(options.allMonitors[0], options.allMonitors[0].address);
        HIDE_FIELD(CMonitor, "summaryStatement");
        if (options.abi_spec.nInterfaces() == 0) {
            HIDE_FIELD(CMonitor, "abi_spec");
        }
        os << ", \"accountedFor\": " << options.allMonitors[0] << endl;
    }
    expContext().fmtMap["meta"] += os.str();

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    if (!isTestMode() && !options.freshen && !options.count && !options.accounting && !options.logs &&
        !options.statements) {
        ostringstream oss;
        oss << "Exported " << padNum6T(options.nProcessed) << " ";
        oss << (!options.className.empty() ? (plural(options.className) + " from ") : "of ");
        oss << padNum6T(options.nTransactions) << " transactions for address "
            << (options.allMonitors.size() ? options.allMonitors[0].address : "");
        LOG_INFO(oss.str());
    }

    if (options.stats.nFiles != options.stats.nSkipped) {
        ostringstream header;
        header << options.stats.Format(substitute(substitute(STR_DISPLAY_ACCTSCRAPESTATS, "\t", ","), "{", "{p:"));
        LOG4(header.str());
        header << endl;

        ostringstream data;
        data << options.stats.Format(substitute(STR_DISPLAY_ACCTSCRAPESTATS, "\t", ","));
        LOG4(data.str());
        data << endl;

        string_q statsFile = configPath("performance_scraper.csv");
        if (!fileExists(statsFile))
            stringToAsciiFile(statsFile, header.str());
        appendToAsciiFile(statsFile, data.str());
    }

    pinlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
bool removeAbi(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", removeAbi, data);
    } else {
        ::remove(path.c_str());
    }
    return true;
}

#if 0
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    namesNotRequired();
    pinlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            continue;

        // Clean the monitor stage of previously unfinished scrapes
        cleanMonitorStage();

        if (options.visitTypes & VIS_FINAL)
            forEveryFileInFolder(indexFolder_blooms, visitFinalIndexFiles, &options);

        if (options.visitTypes & VIS_STAGING)
            forEveryFileInFolder(indexFolder_staging, visitStagingIndexFiles, &options);

        if (options.visitTypes & VIS_UNRIPE)
            forEveryFileInFolder(indexFolder_unripe, visitUnripeIndexFiles, &options);

        for (auto monitor : options.allMonitors) {
            monitor.moveToProduction();
            LOG4(monitor.address, " freshened to ", monitor.getLastVisited(true /* fresh */));
        }
    }

    if (options.stats.nFiles != options.stats.nSkipped) {
        ostringstream header;
        header << options.stats.Format(substitute(substitute(STR_DISPLAY_ACCTSCRAPESTATS, "\t", ","), "{", "{p:"));

        ostringstream data;
        data << options.stats.Format(substitute(STR_DISPLAY_ACCTSCRAPESTATS, "\t", ","));

        LOG4(header.str());
        LOG4(data.str());

        header << endl;
        data << endl;

        string_q statsFile = configPath("performance_scraper.csv");
        if (!fileExists(statsFile))
            stringToAsciiFile(statsFile, header.str());
        appendToAsciiFile(statsFile, data.str());
    }

    pinlib_cleanup();
    return 0;
}

#endif
