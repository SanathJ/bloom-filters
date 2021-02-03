#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <cmath>
#include <bitset>

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
            }
            bloom_filter(size_t n, double epsilon) {
                elements = n;
                desired_error = epsilon;

                // optimal values
                filter_size = round(-1 * (elements * log2(desired_error)) / log(2));
                hash_functions = round(-log2(desired_error));
            }

        private:
            // assumed maximum elements
            size_t elements;
            
            double desired_error;
            
            // optimal filter size based on elements and desired error
            size_t filter_size;

            // optimal hash function count
            int hash_functions;
    };
}

#endif