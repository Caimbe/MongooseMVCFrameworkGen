#ifndef CREATEVIEW_H
#define CREATEVIEW_H

#include "entity.h"

class CreateView
{
    string viewDir;
    string nameEntityL;
    Entity entity;
public:
    CreateView(string& viewDir, Entity& entity);
    void createList();
    void createAdd();
    void createShow();
};

#endif // CREATEVIEW_H
