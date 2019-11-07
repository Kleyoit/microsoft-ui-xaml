#pragma once
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "common.h"
#include "ColumnMajorUniformToLargestGridLayout.g.h"
#include "ColumnMajorUniformToLargestGridLayout.properties.h"
#include "Layout.h"
#include "NonVirtualizingLayout.h"

class ColumnMajorUniformToLargestGridLayout :
    public ReferenceTracker<ColumnMajorUniformToLargestGridLayout, winrt::implementation::ColumnMajorUniformToLargestGridLayoutT, NonVirtualizingLayout>,
    public ColumnMajorUniformToLargestGridLayoutProperties
{
public:
    ColumnMajorUniformToLargestGridLayout();
    winrt::Size MeasureOverride(
        winrt::NonVirtualizingLayoutContext const& context,
        winrt::Size const& availableSize);
    winrt::Size ArrangeOverride(
        winrt::NonVirtualizingLayoutContext const& context,
        winrt::Size const& finalSize);


    void OnColumnSpacingPropertyChanged(const winrt::DependencyPropertyChangedEventArgs&);
    void OnRowSpacingPropertyChanged(const winrt::DependencyPropertyChangedEventArgs&);
    void OnMaximumColumnsPropertyChanged(const winrt::DependencyPropertyChangedEventArgs&);
private:
    winrt::Size LargestChildSize(const winrt::NonVirtualizingLayoutContext& context);
};

