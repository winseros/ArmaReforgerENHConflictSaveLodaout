class M1_CharacterArsenalItemFactionValidationPolicy : M1_CharacterArsenalValidationPolicy
{
	protected ref map<FactionKey, ref set<ResourceName>> m_FactionItems = new map<FactionKey, ref set<ResourceName>>();	
	
	override bool IsInventoryValid(FactionKey factionKey, notnull map<ResourceName, int> inventoryMap, out M1_CharacterArsenalValidationFailure failure)
	{
		set<ResourceName> factionItems;
		if (!m_FactionItems.Find(factionKey, factionItems))
			return false;
		
		MapIterator it = inventoryMap.Begin();
		while (it != inventoryMap.End())
		{
			ResourceName item = inventoryMap.GetIteratorKey(it);
			if (!factionItems.Contains(item))
			{
				failure = new M1_CharacterArsenalItemValidationFailure(item);
				return false;
			}
			it = inventoryMap.Next(it);
		}
		
		return true;
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
			
			set<ResourceName> factionItems = new set<ResourceName>();
			m_FactionItems.Set(faction.GetFactionKey(), factionItems);
			
			foreach (SCR_ArsenalItem arsenalItem : arsenalItems)
				factionItems.Insert(arsenalItem.GetItemResourceName());
		}
	}
	
	void M1_CharacterArsenalItemFactionValidationPolicy(FactionManager factionManager)
	{
		Initialize(factionManager);
	}
}