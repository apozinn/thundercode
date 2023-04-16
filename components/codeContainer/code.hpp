#pragma once
#include <wx/stc/stc.h>

class CodeContainer : public wxStyledTextCtrl {
    wxStyledTextCtrl* m_activeSTC;
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
    void onMarginClick(wxStyledTextEvent& event);
    void OnStyleNeeded(wxStyledTextEvent& event);
    void highlightSTCsyntax(size_t fromPos,size_t toPos,wxString &text);
    void setfoldlevels(size_t fromPos,int startfoldlevel,wxString &text);
	void OnChange(wxStyledTextEvent& event);
    void CharAdd(wxStyledTextEvent& event);
private:
    wxColor m_GCodecolor{255,130,0};
    int m_stylemask=0;
    wxString filename;
    wxDECLARE_NO_COPY_CLASS(CodeContainer);
    wxDECLARE_EVENT_TABLE();
};
