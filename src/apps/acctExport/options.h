#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "etherlib.h"
#include "acctlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

#define CACHE_NONE (0)
#define CACHE_TXS (1 << 2)
#define CACHE_TRACES (1 << 3)
#define CACHE_BYCONFIG (1 << 4)
#define CACHE_BYUSER (1 << 5)
#define CACHE_BYDEFAULT (1 << 6)

//-----------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    bool appearances;
    bool receipts;
    bool logs;
    bool traces;
    bool statements;
    bool accounting;
    bool articulate;
    bool skip_ddos;
    uint64_t max_traces;
    bool freshen;
    uint64_t freshen_max;
    bool factory;
    bool emitter;
    bool count;
    blknum_t first_record;
    blknum_t max_records;
    bool clean;
    // END_CODE_DECLARE

    int write_opt;  // cache options as resolved (see options.cpp for notes)

    CMonitorArray monitors;
    CMonitorCountArray counts;
    CAppearanceArray_base apps;
    uint64_t nProcessed;
    string_q className;
    address_t hackAppAddr;
    CBlockAddressMap prefundAddrMap;
    CBlockAddressMap blkRewardMap;

    CAddressUintMap toAddrMap;
    CAddressUintMap fromAddrMap;
    CAddressUintMap emitterAddrMap;
    CAddressUintMap creationMap;
    CAddressUintMap toTraceAddrMap;
    CAddressUintMap fromTraceAddrMap;
    CAddressUintMap abiMap;

    uint32_t* ts_array;
    size_t ts_cnt;
    blknum_t latestBlock;
    time_q oldestMonitor;

    string_q bytesOnly;

    uint64_t nProcessing;
    uint64_t nTransactions;
    uint64_t nCacheItemsRead;
    uint64_t nCacheItemsWritten;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool loadOneAddress(CAppearanceArray_base& apps, const address_t& addr);
    bool loadAllAppearances(void);

    bool handle_accounting(void);
    bool hanlde_appearances(void);
    bool handle_logs(void);
    bool handle_receipts(void);
    bool handle_statements(void);
    bool handle_traces(void);
    bool handle_clean(void);

    void addNeighbor(CAddressUintMap& map, const address_t& addr);
    void markNeighbors(const CTransaction& trans);
    void articulateAll(CTransaction& trans);
    bool reportNeighbors(void);
};

//--------------------------------------------------------------------------------
extern bool visitStagingIndexFiles(const string_q& path, void* data);
extern bool visitUnripeIndexFiles(const string_q& path, void* data);

//--------------------------------------------------------------------------------
inline string_q plural(const string_q& in) {
    return substitute(toLower(in).substr(1, 1000) + "s", "logentrys", "logs");
}

//--------------------------------------------------------------------------------
inline bool isJson(void) {
    return (expContext().exportFmt == JSON1 || expContext().exportFmt == API1 || expContext().exportFmt == NONE1);
}

#if 0
#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "pinlib.h"
#include "acctlib.h"
#include "acctscrapestats.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    CAcctScrapeStats stats;
    CMonitorArray allMonitors;
    CMonitorArray possibles;
    blkrange_t fileRange;
    size_t visitTypes;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool visitBinaryFile(const string_q& path, void* data);
    bool handle_rm(const CAddressArray& addrs);
};

#define VIS_FINAL (1 << 1)
#define VIS_STAGING (1 << 2)
#define VIS_UNRIPE (1 << 3)

extern bool visitFinalIndexFiles(const string_q& path, void* data);
extern bool visitStagingIndexFiles(const string_q& path, void* data);
extern bool visitUnripeIndexFiles(const string_q& path, void* data);
#endif
