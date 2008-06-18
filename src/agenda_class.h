/* Copyright (C) 2000-2008 Stefan Buehler <sbuehler@ltu.se>

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

/*!
  \file   agenda_class.h
  \author Stefan Buehler <sbuehler@ltu.se>
  \date   Thu Mar 14 08:49:33 2002
  
  \brief  Declarations for agendas.
*/

#ifndef agenda_class_h
#define agenda_class_h

#include <set>
#include "token.h"

class MRecord;

class Workspace;

//! The Agenda class.
/*! An agenda is a list of workspace methods (including keyword data)
  to be executed. There are workspace variables of class agenda that
  can contain a list of methods to execute for a particular purpose,
  for example to compute the lineshape in an absorption
  calculation. 
*/
class Agenda {
public:

  Agenda() : agendaworkspace(NULL)
    {
    }

  Agenda(const Agenda& x);

  void append(const String& methodname, const String& keywordvalue);
  void push_back(MRecord n);
  void execute(bool silent=false) const;
  inline void resize(Index n);
  inline Index nelem() const;
  inline Agenda& operator=(const Agenda& x);
  const Array<MRecord>& Methods () const { return mml; }
  Workspace* workspace () { return agendaworkspace; }
  void set_workspace (Workspace* ws) { agendaworkspace = ws; }
  void set_outputs_to_push_and_dup ();
  bool is_input(Index var) const;
  bool is_output(Index var) const;
  void set_name(const String& nname);
  String name() const;
  const ArrayOfIndex& get_output2push() const { return moutput_push; }
  const ArrayOfIndex& get_output2dup() const { return moutput_dup; }
  void print( ostream& os,
              const String& indent ) const;
private:
  String         mname; /*!< Agenda name. */
  Array<MRecord> mml;   /*!< The actual list of methods to execute. */

  ArrayOfIndex moutput_push;

  ArrayOfIndex moutput_dup;
  Workspace* agendaworkspace;
};

// Documentation with implementation.
ostream& operator<<(ostream& os, const Agenda& a);


/** Method runtime data. In contrast to MdRecord, an object of this
    class contains the runtime information for one method: The method
    id and the keyword parameter values. This is all that the engine
    needs to execute the stack of methods.

    An MRecord includes a member magenda, which can contain an entire
    agenda, i.e., a list of other MRecords. 

    @author Stefan Buehler */
class MRecord {
public:
  MRecord(){ /* Nothing to do here. */ }
  MRecord(const MRecord& x) : mid(x.mid),
                              mvalues(x.mvalues),
                              moutput(x.moutput),
                              minput(x.minput),
                              mtasks(x.mtasks)
    {
    }

  MRecord(const Index id,
          const Array<TokVal>& values,
          const ArrayOfIndex& output,
          const ArrayOfIndex& input,
          const Agenda&       tasks)
    : mid(id),
      mvalues( values ),
      moutput( output ),
      minput(  input  ),
      mtasks( tasks )
  { 
    // Initialization of arrays from other array now works correctly.
  }
  Index                Id()       const { return mid;     }
  const Array<TokVal>& Values()   const { return mvalues; }
  const ArrayOfIndex&  Output()   const { return moutput; }
  const ArrayOfIndex&  Input()    const { return minput;  }
  const Agenda&        Tasks()    const { return mtasks;  }

  //! Assignment operator for MRecord.
  /*! 
    This is necessary, because it is used implicitly if agendas (which
    contain an array of MRecord) are copied. The default assignment
    operator generated by the compiler does not do the right thing!

    This became clear due to a bug when agendas were re-defined in the
    controlfile, which was discoverd by Patrick.

    The problem is that MRecord contains some arrays. The copy semantics
    for Array require the target Array to have the right size. But if we
    overwrite an old MRecord with a new one, we want all arrays to be
    overwritten. We don't care about their old size.

    \param x The other MRecord to assign.

    \return The freshly assigned MRecord.

    \author Stefan Buehler
    \date   2002-12-02
    */
  MRecord& operator=(const MRecord& x)
    {
      mid = x.mid;

      mvalues.resize(x.mvalues.nelem());
      mvalues = x.mvalues;

      moutput.resize(x.moutput.nelem());
      moutput = x.moutput;

      minput.resize(x.minput.nelem());
      minput = x.minput;

      mtasks.resize(x.mtasks.nelem());
      mtasks = x.mtasks;

      return *this;
    }

  //! Get list of generic input only WSVs.
  /*!
    This function returns an array with the indexes of WSVs which are
    only input variables but not output.

    \param[out] ginonly Index array of input only WSVs.

    \author Oliver Lemke
    \date   2008-02-27
  */
  void ginput_only (ArrayOfIndex& ginonly) const
    {
      ginonly = minput;    // Input
      for (ArrayOfIndex::const_iterator j=moutput.begin(); j<moutput.end(); ++j)
        for (ArrayOfIndex::iterator k=ginonly.begin(); k<ginonly.end(); ++k)
          if ( *j == *k )
            {
              //              erase_vector_element(vi,k);
              k = ginonly.erase(k) - 1;
              // We need the -1 here, otherwise due to the
              // following increment we would miss the element
              // behind the erased one, which is now at the
              // position of the erased one.
            }
    }

  // Output operator:
  void                 print( ostream& os,
                              const String& indent ) const;

private:
  /** Method id. */
  Index mid;
  /** List of parameter values (see methods.h for definition of
      TokVal). */
  Array<TokVal> mvalues;
  /** Output workspace variables. */
  ArrayOfIndex moutput;
  /** Input workspace variables. */
  ArrayOfIndex minput;
  /** An agenda, which can be given in the controlfile instead of
      keywords. */
  Agenda mtasks;
};

//! Resize the method list.
/*!
  Resizes the agenda's method list to n elements
 */
inline void Agenda::resize(Index n)
{
  mml.resize(n);
}

//! Return the number of agenda elements.
/*!  
  This is needed, so that we can find out the correct size for
  resize, befor we do a copy.

  \return Number of agenda elements.
*/
inline Index Agenda::nelem() const
{
  return mml.nelem();
}


//! Append a new method to end of list.
/*! 
  This is used by the parser to fill up the agenda.

  \param n New method to add.
*/
inline void Agenda::push_back(MRecord n)
{
  mml.push_back(n);
}


//! Copy constructor.
/*! 
  Copies an agenda.
*/
inline Agenda::Agenda(const Agenda& x)
{
  mml = x.mml;
  mname = x.mname;
  agendaworkspace = x.agendaworkspace;
  moutput_push = x.moutput_push;
  moutput_dup = x.moutput_dup;
}

//! Assignment operator.
/*! 
  Copies an agenda.
*/
inline Agenda& Agenda::operator=(const Agenda& x)
{
  mml = x.mml;
  mname = x.mname;
  agendaworkspace = x.agendaworkspace;
  moutput_push = x.moutput_push;
  moutput_dup = x.moutput_dup;
  return *this;
}

// Documentation is with implementation.
ostream& operator<<(ostream& os, const MRecord& a);

#endif
