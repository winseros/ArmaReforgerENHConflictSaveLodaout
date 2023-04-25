class M1_CharacterArsenalQuantityValidationFailure : M1_CharacterArsenalItemValidationFailure
{
	protected int m_iMaxAllowedQuantity;
	
	int GetMaxAllowedQuantity()
	{
		return m_iMaxAllowedQuantity;
	}
	
	void M1_CharacterArsenalQuantityValidationFailure(ResourceName item, int maxAllowedQuantity)
	{
		m_iMaxAllowedQuantity = maxAllowedQuantity;
	}
}