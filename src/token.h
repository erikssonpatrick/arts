/* Copyright (C) 2000 Stefan Buehler <sbuehler@uni-bremen.de>

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

#ifndef token_h
#define token_h

#include "vecmat.h"

/** The different token value types. These are the types that keyword
    parameters in the controlfile can have. */
enum TokValType { string_t,    int_t,    Numeric_t,
	          Array_string_t, Array_int_t, Vector_t,
                  undefined_t };

/** This stores arbitrary token values and remembers the type. Only
    the correct type can be extracted again. */
class TokVal {
public:

  /** Default Constructor. (Sets type to undefined_t) */
  TokVal() {
    mtype = undefined_t;
  }

  /** To set TokVal from string (C - style). */
  TokVal(const char s[]) {
    mtype = string_t;
    ms = s;
  }

  /** To set TokVal from string (C++ - style). */
  TokVal(const string& s) {
    mtype = string_t;
    ms = s;
  }

  /** To set TokVal from an integer. */
  TokVal(int n) {
    mtype = int_t;
    mn = n;
  }

  /** To set TokVal from a Numeric. */
  TokVal(Numeric x) {
    mtype = Numeric_t;
    mx = x;
  }

  /** To set TokVal from an array of strings. */
  TokVal(Array<string> sv) : msv(sv.size())
  {
    mtype = Array_string_t;
    copy(sv, msv);
  }

  /** To set TokVal from an array of integers. */
  TokVal(Array<int> nv) : mnv(nv.size())
  {
    mtype = Array_int_t;
    copy(nv, mnv);
  }

  /** To set TokVal from a Vector. */
  TokVal(Vector xv) : mxv(xv.size())
  {
    mtype = Vector_t;
    copy(xv, mxv);
  }

  // Conversion functions to return TokVal for the 6 different types: 
  
  /** Return string. */
  operator string() const;
  /** Return int. */
  operator int() const;
  /** Return Numeric. */
  operator Numeric() const;

  /** Return array of strings. */
  operator Array<string>() const;
  /** Return array of integers. */
  operator Array<int>() const;
  /** Return Vector. */
  operator Vector() const;

  /** Output operator. */
  friend ostream& operator<<(ostream& os, const TokVal& a);

private:
  TokValType mtype;
  string       ms;
  int          mn;
  Numeric      mx;   
  Array<string>  msv;
  Array<int>     mnv;
  Vector         mxv;
};


// typedef Array<TokValType> TokValTypeVector;
// typedef Array<TokVal>     TokValVector;



#endif  // token_h
