#ifndef SCALABLE_BLOOM_FILTER_H
#define SCALABLE_BLOOM_FILTER_H

#include "partitioned_bloom_filter.hpp"

namespace sanath
{
    template<typename T>
    class scalable_bloom_filter : public partitioned_bloom_filter<T>
    {
    public:
        scalable_bloom_filter(size_t n, double epsilon = default_error_rate) : partitioned_bloom_filter(n, epsilon)
        {
            
        }

    private:
    };
}

#endif