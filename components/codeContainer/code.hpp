#pragma once
#include <wx/stc/stc.h>
#include "../statusBar/status.hpp"
#include "../../src/languagePrefs/prefs.cpp"
#include "../../src/lexerStyle/lexerStyle.cpp"

const int ANNOTATION_STYLE = wxSTC_STYLE_LASTPREDEFINED + 1;

class CodeContainer : public wxStyledTextCtrl {
    wxStyledTextCtrl* m_activeSTC;
    wxFont font;
public:
    LanguageInfo const* current_lang;
    CodeContainer(wxWindow* parent, wxWindowID ID, wxString path);
    void OnSave(wxCommandEvent& event);
    void LoadPath(wxString path);
private:
    void SetFilename(wxString n_filename) {filename = n_filename;};
    wxString GetFilename() {return filename;};
	void OnChange(wxStyledTextEvent& event);
    void CharAdd(wxStyledTextEvent& event);
    void onMarginClick(wxStyledTextEvent& event);
    void onClick(wxMouseEvent& event);
    void OnArrowsPress(wxKeyEvent& event);
    wxString DeterminePrefs(const wxString& filename);
    bool InitializePrefs(const wxString& name);
    wxString filename;
    StatusBar* status_bar = ((StatusBar*)FindWindowById(ID_STATUS_BAR));
    wxDECLARE_NO_COPY_CLASS(CodeContainer);
    wxDECLARE_EVENT_TABLE();
};