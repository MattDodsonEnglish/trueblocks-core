/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    ENTER("visitFile");

    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);

    } else {
        if (endsWith(path, "acct.bin")) {
            if (isTestMode()) {
                static CStringArray testAddrs = {
                    "0x001d14804b399c6ef80e64576f657660804fec0b", "0x1111111111111111111111111111111111111111",
                    "0x1111122222111112222211111222221111122222", "0x1234567812345678123456781234567812345678",
                    "0x1234567890123456789012345678901234567890", "0x5555533333555553333355555333335555533333",
                    "0x9876543210987654321098765432109876543210", "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359",
                    "0xfb744b951d094b310262c8f986c860df9ab1de65"};
                bool found = false;
                for (auto t : testAddrs)
                    if (contains(path, t))
                        found = true;
                if (!found)
                    return true;
            }

            size_t sizeThen = fileSize(path);
            size_t nRecords = (fileSize(path) / sizeof(CAppearance_base));
            if (!nRecords)
                EXIT_NOMSG(!shouldQuit());

            CAppearance_base* buffer = new CAppearance_base[nRecords];
            if (!buffer)
                EXIT_NOMSG(!shouldQuit()); // continue anyway

            bzero((void*)buffer, nRecords * sizeof(CAppearance_base));
            CArchive archiveIn(READING_ARCHIVE);
            if (!archiveIn.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                archiveIn.Release();
                delete[] buffer;
                EXIT_FAIL("Could not open cache file.");
            }
            archiveIn.Read(buffer, sizeof(CAppearance_base), nRecords);
            archiveIn.Release();

            CAppearanceArray_base apps;
            apps.reserve(nRecords);
            for (size_t i = 0; i < nRecords ; i++) {
                apps.push_back(buffer[i]);
            }
            sort(apps.begin(), apps.end());
            delete[] buffer;

            CAppearance_base prev;
            CAppearanceArray_base deduped;
            for (auto a : apps) {
                if (a.blk != prev.blk || a.txid != prev.txid) {
                    deduped.push_back(a);
                }
                prev = a;
            }

            CArchive archiveOut(WRITING_ARCHIVE);
            archiveOut.Lock(path, modeWriteCreate, LOCK_WAIT);
            for (auto item : deduped)
                archiveOut << item.blk << item.txid;
            archiveOut.Release();

            static bool first = true;
            if (!first)
                cout << ",";
            first = false;
            CMonitor m;
            cout << "{ \"path\": \"" << substitute(path, m.getMonitorPath(""), "$CACHE/") << "\", \"sizeThen\": " << sizeThen << ", \"sizeNow\": " << fileSize(path) << "}" << endl;
        }
    }

    return !shouldQuit();
}

//---------------------------------------------------------------
bool COptions::handle_clean(void) {
    CMonitor m;
    cout << (isTestMode() ? "[" : "");
    bool ret = forEveryFileInFolder(m.getMonitorPath(""), visitFile, NULL);
    cout << (isTestMode() ? "]" : "");
    return ret;
}
