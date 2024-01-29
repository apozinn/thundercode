#include "tabs.hpp"

Tabs::Tabs(wxPanel *parent, wxWindowID ID) : wxPanel(parent, ID)
{
    auto background_color = Themes["dark"]["main"].template get<std::string>();

    SetBackgroundColour(wxColor(background_color));
    sizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel *tabs_controller = new wxPanel(this);
    wxBoxSizer *tabs_controller_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxVector<wxBitmap> arrow_left_bitmap;
    arrow_left_bitmap.push_back(wxBitmap(icons_dir + "arrow_left.png", wxBITMAP_TYPE_PNG));
    arrow_left = new wxStaticBitmap(tabs_controller, wxID_ANY, wxBitmapBundle::FromBitmaps(arrow_left_bitmap));
    // menu->Bind(wxEVT_LEFT_UP, &Tabs::OnMenu, this);

    tabs_controller_sizer->Add(arrow_left, 0, wxLEFT, 5);

    wxVector<wxBitmap> arrow_right_bitmap;
    arrow_right_bitmap.push_back(wxBitmap(icons_dir + "arrow_right.png", wxBITMAP_TYPE_PNG));
    arrow_right = new wxStaticBitmap(tabs_controller, wxID_ANY, wxBitmapBundle::FromBitmaps(arrow_right_bitmap));
    // menu->Bind(wxEVT_LEFT_UP, &Tabs::OnMenu, this);

    tabs_controller_sizer->Add(arrow_right, 0, wxRIGHT, 5);

    tabs_controller->SetSizerAndFit(tabs_controller_sizer);

    sizer->Add(tabs_controller, 0, wxALIGN_CENTER);

    tabs_container = new wxScrolled<wxPanel>(this, ID_TABS_CONTAINER);
    tabs_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);
    tabs_container->SetSizerAndFit(tabs_ctn_sizer);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(icons_dir + "menu_down.png", wxBITMAP_TYPE_PNG));
    menu = new wxStaticBitmap(this, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    menu->Bind(wxEVT_LEFT_UP, &Tabs::OnMenu, this);

    sizer->Add(tabs_container, 1, wxEXPAND | wxTOP | wxBOTTOM, 5);
    sizer->Add(menu, 0, wxALIGN_CENTER | wxRIGHT, 10);
    Add("ThunderCode", "initial_tab");

    SetSizerAndFit(sizer);
    tabs_container->SetScrollRate(20, 20);
    tabs_container->EnableScrolling(true, false);
    Bind(wxEVT_PAINT, &Tabs::OnPaint, this);
}

void Tabs::Add(wxString tab_name, wxString path)
{
    if (!IsShown())
        Show();
    bool exists = false;
    for (auto &a_tab : tabs_container->GetChildren())
    {
        if (a_tab->GetName() == path)
        {
            a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
            a_tab->SetBackgroundColour(wxColour(31, 31, 31));
            exists = true;
        }
        else
        {
            a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(55, 55, 55));
            a_tab->SetBackgroundColour(wxColour(41, 41, 41));
        }
    }
    if (exists)
        return;

    wxPanel *new_tab = new wxPanel(tabs_container);

    new_tab->SetName(path);
    new_tab->Bind(wxEVT_LEFT_UP, &Tabs::Select, this);
    wxBoxSizer *new_tab_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel *tab_infos = new wxPanel(new_tab);
    wxBoxSizer *tab_infos_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText *name = new wxStaticText(tab_infos, wxID_ANY, tab_name);
    name->SetName(path);
    name->SetFont(fontWithOtherSize(name, 17));
    name->Bind(wxEVT_LEFT_UP, &Tabs::Select, this);
    tab_infos_sizer->Add(name, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(wxBitmap(icons_dir + "close.png", wxBITMAP_TYPE_PNG)));
    wxStaticBitmap *close_icon = new wxStaticBitmap(tab_infos, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    close_icon->Bind(wxEVT_LEFT_UP, &Tabs::OnCloseTab, this);
    tab_infos_sizer->Add(close_icon, 0, wxALIGN_CENTER | wxRIGHT, 6);

    tab_infos->Bind(wxEVT_ENTER_WINDOW, &Tabs::OnEnterComp, this);
    tab_infos->Bind(wxEVT_LEAVE_WINDOW, &Tabs::OnLeaveComp, this);

    tab_infos->SetSizerAndFit(tab_infos_sizer);
    new_tab_sizer->Add(tab_infos, 1, wxEXPAND | wxTOP | wxBOTTOM, 6);

    wxPanel *active_bar = new wxPanel(new_tab);
    new_tab_sizer->Add(active_bar, 0, wxEXPAND);

    new_tab->SetSizerAndFit(new_tab_sizer);
    tabs_ctn_sizer->Add(new_tab, 0, wxALIGN_CENTER);
    current_openned_path = path;
    tabs_container->FitInside();
    tabs_container->Scroll(1000, 0);
    new_tab->Bind(wxEVT_PAINT, &Tabs::OnTabPaint, this);

    SetMinSize(wxSize(GetSize().GetWidth(), new_tab->GetSize().GetHeight() + 10));
}

void Tabs::Close(wxString tab_path)
{
    auto codeContainer = ((CodeContainer *)FindWindowByName(tab_path + "_codeContainer"));
    auto imgContainer = ((wxPanel *)FindWindowByLabel(tab_path + "_imgContainer"));
    auto fileContainer = ((FilesTree *)FindWindowById(ID_FILES_TREE));

    if (codeContainer)
        codeContainer->Destroy();
    if (imgContainer)
        imgContainer->Destroy();

    if (codeContainer || imgContainer && tabs_container)
    {
        for (auto &tab : tabs_container->GetChildren())
        {
            if (tab->GetName() == tab_path)
            {
                auto main_code = FindWindowById(ID_MAIN_CODE);
                if (tab_path == current_openned_path)
                {
                    auto prev_tab = tab->GetPrevSibling();
                    auto next_tab = tab->GetNextSibling();

                    if (prev_tab)
                    {
                        auto act_bar = prev_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
                        current_openned_path = prev_tab->GetName();
                    }
                    else if (next_tab)
                    {
                        auto act_bar = next_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
                        current_openned_path = next_tab->GetName();
                    }

                    auto file_ctn = FindWindowByLabel(current_openned_path + "_file_container");
                    if (file_ctn)
                    {
                        if (fileContainer)
                        {
                            if (!prev_tab && !next_tab)
                            {
                                fileContainer->selectedFile->SetBackgroundColour(wxColor(45, 45, 45));
                            }
                            else
                            {
                                fileContainer->selectedFile->SetBackgroundColour(wxColor(45, 45, 45));
                                fileContainer->SetSelectedFile(file_ctn);
                                file_ctn->SetBackgroundColour(wxColor(60, 60, 60));
                            }
                        }
                    }

                    auto other_codeContainer = ((CodeContainer *)FindWindowByName(current_openned_path + "_codeContainer"));
                    if (other_codeContainer)
                        other_codeContainer->Show();

                    auto new_imageContainer = FindWindowByLabel(current_openned_path + "_imgContainer");
                    if (new_imageContainer)
                        new_imageContainer->Show();

                    if (!prev_tab && !next_tab)
                    {
                        this->Hide();
                        FindWindowById(ID_EMPYT_WINDOW)->Show();
                        ((StatusBar *)FindWindowById(ID_STATUS_BAR))->ClearLabels();
                        fileContainer->selectedFile->SetBackgroundColour(wxColor(45, 45, 45));
                        fileContainer->selectedFile = NULL;
                    }
                }

                tab->Destroy();
                tabs_container->GetSizer()->Layout();
                tabs_container->FitInside();
                tabs_container->Update();
                main_code->GetSizer()->Layout();
                main_code->Update();
            }
        }
    }
}

void Tabs::CloseAll()
{
    auto main_code = FindWindowById(ID_MAIN_CODE);
    tabs_container->DestroyChildren();
    Hide();
    for (auto &&other_ct : main_code->GetChildren())
    {
        if (other_ct->GetId() != ID_TABS &&
            other_ct->GetId() != ID_EMPYT_WINDOW)
            other_ct->Destroy();
    }
    FindWindowById(ID_EMPYT_WINDOW)->Show();
}

void Tabs::Select(wxMouseEvent &event)
{
    auto this_tab = ((Tabs *)event.GetEventObject());
    wxString tab_path = this_tab->GetName();
    auto main_code = FindWindowById(ID_MAIN_CODE);

    if (tab_path == current_openned_path)
        return;
    current_openned_path = tab_path;

    for (auto &children : tabs_container->GetChildren())
    {
        if (children->GetName() == tab_path)
        {
            children->SetBackgroundColour(wxColour(31, 31, 31));
            children->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
        }
        else
        {
            children->GetChildren()[1]->SetBackgroundColour(wxColor(55, 55, 55));
            children->SetBackgroundColour(wxColour(20, 20, 20));
        }
    }

    auto codeContainer = ((CodeContainer *)FindWindowByName(tab_path + "_codeContainer"));
    auto imageContainer = ((wxStaticBitmap *)FindWindowByLabel(tab_path + "_imageContainer"));
    auto status_bar = ((StatusBar *)FindWindowById(ID_STATUS_BAR));

    for (auto &&other_ct : main_code->GetChildren())
    {
        if (other_ct->GetId() != ID_TABS)
            other_ct->Hide();
    }

    if (codeContainer)
    {
        codeContainer->Show();
        status_bar->UpdateComps(tab_path, "text", codeContainer->current_lang->name);
    }

    if (imageContainer)
    {
        imageContainer->Show();
        status_bar->UpdateComps(tab_path, "image", "img");
    }

    main_code->GetSizer()->Layout();
    main_code->Update();
}

void Tabs::OnCloseTab(wxMouseEvent &event)
{
    wxObject *obj = event.GetEventObject();
    auto this_tab = ((wxWindow *)obj)->GetGrandParent();
    if (this_tab)
        Tabs::Close(this_tab->GetName());
}

void Tabs::OnMenu(wxMouseEvent &event)
{
    wxMenu *tabsMenu = new wxMenu;
    tabsMenu->Append(ID_CLOSE_ALL_FILES, _("&Close All"));
    tabsMenu->Append(wxID_ANY, _("&First Tab"));
    tabsMenu->Append(wxID_ANY, _("&Last Tab"));
    tabsMenu->Append(wxID_ANY, _("&Close Saved"));
    PopupMenu(tabsMenu);
}

void Tabs::OnEnterComp(wxMouseEvent &event)
{
    auto target = ((wxWindow *)event.GetEventObject());
    auto codeContainer = ((wxStyledTextCtrl *)FindWindowByName(target->GetParent()->GetName() + "_codeContainer"));
    if (target && codeContainer)
    {
        auto icon = ((wxStaticBitmap *)target->GetChildren()[1]);
        if (icon)
        {
            auto icon = ((wxStaticBitmap *)target->GetChildren()[1]);
            if (icon)
            {
                auto codeEditor = ((wxStyledTextCtrl *)codeContainer->GetChildren()[0]);
                if (codeEditor->GetModify())
                {
                    icon->Show();
                    icon->SetBitmap(wxBitmapBundle::FromBitmap(wxBitmap(icons_dir + "close.png")));
                }
            }
        }
    }
}

void Tabs::OnLeaveComp(wxMouseEvent &event)
{
    auto target = ((wxWindow *)event.GetEventObject());
    if (target)
    {
        auto codeContainer = ((wxWindow *)FindWindowByName(target->GetParent()->GetName() + "_codeContainer"));
        wxStaticBitmap *icon = ((wxStaticBitmap *)target->GetChildren()[1]);
        if (codeContainer)
        {
            auto codeEditor = ((wxStyledTextCtrl *)codeContainer->GetChildren()[0]);
            if (codeEditor->GetModify())
            {
                icon->SetBitmap(wxBitmapBundle::FromBitmap(wxBitmap(icons_dir + "white_circle.png")));
            }
            else
            {
                icon->SetBitmap(wxBitmapBundle::FromBitmap(wxBitmap(icons_dir + "close.png")));
            }
        }
    }
}

void Tabs::OnPaint(wxPaintEvent &event)
{
    wxClientDC dc(this);
    if (dc.IsOk())
    {
        auto border_color = Themes["dark"]["borderColor"].template get<std::string>();
        dc.SetPen(wxPen(wxColor(border_color), 0.20));
        dc.DrawLine(0, GetSize().GetHeight() - 1, GetSize().GetWidth(), GetSize().GetHeight() - 1);
        dc.DrawLine(0, GetSize().GetHeight(), 0, 0);
    }
}

void Tabs::OnTabPaint(wxPaintEvent &event)
{
    auto target = ((wxPanel *)event.GetEventObject());
    wxPaintDC dc(target);
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    auto background = Themes["dark"]["highlight"].template get<std::string>();

    if (gc)
    {
        gc->SetPen(wxColor(background));
        gc->SetBrush(wxColor(background));
        gc->DrawRoundedRectangle(0.0, 0.0, static_cast<double>(target->GetSize().GetWidth()), static_cast<double>(target->GetSize().GetHeight()), 10);
        delete gc;
    }
}