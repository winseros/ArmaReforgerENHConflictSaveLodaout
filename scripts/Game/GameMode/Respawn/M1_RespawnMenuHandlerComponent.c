modded class SCR_RespawnMenuHandlerComponent
{
	static SCR_RespawnMenuHandlerComponent GetRespawnMenuHandlerComponent()
	{	
		return SCR_RespawnMenuHandlerComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_RespawnMenuHandlerComponent));
	}
	
	override bool RandomizePlayerLoadout(int playerId)
	{
		if (!SCR_GameModeCampaignMP.Cast(GetGameMode()))
			return super.RandomizePlayerLoadout(playerId);
		
		return false;
	}
}