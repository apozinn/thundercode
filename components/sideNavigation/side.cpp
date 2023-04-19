#include "side.hpp"
#include "../../sidePages/search/search.cpp"
#include "../../sidePages/builder/builder.cpp"
#include "../../sidePages/extensions/extensions.cpp"
#include "../../sidePages/settings/settings.cpp"
#include "../../sidePages/focus/focus.cpp"

SideNavigation::SideNavigation(wxPanel* parent, wxWindowID ID) : wxPanel(parent, ID) 
{
    this->SetBackgroundColour(wxColor(36, 36, 36));
    sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* top_container = new wxPanel(this);
    wxBoxSizer* top_ctn_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* code_page = new wxPanel(top_container);
    code_page->SetName("code_page");
    wxBoxSizer* code_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    top_ctn_sizer->Add(code_page, 0, wxEXPAND);

    wxPanel* code_pg_line = new wxPanel(code_page);
    code_pg_line->SetBackgroundColour(wxColor(255, 255, 255));
    code_pg_sizer->Add(code_pg_line, 1, wxEXPAND);

    wxImagePanel* code_pg_icon = new wxImagePanel(code_page, icons_dir+"code_page.png", wxBITMAP_TYPE_PNG, 26);
    code_pg_icon->SetName("code_page");
    code_pg_sizer->Add(code_pg_icon, 20, wxEXPAND | wxALL, 10);
    code_page->SetSizerAndFit(code_pg_sizer);

    code_pg_icon->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);
    code_page->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);

    wxPanel* search_page = new wxPanel(top_container);
    search_page->SetName("search_page");
    wxBoxSizer* search_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    top_ctn_sizer->Add(search_page, 0, wxEXPAND);

    wxPanel* search_pg_line = new wxPanel(search_page);
    search_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    search_pg_sizer->Add(search_pg_line, 1, wxEXPAND);

    wxImagePanel* search_pg_icon = new wxImagePanel(search_page, icons_dir+"search.png", wxBITMAP_TYPE_PNG, 26);
    search_pg_icon->SetName("search_page");
    search_pg_sizer->Add(search_pg_icon, 20, wxEXPAND | wxALL, 10);
    search_page->SetSizerAndFit(search_pg_sizer);

    search_page->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);
    search_pg_icon->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);

    wxPanel* builder_page = new wxPanel(top_container);
    builder_page->SetName("builder_page");
    wxBoxSizer* builder_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    top_ctn_sizer->Add(builder_page, 0, wxEXPAND);

    wxPanel* builder_pg_line = new wxPanel(builder_page);
    builder_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    builder_pg_sizer->Add(builder_pg_line, 1, wxEXPAND);

    wxImagePanel* builder_pg_icon = new wxImagePanel(builder_page, icons_dir+"builder.png", wxBITMAP_TYPE_PNG, 26);
    builder_pg_icon->SetName("builder_page");
    builder_pg_sizer->Add(builder_pg_icon, 20, wxEXPAND | wxALL, 10);
    builder_page->SetSizerAndFit(builder_pg_sizer);

    builder_page->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);
    builder_pg_icon->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);
   
    wxPanel* extensions_page = new wxPanel(top_container);
    extensions_page->SetName("extensions_page");
    wxBoxSizer* extensions_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    top_ctn_sizer->Add(extensions_page, 0, wxEXPAND);

    wxPanel* extensions_pg_line = new wxPanel(extensions_page);
    extensions_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    extensions_pg_sizer->Add(extensions_pg_line, 1, wxEXPAND);

    wxImagePanel* extensions_pg_icon = new wxImagePanel(extensions_page, icons_dir+"extensions.png", wxBITMAP_TYPE_PNG, 26);
    extensions_pg_icon->SetName("extensions_page");
    extensions_pg_sizer->Add(extensions_pg_icon, 20, wxEXPAND | wxALL, 10);
    extensions_page->SetSizerAndFit(extensions_pg_sizer);

    extensions_page->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);
    extensions_pg_icon->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);
    
    top_container->SetSizerAndFit(top_ctn_sizer);
    
    wxPanel* bottom_container = new wxPanel(this);
    wxBoxSizer* bottom_ctn_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* settings_page = new wxPanel(bottom_container);
    settings_page->SetName("settings_page");
    wxBoxSizer* settings_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    bottom_ctn_sizer->Add(settings_page, 0, wxEXPAND);

    wxPanel* settings_pg_line = new wxPanel(settings_page);
    settings_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    settings_pg_sizer->Add(settings_pg_line, 1, wxEXPAND);

    wxImagePanel* settings_pg_icon = new wxImagePanel(settings_page, icons_dir+"settings.png", wxBITMAP_TYPE_PNG, 26);
    settings_pg_icon->SetName("settings_page");
    settings_pg_sizer->Add(settings_pg_icon, 20, wxEXPAND | wxALL, 10);
    settings_page->SetSizerAndFit(settings_pg_sizer);

    settings_page->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);
    settings_pg_icon->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);

    wxPanel* focus_page = new wxPanel(bottom_container);
    focus_page->SetName("focus_page");
    wxBoxSizer* focus_pg_sizer = new wxBoxSizer(wxHORIZONTAL);
    bottom_ctn_sizer->Add(focus_page, 0, wxEXPAND);

    wxPanel* focus_pg_line = new wxPanel(focus_page);
    focus_pg_line->SetBackgroundColour(wxColor(36, 36, 36));
    focus_pg_sizer->Add(focus_pg_line, 1, wxEXPAND);

    wxImagePanel* focus_pg_icon = new wxImagePanel(focus_page, icons_dir+"focus_mode.png", wxBITMAP_TYPE_PNG, 26);
    focus_pg_icon->SetName("focus_page");
    focus_pg_sizer->Add(focus_pg_icon, 20, wxEXPAND | wxALL, 10);
    focus_page->SetSizerAndFit(focus_pg_sizer);

    focus_page->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);
    focus_pg_icon->Bind(wxEVT_LEFT_UP, &SideNavigation::SelectPage, this);

    bottom_container->SetSizerAndFit(bottom_ctn_sizer);

    sizer->Add(top_container, 1, wxEXPAND);
    sizer->Add(bottom_container, 0, wxEXPAND);
    this->SetSizerAndFit(sizer);
}

void SideNavigation::SelectPage(wxMouseEvent& event) {
    auto target = ((wxPanel*)event.GetEventObject());
    if(target) {
        wxString page_name = target->GetName();

        if(page_name.size() && page_name != "panel") {
            auto side_bar = FindWindowById(ID_FILES_TREE);
            auto side_bar_sizer = side_bar->GetSizer();
            if(!side_bar) {
                return;
            }

            for(auto& children : side_bar->GetChildren()) {
                children->Hide();
            }

            if(page_name == "code_page") {
                FindWindowById(ID_TRIPLE_A)->Show();
            }

            if(page_name == "search_page") {
                if(FindWindowById(ID_SEARCH_PAGE_INPUT)) {
                    auto input = FindWindowById(ID_SEARCH_PAGE_INPUT);
                    input->Show();
                } else {
                    SidePage_Search* search_page = new SidePage_Search(side_bar, ID_SEARCH_PAGE_INPUT);
                    side_bar_sizer->Add(search_page, 1, wxEXPAND | wxLEFT, 5);
                    side_bar->Update();
                    side_bar_sizer->Layout();
                }
            }

            if(page_name == "builder_page") {
                if(FindWindowById(ID_BUILDER_PAGE)) {
                    auto builder_pg = FindWindowById(ID_BUILDER_PAGE);
                    builder_pg->Show();
                } else {
                    SidePage_Builder* builder_page = new SidePage_Builder(side_bar, ID_BUILDER_PAGE);
                    side_bar_sizer->Add(builder_page, 1, wxEXPAND | wxLEFT, 5);
                    side_bar->Update();
                    side_bar_sizer->Layout();
                }
            }

            if(page_name == "extensions_page") {
                if(FindWindowById(ID_EXTENSIONS_PAGE)) {
                    auto extensions_pg = FindWindowById(ID_EXTENSIONS_PAGE);
                    extensions_pg->Show();
                } else {
                    SidePage_Extensions* extensions_page = new SidePage_Extensions(side_bar, ID_EXTENSIONS_PAGE);
                    side_bar_sizer->Add(extensions_page, 1, wxEXPAND | wxLEFT, 5);
                    side_bar->Update();
                    side_bar_sizer->Layout();
                }
            }

            if(page_name == "settings_page") {
                if(FindWindowById(ID_SETTINGS_PAGE)) {
                    auto settings_pg = FindWindowById(ID_SETTINGS_PAGE);
                    settings_pg->Show();
                } else {
                    SidePage_Settings* settings_page = new SidePage_Settings(side_bar, ID_SETTINGS_PAGE);
                    side_bar_sizer->Add(settings_page, 1, wxEXPAND | wxLEFT, 5);
                    side_bar->Update();
                    side_bar_sizer->Layout();
                }
            }

            if(page_name == "focus_page") {
                if(FindWindowById(ID_FOCUS_PAGE)) {
                    auto focus_pg = FindWindowById(ID_FOCUS_PAGE);
                    focus_pg->Show();
                } else {
                    SidePage_Focus* focus_page = new SidePage_Focus(side_bar, ID_FOCUS_PAGE);
                    side_bar_sizer->Add(focus_page, 1, wxEXPAND | wxLEFT, 5);
                    side_bar->Update();
                    side_bar_sizer->Layout();
                }
            }

            for(
                auto& children : FindWindowById(ID_SIDE_NAVIGATION)->GetChildren()
            ) {
                for(
                    auto& c_ : children->GetChildren()
                ) {
                    if(c_->GetName() != page_name) {
                        c_->GetChildren()[0]->SetBackgroundColour(wxColor(36, 36, 36));
                    } else {
                        c_->GetChildren()[0]->SetBackgroundColour(wxColor(255, 255, 255));
                    }
                }   
            }
        }
    }
}

void SideNavigation::OnHover(wxMouseEvent& event) {
    auto target = ((wxPanel*)event.GetEventObject());

    if(target) {}
}

void SideNavigation::OnEndHover(wxMouseEvent& event) {
    auto target = ((wxPanel*)event.GetEventObject());

    if(target) {}
}