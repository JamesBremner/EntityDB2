#include <sstream>
#include <fstream>
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
        const std::vector<std::pair<int,std::string>>& vv )
        {
            cValue v;
            v.pid = ++lastPID;
            for( auto& p : vv )
            {
                v.aid = p.first;
                v.value = p.second;
                myValue.push_back(v);
            }
            save();
        }
}
}