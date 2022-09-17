#include <sstream>
#include <fstream>
#include <algorithm>
#include "cEntity.h"

namespace raven
{
    namespace edb
    {
        int cEntityDB::lastPID = -1;

        void cEntityDB::load(const std::string &fname)
        {
            myValue.clear();
            myfname = fname;
            std::ifstream ifs(myfname);
            if (!ifs.is_open())
                return;
            std::string line;
            cValue v;
            while (std::getline(ifs, line))
            {
                // std::cout << line << "\n";
                std::istringstream ss(line);
                int i;
                ss >> v.aid;
                ss >> v.pid;
                if (v.pid > lastPID)
                    lastPID = v.pid;
                ss >> v.value;
                v.text_from_file();
                myValue.push_back(v);
            }
        }
        void cValue::text_from_file()
        {
            int p = value.find("_");
            while (p != -1)
            {
                value[p] = ' ';
                p = value.find("_");
            }
        }
        std::string cValue::text_for_file()
        {
            std::string sval = value;
            int p = sval.find(" ");
            while (p != -1)
            {
                sval[p] = '_';
                p = sval.find(" ");
            }

            std::stringstream ss;
            ss << (int)aid << " " << pid << " " << sval << "\n";
            return ss.str();
        }
        void cEntityDB::save()
        {
            std::ofstream f(myfname, std::ofstream::trunc);
            if (!f.is_open())
                throw std::runtime_error(
                    "Cannot open database");
            for (auto &v : myValue)
            {
                f << v.text_for_file();
            }
        }

        void cEntityDB::add(
            const std::vector<std::pair<int, std::string>> &vv)
        {
            cValue v;
            v.pid = ++lastPID;
            for (auto &p : vv)
            {
                v.aid = p.first;
                v.value = p.second;
                myValue.push_back(v);
            }
            save();
        }
        void cEntityDB::add(const cEntity &e)
        {
            lastPID++;
            for (auto &av : e.get())
            {
                av.pid = lastPID;
                myValue.push_back(av);
            }
            save();
        }
        void cEntityDB::update(
            const cEntity &e,
            int pid)
        {
            for (auto &av : e.get())
            {
                for (auto &old : myValue)
                {
                    if (old.pid != pid)
                        continue;
                    if (old.aid == av.aid)
                    {
                        old.value = av.value;
                        break;
                    }
                }
            }
            save();
        }

        void cEntityDB::update(const cValue &v)
        {
            for (auto &old : myValue)
            {
                if (old.pid == v.pid)
                {
                    if (old.aid == v.aid)
                        old.value = v.value;
                }
            }
        }
        entityDesc_t cEntityDB::get(
            int pid,
            const std::vector<int> &vatt)
        {
            entityDesc_t ret;
            ret.first = pid;
            for (auto att : vatt)
            {
                for (auto &v : myValue)
                {
                    if (v.pid == pid)
                        if (v.aid == att)
                            ret.second.push_back(v.value);
                }
            }
            return ret;
        }
        entityList_t cEntityDB::entitylist(
            const cEntity &e)
        {
            entityList_t ret;
            for (auto &v : myValue)
            {
                if (v.aid == 1 && v.value == e.type())
                {
                    ret.push_back(
                        get(
                            v.pid,
                            e.AttibuteIndices()));
                }
            }
            return ret;
        }

        cEntity::cEntity(
            std::string type,
            const std::vector<int> &vAtts)
            : myType(type),
              myAtts(vAtts)
        {
            myVals.resize( myAtts.size() );
        }
        void cEntity::set(
            int att,
            const std::string& val)
        {
            myVals[ std::find(myAtts.begin(),myAtts.end(),att)
            - myAtts.begin()] = val;
        }
        void cEntity::set(
            const std::vector<std::string> &vals)
        {
            myVals = vals;
        }
        std::vector<cValue> cEntity::get() const
        {
            std::vector<cValue> ret;
            cValue v;
            for (int k = 0; k < myAtts.size(); k++)
            {
                v.aid = myAtts[k];
                v.value = myVals[k];
                ret.push_back(v);
            }

            // type attribute
            v.aid = 1;
            v.value = myType;
            ret.push_back(v);

            return ret;
        }

    }
}
