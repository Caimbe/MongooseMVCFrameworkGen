#ifndef CREATECONTROLLER_H
#define CREATECONTROLLER_H

#include "entity.h"
#include <fstream>

class CreateController
{
    string nameEntityL;
public:
    CreateController(Entity &entity);

    void generateControlH(Entity& entity);
    void generateControlCpp(Entity& entity);
    void insertListImplemantation(ofstream& file, Entity& entity);
    void insertAddImplemantation(ofstream& file, Entity& entity);
    void insertSaveImplemantation(ofstream& file, Entity& entity);
    void insertEditSaveImplemantation(ofstream& file, Entity& entity);
    void insertEditImplemantation(ofstream& file, Entity& entity);
    void insertShowImplemantation(ofstream& file, Entity& entity);
    void insertDeleteImplemantation(ofstream& file, Entity& entity);
    void insertRequest2Obj(ofstream& file, Entity& entity);
};

#endif // CREATECONTROLLER_H
