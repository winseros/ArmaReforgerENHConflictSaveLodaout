class M1_CharacterArsenalInventoryContainer
{
	protected string m_sContainerName;
	protected ref array<string> m_aItemPrefabNames = {};	
	protected ref array<ref M1_CharacterArsenalInventoryContainer> m_aCompartmentContainers = {};
	protected ref array<ref M1_CharacterArsenalInventoryContainer> m_aAttachedContainers = {};
	
	string GetContainerName()
	{
		return m_sContainerName;
	}
	
	void AddItemPrefabName(string itemPrefabName)
	{
		m_aItemPrefabNames.Insert(itemPrefabName);
	}
	
	void AddCompartmentContainer(notnull M1_CharacterArsenalInventoryContainer container)
	{
		m_aCompartmentContainers.Insert(container);
	}
	
	void AddAttachedContainer(notnull M1_CharacterArsenalInventoryContainer container)
	{
		m_aAttachedContainers.Insert(container);
	}
	
	int GetItemPrefabNames(out notnull array<string> result)
	{
		return result.Copy(m_aItemPrefabNames);
	}
	
	int GetCompartmentContainers(out notnull array<M1_CharacterArsenalInventoryContainer> result)
	{
		//at the moment I write this code, Array.Copy(...) refuses to compile if called for an array<ref ...>
		foreach (M1_CharacterArsenalInventoryContainer c : m_aCompartmentContainers)
			result.Insert(c);
		return m_aCompartmentContainers.Count();
	}
	
	int GetAttachedContainers(out notnull array<M1_CharacterArsenalInventoryContainer> result)
	{
		//at the moment I write this code, Array.Copy(...) refuses to compile if called for an array<ref ...>
		foreach (M1_CharacterArsenalInventoryContainer c : m_aAttachedContainers)
			result.Insert(c);
		return m_aAttachedContainers.Count();
	}
	
	void M1_CharacterArsenalInventoryContainer(string containerName)
	{
		m_sContainerName = containerName;
	}
}