
//#pragma once
#ifndef EOSAIDumbArray2D_h
#define EOSAIDumbArray2D_h

//
// The TYPE value determines the object that will be in the array.
//    e.g. CEOSAIBCArray< float > makes an array of floats (not float*, as some of my containers do).
//
// This container allocates space on the heap, so using it isn't all that quick.
//

template< class TYPE > class CEOSAIBCDumbArray2D
{
	public:
		CEOSAIBCDumbArray2D()
		{
			m_iSizeX = 0;
			m_iSizeY = 0;
			m_pArray = NULL;
		}
		~CEOSAIBCDumbArray2D()
		{
			if( m_pArray ){ delete[] m_pArray; }
		}

		void SetSize( long iSizeX, long iSizeY )
		{
			if( m_iSizeX == iSizeX && m_iSizeY == iSizeY ) return;
			//ASSERT( m_iSizeX == 0 );
			//ASSERT( m_iSizeY == 0 );
			ASSERT( iSizeX > 0 );
			ASSERT( iSizeY > 0 );
			if( m_pArray ){ delete[] m_pArray; m_pArray = NULL; }
			m_pArray = (TYPE*) new TYPE[ iSizeX * iSizeY ];
			m_iSizeX = iSizeX;
			m_iSizeY = iSizeY;
		}
		void operator=( CEOSAIBCDumbArray2D& Other )
		{
			SetSize( Other.m_iSizeX, Other.m_iSizeY );
			for( long x=0; x<m_iSizeX; x++ )
			{
				for( long y=0; y<m_iSizeY; y++ ){ Value( x,y ) = Other.Value( x,y ); }
			}
		}

		//
		TYPE& Value( long iIndexX, long iIndexY )
		{
			ASSERT( iIndexX < m_iSizeX );
			ASSERT( iIndexY < m_iSizeY );
			return (TYPE&)m_pArray[ iIndexX + m_iSizeX*iIndexY ];
		}

		long    m_iSizeX;
		long    m_iSizeY;

	private:
		TYPE*  m_pArray;
};

#endif EOSAIDumbArray2D_h
