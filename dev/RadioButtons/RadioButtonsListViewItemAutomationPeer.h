// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "RadioButtonsListViewItemAutomationPeer.g.h"

class RadioButtonsListViewItemAutomationPeer :
    public ReferenceTracker<
    RadioButtonsListViewItemAutomationPeer,
        winrt::implementation::RadioButtonsListViewItemAutomationPeerT>
{
public:
    RadioButtonsListViewItemAutomationPeer(winrt::FrameworkElement const& owner);

    // IAutomationPeerOverrides 
    winrt::AutomationControlType GetAutomationControlTypeCore();
};
