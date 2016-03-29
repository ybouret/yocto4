#include "yocto/lang/syntax/rule.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            
            rule::alist_t & rule::check_following()
            {
                if(!following.is_valid())
                {
                    following.reset( new alist_t() );
                }
                return *following;
            }

            rule:: ~rule() throw()
            {
            }


            rule:: rule(const string &id, uint32_t uu) :
            next(0),
            prev(0),
            label(id),
            uuid(uu),
            following(0)
            {
            }


            void rule:: grow( xnode * &tree, xnode *leaf ) const throw()
            {
                if(leaf)
                {
                    if(!tree)
                    {
                        tree = leaf;
                    }
                    else
                    {
                        assert(!(tree->terminal));
                        tree->append(leaf);
                    }
                }
            }

            const string & rule:: key() const throw()
            {
                return label;
            }


#if 0
            const void * rule:: content() const throw()
            {
                return 0;
            }
#endif
            
            void rule:: encode_label( ios::ostream &fp ) const
            {
                for(size_t i=0;i<label.size();++i)
                {
                    const char C = label[i];
                    if(C>=32 && C<127)
                    {
                        switch(C)
                        {
                            case '\\': fp("\\\\"); break;
                            case '"' : fp("\\\""); break;

                            default:
                                fp("%c",C);
                        }

                    }
                    else
                    {
                        fp("\\x%02x",C);
                    }
                }
            }
            
            
        }
    }
}


