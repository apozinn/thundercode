#include "code.hpp"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/settings.h"
#include "wx/wxprec.h"
#include "../../utils/randoms.hpp"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/textdlg.h"
#endif

wxBEGIN_EVENT_TABLE(CodeContainer, wxStyledTextCtrl)
wxEND_EVENT_TABLE()

CodeContainer::CodeContainer(
    wxWindow* parent, wxWindowID ID, wxString path
) : wxStyledTextCtrl(parent, ID) {
    SetLabel(path+"_codeContainer");
    SetName(path);
	SetUseTabs(true);
    SetTabWidth(4);
    SetIndent(4);
    SetTabIndents(true);
    SetBackSpaceUnIndents(true);
    SetIndentationGuides(true);

    #ifdef __WXGTK__
        font = wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        font.SetFaceName(wxT("Monospace"));
    #else
        font = wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    #endif

    StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(55, 55, 55));
    StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));
    StyleSetFont(wxSTC_STYLE_DEFAULT, font);

    StyleClearAll();
    SetCaretForeground(wxColour(wxT("WHITE")));

    //margin numbers
    SetMarginWidth(0, TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_99999")));
    SetMarginType(0, wxSTC_MARGIN_NUMBER);
    StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(128, 128, 128));
    StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(55, 55, 55));

    //margin fold
    SetMarginWidth(1, 16);
    SetMarginType(1, wxSTC_MARGIN_SYMBOL);
    SetMarginMask(1, wxSTC_MASK_FOLDERS);
    SetMarginSensitive(1, true);

    SetLexer(wxSTC_LEX_CPP);

    SetProperty(wxT("fold"), wxT("1"));
    SetProperty(wxT("fold.comment"), wxT("1"));
    SetProperty(wxT("fold.compact"), wxT("1"));
    SetProperty(wxT("fold.preprocessor"), wxT("1"));
    SetProperty(wxT("fold.html"), wxT("1"));
    SetProperty(wxT("fold.html.preprocessor"), wxT("1"));
    SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    SetMarginMask(2, wxSTC_MASK_FOLDERS);
    SetFoldMarginColour(true, wxColor(55,55,55));
    SetFoldMarginHiColour(true, wxColor(55,55,55));

    MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDER, wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDER, wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, wxColor(128,128,128));    

    MarkerEnableHighlight(false);
    SetMarginSensitive(2, true);

    SetFoldLevel(0, 1024);
    SetFoldLevel(1, 1024);
    SetFoldLevel(2, 1024|wxSTC_FOLDLEVELHEADERFLAG);
    SetFoldLevel(3, 1025);
    SetFoldLevel(4, 1025);
    SetFoldLevel(5, 1024);
    SetFoldLevel(6, 1024|wxSTC_FOLDLEVELWHITEFLAG);

    Bind(wxEVT_STC_MARGINCLICK, &CodeContainer::onMarginClick, this);
    Bind(wxEVT_STC_MODIFIED, &CodeContainer::OnChange, this);
    Bind(wxEVT_STC_CHARADDED, &CodeContainer::CharAdd, this);
    Bind(wxEVT_LEFT_UP, &CodeContainer::onClick, this);
    Bind(wxEVT_KEY_UP, &CodeContainer::OnArrowsPress, this);

    LoadPath(path);
    RegisterImage(0, wxBitmap(icons_dir+"thunder.png"));
    RegisterImage(1, wxBitmap(icons_dir+"question.png"));
}

void CodeContainer::OnSave(wxCommandEvent& event) {
    for(auto&& children : FindWindowById(ID_MAIN_CODE)->GetChildren()) {
        if(children->GetLabel().find("_CodeContainer") != std::string::npos && children->IsShown()) {
            CodeContainer* children_ct = ((CodeContainer*)children);
            wxString file_path = children->GetName();
            if(file_path.size()) {
                children_ct->SaveFile(file_path);

                for(auto&& tab : FindWindowById(ID_TABS_CONTAINER)->GetChildren()) {
                    if(tab->GetName() == file_path) {
                        auto close_ico = tab->GetChildren()[0]->GetChildren()[1];
                        auto unsaved_ico = tab->GetChildren()[0]->GetChildren()[2];

                        if(close_ico) close_ico->Show();
                        if(unsaved_ico) unsaved_ico->Hide();

                        tab->GetSizer()->Layout();
                        tab->Update();
                    }
                }
            } else {
               wxMessageBox(_("Filename or path don't find"), _("Error in save"), wxOK | wxICON_INFORMATION, this);
            }
        }
    }
}

void CodeContainer::onMarginClick(wxStyledTextEvent& event) {
    if (event.GetMargin() == 1) {
        int lineClick = LineFromPosition(event.GetPosition());
        int levelClick = GetFoldLevel(lineClick);

        if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
            ToggleFold(lineClick);
        }
    }
}

void CodeContainer::OnChange(wxStyledTextEvent& event) {
    wxString key = event.GetText();
    if(GetModify()) {
        auto tabs = FindWindowById(ID_TABS_CONTAINER);
        if(tabs) {
            for(auto&& tab : tabs->GetChildren()) {
                if(tab->GetName() == GetFilename()) {
                    auto close_ico = tab->GetChildren()[0]->GetChildren()[1];
                    auto unsaved_ico = tab->GetChildren()[0]->GetChildren()[2];

                    if(close_ico) close_ico->Hide();
                    if(unsaved_ico) unsaved_ico->Show();

                    tab->GetSizer()->Layout();
                    tab->Update();
                }
            }
        }
    }
    status_bar->UpdateCodeLocale(this);
}

void CodeContainer::OnArrowsPress(wxKeyEvent& event) {
    status_bar->UpdateCodeLocale(this);
}

void CodeContainer::CharAdd(wxStyledTextEvent& event) {
    char chr = (char)event.GetKey();
    int currentLine = GetCurrentLine();
	char previous_char = (char)GetCharAt(GetCurrentPos()-2);

    if (chr == '\n') {
        int currentLineInd;
        int nextLineInd;

        if(currentLine > 0) {
            currentLineInd = GetLineIndentation(currentLine-1);
            nextLineInd = GetLineIndentation(currentLine+1);
        }

        if(previous_char == '{' && !nextLineInd) {
            SetLineIndentation(currentLine, currentLineInd+GetIndent());
            GotoPos(GetCurrentPos() + currentLineInd+GetIndent()-3);
            InsertText(GetCurrentPos(), "\n");
        } else {
            if(currentLineInd && nextLineInd) {
                if(currentLineInd == nextLineInd) {
                    SetLineIndentation(currentLine, currentLineInd);
                    GotoPos(PositionFromLine(currentLine) + currentLineInd-3);
                } else if(nextLineInd > currentLineInd) {
                    SetLineIndentation(currentLine, nextLineInd);
                    GotoPos(PositionFromLine(currentLine) + nextLineInd-3);
                }
            }
        }

    } 

    if (chr == '#') {
        wxString s = "define?0 elif?1 else?0 endif?01 error?0 if?0 ifdef?0 "
                     "ifndef?1 include?0 line?0 pragma?1 undef?0";
        AutoCompShow(0, s);
    }

    if(chr == 'i') {
        wxString s = "import?1 if?0";
            AutoCompShow(0, s);
    }

    if(chr == '(') InsertText(GetCurrentPos(), ")");
    if(chr == '{') InsertText(GetCurrentPos(), "}");
    if(chr == '"') InsertText(GetCurrentPos(), "\"");
    if(chr == '`') InsertText(GetCurrentPos(), "`");
    if(chr == '[') InsertText(GetCurrentPos(), "]");
    if(event.GetKey() == 39) InsertText(GetCurrentPos(), "'");
}

void CodeContainer::onClick(wxMouseEvent& event) {
    status_bar->UpdateCodeLocale(this);
    event.Skip();
}

void CodeContainer::LoadPath(wxString path) {
    if(!IsShown()) Show();
    SelectNone();
    LoadFile(path);
        
    status_bar->UpdateCodeLocale(this);
    ((wxFrame*)FindWindowById(ID_STATUS_BAR)->GetParent())->SetTitle(
        wxFileNameFromPath(path) + " (" + project_name + ") - ThunderCodeContainer"
    );

    InitializePrefs(DeterminePrefs(path));
}

wxString CodeContainer::DeterminePrefs(const wxString &filename) {
    LanguageInfo const* currentInfo;
    int languageNr;
    for(languageNr = 0; languageNr < languages_prefs_size; languageNr++) {
        currentInfo = &languages_prefs[languageNr];
        wxString filepattern = currentInfo->filepattern;
        filepattern.Lower();
        while(!filepattern.empty()) {
            wxString cur = filepattern.BeforeFirst (';');
            if ((cur == filename) ||
                (cur == (filename.BeforeLast ('.') + ".*")) ||
                (cur == ("*." + filename.AfterLast ('.')))) {
                return currentInfo->name;
            }
            filepattern = filepattern.AfterFirst (';');
        }
    }
    return wxEmptyString;
}

bool CodeContainer::InitializePrefs(const wxString &name) {
    LanguageInfo const* currentInfo = nullptr;
    bool found = false;
    int languageNr;
    for (languageNr = 0; languageNr < languages_prefs_size; languageNr++) {
        currentInfo = &languages_prefs[languageNr];
        if (currentInfo->name == name) {
            found = true;
            break;
        }
    }
    if(!found) return false;

    current_lang = currentInfo;
    SetLexer(currentInfo->lexer);

    int Nr;
    for (Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
        StyleSetFont(Nr, font);
    }

    if(global_commonPrefs.syntaxEnable) {
        int keywordnr = 0;
        for(Nr = 0; Nr < STYLE_TYPES_COUNT; Nr++) {
            if (currentInfo->styles[Nr].type == -1) continue;

            const StyleInfo &curType = global_lexer_styles[currentInfo->styles[Nr].type];
            StyleSetFont(Nr, font);

            StyleSetForeground(Nr, wxColor(curType.foreground));
            StyleSetBold(Nr, (curType.fontstyle & mySTC_STYLE_BOLD) > 0);
            StyleSetItalic(Nr, (curType.fontstyle & mySTC_STYLE_ITALIC) > 0);
            StyleSetUnderline(Nr, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0);
            StyleSetVisible(Nr, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0);
            StyleSetCase(Nr, curType.lettercase);

            const char *pwords = currentInfo->styles[Nr].words;
            if (pwords) {
                SetKeyWords(keywordnr, pwords);
                keywordnr += 1;
            }
        }
    }

    return true;
}