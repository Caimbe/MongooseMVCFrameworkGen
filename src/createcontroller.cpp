#include "createcontroller.h"
#include "util.hpp"
#include <fstream>

#define SUFFIX_CONTROL "Controller"


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
            "#include \"controllerbase.h\"\n"
            "#include \"model/"<<boost::algorithm::to_lower_copy(entity.name)<<"model.h\"\n\n"
            "class " << entity.name << SUFFIX_CONTROL << " : public ControllerBase\n{\n"
            "\t"<< entity.name<< "Model model;\n"
            "public:\n"
            "\t" << entity.name<<SUFFIX_CONTROL<<"();\n"
            "\tvoid setup();\n"
            "\tvoid list(Request &request, StreamResponse &response);\n"
            "\tvoid add(Request &request, StreamResponse &response);\n"
            "\tvoid save(Request &request, StreamResponse &response);\n"
            "\tvoid edit(Request &request, StreamResponse &response);\n"
            "\tvoid editSave(Request &request, StreamResponse &response);\n"
            "\tvoid show(Request &request, StreamResponse &response);\n"
            "\tvoid remove(Request &request, StreamResponse &response);\n"
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
              "\taddRoute(\"GET\", \"/"<<nameEntityL<<"/show\", "<<entity.name<<SUFFIX_CONTROL<<", show);\n"
              "\taddRoute(\"GET\", \"/"<<nameEntityL<<"/edit\", "<<entity.name<<SUFFIX_CONTROL<<", edit);\n"
              "\taddRoute(\"POST\", \"/"<<nameEntityL<<"/edit/save\", "<<entity.name<<SUFFIX_CONTROL<<", editSave);\n"
              "\taddRoute(\"POST\", \"/"<<nameEntityL<<"/save\", "<<entity.name<<SUFFIX_CONTROL<<", save);\n"
              "\taddRoute(\"POST\", \"/"<<nameEntityL<<"/remove\", "<<entity.name<<SUFFIX_CONTROL<<", remove);\n"
            "}\n";
    insertListImplemantation(file, entity);
    insertAddImplemantation(file, entity);
    insertShowImplemantation(file, entity);
    insertSaveImplemantation(file,entity);
    insertEditImplemantation(file, entity);
    insertEditSaveImplemantation(file, entity);
    insertDeleteImplemantation(file, entity);
    insertRequest2Obj(file, entity);

}

void CreateController::insertListImplemantation(ofstream &file, Entity &entity)
{
    file << "void " << entity.name<<SUFFIX_CONTROL << "::list(Request &request, StreamResponse &response)\n{\n"
            "\tPage page = createPage(\"/"<<nameEntityL<<"/list.html\");\n"
            "\t"<<entity.name<<"List list = model.list();\n"
            "\tstring lines;\n"
            "\tfor(const "<<entity.name<<"Ptr& "<<nameEntityL<<": list)\n\t{\n"
            "\t\tlines += \"  <tr>\"";
    string paramShow;
    for(Variable var: entity.vecVariable)
    {
        if(var.key){
            paramShow+=(paramShow.size()?"&":"");
            paramShow+=var.name+"=\"+"+nameEntityL+"->get"+upcaseFirstChar(var.name)+"()";
        }
    }
    for(Variable var: entity.vecVariable)
    {
        file << "\n\t\t\"<td>"
             << (var.key?"<a href=/"+nameEntityL+"/show?"+paramShow+"+\">\"+":"\"+")
             << (var.type!="string"?"to_string(":"")
             << nameEntityL<<"->get"
             << upcaseFirstChar(var.name)
             << "()"<<(var.type!="string"?")":"")
             << (var.key?"+\"</a>\"":"")
             << "+\"</td>\"";
    }
    file << "\n\t\t\"</tr>\\n\";\n"
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

void CreateController::insertEditSaveImplemantation(ofstream &file, Entity &entity)
{
    file << "void " << entity.name<<SUFFIX_CONTROL << "::editSave(Request &request, StreamResponse &response)\n{\n"
            "\t"<<entity.name<<"List oldObj = model.list(";
    bool virgula = false;
    for(Variable var: entity.vecVariable){
        if(var.key){
            if(virgula)
                file << " AND ";
            file << "\""<< var.name << "='\"+getSession(request, response).get(\""<<var.name<<"\", \"\")";
            virgula=true;
        }
    }

    file << "+'\\'');\n"
            "\tif(oldObj.empty())\n"
            "\t\tthrow runtime_error(\"Not Exist\");\n"
            "\t"<<entity.name<<" newObj = request2Obj(request);\n"
            "\tmodel.update(*oldObj[0], newObj);\n"
            "\tredirecionar(response, \"/"<<nameEntityL<<"/list\");\n"
            "}\n";
}

void CreateController::insertEditImplemantation(ofstream &file, Entity &entity)
{
    file << "void " << entity.name<<SUFFIX_CONTROL << "::edit(Request &request, StreamResponse &response)\n{\n"
            "\tPage page = createPage(\"/"<<nameEntityL<<"/edit.html\");\n"
            "\t"<<entity.name<<"List "<<nameEntityL<<" = model.list(";
    bool virgula = false;
    for(Variable var: entity.vecVariable){
        if(var.key){
            if(virgula)
                file << " AND ";
            file << "\""<< var.name << "='\"+request.get(\""<<var.name<<"\")";
            virgula=true;
        }
    }
    file << "+'\\'');\n"
            "\tif("<<nameEntityL<<".empty())\n"
            "\t\tthrow runtime_error(\"Not Exist\");\n";

    for(Variable var: entity.vecVariable)
        if(var.key)
            file << "\tgetSession(request, response).setValue( \""<<var.name<<"\", "<<(var.type!="string"?"to_string":"")<<"("<<nameEntityL<<"[0]->get"<<upcaseFirstChar(var.name)<<"()) );\n";

    for(Variable var: entity.vecVariable)
        file << "\tpage.getTagsByName(\""<<var.name<<"\").at(0)->setAttribute(\"value\", "<<(var.type!="string"?"to_string(":"")<<nameEntityL<<"[0]->get"<<upcaseFirstChar(var.name)<<"()"<<(var.type!="string"?")":"")<<" ) ;\n";

    file << "\tresponse << page;\n"
            "}\n";
}

void CreateController::insertShowImplemantation(ofstream &file, Entity &entity)
{
    file << "void " << entity.name<<SUFFIX_CONTROL << "::show(Request &request, StreamResponse &response)\n{\n"
            "\tPage page = createPage(\"/"<<nameEntityL<<"/show.html\");\n"
            "\t"<<entity.name<<"List "<<nameEntityL<<" = model.list(";
    bool virgula = false;
    for(Variable var: entity.vecVariable){
        if(var.key){
            if(virgula)
                file << " OR ";
            file << "\""<< var.name << "='\"+request.get(\""<<var.name<<"\")";
            virgula=true;
        }
    }
    file << "+'\\'');\n"
            "\tif("<<nameEntityL<<".empty())\n"
            "\t\tthrow runtime_error(\"Not Exist\");\n";

    for(Variable var: entity.vecVariable)
        file << "\tpage.insertContentId(\""<<var.name<<"\", "<<nameEntityL<<"[0]->get"<<upcaseFirstChar(var.name)<<"());\n";

    for(Variable var: entity.vecVariable)
        if(var.key)
            file << "\tauto nodes = page.getTagsByName(\""<<var.name<<"\");\n"
                    "\tfor(auto& node: nodes)\n"
                    "\t\tnode->setAttribute(\"value\", "<<nameEntityL<<"[0]->get"<<upcaseFirstChar(var.name)<<"());\n";

    file << "\tresponse << page;\n"
            "}\n";
}

void CreateController::insertDeleteImplemantation(ofstream &file, Entity &entity)
{
    file << "void " << entity.name<<SUFFIX_CONTROL << "::remove(Request &request, StreamResponse &response)\n{\n"
            "\t"<<entity.name<<" obj = request2Obj(request);\n"
            "\tmodel.remove(obj);\n"
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

