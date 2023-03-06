#include "side.hpp"

SideNavigation::SideNavigation(wxPanel* parent, wxWindowID ID) : wxPanel(parent, ID) 
{
	this->SetBackgroundColour(wxColor(36, 36, 36));
        sizer = new wxBoxSizer(wxVERTICAL);

        wxPanel* top_panel = new wxPanel(this);
        wxBoxSizer* top_panel_sizer = new wxBoxSizer(wxVERTICAL);

        //top_panel icons
        file_docker = new wxImagePanel(top_panel, icons_dir+"file-dock.png", wxBITMAP_TYPE_PNG, 28);
        top_panel_sizer->Add(file_docker, 0, wxEXPAND | wxALL, 8);

        search = new wxImagePanel(top_panel, icons_dir+"search.png", wxBITMAP_TYPE_PNG, 28);
        top_panel_sizer->Add(search, 0, wxEXPAND | wxALL, 8);

        wx = new wxImagePanel(top_panel, icons_dir+"wx.png", wxBITMAP_TYPE_PNG, 28);
        top_panel_sizer->Add(wx, 0, wxEXPAND | wxALL, 8);

        extensions = new wxImagePanel(top_panel, icons_dir+"extensions.png", wxBITMAP_TYPE_PNG, 28);
        top_panel_sizer->Add(extensions, 0, wxEXPAND | wxALL, 8);

        wxPanel* bottom_panel = new wxPanel(this);
        wxBoxSizer* bottom_panel_sizer = new wxBoxSizer(wxVERTICAL);

        //bottom_panel icons
        settings = new wxImagePanel(bottom_panel, icons_dir+"settings.png", wxBITMAP_TYPE_PNG, 28);
        bottom_panel_sizer->Add(settings, 0, wxEXPAND | wxALL, 6);

        hidde_sideNavigation = new wxImagePanel(bottom_panel, icons_dir+"hidde_sideNavigation.png", wxBITMAP_TYPE_PNG, 28);
        bottom_panel_sizer->Add(hidde_sideNavigation, 0, wxEXPAND | wxALL, 6);

        //set sizer to panels
        top_panel->SetSizerAndFit(top_panel_sizer);
        bottom_panel->SetSizerAndFit(bottom_panel_sizer);

        sizer->Add(top_panel, 1, wxEXPAND);
        sizer->Add(bottom_panel, 0, wxEXPAND);
        this->SetSizerAndFit(sizer);
}