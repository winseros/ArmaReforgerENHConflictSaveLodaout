class M1_ItemDisplayNameHelper
{
	static bool GetItemDisplayName(ResourceName resourceName, out string displayName)
	{
		//Credits: https://feedback.bistudio.com/T167392

		IEntitySource entitySource = SCR_BaseContainerTools.FindEntitySource(Resource.Load(resourceName));
		if (!entitySource)
			return false;
		
		for(int nComponent, componentCount = entitySource.GetComponentCount(); nComponent < componentCount; nComponent++)
		{
		    IEntityComponentSource componentSource = entitySource.GetComponent(nComponent);
		    if(componentSource.GetClassName().ToType().IsInherited(InventoryItemComponent))
		    {
		        BaseContainer attributesContainer = componentSource.GetObject("Attributes");
		        if (attributesContainer)
		        {
		            BaseContainer itemDisplayNameContainer = attributesContainer.GetObject("ItemDisplayName");
		            if (itemDisplayNameContainer)
		            {
			            UIInfo resultInfo = UIInfo.Cast(BaseContainerTools.CreateInstanceFromContainer(itemDisplayNameContainer));
						displayName = resultInfo.GetName();
						return true;
		            }
		        }
		    }
		}
		
		return false;
	}
}