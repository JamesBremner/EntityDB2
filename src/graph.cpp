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
    src,
    dst,
};

class cNode : public raven::edb::cEntity
{
public:
    cNode()
        : cEntity("node",
                  {(int)eAttribute::name})
    {
    }
};
class cLink : public raven::edb::cEntity
{
public:
    cLink()
        : cEntity("node",
                  {(int)eAttribute::name,
                   (int)eAttribute::src,
                   (int)eAttribute::dst})

    {
    }
};

class cGUI : public cStarterGUI
{
public:
    cGUI();

private:
    wex::label &lb;
};

cGUI::cGUI()
    : cStarterGUI(
          "Graph",
          {50, 50, 1000, 500}),
      lb(wex::maker::make<wex::label>(fm))
{

    wex::menubar mbar(fm);
    wex::menu mfile(fm);
    mfile.append(
        "Read",
        [&](const std::string &title)
        {
            wex::filebox fb(fm);
            auto fname = fb.open();
            if (fname.empty())
                return;
        });
            mbar.append("File", mfile);

    show();
    run();
}

main()
{
    cGUI theGUI;
    return 0;
}
