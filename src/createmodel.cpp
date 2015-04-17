#include "createmodel.h"
#include "util.hpp"
#include <fstream>

CreateModel::CreateModel(Entity &entity)
{
    this->entity = entity;
    nameEntityL = to_lower_copy(entity.name);
    makedir("model");

    createH();
    createCpp();

    clog << entity.name << " Model Genereted.\n";
}

void CreateModel::createH()
{
    ofstream file("model/"+nameEntityL+"model.h");
    string nameEntityU = to_upper_copy(nameEntityL);

    file << "#ifndef "<<nameEntityU<<"MODEL_H\n"
            "#define "<<nameEntityU<<"MODEL_H\n\n"
            "#include \"model.h\"\n"
            "class "<<entity.name<<"Model : public Model\n{\n"
            "public:\n"
            "\tvoid save(const "<<entity.name<<"& "<<nameEntityL<<");\n"
            "\t"<<entity.name<<"List list(const string& where=\"\");\n"
            "};\n"
            "#endif";
}

void CreateModel::createCpp()
{
    ofstream file("model/"+nameEntityL+"model.cpp");

    file << "#include \""<<nameEntityL<<"model.h\"\n\n";
    createList(file);
    createSave(file);
}

void CreateModel::createList(ofstream& file)
{
    file << entity.name<<"List "<<entity.name<<"::list(const string& where)\n{\n"
            "\treturn repository.select<"<<entity.name<<"List>(where);\n"
            "}\n";
}

void CreateModel::createSave(ofstream& file)
{file << "int "<<entity.name<<"::save(const "<<entity.name<<"& obj)\n{\n"
         "\trepository.insert(obj);\n"
         "}\n";
}
