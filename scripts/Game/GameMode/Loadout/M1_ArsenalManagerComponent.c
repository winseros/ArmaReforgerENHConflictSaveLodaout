modded class SCR_ArsenalManagerComponent
{
	protected ref map<int, ref M1_CharacterArsenalInventory> m_mPlayerInventories = new map<int, ref M1_CharacterArsenalInventory>();
	
	bool GetPlayerArsenalInventory(int playerId, out M1_CharacterArsenalInventory inventory)
	{
		return m_mPlayerInventories.Find(playerId, inventory) && inventory != null;
	}
		
	override void SetPlayerArsenalLoadout(int playerId, GameEntity characterEntity)
	{
		if (SCR_ArsenalManagerComponent.IsCampaignGameMode())
		{
			SetPlayerArsenalLoadoutImpl(playerId, characterEntity);
		}
		else
		{
			super.SetPlayerArsenalLoadout(playerId, characterEntity);
		}
	}
	
	protected void SetPlayerArsenalLoadoutImpl(int playerId, GameEntity characterEntity)
	{
		if (playerId == 0)
			return;
		
		if (!characterEntity)
		{
			DoSetPlayerInventory(playerId, null);
			return;
		}
		
		SCR_PlayerController clientPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!clientPlayerController || clientPlayerController.IsPossessing())
		{
			return;
		}
				
		M1_CharacterInventoryReader reader = new M1_CharacterInventoryReader(characterEntity);
		M1_CharacterArsenalInventory inventory = reader.ReadInventory();
		
		DoSetPlayerInventory(playerId, inventory);
	}
	
	protected void DoSetPlayerInventory(int playerId, M1_CharacterArsenalInventory inventory)
	{
		bool inventoryValid = inventory != null;		
		
		m_mPlayerInventories.Set(playerId, inventory);
		
		DoSetPlayerHasLoadout(playerId, inventoryValid, true);
		Rpc(DoSetPlayerHasLoadout, playerId, inventoryValid, true);
	}
	
	protected static bool IsCampaignGameMode()
	{
		return SCR_GameModeCampaignMP.Cast(GetGame().GetGameMode());
	}
}