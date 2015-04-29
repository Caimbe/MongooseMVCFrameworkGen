#ifndef CREATEMODEL_H
#define CREATEMODEL_H

#include "entity.h"

class CreateModel
{
    string nameEntityL;
    Entity entity;
public:
    CreateModel(Entity& entity);
    void createH();
    void createCpp();
    void createList(ofstream& file);
    void createSave(ofstream& file);
    void createUpdade(ofstream& file);
    void createDelete(ofstream& file);
};

#endif // CREATEMODEL_H
