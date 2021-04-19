#ifndef SCALABLE_BLOOM_FILTER_H
#define SCALABLE_BLOOM_FILTER_H

#include "partitioned_bloom_filter.hpp"

namespace sanath
{
    template<typename T, class C = bloom_filter<T> >
    class scalable_bloom_filter
    {
    public:
        scalable_bloom_filter(size_t n, double epsilon = C::default_error_rate / 10,
                                uint16_t s = 3, double r = 0.9)
        {
            current_series_index = 0;
            current_elements = 0;
            slice_growth_factor = s;
            tightening_ratio = r;
            
            // initial values for first filter
            current_max_size = n;
            error_rate = epsilon * (1 - tightening_ratio);
            
            filter_series.push_back(C(current_max_size, error_rate));
        }

        void insert(T& obj) 
        {
            // add new filter if old is full
            if(current_elements > current_max_size)
            {
                current_elements = 0;
                current_max_size *= slice_growth_factor;
                error_rate *= (1 - tightening_ratio);
                
                filter_series.push_back(C(current_max_size, error_rate));
                current_series_index++;
            }

            // add element to filter
            filter_series[current_series_index].insert(obj);
            current_elements++;
        }

        bool lookup(T& obj) 
        {
            for(int i = 0; i <= current_series_index; i++) 
            {
                // if any of the filters report its presence, it might be present
                if (filter_series[i].lookup(obj)) 
                {
                    return true;
                }
            }
            // the object isn't present
            return false;
        }
    
        
    private:
        size_t current_series_index;
        size_t current_elements;
        size_t current_max_size;
        double error_rate;
        uint16_t slice_growth_factor;
        double tightening_ratio;
        
        std::vector< C > filter_series;
    };
}

#endif