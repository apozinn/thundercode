#pragma once

class SideNavigation : public wxPanel {
	wxBoxSizer* sizer;
	wxImagePanel* file_docker;
    wxImagePanel* search;
    wxImagePanel* wx;
    wxImagePanel* extensions;
    wxImagePanel* settings;
    wxImagePanel* hidde_sideNavigation;
public:
	SideNavigation(wxPanel* parent, wxWindowID ID);
	void OnHover(wxMouseEvent& event);
	void OnEndHover(wxMouseEvent& event);
	wxDECLARE_NO_COPY_CLASS(SideNavigation);
};