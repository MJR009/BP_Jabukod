#include "ControlFlow.h"

int ControlFlow::END = 1;
int ControlFlow::ELSE = 0;



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
