#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "database.h"
#include "biglib.h"

namespace qblocks {

    extern string_q cTeal, cOff;
    #define cleanFmt(str) ((str).Substitute("\\n\\\n", "\\n").Substitute("\n", "").Substitute("\\n", "\n").Substitute("\\t", "\t").Substitute("\\r", "\r"))
    //-------------------------------------------------------------------------
    class CToml : public CSharedResource {
    private:
        void Clear(void);
        // no default construction or copies
        CToml(void) {}
        CToml(const CToml& toml);
        CToml& operator= (const CToml& toml);

        //-------------------------------------------------------------------------
        // support class for toml file
        class CTomlKey {
        public:
            string_q keyName;
            string_q value;
            bool     comment;

            CTomlKey(void);
            CTomlKey(const CTomlKey& key);
            CTomlKey& operator=(const CTomlKey& key);
        };
        typedef SFList<CTomlKey*> CTomlKeyList;

        //-------------------------------------------------------------------------
        // support class for toml file
        class CTomlGroup {
        public:
            string_q  groupName;
            bool      isArray;
            bool      isComment;
            CTomlKeyList keys;

            CTomlGroup(void);
            CTomlGroup(const CTomlGroup& group);

            ~CTomlGroup(void);

            CTomlGroup& operator=(const CTomlGroup& group);

            CTomlKey *addKey(const string_q& keyName, const string_q& val, bool commented);
            CTomlKey *findKey(const string_q& keyName) const;

        private:
            void Clear(void);
            void Copy(const CTomlGroup& group);

        };
        typedef SFList<CTomlGroup*> CTomlGroupList;

    protected:
        CTomlGroup *addGroup(const string_q& group, bool commented, bool array);
        CTomlGroup *findGroup(const string_q& group) const;

        CTomlKey *addKey(const string_q& group, const string_q& key, const string_q& val, bool commented);
        CTomlKey *findKey(const string_q& group, const string_q& key) const;

    public:
        CTomlGroupList groups;

        CToml(const string_q& fileName);
        ~CToml(void);

        string_q getType(void) const override { return "CToml"; }

        string_q getConfigStr   (const string_q& group, const string_q& key, const string_q& def) const;
        uint64_t getConfigInt   (const string_q& group, const string_q& key, uint64_t def) const;
        SFUintBN getConfigBigInt(const string_q& group, const string_q& key, SFUintBN def) const;
        bool     getConfigBool  (const string_q& group, const string_q& key, bool def) const;
        string_q getDisplayStr  (bool terse, const string_q& def, const string_q& color=cTeal) const;

        void setConfigStr  (const string_q& group, const string_q& key, const string_q& value);
        void setConfigInt  (const string_q& group, const string_q& key, uint64_t value);
        void setConfigBool (const string_q& group, const string_q& key, bool value);

        bool writeFile(void);
        bool readFile(const string_q& filename);
        void mergeFile(CToml *tomlIn);

        friend ostream& operator<<(ostream& os, const CToml& tomlIn);
    };

    //-------------------------------------------------------------------------
    extern ostream& operator<<(ostream& os, const CToml& t);

}  // namespace qblocks
