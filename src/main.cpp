#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <wex.h>
#include <inputbox.h>
#include "cStarterGUI.h"

#include "cEntity.h"

/// @brief The different attributes in database
enum class eAttribute
{
    none,
    role,
    name,
    nurselicence,
    service,
    expire,
    certification,
    authorization,
    supplies,
};

class cNurse : public raven::edb::cEntity
{
public:
    cNurse()
        : cEntity("nurse",
                  {(int)eAttribute::name,
                   (int)eAttribute::nurselicence,
                   (int)eAttribute::service})
    {
    }
};

class cPatient : public raven::edb::cEntity
{
public:
    cPatient()
        : cEntity("patient",
                  {(int)eAttribute::name,
                   (int)eAttribute::expire,
                   (int)eAttribute::certification,
                   (int)eAttribute::authorization,
                   (int)eAttribute::supplies})
    {
    }
};

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "HHC Tracker",
              {50, 50, 1000, 500}),
          tabs(wex::maker::make<wex::tabbed>(fm)),
          plNurse(wex::maker::make<wex::panel>(tabs)),
          plPatient(wex::maker::make<wex::panel>(tabs)),
          lbNursePanel(wex::maker::make<wex::label>(plNurse)),
          lbNurseOrder(wex::maker::make<wex::label>(plNurse)),
          chNurseOrder(wex::maker::make<wex::choice>(plNurse)),
          lsNurse(wex::maker::make<wex::list>(plNurse)),
          bnNurseAdd(wex::maker::make<wex::button>(plNurse)),

          bnPatientAdd(wex::maker::make<wex::button>(plPatient)),
          lsPatient(wex::maker::make<wex::list>(plPatient)),
          lbPatientOrder(wex::maker::make<wex::label>(plPatient)),
          chPatientOrder(wex::maker::make<wex::choice>(plPatient))
    {
        theDB.load("hhct.txt");

        tabs.move(0, 0, 1000, 500);
        tabs.tabWidth(200);
        tabs.add("Nurses", plNurse);
        tabs.add("Patients", plPatient);

        ConstructNursesPanel();
        ConstructPatientsPanel();

        show();
        tabs.select(0);
        run();
    }

private:
    wex::tabbed &tabs;
    wex::panel &plNurse;
    wex::panel &plPatient;
    wex::label &lbNursePanel;
    wex::label &lbNurseOrder;
    wex::choice &chNurseOrder;
    wex::list &lsNurse;
    wex::list &lsPatient;
    wex::button &bnNurseAdd;

    wex::button &bnPatientAdd;
    wex::label &lbPatientOrder;
    wex::choice &chPatientOrder;

    raven::edb::cEntityDB theDB;
    raven::edb::entityList_t myNurseList;
    raven::edb::entityList_t myPatientList;

    void ConstructNursesPanel();
    void ConstructPatientsPanel();

    void addNurse();
    void listNurse();
    void editNurse();

    void addPatient();
    void listPatient();
    void editPatient();
};

long long secs(
    const raven::edb::entityDesc_t &p,
    int index)
{
    using namespace std::chrono;
    std::tm tm = {};
    std::istringstream ina{p.second[index]};
    ina >> std::get_time(&tm, "%Y-%m-%d");
    return system_clock::from_time_t(std::mktime(&tm)).time_since_epoch().count();
}

void sort(
    raven::edb::entityList_t &list,
    int att)
{
    using namespace std::chrono;

    std::sort(
        list.begin(), list.end(),
        [&](raven::edb::entityDesc_t &a, raven::edb::entityDesc_t &b) -> bool
        {
            return (secs(a, att) < secs(b, att));
        });
}

void cGUI::ConstructNursesPanel()
{
    lbNursePanel.move(20, 40, 100, 30);
    lbNursePanel.text("Nurses");

    bnNurseAdd.move(200, 40, 100, 30);
    bnNurseAdd.text("ADD");
    bnNurseAdd.events().click(
        [&]
        {
            addNurse();
            listNurse();
        });

    lbNurseOrder.move(20, 70, 40, 30);
    lbNurseOrder.text("Order");
    chNurseOrder.move(80, 70, 100, 100);
    chNurseOrder.add("Licence");
    chNurseOrder.add("InService");
    chNurseOrder.select(0);
    chNurseOrder.events().select(
        chNurseOrder.id(),
        [&]
        {
            listNurse();
        });

    lsNurse.move(10, 120, 900, 400);
    lsNurse.events().select(
        lsNurse.id(),
        [&]
        {
            editNurse();
            listNurse();
        });
    listNurse();
}

void cGUI::ConstructPatientsPanel()
{
    bnPatientAdd.move(200, 40, 100, 30);
    bnPatientAdd.text("ADD");
    bnPatientAdd.events().click(
        [&]
        {
            addPatient();
            listPatient();
        });

    lbPatientOrder.move(20, 70, 40, 30);
    lbPatientOrder.text("Order");
    chPatientOrder.move(80, 70, 100, 100);
    chPatientOrder.add("Expire");
    chPatientOrder.add("Certicication");
    chPatientOrder.add("Authorization");
    chPatientOrder.add("Supplies");
    chPatientOrder.select(0);
    chPatientOrder.events().select(
        chPatientOrder.id(),
        [&]
        {
            listPatient();
        });

    lsPatient.move(10, 120, 900, 400);
    lsPatient.events().select(
        lsPatient.id(),
        [&]
        {
            editPatient();
            listPatient();
        });

    listPatient();
}

void cGUI::addNurse()
{
    wex::inputbox ib;
    ib.text("Nurse");
    ib.add("Name", "");
    ib.add("Licence", "");
    ib.add("Service", "");

    ib.showModal();

    std::vector<std::string> vals;
    vals.push_back(ib.value("Name"));
    vals.push_back(ib.value("Licence"));
    vals.push_back(ib.value("Service"));
    cNurse N;
    N.set(vals);
    theDB.add(N);
}
void cGUI::editNurse()
{
    cNurse N;
    int pid = myNurseList[lsNurse.selectedIndex()].first;
    auto nurse = theDB.get(pid, N.AttibuteIndices());

    wex::inputbox ib;
    ib.text("Nurse");
    ib.add("Name", nurse.second[0]);
    ib.add("Licence", nurse.second[1]);
    ib.add("Service", nurse.second[2]);

    ib.showModal();

    std::vector<std::string> vals;
    vals.push_back(ib.value("Name"));
    vals.push_back(ib.value("Licence"));
    vals.push_back(ib.value("Service"));
    cNurse P;
    P.set(vals);
    theDB.update(P, nurse.first);
}
void cGUI::addPatient()
{
    wex::inputbox ib;
    ib.labelWidth(100);
    ib.text("Patient");
    ib.add("Name", "");
    ib.add("Expire", "");
    ib.add("Certification", "");
    ib.add("Authorization", "");
    ib.add("Supplies", "");

    ib.showModal();

    std::vector<std::string> vals;
    vals.push_back(ib.value("Name"));
    vals.push_back(ib.value("Expire"));
    vals.push_back(ib.value("Certification"));
    vals.push_back(ib.value("Authorization"));
    vals.push_back(ib.value("Supplies"));
    cPatient P;
    P.set(vals);
    theDB.add(P);
}
void cGUI::editPatient()
{
    cPatient P;
    int pid = myPatientList[lsPatient.selectedIndex()].first;
    auto patient = theDB.get(pid, P.AttibuteIndices());

    wex::inputbox ib;
    ib.text("Patient");
    ib.labelWidth(100);
    ib.add("Name", patient.second[0]);
    ib.add("Expire", patient.second[1]);
    ib.add("Certification", patient.second[2]);
    ib.add("Authorization", patient.second[3]);
    ib.add("Supplies", patient.second[4]);

    ib.showModal();

    std::vector<std::string> vals;
    vals.push_back(ib.value("Name"));
    vals.push_back(ib.value("Expire"));
    vals.push_back(ib.value("Certification"));
    vals.push_back(ib.value("Authorization"));
    vals.push_back(ib.value("Supplies"));
    P.set(vals);
    theDB.update(P, patient.first);
}
void cGUI::listNurse()
{
    // clear the old list
    lsNurse.clear();

    // calculate date attribute index
    int dateIndex;
    switch (chNurseOrder.selectedIndex())
    {
    case 0:
        dateIndex = 1;
        break;
    case 1:
        dateIndex = 2;
        break;
    default:
        return;
    }

    myNurseList = theDB.entitylist(cNurse());
    sort(myNurseList, dateIndex);

    // populate list
    for (auto &n : myNurseList)
    {
        std::stringstream ss;
        for (auto &s : n.second)
        {
            ss << std::setw(16) << s;
        }
        lsNurse.add(ss.str());
    }
}
void cGUI::listPatient()
{
    lsPatient.clear();

    myPatientList = theDB.entitylist(cPatient());
    sort(
        myPatientList,
        chPatientOrder.selectedIndex() + 1);

    // populate list
    for (auto &n : myPatientList)
    {
        std::stringstream ss;
        for (auto &s : n.second)
        {
            ss << std::setw(16) << s;
        }
        lsPatient.add(ss.str());
    }
}



main()
{
    cGUI theGUI;
    return 0;
}
