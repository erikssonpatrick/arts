/* Copyright (C) 2000 Stefan Buehler <sbuehler@uni-bremen.de>
                      Patrick Eriksson <patrick@rss.chalmers.se>

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */



////////////////////////////////////////////////////////////////////////////
//   File description
////////////////////////////////////////////////////////////////////////////
/**
  \file   vecmat.h
  
  This contains now MTL based matrix and vector classes. 

  \date   2001-01-08
  \author Stefan Buehler
*/



#ifndef vecmat_h
#define vecmat_h

//--------------------< Global ARTS header >--------------------
// Defines for example the type Numeric.
#include "arts.h"

//--------------------< STL headers: >--------------------
#include <vector>		// Vektor class from the STL

//--------------------< MTL headers: >--------------------
#include "mtl/mtl.h"
#include "mtl/utils.h"
#include "mtl/matrix.h"
#include "mtl/linalg_vec.h"

//using namespace mtl;
//using namespace std;

// typedef mtl::matrix< Numeric, 
//   rectangle<>, 
//   dense<>, 
//   row_major>::type Matrix; 

typedef mtl::dense1D<Numeric> Vector;

typedef mtl::matrix<Numeric>::type Matrix;

typedef mtl::matrix<Numeric,mtl::rectangle<>,mtl::compressed<> >::type SPARSE;

typedef mtl::matrix<Numeric,mtl::symmetric<mtl::upper>,mtl::banded<> >::type SYMMETRIC;


//#define Array mtl::dense1D       
#define Array std::vector


/** Output function for vectors (for example Vector and Array).

    This function is taken initially from MTL, with the main difference
    that it can print to any output stream, not just to stdout. Also
    the format of the generated output has been changed.

    \param os The output stream.
    \param x  The vector to print.

    \author Stefan Buehler
    \date   2000-12-06
 **/
template <class OS, class Vector>
inline void
print_vector(OS& os, Vector x)
{
  using namespace mtl;
  typename Vector::iterator i = x.begin();

  if (mtl::not_at(i, x.end()))
    {
      os << *i;
      ++i;
    }
  while (mtl::not_at(i, x.end()))
    {
      os << " " << *i;
      ++i;
    }
}

/** Output function for matrices.

    This function is taken initially from MTL, with the main difference
    that it can print to any output stream, not just to stdout. Also
    the format of the generated output has been changed.

    \param os The output stream.
    \param A  The matrix to print.

    \author Stefan Buehler
    \date   2000-12-06
 **/
template <class OS, class Matrix>
inline void
print_all_matrix(OS& os, const Matrix& A)
{
  using namespace mtl;
  typedef typename matrix_traits<Matrix>::size_type Int;
  Int i,j;

  // We have to use the (,) form for indexing here, since [][] does
  // not work with the trans() adapter!

  for (i=0; i < A.nrows(); ++i)
    {
      if (0 < A.ncols())
	os << A(i,0);
      for (j=1; j < A.ncols(); ++j)
	os << " " << A(i,j);
      if (i != A.nrows()-1 )
	os << "\n";
    }
}


/** Input function for vectors (for example Vector and Array).

    This functions reads an MTL vector from an input stream. The format is
    very simple, first there should be an integer indicating the
    length of the vector, then the elements. Thus, it will work for
    arbitrary element types, providing their input operator is
    overloaded correctly. 

    \param is The input stream.
    \retval x  The vector to read.

    \author Stefan Buehler
    \date   2000-12-08
 **/
template <class Vector>
inline void
read_vector_from_stream(Vector& x, istream& is)
{
  Index n;
  is >> n;
  //  cout << "n: " << n << "\n";
  resize(x,n);
  for(Index i=0; i<n; ++i)
    {
      //      cout << "- " << i << "\n";
      is >> x[i];
    }
}

/** Input function for matrices.

    This functions reads an MTL matrix from an input stream. The format is
    very simple, first there should be two integers indicating the
    dimensions of the matrix, then the elements. Thus, it will work for
    arbitrary element types, providing their input operator is
    overloaded correctly. 

    Should also work on sparse matrices, although this should be not
    usefull bacause all elements of the sparse matrix will be set.

    \param is The input stream.
    \retval A  The matrix to read.

    \author Stefan Buehler
    \date   2000-12-23
 **/
template <class M>
inline void
read_matrix_from_stream(M& A, istream& is)
{
  Index nr,nc;
  is >> nr >> nc;
//   cout << "nr: " << nr << "\n";
//   cout << "nc: " << nc << "\n";
  resize(A,nr,nc);
  for(Index ir=0; ir<nr; ++ir)
    for(Index ic=0; ic<nc; ++ic)
    {
      //      cout << "- " << i << "\n";
      is >> A[ir][ic];
    }
}


// Output and Input operators are not as general as the algorithms
// defined above. Nevertheless, they are needed for the most common
// cases. Otherwise, we would for example have to explicitly program
// all input cases for ArrayS of MATRICES or ArrayS of ArrayS.

// Output operator for Array:
template <class T>
std::ostream& operator<<(std::ostream& s, const Array<T>& A)
{
  print_vector(s, A);
  return s;
}

// Output operator for Vector:
std::ostream& operator<<(std::ostream& s, const Vector& A);

// Output operator for Vector subrange:
std::ostream& operator<<(std::ostream& s, const Vector::subrange_type& A);

// Output operator for Matrix:
std::ostream& operator<<(std::ostream& s, const Matrix& A);

// Output operator for sub Matrix:
std::ostream& operator<<(std::ostream& s, const Matrix::submatrix_type& A);

// Output operator for SPARSE:
std::ostream& operator<<(std::ostream& s, const SPARSE& A);

// Output operator for SYMMETRIC:
std::ostream& operator<<(std::ostream& s, const SYMMETRIC& A);


// Input operator for Array:
template <class T>
std::istream& operator>>(std::istream& s, Array<T>& A)
{
  read_vector_from_stream(A,s);
  return s;
}

// Input operator for Vector:
std::istream& operator>>(std::istream& s, Vector& A);

// Input operator for Matrix:
std::istream& operator>>(std::istream& s, Matrix& A);


/** Erase an element of a vector (for example Vector and Array). This
    functionality is missing in MTL vectors, but we need in some
    places. 


    \retval x  The vector to treat.
    \param k   Index of the element to erase.

    \author Stefan Buehler
    \date   2000-12-08
 **/
template <class Vector>
inline void
erase_vector_element(Vector& x, Index k)
{
  Vector y(x.size()-1);
  copy(x(0,k),y(0,k));
  copy(x(k+1,x.size()),y(k,x.size()-1));
  x = y;
}



// Implement some MTL functionality for STL based Array.
// -----------------------------------------------------

// Copy algorithm for Arrays:

template<class T>
inline void 
copy(const Array<T>& a, Array<T>& b)
{
  assert( a.size()==b.size() );
  std::copy( a.begin(), a.end(), b.begin() );
}

// Set all element to the given value. It would have been nicer to
// call this set, like in MTL, but there is a name conflict with
// STD:set. This is actually also a problem for the MTL::set, so this
// name is better.

template<class T>
inline void 
setto(Array<T>& a, const T& x)
{
  std::fill( a.begin(), a.end(), x );
}

// Not general, therefor not so useful.
//
// template<class T>
// inline void 
// addto(Array<T>& a, const T& x)
// {
//   // FIXME: Can this be made more efficient?
//   Array<T> dummy( a.size(), x );
//   mtl::add( dummy, a );
// }

// Same for MTL based types for consistency

inline void 
setto(Vector& a, Numeric x)
{
  mtl::set(a,x);
}

inline void 
setto(Matrix& A, Numeric x)
{
  mtl::set(A,x);
}

inline void 
setto(SYMMETRIC& A, Numeric x)
{
  mtl::set(A,x);
}


inline void 
setto(SPARSE& A, Numeric x)
{
  mtl::set(A,x);
}

// Not general, therefor not so useful.
//
// inline void 
// addto(Vector& a, Numeric x)
// {
//   // FIXME: Can this be made more efficient?
//   Vector dummy( a.size(), x );
//   mtl::add( dummy, a );
// }

// inline void 
// addto(Matrix& a, Numeric x)
// {
//   // FIXME: Can this be made more efficient?
//   Matrix dummy( a.nrows(), a.ncols() );
//   setto( dummy, x );
//   mtl::add( dummy, a );
// }

// Max and min functions:

template<class T>
inline T 
max(const Array<T>& a)
{
  return *std::max_element( a.begin(), a.end() );
}

template<class T>
inline T 
min(const Array<T>& a)
{
  return *std::min_element( a.begin(), a.end() );
}


// Resize functions for Array, Vector, Matrix, SPARSE and SYMMETRIC

template <class T>
void resize(Array<T>& x, Index n)
{
  //  x.resize(n);   DANGEROUS!

  // We have to do it like that here, otherwise we get problems with
  // the initialization of Arrays of Arrays. This would be inefficient
  // for STL based Arrays!
  x = Array<T>(n);
}

void resize(Vector& x, 	  Index n);
void resize(string& x,    Index n);

void resize(Matrix& x, 	  Index r, Index c);
void resize(SPARSE& x, 	  Index r, Index c);
void resize(SYMMETRIC& x, Index r, Index c);


// Like-like operations (Matrix-Matrix or Vector-Vector). These
// templates should work for both cases!

// Element-vise multiplication and division. 

// template <class T>
// T emult(const T& a, const T& b)
// {
//   assert( a.size()==b.size() );
//   T c(a.size());
//   ele_mult(a,b,c);
//   return c;
// }

// template <class T>
// T ediv(const T& a, const T& b)
// {
//   assert( a.size()==b.size() );
//   T c(a.size());
//   ele_div(a,b,c);
//   return c;
// }

// // Plus and minus operator.

// template <class T>
// T operator+(const T& a, const T& b)
// {
//   assert( a.size()==b.size() );
//   T c(a.size());
//   copy(a,c);
//   add(b,c);
//   return c;
// }

// template <class T>
// T operator-(const T& a, const T& b)
// {
//   assert( a.size()==b.size() );
//   T c(a.size());
//   copy(a,c);
//   add(scaled(b,-1),c);
//   return c;
// }

// // * and / operator.

// template <class T>
// T operator*(const T& a, const T& b)
// {
//   assert( a.size()==b.size() );
//   T c(a.size());
//   mult(a,b,c);
//   return c;
// }

// template <class T>
// T operator-(const T& a, const T& b)
// {
//   assert( a.size()==b.size() );
//   T c(a.size());
//   copy(a,c);
//   add(scaled(b,-1),c);
//   return c;
// }

void transf( const Vector& x,
	     double (&my_func)(double),
	     Vector& y );

Vector transf( const Vector& x,
	       double (&my_func)(double) );

void transf( const Matrix& x,
	     double (&my_func)(double),
	     Matrix& y );

Matrix transf( const Matrix& x,
	       double (&my_func)(double) );



////////////////////////////////////////////////////////////////////////////
//   Definition of arrays of matrices and vectors
////////////////////////////////////////////////////////////////////////////

/** An array of matrices. */
typedef Array<Matrix> ArrayofMatrix;

/** An array of vectors. */
typedef Array<Vector> ArrayofVector;

/** An array of integers. */
typedef Array<size_t> Arrayofsizet;

/** An array of strings. */
typedef Array<string> Arrayofstring;




#endif // vecmat_h
