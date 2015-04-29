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
    void createEdit();
};

#endif // CREATEVIEW_H
