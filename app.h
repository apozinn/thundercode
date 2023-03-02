#pragma once

enum {
    ID_Quit = 1,
    ID_ABOUT,
    ID_CONFIG_BUTTON,
    ID_HIDDE_MENUTOOLS,
    ID_CODE_EDITOR_COMP,
    ID_CODE_BLOCK,
    ID_NEW_FILE,
    ID_TABS_CONTAINER,
    ID_TAB,
    ID_NAVIGATION_COMP,
    ID_OPEN_FOLDER,
    ID_PROJECT_NAME,
    ID_FILES_TREE_COMP,
    ID_SIDE_NAVIGATION,
};

wxString project_path;
std::string project_name;
wxString icons_dir;

wxString stringToWxString(std::string str) {
    wxString r_str = str;
    return r_str;
}