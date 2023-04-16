#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
    #include "wx/textdlg.h"
#endif

#include "wx/file.h"
#include "wx/filename.h"
#include "wx/settings.h"
#include "code.hpp"

wxBEGIN_EVENT_TABLE(CodeContainer, wxStyledTextCtrl)
wxEND_EVENT_TABLE()

enum
{
    MARGIN_LINE_NUMBERS
};

CodeContainer::CodeContainer(wxPanel* parent, wxWindowID ID) : wxStyledTextCtrl(parent, ID) 
{
	this->SetUseTabs(true);
    this->SetTabWidth(4);
    this->SetIndent(4);
    this->SetTabIndents(true);
    this->SetBackSpaceUnIndents(true);
    this->SetViewEOL(false);
    this->SetViewWhiteSpace(false);
    this->SetIndentationGuides(true);

    this->SetMarginWidth(0, this->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_99999")));
    this->SetCaretForeground(wxColour(wxT("WHITE")));

    this->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(70, 70, 70));
    this->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));

	wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
    this->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    this->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

    this->StyleClearAll();

    this->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(45, 120, 210));
    this->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(70, 70, 70));
}

void CodeContainer::OnSave(wxCommandEvent& WXUNUSED(event)) {
	auto codeContainer = ((CodeContainer*)FindWindowById(ID_CODE_CONTAINER));
	if(codeContainer) {
	    wxString filename = codeContainer->GetFilename();
	    if(filename.size()) {
	        codeContainer->SaveFile(filename);
	    } else {
	         wxMessageBox(_("Filename dont finded"), _("Error in save"), 
	            wxOK | wxICON_INFORMATION, this);
	    }
	}
}