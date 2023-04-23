class M1_CharacterInventoryReader
{
	protected IEntity m_Character;
	
	private ref M1_CharacterArsenalKeyHelper m_KeyHelper;
	
	protected void InflateContainer(M1_CharacterArsenalInventoryContainer container, BaseInventoryStorageComponent storage)
	{
		array<IEntity> items = {};
		storage.GetAll(items);
		foreach (IEntity item : items)
		{
			if (storage.Contains(item))
				container.AddItemPrefabName(item.GetPrefabData().GetPrefabName());
		}
		
		array<BaseInventoryStorageComponent> compartmentStorages = {};
		storage.GetOwnedStorages(compartmentStorages, 0, false);		
		foreach (BaseInventoryStorageComponent compartmentStorage : compartmentStorages)
		{
			string compartmentKey = m_KeyHelper.ComputeStorageKey(compartmentStorage);
			M1_CharacterArsenalInventoryContainer compartmentContainer = new M1_CharacterArsenalInventoryContainer(compartmentKey);
			InflateContainer(compartmentContainer, compartmentStorage);
			container.AddCompartmentContainer(compartmentContainer);
		}
		
		array<BaseInventoryStorageComponent> attachedStorages = {};
		storage.GetOwnedStorages(attachedStorages, 1, false);		
		foreach (BaseInventoryStorageComponent attachedStorage : attachedStorages)
		{
			string attachedKey = m_KeyHelper.ComputeStorageKey(attachedStorage);
			M1_CharacterArsenalInventoryContainer attachedContainer = new M1_CharacterArsenalInventoryContainer(attachedKey);
			InflateContainer(attachedContainer, attachedStorage);
			container.AddAttachedContainer(attachedContainer);
		}
	}
	
	protected void InflateEquipment(M1_CharacterArsenalInventory inventory, IEntity character)
	{
		SCR_CharacterInventoryStorageComponent storage = SCR_CharacterInventoryStorageComponent.Cast(m_Character.FindComponent(SCR_CharacterInventoryStorageComponent));
		InflateContainer(inventory, storage);
	}
	
	protected void InflateWeapons(M1_CharacterArsenalInventory inventory, IEntity character)
	{
		BaseWeaponManagerComponent weaponManager = BaseWeaponManagerComponent.Cast(character.FindComponent(BaseWeaponManagerComponent));
		if (!weaponManager)
			return;
		
		array<WeaponSlotComponent> weaponSlots = {};
		weaponManager.GetWeaponsSlots(weaponSlots);
		
		foreach (WeaponSlotComponent weaponSlot : weaponSlots)
		{
			IEntity weapon = weaponSlot.GetWeaponEntity();
			if (!weapon)
				continue;
			
			int index = weaponSlot.GetWeaponSlotIndex();	
			inventory.AddWeapon(new M1_CharacterArsenalWeaponContainer(weapon.GetPrefabData().GetPrefabName(), index));
		}
	}
	
	M1_CharacterArsenalInventory ReadInventory()
	{
		FactionAffiliationComponent faction = FactionAffiliationComponent.Cast(m_Character.FindComponent(FactionAffiliationComponent));
		if (!faction)
		{
			Print("The character must have a faction", LogLevel.ERROR);
			return null;
		}
		
		M1_CharacterArsenalInventory inventory = new M1_CharacterArsenalInventory("Root", faction.GetAffiliatedFaction().GetFactionKey());
		
		InflateEquipment(inventory, m_Character);
		InflateWeapons(inventory, m_Character);
		
		return inventory;
	}
	
	void M1_CharacterInventoryReader(notnull IEntity character)
	{
		m_Character = character;
		m_KeyHelper = M1_CharacterArsenalKeyHelper(character);
	}
}