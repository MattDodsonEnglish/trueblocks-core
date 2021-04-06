#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved.
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "pinlib.h"
#include "acctlib.h"

//--------------------------------------------------------------------------
class CConsolidator : public CBlockProgress {
  public:
    bool pin;
    blknum_t distFromHead;
    blknum_t prevBlock;
    blknum_t blazeStart;
    blknum_t blazeRipe;
    blknum_t blazeCnt;
    string_q oldStage;
    string_q newStage;
    string_q tmpFile;
    string_q tmp_fn;
    ofstream tmp_stream;
    CPinnedChunkArray pinList;

    explicit CConsolidator(const CBlockProgress& prog);
    bool stage_chunks(void);
    bool consolidate_chunks(void);
    bool write_chunks(blknum_t chunkSize, bool once);
    string_q Format(void);

  private:
    CConsolidator(void) {
    }
};

//--------------------------------------------------------------------------
// These defines give us control over the scrape when we're testing. A bit cludgy, but effective.
#define CLIENT (isLiveTest() ? (80 + (runs)) : (cons.client))
#define N_BLOCKS (isLiveTest() ? 16 : cons.blazeCnt)
#define MAX_ROWS (isLiveTest() ? 13 : 2000000)
#define SNAP_TO_GRID_BLKS (isLiveTest() ? 12 : 100000)
// Dont' start snapping to grid until the 2016 DDos and then do so for the rest of the chain
#define FIRST_SNAP_TO_GRID 2250000
#define TEST_RUNS 3

//--------------------------------------------------------------------------
#define LOG_INDEX8(fn, extra)                                                                                          \
    LOG_FN8(fn);                                                                                                       \
    if (isLiveTest()) {                                                                                                \
        LOG8(string_q("Contents of ") + (#fn) + (extra) + ":");                                                        \
        LOG8(substitute(asciiFileToString(fn), "\n", "\n: 8------"));                                                  \
    }

//--------------------------------------------------------------------------
#define LOG_INDEX3(fn, extra)                                                                                          \
    LOG_FN3(fn);                                                                                                       \
    if (isLiveTest()) {                                                                                                \
        LOG3(string_q("Contents of ") + (#fn) + (extra) + ":");                                                        \
        LOG3(substitute(asciiFileToString(fn), "\n", "\n: 3------"));                                                  \
    }
