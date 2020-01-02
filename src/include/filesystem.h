#include "types.h"

namespace ice
{
    std::string pwd();

    std::vector<std::string> ls(const std::string& path);

    void mkdir(const std::string& path);

    void mkdirp(const std::string& path);

    void rm(const std::string& path);

    void rdmdir(const std::string& path);
}