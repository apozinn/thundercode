#pragma once

#include <iostream>
#include <cstring>

wxString stringToWxString(std::string str) {
    wxString r_str = str;
    return r_str;
}

wxFont fontWithOtherSize(wxStaticText* cmp, int size) {
    wxFont font = cmp->GetFont(); 
    font.SetPixelSize(wxSize(size, size));
    return font;
}
 
const char *FormatBytes(long long bytes, char *str)
{
    const char *sizes[5] = { "B", "KB", "MB", "GB", "TB" };
    int i;
    double dblByte = bytes;
    for (i = 0; i < 5 && bytes >= 1024; i++, bytes /= 1024)
        dblByte = bytes / 1024.0;
 
    return strcat(strcat(str, " "), sizes[i]);
}