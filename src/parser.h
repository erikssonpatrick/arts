/* Header file for functions and classes related to parsing the
   control text. */ 

#ifndef parser_h
#define parser_h

#include "vecmat.h"
#include "token.h"

/** Method runtime data. In contrast to MdRecord, an object of this
    class contains the runtime information for one method: The method
    id and the keyword parameter values. This is all that the engine
    needs to execute the stack of methods.

    Elements of this class are used both to hold the true runtime
    information and the example data.

    @author Stefan Buehler */
class MRecord {
public:
  MRecord(const int id,
	  const ARRAY<TokVal>& values,
	  const ARRAY<size_t>& output,
	  const ARRAY<size_t>& input)
    : mid(id),
      mvalues(values),
      moutput(output),
      minput(input) { /* Nothing to do here. */ }
  int                  Id()     const { return mid;     }
  const ARRAY<TokVal>& Values() const { return mvalues; }
  const ARRAY<size_t>& Output() const { return moutput; }
  const ARRAY<size_t>& Input()  const { return minput; }

private:
  /** Method id. */
  int mid;
  /** List of parameter values (see methods.h for definition of
      TokVal). */
  ARRAY<TokVal> mvalues;
  /** Output workspace variables (for generic methods). */
  ARRAY<size_t> moutput;
  /** Input workspace variables (for generic methods). */
  ARRAY<size_t> minput;
};


/** A smart class to hold the text for parsing. A variable of this
    class can hold not only the text of the ARTS Control file, but
    also a position in the text. This is handy for parsing. There is
    also a function to return the current character and functions to
    advance the position. (AdvanceChar advances the position to the
    next character, doing a line break if necessary; AdvanceLine goes
    to the next line.)
    
    mLine and mColumn are 0 based, but Line and Column are 1 based.
    
    @author Stefan Buehler */
class SourceText {
public:

  /** Default constructor. */
  SourceText() :  mLine(0), mColumn(0) { /* Nothing to be done here. */ };

  /** Appends contents of file to the source text.
      @see read_text_from_file */
  void AppendFile(const string& name);

  /** Return the current character. */
  char Current() {
    return mText[mLine][mColumn];
  }

  /** Advance position pointer by one character. Sets mLineBreak if a
      line break occured.  
   
      @exception Eot The end of text is reached. */
  void AdvanceChar();

  /** Advances position pointer by one line.

      @exception Eot The end of the text is reached. */
  void AdvanceLine();

  /** Return the filename associated with the current position. */
  const string& File();

  /** Return the line number, but for the file that is associated
      with the current position. */
  int Line();

  /** Return the current column. */
  int Column() { return mColumn+1; }

  /** This sets the pointer to the first existing character in the
      text. (First few lines could be empty). */
  void Init();

  /** Read the line break flag. Set this to false before an operation
      that you want to monitor and check it afterwards. */
  bool& LineBreak() { return mLineBreak; }

  /** Const version of LineBreak
      @see LineBreak */
  bool  LineBreak() const { return mLineBreak; }

  /** Output operator for SourceText. (Only used for debugging) */
  friend ostream& operator << (ostream& os, const SourceText& text);

private:

  /** The text. */
  ARRAY<string> mText;

  /** Line position in the text. (0 based!) */
  size_t mLine;

  /** Column position in the text. (0 based!) */
  size_t mColumn;

  /** Remember where which source file starts. */
  vector<size_t> mSfLine;

  /** Names associated with @see mSfLine. */
  vector<string> mSfName;

  /** Is set to true if the last operation caused a line
      break. Has to be cleared explicitly! */
  bool mLineBreak;
};

/** The main function of the parser. This will parse the entire
    text. FIXME: Add more documentation here.

    @param tasklist Output. The method runtime data that can be used to
                    execute the methods.
    @param text The control text

    @author Stefan Buehler */
void parse_main(ARRAY<MRecord>& tasklist, SourceText& text);


#endif
