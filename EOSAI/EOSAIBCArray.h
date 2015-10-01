
#pragma once

//
// The TYPE value determines the object that will be in the array.
//    e.g. CEOSAIBCArray< float > makes an array of floats (not float*, as some of my containers do).
//

#define BCARRAY_STATIC_SIZE 1000

template<class TYPE> class CEOSAIBCArray
{
	public:
		CEOSAIBCArray()
		{
			m_iArraySize = BCARRAY_STATIC_SIZE;
			m_iNewBlockSize = 1000;
			m_pArray = NULL;
			Clear();
		}
		~CEOSAIBCArray()
		{
			free( m_pArray );
		}

		void SetNewBlockSize( long i )
		{
			ASSERT( false );
			ASSERT( i > 0 ); 
			m_iNewBlockSize = i;
		}

		void Clear()
		{
			if( m_pArray )
			{
				memset( m_pArray, 0, sizeof( TYPE )*m_iArraySize );
			}
			memset( m_StaticArray, 0, sizeof( TYPE )*BCARRAY_STATIC_SIZE );
		}
		//
		/*
		inline TYPE& operator[]( long index )
		{
			ASSERT( index < m_iArraySize );
			return (TYPE&)(m_pArray[ index ]);
		}
		*/
		long GetLargestValidIndex()
		{
			long iMax = 0;
			if( m_pArray )
			{
				for( long i=0; i<m_iArraySize; i++ )
				{
					if( m_pArray[ i ] ){ iMax = i; }
				}
			}
			else
			{
				for( long i=0; i<m_iArraySize; i++ )
				{
					if( m_StaticArray[ i ] ){ iMax = i; }
				}
			}
			return iMax;
		}
		inline TYPE  Get( long index )
		{
			ASSERT( index < m_iArraySize );
			if( m_pArray )
			{
				return m_pArray[ index ];
			}
			return m_StaticArray[ index ];
		}
		//
		inline void  Set( long index, TYPE obj )
		{
			// resize it?
			if( index >= m_iArraySize )
			{
				long iNewSize = max( index+1, m_iArraySize + m_iNewBlockSize );
				if( m_pArray )
				{
					m_pArray = ( TYPE* ) realloc( m_pArray, sizeof( TYPE )*iNewSize );
				}
				else
				{
					m_pArray = ( TYPE* ) realloc( NULL, sizeof( TYPE )*iNewSize );
					memmove( m_pArray, m_StaticArray, sizeof( TYPE )*BCARRAY_STATIC_SIZE );
				}
				m_iArraySize = iNewSize;
				ASSERT( m_pArray );
			}
			if( m_pArray )
			{
				m_pArray[ index ] = obj;
			}
			else
			{
				m_StaticArray[ index ] = obj;
			}
		}

		long    m_iArraySize;
		long    m_iNewBlockSize;
		TYPE*   m_pArray;  // pointer to the m_pArray
		TYPE    m_StaticArray[BCARRAY_STATIC_SIZE];  // pointer to the m_pArray
};

