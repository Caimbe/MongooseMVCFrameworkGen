#ifndef PROCESSFILES_H
#define PROCESSFILES_H

#include <fstream>
#include "entity.h"
#include "iostream"

struct Options
{
    vector<string> vecFileName;
    string viewDir;
    bool model = false;
    bool view = false;
    bool controller = false;
};

class ProcessFiles
{
    vector<Entity> vecEntity;

    Entity createEntity(const ifstream& file, const vector<Entity>& vecEntityName=vector<Entity>());
    void createControllerBase();
    void createModelBase();
public:
    ProcessFiles(Options& options);
};

#endif // PROCESSFILES_H
