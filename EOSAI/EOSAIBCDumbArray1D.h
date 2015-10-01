
//#pragma once
#ifndef EOSAIDumbArray1D_h
#define EOSAIDumbArray1D_h

//
// The TYPE value determines the object that will be in the array.
//    e.g. CEOSAIBCArray< float > makes an array of floats (not float*, as some of my containers do).
//
// This container allocates space on the heap, so using it isn't all that quick.
//

template< class TYPE > class CEOSAIBCDumbArray1D
{
	public:
		CEOSAIBCDumbArray1D()
		{
			m_iSize = 0;
			m_pArray = NULL;
		}
		~CEOSAIBCDumbArray1D()
		{
			delete[] m_pArray;
		}

		void Reset()
		{
			delete[] m_pArray;
			m_iSize = 0;
			m_pArray = NULL;
		}
		void SetSize( long iNewSize )
		{
			if( m_iSize == iNewSize ) return;
			ASSERT( m_iSize == 0 );
			ASSERT( iNewSize > 0 );
			if( m_iSize != iNewSize ){ Reset(); }
			m_pArray = new TYPE[iNewSize];
			m_iSize = iNewSize;
		}

		//
		inline TYPE& operator[]( long index )
		{
			ASSERT( index >= 0 );
			ASSERT( index < m_iSize );
			//if( index <  0       ) return NULL;
			//if( index >= m_iSize ) return NULL;
			return (TYPE&)m_pArray[ index ];
		}
		inline TYPE& Value( long index )
		{
			ASSERT( index >= 0 );
			ASSERT( index < m_iSize );
			//if( index <  0       ) return NULL;
			//if( index >= m_iSize ) return NULL;
			return (TYPE&)m_pArray[ index ];
		}

		long    m_iSize;

	private:
		TYPE*   m_pArray;
};

#endif EOSAIDumbArray1D_h

