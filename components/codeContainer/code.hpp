#pragma once
#include <wx/stc/stc.h>
#include "../statusBar/status.hpp"

class CodeContainer : public wxStyledTextCtrl {
    wxStyledTextCtrl* m_activeSTC;
public:
    CodeContainer(wxWindow* parent, wxWindowID ID, wxString path);
    void OnSave(wxCommandEvent& event);
    void LoadNewFile(wxString path) {
        if(!this->IsShown()) this->Show();
        this->LoadFile(path);
        this->SelectNone();
        SetFilename(path);
        
        if(FindWindowById(ID_STTSBAR_CODELOCALE)) {
            ((wxStaticText*)FindWindowById(ID_STTSBAR_CODELOCALE))->SetLabel(
                "Line "+std::to_string(GetCurrentLine())+", Column "+std::to_string(GetColumn(GetCurrentPos()))
            );
        }

        if(auto mainFrame = ((wxFrame*)FindWindowById(ID_STATUS_BAR)->GetParent())) {
            mainFrame->SetTitle(wxFileNameFromPath(path) + " (" + project_name + ") - ThunderCode");
        }
    }
    void SetFilename(wxString n_filename) {filename = n_filename;};
    wxString GetFilename() {return filename;};
    void onMarginClick(wxStyledTextEvent& event);
    void OnStyleNeeded(wxStyledTextEvent& event);
    void highlightSTCsyntax(size_t fromPos,size_t toPos,wxString &text);
    void setfoldlevels(size_t fromPos,int startfoldlevel,wxString &text);
	void OnChange(wxStyledTextEvent& event);
    void CharAdd(wxStyledTextEvent& event);
    void onClick(wxMouseEvent& event);
    void OnArrowsPress(wxKeyEvent& event);
private:
    wxString filename;
    wxDECLARE_NO_COPY_CLASS(CodeContainer);
    wxDECLARE_EVENT_TABLE();
};