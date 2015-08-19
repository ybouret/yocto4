#ifndef YOCTO_PARALLEL_LAYOUT_INCLUDED
#define YOCTO_PARALLEL_LAYOUT_INCLUDED 1


namespace yocto
{
    namespace parallel
    {
        class layout : public object
        {
        public:
            virtual ~layout() throw();
            const size_t dimensions;     //!< 1 | 2 | 3


        protected:
            explicit layout(const size_t dim) throw();

            size_t setup(const void  *lower,
                         const void  *upper,
                         const void  *pitch,
                         const void  *width) throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(layout);
        };

    }
}

#endif

