#include "search.hpp"

#include "../../members/customRectangle.cpp"

SidePage_Search::SidePage_Search(
	wxWindow* parent, wxWindowID ID
) : wxPanel(parent, ID) {
	this->SetBackgroundColour(wxColor(45, 45, 45));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizerAndFit(sizer);
}