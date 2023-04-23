class M1_CharacterArsenalInventory : M1_CharacterArsenalInventoryContainer
{
	protected FactionKey m_FactionKey;
	protected ref array<ref M1_CharacterArsenalWeaponContainer> m_aWeapons = {};
	
	void AddWeapon(notnull M1_CharacterArsenalWeaponContainer weapon)
	{
		m_aWeapons.Insert(weapon);
	}
	
	int GetWeapons(out notnull array<M1_CharacterArsenalWeaponContainer> result)
	{		
		//at the moment I write this code, Array.Copy() refuses to compile if called for an array<ref ...>
		foreach (M1_CharacterArsenalWeaponContainer w : m_aWeapons)
			result.Insert(w);
		return m_aWeapons.Count();
	}
	
	FactionKey GetAffiliatedFactionKey()
	{
		return m_FactionKey;
	}
	
	void M1_CharacterArsenalInventory(string containerName, FactionKey factionKey)
	{
		m_FactionKey = factionKey;
	}
}