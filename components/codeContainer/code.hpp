#pragma once
#include <wx/stc/stc.h>

class CodeContainer : public wxStyledTextCtrl {
public:
    CodeContainer(wxPanel* parent, wxWindowID ID);
    void OnSave(wxCommandEvent& event);
    void LoadNewFile(wxString file_path) {
        this->LoadFile(file_path);
        this->SelectNone();
        this->SetLexer(wxSTC_LEX_CPP);
        SetFilename(file_path);
        if(!this->IsShown()) this->Show();
    }
    void SetFilename(wxString n_filename) {filename = n_filename;};
    wxString GetFilename() {return filename;};
private:
    wxString filename;
    wxDECLARE_NO_COPY_CLASS(CodeContainer);
    wxDECLARE_EVENT_TABLE();
};
