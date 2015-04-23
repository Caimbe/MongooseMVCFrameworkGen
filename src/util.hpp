#ifndef UTIL_HPP
#define UTIL_HPP

#include <iomanip>
#include <sstream>
#ifndef WIN32
#   define DLL_EXPORT
#	include <unistd.h>
#   include <sys/stat.h>
#else
#   define DLL_EXPORT __declspec(dllexport)
#   include <direct.h>
#endif
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;

inline std::string to_string(const long double &_Val, const streamsize& precision)
{	// convert long double to string

    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << _Val;
    return out.str();
}

inline std::string to_string(const std::tm& tm, const char* format="%4d-%02d-%02d")
{
    std::tm _default = std::tm();
    if(difftime(mktime((std::tm*)&tm), mktime(&_default)) == 0)
        return "";

    char buffer[100];
    sprintf(buffer, format, tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday);

    return buffer;
}


inline string table2className(string& table)
{
    string className = table;
    className[0] = toupper(className[0]);
    for(size_t find = className.find("_"); find != string::npos; find=className.find("_"))
    {
        className.erase(find, 1);
        className[find] = toupper(className[find]);
    }
    return className;
}
inline string upcaseFirstChar(string& table)
{
    string className = table;
    className[0] = toupper(className[0]);
    return className;
}

#if _MSC_VER || __MINGW32__
#	define makedir(x) mkdir(x)
#else
#	define makedir(x) mkdir(x, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

#endif // UTIL_HPP
