#include <iostream>
#include <string>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <wx/fs_mem.h>
#include <wx/textdlg.h>
#include <wx/sysopt.h>
#include <wx/splitter.h>
#include <wx/socket.h>
#include <wx/aboutdlg.h>
#include <wx/utils.h>
#include <wx/nativewin.h>
#include <wx/process.h>
#include <wx/infobar.h>
#include <wx/log.h>
#include <wx/stc/stc.h>
#include "wx/image.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/mstream.h"
#include "wx/wfstream.h"
#include "wx/quantize.h"
#include "wx/stopwatch.h"
#include <wx/richtext/richtextctrl.h>
#include "../app.h"

enum
{
    MARGIN_LINE_NUMBERS
};


class CodeEditor : public wxPanel {
public: 
    CodeEditor(wxPanel *parent) : wxPanel(
        parent, ID_CODE_EDITOR_COMP, wxDefaultPosition, wxDefaultSize
    ) {
    	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        wxStyledTextCtrl* code_block = new wxStyledTextCtrl(
            this, ID_CODE_BLOCK, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString
        );

        code_block->SetUseTabs(true);
        code_block->SetTabWidth(4);
        code_block->SetIndent(4);
        code_block->SetTabIndents(true);
        code_block->SetBackSpaceUnIndents(true);
        code_block->SetViewEOL(false);
        code_block->SetViewWhiteSpace(false);
        code_block->SetIndentationGuides(true);
        code_block->SetReadOnly(false);

        code_block->SetMarginWidth(0, code_block->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_9999")));
        code_block->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
        code_block->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("BLACK")));
        code_block->SetSelBackground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
        code_block->SetSelForeground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));

        code_block->SetCaretForeground(wxColour(wxT("WHITE")));
        code_block->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(70, 70, 70));
        code_block->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));

        code_block->StyleClearAll();

        code_block->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(190, 180, 180));
        code_block->StyleSetBackground (wxSTC_STYLE_LINENUMBER, wxColor(70, 70, 70));
        code_block->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

        sizer->Add(code_block, 1, wxEXPAND);
        this->SetSizerAndFit(sizer);
    }

    void OnChangeCode(wxCommandEvent& event);
private:
    wxPanel *code_editor_comp;
};

void CodeEditor::OnChangeCode(wxCommandEvent& WXUNUSED(event)) {}
