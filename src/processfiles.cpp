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
            entity.vecVariable.push_back(var);
//            cout << var.name << ':' << var.type << endl;
        }
    }
    return entity;
}

