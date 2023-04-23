modded class SCR_SelectSpawnPointSubMenu
{
	protected M1_ConflictLoadoutWidgetComponent m_LoadoutWidget;
	
	override void GetWidgets()
	{
		super.GetWidgets();
		m_LoadoutWidget = M1_ConflictLoadoutWidgetComponent.GetWidget("M1_SelectLoadout", GetRootWidget(), true);
	}
	
	protected override bool ConfirmSelection()
	{
		SCR_BasePlayerLoadout loadout;
		bool reqSpawnPoint = super.ConfirmSelection();
		if (!m_LoadoutWidget || !TryGetDesiredLoadout(m_LoadoutWidget.GetLoadoutType(), loadout))
		{
			return reqSpawnPoint;
		}
		else
		{	
			bool reqLoadout = RequestLoadout(loadout);
			return reqSpawnPoint && reqLoadout;
		}
	}
	
	override void SelectSpawnPoint()
	{
		super.SelectSpawnPoint();
		
		if (!m_LoadoutWidget)
			return;
		
		SCR_SpawnPoint selectedSpawnPoint = SCR_SpawnPoint.GetSpawnPointByRplId(m_SelectedSpawnPointId);
		if (selectedSpawnPoint)
			m_LoadoutWidget.SetSpawnPoint(selectedSpawnPoint);
	}
	
	protected bool TryGetDesiredLoadout(M1_EConflictLoadoutType loadoutType, out SCR_BasePlayerLoadout loadout)
	{
		SCR_LoadoutManager loadoutManager = GetGame().GetLoadoutManager();
		if (!loadoutManager)
			return false;
		
		Faction faction = SCR_RespawnSystemComponent.GetLocalPlayerFaction();
		if (!faction)
			return false;
		
		array<ref SCR_BasePlayerLoadout> loadouts = new array<ref SCR_BasePlayerLoadout>();
		int count = loadoutManager.GetPlayerLoadoutsByFaction(faction, loadouts);
		if (count != 2)
		{
			Print("The loadout manager must contain exactly two loadouts to work", LogLevel.ERROR);
			return false;
		}
		
		if (loadoutType == M1_EConflictLoadoutType.Unset)
			return false;
		
		loadout = loadouts.Get(0);	
		if (loadoutType == M1_EConflictLoadoutType.Saved)
			loadout = loadouts.Get(1);
		
		return true;
	}
}