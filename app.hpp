#pragma once

wxString stringToWxString(std::string str) {
    wxString r_str = str;
    return r_str;
}

wxFont fontWithOtherSize(wxStaticText* cmp, int size) {
    wxFont font = cmp->GetFont(); 
    font.SetPixelSize(wxSize(size, size));
    return font;
}