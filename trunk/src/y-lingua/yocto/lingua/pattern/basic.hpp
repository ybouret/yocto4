#ifndef YOCTO_LINGUA_PATTERN_BASIC_INCLUDED
#define YOCTO_LINGUA_PATTERN_BASIC_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/code/fourcc.hpp"
#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    namespace lingua
    {
        
        //! accept one char by a virtual is_valid method
        class one_char : public pattern
        {
        public:
            virtual ~one_char() throw();
            
            virtual bool accept( source &src );
            
        protected:
            explicit one_char() throw();
            
        private:
            virtual bool is_valid(char) const throw() = 0;
            YOCTO_DISABLE_COPY_AND_ASSIGN(one_char);
        };
        
        //! accept any char
        class any1 : public one_char
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('A', 'N', 'Y', '1');
            virtual ~any1() throw();
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;

            static any1 *create();
            
        protected:
            explicit any1() throw();
            
        private:
            virtual bool is_valid(char) const throw(); //!< true
            YOCTO_DISABLE_ASSIGN(pattern);
        };
        
        //! accept a single char
        class single : public one_char
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('S', 'N', 'G', 'L');
            virtual ~single() throw();
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;

            static single * create( char );
            const char      value;
            
        private:
            explicit single( char ) throw();
            virtual bool is_valid(char) const throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(single);
        };
        
        //! accept a range of char
        class range : public one_char
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('R', 'N', 'G', 'E');
            
            virtual ~range() throw();
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;

            static range *create(int,int);
            const int lower;
            const int upper;
            
        private:
            explicit range(int,int) throw();
            virtual bool is_valid(char) const throw();
            range(const range &) throw();
            YOCTO_DISABLE_ASSIGN(range);
        };
        
        //! accept some chars
        class choice : public one_char
        {
        public:
            static const size_t default_capacity = 16;
            
            virtual ~choice() throw();
            
            void append(char c);
            void append(int lo,int hi);
            
        protected:
            explicit choice();
            choice(const choice &); //!< can't copy an empty choice
            void write( ios::ostream &fp ) const;

            sorted_vector<uint8_t> chars;
            
        private:
            YOCTO_DISABLE_ASSIGN(choice);
        };
        
        //! accept whithin some chars
        class within : public choice
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('W', '/', 'I', 'N');
            virtual ~within() throw();
            
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;

            static  within *create();
            
        private:
            virtual bool is_valid(char) const throw();
            explicit within();
            within( const within &);
            YOCTO_DISABLE_ASSIGN(within);
        };
        
        //! reject some chars
        class none : public choice
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('N', 'O', 'N', 'E');
            virtual ~none() throw();
            
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;

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
