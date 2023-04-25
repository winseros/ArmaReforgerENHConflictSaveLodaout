class M1_CharacterArsenalItemValidationFailure : M1_CharacterArsenalValidationFailure
{
	protected ResourceName m_iItem;
	
	ResourceName GetItem()
	{
		return m_iItem;
	}
	
	void M1_CharacterArsenalItemValidationFailure(ResourceName item)
	{
		m_iItem = item;
	}
}