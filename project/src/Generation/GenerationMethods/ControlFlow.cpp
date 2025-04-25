/**
 * @file ControlFlow.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link ControlFlow.h
 */

#include "ControlFlow.h"

const vector<string> ControlFlow::MakeNewIFLabelSet() {
    string unique = ControlFlow::GetUnique();
    vector<string> labels;

    labels.push_back("__else_" + unique);
    labels.push_back("__if_end_" + unique);

    return labels;
}

int ControlFlow::ELSE = 0;
int ControlFlow::END = 1;



const vector<string> ControlFlow::MakeNewWHILELabelSet() {
    string unique = ControlFlow::GetUnique();
    vector<string> labels;

    labels.push_back("__while_start_" + unique);
    labels.push_back("__while_body_" + unique); // needed for redo statement
    labels.push_back("__while_end_" + unique);

    return labels;
}

int ControlFlow::WHILE_START = 0;
int ControlFlow::WHILE_BODY = 1;
int ControlFlow::WHILE_END = 2;



const vector<string> ControlFlow::MakeNewFORLabelSet() {
    string unique = ControlFlow::GetUnique();
    vector<string> labels;

    labels.push_back("__for_init_" + unique);
    labels.push_back("__for_start_" + unique);
    labels.push_back("__for_body_" + unique);
    labels.push_back("__for_update_" + unique);
    labels.push_back("__for_end_" + unique);

    return labels;
}

int ControlFlow::FOR_INIT = 0;
int ControlFlow::FOR_START = 1;
int ControlFlow::FOR_BODY = 2;
int ControlFlow::FOR_UPDATE = 3;
int ControlFlow::FOR_END = 4;



const vector<string> ControlFlow::MakeNewFOREACHLabelSet() {
    string unique = ControlFlow::GetUnique();
    vector<string> labels;

    labels.push_back("__foreach_init_" + unique);
    labels.push_back("__foreach_body_" + unique);
    labels.push_back("__foreach_step_" + unique);
    labels.push_back("__foreach_end_" + unique);

    return labels;
}

int ControlFlow::FOREACH_INIT = 0;
int ControlFlow::FOREACH_BODY = 1;
int ControlFlow::FOREACH_STEP = 2;
int ControlFlow::FOREACH_END = 3;



string ControlFlow::GetUnique() {
    static int id = 0;

    stringstream labelNumber;
    labelNumber << setw(4) << setfill('0') << id;

    id++;

    return labelNumber.str();
}
