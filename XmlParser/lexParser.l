/********************************************************
 * lexParser.l
 ********************************************************/
%option noyywrap
%option prefix="xml_"

%{
#include "lexParser.tab.hpp"
%}

START_TAG "<"
STOP_TAG ">"
END_TAG "</"|"/>"
START_VERSION "<?xml"
STOP_VERSION "?>"
START_INFO "<!--"
STOP_INFO "-->"
TEXT [a-zA-Z0-9_]
SYMBOLS [\/+-,.:;%$#@={}\[\]()*]
TEQUAL "="
TSPACE " "*
TQUOTATION "\""
TTAB "\t"
END_LINE "\n"
CDATA_BEGIN "<![CDATA["
CDATA_END "]]>"

%%


{START_TAG}     { xml_lval.sym = xml_text; return START_TAG; }
{STOP_TAG}      { xml_lval.sym = xml_text; return STOP_TAG; }
{START_VERSION} { xml_lval.sym = xml_text; return START_VERSION; }
{STOP_VERSION}  { xml_lval.sym = xml_text; return STOP_VERSION; }
{START_INFO}    { xml_lval.sym = xml_text; return START_INFO; }
{STOP_INFO}     { xml_lval.sym = xml_text; return STOP_INFO; }
{END_TAG}       { xml_lval.sym = xml_text; return END_TAG; }
{END_LINE}      { }
{TEXT}          { xml_lval.unit_sym = *xml_text; return TEXT; }
{TEQUAL}        { xml_lval.unit_sym = *xml_text; return TEQUAL; }
{TSPACE}        { xml_lval.unit_sym = *xml_text; return TSPACE; }
{TQUOTATION}    { xml_lval.unit_sym = *xml_text; return TQUOTATION; }
{CDATA_BEGIN}   { xml_lval.unit_sym = *xml_text; return CDATA_BEGIN; }
{CDATA_END}     { xml_lval.unit_sym = *xml_text; return CDATA_END; }
{SYMBOLS}       { xml_lval.unit_sym = *xml_text; return SYMBOLS; }
[\t]+           { }

<<EOF>>         { return 0; }


%%
