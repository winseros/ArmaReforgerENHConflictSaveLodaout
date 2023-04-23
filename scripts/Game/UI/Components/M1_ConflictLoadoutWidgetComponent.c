enum M1_EConflictLoadoutType
{
	Unset,
	Default,
	Saved
}

class M1_ConflictLoadoutWidgetComponent : SCR_ScriptedWidgetComponent 
{	
	protected SCR_ButtonBaseComponent m_BtnDefault;
	protected SCR_ButtonBaseComponent m_BtnSaved;
	
	private SCR_SpawnPoint m_SpawnPoint;
	private M1_EConflictLoadoutType m_ActualLoadoutType = M1_EConflictLoadoutType.Unset;
	private static M1_EConflictLoadoutType m_DesiredLoadoutType = M1_EConflictLoadoutType.Default;
	
	static M1_ConflictLoadoutWidgetComponent GetWidget(string name, Widget parent, bool searchAllChildren = true)
	{
		return M1_ConflictLoadoutWidgetComponent.Cast(SCR_ScriptedWidgetComponent.GetComponent(M1_ConflictLoadoutWidgetComponent, name, parent, searchAllChildren));
	}
	
	void SetSpawnPoint(SCR_SpawnPoint spawnPoint)
	{
		if (!m_wRoot.IsVisible())
			return;
		
		m_SpawnPoint = spawnPoint;
		UpdateComponentState();
	}
	
	M1_EConflictLoadoutType GetLoadoutType()
	{
		return m_ActualLoadoutType;
	}
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		if (SCR_Global.IsEditMode())
			return;
		
		SCR_RespawnMenuHandlerComponent respawnMenu = SCR_RespawnMenuHandlerComponent.GetRespawnMenuHandlerComponent();
		if (!IsCampaignGameMode() || !IsConfiguredArsenal() || !respawnMenu || respawnMenu.GetAllowLoadoutSelection())
		{
			w.SetVisible(false);
			return;
		}
		
		m_BtnDefault = SCR_ButtonBaseComponent.GetButtonBase("ButtonDefault", w, true);
		m_BtnSaved = SCR_ButtonBaseComponent.GetButtonBase("ButtonSaved", w, true);
		
		m_BtnDefault.m_OnClicked.Insert(OnDefaultClick);
		m_BtnSaved.m_OnClicked.Insert(OnSavedClick);
		
		UpdateComponentState();
	}
	
	protected void UpdateComponentState()
	{
		if (m_SpawnPoint == null)
		{
			DisableSavedLoadout();
			return;
		}
		
		if (!SpawnPointHasArmory())
		{
			DisableSavedLoadout();
			return;
		}
		
		if (!PlayerHasCustomLoadout())
		{
			DisableSavedLoadout();
			return;
		}
		
		ResetDesiredState();
	}
	
	protected void OnDefaultClick()
	{
		m_BtnSaved.SetToggled(false);
		m_BtnDefault.SetToggled(true);
		SetLoadoutType(M1_EConflictLoadoutType.Default, true);
	}
	
	protected void OnSavedClick()
	{
		m_BtnSaved.SetToggled(true);
		m_BtnDefault.SetToggled(false);
		SetLoadoutType(M1_EConflictLoadoutType.Saved, true);
	}
	
	protected void DisableSavedLoadout()
	{
		m_BtnSaved.SetToggled(false);
		m_BtnSaved.SetEnabled(false);
		m_BtnDefault.SetToggled(true);
		
		SetLoadoutType(M1_EConflictLoadoutType.Default, false);
	}
	
	protected void ResetDesiredState()
	{
		m_BtnSaved.SetEnabled(true);
		if (m_DesiredLoadoutType == M1_EConflictLoadoutType.Default)
		{
			m_BtnSaved.SetToggled(false);
			m_BtnDefault.SetToggled(true);
		}
		if (m_DesiredLoadoutType == M1_EConflictLoadoutType.Saved)
		{
			m_BtnSaved.SetToggled(true);
			m_BtnDefault.SetToggled(false);
		}
		SetLoadoutType(m_DesiredLoadoutType, false);
	}
	
	protected bool IsCampaignGameMode()
	{
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.Cast(GetGame().GetGameMode());
		return campaign != null;
	}
	
	protected bool SpawnPointHasArmory()
	{
		SCR_CampaignBase base = SCR_CampaignBase.Cast(m_SpawnPoint.GetParent());
		if (base == null)
			return false;		
			
		if (base.GetType() == CampaignBaseType.MAIN)
			return true;
		
		SCR_CampaignServiceComponent armory = base.GetBaseService(ECampaignServicePointType.ARMORY);
		return armory != null;
	}
	
	protected bool IsConfiguredArsenal()
	{
		SCR_ArsenalManagerComponent arsenal;
		SCR_ArsenalManagerComponent.GetArsenalManager(arsenal);
		return arsenal != null;
	}
	
	protected bool PlayerHasCustomLoadout()
	{
		SCR_ArsenalManagerComponent arsenal;
		bool result = SCR_ArsenalManagerComponent.GetArsenalManager(arsenal) && arsenal.GetLocalPlayerLoadoutAvailable();
		return result;
	}
	
	protected void SetLoadoutType(M1_EConflictLoadoutType actualLoadoutType, bool alsoUpdateDesiredType)
	{
		m_ActualLoadoutType = actualLoadoutType;
		if (alsoUpdateDesiredType)
			m_DesiredLoadoutType = actualLoadoutType;
	}
}