
#pragma once

#include "afxtempl.h"

template<class TYPE, class ARG_TYPE = TYPE& > class CEOSAIListSort : public CList<TYPE, ARG_TYPE> 
{
	public:
		CEOSAIListSort()
		{
			m_SortFunction = NULL;
		}
		void SetSortFunction( bool (*func)( TYPE, TYPE ) )
		{
			m_SortFunction = func;
		}

		void Remove( TYPE pObj )
		{
			POSITION pos = this->GetHeadPosition();
			while( pos )
			{
				POSITION prevPos = pos;
				if( this->GetNext( pos ) == pObj ){ this->RemoveAt( prevPos ); return; }
			}
		}

		void InsertSorted( TYPE pNewObj )
		{
			if( this->IsEmpty() ){ this->AddHead( pNewObj ); return; }
			// Look at the front and back
			if( (*m_SortFunction)( this->GetHead(), pNewObj ) == false ){ this->AddHead( pNewObj ); return; }
			if( (*m_SortFunction)( this->GetTail(), pNewObj ) ){ this->AddTail( pNewObj ); return; }
			// Try to drop it in somewhere (move from back to front)
			POSITION pos = this->GetTailPosition();
			while( pos )
			{
				POSITION prevPos = pos;
				TYPE pExistingObj = this->GetPrev( pos );
				if( (*m_SortFunction)( pExistingObj, pNewObj ) ){ this->InsertAfter( prevPos, pNewObj ); return; }
			}
			ASSERT( false );
		}
		void InsertSorted( TYPE pNewObj, bool (*func)( TYPE, TYPE ) )
		{
			if( this->IsEmpty() ){ this->AddHead( pNewObj ); return; }
			// Look at the front and back
			if( (*func)( this->GetHead(), pNewObj ) == false ){ this->AddHead( pNewObj ); return; }
			if( (*func)( this->GetTail(), pNewObj ) ){ this->AddTail( pNewObj ); return; }
			// Try to drop it in somewhere (move from back to front)
			POSITION pos = this->GetTailPosition();
			while( pos )
			{
				POSITION prevPos = pos;
				TYPE pExistingObj = this->GetPrev( pos );
				if( (*func)( pExistingObj, pNewObj ) ){ this->InsertAfter( prevPos, pNewObj ); return; }
			}
			bool b1 = (*func)( this->GetHead(), pNewObj );
			ASSERT( false );
			//
			pos = GetTailPosition();
			while( pos )
			{
				POSITION prevPos = pos;
				TYPE pExistingObj = this->GetPrev( pos );
				if( (*func)( pExistingObj, pNewObj ) )
				{
					this->InsertAfter( prevPos, pNewObj );
					return;
				}
			}
			bool b2 = (*func)( this->GetHead(), pNewObj );
			int z=0;
		}

		// This should only be called when [TYPE] is a pointer
		void  Sort( bool (*func)( TYPE, TYPE ) )
		{
			// Sort the list, use the (*func) as the GreaterThan/LessThan test
			// First: check if anything out of order.  If no, exit out.  If yes, do a one-step bubble-sort, then more
			TYPE pCurrObj = 0;
			TYPE pPrevObj = 0;
			POSITION prevPos;
			POSITION currPos;
			POSITION nextPos;
			long iItemsSwitched = 0;
			prevPos = currPos = nextPos = this->GetHeadPosition();
			if( prevPos )
			{
				TYPE pPrevObj = this->GetNext( currPos );
				if( currPos )
				{
					nextPos  = currPos;
					while( nextPos )
					{
						pCurrObj = this->GetNext( nextPos );

						if( pPrevObj && pCurrObj )
						{
							if( (*func)( pPrevObj, pCurrObj ) == false )
							{
								/*
								this->SetAt( currPos, pPrevObj );
								this->SetAt( prevPos, pCurrObj );

								TYPE pTemp = pPrevObj;
								pPrevObj = pCurrObj;
								pCurrObj = pTemp;
								*/
								iItemsSwitched++;
							}
						}
						pPrevObj  = pCurrObj;
						prevPos  = currPos;
						currPos  = nextPos;
					}
				}
			}
			if( iItemsSwitched == 0 ) return;

			// Do a large sort
			long iListSize = (long) GetCount();
			if( iListSize <= 1 ) return;
			long iStartingBlocks = 3;
			if(      iItemsSwitched*4  > iListSize ){ iStartingBlocks = 3;  }
			else if( iItemsSwitched*8  > iListSize ){ iStartingBlocks = 10; }
			else{                                     iStartingBlocks = 30; }
			long iMax = iListSize/5;
			TYPE pObj1 = 0;
			TYPE pObj2 = 0;
			for( long iBlocks=iStartingBlocks; iBlocks<iMax; iBlocks = (iBlocks+4) + iBlocks/2 )
			{
				long iStepSize = iListSize/iBlocks;

				POSITION pos1 = GetHeadPosition();
				POSITION pos2 = FindIndex( iStepSize );
				for( long iDirection=0; iDirection<2; iDirection++ )
				{
					while( pos1 && pos2 )
					{
						pObj1 = GetAt( pos1 );
						pObj2 = GetAt( pos2 );

						if( (*func)( pObj1, pObj2 ) == false )
						{
							this->SetAt( pos2, pObj1 );
							this->SetAt( pos1, pObj2 );
						}
						if( iDirection == 0 )
						{
							GetNext( pos1 );
							GetNext( pos2 );
						}
						else
						{
							GetPrev( pos1 );
							GetPrev( pos2 );
						}
					}
					if( iDirection == 0 )
					{
						pos2 = GetTailPosition();
						GetPrev( pos1 );
					}
				}
			}
			// Now do a bubble-sort for all the fine-sorting
			iItemsSwitched = 1;
			while( iItemsSwitched > 0 )
			{
				iItemsSwitched = 0;
				
				POSITION prevPos;
				POSITION currPos;
				POSITION nextPos;

				prevPos = currPos = nextPos = this->GetHeadPosition();
				if( prevPos )
				{
					TYPE pPrevObj = this->GetNext( currPos );
					if( currPos )
					{
						nextPos  = currPos;
						while( nextPos )
						{
							pCurrObj = this->GetNext( nextPos );

							if( pPrevObj && pCurrObj )
							{
								if( (*func)( pPrevObj, pCurrObj ) == false )
								{
									this->SetAt( currPos, pPrevObj );
									this->SetAt( prevPos, pCurrObj );

									TYPE pTemp = pPrevObj;
									pPrevObj = pCurrObj;
									pCurrObj = pTemp;

									iItemsSwitched++;
								}
							}

							pPrevObj  = pCurrObj;
							prevPos  = currPos;
							currPos  = nextPos;
						}
					}
				}
			}
			/*
			// Sort the list, use the (*func) as the GreaterThan/LessThan test
			//
			bool bOrderChanged = true;
			while( bOrderChanged )
			{
				bOrderChanged = false;

				TYPE pCurrObj = 0;

				POSITION prevPos;
				POSITION currPos;
				POSITION nextPos;

				prevPos = currPos = nextPos = this->GetHeadPosition();
				if( prevPos )
				{
					TYPE pPrevObj = this->GetNext( currPos );
					if( currPos )
					{
						nextPos  = currPos;
						while( nextPos )
						{
							pCurrObj = this->GetNext( nextPos );

							if( pPrevObj && pCurrObj )
							{
								if( (*func)( pPrevObj, pCurrObj ) == false )
								{
									this->SetAt( currPos, pPrevObj );
									this->SetAt( prevPos, pCurrObj );

									TYPE pTemp = pPrevObj;
									pPrevObj = pCurrObj;
									pCurrObj = pTemp;

									bOrderChanged = true;
								}
							}

							pPrevObj  = pCurrObj;
							prevPos  = currPos;
							currPos  = nextPos;
						}
					}
				}
			}
			*/
		}
		// This should only be called when [TYPE] is a pointer
		void  Sort()
		{
			// Sort the list, use the (*func) as the GreaterThan/LessThan test
			// First: check if anything out of order.  If no, exit out.  If yes, do a one-step bubble-sort, then more
			TYPE pCurrObj = 0;
			TYPE pPrevObj = 0;
			POSITION prevPos;
			POSITION currPos;
			POSITION nextPos;
			long iItemsSwitched = 0;
			prevPos = currPos = nextPos = this->GetHeadPosition();
			if( prevPos )
			{
				TYPE pPrevObj = this->GetNext( currPos );
				if( currPos )
				{
					nextPos  = currPos;
					while( nextPos )
					{
						pCurrObj = this->GetNext( nextPos );

						if( pPrevObj && pCurrObj )
						{
							if( (*m_SortFunction)( pPrevObj, pCurrObj ) == false )
							{
								/*
								this->SetAt( currPos, pPrevObj );
								this->SetAt( prevPos, pCurrObj );

								TYPE pTemp = pPrevObj;
								pPrevObj = pCurrObj;
								pCurrObj = pTemp;
								*/
								iItemsSwitched++;
							}
						}
						pPrevObj  = pCurrObj;
						prevPos  = currPos;
						currPos  = nextPos;
					}
				}
			}
			if( iItemsSwitched == 0 ) return;

			// Do a large sort
			long iListSize = (long) GetCount();
			if( iListSize <= 1 ) return;
			long iStartingBlocks = 3;
			if(      iItemsSwitched*4  > iListSize ){ iStartingBlocks = 3;  }
			else if( iItemsSwitched*8  > iListSize ){ iStartingBlocks = 10; }
			else{                                     iStartingBlocks = 30; }
			long iMax = iListSize/5;
			TYPE pObj1 = 0;
			TYPE pObj2 = 0;
			for( long iBlocks=iStartingBlocks; iBlocks<iMax; iBlocks = (iBlocks+4) + iBlocks/2 )
			{
				long iStepSize = iListSize/iBlocks;

				POSITION pos1 = GetHeadPosition();
				POSITION pos2 = FindIndex( iStepSize );
				for( long iDirection=0; iDirection<2; iDirection++ )
				{
					while( pos1 && pos2 )
					{
						pObj1 = GetAt( pos1 );
						pObj2 = GetAt( pos2 );

						if( (*m_SortFunction)( pObj1, pObj2 ) == false )
						{
							this->SetAt( pos2, pObj1 );
							this->SetAt( pos1, pObj2 );
						}
						if( iDirection == 0 )
						{
							GetNext( pos1 );
							GetNext( pos2 );
						}
						else
						{
							GetPrev( pos1 );
							GetPrev( pos2 );
						}
					}
					if( iDirection == 0 )
					{
						pos2 = GetTailPosition();
						GetPrev( pos1 );
					}
				}
			}
			// Now do a bubble-sort for all the fine-sorting
			iItemsSwitched = 1;
			while( iItemsSwitched > 0 )
			{
				iItemsSwitched = 0;
				
				POSITION prevPos;
				POSITION currPos;
				POSITION nextPos;

				prevPos = currPos = nextPos = this->GetHeadPosition();
				if( prevPos )
				{
					TYPE pPrevObj = this->GetNext( currPos );
					if( currPos )
					{
						nextPos  = currPos;
						while( nextPos )
						{
							pCurrObj = this->GetNext( nextPos );

							if( pPrevObj && pCurrObj )
							{
								if( (*m_SortFunction)( pPrevObj, pCurrObj ) == false )
								{
									this->SetAt( currPos, pPrevObj );
									this->SetAt( prevPos, pCurrObj );

									TYPE pTemp = pPrevObj;
									pPrevObj = pCurrObj;
									pCurrObj = pTemp;

									iItemsSwitched++;
								}
							}

							pPrevObj  = pCurrObj;
							prevPos  = currPos;
							currPos  = nextPos;
						}
					}
				}
			}
			/*
			// Sort the list, use the (*func) as the GreaterThan/LessThan test
			//
			bool bOrderChanged = true;
			while( bOrderChanged )
			{
				bOrderChanged = false;

				TYPE pCurrObj = 0;

				POSITION prevPos;
				POSITION currPos;
				POSITION nextPos;

				prevPos = currPos = nextPos = this->GetHeadPosition();
				if( prevPos )
				{
					TYPE pPrevObj = this->GetNext( currPos );
					if( currPos )
					{
						nextPos  = currPos;
						while( nextPos )
						{
							pCurrObj = this->GetNext( nextPos );

							if( pPrevObj && pCurrObj )
							{
								if( (*m_SortFunction)( pPrevObj, pCurrObj ) == false )
								{
									this->SetAt( currPos, pPrevObj );
									this->SetAt( prevPos, pCurrObj );

									TYPE pTemp = pPrevObj;
									pPrevObj = pCurrObj;
									pCurrObj = pTemp;

									bOrderChanged = true;
								}
							}

							pPrevObj  = pCurrObj;
							prevPos  = currPos;
							currPos  = nextPos;
						}
					}
				}
			}
			*/
		}

		bool (*m_SortFunction)( TYPE, TYPE );
};

