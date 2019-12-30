#include "types.h"
#include <vector>

namespace ice
{
    template<class AllocatorType = std::allocator<byte_type>>
    class buffer : public std::vector<byte_type>
    {

    };
}
