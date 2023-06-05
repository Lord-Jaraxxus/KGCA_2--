#include "K_List.h"

bool K_List::Frame()
{
	K_UIObject* LastUI = nullptr;
	for (auto Iter : m_pUIList)
	{
		Iter->Frame();
		Iter->m_bDraggable = false;
		if (Iter->m_bIsClicked) { Iter->m_bIsClicked = false;  LastUI = Iter; }
	}
	if (LastUI != nullptr)
	{
		LastUI->m_bDraggable = true;
		LastUI->m_bIsClicked = true;
	}

	return true;
}

bool K_List::Render()
{
	return true;
}
