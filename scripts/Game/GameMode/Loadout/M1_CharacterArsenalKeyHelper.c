class M1_CharacterArsenalKeyHelper
{
	private IEntity m_Character;
	
	string ComputeStorageKey(notnull BaseInventoryStorageComponent storage)
	{
		IEntity owner = storage.GetOwner();
		if (owner == m_Character)
			return "<Player>";
		
		return owner.GetPrefabData().GetPrefabName();
	}
	
	void M1_CharacterArsenalKeyHelper(IEntity character)
	{
		m_Character = character;
	}
}