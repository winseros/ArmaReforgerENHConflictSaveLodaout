[BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
class M1_NotificationCharacterArsenalItemForbidden : SCR_NotificationDisplayData
{
	override string GetText(SCR_NotificationData data)
	{	
		int itemId;
		data.GetParams(itemId);
		
		string displayName;
		if (GetItemDisplayName(itemId, displayName))
			data.SetNotificationTextEntries(displayName);
				
		return super.GetText(data);
	}
	
	protected bool GetItemDisplayName(int itemId, out string displayName)
	{
		SCR_ArsenalManagerComponent arsenalManager;
		if (!SCR_ArsenalManagerComponent.GetArsenalManager(arsenalManager))
			return false;
		
		M1_ArsenalItemIdMap itemMap = arsenalManager.GetItemIdMap();
			
		ResourceName resourceName;			
		bool result = itemMap.GetItemResourceNameById(itemId, resourceName) 
			&& M1_ItemDisplayNameHelper.GetItemDisplayName(resourceName, displayName);
			
		return result;		
	}
}