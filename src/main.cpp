#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "processfiles.h"

using namespace std;

enum ExitReturn{
    SUCCESS=0,
    SUCCESS_IN_COMMAND_LINE=1,
    ERROR_IN_COMMAND_LINE=2,
    ERROR_IN_FILE_LOCK=3,
    ERROR_UNHANDLED_EXCEPTION=4,
};


Options process_command_line(int argc, char** argv);

int main(int argc, char** argv)
{
    try{
        Options options = process_command_line(argc, argv);

        ProcessFiles p(options);

    }catch(exception& ex){
        std::cerr << "Unhandled Exception reached the top of main: "
                  << ex.what() << ", application will now exit" << std::endl;
        return ExitReturn::ERROR_UNHANDLED_EXCEPTION;
    }

    return ExitReturn::SUCCESS;
}

Options process_command_line(int argc, char** argv)
{
    namespace po = boost::program_options;
    Options opc;
    po::options_description desc("Options");

    desc.add_options()
            ("help,h", "Print this messages")
            ("input-file", po::value< vector<string> >(), "input files")
            ("dir-view,d", po::value<string>()->default_value("view/"), "set dir to generete files for view, default is: ./view")
            ("model,m", po::value<bool>()->default_value(false), "create class model")
            ("view,v", po::value<bool>()->default_value(false), "create file html view")
            ("controller,c", po::value<bool>()->default_value(false), "create class controller")
    ;

    /************input-file not obrig.*****/
    po::positional_options_description p;
    p.add("input-file", -1);
    /***************************************/

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm); // throws on error, so do after help in case there are any problems

    if (vm.count("help") || vm.size()==0)
    {
        std::cout << "Basic Command Line Parameter App" << std::endl
                  << desc << std::endl;
        std::exit(ExitReturn::SUCCESS);
    }
    if (vm.count("input-file"))
    {
        opc.vecFileName = vm["input-file"].as< vector<string> >();
    }
    if(vm.count("dir-view"))
    {
        opc.viewDir = vm["dir-view"].as<string>();
    }
    if(vm.count("model"))
    {
        opc.model = vm["model"].as<bool>();
    }
    if(vm.count("view"))
    {
        opc.view = vm["view"].as<bool>();
    }
    if(vm.count("controller"))
    {
        opc.controller = vm["controller"].as<bool>();
    }

    return opc;
}
