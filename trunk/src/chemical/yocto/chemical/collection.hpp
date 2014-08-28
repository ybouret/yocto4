#ifndef YOCTO_CHEMICAL_COLLECTION_INCLUDED
#define YOCTO_CHEMICAL_COLLECTION_INCLUDED 1

#include "yocto/chemical/species.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace chemical
    {
        
        
        //! a collection of species
        /**
         the reference counting is done by 
         the solutions...
         */
        class collection
        {
        public:
            explicit collection(size_t n=0);
            virtual ~collection() throw();
            
            size_t size() const throw();
            
            species &add( const string &name, const int charge); //!< only if not in use
            species &add( const char   *name, const int charge); //!< wrapper
            void     remove( const string &name);                //!< only if not in use
            void     remove( const char   *name);                //!< wrapper
            
            species::pointer & operator[](const string &name);
            species::pointer & operator[](const char *  name);
            
            const species::pointer & operator[](const string &name) const;
            const species::pointer & operator[](const char *  name) const;

            //! set indices for all species
            void rebuild_indices() const throw();
            
            typedef species::database::iterator       iterator;
            typedef species::database::const_iterator const_iterator;
            iterator       begin() throw();
            iterator       end()   throw();
            const_iterator begin() const throw();
            const_iterator end()   const throw();
            
            void         increase() const throw();
            void         decrease() const throw();
            const size_t count;
            void output( std::ostream & ) const;
            
            inline friend std::ostream & operator<<( std::ostream &os, const collection &self)
            {
                self.output(os);
                return os;
            }
            
            size_t index_of(const string &name) const;
            size_t index_of(const char   *name) const;
            
        private:
            species::database  db;
            YOCTO_DISABLE_COPY_AND_ASSIGN(collection);
            
        public:
            const size_t max_name_length;
            void find_max_name_length() const throw();
            bool is_consistent() const throw();
            
        };
    }
}

#endif
