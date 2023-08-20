#include "code.hpp"
#include "wx/settings.h"
#include "../../utils/randoms.hpp"

CodeContainer::CodeContainer(
	wxWindow* parent, wxString path
) : wxScrolled<wxPanel>(parent) {
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	codeEditor = new wxStyledTextCtrl(this);
	codeMap = new wxStyledTextCtrl(this);

	sizer->Add(codeEditor, 1, wxEXPAND);
	sizer->Add(codeMap, 0, wxEXPAND);
	SetSizerAndFit(sizer);

    font = wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    font.SetFaceName(wxT("Monospace"));

	LoadPath(path);
	SetName(path+"_codeContainer");
    SetLabel(path);
}

bool CodeContainer::LoadPath(wxString path) {
	wxFileName file_props(path);
	if(file_props.IsOk()) {
		if(file_props.FileExists()) {
			bool codeEditor_loadSuccessfully = codeEditor->LoadFile(path);
			bool codeMap_loadSuccessfully = codeMap->LoadFile(path);

			if(codeEditor_loadSuccessfully && codeMap_loadSuccessfully) {
				status_bar->UpdateCodeLocale(codeEditor);
			    ((wxFrame*)FindWindowById(ID_STATUS_BAR)->GetParent())->SetTitle(
			        wxFileNameFromPath(path) + " (" + project_name + ") - ThunderCode"
			    );

			    InitializePrefs(DeterminePrefs(path));
			    return true;
			}
		}
	}
	return false;
}

wxString CodeContainer::DeterminePrefs(wxString filename) {
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

bool CodeContainer::InitializePrefs(wxString name) {
	if(!codeEditor || !codeMap) return false;

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

    if(!found) {
        current_lang = &languages_prefs[0];
        return true;
    } else {
	    current_lang = currentInfo;
    	CodeEditorInitPrefs();
    	CodeMapInitPrefs();

    	int Nr;
	    for (Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
	        codeEditor->StyleSetFont(Nr, font);
	        codeMap->StyleSetFont(Nr, font);
	    }

	    if(global_commonPrefs.syntaxEnable) {
	        int keywordnr = 0;
	        for(Nr = 0; Nr < STYLE_TYPES_COUNT; Nr++) {
	            if (currentInfo->styles[Nr].type == -1) continue;

	            const StyleInfo &curType = global_lexer_styles[currentInfo->styles[Nr].type];
	            codeEditor->StyleSetFont(Nr, font);
	            codeMap->StyleSetFont(Nr, font);

	            codeEditor->StyleSetForeground(Nr, wxColor(curType.foreground));
	            codeMap->StyleSetForeground(Nr, wxColor(curType.foreground));
	            codeEditor->StyleSetBold(Nr, (curType.fontstyle & mySTC_STYLE_BOLD) > 0);
	            codeMap->StyleSetBold(Nr, (curType.fontstyle & mySTC_STYLE_BOLD) > 0);
	            codeEditor->StyleSetItalic(Nr, (curType.fontstyle & mySTC_STYLE_ITALIC) > 0);
	            codeMap->StyleSetItalic(Nr, (curType.fontstyle & mySTC_STYLE_ITALIC) > 0);
	            codeEditor->StyleSetUnderline(Nr, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0);
	            codeMap->StyleSetUnderline(Nr, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0);
	            codeEditor->StyleSetVisible(Nr, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0);
	            codeMap->StyleSetVisible(Nr, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0);
	            codeEditor->StyleSetCase(Nr, curType.lettercase);
	            codeMap->StyleSetCase(Nr, curType.lettercase);

	            const char *pwords = currentInfo->styles[Nr].words;
	            if (pwords) {
	                codeEditor->SetKeyWords(keywordnr, pwords);
	                codeMap->SetKeyWords(keywordnr, pwords);
	                keywordnr += 1;
	            }
	        }
	    }
	    return true;
    }
}

void CodeContainer::CodeEditorInitPrefs() {
	codeEditor->SetUseTabs(true);
    codeEditor->SetTabWidth(4);
    codeEditor->SetIndent(4);
    codeEditor->SetTabIndents(true);
    codeEditor->SetBackSpaceUnIndents(true);
    codeEditor->SetIndentationGuides(true);

    codeEditor->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(55, 55, 55));
    codeEditor->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));
    codeEditor->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

    codeEditor->StyleClearAll();
    codeEditor->SetCaretForeground(wxColour(wxT("WHITE")));

    codeEditor->SetMarginWidth(0, codeEditor->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_99999")));
    codeEditor->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    codeEditor->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(128, 128, 128));
    codeEditor->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(55, 55, 55));

    codeEditor->SetMarginWidth(1, 16);
    codeEditor->SetMarginType(1, wxSTC_MARGIN_SYMBOL);
    codeEditor->SetMarginMask(1, wxSTC_MASK_FOLDERS);
    codeEditor->SetMarginSensitive(1, true);

    codeEditor->Bind(wxEVT_STC_MARGINCLICK, &CodeContainer::OnMarginClick, this);
    codeEditor->Bind(wxEVT_STC_MODIFIED, &CodeContainer::OnChange, this);
    codeEditor->Bind(wxEVT_STC_CHARADDED, &CodeContainer::CharAdd, this);
    codeEditor->Bind(wxEVT_LEFT_UP, &CodeContainer::OnClick, this);
    codeEditor->Bind(wxEVT_KEY_UP, &CodeContainer::OnArrowsPress, this);
    codeEditor->Bind(wxEVT_STC_UPDATEUI, &CodeContainer::OnScroll, this);

    codeEditor->RegisterImage(0, wxBitmap(icons_dir+"thunder.png"));
    codeEditor->RegisterImage(1, wxBitmap(icons_dir+"question.png"));
    codeEditor->SetLexer(current_lang->lexer);

    codeEditor->SetProperty(wxT("fold"), wxT("1"));
    codeEditor->SetProperty(wxT("fold.comment"), wxT("1"));
    codeEditor->SetProperty(wxT("fold.compact"), wxT("1"));
    codeEditor->SetProperty(wxT("fold.preprocessor"), wxT("1"));
    codeEditor->SetProperty(wxT("fold.html"), wxT("1"));
    codeEditor->SetProperty(wxT("fold.html.preprocessor"), wxT("1"));
    codeEditor->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    codeEditor->SetMarginMask(2, wxSTC_MASK_FOLDERS);
    codeEditor->SetFoldMarginColour(true, wxColor(55,55,55));
    codeEditor->SetFoldMarginHiColour(true, wxColor(55,55,55));

    codeEditor->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW);
    codeEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, wxColor(55,55,55));
    codeEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, wxColor(128,128,128));

    codeEditor->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW);
    codeEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, wxColor(55,55,55));
    codeEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, wxColor(128,128,128));

    codeEditor->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER);
    codeEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, wxColor(55,55,55));
    codeEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, wxColor(128,128,128));

    codeEditor->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN);
    codeEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, wxColor(55,55,55));
    codeEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, wxColor(128,128,128));

    codeEditor->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
    codeEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, wxColor(55,55,55));
    codeEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, wxColor(128,128,128));

    codeEditor->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE);
    codeEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, wxColor(55,55,55));
    codeEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, wxColor(128,128,128));

    codeEditor->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER);
    codeEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, wxColor(55,55,55));
    codeEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, wxColor(128,128,128));    

    codeEditor->MarkerEnableHighlight(false);
    codeEditor->SetMarginSensitive(2, true);

    codeEditor->SetFoldLevel(0, 1024);
    codeEditor->SetFoldLevel(1, 1024);
    codeEditor->SetFoldLevel(2, 1024|wxSTC_FOLDLEVELHEADERFLAG);
    codeEditor->SetFoldLevel(3, 1025);
    codeEditor->SetFoldLevel(4, 1025);
    codeEditor->SetFoldLevel(5, 1024);
    codeEditor->SetFoldLevel(6, 1024|wxSTC_FOLDLEVELWHITEFLAG);
}

void CodeContainer::CodeMapInitPrefs() {
    codeMap->SetReadOnly(true);
    codeMap->SetUseHorizontalScrollBar(false); 
    codeMap->SetUseVerticalScrollBar(false); 
    codeMap->SetScrollWidth(codeEditor->GetSize().GetHeight()*5);

    codeMap->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(55, 55, 55));
    codeMap->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));
    codeMap->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

    codeMap->StyleClearAll();
    codeMap->SetCaretForeground(wxColor(55, 55, 55));

    codeMap->SetMarginWidth(0, 0);
    codeMap->SetMarginType(0, 0);
    codeMap->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(128, 128, 128));
    codeMap->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(55, 55, 55));

    codeMap->SetMarginWidth(1, 0);
    codeMap->SetMarginType(1, 0);
    codeMap->SetMarginMask(1, 0);
    codeMap->SetMarginSensitive(1, false);

    codeMap->Bind(wxEVT_STC_UPDATEUI, &CodeContainer::OnScroll, this);
    codeMap->SetLexer(current_lang->lexer);

    codeMap->SetZoom(-10);
    codeMap->SetSize(wxSize(100, codeMap->GetSize().GetHeight()*2));
    codeMap->SetMinSize(wxSize(100, codeMap->GetSize().GetHeight()*2));

    codeMap->SetExtraAscent(0);
    codeMap->SetExtraAscent(-1);

    codeMap->SetScrollWidthTracking(false);
    codeMap->SetCaretWidth(0);
}

void CodeContainer::OnSave(wxCommandEvent& event) {
    for(auto&& children : FindWindowById(ID_MAIN_CODE)->GetChildren()) {
        if(children->GetName().find("_codeContainer") != std::string::npos && children->IsShown()) {
            auto this_codeEditor = ((wxStyledTextCtrl*)children->GetChildren()[0]);
            wxString this_path = this_codeEditor->GetParent()->GetLabel();

            if(this_codeEditor) {
            	for(auto&& tab : FindWindowById(ID_TABS_CONTAINER)->GetChildren()) {
                    if(tab->GetName() == this_path) {
                        this_codeEditor->SaveFile(this_path);
                        auto close_ico = tab->GetChildren()[0]->GetChildren()[1];
                        auto unsave = tab->GetChildren()[0]->GetChildren()[2];

                        if(close_ico) close_ico->Show();
                        if(unsave) unsave->Hide();

                        tab->GetSizer()->Layout();
                        tab->Update();
                    }
                }
            } else wxMessageBox(_("Save file failure"), _("Save File"), wxOK | wxICON_INFORMATION, this);
        }
    }
}

void CodeContainer::OnMarginClick(wxStyledTextEvent& event) {
    if (event.GetMargin() == 1) {
        int lineClick = codeEditor->LineFromPosition(event.GetPosition());
        int levelClick = codeEditor->GetFoldLevel(lineClick);
        if((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) codeEditor->ToggleFold(lineClick);
    }
}

void CodeContainer::OnChange(wxStyledTextEvent& event) {
    wxString key = event.GetText();
    if(codeEditor->GetModify()) {
        for(auto&& tab : FindWindowById(ID_TABS_CONTAINER)->GetChildren()) {
            if(tab->GetName() == GetLabel()) {
                auto close_ico = tab->GetChildren()[0]->GetChildren()[1];
                auto unsave = ((wxStaticBitmap*)tab->GetChildren()[0]->GetChildren()[2]);

                if(close_ico) close_ico->Hide();
                if(unsave) {
                    wxVector<wxBitmap> bitmaps;
                    bitmaps.push_back(wxBitmap(wxBitmap(icons_dir+"white_circle.png", wxBITMAP_TYPE_PNG)));
                    unsave->SetBitmap(wxBitmapBundle::FromBitmaps(bitmaps));
                    unsave->Show();
                }

                tab->GetSizer()->Layout();
                tab->Update();
            }
        }
    }
    status_bar->UpdateCodeLocale(codeEditor);
}

void CodeContainer::OnArrowsPress(wxKeyEvent& event) {
    status_bar->UpdateCodeLocale(codeEditor);
}

void CodeContainer::CharAdd(wxStyledTextEvent& event) {
    char chr = (char)event.GetKey();
    int currentLine = codeEditor->GetCurrentLine();
	char previous_char = (char)codeEditor->GetCharAt(codeEditor->GetCurrentPos()-2);

    if (chr == '\n') {
        int currentLineInd;
        int nextLineInd;

        if(currentLine > 0) {
            currentLineInd = codeEditor->GetLineIndentation(currentLine-1);
            nextLineInd = codeEditor->GetLineIndentation(currentLine+1);
        }

        if(previous_char == '{' && !nextLineInd) {
            codeEditor->SetLineIndentation(currentLine, currentLineInd+codeEditor->GetIndent());
            codeEditor->GotoPos(codeEditor->GetCurrentPos() + currentLineInd+codeEditor->GetIndent()-3);
            codeEditor->InsertText(codeEditor->GetCurrentPos(), "\n");
        } else {
            if(currentLineInd && nextLineInd) {
                if(currentLineInd == nextLineInd) {
                    codeEditor->SetLineIndentation(currentLine, currentLineInd);
                    codeEditor->GotoPos(codeEditor->PositionFromLine(currentLine) + currentLineInd-3);
                } else if(nextLineInd > currentLineInd) {
                    codeEditor->SetLineIndentation(currentLine, nextLineInd);
                    codeEditor->GotoPos(codeEditor->PositionFromLine(currentLine) + nextLineInd-3);
                }
            }
        }

    } 

    if (chr == '#') {
        wxString s = "define?0 elif?1 else?0 endif?01 error?0 if?0 ifdef?0 "
                     "ifndef?1 include?0 line?0 pragma?1 undef?0";
        codeEditor->AutoCompShow(0, s);
    }

    if(chr == 'i') {
        wxString s = "import?1 if?0";
            codeEditor->AutoCompShow(0, s);
    }

    if(chr == '(') codeEditor->InsertText(codeEditor->GetCurrentPos(), ")");
    if(chr == '{') codeEditor->InsertText(codeEditor->GetCurrentPos(), "}");
    if(chr == '"') codeEditor->InsertText(codeEditor->GetCurrentPos(), "\"");
    if(chr == '`') codeEditor->InsertText(codeEditor->GetCurrentPos(), "`");
    if(chr == '[') codeEditor->InsertText(codeEditor->GetCurrentPos(), "]");
    if(event.GetKey() == 39) codeEditor->InsertText(codeEditor->GetCurrentPos(), "'");
}

void CodeContainer::OnClick(wxMouseEvent& event) {
    status_bar->UpdateCodeLocale(codeEditor);
    event.Skip();
}

void CodeContainer::OnScroll(wxStyledTextEvent& event) {
    codeMap->SetFirstVisibleLine(codeEditor->GetFirstVisibleLine()-10);
}
