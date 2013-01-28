#ifndef PYCK_C_MATRIX_INCLUDED
#define PYCK_C_MATRIX_INCLUDED 1

#include "./os.hpp"
#include <new>
#include <iostream>

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

	inline ~C_Matrix() throw()
	{
		delete []wksp;
		wksp = 0;
		row  = 0;

	}

	inline Row &       operator[](size_t r) throw()       { assert(r<rows); return row[r]; }
	inline const Row & operator[](size_t r) const throw() { assert(r<rows); return row[r]; }

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

private:
	uint8_t *wksp;
	Row     *row;

	C_Matrix( const C_Matrix & );
	C_Matrix&operator=(const C_Matrix&);

	inline void build()
	{
		const size_t bytes_for_rows = sizeof(Row) * rows;
		const size_t bytes_for_data = items * sizeof(T);
		wksp = new uint8_t[ bytes_for_rows + bytes_for_data ];
		row  = (Row *) &wksp[0];
		T *p = (T *)   &wksp[bytes_for_rows];
		for(size_t i=0; i < rows; ++i, p += cols ) 
			new (row+i) Row(p,cols);
	}

};

#endif

