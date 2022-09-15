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

void cDB::addNurse(
    const std::string &name,
    const std::string &licence,
    const std::string &service)
{
    std::vector<std::pair<int, std::string>> v;
    v.push_back(std::make_pair((int)eAttribute::name, name));
    v.push_back(std::make_pair((int)eAttribute::role, "nurse"));
    v.push_back(std::make_pair((int)eAttribute::nurselicence, licence));
    v.push_back(std::make_pair((int)eAttribute::service, service));

    add(v);
}
void cDB::addPatient(
    const std::vector<std::string> &vals)
{
    std::vector<std::pair<int, std::string>> v;
    v.push_back(std::make_pair((int)eAttribute::name, vals[0]));
    v.push_back(std::make_pair((int)eAttribute::role, "patient"));
    v.push_back(std::make_pair((int)eAttribute::expire, vals[1]));
    v.push_back(std::make_pair((int)eAttribute::certification, vals[2]));
    v.push_back(std::make_pair((int)eAttribute::authorization, vals[3]));
    v.push_back(std::make_pair((int)eAttribute::supplies, vals[4]));

    add(v);
}

void cDB::updateNurse(
    int pid,
    const std::string &name,
    const std::string &licence,
    const std::string &service)
{
    raven::edb::cValue v;
    v.pid = pid;

    v.aid = (int)eAttribute::name;
    v.value = name;
    update( v );
    v.aid = (int)eAttribute::nurselicence;
    v.value = licence;
    update( v );
    v.aid = (int)eAttribute::service;
    v.value = service;
    update( v );

    save();
}

void cDB::updatePatient(
    int pid,
    const std::vector<std::string> &vals)
{
    raven::edb::cValue v;
    v.pid = pid;

    v.aid = (int)eAttribute::name;
    v.value = vals[0];
    update( v );
    v.aid = (int)eAttribute::expire;
    v.value = vals[1];
    update( v );
    v.aid = (int)eAttribute::certification;
    v.value = vals[2];
    update( v );
    v.aid = (int)eAttribute::authorization;
    v.value = vals[3];
    update( v );
    v.aid = (int)eAttribute::supplies;
    v.value = vals[4];
    update( v );

    save();
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
                nursePID(v.pid));
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
                patientPID(v.pid));
        }
    }

    sort(myPatientList, att);

    return myPatientList;
}

cDB::person_t cDB::nurselist(int listIndex)
{
    return nursePID(
        myNurseList[listIndex].first);
}
cDB::person_t cDB::nursePID(int pid)
{
    person_t ret;
    ret.first = pid;
    ret.second.resize(4);
    for (auto &v : myValue)
    {
        if (v.aid == (int)eAttribute::name && v.pid == pid)
            ret.second[0] = v.value;
        if (v.aid == (int)eAttribute::nurselicence && v.pid == pid)
            ret.second[1] = v.value;
        if (v.aid == (int)eAttribute::service && v.pid == pid)
            ret.second[2] = v.value;
    }
    return ret;
}
cDB::person_t cDB::patientlist(int listIndex)
{
    return patientPID(
        myPatientList[listIndex].first);
}
cDB::person_t cDB::patientPID(int pid)
{
    person_t ret;
    ret.first = pid;
    ret.second.resize(5);
    for (auto &v : myValue)
    {
        if (v.aid == (int)eAttribute::name && v.pid == pid)
            ret.second[0] = v.value;
        if (v.aid == (int)eAttribute::expire && v.pid == pid)
            ret.second[1] = v.value;
        if (v.aid == (int)eAttribute::certification && v.pid == pid)
            ret.second[2] = v.value;
        if (v.aid == (int)eAttribute::authorization && v.pid == pid)
            ret.second[3] = v.value;
        if (v.aid == (int)eAttribute::supplies && v.pid == pid)
            ret.second[4] = v.value;
    }
    return ret;
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

