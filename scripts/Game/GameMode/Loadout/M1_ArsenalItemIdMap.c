class M1_ArsenalItemIdMap
{	
	protected ref map<int, ResourceName> m_aIdToRes = new map<int, ResourceName>();
	protected ref map<ResourceName, int> m_aResToId = new map<ResourceName, int>();
	
	protected void RegisterItem(SCR_ArsenalItem item)
	{
		ResourceName res = item.GetItemResourceName();
		int id = m_aIdToRes.Count();
		
		m_aIdToRes.Insert(id, res);
		m_aResToId.Insert(res, id);		
	}
	
	bool GetItemIdByResourceName(ResourceName resourceName, out int itemId)
	{
		return m_aResToId.Find(resourceName, itemId);
	}
	
	bool GetItemResourceNameById(int itemId, out ResourceName resourceName)
	{
		return m_aIdToRes.Find(itemId, resourceName);
	}
	
	private void Initialize(FactionManager factionManager)
	{
		array<Faction> factions = {};
		factionManager.GetFactionsList(factions);		
		
		foreach (Faction faction : factions)
		{
			SCR_Faction scriptedFaction = SCR_Faction.Cast(faction);
			if (!scriptedFaction)
				continue;
			
			array<ref SCR_ArsenalItem> arsenalItems;
			if (!scriptedFaction.GetArsenalItems(arsenalItems))
				continue;			
			
			// I expect this code to provide the same order of items both on the server and on clients
			// so instead of running on the server and then syncing to clients
			// we just run it everywere and expect to get the same result
			foreach (SCR_ArsenalItem arsenalItem : arsenalItems)
				RegisterItem(arsenalItem);
		}
	}
	
	void M1_ArsenalItemIdMap(FactionManager factionManager)
	{
		Initialize(factionManager);
	}
}