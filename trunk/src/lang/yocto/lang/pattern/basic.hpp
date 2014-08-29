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
            virtual bool is_valid( const code_type C ) const throw() = 0;
            YOCTO_DISABLE_COPY_AND_ASSIGN(one_char);
        };
        
        //______________________________________________________________________
        //
        // accept any char
        //______________________________________________________________________
        class any1 : public one_char
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('A', 'N', 'Y', '1');
            virtual ~any1() throw();
            static  pattern *create();
            virtual pattern *clone() const;
            
        private:
            explicit any1() throw();
            virtual bool is_valid( const code_type ) const throw(); //!< true
            YOCTO_DISABLE_COPY_AND_ASSIGN(any1);
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
            
            
            const int value; //!< data = &value
            
            static  pattern *create( const int val );
            virtual pattern *clone() const;

        private:
            virtual bool is_valid( const code_type ) const throw();
            explicit single(const int which) throw();
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(single);
            
        };
        
        
        //______________________________________________________________________
        //
        // matching a range of char
        //______________________________________________________________________
        class range : public one_char
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('R', 'A', 'N', 'G');
            virtual ~range() throw();
            
            const code_type lower;
            const code_type upper;
            
            static  pattern *create( const code_type lo, const code_type up);
            virtual pattern *clone() const;
            
        private:
            explicit range(const int lo, const int up) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(range);
            virtual bool is_valid( const code_type ) const throw();
        };
        
        //______________________________________________________________________
        //
        // using a choice of char
        //______________________________________________________________________
        class choice : public one_char
        {
        public:
            virtual ~choice() throw();
            
            bytes_store db;
            
        protected:
            explicit choice(const uint32_t id);
            choice(const choice &other);

        private:
            YOCTO_DISABLE_ASSIGN(choice);
        };
        
        //______________________________________________________________________
        //
        // within a choice of char
        //______________________________________________________________________
        class within : public choice
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('W', '/', ' ', ' ');
            virtual ~within() throw();
            
            static  choice  *create();
            virtual pattern *clone() const;
            
        private:
            YOCTO_DISABLE_ASSIGN(within);
            explicit within();
            within(const within&);
            virtual bool is_valid(const code_type) const throw();
            
        };
        
        //______________________________________________________________________
        //
        // none in choice of char
        //______________________________________________________________________
        class none : public choice
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('N', 'O', 'N', 'E');
            virtual ~none() throw();
            
            static  choice  *create();
            virtual pattern *clone() const;
            
        private:
            YOCTO_DISABLE_ASSIGN(none);
            explicit none();
            none(const none&);
            virtual bool is_valid(const code_type) const throw();
            
        };

        
        
    }
}

#endif

