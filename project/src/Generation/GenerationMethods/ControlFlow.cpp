#include "ControlFlow.h"

const vector<string> ControlFlow::MakeNewIFLabelSet() {
    static int id = 0;

    vector<string> labels;
    stringstream labelNumber;
    labelNumber << setw(4) << setfill('0') << id;

    labels.push_back("__else_" + labelNumber.str());
    labels.push_back("__if_end_" + labelNumber.str());

    id++;

    return labels;
}

int ControlFlow::ELSE = 0;
int ControlFlow::END = 1;



const vector<string> ControlFlow::MakeNewWHILELabelSet() {
    static int id = 0;

    vector<string> labels;
    stringstream labelNumber;
    labelNumber << setw(4) << setfill('0') << id;

    labels.push_back("__while_start_" + labelNumber.str());
    labels.push_back("__while_body_" + labelNumber.str()); // needed for redo statement
    labels.push_back("__while_end_" + labelNumber.str());

    id++;

    return labels;
}

int ControlFlow::WHILE_START = 0;
int ControlFlow::WHILE_BODY = 1;
int ControlFlow::WHILE_END = 2;
