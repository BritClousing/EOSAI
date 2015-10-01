
#pragma once

class CEOSAITransportAssistedPath
{
	public:
		CEOSAITransportAssistedPath()
		{
			Clear();
		}
		//
		void Clear()
		{
			ClearMultiRegionData();
			ClearAIRegionData();
		}
		void ClearMultiRegionData()
		{
			m_GroundUnitToPickupMultiRegionPath.RemoveAll();
			m_TransportToPickupMultiRegionPath.RemoveAll();
			m_TransportPickupToDropoffMultiRegionPath.RemoveAll();
			m_GroundUnitFromDropoffToTargetMultiRegionPath.RemoveAll();
			m_iLandPickupMultiRegion = 0;
			m_iWaterPickupMultiRegion = 0;
			m_iLandDropoffMultiRegion = 0;
			m_iWaterDropoffMultiRegion = 0;
		}
		void ClearAIRegionData()
		{
			m_GroundUnitToPickupAIRegionPath.RemoveAll();
			m_TransportToPickupAIRegionPath.RemoveAll();
			m_TransportPickupToDropoffAIRegionPath.RemoveAll();
			m_GroundUnitFromDropoffToTargetAIRegionPath.RemoveAll();
			m_iLandPickupAIRegion = 0;
			m_iWaterPickupAIRegion = 0;
			m_iLandDropoffAIRegion = 0;
			m_iWaterDropoffAIRegion = 0;
		}

		//void  SetTransport( CEOSAIUnit2* pTransport );
		//void  AddTransportee( CEOSAIUnit2* pTransportee, CEOSAILocation StartLocation, CEOSAILocation EndLocation );

		//
		// Results
		//
			// The pathway of MultiRegions
			CList< long >  m_GroundUnitToPickupMultiRegionPath;
			CList< long >  m_TransportToPickupMultiRegionPath;
			CList< long >  m_TransportPickupToDropoffMultiRegionPath;
			CList< long >  m_GroundUnitFromDropoffToTargetMultiRegionPath;
			long  m_iLandPickupMultiRegion;
			long  m_iWaterPickupMultiRegion;
			long  m_iLandDropoffMultiRegion;
			long  m_iWaterDropoffMultiRegion;

			// The pathway of AIRegions
			CList< long >  m_GroundUnitToPickupAIRegionPath;
			CList< long >  m_TransportToPickupAIRegionPath;
			CList< long >  m_TransportPickupToDropoffAIRegionPath;
			CList< long >  m_GroundUnitFromDropoffToTargetAIRegionPath;
			long  m_iLandPickupAIRegion;
			long  m_iWaterPickupAIRegion;
			long  m_iLandDropoffAIRegion;
			long  m_iWaterDropoffAIRegion;
};

