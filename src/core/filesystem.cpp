#include "core/memory.h"
#include "core/filesystem.h"
#include <vector>
#include <string>
#include <stdexcept>
#if defined(__linux__)
#define PATH_MAX 4096
#include <unistd.h>
#include <glob.h>
#include <cstdlib>

int error_function(const char *epath, int eerrno)
{
    return 0;
}

namespace ice
{

}

#endif