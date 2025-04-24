/**
 * @file ControlFlow.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link ControlFlow.h
 */

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



const vector<string> ControlFlow::MakeNewFORLabelSet() {
    static int id = 0;

    vector<string> labels;
    stringstream labelNumber;
    labelNumber << setw(4) << setfill('0') << id;

    labels.push_back("__for_init_" + labelNumber.str());
    labels.push_back("__for_start_" + labelNumber.str());
    labels.push_back("__for_body_" + labelNumber.str());
    labels.push_back("__for_update_" + labelNumber.str());
    labels.push_back("__for_end_" + labelNumber.str());

    id++;

    return labels;
}

int ControlFlow::FOR_INIT = 0;
int ControlFlow::FOR_START = 1;
int ControlFlow::FOR_BODY = 2;
int ControlFlow::FOR_UPDATE = 3;
int ControlFlow::FOR_END = 4;
