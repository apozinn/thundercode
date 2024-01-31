#include "./files.hpp"

#include <vector>
#include <wx/scrolwin.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/statline.h>
#include <wx/app.h>
#include <wx/statbmp.h>
#include "../../main.hpp"

#include "../../utils/randoms.hpp"
#include "../../members/imagePanel.cpp"
#include "../statusBar/status.hpp"
#include "../codeContainer/code.hpp"
#include "../tabs/tabs.hpp"
#include <wx/graphics.h>
#include "./navigationButtons.cpp"
#include "searchFiles.cpp"

FilesTree::FilesTree(wxWindow *parent, wxWindowID ID) : wxPanel(parent, ID)
{
    auto background_color = Themes["dark"]["main"].template get<std::string>();
    SetBackgroundColour(wxColor(background_color));
    sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel *top_content = new wxPanel(this, ID_FILES_TREE_TOP_CONTENT);
    wxBoxSizer *top_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

    NavigationButtons *navigationButtons = new NavigationButtons(top_content, wxColor(background_color));
    top_ctn_sizer->Add(navigationButtons, 1, wxEXPAND);
    top_content->Bind(wxEVT_PAINT, &FilesTree::OnPaint, this);

    top_content->SetSizerAndFit(top_ctn_sizer);
    sizer->Add(top_content, 0, wxEXPAND | wxTOP, 8);

    SearchFiles *searchFiles = new SearchFiles(this, ID_SEARCH_FILES, wxColor(background_color));

    sizer->Add(searchFiles, 0, wxEXPAND | wxTOP, 10);
    searchFiles->Bind(wxEVT_PAINT, &FilesTree::OnPaint, this);

    wxPanel *project_files = new wxPanel(this, ID_PROJECT_FILES);
    wxBoxSizer *pjt_files_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel *project_tools = new wxPanel(project_files, ID_PROJECT_TOOLS_BAR);
    project_tools->Bind(wxEVT_LEFT_UP, &FilesTree::ToggleDir, this);
    pjt_files_sizer->Add(project_tools, 0, wxEXPAND | wxTOP | wxLEFT, 5);
    wxBoxSizer *pjt_tools_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(icons_dir + "dir_arrow.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap *pjt_arrow = new wxStaticBitmap(project_tools, ID_PJT_TOOLS_ARROW, wxBitmapBundle::FromBitmaps(bitmaps));
    pjt_tools_sizer->Add(pjt_arrow, 0, wxEXPAND | wxBOTTOM, 2);

    wxStaticText *pjt_name = new wxStaticText(project_tools, ID_PJT_TOOLS_PJTNAME, project_name);
    pjt_name->Bind(wxEVT_LEFT_UP, &FilesTree::ToggleDir, this);
    pjt_name->SetFont(fontWithOtherSize(pjt_name, 16));
    pjt_tools_sizer->Add(pjt_name, 1, wxEXPAND | wxLEFT, 4);
    project_tools->SetSizerAndFit(pjt_tools_sizer);

    project_files_ctn = new wxScrolled<wxPanel>(project_files, ID_PROJECT_FILES_CTN);
    pjt_files_sizer->Add(project_files_ctn, 1, wxEXPAND);
    auto pjt_files_ctn_sizer = new wxBoxSizer(wxVERTICAL);
    project_files_ctn->SetSizerAndFit(pjt_files_ctn_sizer);
    project_files->SetSizerAndFit(pjt_files_sizer);

    sizer->Add(project_files, 1, wxEXPAND);
    SetSizerAndFit(sizer);
    if (!project_path.size())
        pjt_arrow->Hide();

    Bind(wxEVT_PAINT, &FilesTree::OnPaint, this);
}

void FilesTree::Update()
{
    project_files_ctn->DestroyChildren();
    project_files_ctn->SetName(project_path);
    if (auto project_name_comp = FindWindowById(ID_PJT_TOOLS_PJTNAME))
        project_name_comp->SetLabel(project_name);
    if (auto project_arrow = FindWindowById(ID_PJT_TOOLS_ARROW))
        project_arrow->Show();

    Create(project_path.ToStdString(), project_files_ctn);

    project_files_ctn->FitInside();
    project_files_ctn->SetScrollRate(20, 20);
    selectedFile = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(0, 0));
}

void FilesTree::Create(std::string path, wxWindow *parent)
{
    std::vector<std::string> folders_buffer;
    std::vector<std::string> files_buffer;

    fileManager->ListChildrens(path, [&](const std::string &path, const std::string &type, const std::string &name)
                               {
        if(type == "dir") {
            folders_buffer.push_back(name);
        } else files_buffer.push_back(name); });

    std::sort(folders_buffer.begin(), folders_buffer.end());
    std::sort(files_buffer.begin(), files_buffer.end());

    for (auto &&f_ : folders_buffer)
    {
        fileManager->ListChildrens(path, [&](const std::string &path, const std::string &type, const std::string &name)
                                   {
            if(type == "dir") {
                if(name == f_) CreateDir(parent, name, path);
            } });
    }

    for (auto &&f_ : files_buffer)
    {
        fileManager->ListChildrens(path, [&](const std::string &path, const std::string &type, const std::string &name)
                                   {
            if(type == "file") {
                if(name == f_) CreateFile(parent, name, path);
            } });
    }
}

void FilesTree::CreateFile(
    wxWindow *parent, wxString name, wxString path)
{
    if (!parent || FindWindowByLabel(path + "_file_container"))
        return;
    auto parent_sizer = parent->GetSizer();
    if (!parent_sizer)
    {
        auto new_sizer = new wxBoxSizer(wxVERTICAL);
        parent->SetSizerAndFit(new_sizer);
        parent_sizer = new_sizer;
    }

    wxPanel *file_container = new wxPanel(parent);

    file_container->Bind(wxEVT_ENTER_WINDOW, &FilesTree::OnEnterComp, this);
    file_container->Bind(wxEVT_LEAVE_WINDOW, &FilesTree::OnLeaveComp, this);

    file_container->Bind(wxEVT_RIGHT_UP, &FilesTree::onFileRightClick, this);
    file_container->SetMinSize(wxSize(file_container->GetSize().GetWidth(), 20));
    file_container->SetSize(file_container->GetSize().GetWidth(), 20);
    file_container->SetName(path);
    file_container->SetLabel(path + "_file_container");
    file_container->Bind(wxEVT_LEFT_UP, &FilesTree::OnFileSelect, this);
    wxBoxSizer *file_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

    LanguageInfo const *currentLanguageInfo;
    LanguageInfo const *currentInfo;
    int languageNr;

    bool found;
    for (languageNr = 0; languageNr < languages_prefs_size; languageNr++)
    {
        currentInfo = &languages_prefs[languageNr];
        wxString filepattern = currentInfo->filepattern;
        filepattern.Lower();

        while (!filepattern.empty() && !found)
        {
            wxString cur = filepattern.BeforeFirst(';');
            if ((cur == path) ||
                (cur == (path.BeforeLast('.') + ".*")) ||
                (cur == ("*." + path.AfterLast('.'))))
            {
                found = true;
                currentLanguageInfo = currentInfo;
            }
            filepattern = filepattern.AfterFirst(';');
        }
    }

    if (!found)
        currentLanguageInfo = &languages_prefs[0];

    wxVector<wxBitmap> bitmaps;
    auto last_dot = path.find_last_of(".");
    if (last_dot != std::string::npos)
    {
        std::string file_ext = path.ToStdString().substr(last_dot + 1);
        if (file_ext.size() && file_ext != path)
        {
            if (file_ext == "png" || file_ext == "jpg" || file_ext == "jpeg")
            {
                bitmaps.push_back(wxBitmap(icons_dir + "file_ext" + "/image_ext.svg"));
            }
            else
            {
                bitmaps.push_back(wxBitmap(icons_dir + currentLanguageInfo->icon_path));
            }
        }
    } else bitmaps.push_back(wxBitmap(icons_dir + "/file_ext/no_ext.svg", wxBITMAP_TYPE_PNG));

    wxStaticBitmap *file_icon = new wxStaticBitmap(file_container, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    file_ctn_sizer->Add(file_icon, 0, wxALIGN_CENTRE_VERTICAL | wxLEFT, 8);

    wxStaticText *file_name = new wxStaticText(file_container, wxID_ANY, name);
    file_name->SetForegroundColour(wxColor(245, 245, 245));
    file_name->SetName(path);
    file_name->Bind(wxEVT_LEFT_UP, &FilesTree::OnFileSelect, this);
    file_name->Bind(wxEVT_RIGHT_UP, &FilesTree::onFileRightClick, this);
    file_name->SetFont(fontWithOtherSize(file_name, 18));
    file_ctn_sizer->Add(file_name, 0, wxALIGN_CENTRE_VERTICAL | wxLEFT, 5);

    file_container->SetSizerAndFit(file_ctn_sizer);
    parent_sizer->Add(file_container, 0, wxEXPAND | wxLEFT | wxTOP, 2);
}

void FilesTree::CreateDir(
    wxWindow *parent, wxString name, wxString path)
{
    if (!parent || FindWindowByLabel(path + "_dir_container"))
        return;
    auto parent_sizer = parent->GetSizer();
    if (!parent_sizer)
    {
        wxBoxSizer *n_s = new wxBoxSizer(wxVERTICAL);
        parent->SetSizerAndFit(n_s);
        parent_sizer = n_s;
    }

    wxPanel *dir_container = new wxPanel(parent);

    dir_container->Bind(wxEVT_ENTER_WINDOW, &FilesTree::OnEnterComp, this);
    dir_container->Bind(wxEVT_LEAVE_WINDOW, &FilesTree::OnLeaveComp, this);

    dir_container->SetMinSize(wxSize(dir_container->GetSize().GetWidth(), 20));
    dir_container->SetSize(dir_container->GetSize().GetWidth(), 20);
    dir_container->SetName(path);
    dir_container->SetLabel(path + "_dir_container");
    wxBoxSizer *dir_ctn_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel *dir_props = new wxPanel(dir_container);
    dir_props->SetLabel("dir_props");
    dir_props->Bind(wxEVT_LEFT_UP, &FilesTree::ToggleDir, this);
    dir_props->Bind(wxEVT_RIGHT_UP, &FilesTree::onDirRightClick, this);
    wxBoxSizer *props_sizer = new wxBoxSizer(wxHORIZONTAL);
    dir_ctn_sizer->Add(dir_props, 0, wxEXPAND | wxLEFT, 8);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(icons_dir + "dir_arrow.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap *dir_arrow = new wxStaticBitmap(dir_props, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    props_sizer->Add(dir_arrow, 0, wxEXPAND | wxTOP, 2);

    wxStaticText *dir_name = new wxStaticText(dir_props, wxID_ANY, name);
    dir_name->SetName("dir_name");
    dir_name->Bind(wxEVT_LEFT_UP, &FilesTree::ToggleDir, this);
    dir_name->Bind(wxEVT_RIGHT_UP, &FilesTree::onDirRightClick, this);
    dir_name->SetFont(fontWithOtherSize(dir_name, 18));
    props_sizer->Add(dir_name, 0, wxEXPAND | wxLEFT, 4);
    dir_props->SetSizerAndFit(props_sizer);

    wxPanel *dir_childrens = new wxPanel(dir_container);
    dir_childrens->Bind(wxEVT_PAINT, &FilesTree::OnPaint, ((FilesTree *)dir_childrens));
    wxBoxSizer *dir_childrens_sizer = new wxBoxSizer(wxVERTICAL);
    dir_childrens->SetSizerAndFit(dir_childrens_sizer);
    dir_ctn_sizer->Add(dir_childrens, 0, wxEXPAND | wxLEFT, 10);

    dir_container->SetSizerAndFit(dir_ctn_sizer);
    dir_childrens->Hide();
    parent_sizer->Add(dir_container, 0, wxEXPAND | wxLEFT, 2);
}

void FilesTree::OnFileSelect(wxMouseEvent &event)
{
    auto file = ((wxWindow *)event.GetEventObject());
    if (file->GetLabel().find("file_container") == std::string::npos)
        file = file->GetParent();
    wxString path = file->GetName();

    if (path.size())
    {
        OpenFile(path);
        if (selectedFile)
        {
            selectedFile->SetBackgroundColour(wxColor(45, 45, 45));
        }
        selectedFile = file;
        selectedFile->SetBackgroundColour(wxColor(60, 60, 60));
    }
}

void FilesTree::OpenFile(wxString path)
{
    auto main_code = FindWindowById(ID_MAIN_CODE);
    auto tabsContainer = ((Tabs *)FindWindowById(ID_TABS));
    auto codeContainer = ((CodeContainer *)FindWindowByName(path + "_codeContainer"));
    auto status_bar = ((StatusBar *)FindWindowById(ID_STATUS_BAR));

    for (auto &&other_ct : main_code->GetChildren())
        if (other_ct->GetId() != ID_TABS)
            other_ct->Hide();

    wxFileName file_props = wxFileName(path);
    wxString file_ext = file_props.GetExt();

    auto is_text_file = [&]()
    {
        if (!codeContainer)
        {
            codeContainer = new CodeContainer(main_code, path);
            main_code->GetSizer()->Add(codeContainer, 1, wxEXPAND);
        }
        else
            codeContainer->Show();

        status_bar->UpdateComps(path, "text", codeContainer->current_lang->name);
    };

    if (file_ext.size())
    {
        wxImage image = wxImage();
        if (image.CanRead(path))
        {
            image.LoadFile(path);
            if (!image.IsOk())
                return;
            if (image.GetWidth() > 1000 || image.GetHeight() > 1000)
            {
                image.Rescale(image.GetWidth() / 2, image.GetHeight() / 2);
            }
            wxVector<wxBitmap> bitmaps;
            bitmaps.push_back(wxBitmap(image));
            wxStaticBitmap *image_container = new wxStaticBitmap(main_code, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
            image_container->SetLabel(path + "_imageContainer");
            main_code->GetSizer()->Add(image_container, 1, wxALIGN_CENTER);
            status_bar->UpdateComps(path, "image", "img");
        }
        else
            is_text_file();
    }
    else
        is_text_file();

    tabsContainer->Add(wxFileNameFromPath(path.substr(0, path.size())), path);
}

void FilesTree::ToggleDir(wxMouseEvent &event)
{
    auto dir_container = ((wxWindow *)event.GetEventObject());
    if (dir_container->GetId() == ID_PROJECT_TOOLS_BAR || dir_container->GetId() == ID_PJT_TOOLS_PJTNAME)
    {
        dir_container = project_files_ctn;
    }

    if (dir_container == project_files_ctn)
    {
        auto dir_arrow_ctn = ((wxStaticBitmap *)FindWindowById(ID_PJT_TOOLS_ARROW));
        auto arrow_bit = dir_arrow_ctn->GetBitmap();
        wxVector<wxBitmap> bitmaps;

        if (dir_container->IsShown())
        {
            dir_container->Hide();
            bitmaps.push_back(wxBitmap(arrow_bit.ConvertToImage().Rotate90(true), -1));
        }
        else
        {
            dir_container->Show();
            bitmaps.push_back(wxBitmap(arrow_bit.ConvertToImage().Rotate90(false), -1));
        }

        dir_arrow_ctn->SetBitmap(wxBitmapBundle::FromBitmaps(bitmaps));
        return;
    }

    if (dir_container->GetLabel() == "dir_props")
    {
        dir_container = dir_container->GetParent();
    }
    else if (dir_container->GetName() == "dir_name")
    {
        dir_container = dir_container->GetGrandParent();
    }

    if (dir_container)
    {
        auto dir_arrow_ctn = ((wxStaticBitmap *)dir_container->GetChildren()[0]->GetChildren()[0]);
        auto dir_childrens = dir_container->GetChildren()[1];
        wxString path = dir_container->GetName();

        if (dir_childrens && dir_arrow_ctn)
        {
            auto arrow_bit = dir_arrow_ctn->GetBitmap();
            wxVector<wxBitmap> bitmaps;

            if (dir_childrens->IsShown())
            {
                dir_childrens->Hide();
                bitmaps.push_back(wxBitmap(arrow_bit.ConvertToImage().Rotate90(false), -1));
            }
            else
            {
                Create(path.ToStdString() + "/", dir_childrens);
                dir_childrens->Show();
                bitmaps.push_back(wxBitmap(arrow_bit.ConvertToImage().Rotate90(true), -1));
            }

            FitContainer(dir_childrens);

            dir_arrow_ctn->SetBitmap(wxBitmapBundle::FromBitmaps(bitmaps));
            dir_childrens->GetSizer()->Layout();
            dir_childrens->Update();
            dir_container->GetSizer()->Layout();
            dir_container->Update();
        }
    }
}

void FilesTree::onTopMenuClick(wxMouseEvent &event)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_ANY, _("&New File"));
    menuFile->Append(ID_CREATE_DIR, _("&New Folder"));
    menuFile->Append(wxID_ANY, _("&Open terminal"));
    PopupMenu(menuFile);
}

void FilesTree::onFileRightClick(wxMouseEvent &event)
{
    auto target = ((wxWindow *)event.GetEventObject());
    if (!target->GetName().size())
        return;
    menufile_path = target->GetName();

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_RENAME_FILE, _("&Rename"));
    menuFile->Append(ID_DELETE_FILE, _("&Delete File"));
    PopupMenu(menuFile);
}

void FilesTree::onDirRightClick(wxMouseEvent &event)
{
    auto eventObj = ((wxWindow *)event.GetEventObject());
    wxWindow *target;

    if (eventObj->GetLabel() == "dir_props")
    {
        target = eventObj->GetParent();
    }
    else if (eventObj->GetName() == "dir_name")
    {
        target = eventObj->GetGrandParent();
    }

    if (!target->GetName().size())
        return;
    menudir_path = target->GetName();

    wxMenu *menuDir = new wxMenu;
    menuDir->Append(ID_RENAME_DIR, _("&Rename"));
    menuDir->Append(ID_CREATE_FILE, _("&New File"));
    menuDir->Append(ID_CREATE_DIR, _("&New Folder"));
    menuDir->Append(wxID_ANY, _("&Open Folder"));
    menuDir->Append(ID_DELETE_DIR, _("&Delete Folder"));
    PopupMenu(menuDir);
}

void FilesTree::OnPaint(wxPaintEvent &event)
{
    auto target = ((wxPanel *)event.GetEventObject());
    wxClientDC dc(this);
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
    if (!gc)
        return;

    if (target->GetId() == ID_FILES_TREE)
    {
    }
    else if (target->GetId() == ID_SEARCH_FILES)
    {
        auto border_color = Themes["dark"]["borderColor"].template get<std::string>();
        dc.SetPen(wxPen(wxColor(border_color), 0.20));
        dc.DrawLine(0, target->GetPosition().y, target->GetSize().GetWidth(), target->GetPosition().y);
        dc.DrawLine(0, target->GetPosition().y + target->GetSize().GetHeight(), target->GetSize().GetWidth(), target->GetPosition().y + target->GetSize().GetHeight());
    }
    else if (target->GetId() == ID_FILES_TREE_TOP_CONTENT)
    {
        auto border_color = Themes["dark"]["borderColor"].template get<std::string>();
        dc.SetPen(wxPen(wxColor(border_color), 0.20));
        dc.DrawLine(target->GetSize().GetWidth(), target->GetSize().GetHeight() - 10, 0, target->GetSize().GetHeight() - 10);
    }
    else
    {
        gc->SetPen(gc->CreatePen(wxGraphicsPenInfo(wxColor(128, 128, 128)).Width(1.25).Style(wxPENSTYLE_DOT)));
        gc->SetBrush(wxColor(128, 128, 128));

        wxGraphicsPath path = gc->CreatePath();
        path.MoveToPoint(0.0, 0.0);
        path.AddLineToPoint(0.0, static_cast<double>(target->GetSize().GetHeight()));
        gc->StrokePath(path);
    }

    delete gc;
}

void FilesTree::FitContainer(wxWindow *window)
{
    auto next_parent = window;
    bool has_next_parent = true;
    while (has_next_parent)
    {
        int height = 0;
        for (auto &&children : next_parent->GetChildren())
        {
            if (children->IsShown())
                height = height + children->GetSize().GetHeight();
        }

        next_parent->SetSize(next_parent->GetSize().GetWidth(), height);
        next_parent->SetMinSize(wxSize(next_parent->GetSize().GetWidth(), height));

        if (next_parent->GetParent() && next_parent->GetId() != ID_PROJECT_FILES_CTN)
        {
            next_parent = next_parent->GetParent();
        }
        else
            has_next_parent = false;
    }
}

void FilesTree::OnCreateDir(wxCommandEvent &event)
{
    wxString folder_name = wxGetTextFromUser("Enter the folder name: ", "Create Folder", "");
    if (folder_name.empty())
        return;
    bool created = fileManager->CreateDir(menudir_path + "/" + folder_name);
}

void FilesTree::OnCreateFile(wxCommandEvent &event)
{
    wxString file_name = wxGetTextFromUser("Enter the file name: ", "Create File", "");
    if (file_name.empty())
        return;

    if (menudir_path.Len() > 1000)
    {
        if (project_path.Len() > 1000)
        {
            wxMessageBox(_("You can't create a file in a empty project"), _("Create File"), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            fileManager->CreateFile(project_path + file_name);
            OpenFile(project_path + file_name);
        }
    }
    else
    {
        fileManager->CreateFile(menudir_path + "/" + file_name);
        OpenFile(menudir_path + "/" + file_name);
    }
}

void FilesTree::OnDeleteDir(wxCommandEvent &event)
{
    bool deleted = fileManager->DeleteDir(menudir_path);
}

void FilesTree::OnDeleteFile(wxCommandEvent &event)
{
    bool deleted = fileManager->DeleteFile(menufile_path);
}

void FilesTree::OnTreeModifyed(wxString old_path, wxString new_path)
{
    wxFileName path_props(old_path);
    wxString parent_path;

    if (path_props.IsDir())
    {
        parent_path = path_props.GetPath().substr(0, path_props.GetPath().find_last_of("/"));
    }
    else
    {
        parent_path = path_props.GetPath();
    }

    auto parent = FindWindowByName(parent_path);
    if (parent_path + "/" == project_path)
        parent = project_files_ctn;
    if (parent)
    {
        if (parent->GetId() != ID_PROJECT_FILES_CTN)
            parent = parent->GetChildren()[1];
        parent->DestroyChildren();
        Create(parent_path.ToStdString() + "/", parent);
        FitContainer(parent);
    }
}

void FilesTree::OnEnterComp(wxMouseEvent &event)
{
    auto target = ((wxWindow *)event.GetEventObject());
    auto background_color = Themes["dark"]["main"].template get<std::string>();
    auto highlight = Themes["dark"]["highlight"].template get<std::string>();

    if (target)
    {
        if (target->GetGrandParent()->GetBackgroundColour() == wxColour(highlight))
        {
            target->GetGrandParent()->SetBackgroundColour(wxColor(background_color));
        }
        target->SetBackgroundColour(wxColor(highlight));
    }
}

void FilesTree::OnLeaveComp(wxMouseEvent &event)
{
    auto target = ((wxWindow *)event.GetEventObject());
    auto background_color = Themes["dark"]["main"].template get<std::string>();
    if (target)
    {
        if (selectedFile == target)
            return;
        target->SetBackgroundColour(wxColor(background_color));
    }
}

void FilesTree::OnFileRename(wxCommandEvent &event)
{
    wxString new_name = wxGetTextFromUser("Enter the new file name: ", "Rename File", "");
    if (new_name.empty())
        return;
    if (menufile_path.empty())
        return;

    wxString target_parent = menufile_path.substr(0, menufile_path.find_last_of("/") + 1);
    wxString new_path = target_parent + new_name;

    if (!wxRename(menufile_path, new_path))
    {
        wxMessageBox("There was an error renaming the file", "", wxOK | wxICON_INFORMATION);
    }
}

void FilesTree::OnDirRename(wxCommandEvent &event)
{
    wxString new_name = wxGetTextFromUser("Enter the new dir name: ", "Rename Dir", "");
    if (new_name.empty())
        return;
    if (menudir_path.empty())
        return;

    wxString target_parent = menudir_path.substr(0, menudir_path.find_last_of("/") + 1);
    wxString new_path = target_parent + new_name;

    if (!wxRename(menudir_path, new_path))
    {
        wxMessageBox("There was an error renaming the dir", "", wxOK | wxICON_INFORMATION);
    }
}