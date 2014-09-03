#include "yocto/lang/pattern/basic.hpp"

namespace yocto
{
    namespace lang
    {
        
        one_char:: ~one_char() throw() {}
        
        one_char:: one_char( const uint32_t user_type ) throw() :
        pattern(user_type)
        {
        }
        
        
        bool one_char:: match(source &src,ios::istream &fp)
        {
            assert( 0 == size );
            t_char *ch = src.get(fp);
            
            if(ch)
            {
                if( is_valid(ch->code) )
                {
                    push_back(ch);
                    return true;
                }
                else
                {
                    src.unget(ch);
                    return false;
                }
            }
            else
                return false;
        }
    }
}


namespace yocto
{
    namespace lang
    {
        any1:: ~any1() throw()
        {
        }
        
        any1:: any1() throw() : one_char( tag )
        {
            self = (any1 *)this;
        }
        
        pattern * any1:: create() { return new any1(); }
        
        pattern * any1::clone() const
        {
            return new any1();
        }
        
        bool any1:: is_valid(const code_type) const throw() { return true; }
        
        void any1:: viz(ios::ostream &fp) const
        {
            fp.viz(this); fp << " [label=ANY1];\n";
        }
        
        void any1:: save( ios::ostream &fp) const
        {
            fp.emit(tag);
        }
        
        void any1::detect( first_chars &fc ) const
        {
            fc.free();
            fc.ensure(256);
            fc.accept_empty = false;
            for(int i=255;i>=0;--i) fc.add(code_type(i));
        }
        
    }
    
}

namespace yocto
{
    namespace lang
    {
        single:: ~single() throw()
        {
        }
        
        single:: single(const int which) throw() :
        one_char(tag),
        value(which)
        {
            self= (single *)this;
        }
        
        pattern * single:: create( const code_type val )
        {
            return new single(val);
        }
        
        pattern *single:: clone() const
        {
            return new single( value );
        }
        
        bool single:: is_valid(const code_type C) const throw()
        {
            return C == value;
        }
        
        void single:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [label=\"'";
            outviz(value, fp);
            fp << "'\"];\n";
        }
        
        void single:: save( ios::ostream &fp) const
        {
            fp.emit(tag);
            fp.emit<code_type>(value);
        }
        
        void single:: detect(first_chars &fc) const
        {
            fc.free();
            fc.ensure(1);
            fc.accept_empty = false;
            fc.add(value);
        }
        
    }
    
}

#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace lang
    {
        range:: ~range() throw()
        {}
        
        range:: range( const code_type lo, const code_type up) throw() :
        one_char(tag),
        lower( min_of(lo,up) ),
        upper( max_of(lo,up) )
        {
            self = (range *)this;
        }
        
        bool range:: is_valid(const code_type C) const throw()
        {
            return C>=lower && C <=upper;
        }
        
        pattern * range:: clone() const
        {
            return new range(lower,upper);
        }
        
        pattern * range:: create(const code_type lo, const code_type up)
        {
            return new range(lo,up);
        }
        
        void range:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [label=\"['";
            outviz(lower, fp);
            fp << "'-'";
            outviz(upper,fp);
            fp << "']\"];\n";
        }
        
        void range:: save( ios::ostream &fp) const
        {
            fp.emit(tag);
            fp.emit(lower);
            fp.emit(upper);
        }
        
        void range:: detect(first_chars &fc) const
        {
            fc.free();
            fc.accept_empty = false;
            const size_t n = size_t(upper-lower)+1;
            code_type    c = lower;
            for(size_t i=n;i>0;--i,++c)
            {
                fc.add(c);
            }
        }
        
    }
    
}


namespace yocto
{
    namespace lang
    {
        choice:: ~choice() throw()
        {
        }
        
        choice:: choice( const uint32_t id ) :
        one_char(id),
        chars()
        {
            
        }
        
        choice:: choice(const choice &other ) :
        one_char(other.type),
        chars( other.chars )
        {
        }
        
        void choice:: __viz( ios::ostream &fp ) const
        {
            
            for(size_t i=1; i <= chars.size(); ++i)
            {
                outviz(chars[i], fp);
            }
            
        }
        
        
        void choice:: __save( ios::ostream &fp ) const
        {
            const uint32_t n = chars.size();
            fp.emit(n);
            for(uint32_t i=1; i <= n; ++i)
            {
                fp.emit(chars[i]);
            }
        }
        
        void choice:: append(char lo, char up)
        {
            if(lo>up) cswap(lo, up);
            for(size_t n = size_t(up-lo)+1;n>0;--n,++lo)
            {
                (void) chars.insert(lo);
            }
        }
        
        
    }
}

namespace yocto
{
    namespace lang
    {
        within:: ~within() throw() {}
        
        within:: within() :
        choice(tag)
        {
            self = (within *)this;
        }
        
        within:: within( const within &other ) :
        choice(other)
        {
            self = (within *)this;
        }
        
        
        choice * within:: create()
        {
            return new within();
        }
        
        pattern * within:: clone() const
        {
            return new within(*this);
        }
        
        bool within:: is_valid(const code_type C) const throw()
        {
            return chars.search(C);
        }
        
        void within:: viz(ios::ostream &fp) const
        {
            fp.viz(this);
            fp << " [ label=\"[";
            __viz(fp);
            fp << "]\"];\n";
        }
        
        void within:: save( ios::ostream &fp) const
        {
            fp.emit(tag);
            __save(fp);
        }
        
        void within:: detect(first_chars &fc) const
        {
            fc.free();
            const size_t n = chars.size();
            if(n>0)
            {
                fc.accept_empty = false;
                fc.ensure(n);
                for(size_t i=1;i<=n;++i) fc.add( chars[i] );
            }
            else
            {
                fc.accept_empty = true;
            }
        }
    }
    
}


namespace yocto
{
    namespace lang
    {
        none:: ~none() throw() {}
        
        none:: none() :
        choice(tag)
        {
            self = (none *)this;
        }
        
        none:: none( const none &other ) :
        choice(other)
        {
            self = (none *)this;
        }
        
        
        choice * none:: create()
        {
            return new none();
        }
        
        pattern * none:: clone() const
        {
            return new none(*this);
        }
        
        bool none:: is_valid(const code_type C) const throw()
        {
            return !chars.search(C);
        }
        
        void none:: viz(ios::ostream &fp) const
        {
            fp.viz(this);
            fp << " [ label=\"[^";
            __viz(fp);
            fp << "]\"];\n";
        }
        
        void none:: save( ios::ostream &fp) const
        {
            fp.emit(tag);
            __save(fp);
        }
        
        void none:: detect(first_chars &fc) const
        {
            fc.free();
            const size_t n = chars.size();
            if(n>=256)
            {
                fc.accept_empty = true;
            }
            else
            {
                fc.ensure(256-n);
                for(int i=0;i<=255;++i)
                {
                    const code_type c(i);
                    if( chars.search(c) )
                        continue;
                    fc.add(c);
                }
            }
        }
        
        
    }
    
}







