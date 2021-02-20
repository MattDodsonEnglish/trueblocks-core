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

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more addresses (0x...) to export"),
    COption("topics", "", "list<topic>", OPT_POSITIONAL, "filter by one or more logs topics (only for --logs option)"),
    COption("appearances", "p", "", OPT_SWITCH, "export a list of appearances"),
    COption("receipts", "r", "", OPT_SWITCH, "export receipts instead of transaction list"),
    COption("logs", "l", "", OPT_SWITCH, "export logs instead of transaction list"),
    COption("traces", "t", "", OPT_SWITCH, "export traces instead of transaction list"),
    COption("statements", "T", "", OPT_SWITCH, "export reconcilations instead of transaction list"),
    COption("accounting", "C", "", OPT_SWITCH, "export accounting records instead of transaction list"),
    COption("articulate", "a", "", OPT_SWITCH, "articulate transactions, traces, logs, and outputs"),
    COption("cache_txs", "i", "", OPT_SWITCH, "write transactions to the cache (see notes)"),
    COption("cache_traces", "R", "", OPT_SWITCH, "write traces to the cache (see notes)"),
    COption("skip_ddos", "d", "", OPT_HIDDEN | OPT_TOGGLE, "toggle skipping over 2016 dDos transactions ('on' by default)"),  // NOLINT
    COption("max_traces", "m", "<uint64>", OPT_HIDDEN | OPT_FLAG, "if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250)"),  // NOLINT
    COption("freshen", "f", "", OPT_HIDDEN | OPT_SWITCH, "freshen but do not print the exported data"),
    COption("freshen_max", "F", "<uint64>", OPT_HIDDEN | OPT_FLAG, "maximum number of records to process for --freshen option"),  // NOLINT
    COption("factory", "y", "", OPT_HIDDEN | OPT_SWITCH, "scan for contract creations from the given address(es) and report address of those contracts"),  // NOLINT
    COption("emitter", "M", "", OPT_HIDDEN | OPT_SWITCH, "available for --logs option only, export will only export if the address emitted the event"),  // NOLINT
    COption("count", "U", "", OPT_SWITCH, "only available for --appearances mode, if present return only the number of records"),  // NOLINT
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_DEPRECATED, "first block to process (inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_DEPRECATED, "last block to process (inclusive)"),
    COption("first_record", "c", "<blknum>", OPT_HIDDEN | OPT_FLAG, "the first record to process"),
    COption("max_records", "e", "<blknum>", OPT_HIDDEN | OPT_FLAG, "the maximum number of records to process before reporting"),  // NOLINT
    COption("clean", "", "", OPT_HIDDEN | OPT_SWITCH, "clean (i.e. remove duplicate appearances) from all existing monitors"),  // NOLINT
    COption("staging", "s", "", OPT_HIDDEN | OPT_SWITCH, "enable search of staging (not yet finalized) folder"),
    COption("unripe", "u", "", OPT_HIDDEN | OPT_SWITCH, "enable search of unripe (neither staged nor finalized) folder (assumes --staging)"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Export full detail of transactions for one or more Ethereum addresses."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER("parseArguments");
    if (!standardOptions(command))
        EXIT_NOMSG(false);

    // BEG_CODE_LOCAL_INIT
    CAddressArray addrs;
    CTopicArray topics;
    blknum_t start = NOPOS;
    blknum_t end = NOPOS;
    bool staging = false;
    bool unripe = false;
    // END_CODE_LOCAL_INIT

    blknum_t unripeBlk, ripeBlk, stagingBlk, finalizedBlk;
    getLatestBlocks(unripeBlk, ripeBlk, stagingBlk, finalizedBlk, latestBlock);

    blknum_t latest = latestBlock;
    string_q origCmd = command;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-p" || arg == "--appearances") {
            appearances = true;

        } else if (arg == "-r" || arg == "--receipts") {
            receipts = true;

        } else if (arg == "-l" || arg == "--logs") {
            logs = true;

        } else if (arg == "-t" || arg == "--traces") {
            traces = true;

        } else if (arg == "-T" || arg == "--statements") {
            statements = true;

        } else if (arg == "-C" || arg == "--accounting") {
            accounting = true;

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (arg == "-i" || arg == "--cache_txs") {
            cache_txs = true;

        } else if (arg == "-R" || arg == "--cache_traces") {
            cache_traces = true;

        } else if (arg == "-d" || arg == "--skip_ddos") {
            skip_ddos = !skip_ddos;

        } else if (startsWith(arg, "-m:") || startsWith(arg, "--max_traces:")) {
            if (!confirmUint("max_traces", max_traces, arg))
                return false;

        } else if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (startsWith(arg, "-F:") || startsWith(arg, "--freshen_max:")) {
            if (!confirmUint("freshen_max", freshen_max, arg))
                return false;

        } else if (arg == "-y" || arg == "--factory") {
            factory = true;

        } else if (arg == "-M" || arg == "--emitter") {
            emitter = true;

        } else if (arg == "-U" || arg == "--count") {
            count = true;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--start:")) {
            if (!confirmUint("start", start, arg))
                return false;

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--end:")) {
            if (!confirmUint("end", end, arg))
                return false;

        } else if (startsWith(arg, "-c:") || startsWith(arg, "--first_record:")) {
            if (!confirmBlockNum("first_record", first_record, arg, latest))
                return false;

        } else if (startsWith(arg, "-e:") || startsWith(arg, "--max_records:")) {
            if (!confirmBlockNum("max_records", max_records, arg, latest))
                return false;

        } else if (arg == "--clean") {
            clean = true;

        } else if (arg == "-s" || arg == "--staging") {
            staging = true;

        } else if (arg == "-u" || arg == "--unripe") {
            unripe = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else if (isAddress(arg)) {
            if (!parseAddressList2(this, addrs, arg))
                return false;

        } else {
            if (!parseTopicList2(this, topics, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (clean) {
        if (!handle_clean())
            return usage("Clean function returned false.");
        return false;
    }

    // Handle the easy cases first...
    if (isCrudCommand()) {
        if (crudCommand == "delete" || crudCommand == "undelete" || crudCommand == "remove")
            return handle_rm(addrs);
        return usage("You may only use --delete, --undelete, or --remove on monitors.");
    }

    // We need at least one address to scrape...
    if (addrs.size() == 0)
        EXIT_USAGE("You must provide at least one Ethereum address.");

    if ((appearances + receipts + logs + traces) > 1)
        EXIT_USAGE("Please export only one of list, receipts, logs, or traces.");

    if (emitter && !logs)
        EXIT_USAGE("The emitter option is only available when exporting logs.");

    if (emitter && addrs.size() > 1)
        EXIT_USAGE("The emitter option is only available when exporting logs from a single address.");

    if (factory && !traces)
        EXIT_USAGE("The facotry option is only available when exporting traces.");

    if (count && (receipts || logs || traces || emitter || factory))
        EXIT_USAGE("--count option is only available with --appearances option.");

    if ((accounting || statements) && (addrs.size() != 1))
        EXIT_USAGE("You may only use --accounting option with a single address.");

    if ((accounting || statements) && freshen)
        EXIT_USAGE("Do not use the --accounting option with --freshen.");

    if ((accounting || statements) && (appearances || logs || traces))
        EXIT_USAGE("Do not use the --accounting option with other options.");

    // Where will we start?
    blknum_t firstBlockToVisit = NOPOS;

    for (auto addr : addrs) {
        CMonitor monitor;
        monitor.setValueByName("address", toLower(addr));
        monitor.setValueByName("name", toLower(addr));
        monitor.clearLocks();
        monitor.finishParse();
        monitor.fm_mode = (fileExists(monitor.getMonitorPath(monitor.address)) ? FM_PRODUCTION : FM_STAGING);
        if (monitor.exists()) {
            string_q unused;
            if (monitor.isLocked(unused))
                LOG_ERR(
                    "The cache file is locked. The program is either already "
                    "running or it did not end cleanly the\n\tlast time it ran. "
                    "Quit the already running program or, if it is not running, "
                    "remove the lock\n\tfile: " +
                    monitor.getMonitorPath(addr) + +".lck'. Proceeding anyway...");
            string_q msg;
            if (monitor.isLocked(msg))  // If locked, we fail
                EXIT_USAGE(msg);
            firstBlockToVisit = min(firstBlockToVisit, monitor.getLastVisited());
            LOG_TEST("Monitor found for", addr);
            LOG_TEST("Monitor path", monitor.getMonitorPath(monitor.address));
            LOG_TEST("Last visited block", monitor.getLastVisitedBlock());
        } else {
            LOG_TEST("Monitor not found for", addr);
        }
        allMonitors.push_back(monitor);
    }

    if (!setDisplayFormatting())
        return false;

    if (appearances || count)
        articulate = false;
    if (articulate) {
        abi_spec.loadAbisFromKnown();
        for (auto monitor : allMonitors) {
            if (isContractAt(monitor.address, latestBlock))
                abi_spec.loadAbiFromEtherscan(monitor.address);
        }
    }

    // Are we visiting unripe and/or staging in our search?
    if (staging)
        visitTypes |= VIS_STAGING;
    if (unripe) {
        if (!(visitTypes & VIS_STAGING))
            EXIT_USAGE("You must also specify --staging when using --unripe.");
        visitTypes |= VIS_UNRIPE;
    }

    // Last block depends on scrape type or user input `end` option (with appropriate check)
    blknum_t lastBlockToVisit = max((blknum_t)1, (visitTypes & VIS_UNRIPE)    ? unripeBlk
                                                 : (visitTypes & VIS_STAGING) ? stagingBlk
                                                                              : finalizedBlk);

    // Mark the range...
    listRange = make_pair((firstBlockToVisit == NOPOS ? 0 : firstBlockToVisit), lastBlockToVisit);

    if (!freshen_internal())  // getEnvStr("FRESHEN_FLAG S")))
        return usage("'freshen_internal' returned false.");

    if (count) {
        for (auto monitor : allMonitors) {
            CMonitorCount monCount;
            monCount.address = monitor.address;
            monCount.fileSize = fileSize(monitor.getMonitorPath(monitor.address));
            monCount.nRecords = monCount.fileSize / sizeof(CAppearance_base);
            counts.push_back(monCount);
        }
    }

    // If the chain is behind the monitor (for example, the user is re-syncing), quit silently...
    if (latest < listRange.first) {
        LOG4("Chain is behind the monitor.");
        EXIT_NOMSG(false);
    }

    // If there's nothing to scrape, quit silently...
    if (listRange.first >= listRange.second) {
        LOG8("Account scraper is up to date.");
        EXIT_NOMSG(false);
    }

    if (start != NOPOS)
        exportRange.first = start;
    if (end != NOPOS)
        exportRange.second = end;

    //    // Accumulate the addresses into the allMonitors list and decide where we should start
    //    for (auto addr : addrs) {
    //        CMonitor monitor;
    //
    //        monitor.setValueByName("address", toLower(addr));
    //        monitor.setValueByName("name", toLower(addr));
    //
    //        if (monitor.exists()) {
    //            string_q unused;
    //            if (monitor.isLocked(unused))
    //                LOG_ERR(
    //                        "The cache file is locked. The program is either already "
    //                        "running or it did not end cleanly the\n\tlast time it ran. "
    //                        "Quit the already running program or, if it is not running, "
    //                        "remove the lock\n\tfile: " +
    //                        monitor.getMonitorPath(addr) + +".lck'. Proceeding anyway...");
    //            monitor.clearLocks();
    //            monitor.finishParse();
    //            monitor.fm_mode = (fileExists(monitor.getMonitorPath(monitor.address)) ? FM_PRODUCTION :
    //            FM_STAGING); string_q msg; if (monitor.isLocked(msg))  // If locked, we fail
    //                EXIT_USAGE(msg);
    //            firstBlockToVisit = min(firstBlockToVisit, monitor.getLastVisited());
    //        } else {
    //            monitor.clearLocks();
    //            monitor.finishParse();
    //            monitor.fm_mode = (fileExists(monitor.getMonitorPath(monitor.address)) ? FM_PRODUCTION :
    //            FM_STAGING); cleanMonitorStage(); if (visitTypes & VIS_FINAL)
    //                forEveryFileInFolder(indexFolder_blooms, visitFinalIndexFiles, this);
    //            if (visitTypes & VIS_STAGING)
    //                forEveryFileInFolder(indexFolder_staging, visitStagingIndexFiles, this);
    //            if (visitTypes & VIS_UNRIPE)
    //                forEveryFileInFolder(indexFolder_unripe, visitUnripeIndexFiles, this);
    //            //            for (auto monitor : allMonitors) {
    //            monitor.moveToProduction();
    //            LOG4(monitor.address, " freshened to ", monitor.getLastVisited(true /* fresh */));
    //            //            }
    //            string_q msg;
    //            if (monitor.isLocked(msg))  // If locked, we fail
    //                EXIT_USAGE(msg);
    //            firstBlockToVisit = min(firstBlockToVisit, monitor.getLastVisited());
    //        }
    //        if (monitor.exists()) {
    //            allMonitors.push_back(monitor);
    //        } else {
    //            LOG4("Monitor not found: ", monitor.address, ". Skipping...");
    //        }
    //    }

    LOG_TEST("nMonitors", allMonitors.size());
    LOG_TEST("exportRange.first", exportRange.first);
    LOG_TEST("exportRange.second", exportRange.second);
    LOG_TEST("listRange.first", listRange.first);
    LOG_TEST("listRange.second", listRange.second);
    LOG_TEST("first_record", first_record);
    LOG_TEST("max_records", max_records);
    LOG_TEST_BOOL("appearances", appearances);
    LOG_TEST_BOOL("receipts", receipts);
    LOG_TEST_BOOL("logs", logs);
    LOG_TEST_BOOL("traces", traces);
    LOG_TEST_BOOL("statements", statements);
    LOG_TEST_BOOL("articulate", articulate);
    LOG_TEST_BOOL("freshen", freshen);
    LOG_TEST_BOOL("factory", factory);
    LOG_TEST_BOOL("emitter", emitter);
    LOG_TEST_BOOL("count", count);
    LOG_TEST_BOOL("clean", clean);

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND);
    optionOn(OPT_CRUD);
    // Since we need prefunds, let's load the names library here
    CAccountName unused;
    getNamedAccount(unused, "0x0");

    // BEG_CODE_INIT
    appearances = false;
    receipts = false;
    logs = false;
    traces = false;
    statements = false;
    accounting = false;
    articulate = false;
    cache_txs = getGlobalConfig("acctExport")->getConfigBool("settings", "cache_txs", false);
    cache_traces = getGlobalConfig("acctExport")->getConfigBool("settings", "cache_traces", false);
    skip_ddos = getGlobalConfig("acctExport")->getConfigBool("settings", "skip_ddos", true);
    max_traces = getGlobalConfig("acctExport")->getConfigInt("settings", "max_traces", 250);
    freshen = false;
    freshen_max = 5000;
    factory = false;
    emitter = false;
    count = false;
    first_record = 0;
    max_records = NOPOS;
    clean = false;
    // END_CODE_INIT

    nProcessed = 0;
    nProcessing = 0;
    nTransactions = 0;
    nCacheItemsRead = 0;
    nCacheItemsWritten = 0;
    listRange.second = getLatestBlock_cache_ripe();

    allMonitors.clear();
    counts.clear();
    apps.clear();

    expContext().accountedFor = "";
    bytesOnly = "";

    // We don't clear these because they are part of meta data
    // prefundAddrMap.clear();
    // blkRewardMap.clear();
    // toNameExistsMap.clear();
    // fromNameExistsMap.clear();
    // abiMap.clear();

    oldestMonitor = latestDate;

    minArgs = 0;
    fileRange = make_pair(NOPOS, NOPOS);
    visitTypes = VIS_FINAL;
    allMonitors.clear();
    possibles.clear();

    // Establish folders. This may be redundant, but it's cheap.
    establishMonitorFolders();
    establishFolder(indexFolder);
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_unripe);
    establishFolder(indexFolder_ripe);
    establishFolder(configPath("cache/tmp/"));
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    ts_array = NULL;
    ts_cnt = 0;
    Init();

    CMonitorCount::registerClass();
    CAppearanceDisplay::registerClass();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("`addresses` must start with '0x' and be forty two characters long.");
    notes.push_back("By default, transactions and traces are cached if the number of exported | items is <= to 1,000 items. Otherwise, if you specify any `write_*` options, | your preference predominates.");  // NOLINT
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::setDisplayFormatting(void) {
    ENTER("setDisplayFormatting");

    if (count) {
        bool isText = expContext().exportFmt != JSON1 && expContext().exportFmt != API1;
        string_q format =
            getGlobalConfig("acctExport")->getConfigStr("display", "format", isText ? STR_DISPLAY_MONITORCOUNT : "");
        expContext().fmtMap["monitorcount_fmt"] = cleanFmt(format);
        expContext().fmtMap["header"] = isNoHeader ? "" : cleanFmt(format);

    } else {
        string_q hide = substitute(defHide, "|CLogEntry: data, topics", "");
        manageFields(hide, false);
        string_q show =
            defShow + (isApiMode() ? "|CTransaction:encoding,function,input,etherGasCost|CTrace:traceAddress" : "");
        manageFields(show, true);
        if (expContext().exportFmt != JSON1 && expContext().exportFmt != API1) {
            string_q format;

            format = getGlobalConfig("acctExport")->getConfigStr("display", "format", STR_DISPLAY_TRANSACTION);
            expContext().fmtMap["transaction_fmt"] = cleanFmt(format);
            manageFields("CTransaction:" + format);

            if (format.empty())
                EXIT_USAGE("For non-json export a 'trans_fmt' string is required. Check your config file.");
            if (!contains(toLower(format), "trace"))
                HIDE_FIELD(CTransaction, "traces");

            format = getGlobalConfig("acctExport")->getConfigStr("display", "receipt", STR_DISPLAY_RECEIPT);
            expContext().fmtMap["receipt_fmt"] = cleanFmt(format);
            manageFields("CReceipt:" + format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "log", STR_DISPLAY_LOGENTRY);
            expContext().fmtMap["logentry_fmt"] = cleanFmt(format);
            manageFields("CLogEntry:" + format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "statement", STR_DISPLAY_RECONCILIATION);
            expContext().fmtMap["reconciliation_fmt"] = cleanFmt(format);
            manageFields("CReconciliation:" + format);

            format = getGlobalConfig("acctExport")->getConfigStr("display", "trace", STR_DISPLAY_TRACE);
            expContext().fmtMap["trace_fmt"] = cleanFmt(format);
            manageFields("CTrace:" + format);

            // This doesn't really work because CAppearance_base is not a subclass of CBaseNode. We phony it here for
            // future reference.
            format =
                getGlobalConfig("acctExport")->getConfigStr("display", "appearances", STR_DISPLAY_APPEARANCEDISPLAY);
            expContext().fmtMap["appearancedisplay_fmt"] = cleanFmt(format);
            manageFields("CAppearanceDisplay:" + format);
        }
        HIDE_FIELD(CFunction, "stateMutability");
        HIDE_FIELD(CParameter, "str_default");
        HIDE_FIELD(CParameter, "components");
        HIDE_FIELD(CParameter, "internalType");
        HIDE_FIELD(CTransaction, "datesh");
        HIDE_FIELD(CTransaction, "time");
        HIDE_FIELD(CTransaction, "age");
        SHOW_FIELD(CTransaction, "traces");

        expContext().fmtMap["header"] = "";
        if (!isNoHeader) {
            if (traces) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["trace_fmt"]);
            } else if (receipts) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["receipt_fmt"]);
            } else if (logs) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["logentry_fmt"]);
            } else if (appearances) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["appearancedisplay_fmt"]);
            } else if (statements) {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["reconciliation_fmt"]);
            } else {
                expContext().fmtMap["header"] = cleanFmt(expContext().fmtMap["transaction_fmt"]);
            }
        }

        if (logs) {
            SHOW_FIELD(CLogEntry, "blockNumber");
            SHOW_FIELD(CLogEntry, "transactionIndex");
        }

        if (freshen)
            expContext().exportFmt = NONE1;

        if (accounting || statements) {
            expContext().accountedFor = allMonitors[0].address;
            bytesOnly = substitute(expContext().accountedFor, "0x", "");
            articulate = true;
            manageFields("CTransaction:statements", true);
            manageFields("CTransaction:reconciliations", false);
            bool nodeHasBals = nodeHasBalances(false);
            string_q rpcProvider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");
            if (!nodeHasBals) {
                string_q balanceProvider = getGlobalConfig()->getConfigStr("settings", "balanceProvider", rpcProvider);
                if (rpcProvider == balanceProvider || balanceProvider.empty())
                    EXIT_USAGE("--accounting requires historical balances. The RPC server does not have them.");
                setRpcProvider(balanceProvider);
                if (!nodeHasBalances(false))
                    EXIT_USAGE("balanceServer is set, but it does not have historical state.");
            }
        }
    }
    return true;
}

#define LOG_TEST_VAL(a, b)                                                                                             \
    {                                                                                                                  \
        if (b != 0)                                                                                                    \
            LOG_TEST(a, "--value--")                                                                                   \
    }

//------------------------------------------------------------------------------------------------
bool COptions::freshen_internal(void) {
    LOG_TEST_VAL("stats.nFiles", stats.nFiles);
    LOG_TEST_VAL("stats.nSkipped", stats.nSkipped);
    LOG_TEST_VAL("stats.nChecked", stats.nChecked);
    LOG_TEST_VAL("stats.nBloomMisses", stats.nBloomMisses);
    LOG_TEST_VAL("stats.nBloomHits", stats.nBloomHits);
    LOG_TEST_VAL("stats.nFalsePositive", stats.nFalsePositive);
    LOG_TEST_VAL("stats.nPositive", stats.nPositive);
    LOG_TEST_VAL("stats.nRecords", stats.nRecords);

    // Clean the monitor stage of previously unfinished scrapes
    cleanMonitorStage();

    if (visitTypes & VIS_FINAL)
        forEveryFileInFolder(indexFolder_blooms, visitFinalIndexFiles, this);

    if (visitTypes & VIS_STAGING)
        forEveryFileInFolder(indexFolder_staging, visitStagingIndexFiles, this);

    if (visitTypes & VIS_UNRIPE)
        forEveryFileInFolder(indexFolder_unripe, visitUnripeIndexFiles, this);

    for (auto monitor : allMonitors) {
        monitor.moveToProduction();
        LOG4(monitor.address, " freshened to ", monitor.getLastVisited(true /* fresh */));
    }

    LOG_TEST_VAL("stats.nFiles", stats.nFiles);
    LOG_TEST_VAL("stats.nSkipped", stats.nSkipped);
    LOG_TEST_VAL("stats.nChecked", stats.nChecked);
    LOG_TEST_VAL("stats.nBloomMisses", stats.nBloomMisses);
    LOG_TEST_VAL("stats.nBloomHits", stats.nBloomHits);
    LOG_TEST_VAL("stats.nFalsePositive", stats.nFalsePositive);
    LOG_TEST_VAL("stats.nPositive", stats.nPositive);
    LOG_TEST_VAL("stats.nRecords", stats.nRecords);

    // size_t cnt = 0, cnt2 = 0;
    // string_q tenAddresses;
    // for (auto f : fa) {
    //     bool needsUpdate = true;
    //     if (needsUpdate) {
    //         LOG4(cTeal, "Needs update ", f.address, string_q(80, ' '), cOff);
    //         tenAddresses += (f.address + " ");
    //         if (!(++cnt % 10)) {  // we don't want to do too many addrs at a time
    //             tenAddresses += "|";
    //             cnt = 0;
    //         }
    //     } else {
    //         LOG4(cTeal, "Updating addresses ", f.address, " ", cnt2, " of ", fa.size(), string_q(80, ' '), cOff,
    //         "\r");
    //     }
    //     cnt2++;
    // }

    // // Process them until we're done
    // uint64_t cur = 0;
    // while (!tenAddresses.empty()) {
    //     string_q thisFive = nextTokenClear(tenAddresses, '|');
    //     string_q cmd = substitute(base.str(), "[ADDRS]", thisFive);
    //     // LOG_CALL(cmd);
    //     // clang-format off
    //     uint64_t n = countOf(thisFive, ' ');
    //     if (fa.size() > 1)
    //         LOG_INFO(cTeal, "Updating addresses ", cur+1, "-", (cur+n), " of ", fa.size(), string_q(80, ' '), cOff);
    //     cur += n;
    //     LOG_TEST("cmd: ", cmd);
    //     if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
    //     // clang-format on
    //     if (!tenAddresses.empty())
    //         usleep(50000);  // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
    // }

    // for (CMonitor& f : fa)
    //     f.needsRefresh = (f.cntBefore != f.getRecordCount());

    return true;
    // // EXIT_NOMSG(true);
}

// TODO(tjayrush): If an abi file is changed, we should re-articulate.
// TODO(tjayrush): accounting can not be freshen, appearances, logs, receipts, traces, but must be articulate - why?
// TODO(tjayrush): accounting must be exportFmt API1 - why?
// TODO(tjayrush): accounting must be for one monitor address - why?
// TODO(tjayrush): accounting requires node balances - why?
// TODO(tjayrush): Used to do this: if any ABI files was newer, re-read abi and re-articulate in cache
// TODO(tjayrush): What does prefundAddrMap and prefundWeiMap do? Needs testing
// TODO(tjayrush): What does blkRewardMap do? Needs testing
// TODO(tjayrush): Reconciliation loads traces -- plus it reduplicates the isSuicide, isGeneration, isUncle shit
// TODO(tjayrush): updateLastExport is really weird
// TODO(tjayrush): writeLastBlock is really weird
// TODO(tjayrush): We used to write traces sometimes
// TODO(tjayrush): We used to cache the monitored txs - I think it was pretty fast (we used the monitor staging folder)
