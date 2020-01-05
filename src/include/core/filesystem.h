#include "types.h"

namespace ice
{
namespace core
{

class file
{
public:
    file();
    file(const std::string& path);
    
};

class system
{
public:
    std::string pwd();

    std::vector<std::string> ls(const std::string& path);

    void mkdir(const std::string& path);

    void mkdirp(const std::string& path);

    void rm(const std::string& path);

    void rdmdir(const std::string& path);
};

}
}