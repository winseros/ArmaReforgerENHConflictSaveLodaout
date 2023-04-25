class M1_CharacterArsenalItemQuantity
{
	protected SCR_EArsenalItemType m_eItemType;
	
	protected int m_iMaxQuantity;
	
	SCR_EArsenalItemType GetItemType()
	{
		return m_eItemType;
	}
	
	int GetMaxQuantity()
	{
		return m_iMaxQuantity;
	}
	
	void M1_CharacterArsenalItemQuantity(SCR_EArsenalItemType itemType, int maxQuantity)
	{
		m_eItemType = itemType;
		m_iMaxQuantity = maxQuantity;
	}
}

class M1_CharacterArsenalItemQuantityValidationPolicy : M1_CharacterArsenalValidationPolicy
{
	protected ref map<ResourceName, SCR_EArsenalItemType> m_aItemTypes = new map<ResourceName, SCR_EArsenalItemType>();
	protected ref map<SCR_EArsenalItemType, int> m_aQuantityLimits = new map<SCR_EArsenalItemType, int>();
	
	override bool IsInventoryValid(FactionKey factionKey, notnull map<ResourceName, int> inventoryMap, out M1_CharacterArsenalValidationFailure failure)
	{
		map<SCR_EArsenalItemType, int> inventotyQuantities = new map<SCR_EArsenalItemType, int>();
		
		MapIterator it = inventoryMap.Begin();
		while (it != inventoryMap.End())
		{
			ResourceName item = inventoryMap.GetIteratorKey(it);
			SCR_EArsenalItemType inventoryType;
			if (m_aItemTypes.Find(item, inventoryType))
			{
				int currentCount;
				if (!inventotyQuantities.Find(inventoryType, currentCount))
					currentCount = 0;
				currentCount += inventoryMap.GetIteratorElement(it);
				int limit = m_aQuantityLimits.Get(inventoryType);
				if (currentCount > limit)
				{
					failure = new M1_CharacterArsenalQuantityValidationFailure(item, limit);
					return false;
				}
				inventotyQuantities.Set(inventoryType, currentCount);
			}
			it = inventoryMap.Next(it);
		}
		return true;
	}
	
	private void Initialize(FactionManager factionManager, array<ref M1_CharacterArsenalItemQuantity> itemQuantityLimits)
	{		
		foreach (M1_CharacterArsenalItemQuantity quantityLimit : itemQuantityLimits)
			m_aQuantityLimits.Insert(quantityLimit.GetItemType(), quantityLimit.GetMaxQuantity());
		
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
				if (m_aQuantityLimits.Contains(arsenalItem.GetItemType()))
				{
					m_aItemTypes.Insert(arsenalItem.GetItemResourceName(), arsenalItem.GetItemType());
				}
			}
		}
	}
	
	void M1_CharacterArsenalItemQuantityValidationPolicy(FactionManager factionManager, array<ref M1_CharacterArsenalItemQuantity> itemQuantityLimits)
	{
		Initialize(factionManager, itemQuantityLimits);
	}
}