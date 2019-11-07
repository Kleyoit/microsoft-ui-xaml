// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "common.h"
#include "RadioButtons.h"
#include "Vector.h"
#include "RuntimeProfiler.h"
#include "ResourceAccessor.h"

RadioButtons::RadioButtons()
{
    __RP_Marker_ClassById(RuntimeProfiler::ProfId_RadioButtons);

    m_selectionModel = winrt::SelectionModel{};
    m_selectionModel.SingleSelect(true);
    m_selectionModel.Source(GetItemsSource());

    const auto items = winrt::make<Vector<winrt::IInspectable>>();
    SetValue(s_ItemsProperty, items);

    SetDefaultStyleKey(this);

    s_childHandlersProperty =
        InitializeDependencyProperty(
            L"ChildHandlers",
            winrt::name_of<ChildHandlers>(),
            winrt::name_of<winrt::RadioButtons>(),
            true /* isAttached */,
            nullptr,
            nullptr);
}

void RadioButtons::OnApplyTemplate()
{
    const winrt::IControlProtected controlProtected{ *this };

    if (const auto repeater = GetTemplateChildT<winrt::ItemsRepeater>(L"InnerRepeater", controlProtected))
    {
        m_repeaterElementPreparedRevoker = repeater.ElementPrepared(winrt::auto_revoke, { this, &RadioButtons::OnRepeaterElementPrepared });
        m_repeaterElementClearingRevoker = repeater.ElementClearing(winrt::auto_revoke, { this, &RadioButtons::OnRepeaterElementClearing });
        m_repeaterLoadedRevoker = repeater.Loaded(winrt::auto_revoke, { this, &RadioButtons::OnRepeaterLoaded });
        m_repeater.set(repeater);
    }

    UpdateItemsSource();
}

void RadioButtons::OnRepeaterLoaded(const winrt::IInspectable&, const winrt::RoutedEventArgs&)
{
    if (auto repeater = m_repeater.get())
    {
        // Override normal up/down behavior -- down should always go to the next item and up to the previous.
        m_repeaterKeyDownRevoker = repeater.KeyDown(winrt::auto_revoke, { this, &RadioButtons::OnRepeaterKeyDown });
        m_repeaterKeyUpRevoker = repeater.KeyUp(winrt::auto_revoke, { this, &RadioButtons::OnRepeaterKeyUp });

        UpdateSelectedIndex();
        UpdateSelectedItem();
        UpdateMaximumColumns();
    }
}

void RadioButtons::OnRepeaterKeyDown(const winrt::IInspectable&, const winrt::KeyRoutedEventArgs& args)
{
    if (args.Key() == winrt::VirtualKey::Control)
    {
        m_isControlDown = true;
    }
}

void RadioButtons::OnRepeaterKeyUp(const winrt::IInspectable&, const winrt::KeyRoutedEventArgs& args)
{
    if (args.Key() == winrt::VirtualKey::Control)
    {
        m_isControlDown = false;
    }
}

void RadioButtons::OnChildKeyDown(const winrt::IInspectable& sender, const winrt::KeyRoutedEventArgs& args)
{
    if (args.OriginalKey() == winrt::VirtualKey::Down)
    {
        MoveSelectionNext();
        args.Handled(true);
    }
    else if (args.OriginalKey() == winrt::VirtualKey::Up)
    {
        MoveSelectionPrevious();
        args.Handled(true);
    }
    else if (args.OriginalKey() == winrt::VirtualKey::Right)
    {
        MoveSelectionRight(sender.try_as<winrt::UIElement>());
        args.Handled(true);
    }
    else if (args.OriginalKey() == winrt::VirtualKey::Left)
    {
        MoveSelectionLeft(sender.try_as<winrt::UIElement>());
        args.Handled(true);
    }
}

void RadioButtons::OnChildGotFocus(const winrt::IInspectable& sender, const winrt::RoutedEventArgs&)
{
    if (!m_isControlDown)
    {
        if (auto const senderAsToggle = sender.try_as<winrt::ToggleButton>())
        {
            senderAsToggle.IsChecked(true);
        }
    }
}

void RadioButtons::OnRepeaterElementPrepared(const winrt::ItemsRepeater&, const winrt::ItemsRepeaterElementPreparedEventArgs& args)
{
    if (args)
    {
        if (auto const toggleButton = args.Element().try_as<winrt::ToggleButton>())
        {
            auto childHandlers = winrt::make_self<ChildHandlers>();
            childHandlers->checkedRevoker = toggleButton.Checked(winrt::auto_revoke, { this, &RadioButtons::OnChildChecked });
            childHandlers->uncheckedRevoker = toggleButton.Unchecked(winrt::auto_revoke, { this, &RadioButtons::OnChildUnchecked });
            childHandlers->keyDownRevoker = toggleButton.KeyDown(winrt::auto_revoke, { this, &RadioButtons::OnChildKeyDown });
            childHandlers->previewKeyDownRevoker = toggleButton.PreviewKeyDown(winrt::auto_revoke, { this, &RadioButtons::OnChildKeyDown });
            childHandlers->gotFocusRevoker = toggleButton.GotFocus(winrt::auto_revoke, { this, &RadioButtons::OnChildGotFocus });

            toggleButton.SetValue(s_childHandlersProperty, childHandlers.as<winrt::IInspectable>());
        }
    }
}

void RadioButtons::OnRepeaterElementClearing(const winrt::ItemsRepeater&, const winrt::ItemsRepeaterElementClearingEventArgs& args)
{
    if (args)
    {
        if (auto const element = args.Element())
        {
            if (auto const childHandlers = element.GetValue(s_childHandlersProperty).try_as<ChildHandlers>())
            {
                childHandlers->checkedRevoker.revoke();
                childHandlers->uncheckedRevoker.revoke();
                childHandlers->keyDownRevoker.revoke();
                childHandlers->previewKeyDownRevoker.revoke();
            }
        }
    }
}

void RadioButtons::OnRepeaterCollectionChanged(const winrt::ItemsSourceView&, const winrt::IInspectable&)
{
    //UpdateMaximumColumns();
}

void RadioButtons::OnChildChecked(const winrt::IInspectable& sender, const winrt::RoutedEventArgs&)
{
    if (auto const repeater = m_repeater.get())
    {
        if (auto const selectedIndex = m_selectionModel.SelectedIndex())
        {
            if (selectedIndex.GetSize() == 1)
            {
                if (auto const toggleButton = repeater.TryGetElement(selectedIndex.GetAt(0)).try_as<winrt::IToggleButton>())
                {
                    toggleButton.IsChecked(false);
                }
            }
        }
        if (auto const senderAsUIE = sender.as<winrt::UIElement>())
        {
            m_selectionModel.Select(repeater.GetElementIndex(senderAsUIE));
        }
    }
}

void RadioButtons::OnChildUnchecked(const winrt::IInspectable& sender, const winrt::RoutedEventArgs&)
{
    if (auto const repeater = m_repeater.get())
    {
        if (auto const senderAsUIE = sender.as<winrt::UIElement>())
        {
            m_selectionModel.Deselect(repeater.GetElementIndex(senderAsUIE));
        }
    }
}

bool RadioButtons::MoveSelectionNext()
{
    return MoveSelection(1, MissStrategy::next);
}

bool RadioButtons::MoveSelectionPrevious()
{
    return MoveSelection(-1, MissStrategy::previous);
}

bool RadioButtons::MoveSelectionRight(const winrt::UIElement& focusedElement)
{
    if (auto const repeater = m_repeater.get())
    {
        if (auto const itemsSourceView = repeater.ItemsSourceView())
        {
            if (focusedElement)
            {
                return MoveSelection(
                    IncrementForRightMove(
                        repeater.GetElementIndex(focusedElement),
                        itemsSourceView.Count(),
                        MaximumColumns()),
                    MissStrategy::aroundRight);
            }
        }
    }
    return false;
}

bool RadioButtons::MoveSelectionLeft(const winrt::UIElement& focusedElement)
{
    if (auto const repeater = m_repeater.get())
    {
        if (auto const itemsSourceView = repeater.ItemsSourceView())
        {
            if (focusedElement)
            {
                return MoveSelection(
                    IncrementForLeftMove(
                        repeater.GetElementIndex(focusedElement),
                        itemsSourceView.Count(),
                        MaximumColumns()),
                    MissStrategy::aroundLeft);
            }
        }
    }
    return false;
}

bool RadioButtons::MoveSelection(int initialIndexIncrement, MissStrategy missStrategy)
{
    if (auto const repeater = m_repeater.get())
    {
        if (auto const focusedElement = winrt::FocusManager::GetFocusedElement().try_as<winrt::UIElement>())
        {
            auto focusedIndex = repeater.GetElementIndex(focusedElement);
            
            if (focusedIndex >= 0)
            {
                auto const fromIndex = focusedIndex;
                focusedIndex += initialIndexIncrement;
                auto const originalFocusedIndex = focusedIndex;
                auto const itemCount = repeater.ItemsSourceView().Count();
                auto const maxColumns = MaximumColumns();

                while (focusedIndex >= 0 && focusedIndex < itemCount)
                {
                    std::vector<int> visited{};
                    auto distance = 1;
                    if (auto const item = repeater.TryGetElement(focusedIndex))
                    {
                        if (auto const itemAsControl = item.try_as<winrt::IControl>())
                        {
                            if (itemAsControl.IsEnabled() && itemAsControl.IsTabStop())
                            {
                                itemAsControl.Focus(winrt::Windows::UI::Xaml::FocusState::Keyboard);
                                return true;
                            }
                        }
                    }

                    visited.push_back(focusedIndex);
                    auto const [foundCandidate, candidate, newDistance] =
                        GetNextIndex(
                            missStrategy,
                            focusedIndex,
                            visited,
                            originalFocusedIndex,
                            fromIndex,
                            distance,
                            itemCount,
                            maxColumns);

                    if (foundCandidate)
                    {
                        focusedIndex = candidate;
                        distance = newDistance;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}


std::tuple<bool, int, int> RadioButtons::GetNextIndex(
    MissStrategy missStrategy,
    int focusedIndex,
    const std::vector<int>& visited,
    int originalFocusedIndex,
    int fromIndex,
    int distance,
    int itemCount,
    int maxColumns)
{
    auto const fromIndexColumn = ColumnFromIndex(fromIndex, itemCount, maxColumns);
    switch (missStrategy)
    {
    case MissStrategy::next:
        return std::make_tuple(true, focusedIndex + 1, 0);
        break;
    case MissStrategy::previous:
        return std::make_tuple(true, focusedIndex - 1, 0);
        break;
    case MissStrategy::aroundRight:
        while (true)
        {
            boolean parametersOkay = false;
            auto const next = originalFocusedIndex + distance;
            auto const previous = originalFocusedIndex - distance;
            if (ColumnFromIndex(previous, itemCount, maxColumns) > fromIndexColumn)
            {
                if (std::find(visited.begin(), visited.end(), next) == visited.end())
                {
                    return std::make_tuple(true, next, distance);
                }
                parametersOkay = true;
            }
            else if (next < itemCount)
            {
                if (std::find(visited.begin(), visited.end(), next) == visited.end())
                {
                    return std::make_tuple(true, next, distance);
                }
                parametersOkay = true;
            }

            if (parametersOkay)
            {
                distance++;
            }
            else
            {
                return std::make_tuple(false, 0, 0);
            }
        }
        return std::make_tuple(false, 0, 0);
        break;
    case MissStrategy::aroundLeft:
        while (true)
        {
            boolean parametersOkay = false;
            auto const next = originalFocusedIndex + distance;
            auto const previous = originalFocusedIndex - distance;
            if (ColumnFromIndex(next, itemCount, maxColumns) < fromIndexColumn)
            {
                if (std::find(visited.begin(), visited.end(), next) == visited.end())
                {
                    return std::make_tuple(true, next, distance);
                }
                parametersOkay = true;
            }
            else if (previous >= 0)
            {
                if (std::find(visited.begin(), visited.end(), next) == visited.end())
                {
                    return std::make_tuple(true, next, distance);
                }
                parametersOkay = true;
            }

            if (parametersOkay)
            {
                distance++;
            }
            else
            {
                return std::make_tuple(false, 0, 0);
            }
        }
        return std::make_tuple(false, 0, 0);
        break;
    default:
        MUX_ASSERT(false);
        return std::make_tuple(false, 0, 0);
    }
}

int RadioButtons::ColumnFromIndex(int index, int itemCount, int maxColumns)
{
    MUX_ASSERT(index < itemCount);
    auto const itemsPerColumn = static_cast<int>(std::floor(static_cast<double>(itemCount) / static_cast<double>(maxColumns)));
    auto const numberOfColumnsWithExtraElements = itemCount % maxColumns;

    auto count = index;
    int currentColumn = 0;
    for (currentColumn; currentColumn < numberOfColumnsWithExtraElements; currentColumn++)
    {
        count -= itemsPerColumn + 1;
        if (count < 0)
        {
            return currentColumn;
        }
    }
    for (currentColumn; currentColumn < maxColumns; currentColumn++)
    {
        count -= itemsPerColumn;
        if (count < 0)
        {
            return currentColumn;
        }
    }
    throw winrt::hresult_invalid_argument();
}

int RadioButtons::IncrementForRightMove(int index, int itemCount, int maxColumns)
{
    // If we are moving right and are on exactly the last element which is in a larger column,
    // then we need to traverse 1 less than the other scenarios.
    auto const itemsPerColumn = static_cast<int>(std::floor(static_cast<double>(itemCount) / static_cast<double>(maxColumns)));
    auto const numberOfColumnsWithExtraElements = itemCount % maxColumns;
    if (index == (numberOfColumnsWithExtraElements * (itemsPerColumn + 1)) - 1)
    {
        return itemsPerColumn;
    }
    return IncrementForHorizontalMove(index, itemCount, maxColumns, 0);
}

int RadioButtons::IncrementForLeftMove(int index, int itemCount, int maxColumns)
{
    return -1 * IncrementForHorizontalMove(index, itemCount, maxColumns, 1);
}

int RadioButtons::IncrementForHorizontalMove(int index, int itemCount, int maxColumns, int numberOfSmallerColumnsToAccept)
{
    auto const column = ColumnFromIndex(index, itemCount, maxColumns);
    auto const itemsPerColumn = static_cast<int>(std::floor(static_cast<double>(itemCount) / static_cast<double>(maxColumns)));
    auto const numberOfColumnsWithExtraElements = itemCount % maxColumns;
    if (column < numberOfColumnsWithExtraElements + numberOfSmallerColumnsToAccept)
    {
        return itemsPerColumn + 1;
    }
    else
    {
        return itemsPerColumn;
    }
}

void RadioButtons::OnPropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args)
{
    winrt::IDependencyProperty property = args.Property();

    if (property == s_ItemsProperty || property == s_ItemsSourceProperty)
    {
        UpdateItemsSource();
    }
    else if (property == s_MaximumColumnsProperty)
    {
        UpdateMaximumColumns();
    }
    else if (property == s_SelectedIndexProperty)
    {
        UpdateSelectedIndex();
    }
    else if (property == s_SelectedItemProperty)
    {
        UpdateSelectedItem();
    }
}

void RadioButtons::UpdateItemsSource()
{
    if (auto repeater = m_repeater.get())
    {
        m_itemsSourceChanged.revoke();
        repeater.ItemsSource(GetItemsSource());
        //todo This is borked, help me jevan.
        //if (auto itemsSourceView = repeater.ItemsSourceView())
        //{
        //    m_itemsSourceChanged = itemsSourceView.CollectionChanged(winrt::auto_revoke, { this, &RadioButtons::OnRepeaterCollectionChanged });
        //}
    }
}

winrt::IInspectable RadioButtons::GetItemsSource()
{
    if (auto const itemsSource = ItemsSource())
    {
        return itemsSource;
    }
    else
    {
        return Items();
    }
}

void RadioButtons::UpdateMaximumColumns()
{
    // The only way to get UniformGridLayout to put the items in column major order
    // is to make the layout horizontal. However a horizontal UniformGridLayout only provides
    // a setter for the number of rows, so we have to calculate the appropriate rows value
    // for the number of elements our repeater currently has.
    const auto maxColumns = MaximumColumns();
    if (maxColumns <= 0)
    {
        throw winrt::hresult_invalid_argument();
    }

    if (auto const repeater = m_repeater.get())
    {
        if (auto const layout = repeater.Layout().try_as<winrt::ColumnMajorUniformToLargestGridLayout>())
        {
            if (layout.MaximumColumns() != maxColumns)
            {
                layout.MaximumColumns(maxColumns);
                repeater.InvalidateMeasure();
            }
        }
    }
}

void RadioButtons::UpdateSelectedItem()
{

}

void RadioButtons::UpdateSelectedIndex()
{

}

winrt::UIElement RadioButtons::ContainerFromItem(winrt::IInspectable const& item)
{
    if (auto repeater = m_repeater.get())
    {
        if (const auto itemTemplate = repeater.ItemTemplate())
        {
            if (const auto itemFactory = itemTemplate.try_as<winrt::RadioButtonsElementFactory>())
            {
                return itemFactory.ContainerFromItem(item);
            }
        }
    }
    return nullptr;
}

winrt::UIElement RadioButtons::ContainerFromIndex(int index)
{
    if (auto repeater = m_repeater.get())
    {
        return repeater.TryGetElement(index);
    }
    return nullptr;
}
