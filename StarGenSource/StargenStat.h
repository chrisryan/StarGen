#pragma once
/*
 * StargenStat used to hold internal statistic data
 */

namespace StarGen {

    class StargenStat {
        public:
            StargenStat() {
                this->earthlike = 0;
                this->total_earthlike = 0;
                this->habitable = 0;
                this->total_habitable = 0;

                this->min_breathable_terrestrial_g = 1000.0;                                                                    
                this->min_breathable_g = 1000.0;                                                                                
                this->max_breathable_terrestrial_g = 0.0;                                                                       
                this->max_breathable_g = 0.0;                                                                                   
                this->min_breathable_temp = 1000.0;                                                                             
                this->max_breathable_temp = 0.0;                                                                                
                this->min_breathable_p = 100000.0;                                                                              
                this->max_breathable_p = 0.0;                                                                                   
                this->min_breathable_terrestrial_l = 1000.0;                                                                    
                this->min_breathable_l = 1000.0;                                                                                
                this->max_breathable_terrestrial_l = 0.0;                                                                       
                this->max_breathable_l = 0.0;                                                                                   
                this->max_moon_mass = 0.0;
            }

            int earthlike;
            int total_earthlike;
            int habitable;
            int total_habitable;

            long double min_breathable_terrestrial_g;
            long double min_breathable_g;
            long double max_breathable_terrestrial_g;
            long double max_breathable_g;
            long double min_breathable_temp;
            long double max_breathable_temp;
            long double min_breathable_p;
            long double max_breathable_p;
            long double min_breathable_terrestrial_l;
            long double min_breathable_l;
            long double max_breathable_terrestrial_l;
            long double max_breathable_l;
            long double max_moon_mass;
    };
};
