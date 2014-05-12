#ifndef PYCK_C_MATRIX_INCLUDED
#define PYCK_C_MATRIX_INCLUDED 1

#include "./exception.hpp"
#include <new>
#include <iostream>
#include <fstream>

template <typename T>
class C_Matrix
{
public:
	class Row
	{
	public:
		const size_t cols;
		Row(T *p, size_t nc) throw(): cols(nc), r(p) { assert(r);assert(cols);}
        
		inline T &       operator[]( size_t c ) throw()       { assert(c<cols); return r[c]; }
		inline const T & operator[]( size_t c ) const throw() { assert(c<cols); return r[c]; }
        
	private:
		T *r;
		~Row();
		Row(const Row&);
		Row&operator=(const Row&);
	};
    
	const size_t rows;
	const size_t cols;
	const size_t items;;
    
	inline C_Matrix( size_t nr, size_t nc) :
	rows(nr),
    cols(nc),
    items( rows*cols ),
    wksp(0),
    row(0)
	{
		build();
	}
    
    inline C_Matrix() throw() :
    rows(0),
    cols(0),
    items(0),
    wksp(0),
    row(0)
    {
        
    }
    
	virtual ~C_Matrix() throw()
	{
		delete []wksp;
		wksp = 0;
		row  = 0;
	}
    
    inline void swap_with( C_Matrix &other ) throw()
    {
        cswap_const(rows,other.rows);
        cswap_const(cols,other.cols);
        cswap_const(items,other.items);
        cswap(wksp,other.wksp);
        cswap(row,other.row);
    }
    
    inline void make( size_t nr, size_t nc )
    {
        if(rows!=nr||cols!=nc)
        {
            C_Matrix  tmp(nr,nc);
            swap_with(tmp);
        }
    }
    
    inline void ldz() throw()
    {
        if(rows>0)
        {
            T *p = &row[0][0];
            for(size_t i=items;i>0;--i,++p) *p = 0;
        }
    }
    
    
	inline Row &       operator[](size_t r) throw()       { assert(r<rows); return row[r]; }
	inline const Row & operator[](size_t r) const throw() { assert(r<rows); return row[r]; }
    
    //! scilab/octave output
	friend inline std::ostream & operator<<( std::ostream &os, const C_Matrix &M)
	{
		os << "[";
		for( size_t i=0; i < M.rows; ++i )
		{
			for(size_t j=0; j < M.cols; ++j )
			{
				os << ' ' << M[i][j];
			}
			if(i<M.rows-1)
			{
				os << ';';
			}
		}
		os << "]";
		return os;
	}
    
    inline void save_ascii( const char *filename ) const
    {
        assert(filename);
        std::ofstream fp(filename);
        if(fp.is_open())
        {
            fp << rows << "\n";
            fp << cols << "\n";
            for( size_t i=0; i < rows; ++i )
            {
                for(size_t j=0; j < cols; ++j )
                {
                    fp << (*this)[i][j] << "\n";
                }
            }
            fp.close();
        }
        else throw Exception("couldn't open '%s'",filename);
    }
    
private:
	uint8_t *wksp;
	Row     *row;
    
	C_Matrix( const C_Matrix & );
	C_Matrix&operator=(const C_Matrix&);
    
	inline void build()
	{
        if( (rows>0&&cols==0) || (cols>0&&rows==0) )
            throw Exception("Invalid C_Matrix(%u,%u)", unsigned(rows), unsigned(cols) );
        if( rows> 0 )
        {
            const size_t bytes_for_rows = sizeof(Row) * rows;
            const size_t bytes_for_data = items * sizeof(T);
            wksp = new uint8_t[ bytes_for_rows + bytes_for_data ];
            row  = (Row *) &wksp[0];
            T *p = (T *)   &wksp[bytes_for_rows];
            for(size_t i=0; i < rows; ++i, p += cols )
                new (row+i) Row(p,cols);
        }
	}
    
};

#endif

