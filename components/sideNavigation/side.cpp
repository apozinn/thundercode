#include "side.hpp"

SideNavigation::SideNavigation(wxPanel* parent, wxWindowID ID) : wxPanel(parent, ID) 
{
	this->SetBackgroundColour(wxColor(36, 36, 36));
        sizer = new wxBoxSizer(wxVERTICAL);

        wxPanel* top_panel = new wxPanel(this);
        wxBoxSizer* top_panel_sizer = new wxBoxSizer(wxVERTICAL);

        //top_panel icons
        wxPanel* file_docker = new wxPanel(top_panel, ID_FILE_DOCKER);
        wxBoxSizer* fl_sizer = new wxBoxSizer(wxHORIZONTAL);

        wxPanel* fl_active = new wxPanel(file_docker);
        fl_active->SetBackgroundColour(wxColor(255, 255, 255));
        fl_sizer->Add(fl_active, 0, wxEXPAND);

        wxImagePanel* fl_icon = new wxImagePanel(file_docker, icons_dir+"file-dock.png", wxBITMAP_TYPE_PNG, 28);
        fl_sizer->Add(fl_icon, 1, wxEXPAND | wxALL, 10);

        file_docker->SetSizerAndFit(fl_sizer);

        top_panel_sizer->Add(file_docker, 0, wxEXPAND);

        search = new wxImagePanel(top_panel, icons_dir+"search.png", wxBITMAP_TYPE_PNG, 28);
        top_panel_sizer->Add(search, 0, wxEXPAND | wxALL, 10);

        wx = new wxImagePanel(top_panel, icons_dir+"wx.png", wxBITMAP_TYPE_PNG, 28);
        top_panel_sizer->Add(wx, 0, wxEXPAND | wxALL, 10);

        extensions = new wxImagePanel(top_panel, icons_dir+"extensions.png", wxBITMAP_TYPE_PNG, 28);
        top_panel_sizer->Add(extensions, 0, wxEXPAND | wxALL, 10);

        wxPanel* bottom_panel = new wxPanel(this);
        wxBoxSizer* bottom_panel_sizer = new wxBoxSizer(wxVERTICAL);

        //bottom_panel icons
        settings = new wxImagePanel(bottom_panel, icons_dir+"settings.png", wxBITMAP_TYPE_PNG, 28);
        bottom_panel_sizer->Add(settings, 0, wxEXPAND | wxALL, 10);

        hidde_sideNavigation = new wxImagePanel(bottom_panel, icons_dir+"hidde_sideNavigation.png", wxBITMAP_TYPE_PNG, 28);
        bottom_panel_sizer->Add(hidde_sideNavigation, 0, wxEXPAND | wxALL, 10);

        //set sizer to panels
        top_panel->SetSizerAndFit(top_panel_sizer);
        bottom_panel->SetSizerAndFit(bottom_panel_sizer);

        sizer->Add(top_panel, 1, wxEXPAND);
        sizer->Add(bottom_panel, 0, wxEXPAND);
        this->SetSizerAndFit(sizer);
}