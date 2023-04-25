class M1_CharacterArsenalInventoryValidator
{
	protected ref array<ref M1_CharacterArsenalValidationPolicy> m_aValidationPolicies;
	
	protected void CollectItem(ResourceName item, notnull map<ResourceName, int> inventoryMap)
	{
		int count;
		if (inventoryMap.Find(item, count))
			count = count + 1;
		else
			count = 1;
		
		inventoryMap.Set(item, count);
	}
	
	protected void CollectFromContainer(M1_CharacterArsenalInventoryContainer container, notnull map<ResourceName, int> inventoryMap)
	{
		array<string> items = {};
		container.GetItemPrefabNames(items);
		foreach (string item : items)
			CollectItem(item, inventoryMap);
		
		array<M1_CharacterArsenalInventoryContainer> compartments = {};
		container.GetCompartmentContainers(compartments);
		foreach (M1_CharacterArsenalInventoryContainer compartment : compartments)
			CollectFromContainer(compartment, inventoryMap);
		
		array<M1_CharacterArsenalInventoryContainer> attachments = {};
		container.GetAttachedContainers(attachments);
		foreach (M1_CharacterArsenalInventoryContainer attachment : attachments)
			CollectFromContainer(attachment, inventoryMap);
	}
	
	protected void CollectFromWeapons(M1_CharacterArsenalInventory inventory, out notnull map<ResourceName, int> inventoryMap)
	{
		array<M1_CharacterArsenalWeaponContainer> weapons = {};
		inventory.GetWeapons(weapons);
		foreach (M1_CharacterArsenalWeaponContainer weapon : weapons)
			CollectItem(weapon.GetPrefabName(), inventoryMap);
	}
	
	protected map<ResourceName, int> BuildInventoryMap(M1_CharacterArsenalInventory inventory)
	{
		map<ResourceName, int> result = new map<ResourceName, int>();
		CollectFromContainer(inventory, result);
		CollectFromWeapons(inventory, result);
		return result;
	}
	
	protected void InitPolicies()
	{
		if (m_aValidationPolicies)
			return;
		
		m_aValidationPolicies = {
			new M1_CharacterArsenalItemFactionValidationPolicy(GetGame().GetFactionManager()),
			new M1_CharacterArsenalItemTypeValidationPolicy(GetGame().GetFactionManager(), {
				SCR_EArsenalItemType.RADIO_BACKPACK
			}),
			new M1_CharacterArsenalItemQuantityValidationPolicy(GetGame().GetFactionManager(), {
				new M1_CharacterArsenalItemQuantity(SCR_EArsenalItemType.HEAL, 2)
			})
		};
	}
	
	bool ValidateAndReturnFirstFailure(FactionKey factionKey, M1_CharacterArsenalInventory inventory, out M1_CharacterArsenalValidationFailure firstFailure)
	{
		InitPolicies();
		
		map<ResourceName, int> inventoryMap = BuildInventoryMap(inventory);
		
		foreach (M1_CharacterArsenalValidationPolicy policy : m_aValidationPolicies)
		{
			if (!policy.IsInventoryValid(factionKey, inventoryMap, firstFailure))
				return false;
		}
		
		return true;
	}
}