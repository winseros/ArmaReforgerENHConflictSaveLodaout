class M1_CharacterArsenalWeaponContainer
{
	protected string m_sPrefabName;
	protected int m_iSlotIndex;
	
	string GetPrefabName()
	{
		return m_sPrefabName;
	}
	
	int GetSlotIndex()
	{
		return m_iSlotIndex;
	}
	
	void M1_CharacterArsenalWeaponContainer(string prefabName, int slotIndex)
	{
		m_sPrefabName = prefabName;
		m_iSlotIndex = slotIndex;
	}
}