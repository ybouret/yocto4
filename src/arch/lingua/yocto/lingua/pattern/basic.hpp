#ifndef YOCTO_LINGUA_PATTERN_BASIC_INCLUDED
#define YOCTO_LINGUA_PATTERN_BASIC_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
    namespace lingua
    {
        
        //______________________________________________________________________
        //
        //! accept one char by a virtual is_valid method
        //______________________________________________________________________
        class one_char : public pattern
        {
        public:
            virtual ~one_char() throw();
            
            virtual bool accept( source &src );
            
        protected:
            explicit one_char(uint32_t t) throw();
            
        private:
            virtual bool is_valid(char) const throw() = 0;
            YOCTO_DISABLE_COPY_AND_ASSIGN(one_char);
        };
        
        //______________________________________________________________________
        //
        //! accept any char
        //______________________________________________________________________
        class any1 : public one_char
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('A', 'N', 'Y', '1');
            virtual ~any1() throw();
            Y_LINGUA_PATTERN_API();
            
            static any1 *create();
            
            
        protected:
            explicit any1() throw();
            
        private:
            virtual bool is_valid(char) const throw(); //!< true
            YOCTO_DISABLE_ASSIGN(pattern);
        };
        
        //______________________________________________________________________
        //
        //! accept a single char
        //______________________________________________________________________
        class single : public one_char
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('S', 'N', 'G', 'L');
            virtual ~single() throw();
            Y_LINGUA_PATTERN_API();

            static single * create( char ); //! single->data = &value
            const char      value;
            
        private:
            explicit single( char ) throw();
            virtual bool is_valid(char) const throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(single);
        };
        
        //______________________________________________________________________
        //
        //! accept a range of char
        //______________________________________________________________________
        class range : public one_char
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('R', 'N', 'G', 'E');
            
            virtual ~range() throw();
            Y_LINGUA_PATTERN_API();


            static range *create(int,int); //!< range->data = &lower
            const int lower;
            const int upper;
            
        private:
            explicit range(int,int) throw();
            virtual bool is_valid(char) const throw();
            range(const range &) throw();
            YOCTO_DISABLE_ASSIGN(range);
        };
        
        //______________________________________________________________________
        //
        //! accept some chars
        //______________________________________________________________________
        class choice : public one_char
        {
        public:
            
            virtual ~choice() throw();
            
            void append(char c);
            void append(int lo,int hi);
            byte_store chars;
            
        protected:
            explicit choice(uint32_t t) throw();
            choice(const choice &); //!< can't copy an empty choice
            void write( ios::ostream &fp ) const;
            void __viz( ios::ostream &fp ) const;
            
        private:
            YOCTO_DISABLE_ASSIGN(choice);
        };
        
        //______________________________________________________________________
        //
        //! accept whithin some chars
        //______________________________________________________________________
        class within : public choice
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('W', '/', 'I', 'N');
            virtual ~within() throw();
            
            Y_LINGUA_PATTERN_API();


            static  within *create();
            
        private:
            virtual bool is_valid(char) const throw();
            explicit within();
            within( const within &);
            YOCTO_DISABLE_ASSIGN(within);
        };
        
        //______________________________________________________________________
        //
        //! reject some chars
        //______________________________________________________________________
        class none : public choice
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('N', 'O', 'N', 'E');
            virtual ~none() throw();
            
            Y_LINGUA_PATTERN_API();


            static  none    *create();
            
        private:
            virtual bool is_valid(char) const throw();
            explicit none();
            none( const none &);
            YOCTO_DISABLE_ASSIGN(none);
        };
        
    }
}
#endif
