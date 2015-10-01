
#pragma once

// The Type class must be a pointer!

template< class TYPE > class CEOSAILimitedList
{
	public:
		CEOSAILimitedList()
		{
			m_iMaximumSize = 0;
			m_fMinimumValue = 0.0f;
			m_Array = 0;
			m_bIOwnTheObjects = false;
			ASSERT( sizeof( TYPE ) == 4 );
		}
		~CEOSAILimitedList()
		{
			if( m_bIOwnTheObjects )
			{
				for( long i=0; i<m_iMaximumSize; i++ )
				{
					if( m_Array[i] != 0 )
					{
						delete m_Array[i];
						m_Array[i] = 0;
					}
				}
			}
			free( m_Array );
			free( m_ValueArray );
		}

		// Initialization
		void  SetMaximumSize( long i )
		{
			m_iMaximumSize = i;
			ASSERT( m_Array == NULL );
			m_Array = (TYPE*) calloc( sizeof( TYPE ), m_iMaximumSize );
			m_ValueArray = (float*) calloc( sizeof( float ), m_iMaximumSize );
		}
		void  Clear()
		{
			for( long i=0; i<m_iMaximumSize; i++ )
			{
				m_Array[i] = 0;
				m_ValueArray[i] = 0.0f;
			}
		}
		long  GetMaximumSize(){ return m_iMaximumSize; }
		void  SetMinimumValue( float fVal ){ m_fMinimumValue = fVal; }
		void  IOwnTheObjects( bool b ){ m_bIOwnTheObjects = b; }

		//
		float GetTailValue()
		{
			float fTailValue = m_ValueArray[m_iMaximumSize-1];
			if( m_fMinimumValue > fTailValue )
			{
				return m_fMinimumValue;
			}
			else
			{
				return fTailValue;
			}
		}
		//
		bool  WillBeInserted( TYPE item, float fVal )
		{
			return ( fVal > GetTailValue );
		}
		//
		bool  Add( TYPE item, float fVal )
		{
			ASSERT( m_Array );

			// What if my value is too small to go into the list?
			if( m_fMinimumValue > fVal || 
			  //( m_Array[m_iMaximumSize-1] != 0 && m_ValueArray[m_iMaximumSize-1] > fVal ) )
			    m_ValueArray[m_iMaximumSize-1] > fVal )
			{
				if( m_bIOwnTheObjects )
				{
					delete item;
				}
				return false; // the value is too small to add to the list
			}
			bool bItemHasBeenInserted = false;
			for( long i=0; i<m_iMaximumSize; i++ )
			{
				if( m_Array[i] == 0 )
				{
					m_Array[i] = item;
					m_ValueArray[i] = fVal;
					return true;
				}
				else
				{
					bool bInsertThisValue = false;
					if( m_ValueArray[i] < fVal )
					{
						// delete the end object? (if it exists)
						if( m_Array[m_iMaximumSize-1] &&
							m_bIOwnTheObjects )
						{
							delete m_Array[m_iMaximumSize-1];
							m_Array[m_iMaximumSize-1] = 0;
						}

						// shift everyone else down
						for( long j=m_iMaximumSize-1; j>i; j-- )
						{
							m_Array[j] = m_Array[j-1];
							m_ValueArray[j] = m_ValueArray[j-1];
						}
						m_Array[i] = item;
						m_ValueArray[i] = fVal;
						return true;
					}
				}
			}
			return false;
		}
		//
		void  Remove( TYPE item )
		{
			ASSERT( m_Array );
			for( long i=0; i<m_iMaximumSize; i++ )
			{
				if( m_Array[i] == item )
				{
					m_Array[i] = 0;
					m_ValueArray[i] = 0.0f;

					// shift everyone else up
					for( long j=i; j<m_iMaximumSize-1; j++ )
					{
						m_Array[j] = m_Array[j+1];
						m_ValueArray[j] = m_ValueArray[j+1];
					}
					m_Array[m_iMaximumSize-1] = 0;
					m_ValueArray[m_iMaximumSize-1] = 0.0f;
				}
			}
		}
		//
		bool   m_bIOwnTheObjects;
		long   m_iMaximumSize;
		float  m_fMinimumValue;
		TYPE*  m_Array;
		float* m_ValueArray;
};
