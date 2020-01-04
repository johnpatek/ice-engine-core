#include "types.h"
#include "memory.h"

namespace ice
{
namespace core
{
    template<class T>class resource
    {
    public:
        resource(
            const byte_type * data, 
            const size_type size)
        {
            deserialize(data,size);
        }

        static T deserialize(
            const byte_type * data, 
            const size_type size);

        static buffer serialize(
            const T& t);
    };
}
}