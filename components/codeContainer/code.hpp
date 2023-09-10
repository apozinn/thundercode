#pragma once
#include <wx/stc/stc.h>
#include "../statusBar/status.hpp"
#include "../../src/languagePrefs/prefs.cpp"
#include "../../src/lexerStyle/lexerStyle.cpp"

const int ANNOTATION_STYLE = wxSTC_STYLE_LASTPREDEFINED + 1;

class CodeContainer : public wxScrolled<wxPanel> {
public:
	wxStyledTextCtrl* codeEditor = nullptr;
	wxStyledTextCtrl* codeMap = nullptr;
	LanguageInfo const* current_lang;
	CodeContainer(wxWindow* parent, wxString path);
	bool LoadPath(wxString path);
	void OnSave(wxCommandEvent& event);
private:
	wxFont font;
	StatusBar* status_bar = ((StatusBar*)FindWindowById(ID_STATUS_BAR));
	wxPoint codeMapClickPoint = wxPoint(0, 0);
	wxString DeterminePrefs(wxString filename);
	bool InitializePrefs(wxString name);
	void CodeEditorInitPrefs();
	void CodeMapInitPrefs();
	void OnChange(wxStyledTextEvent& event);
	void CharAdd(wxStyledTextEvent& event);
	void OnMarginClick(wxStyledTextEvent& event);
	void OnClick(wxMouseEvent& event);
	void OnArrowsPress(wxKeyEvent& event);
	void OnScroll(wxStyledTextEvent& event);
	void OnToggleLineComment(wxCommandEvent& event);
	void OnMapClick(wxMouseEvent& event);
	void OnCodeMapPaint(wxPaintEvent& event);
	void OnPainted(wxStyledTextEvent& event);
	void OnCodeEditorScroll(wxStyledTextEvent& event);
	void OnCodeMapScroll(wxStyledTextEvent& event);
	void SyncMap(wxStyledTextCtrl* comp);
	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(CodeContainer, wxPanel)
wxEND_EVENT_TABLE()