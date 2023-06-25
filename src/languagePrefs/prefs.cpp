#pragma once

#include "prefs.hpp"
#include "languages/CPP.cpp"

const CommonInfo global_commonPrefs = {
    true,  // syntaxEnable
    true,  // foldEnable
    true,  // indentEnable
    false, // overTypeInitial
    false, // readOnlyInitial
    false,  // wrapModeInitial
    false, // displayEOLEnable
    false, // IndentGuideEnable
    true,  // lineNumberEnable
    false, // longLineOnEnable
    false, // whiteSpaceEnable
};

const LanguageInfo languages_prefs [] = {
    CPP_PREFS,
};
const int languages_prefs_size = WXSIZEOF(languages_prefs); 