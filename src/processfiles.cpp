#include "processfiles.h"
#include <sstream>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "util.hpp"
#include "createcontroller.h"
#include "createview.h"
#include "createmodel.h"

ProcessFiles::ProcessFiles(Options& options)
{
    if(options.controllerDefaul)
        createControllerBase();
    if(options.modelDefaul)
        createModelBase();

    for(const string& fileName: options.vecFileName)
    {
        ifstream file(fileName);
        Entity entity = createEntity(file);
        vecEntity.push_back(entity);
    }
    for(const string& fileName: options.vecFileName)
    {
        ifstream file(fileName);
        Entity entity = createEntity(file, vecEntity);
        if(options.model)
            CreateModel m(entity);
        if(options.view)
            CreateView v(options.viewDir, entity);
        if(options.controller)
            CreateController c(entity, vecEntity);
    }
}




Entity ProcessFiles::createEntity(const ifstream &file, const vector<Entity>& vecEntityName)
{
    Entity entity;
    stringstream contentFile;
    contentFile << file.rdbuf();
    size_t pos;
    boost::cmatch res;
    string types = "int|short|long|bool|char|float|double|string|tm";
    for(const Entity& entity: vecEntityName)
        types+="|"+entity.name+"Ptr";
    boost::regex rx("("+types+")\\s\\S*[^\\)](;|\\s|=)", boost::regex::extended);

    for (std::string line; std::getline(contentFile, line); )
    {
        if((pos = line.find("class"))!=string::npos){
            entity.name = line.substr(pos+6);
            continue;
        }

        if( boost::regex_search(line.c_str(), res, rx) && line.find('(')==string::npos ){
            string str = res.str();
            Variable var;
            size_t posB = str.find(' ');
            size_t posV = str.rfind(';');
            var.name = str.substr(posB+1, posV-posB-1);
            var.type = str.substr(0, posB);
            if(line.find("//key:")!=string::npos)
                var.key=true;
            entity.vecVariable.push_back(var);
        }
    }
    return entity;
}

void ProcessFiles::createControllerBase()
{
    ofstream file(DIR_CONTROL"controllerbase.h");
    file << "#ifndef CONTROLLERBASE_H\n"
        "#define CONTROLLERBASE_H\n\n"

        "#include <mongoose/WebController.h>\n"
        "#include <mongoose/Server.h>\n"
        "#include <htmlcxx/html/Page.h>\n"
        "#include \"util.hpp\"\n\n"

        "using namespace std;\n"
        "using namespace Mongoose;\n"
        "using namespace htmlcxx::HTML;\n\n"

        "class ControllerBase : public WebController\n"
        "{\n"
        "public:\n"
            "\tControllerBase();\n"
            "\tvoid redirecionar(StreamResponse& response, const string& local);\n"
            "\tPage createPage(const string& fileName);\n"
        "};\n\n"

        "#endif // Controlador_H\n";

    ofstream filecpp(DIR_CONTROL"controllerbase.cpp");
    filecpp << "#include \"controllerbase.h\"\n\n"

               "ControllerBase::ControllerBase()\n"
               "{\n"
               "}\n\n"


               "void ControllerBase::redirecionar(StreamResponse& response, const string& local)\n"
               "{\n"
                   "\tresponse.setCode(301);\n"
                   "\tresponse.setHeader(\"Location\", local);\n"
               "}\n\n"

               "Page ControllerBase::createPage(const string &fileName)\n"
               "{\n"
                   "\tPage page;\n"
                   "\tifstream ftemplate(server->getOption(\"document_root\")+server->getOption(\"template_page\"));\n"
                   "\tifstream fcontent(server->getOption(\"document_root\")+fileName);\n"
                   "\tif(fcontent.is_open())\n"
                   "\t{\n"
                    "\t\tif(ftemplate.is_open()){\n"
                       "\t\t\tpage.setContent(ftemplate);\n"
                       "\t\t\tpage.insertContentId(\"content\", fcontent);\n"
                    "\t\t}else{\n"
                        "\t\t\tpage.setContent(fcontent);\n"
                    "\t\t}\n"
                   "\t}\n"
                   "\telse\n"
                       "\t\tthrow runtime_error(\"content file not found: \"+fileName);\n\n"

                   "\treturn page;\n"
               "}\n";
}

void ProcessFiles::createModelBase()
{
    ofstream file("model/model.h");
    file << "#ifndef MODEL_H\n"
        "#define MODEL_H\n\n"

        "#include \"repository/repository.h\"\n\n"

        "using namespace std;\n"

        "class Model\n"
        "{\n"
        "public:\n"
            "\tstatic Repository repository;\n"
            "\tModel();\n"
            "\tvoid openRepository(const string& url);"
        "};\n\n"

        "#endif // Controlador_H\n";

    ofstream filecpp("model/model.cpp");
    filecpp << "#include \"model.h\"\n\n"

        "Repository Model::repository;\n\n"
        "Model::Model()\n"
        "{\n"
        "}\n\n"

        "void Model::openRepository(const string& url)\n{\n"
            "\trepository.open(url);\n"
        "}\n\n";
}

