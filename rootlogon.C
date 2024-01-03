{
gROOT->LoadMacro("macros/analysis.cxx+");
setStyle();
analysis();
gROOT->ProcessLine(".q");
}
