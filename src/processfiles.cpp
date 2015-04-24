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
    createControllerBase();
    for(const string& fileName: options.vecFileName)
    {
        ifstream file(fileName);
        Entity entity = createEntity(file);
        CreateModel m(entity);
        CreateView v(options.viewDir, entity);
        CreateController c(entity);
    }
}




Entity ProcessFiles::createEntity(const ifstream &file)
{
    Entity entity;
    stringstream contentFile; contentFile << file.rdbuf();
    size_t pos;
    boost::cmatch res;
    boost::regex rx("(int|short|long|float|double|string|tm)\\s\\S*[^\\)];", boost::regex::extended);

    for (std::string line; std::getline(contentFile, line); )
    {
        if((pos = line.find("class"))!=string::npos)
            entity.name = line.substr(pos+6);

        if( boost::regex_search(line.c_str(), res, rx) ){
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
    ofstream file("control/controllerbase.h");
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

    ofstream filecpp("control/controllerbase.cpp");
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
                   "\tif(ftemplate.is_open())\n"
                       "\t\tpage.setContent(ftemplate);\n"
                   "\telse\n"
                       "\t\tthrow runtime_error(\"template file not found! The option template_page is: \"+server->getOption(\"template_page\"));\n\n"

                   "\tifstream fcontent(server->getOption(\"document_root\")+fileName);\n"
                   "\tif(fcontent.is_open())\n"
                       "\t\tpage.insertContentId(\"content\", fcontent);\n"
                   "\telse\n"
                       "\t\tthrow runtime_error(\"content file not found: \"+fileName);\n\n"

                   "\tcout << ftemplate;\n\n"

                   "\treturn page;\n"
               "}\n";
}

