class M1_NotificationParameterPack
{
	int m_p1, m_p2, m_p3, m_p4, m_p5;
	
	void GetParams(out int p1, out int p2, out int p3, out int p4, out int p5)
	{
		p1 = m_p1;
		p2 = m_p2;
		p3 = m_p3;
		p4 = m_p4;
		p5 = m_p5;
	}
	
	void M1_NotificationParameterPack(int p1, int p2 = 0, int p3 = 0, int p4 = 0, int p5 = 0)
	{
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
		m_p4 = p4;
		m_p5 = p5;
	}
}

class M1_NotificationDataGenerator
{
	protected ref M1_ArsenalItemIdMap m_ItemIdMap;
	
	protected bool GenerateQuantityValidationFailure(M1_CharacterArsenalQuantityValidationFailure failure, out ENotification notification, out M1_NotificationParameterPack params)
	{		
		int itemId;
		if (!m_ItemIdMap.GetItemIdByResourceName(failure.GetItem(), itemId))
			return false;
				
		params = new M1_NotificationParameterPack(p1: itemId, p2: failure.GetMaxAllowedQuantity());
		notification = ENotification.M1_INVENTORY_SAVE_FAILED_ITEM_QUANTITY;
		
		return true;
	}
	
	protected bool GenerateItemValidationFailure(M1_CharacterArsenalItemValidationFailure failure, out ENotification notification, out M1_NotificationParameterPack params)
	{
		int itemId;
		if (!m_ItemIdMap.GetItemIdByResourceName(failure.GetItem(), itemId))
			return false;
				
		params = new M1_NotificationParameterPack(p1: itemId);
		notification = ENotification.M1_INVENTORY_SAVE_FAILED_ITEM_RESTRICTED;		
		return true;
	}
	
	bool GenerateNotificationData(M1_CharacterArsenalValidationFailure failure, out ENotification notification, out M1_NotificationParameterPack params)
	{
		M1_CharacterArsenalQuantityValidationFailure f1 = M1_CharacterArsenalQuantityValidationFailure.Cast(failure);
		if (f1)
		{
			return GenerateQuantityValidationFailure(f1, notification, params);
		}
		M1_CharacterArsenalItemValidationFailure f2 = M1_CharacterArsenalItemValidationFailure.Cast(failure);
		if (f2)
		{
			return GenerateItemValidationFailure(f2, notification, params);
		}
		return false;
	}
	
	void M1_NotificationDataGenerator(notnull M1_ArsenalItemIdMap itemIdMap)
	{
		m_ItemIdMap = itemIdMap;
	}
}