#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <map>
#include <algorithm>

#include "cDB.h"

cDB::cDB()
{
    raven::edb::cEntityDB::load("hhct.txt");
}

cDB::vperson_t cDB::nursebyDate(int att)
{
    myNurseList.clear();
    for (auto &v : myValue)
    {
        if (v.aid == (int)eAttribute::role && v.value == "nurse")
        {
            // add nurse to list
            myNurseList.push_back(
                get(
                    v.pid,
                    {(int)eAttribute::name,
                     (int)eAttribute::nurselicence,
                     (int)eAttribute::service}));
        }
    }
    // sort into date order
    sort(myNurseList, att);

    return myNurseList;
}

cDB::vperson_t cDB::patientbyDate(int att)
{
    myPatientList.clear();
    for (auto &v : myValue)
    {
        if (v.aid == (int)eAttribute::role && v.value == "patient")
        {
            myPatientList.push_back(
                get(
                    v.pid,
                    {(int)eAttribute::name,
                     (int)eAttribute::expire,
                     (int)eAttribute::certification,
                     (int)eAttribute::authorization,
                     (int)eAttribute::supplies}));
        }
    }

    sort(myPatientList, att);

    return myPatientList;
}

cDB::person_t cDB::nurselist(int listIndex)
{
    return get(
        myNurseList[listIndex].first,
        {(int)eAttribute::name,
         (int)eAttribute::nurselicence,
         (int)eAttribute::service});
}

cDB::person_t cDB::patientlist(int listIndex)
{
    return get(
        myPatientList[listIndex].first,
        {(int)eAttribute::name,
         (int)eAttribute::expire,
         (int)eAttribute::certification,
         (int)eAttribute::authorization,
         (int)eAttribute::supplies});
}

long long cDB::secs(
    const cDB::person_t &p,
    int index)
{
    using namespace std::chrono;
    std::tm tm = {};
    std::istringstream ina{p.second[index]};
    ina >> std::get_time(&tm, "%Y-%m-%d");
    return system_clock::from_time_t(std::mktime(&tm)).time_since_epoch().count();
}

void cDB::sort(
    vperson_t &list,
    int att)
{
    using namespace std::chrono;

    std::sort(
        list.begin(), list.end(),
        [&](person_t &a, person_t &b) -> bool
        {
            return (secs(a, att) < secs(b, att));
        });
}
