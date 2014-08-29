#ifndef YOCTO_LANG_PATTERN_BASIC_INCLUDED
#define YOCTO_LANG_PATTERN_BASIC_INCLUDED 1

#include "yocto/lang/pattern.hpp"

namespace yocto
{
    namespace lang
    {
        
        //______________________________________________________________________
        //
        // interface for pattern returning one valid char
        //______________________________________________________________________
        class one_char : public pattern
        {
        public:
            virtual ~one_char() throw();
            virtual  bool match( source &src, ios::istream &fp);
            
        protected:
            explicit one_char(const uint32_t user_type) throw();
            
        private:
            virtual bool is_valid( int C ) const throw() = 0;
            YOCTO_DISABLE_COPY_AND_ASSIGN(one_char);
        };
        
        class any1 : public one_char
        {
        public:
            
        private:
            YOCTO_DISABLE_ASSIGN(any1);
        };
        
        
        //______________________________________________________________________
        //
        // matching a single char
        //______________________________________________________________________
        class single : public one_char
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('S', 'N', 'G', 'L');
            virtual ~single() throw();
            
            YOCTO_LANG_PATTERN_API();
            
            const int value; //!< data = &value
            static single *create( const int val );
            
        private:
            virtual bool is_valid( int C ) const throw();
            explicit single( const int which) throw();
            
            YOCTO_DISABLE_ASSIGN(single);
            
        };
        
        
        
    }
}

#endif

