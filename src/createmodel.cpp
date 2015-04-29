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
            "\tint save("<<entity.name<<"& "<<nameEntityL<<");\n"
            "\tvoid update("<<entity.name<<"& "<<nameEntityL<<");\n"
            "\tvoid update("<<entity.name<<"& oldObj, "<<entity.name<<"& newObj);\n"
            "\tvoid remove("<<entity.name<<"& "<<nameEntityL<<");\n"
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
    createUpdade(file);
    createDelete(file);
}

void CreateModel::createList(ofstream& file)
{
    file << entity.name<<"List "<<entity.name<<"Model::list(const string& where)\n{\n"
            "\treturn repository.select<"<<entity.name<<"List>(where);\n"
            "}\n";
}

void CreateModel::createSave(ofstream& file)
{
    file << "int "<<entity.name<<"Model::save("<<entity.name<<"& obj)\n{\n"
         "\treturn repository.insert(obj);\n"
            "}\n";
}

void CreateModel::createUpdade(ofstream &file)
{
    file << "void "<<entity.name<<"Model::update("<<entity.name<<"& obj)\n{\n"
         "\trepository.update(obj);\n"
         "}\n";

    file << "void "<<entity.name<<"Model::update("<<entity.name<<"& oldObj, "<<entity.name<<"& newObj)\n{\n"
         "\trepository.update(oldObj, newObj);\n"
         "}\n";
}

void CreateModel::createDelete(ofstream &file)
{
    file << "void "<<entity.name<<"Model::remove("<<entity.name<<"& obj)\n{\n"
         "\trepository.remove(obj);\n"
         "}\n";

}
