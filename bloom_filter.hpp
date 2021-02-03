#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <cmath>
#include <bitset>
#include <memory>

#define DEFAULT_ERROR_RATE 0.0001

namespace sanath {
    template<typename T>
    class bloom_filter {
        public:
            bloom_filter(size_t n) {
                elements = n;
                desired_error = DEFAULT_ERROR_RATE;

                // optimal values
                filter_size = round(-1 * (elements * log2(desired_error)) / log(2));
                hash_functions = round(-log2(desired_error));

                filter = std::unique_ptr< std::bitset > (new std::bitset<filter_size>);
            }
            bloom_filter(size_t n, double epsilon) {
                elements = n;
                desired_error = epsilon;

                // optimal values
                filter_size = round(-1 * (elements * log2(desired_error)) / log(2));
                hash_functions = round(-log2(desired_error));
            }
            void insert(T obj) {
                for(int i = 0; i < hash_functions; i++) {
                    filter->set(hash(obj, i));
                }
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
            std::unique_ptr< std::bitset > filter;

            // hash function (djb2)
            unsigned long long hash(T *obj, unsigned long long init) {
                char *ptr = reinterpret_cast<char *>(obj);

                for(size_t i = 0, len = sizeof(*obj); i < len; i++) {
                    init = ((init << 5) + init) + ptr[i];
                }

                // scale down
                return init % filter_size;
            }
    };
}

#endif