#pragma once

#include "prefs.hpp"

#include "languages/CPP.cpp"
#include "languages/BINARY.cpp"
#include "languages/PLAINTEXT.cpp"
#include "languages/JSON.cpp"
#include "languages/CMAKE.cpp"

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
    PLAINTEXT_PREFS,
    BINARY_PREFS,
    CPP_PREFS,
    JSON_PREFS,
    CMAKE_PREFS,
};
const int languages_prefs_size = WXSIZEOF(languages_prefs); 