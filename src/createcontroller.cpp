#include "createcontroller.h"
#include "util.hpp"
#include <fstream>

#define SUFFIX_CONTROL "Controlador"

CreateController::CreateController(Entity &entity)
{
    nameEntityL = to_lower_copy(entity.name);
    makedir("control");
    generateControlH(entity);
    generateControlCpp(entity);
    clog << entity.name << " Controll Genereted.\n";
}

void CreateController::generateControlH(Entity &entity)
{
    string fileName = boost::algorithm::to_lower_copy(entity.name+SUFFIX_CONTROL);
    std::ofstream file("control/"+fileName+".h");

    boost::algorithm::to_upper(fileName);
    file << "#ifndef " << fileName << "_H\n";
    file << "#define " << fileName << "_H\n\n";
    file << "#include \"controlador.h\"\n\n"
            "class " << entity.name << SUFFIX_CONTROL << " : public Controlador\n{\n"
            "public:\n"
            "\t" << entity.name<<SUFFIX_CONTROL<<"();\n"
            "\tvoid setup();\n"
            "\tvoid list(Request &request, StreamResponse &response);\n"
            "\tvoid add(Request &request, StreamResponse &response);\n"
            "\tvoid save(Request &request, StreamResponse &response);\n"
            "};\n\n"
            "#endif";

}

void CreateController::generateControlCpp(Entity &entity)
{
    string fileName = boost::algorithm::to_lower_copy(entity.name+SUFFIX_CONTROL);
    std::ofstream file("control/"+fileName+".cpp");

    file << "#include \"" << fileName <<".h\"\n"
            "#include <htmlcxx/html/Page.h>\n"
            "using namespace htmlcxx::HTML;\n\n"
            <<entity.name<<SUFFIX_CONTROL<<"::"<<entity.name<<SUFFIX_CONTROL<<"()\n{\n}\n\n"
            "void " << entity.name<<SUFFIX_CONTROL << "::setup()\n{\n"
              "\taddRoute(\"GET\", \"/"<<nameEntityL<<"/list\", "<<entity.name<<SUFFIX_CONTROL<<", list);\n"
              "\taddRoute(\"GET\", \"/"<<nameEntityL<<"/add\", "<<entity.name<<SUFFIX_CONTROL<<", add);\n"
              "\taddRoute(\"POST\", \"/"<<nameEntityL<<"/save\", "<<entity.name<<SUFFIX_CONTROL<<", save);\n"
            "}\n";
    insertListImplemantation(file, entity);
    insertAddImplemantation(file, entity);
    insertSaveImplemantation(file,entity);

}

void CreateController::insertListImplemantation(ofstream &file, Entity &entity)
{
    file << "void " << entity.name<<SUFFIX_CONTROL << "::list(Request &request, StreamResponse &response)\n{\n"
            "\tPage page = createPage(\"/"<<nameEntityL<<"/list.html\");\n"
            "\tresponse << page;\n"
            "}\n";
}

void CreateController::insertAddImplemantation(ofstream &file, Entity &entity)
{
    file << "void " << entity.name<<SUFFIX_CONTROL << "::add(Request &request, StreamResponse &response)\n{\n"
            "\tPage page = createPage(\"/"<<nameEntityL<<"/add.html\");\n"
            "\tresponse << page;\n"
            "}\n";
}

void CreateController::insertSaveImplemantation(ofstream &file, Entity &entity)
{
    file << "void " << entity.name<<SUFFIX_CONTROL << "::save(Request &request, StreamResponse &response)\n{\n"
            "\tredirecionar(response, \"/"<<nameEntityL<<"/list\");\n"
            "}\n";
}

