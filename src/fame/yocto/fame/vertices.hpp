#ifndef YOCTO_VERTICES_INCLUDED
#define YOCTO_VERTICES_INCLUDED 1

#include "yocto/fame/array1d.hpp"
#include "yocto/core/list.hpp"


namespace yocto
{
    namespace fame
    {
        template <size_t DIM,typename T>
        class Vertex
        {
        public:
            YOCTO_MAKE_OBJECT
            typedef typename vertex_for<DIM,T>::type Type;
            
            Vertex *next;
            Vertex *prev;
            
            inline Vertex( T &X ) throw() : next(0), prev(0), q()
            {
                clr();
                q[0] = &X;
            }
            
            inline Vertex( T &X, T &Y ) throw() : next(0), prev(0), q()
            {
                clr();
                q[0] = &X;
                q[1] = &Y;
            }
            
            inline Vertex( T &X, T &Y, T &Z ) throw() : next(0), prev(0), q()
            {
                clr();
                q[0] = &X;
                q[1] = &Y;
                q[2] = &Z;
            }
            
            
            inline  T       &x() throw()       { assert(DIM==1); assert(0!=q[0]); return *q[0]; }
            inline  const T &x() const throw() { assert(DIM==1); assert(0!=q[0]); return *q[0]; }
            
            inline  T       &y() throw()       { assert(DIM==2); assert(0!=q[1]); return *q[1]; }
            inline  const T &y() const throw() { assert(DIM==2); assert(0!=q[1]); return *q[1]; }
            
            inline  T       &z() throw()       { assert(DIM==3); assert(0!=q[2]); return *q[2]; }
            inline  const T &z() const throw() { assert(DIM==3); assert(0!=q[2]); return *q[2]; }
            
            
            inline         ~Vertex() throw() {}
            
            
            class List : public core::list_of_cpp<Vertex>
            {
            public:
                inline explicit List() throw() {}
                inline virtual ~List() throw() {}
                
                inline void append( T &X )                     { this->push_back( new Vertex(X)     ); }
                inline void append( T &X, T &Y ) throw()       { this->push_back( new Vertex(X,Y)   ); }
                inline void append( T &X, T &Y, T &Z ) throw() { this->push_back( new Vertex(X,Y,Z) ); }
                
                
                inline void Rectilinear( array1D<T> &X )
                {
                    for(unit_t i=X.lower;i<=X.upper;++i) append(X[i]);
                }
                
                inline void Rectilinear( array1D<T> &X, array1D<T> &Y )
                {
                    for(unit_t i=X.lower;i<=X.upper;++i)
                    {
                        for(unit_t j=Y.lower;j<=Y.upper;++j)
                        {
                            append(X[i],Y[j]);
                        }
                    }
                }
                
                inline void Rectilinear( array1D<T> &X, array1D<T> &Y, array1D<T> &Z )
                {
                    for(unit_t i=X.lower;i<=X.upper;++i)
                    {
                        for(unit_t j=Y.lower;j<=Y.upper;++j)
                        {
                            for(unit_t k=Z.lower;k<=Z.upper;++k)
                            {
                                append(X[i],Y[j],Z[k]);
                            }
                        }
                    }
                }
                
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(List);
            };
            
        private:
            T *q[DIM];
            inline void clr() throw() { for(size_t i=0;i<DIM;++i) q[i]=0; }
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(Vertex);
        };
    }
}

#endif
