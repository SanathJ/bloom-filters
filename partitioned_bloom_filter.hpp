#ifndef PARTITIONED_BLOOM_FILTER_H
#define PARTITIONED_BLOOM_FILTER_H

#include "bloom_filter.hpp"

namespace sanath
{
    template<typename T>
    class partitioned_bloom_filter : public bloom_filter<T>
    {
    public:
        partitioned_bloom_filter(size_t n, double epsilon = default_error_rate) : bloom_filter(n, epsilon)
        {
            // lowest number divisible by hash_functions greater than optimal
            filter_size = round((-1 * (elements * log2(desired_error)) / log(2)) / hash_functions) * hash_functions;
            
            if(filter_size > std::numeric_limits<uint32_t>::max())
            {
                throw std::runtime_error("Filter cannot grow to this size");
            }

            filter.resize(filter_size, false);

            partition_size = filter_size / hash_functions;
        }
        uint32_t get_partition_size()
        {
            return partition_size;
        }
    protected:
        uint32_t partition_size;

        uint32_t hash(T *key, uint32_t k)
        {
            hasher hash_provider;
            
            uint32_t hash1 = hash_provider.fnv1a(key, sizeof(T));
            uint32_t hash2 = hash_provider.MurmurHash3(key, sizeof(T));

            // Kirsch-Mitzenmacher optimization (index within partion [0, partition_size - 1])
            uint64_t index = (hash1 + ((k + 1) * hash2)) % partition_size;

            // translate index to final index
            return (k * partition_size) + index;
        }
    };
}

#endif