/**
 * @file ControlFlow.h
 * @author Martin Jabůrek
 *
 * @brief Generation of control flow structure labels.
 */

#pragma once
#include "common.h"

/**
 * @class ControlFlow
 * @brief Values and methods to generate a unique set of labels for program control flow structures.
 * 
 */
class ControlFlow {
public:
    /// @brief Generates labels for an if-else statement.
    static const vector<string> MakeNewIFLabelSet();
    static int ELSE; ///< The order at which the else branch label is.
    static int END; ///< The order at which the end of the statement label is.

    /// @brief Generates labels for a while statement.
    static const vector<string> MakeNewWHILELabelSet();
    static int WHILE_START; ///< Location of the while condition label.
    static int WHILE_BODY; ///< Location of the while body label.
    static int WHILE_END; ///< Location of the label at the end of the while loop.

    /// @brief Generates labels for a for statement.
    static const vector<string> MakeNewFORLabelSet();
    static int FOR_INIT; ///< Location of the for loop inicialization part label.
    static int FOR_START; ///< Location of the for condition label.
    static int FOR_BODY; ///< Location of the for body label.
    static int FOR_UPDATE; ///< Location of the label preceeding the update step of the for loop.
    static int FOR_END; ///< Location of the label at the end of the for loop.

    /// @brief Generates labels for a foreach statement.
    static const vector<string> MakeNewFOREACHLabelSet();
    static int FOREACH_INIT; ///< Location of inicialisation part label of the foreach loop.
    static int FOREACH_BODY; ///< Location of the foreach body label.
    static int FOREACH_STEP; ///< Location of the foreach label located before index update.
    static int FOREACH_END; ///< Location of the foreach ending label.

private:
    /// @brief Generates a numerical string for better unique identification of a control flow structure.
    static string GetUnique();
};
