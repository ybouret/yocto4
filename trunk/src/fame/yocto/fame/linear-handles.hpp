#ifndef YOCTO_FAME_LINEAR_HANDLES_INCLUDED
#define YOCTO_FAME_LINEAR_HANDLES_INCLUDED 1

#include "yocto/fame/linear.hpp"


namespace yocto
{
    namespace fame
    {
        
        //! storing linear handles for data exchange
        /**
         the pointers must come from dynamically allocated linear spaces.
         */
        class linear_handles
        {
        public:
            const size_t chunk_size;
            explicit linear_handles() throw();
            explicit linear_handles(size_t n);
            virtual ~linear_handles() throw();
            
            void append(linear_space *l);
            void remove(const linear_space *l) throw();
            size_t size() const throw();
            void   free() throw();
            
            linear_space       & operator[](size_t i) throw();
            const linear_space & operator[](const size_t i) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear_handles);
            sorted_vector<linear_ptr,linear_comparator> handles;
        };
        
    }
}

#endif
