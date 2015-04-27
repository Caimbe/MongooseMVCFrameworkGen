#include "createview.h"
#include "util.hpp"
#include <fstream>

CreateView::CreateView(string &viewDir, Entity &entity)
{
    this->viewDir = viewDir;
    this->entity = entity;
    nameEntityL = to_lower_copy(entity.name);
    makedir(viewDir.c_str());
    makedir((viewDir+"/"+nameEntityL).c_str());

    createList();
    createAdd();
    createShow();

    clog << entity.name << " View Genereted.\n";
}

void CreateView::createList()
{
    ofstream file(viewDir+"/"+nameEntityL+"/list.html");
    if(!file.is_open())
        throw runtime_error("not possibele create "+viewDir+"/"+nameEntityL+"/list.html");

    file << "<h2>List</h2>\n"
            "<a href=/"<<nameEntityL<<"/add>Add</a>\n"
            "<table id='tlist'>\n"
            "  <tr>\n";
    for(const Variable& var: entity.vecVariable)
        file << "    <th>"<<var.name<<"</th>\n";
    file << "  </tr>\n"
            "</table>\n";
}

void CreateView::createAdd()
{
    ofstream file(viewDir+"/"+nameEntityL+"/add.html");
    if(!file.is_open())
        throw runtime_error("not possibele create "+viewDir+"/"+nameEntityL+"/add.html");

    file << "<h2>Add</h2>\n"
            "<form method='post' action='/"<<nameEntityL<<"/save'>"
            "  <fieldset>"
            "   <legend>New "<<entity.name<<"</legend>";
    for(const Variable& var: entity.vecVariable)
        file << "   <label>"<<var.name<<": <input name='"<<var.name<<"'></label>";
    file << "  </fieldset>"
            "<input type='submit' value='Submit'>"
            "</form>";
}

void CreateView::createShow()
{
    ofstream file(viewDir+"/"+nameEntityL+"/show.html");
    if(!file.is_open())
        throw runtime_error("not possibele create "+viewDir+"/"+nameEntityL+"/show.html");

    file << "<h2>Show</h2>\n"
            "  <fieldset>"
            "   <legend>"<<entity.name<<"</legend>";
    for(const Variable& var: entity.vecVariable)
        file << "   <span>"<<var.name<<":</span> <label id='"<<var.name<<"'></label><br/>\n";
    file << "  </fieldset>";
}
