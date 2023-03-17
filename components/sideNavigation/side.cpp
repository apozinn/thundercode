#include "side.hpp"

SideNavigation::SideNavigation(wxPanel* parent, wxWindowID ID) : wxPanel(parent, ID) 
{
    this->SetBackgroundColour(wxColor(36, 36, 36));
    sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* top_container = new wxPanel(this);
    wxBoxSizer* top_ctn_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* code_page = new wxPanel(top_container);
    wxBoxSizer* code_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    top_ctn_sizer->Add(code_page, 0, wxEXPAND);

    wxPanel* code_pg_line = new wxPanel(code_page);
    code_pg_line->SetBackgroundColour(wxColor(255, 0, 180));
    code_pg_sizer->Add(code_pg_line, 1, wxEXPAND);

    wxImagePanel* code_pg_icon = new wxImagePanel(code_page, icons_dir+"code_page.png", wxBITMAP_TYPE_PNG, 26);
    code_pg_sizer->Add(code_pg_icon, 20, wxEXPAND | wxALL, 10);
    code_page->SetSizerAndFit(code_pg_sizer);

    wxPanel* search_page = new wxPanel(top_container);
    wxBoxSizer* search_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    top_ctn_sizer->Add(search_page, 0, wxEXPAND);

    wxPanel* search_pg_line = new wxPanel(search_page);
    search_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    search_pg_sizer->Add(search_pg_line, 1, wxEXPAND);

    wxImagePanel* search_pg_icon = new wxImagePanel(search_page, icons_dir+"search.png", wxBITMAP_TYPE_PNG, 26);
    search_pg_sizer->Add(search_pg_icon, 20, wxEXPAND | wxALL, 10);
    search_page->SetSizerAndFit(search_pg_sizer);

    wxPanel* builder_page = new wxPanel(top_container);
    wxBoxSizer* builder_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    top_ctn_sizer->Add(builder_page, 0, wxEXPAND);

    wxPanel* builder_pg_line = new wxPanel(builder_page);
    builder_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    builder_pg_sizer->Add(builder_pg_line, 1, wxEXPAND);

    wxImagePanel* builder_pg_icon = new wxImagePanel(builder_page, icons_dir+"builder.png", wxBITMAP_TYPE_PNG, 26);
    builder_pg_sizer->Add(builder_pg_icon, 20, wxEXPAND | wxALL, 10);
    builder_page->SetSizerAndFit(builder_pg_sizer);
   
    wxPanel* extensions_page = new wxPanel(top_container);
    wxBoxSizer* extensions_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    top_ctn_sizer->Add(extensions_page, 0, wxEXPAND);

    wxPanel* extensions_pg_line = new wxPanel(extensions_page);
    extensions_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    extensions_pg_sizer->Add(extensions_pg_line, 1, wxEXPAND);

    wxImagePanel* extensions_pg_icon = new wxImagePanel(extensions_page, icons_dir+"extensions.png", wxBITMAP_TYPE_PNG, 26);
    extensions_pg_sizer->Add(extensions_pg_icon, 20, wxEXPAND | wxALL, 10);
    extensions_page->SetSizerAndFit(extensions_pg_sizer);
    
    top_container->SetSizerAndFit(top_ctn_sizer);
    
    wxPanel* bottom_container = new wxPanel(this);
    wxBoxSizer* bottom_ctn_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* settings_page = new wxPanel(bottom_container);
    wxBoxSizer* settings_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    bottom_ctn_sizer->Add(settings_page, 0, wxEXPAND);

    wxPanel* settings_pg_line = new wxPanel(settings_page);
    settings_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    settings_pg_sizer->Add(settings_pg_line, 1, wxEXPAND);

    wxImagePanel* settings_pg_icon = new wxImagePanel(settings_page, icons_dir+"settings.png", wxBITMAP_TYPE_PNG, 26);
    settings_pg_sizer->Add(settings_pg_icon, 20, wxEXPAND | wxALL, 10);
    settings_page->SetSizerAndFit(settings_pg_sizer);

    wxPanel* focus_page = new wxPanel(bottom_container);
    wxBoxSizer* focus_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    bottom_ctn_sizer->Add(focus_page, 0, wxEXPAND);

    wxPanel* focus_pg_line = new wxPanel(focus_page);
    focus_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    focus_pg_sizer->Add(focus_pg_line, 1, wxEXPAND);

    wxImagePanel* focus_pg_icon = new wxImagePanel(focus_page, icons_dir+"focus_mode.png", wxBITMAP_TYPE_PNG, 26);
    focus_pg_sizer->Add(focus_pg_icon, 20, wxEXPAND | wxALL, 10);
    focus_page->SetSizerAndFit(focus_pg_sizer);

    focus_page->Connect(wxID_ANY, wxEVT_MOTION, wxMouseEventHandler(SideNavigation::OnHover));
    focus_page->Connect(wxID_ANY, wxEVT_LEAVE_WINDOW, wxMouseEventHandler(SideNavigation::OnEndHover));

    bottom_container->SetSizerAndFit(bottom_ctn_sizer);

    sizer->Add(top_container, 1, wxEXPAND);
    sizer->Add(bottom_container, 0, wxEXPAND);
    this->SetSizerAndFit(sizer);
}

void SideNavigation::OnHover(wxMouseEvent& event) {
    auto target = ((wxPanel*)event.GetEventObject());

    if(target) {
        return;
        wxPanel* active_bar = new wxPanel(target);
        active_bar->SetBackgroundColour(wxColor(255, 0, 180));

        wxSizer* tgt_sizer = target->GetSizer();
        if(tgt_sizer) {
            tgt_sizer->Insert(0, active_bar, 1, wxEXPAND);
            target->Update();
            tgt_sizer->Layout();
        }
    }
}

void SideNavigation::OnEndHover(wxMouseEvent& event) {
    auto target = ((wxPanel*)event.GetEventObject());

    if(target) {
        auto tgt_active_bar = target->GetChildren()[0];
        if(tgt_active_bar) tgt_active_bar->Destroy();
    }
}