#include "yocto/lingua/syntax/analyzer.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/lingua/syntax/compound.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sort/quick.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            analyzer:: ~analyzer() throw()
            {
            }
            
            analyzer:: analyzer(const grammar &g)
            {

                const size_t   nr = g.num_rules();
                vector<string> Terms(nr,as_capacity);
                vector<string> Rules(nr,as_capacity);

                for(const rule *r = g.top_level();r;r=r->next)
                {
                    if(r->flags==property::jettison)
                        continue;

                    switch(r->uuid)
                    {
                        case terminal::UUID:
                            Terms.push_back(r->label);
                            break;
                            
                        case aggregate::UUID:
                            Rules.push_back(r->label);
                            break;
                    }
                }

                quicksort(Terms);
                quicksort(Rules);
                std::cerr << "|_Terms" << std::endl;
                for(size_t i=1;i<=Terms.size();++i)
                {
                    std::cerr << "  |_" << Terms[i] << std::endl;
                }
                std::cerr << "|_Rules" << std::endl;
                for(size_t i=1;i<=Rules.size();++i)
                {
                    std::cerr << "  |_" << Rules[i] << std::endl;
                }
                
            }
            
        }
    }
}
