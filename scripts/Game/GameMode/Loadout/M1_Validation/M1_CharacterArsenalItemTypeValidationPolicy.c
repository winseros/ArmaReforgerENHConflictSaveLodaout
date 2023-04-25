class M1_CharacterArsenalItemTypeValidationPolicy : M1_CharacterArsenalValidationPolicy
{
	protected ref set<ResourceName> m_aForbiddenItemTypes = new set<ResourceName>();
	
	override bool IsInventoryValid(FactionKey factionKey, notnull map<ResourceName, int> inventoryMap, out M1_CharacterArsenalValidationFailure failure)
	{
		MapIterator it = inventoryMap.Begin();
		while (it != inventoryMap.End())
		{
			ResourceName item = inventoryMap.GetIteratorKey(it);
			if (m_aForbiddenItemTypes.Contains(item))
			{
				failure = new M1_CharacterArsenalItemValidationFailure(item);
				return false;
			}
			it = inventoryMap.Next(it);
		}
		return true;
	}
	
	private void Initialize(FactionManager factionManager, array<SCR_EArsenalItemType> forbiddenItemTypes)
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
			
			foreach (SCR_ArsenalItem arsenalItem : arsenalItems)
			{
				if (forbiddenItemTypes.Contains(arsenalItem.GetItemType()))
				{
					m_aForbiddenItemTypes.Insert(arsenalItem.GetItemResourceName());
				}
			}
		}
	}
	
	void M1_CharacterArsenalItemTypeValidationPolicy(FactionManager factionManager, array<SCR_EArsenalItemType> forbiddenItemTypes)
	{
		Initialize(factionManager, forbiddenItemTypes);
	}
}