#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <cmath>
#include <vector>
#include "hasher.hpp"

#define DEFAULT_ERROR_RATE 0.0001

namespace sanath 
{
    template<typename T>
    class bloom_filter 
    {
    public:
        bloom_filter(size_t n, double epsilon = DEFAULT_ERROR_RATE) 
        {
            elements = n;
            desired_error = epsilon;

            // optimal values
            filter_size = round(-1 * (elements * log2(desired_error)) / log(2));
            hash_functions = round(-log2(desired_error));

            filter.resize(filter_size, false);
        }

        void insert(T& obj) 
        {
            for(int i = 0; i < hash_functions; i++) 
            {
                filter[hash(&obj, i)] = true;
            }
        }
        
        bool lookup(T& obj) 
        {
            for(int i = 0; i < hash_functions; i++) 
            {
                // if any of the hashed bits are not set, the object is not present
                if (!filter[hash(&obj, i)]) 
                {
                    return false;
                }
            }
            // the object may be present
            return true;
        }
    
    private:
        // assumed maximum elements
        size_t elements;
        
        double desired_error;
        
        // optimal filter size based on elements and desired error
        size_t filter_size;

        // optimal hash function count
        int hash_functions;

        // pointer to filter
        std::vector<bool> filter;

        uint32_t hash(T *key, uint32_t k)
        {
            hasher hash_provider;
            
            uint32_t hash1 = hash_provider.fnv1a(key, sizeof(T));
            uint32_t hash2 = hash_provider.MurmurHash3(key, sizeof(T));

            // Kirsch-Mitzenmacher optimization
            return (hash1 + ((k + 1) * hash2)) % filter_size;
        }

    };
}

#endif