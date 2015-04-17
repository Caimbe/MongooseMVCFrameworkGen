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
};

#endif // CREATEMODEL_H
