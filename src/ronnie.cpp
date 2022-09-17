#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"
#include "cEntity.h"

/// @brief The different attributes in database
enum class eAttribute
{
    none,
    role,
    name,
    note,
    owner,
    date,
};

class cMaster : public raven::edb::cEntity
{
public:
    cMaster()
        : cEntity("master",
                  {(int)eAttribute::name,
                   (int)eAttribute::note,
                   (int)eAttribute::date})
    {
    }
};
class cNote : public raven::edb::cEntity
{
public:
    cNote()
        : cEntity("note",
                  {(int)eAttribute::owner,
                   (int)eAttribute::note,
                   (int)eAttribute::date})
    {
    }
};

class cGUI : public cStarterGUI
{
public:
    cGUI();

private:
    wex::label &lbname;
    wex::editbox &ebname;
    wex::button &bnCreate;
    wex::multiline &medNote;
    wex::button &bnRead;
    wex::list &lsMaster;

    raven::edb::cEntityDB theDB;
    raven::edb::entityList_t myMasterList;

    void create();
    void read();
};

cGUI::cGUI()
    : cStarterGUI(
          "CRM",
          {50, 50, 1000, 500}),
      lbname(wex::maker::make<wex::label>(fm)),
      ebname(wex::maker::make<wex::editbox>(fm)),
      bnCreate(wex::maker::make<wex::button>(fm)),
      medNote(wex::maker::make<wex::multiline>(fm)),
      bnRead(wex::maker::make<wex::button>(fm)),
      lsMaster(wex::maker::make<wex::list>(fm))
{
    theDB.load("ronnie.txt");

    lbname.move({10, 10, 100, 30});
    lbname.text("Name");
    ebname.move(150, 10, 200, 30);
    ebname.text("");
    medNote.move(10, 50, 400, 100);
    medNote.text("");
    lsMaster.move(500, 5, 180, 400);

    bnCreate.move(30, 200, 100, 30);
    bnCreate.text("CREATE");
    bnCreate.events().click(
        [&]
        {
            create();
        });
    bnRead.move(150, 200, 100, 30);
    bnRead.text("READ");
    bnRead.events().click(
        [&]
        {
            read();
        });

    lsMaster.events().select(
        lsMaster.id(),
        [&]
        {
            cMaster M;
            auto desc = theDB.get(
                myMasterList[lsMaster.selectedIndex()].first,
                M.AttibuteIndices());
            ebname.text(desc.second[0]);
            medNote.text(desc.second[1]);
            lsMaster.show(false);
            fm.update();
        });

    show();
    lsMaster.show(false);
    
    run();
}

void cGUI::create()
{
    cMaster M;
    M.set((int)eAttribute::name, ebname.text());
    M.set((int)eAttribute::note, medNote.text());
    theDB.add(M);
}

void cGUI::read()
{
    cMaster M;
    myMasterList = theDB.entitylist(M);

    lsMaster.clear();
    for (auto &m : myMasterList)
    {
        lsMaster.add(m.second[0]);
    }
    lsMaster.show();
}

main()
{
    cGUI theGUI;
    return 0;
}
