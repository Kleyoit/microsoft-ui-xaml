// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "common.h"
#include "ResourceAccessor.h"
#include "Utils.h"
#include "ColumnMajorUniformToLargestGridLayout.h"

CppWinRTActivatableClassWithBasicFactory(ColumnMajorUniformToLargestGridLayout);

// IItemsControlOverrides

ColumnMajorUniformToLargestGridLayout::ColumnMajorUniformToLargestGridLayout()
{
}

winrt::Size ColumnMajorUniformToLargestGridLayout::MeasureOverride(
    winrt::NonVirtualizingLayoutContext const& context,
    winrt::Size const& availableSize)
{
    if (auto const children = context.Children())
    {
        auto const maxColumns = std::max(1, MaximumColumns());
        MUX_ASSERT(maxColumns > 0);
        auto const maxItemsPerColumn = static_cast<int>(std::ceil(static_cast<double>(children.Size()) / static_cast<double>(maxColumns)));

        for (auto const child : children)
        {
            child.Measure(availableSize);
        }
        auto const largestChildSize = LargestChildSize(context);

        return winrt::Size(
            largestChildSize.Width * static_cast<float>(std::min(
                static_cast<double>(maxColumns),
                static_cast<double>(children.Size()))),
            largestChildSize.Height * maxItemsPerColumn);
    }
    return winrt::Size(0, 0);
}

winrt::Size ColumnMajorUniformToLargestGridLayout::ArrangeOverride(
    winrt::NonVirtualizingLayoutContext const& context,
    winrt::Size const& finalSize)
{
    if (auto const children = context.Children())
    {
        auto const maxColumns = std::max(1, MaximumColumns());
        MUX_ASSERT(maxColumns > 0);
        auto const itemCount = children.Size();
        auto const minitemsPerColumn = static_cast<int>(std::floor(static_cast<double>(itemCount) / static_cast<double>(maxColumns)));
        auto const numberOfColumnsWithExtraElements = static_cast<int>(itemCount % maxColumns);

        auto const largestChildSize = LargestChildSize(context);
        auto const columnSpacing = ColumnSpacing();
        auto const rowSpacing = RowSpacing();

        auto horizontalOffset = 0.0f;
        auto verticalOffset = 0.0f;
        auto index = 0;
        auto column = 0;
        for (auto const child : children)
        {
            auto const desiredSize = child.DesiredSize();
            child.Arrange(winrt::Rect{ horizontalOffset, verticalOffset, desiredSize.Width, desiredSize.Height });
            if (column < numberOfColumnsWithExtraElements)
            {
                if (index % (minitemsPerColumn + 1) == minitemsPerColumn)
                {
                    horizontalOffset += largestChildSize.Width + columnSpacing;
                    verticalOffset = 0.0;
                    column++;
                }
                else
                {
                    verticalOffset += largestChildSize.Height + rowSpacing;
                }
            }
            else
            {
                auto const indexAfterExtraLargeColumns = index - (numberOfColumnsWithExtraElements * (minitemsPerColumn + 1));
                if (indexAfterExtraLargeColumns % minitemsPerColumn == minitemsPerColumn - 1)
                {
                    horizontalOffset += largestChildSize.Width + columnSpacing;
                    verticalOffset = 0.0;
                    column++;
                }
                else
                {
                    verticalOffset += largestChildSize.Height + rowSpacing;
                }
            }
            index++;
        }

    }
    return finalSize;
}

winrt::Size ColumnMajorUniformToLargestGridLayout::LargestChildSize(winrt::NonVirtualizingLayoutContext const& context)
{
    auto largestChildWidth = 0.0f;
    auto largestChildHeight = 0.0f;
    for (auto const child : context.Children())
    {
        auto const desiredSize = child.DesiredSize();
        if (desiredSize.Width > largestChildWidth)
        {
            largestChildWidth = desiredSize.Width;
        }
        if (desiredSize.Height > largestChildHeight)
        {
            largestChildHeight = desiredSize.Height;
        }
    }
    return winrt::Size(largestChildWidth, largestChildHeight);
}

void ColumnMajorUniformToLargestGridLayout::OnColumnSpacingPropertyChanged(const winrt::DependencyPropertyChangedEventArgs&)
{
    InvalidateMeasure();
}

void ColumnMajorUniformToLargestGridLayout::OnRowSpacingPropertyChanged(const winrt::DependencyPropertyChangedEventArgs&)
{
    InvalidateMeasure();
}

void ColumnMajorUniformToLargestGridLayout::OnMaximumColumnsPropertyChanged(const winrt::DependencyPropertyChangedEventArgs&)
{
    InvalidateMeasure();
}
