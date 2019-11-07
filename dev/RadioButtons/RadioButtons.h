// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "common.h"

#include "RadioButtons.g.h"
#include "RadioButtons.properties.h"

class ChildHandlers : public winrt::implements<ChildHandlers, winrt::IInspectable>
{
public:
    winrt::ToggleButton::Checked_revoker checkedRevoker;
    winrt::ToggleButton::Unchecked_revoker uncheckedRevoker;
    winrt::ToggleButton::KeyDown_revoker keyDownRevoker;
    winrt::ToggleButton::PreviewKeyDown_revoker previewKeyDownRevoker;
    winrt::ToggleButton::GotFocus_revoker gotFocusRevoker;
};

class RadioButtons :
    public ReferenceTracker<RadioButtons, winrt::implementation::RadioButtonsT>,
    public RadioButtonsProperties
{

public:
    RadioButtons();

    // IFrameworkElement
    void OnApplyTemplate();

    winrt::UIElement ContainerFromItem(winrt::IInspectable const& item);
    winrt::UIElement ContainerFromIndex(int index);

    void OnPropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args);

    GlobalDependencyProperty s_childHandlersProperty{ nullptr };

private:
    enum MissStrategy
    {
        next,
        previous,
        aroundLeft,
        aroundRight,
    };

    void OnRepeaterLoaded(const winrt::IInspectable&, const winrt::RoutedEventArgs&);
    void OnRepeaterKeyDown(const winrt::IInspectable&, const winrt::KeyRoutedEventArgs& args);
    void OnRepeaterKeyUp(const winrt::IInspectable&, const winrt::KeyRoutedEventArgs& args);
    void OnRepeaterElementPrepared(const winrt::ItemsRepeater&, const winrt::ItemsRepeaterElementPreparedEventArgs& args);
    void OnRepeaterElementClearing(const winrt::ItemsRepeater&, const winrt::ItemsRepeaterElementClearingEventArgs& args);
    void OnRepeaterCollectionChanged(const winrt::ItemsSourceView&, const winrt::IInspectable&);
    void OnChildChecked(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnChildUnchecked(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnChildKeyDown(const winrt::IInspectable& sender, const winrt::KeyRoutedEventArgs& args);
    void OnChildGotFocus(const winrt::IInspectable& sender, const winrt::RoutedEventArgs&);

    void UpdateItemsSource();
    winrt::IInspectable GetItemsSource();
    void UpdateMaximumColumns();

    void UpdateSelectedItem();
    void UpdateSelectedIndex();

    bool MoveSelectionNext();
    bool MoveSelectionPrevious();
    bool MoveSelectionRight(const winrt::UIElement& focusedElement);
    bool MoveSelectionLeft(const winrt::UIElement& focusedElement);
    bool MoveSelection(int initialIndexIncrement, MissStrategy missStrategy);
    static std::tuple<bool, int, int> GetNextIndex(
        MissStrategy missStrategy,
        int focusedIndex,
        const std::vector<int>& visited,
        int originalFocusedIndex,
        int fromIndex,
        int distance,
        int itemCount,
        int maxColumns);

    static int ColumnFromIndex(int index, int itemCount, int maxColumns);
    static int IncrementForRightMove(int index, int itemCount, int maxColumns);
    static int IncrementForLeftMove(int index, int itemCount, int maxColumns);
    static int IncrementForHorizontalMove(int index, int itemCount, int maxColumns, int numberOfSmallerColumnsToAccept);

    bool m_isControlDown{ false };
    int m_selectedIndex{ -1 };

    tracker_ref<winrt::ItemsRepeater> m_repeater{ this };

    winrt::SelectionModel m_selectionModel{};
    winrt::Control::Loaded_revoker m_repeaterLoadedRevoker{};
    winrt::Control::KeyDown_revoker m_repeaterKeyDownRevoker{};
    winrt::Control::KeyUp_revoker m_repeaterKeyUpRevoker{};
    winrt::ItemsSourceView::CollectionChanged_revoker m_itemsSourceChanged{};
    winrt::ItemsRepeater::ElementPrepared_revoker m_repeaterElementPreparedRevoker{};
    winrt::ItemsRepeater::ElementClearing_revoker m_repeaterElementClearingRevoker{};

    std::vector<ChildHandlers> m_childHandlers{};

    winrt::IInspectable m_pointerPressedEventHandler{ nullptr };
};
