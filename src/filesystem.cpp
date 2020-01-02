#include "filesystem.h"
#include "buffer.h"
#include <vector>
#include <string>

#if defined(__linux__)
#define PATH_MAX 4096
#include <unistd.h>
#include <glob.h>
#include <cstdlib>

int error_function(const char *epath, int eerrno)
{

}

namespace ice
{
    std::string pwd()
    {
        buffer buf;
        buf.resize(PATH_MAX);
        getcwd(
            (char*)buf.data(),
            buf.size());
        if(buf.data() == NULL)
        {
            throw std::runtime_error("Failed to print working directory");
        }
        return std::string(
            (char*)buf.data(),
            buf.size());
    }

    std::vector<std::string> ls(const std::string& path, const std::string& pattern)
    {
        int glob_result;
        std::vector<std::string> list;
        glob_t files;
        glob_result = glob(
            pattern.c_str(),
            0,
            error_function,
            &files);
        if(glob_result == GLOB_NOSPACE)
        {
            throw std::runtime_error("Out of memory");
        } 
        else if(glob_result == GLOB_ABORTED)
        {
            throw std::runtime_error("Read error");
        }
        else if(glob_result == GLOB_NOMATCH)
        {
            throw std::runtime_error("No matching file");
        }
        else
        {
            char ** paths = files.gl_pathv;
            list.reserve(files.gl_pathc);
            while(list.size() < list.capacity())
            {
                list.push_back(std::string(
                    *paths,
                    strlen(*paths)));
            }
        }
        return list; 
    }

    void mkdir(const std::string& path);

    void mkdirp(const std::string& path);

    void rm(const std::string& path);

    void rdmdir(const std::string& path);
}

#endif