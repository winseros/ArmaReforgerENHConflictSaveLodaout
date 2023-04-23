[BaseContainerProps(configRoot: true), BaseContainerCustomTitleField("m_sLoadoutName")]
class M1_ArsenalPlayerLoadout : SCR_FactionPlayerLoadout 
{
	override bool IsLoadoutAvailable(int playerId)
	{
		SCR_ArsenalManagerComponent arsenalManager;	
		if (SCR_ArsenalManagerComponent.GetArsenalManager(arsenalManager))
		{
			M1_CharacterArsenalInventory inventory;
			return arsenalManager.GetPlayerArsenalInventory(playerId, inventory);
		}
		return false;
	}

	override bool IsLoadoutAvailableClient()
	{
		SCR_ArsenalManagerComponent arsenalManager;
		if (SCR_ArsenalManagerComponent.GetArsenalManager(arsenalManager))
		{
			return arsenalManager.GetLocalPlayerLoadoutAvailable();
		}
		return false;
	}
	
	override void OnLoadoutSpawned(GenericEntity pOwner, int playerId)
	{
		super.OnLoadoutSpawned(pOwner, playerId);
		
		SCR_ArsenalManagerComponent arsenalManager;
		if (!SCR_ArsenalManagerComponent.GetArsenalManager(arsenalManager))
			return;
		
		M1_CharacterArsenalInventory inventory;
		if (!arsenalManager.GetPlayerArsenalInventory(playerId, inventory))
			return;
		
		GameEntity playerEntity = GameEntity.Cast(pOwner);
		string playerArsenalItems = string.Empty;
		if (!playerEntity)
			return;
		
		FactionAffiliationComponent faction = FactionAffiliationComponent.Cast(playerEntity.FindComponent(FactionAffiliationComponent));
		if (!faction)
			return;
		
		if (faction.GetAffiliatedFaction().GetFactionKey() != inventory.GetAffiliatedFactionKey())
			return;
		
		M1_CharacterInventoryWriter writer = new M1_CharacterInventoryWriter(playerEntity);
		writer.WriteInventory(inventory);
	}
}