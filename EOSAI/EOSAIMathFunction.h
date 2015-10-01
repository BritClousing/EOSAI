
#pragma once

/*
	The AIFunction class is used to return the value f( x ) given some x value.
	It's primary use is in assigning a value depending on the number of units allocated
		to a task.

	Note: The value is noncumulative.  If f( 1 ) = 1, and f( 2 ) = 1, then the second
		value is worth nothing.

	Examples:
	
	Number of Combat Units sent to invade an island (which is occupied):

           |Good              *  *
      Value|Fair           *
           |Bad   *  *  *
                  1  2  3  4  5  6
                  Military Strength

	Number of Combat Units sent to invade an island (which is unoccupied):

           |Good            
      Value|Fair  *  *  *  *  *  *
           |Bad          
                  1  2  3  4  5  6
                  CitRes Hunters
*/

//#include "ListOwned.h"

class CEOSAIFunctionPair
{
	public:
		CEOSAIFunctionPair( float fInput, float fOutput ){ m_fInput = fInput; m_fOutput = fOutput; }

		float m_fInput;
		float m_fOutput;
};

class CEOSAIMathFunction
{
	public:
		CEOSAIMathFunction(){}
		~CEOSAIMathFunction(){ while( m_FunctionPairsList.IsEmpty() == FALSE ){ delete m_FunctionPairsList.RemoveHead(); } }

		void   Clear();//{ while( m_FunctionPairsList.DeleteAll(); }
		void   SetInputOutput( float fInput, float fOutput );
		float  GetOutput( float fInput );

	private:
		CList< CEOSAIFunctionPair* >  m_FunctionPairsList;
};

