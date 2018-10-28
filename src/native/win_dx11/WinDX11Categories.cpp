
#include "EUICategories.h"
#include "WinDX11Categories.h"
#include "EUIScrollBar.h"
#include "WinDX11ScrollBar.h"

#ifdef PLATFORM_WIN_DX11

WinDX11Categories::WinDX11Categories(EUIWidget* owner) : NativeCategories(owner)
{
	overallHeight = 0;

	/*handle = CreateWindow("STATIC", "", SS_LEFT | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
	                      (int)Owner()->x, (int)Owner()->y, (int)Owner()->width, (int)Owner()->height,
	                      ((WinWidget*)Owner()->parent->nativeWidget)->GetHandle(), win_id, NULL, NULL);
	win_id++;

	if (!Owner()->auto_size)
	{
		Owner()->nativeWidget = this;
		scrollbar = new EUIScrollBar(Owner(), false, Owner()->width - 20, 0, 20, Owner()->height);
		scrollbar->Show(false);
		scrollbar->SetListener(0, this, 0);
	}

	MakeSubClassing();*/
}

WinDX11Categories::~WinDX11Categories()
{
}

EUICategories* WinDX11Categories::Owner()
{
	return (EUICategories*)owner;
}

void WinDX11Categories::OnSrollerPosChange(EUIScrollBar* sender, int pos)
{
	UpdateChildPos();
	Redraw();
}

void WinDX11Categories::CalcThumb()
{
	overallHeight = 0;

	for (int i = 0; i < (int)Owner()->categories.size(); i++)
	{
		EUICategories::Category& category = Owner()->categories[i];

		overallHeight += theme->categoryHeight;

		for (int j = 0; j < (int)category.childs.size(); j++)
		{
			if (category.opened)
			{
				overallHeight += category.childs[j]->GetHeight();
			}
		}
	}

	if (!Owner()->auto_size)
	{
		scrollbar->SetPos(Owner()->width - 20, 0);
		scrollbar->SetSize(20, Owner()->height);

		float delta = overallHeight - Owner()->height;
		scrollbar->Show(delta > 0.0f);

		if (delta > 0.0f)
		{
			scrollbar->SetLimit(1, (int)delta);
		}
	}
}

void WinDX11Categories::UpdateChildPos()
{
	Owner()->allowCallOnChildShow = false;

	float pos = 0;

	if (scrollbar && scrollbar->IsVisible())
	{
		pos = -(float)scrollbar->GetPosition();
	}

	for (int i = 0; i < (int)Owner()->categories.size(); i++)
	{
		EUICategories::Category& category = Owner()->categories[i];

		category.y = pos;
		pos += theme->categoryHeight;

		category.visible = false;

		for (int j = 0; j < (int)category.childs.size(); j++)
		{
			if (!category.childsVis[j])
			{
				continue;
			}

			category.visible = true;
			category.childs[j]->Show(category.opened);

			if (category.opened)
			{
				category.childs[j]->SetPos(category.childs[j]->GetX(), (int)pos);
				pos += category.childs[j]->GetHeight();
			}
		}

		if (!category.visible)
		{
			pos -= theme->categoryHeight;
		}
	}

	if (Owner()->auto_size)
	{
		Owner()->SetSize(Owner()->width, (int)pos);

		if (Owner()->parent)
		{
			EUICategories* cat = dynamic_cast<EUICategories*>(Owner()->parent);

			if (cat)
			{
				cat->Native()->UpdateChildPos();
			}
		}
	}

	Owner()->allowCallOnChildShow = true;
}

void WinDX11Categories::Resize()
{
	CalcThumb();
	UpdateChildPos();
	NativeCategories::Resize();
}

void WinDX11Categories::Draw()
{
	/*UINT state = EUITheme::UISTATE_NORMAL;

	if (!Owner()->IsEnabled())
	{
		state = EUITheme::UISTATE_DISABLED;
	}

	COLORREF color = theme->GetColor("LABEL_BACK");
	//theme->DrawGradient(GetDC(handle), { 0, 0, (LONG)Owner()->width, (LONG)Owner()->height }, color, color, false, 2);

	for (int i = 0; i < (int)Owner()->categories.size(); i++)
	{
		EUICategories::Category& category = Owner()->categories[i];

		if (!category.visible)
		{
			continue;
		}

		RECT rc = { 0, (LONG)category.y, (LONG)Owner()->width, (LONG)(category.y + theme->categoryHeight) };

		UINT sub_state = 0;

		if (category.opened)
		{
			sub_state = EUITheme::UISTATE_PUSHED;
		}

		//theme->DrawCategory(GetDC(handle), rc, category.name, state | sub_state, DT_SINGLELINE);
	}

	if (thumbHeight > 0)
	{
		RECT rc = { 0, 0, (LONG)Owner()->width, (LONG)Owner()->height };
		theme->DrawScrollBar(GetDC(handle), rc, (int)thumbPos, (int)thumbHeight, state);
	}*/
}
#endif
