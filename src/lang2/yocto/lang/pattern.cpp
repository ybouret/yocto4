#include "yocto/lang/pattern.hpp"

namespace yocto
{
    namespace lang
    {
        pattern:: pattern(uint32_t ID) throw() : next(0), prev(0), uuid(ID)
        {
        }


        pattern:: ~pattern() throw()
        {

        }

        pattern:: pattern(const pattern &other) throw() :
        uuid(other.uuid)
        {}

        const void *pattern:: content() const throw() { return 0; }

        void  pattern:: encode( code_t code, ios::ostream &fp)
        {
            const char C(code);
            if(C>=32&&C<127)
            {
                fp("'%c'",C);
            }
            else
            {
                fp("x%02x",C);
            }
            
        }

        void  pattern:: vizlink( ios::ostream &fp, const pattern *src, const pattern *tgt)
        {
            fp.viz(src); fp << " -> "; fp.viz( tgt ); fp << ";\n";
        }
    }
}


namespace yocto
{
    namespace lang
    {
        p_list::  p_list() throw() : core::list_of_cpp<pattern>() {}
        p_list:: ~p_list() throw() {}

        p_list:: p_list(const p_list &other) :core::list_of_cpp<pattern>()
        {
            for(pattern *p = other.head; p; p=p->next)
            {
                push_back( p->clone() );
            }
        }

        p_list & p_list:: operator=( const p_list &other )
        {
            p_list tmp(other);
            swap_with(tmp);
            return *this;
        }

    }


}
