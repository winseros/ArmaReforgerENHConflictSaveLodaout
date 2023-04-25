class M1_CharacterInventoryWriter
{
	protected IEntity m_Character;
	
	protected ref map<string, ref Resource> m_PrefabResources = new map<string, ref Resource>();
	
	private ref M1_CharacterArsenalKeyHelper m_KeyHelper;
	
	protected Resource GetPrefabResource(string prefabName)
	{
		Resource res;
		if (!m_PrefabResources.Find(prefabName, res))
		{
			res = Resource.Load(prefabName);
			m_PrefabResources.Set(prefabName, res);
		}
		return res;
	}
	
	protected WeaponSlotComponent FindWeaponSlot(array<WeaponSlotComponent> slots, int index)
	{
		foreach (WeaponSlotComponent slot : slots)
		{
			if (slot.GetWeaponSlotIndex() == index)
			{
				return slot;
			}
		}
		return null;
	}
	
	protected IEntity SpawnItem(string prefabResourceName)
	{
		Resource prefabResource = GetPrefabResource(prefabResourceName);			
        IEntity item = GetGame().SpawnEntityPrefab(prefabResource, null, null);
		return item;
	}
	
	protected bool TryTakeFirstMatchingStorageForContainer(array<BaseInventoryStorageComponent> storages, M1_CharacterArsenalInventoryContainer container, out BaseInventoryStorageComponent result)
	{
		for (int i = 0; i < storages.Count(); i++)
		{
			result = storages.Get(i);
			string storageKey = m_KeyHelper.ComputeStorageKey(result);
			if (storageKey == container.GetContainerName())
			{				
				storages.Remove(i);
				return true;
			}
		}
		return false;
	}
	
	protected void WriteContainer(M1_CharacterArsenalInventoryContainer container, BaseInventoryStorageComponent storage, SCR_InventoryStorageManagerComponent storageManager)
	{
		array<string> itemPrefabNames = {};
		container.GetItemPrefabNames(itemPrefabNames);
		
		foreach (string itemPrefabName : itemPrefabNames)
		{
			IEntity item = SpawnItem(itemPrefabName);
			if (!storageManager.TryInsertItemInStorage(item, storage))
			{
				Print(string.Format("Could not equip item: %1", itemPrefabName), LogLevel.WARNING);
			}
		}
		
		array<M1_CharacterArsenalInventoryContainer> compartmentContainers = {};
		container.GetCompartmentContainers(compartmentContainers);
	
		array<BaseInventoryStorageComponent> compartmentStorages = {};
		storage.GetOwnedStorages(compartmentStorages, 0, false);

		foreach (M1_CharacterArsenalInventoryContainer compartmentContainer : compartmentContainers)
		{
			BaseInventoryStorageComponent compartmentStorage;			
			if (TryTakeFirstMatchingStorageForContainer(compartmentStorages, compartmentContainer, compartmentStorage))
			{
				WriteContainer(compartmentContainer, compartmentStorage, storageManager);
			}
		}
		
		array<M1_CharacterArsenalInventoryContainer> attachedContainers = {};
		container.GetAttachedContainers(attachedContainers);
		
		array<BaseInventoryStorageComponent> attachedStorages = {};
		storage.GetOwnedStorages(attachedStorages, 1, false);
				
		foreach (M1_CharacterArsenalInventoryContainer attachedContainer : attachedContainers)
		{
			BaseInventoryStorageComponent attachedStorage;			
			if (TryTakeFirstMatchingStorageForContainer(attachedStorages, attachedContainer, attachedStorage))
			{
				WriteContainer(attachedContainer, attachedStorage, storageManager);
			}
		}
	}
	
	protected void WriteEquipment(M1_CharacterArsenalInventory inventory, IEntity character)
	{
		SCR_CharacterInventoryStorageComponent storage = SCR_CharacterInventoryStorageComponent.Cast(character.FindComponent(SCR_CharacterInventoryStorageComponent));
		SCR_InventoryStorageManagerComponent storageManager = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
		WriteContainer(inventory, storage, storageManager);
	}
	
	protected void WriteWeapons(M1_CharacterArsenalInventory inventory, IEntity character)
	{
		BaseWeaponManagerComponent weaponManager = BaseWeaponManagerComponent.Cast(character.FindComponent(BaseWeaponManagerComponent));
		
		array<WeaponSlotComponent> weaponSlots = {};
		weaponManager.GetWeaponsSlots(weaponSlots);
		
		array<M1_CharacterArsenalWeaponContainer> weapons = {};
		inventory.GetWeapons(weapons);
			
		foreach (M1_CharacterArsenalWeaponContainer weapon : weapons)
		{
			WeaponSlotComponent slot = FindWeaponSlot(weaponSlots, weapon.GetSlotIndex());
			if (!slot)
				continue;
			IEntity item = SpawnItem(weapon.GetPrefabName());
			weaponManager.SetSlotWeapon(slot, item);
		}
	}
	
	void WriteInventory(notnull M1_CharacterArsenalInventory inventory)
	{
		WriteEquipment(inventory, m_Character);
		WriteWeapons(inventory, m_Character);
	}
	
	void M1_CharacterInventoryWriter(IEntity character)
	{
		m_Character = character;
		m_KeyHelper = new M1_CharacterArsenalKeyHelper(character);
	}
}