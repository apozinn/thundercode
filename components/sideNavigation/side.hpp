#pragma once

#include "../../pages/settings/settings.cpp"

class SideNavigation : public wxPanel {
	wxBoxSizer* sizer;
	wxImagePanel* file_docker;
    wxImagePanel* search;
    wxImagePanel* wx;
    wxImagePanel* extensions;
    wxImagePanel* settings;
    wxImagePanel* hidde_sideNavigation;
    SettingsPage* settings_page;
	bool focusModeIsEnable = false;
public:
	SideNavigation(wxPanel* parent, wxWindowID ID);
	void OnHover(wxMouseEvent& event);
	void OnEndHover(wxMouseEvent& event);
	void SelectPage(wxMouseEvent& event);
	void OnFocusMode(wxCommandEvent& event);
	wxDECLARE_NO_COPY_CLASS(SideNavigation);
};
