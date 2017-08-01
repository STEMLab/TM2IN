#ifndef MANAGERIMPL_H
#define MANAGERIMPL_H

#include "Manager.h"


class ManagerImpl : public Manager{
public:
    ManagerImpl();
    virtual ~ManagerImpl();

    int import(const char*);
    int makeSurfaces();
    int cleaning();

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
