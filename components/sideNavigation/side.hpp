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
	wxDECLARE_NO_COPY_CLASS(SideNavigation);
};