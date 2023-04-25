[BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
class M1_NotificationCharacterArsenalItemQuantity : M1_NotificationCharacterArsenalItemForbidden
{
	override string GetText(SCR_NotificationData data)
	{	
		int itemId, itemMaxQuantity;
		data.GetParams(itemId, itemMaxQuantity);

		string displayName;
		if (GetItemDisplayName(itemId, displayName))
			data.SetNotificationTextEntries(displayName, itemMaxQuantity.ToString());
				
		return super.GetText(data);
	}
}