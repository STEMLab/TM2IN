#ifndef MANAGERIMPL_H
#define MANAGERIMPL_H

#include "manage/Manager.h"


class ManagerImpl : public Manager{
public:
    ManagerImpl();
    virtual ~ManagerImpl();

    int import(const char*);
    int makeSurfaces(double degree);
    int cleaning(int, double startDegree);
    //import and export
    int exportJSON(string f_path);
    int exportCombined(string f_path);
    int importCombined(string f_path);
    int makeSolids(double degree);

protected:


private:
};

#endif // MANAGERIMPL_H
