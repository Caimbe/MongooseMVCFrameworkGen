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
    file << "#ifndef " << fileName << "_H\n"
            "#define " << fileName << "_H\n\n"
            "#include \"controlador.h\"\n"
            "#include \"model/"<<boost::algorithm::to_lower_copy(entity.name)<<"model.h\"\n\n"
            "class " << entity.name << SUFFIX_CONTROL << " : public Controlador\n{\n"
            "\t"<< entity.name<< "Model model;\n"
            "public:\n"
            "\t" << entity.name<<SUFFIX_CONTROL<<"();\n"
            "\tvoid setup();\n"
            "\tvoid list(Request &request, StreamResponse &response);\n"
            "\tvoid add(Request &request, StreamResponse &response);\n"
            "\tvoid save(Request &request, StreamResponse &response);\n"
            "\t"<<entity.name<<" request2Obj(Request& request);\n"
            "};\n\n"
            "#endif";

}

void CreateController::generateControlCpp(Entity &entity)
{
    string fileName = boost::algorithm::to_lower_copy(entity.name+SUFFIX_CONTROL);
    std::ofstream file("control/"+fileName+".cpp");

    file << "#include \"" << fileName <<".h\"\n"
            "#include <htmlcxx/html/Page.h>\n"
            "#include <boost/lexical_cast.hpp>\n"
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
    insertRequest2Obj(file, entity);

}

void CreateController::insertListImplemantation(ofstream &file, Entity &entity)
{
    file << "void " << entity.name<<SUFFIX_CONTROL << "::list(Request &request, StreamResponse &response)\n{\n"
            "\tPage page = createPage(\"/"<<nameEntityL<<"/list.html\");\n"
            "\t"<<entity.name<<"List list = model.list();\n"
            "\tstring lines;\n"
            "\tfor(const "<<entity.name<<"Ptr& "<<nameEntityL<<": list)\n\t{\n"
            "\t\tlines += \"<tr>\"";
    for(Variable var: entity.vecVariable){
        if(var.type != "string")
            file << "\n\t\t\"<td>\"+to_string("<<nameEntityL<<"->get"<<upcaseFirstChar(var.name)<<"())+\"</td>\"";
        else
            file << "\n\t\t\"<td>\"+"<<nameEntityL<<"->get"<<upcaseFirstChar(var.name)<<"()+\"</td>\"";
    }
    file << "\n\t\t\"<tr>\";\n"
            "\t}\n"
            "\tpage.insertContentId(\"tlist\", lines);\n"
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
            "\t"<<entity.name<<" obj = request2Obj(request);\n"
            "\tmodel.save(obj);\n"
            "\tredirecionar(response, \"/"<<nameEntityL<<"/list\");\n"
            "}\n";
}

void CreateController::insertRequest2Obj(ofstream &file, Entity &entity)
{
    file << entity.name <<' '<< entity.name<<SUFFIX_CONTROL << "::request2Obj(Request &request)\n{\n"
            "\t"<<entity.name<<" obj;\n";

    for(Variable var: entity.vecVariable)
    {
        if(var.type == "tm")
            file << "\tobj.set"<<upcaseFirstChar(var.name)<<"( strtotm( request.get(\""<< var.name<<"\")) );\n";
        else if(var.type == "string")
            file << "\tobj.set"<<upcaseFirstChar(var.name)<<"( request.get(\""<< var.name<<"\") );\n";
        else
            file << "\tobj.set"<<upcaseFirstChar(var.name)<<"( boost::lexical_cast<"<<var.type<<">( request.get(\""<< var.name<<"\")) );\n";
    }

    file << "\treturn obj;\n"
            "}\n";
}

