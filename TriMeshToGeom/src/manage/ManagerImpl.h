#ifndef MANAGERIMPL_H
#define MANAGERIMPL_H

#include "manage/Manager.h"


class ManagerImpl : public Manager{
public:
    ManagerImpl();
    virtual ~ManagerImpl();

    int import(const char*);
    int makeSurfaces();
    int cleaning(int);
    //import and export
    int exportJSON(string f_path);
    int exportCombined(string f_path);
    int importCombined(string f_path);

    //TODO
    void makeSolids() {};


protected:


private:
};

#endif // MANAGERIMPL_H
