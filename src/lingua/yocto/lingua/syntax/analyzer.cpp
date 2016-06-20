#include "yocto/lingua/syntax/analyzer.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/lingua/syntax/compound.hpp"

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
                for(const rule *r = g.top_level();r;r=r->next)
                {
                    if(r->flags==property::jettison)
                        continue;
                    switch(r->uuid)
                    {
                        case terminal::UUID:
                            std::cerr << "register TERM " << r->label << std::endl;
                            break;
                            
                        case aggregate::UUID:
                            std::cerr << "register RULE " << r->label << std::endl;
                            break;
                    }
                    
                }
                
            }
            
        }
    }
}
