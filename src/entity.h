#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

using namespace std;

struct Variable{
    string type;
    string name;
};

struct Entity
{
public:
    string name;
    vector<Variable> vecVariable;
    Entity();
};

#endif // ENTITY_H
