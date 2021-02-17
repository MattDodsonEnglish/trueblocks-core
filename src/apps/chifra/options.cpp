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
    COption("commands", "", "<string>", OPT_REQUIRED | OPT_POSITIONAL, "which command to run"),
    COption("", "", "", OPT_DESCRIPTION, "Control all options of the TrueBlocks tools."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern string_q getSubcommands(void);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    bool tool_help = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
            // END_CODE_AUTO

        } else if (arg == "-h" || arg == "--help") {
            if (mode.empty() || mode == "serve") {
                optionOn(OPT_HELP);
                verbose = true;
                return usage();
            }
            setenv("PROG_NAME", ("chifra " + mode).c_str(), true);
            tool_help = true;

        } else if (mode.empty() && startsWith(arg, '-')) {
            if (!builtInCmd(arg))
                return usage("Missing mode: " + arg);

        } else {
            bool exists = false;
            string_q lower = toLower(arg);
            for (auto a : addrs) {
                if (a == lower)
                    exists = true;
            }

            string subCommands = "|" + getSubcommands() + "|";
            bool isStatus = (mode == "status");
            if (!isStatus && contains(subCommands, "|" + arg + "|")) {
                if (!mode.empty())
                    return usage("Please specify one of " + getSubcommands() + ". " + mode + ":" + arg);
                mode = arg;


            } else if (isAddress(arg) || arg == "--known" || arg == "--monitored") {
                if (!exists)
                    addrs.push_back(lower);

            } else if (mode == "when") {
                if (!exists)
                    addrs.push_back(lower);

            } else {
                tool_flags += (arg + " ");
            }
        }
    }

    string_q origMode = mode;
    if (mode.empty()) {
        optionOn(OPT_HELP);
        verbose = true;
        return usage("You must specify a command to run.");
    }

    // Handle base layer options
    if (tool_help)
        tool_flags += " --help";

    if (isNoHeader)
        tool_flags += " --no_header";

    if (expContext().asEther)
        tool_flags += " --ether";

    if (expContext().asDollars)
        tool_flags += " --dollars";

    if (expContext().asParity)
        tool_flags += " --parity";

    if (verbose && !contains(tool_flags, "-v"))
        tool_flags += (" -v:" + uint_2_Str(verbose));

    tool_flags += addExportMode(expContext().exportFmt);
    tool_flags = trim(tool_flags, ' ');

    if (contains(tool_flags, "help")) {
        if (cmdMap[mode].empty())
            return usage("Incorrect mode: " + mode + ".");
        ostringstream cmd;
        cmd << cmdMap[mode] << " --help";
        if (verbose)
            cmd << " --verbose " << verbose << endl;
        // clang-format off
        if (system(cmd.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        return false;
    }

    // if (mocked) {
    //     string_q which = origMode;
    //     if (origMode == "names") {
    //         if (contains(tool_flags, "tags")) {
    //             origMode = "tags";
    //         } else if (contains(tool_flags, "collections")) {
    //             origMode = "collections";
    //         }
    //     } else if (origMode == "status") {
    //         if (contains(tool_flags, "monitors")) {
    //             origMode = "monitors";
    //         }
    //     }

    //     uint64_t nMocked = getGlobalConfig("")->getConfigInt("dev", "n_mocked", 100);
    //     string_q path = configPath("mocked/" + origMode + ".json");
    //     if (fileExists(path)) {
    //         if (origMode == "export") {
    //             // simulate listing
    //             for (size_t i = 0; i < nMocked; i++) {
    //                 LOG_PROGRESS1("Extracting", i, nMocked, "\r");
    //                 usleep(30000);
    //             }
    //             CStringArray lines;
    //             asciiFileToLines(path, lines);
    //             size_t cnt = 0;
    //             size_t record = 0;
    //             size_t recordSize = lines.size() / nMocked;
    //             for (auto line : lines) {
    //                 cout << line << endl;
    //                 if (!(++cnt % recordSize)) {
    //                     LOG_PROGRESS1("Displaying", record++, nMocked, "\r");
    //                     usleep(10000);
    //                 }
    //             }
    //             return false;
    //         } else {
    //             cout << asciiFileToString(path);
    //             return false;
    //         }
    //     }
    //     tool_flags += " --mocked ";
    // }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_HELP | OPT_CRUD | OPT_OUTPUT | OPT_VERBOSE | OPT_FMT);

    // BEG_CODE_INIT
    // END_CODE_INIT

    addrs.clear();
    tool_flags = "";
    mode = "";
    minArgs = 0;
}

extern const char* STR_FULL_HELP;
//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // clang-format off
    notes.push_back(STR_FULL_HELP);
    // clang-format on

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q addExportMode(format_t fmt) {
    if (isApiMode() && fmt == API1)
        return "";
    if (!isApiMode() && fmt == TXT1)
        return "";
    switch (fmt) {
        case NONE1:
            return " --fmt none";
        case JSON1:
            return " --fmt json";
        case TXT1:
            return " --fmt txt";
        case CSV1:
            return " --fmt csv";
        case API1:
            return " --fmt api";
        default:
            break;
    }
    return "";
}

//------------------------------------------------------------------------------------------------
bool COptions::handle_commands(void) {
    ENTER("handle_" + mode);
    nodeRequired();

    if (mode == "scrape") {
        if (isApiMode() && contains(tool_flags, "run")) {
            cout << "{ \"status\": \"cannot run\" }";
            LOG_ERR(
                "Use the API only to pause, restart, or quit the scraper -- to run, start in a new window with chifra "
                "scrape run.");
            return EXIT_FAILURE;
        }
        // setenv("FRESHEN_FLAG S", freshen_flags.c_str(), true);
    }

    // URLs require key/value pairs, command lines don't so we remove unneeded keys
    CStringArray removes = {"--names", "--terms", "--addrs"};
    for (auto remove : removes)
        if (remove != "--addrs" || mode != "blocks")
            tool_flags = substitute(tool_flags, remove, "");

    CStringArray scraper = {"--restart", "--pause", "--quit"};
    for (auto cmd : scraper)
        replace(tool_flags, cmd, substitute(cmd, "--", ""));

    ostringstream os;
    os << cmdMap[mode] << " " << tool_flags;
    for (auto addr : addrs)
        os << " " << addr;

    // clang-format off
    LOG_CALL(os.str());
    return system(os.str().c_str());
    // clang-format on
}

//------------------------------------------------------------------------------------------------
map<string, string> cmdMap = {{"slurp", "ethslurp"},
                              {"collections", "ethNames --collections"},
                              {"names", "ethNames"},
                              {"tags", "ethNames --tags"},
                              {"abis", "grabABI"},
                              {"blocks", "getBlock"},
                              {"transactions", "getTrans"},
                              {"receipts", "getReceipt"},
                              {"logs", "getLogs"},
                              {"traces", "getTrace"},
                              {"quotes", "ethQuote"},
                              {"state", "getState"},
                              {"tokens", "getTokenInfo"},
                              {"when", "whenBlock"},
                              {"where", "whereBlock"},
                              {"status", "cacheStatus"},
                              {"monitors", "acctExport monitors"},
                              {"export", "acctExport export"},
                              {"scrape", "blockScrape"},
                              {"dive", "turboDive"},
                              {"serve", "tbServer"},
                              {"pins", "pinStatus"},
                              {"explore", "ethscan.py"}};

//------------------------------------------------------------------------------------------------
const char* cmdList =
    "monitor|"
    "export|"
    "collections|"
    "names|"
    "tags|"
    "abis|"
    "blocks|"
    "transactions|"
    "receipts|"
    "logs|"
    "traces|"
    "state|"
    "tokens|"
    "when|"
    "init|"
    "scrape|"
    "serve|"
    "pins|"
    "status|"
    "explore|"
    "slurp|"
    "quotes|"
    "where|";

//------------------------------------------------------------------------------------------------
string_q getSubcommands(void) {
    CStringArray cmds;
    explode(cmds, cmdList, '|');
    ostringstream out;
    for (auto cmd : cmds)
        out << cmd << "|";
    string_q l = trim(out.str(), '|');
    return "list<enum[" + l + "]>";
}

//------------------------------------------------------------------------------------------------
const char* STR_FULL_HELP = 
        "Get more detailed help with `'chifra <cmd> --help'`.|"
                "MONITORS|"
                "  monitor       add, remove, clean, and list appearances of address(es) on the chain|"
                "  export        export details for each appearance (as transacitons, logs, traces, balances, etc.)|"
                "SHARED DATA|"
                "  collections   list and/or share collections (groups of addresses)|"
                "  names         list and/or share named addresses|"
                "  tags          list and/or share tags (subgroups of addresses)|"
                "  abis          list and/or share abi signatures|"
                "BLOCKCHAIN DATA|"
                "  blocks        export block-related data|"
                "  transactions  export transaction-related data|"
                "  receipts      export receipt-related data|"
                "  logs          export log-related data|"
                "  traces        export trace-related data|"
                "  state         export parts of the state for given address(es)|"
                "  tokens        export data related to ERC20 and/or ERC721 token(s)|"
                "  when          return a date given a block number or a block number given a date|"
                "ADMIN|"
                "  init          initialize TrueBlocks databases|"
                "  scrape        scrape the chain and build an index of address appearances (aka digests)|"
                "  serve         serve the TrueBlocks API via tbServer|"
                "  pins          query the status of the pinning system|"
                "  status        query the status of the system|"
                "OTHER|"
                "  explore       open the configured block explorer for the given address|"
                "  slurp         export details by querying EtherScan (note: will not return as many appearances as --list)|"
                "  quotes        return prices collected from configured remote API|"
                "  where         determine the location of block(s), either local or remote cache, or on-chain";
