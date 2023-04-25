modded class SCR_ArsenalManagerComponent
{
	protected ref map<int, ref M1_CharacterArsenalInventory> m_mPlayerInventories = new map<int, ref M1_CharacterArsenalInventory>();
	
	protected ref M1_CharacterArsenalInventoryValidator m_InventoryValidator = new M1_CharacterArsenalInventoryValidator();	
	
	protected ref M1_ArsenalItemIdMap m_ItemIdMap;
	
	protected ref M1_NotificationDataGenerator m_NotificationDataGenerator;	
	
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
		
		FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast(characterEntity.FindComponent(FactionAffiliationComponent));
		if (!factionComponent)
		{
			return;
		}
		
		SCR_PlayerController clientPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!clientPlayerController || clientPlayerController.IsPossessing())
		{
			return;
		}
				
		M1_CharacterInventoryReader reader = new M1_CharacterInventoryReader(characterEntity);
		M1_CharacterArsenalInventory inventory = reader.ReadInventory();
		
		ResourceName firstRestrictedItem;
		FactionKey factionKey = factionComponent.GetAffiliatedFaction().GetFactionKey();
		
		M1_CharacterArsenalValidationFailure validationFailure;
		if (m_InventoryValidator.ValidateAndReturnFirstFailure(factionKey, inventory, validationFailure))
		{
			DoSetPlayerInventory(playerId, inventory);			
		}
		else
		{
			DoFailSaveInventory(playerId, validationFailure);
		}
	}
	
	protected void DoSetPlayerInventory(int playerId, M1_CharacterArsenalInventory inventory)
	{
		bool inventoryValid = inventory != null;		
		
		m_mPlayerInventories.Set(playerId, inventory);
		
		DoSetPlayerHasLoadout(playerId, inventoryValid, true);
		Rpc(DoSetPlayerHasLoadout, playerId, inventoryValid, true);
	}
	
	protected void DoFailSaveInventory(int playerId, M1_CharacterArsenalValidationFailure failure)
	{
		M1_NotificationParameterPack params;
		ENotification notificationId;
		if (!GetNotificationDataGenerator().GenerateNotificationData(failure, notificationId, params))
		{
			Print(string.Format("Could not generate a notification for the failure: %1", failure), LogLevel.WARNING);
			return;
		}
		
		SCR_NotificationsComponent.SendToPlayer(playerId, notificationId, params.P1(), params.P2(), params.P3(), params.P4(), params.P5());
	}
	
	protected static bool IsCampaignGameMode()
	{
		return SCR_GameModeCampaignMP.Cast(GetGame().GetGameMode());
	}
	
	protected M1_NotificationDataGenerator GetNotificationDataGenerator()
	{
		if (!m_NotificationDataGenerator)
			m_NotificationDataGenerator = new M1_NotificationDataGenerator(GetItemIdMap());
		return m_NotificationDataGenerator;
	}
	
	M1_ArsenalItemIdMap GetItemIdMap()
	{
		if (!m_ItemIdMap)
			m_ItemIdMap = new M1_ArsenalItemIdMap(GetGame().GetFactionManager());
		return m_ItemIdMap;
	}
}