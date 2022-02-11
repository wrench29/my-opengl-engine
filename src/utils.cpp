#include <string>
#include <vector>
#include <libgen.h>         // dirname
#include <unistd.h>         // readlink
#include <linux/limits.h>   // PATH_MAX

#include <boost/algorithm/string.hpp>

#include "utils.hpp"

std::string assets_dir()
{
    std::vector<std::string> splitted_raw_path;
    // std::string raw_path = std::filesystem::current_path();

    char result[PATH_MAX] = {0};
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    assert(count != -1);
    std::string raw_path = std::string(dirname(result));

    boost::split(splitted_raw_path, raw_path, boost::is_any_of("/"));
    if (splitted_raw_path[splitted_raw_path.size() - 1] == "build")
    {
        splitted_raw_path[splitted_raw_path.size() - 1] = "assets";    
    }
    else
    {
        splitted_raw_path.push_back("assets");
    }
    std::string ready_path;
    for (std::string &str : splitted_raw_path)
    {
        ready_path.append(str);
        ready_path.append("/");
    }
    return ready_path;
}
