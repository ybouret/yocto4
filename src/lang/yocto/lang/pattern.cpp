#include "yocto/lang/pattern.hpp"
#include "yocto/ios/ocstream.hpp"

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
        next(0),
        prev(0),
        uuid(other.uuid)
        {}

        const void *pattern:: content() const throw() { return 0; }

        void  pattern:: encode( code_t code, ios::ostream &fp)
        {
            const char C(code);
            if(C>=32 && C<127 && C!='\"' && C!='\\')
            {
                fp("'%c'",C);
            }
            else
            {
                fp("x%02x",C);
            }

        }

        void  pattern::encode( const token &tkn, ios::ostream &fp)
        {
            fp << '\'';
            for(const t_char *ch=tkn.head;ch;ch=ch->next)
            {
                const char C(ch->code);
                if(C>=32 && C<127)
                {
                    switch(C)
                    {
                        case '\"': fp("\\\""); break;
                        case '\\': fp("\\\\"); break;
                        default:
                            fp("%c",C);
                    }
                }
                else
                {
                    fp("\\x%02x",C);
                }

            }
            fp << '\'';
        }

        void  pattern:: vizlink( ios::ostream &fp, const pattern *src, const pattern *tgt)
        {
            fp.viz(src); fp << " -> "; fp.viz( tgt ); fp << ";\n";
        }

        void pattern:: graphviz(const string &filename) const
        {
            ios::ocstream fp(filename,false);
            fp("digraph G {\n");
            viz(fp);
            fp("}\n");
        }

    }

}

#include "yocto/ios/osstream.hpp"
#include "yocto/string/base64.hpp"

namespace yocto
{
    namespace lang
    {

        string  pattern:: compiled() const
        {
            string ans;
            ios::osstream fp(ans);
            this->save(fp);
            return ans;
        }


        string  pattern:: toBase64() const
        {
            static  const base64::IO &Base64 = base64::IO::instance();

            const string bin = compiled();
            return Base64.Encode(bin);
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
