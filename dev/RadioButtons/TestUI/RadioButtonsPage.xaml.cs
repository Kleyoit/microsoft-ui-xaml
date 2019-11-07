// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Markup;
using Windows.UI;
using System.Windows.Input;
using System.Collections.ObjectModel;

using RadioButtons = Microsoft.UI.Xaml.Controls.RadioButtons;

namespace MUXControlsTestApp
{
    [TopLevelTestPage(Name = "RadioButtons", Icon = "RadioButton.png")]
    public sealed partial class RadioButtonsPage : TestPage
    {
        ObservableCollection<string> m_stringItemCollection;
        ObservableCollection<RadioButton> m_radioButtonItemCollection;
        bool m_loaded = false;

        public RadioButtonsPage()
        {
            this.InitializeComponent();
            m_stringItemCollection = new ObservableCollection<string>();
            m_radioButtonItemCollection = new ObservableCollection<RadioButton>();
            this.Loaded += RadioButtonsPage_Loaded;
        }

        private void RadioButtonsPage_Loaded(object sender, RoutedEventArgs e)
        {
            m_loaded = true;
            SetMaximumColumnsButton_Click(null, null);
            SetNumberOfItemsButton_Click(null, null);
            UpdateRadioButtonsSource();
            UpdateDisplayRadioButton();
        }

        private void SetMaximumColumnsButton_Click(object sender, RoutedEventArgs e)
        {
           if (UInt32.TryParse(MaximumColumnsTextBlock.Text, out uint value))
           {
                TestRadioButtons.MaximumColumns = (int)value;
           }
        }

        private void SetNumberOfItemsButton_Click(object sender, RoutedEventArgs e)
        {
            if (UInt32.TryParse(NumberOfItemsTextBlock.Text, out uint value))
            {
                m_stringItemCollection.Clear();
                m_radioButtonItemCollection.Clear();
                for (int i = 0; i<value; i++)
                {
                    m_stringItemCollection.Add(i.ToString());
                    var radioButton = new RadioButton();
                    radioButton.Content = i.ToString() + "Radio Button";
                    m_radioButtonItemCollection.Add(radioButton);
                }
            }
        }

        private void SourceComboBoxSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateRadioButtonsSource();
        }

        private void ItemTypeComboBoxSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateRadioButtonsSource();
        }

        private void UpdateDisplayRadioButtonButton_Click(object sender, RoutedEventArgs e)
        {
            UpdateDisplayRadioButton();
        }
        private void InsertDisplayRadioButtonButton_Click(object sender, RoutedEventArgs e)
        {
            if(UpdateDisplayRadioButton())
            {
                var radioButton = new RadioButton();
                radioButton.Content = DisplayRadioButton.Content;
                radioButton.IsEnabled = !(bool)CustomDisabledCheckBox.IsChecked;
                m_radioButtonItemCollection.Insert(Int32.Parse(CustomIndexTextBox.Text), radioButton);
            }
        }

        private void ClearRadioButtonsEventsButton_Click(object sender, RoutedEventArgs e)
        {
        }

        private void UpdateRadioButtonsSource()
        {
            if (m_loaded)
            {
                var itemType = ItemTypeComboBox.SelectedItem;
                var source = SourceComboBox.SelectedItem;
                if (source == ItemsComboBoxItem)
                {
                    TestRadioButtons.ItemsSource = null;
                    if (itemType == StringsComboBoxItem)
                    {
                        foreach (var item in m_stringItemCollection)
                        {
                            TestRadioButtons.Items.Add(item);
                        }
                    }
                    else if (itemType == RadioButtonElementsComboBoxItem)
                    {
                        foreach (var item in m_radioButtonItemCollection)
                        {
                            TestRadioButtons.Items.Add(item);
                        }
                    }
                }
                else if (SourceComboBox.SelectedItem == ItemsSourceComboBoxItem)
                {
                    TestRadioButtons.Items.Clear();
                    if (itemType == StringsComboBoxItem)
                    {
                        TestRadioButtons.ItemsSource = m_stringItemCollection;
                    }
                    else if (itemType == RadioButtonElementsComboBoxItem)
                    {
                        TestRadioButtons.ItemsSource = m_radioButtonItemCollection;
                    }
                }
            }
        }

        private bool UpdateDisplayRadioButton()
        {
            if (UInt32.TryParse(CustomIndexTextBox.Text, out uint value))
            {
                if (value >= m_radioButtonItemCollection.Count)
                {
                    DisplayRadioButtonErrorMessage.Text = "Index out of Range";
                    DisplayRadioButtonErrorMessage.Foreground = new SolidColorBrush(Colors.DarkRed);
                    return false;
                }
            }
            else
            {
                DisplayRadioButtonErrorMessage.Text = "Malformed Index";
                DisplayRadioButtonErrorMessage.Foreground = new SolidColorBrush(Colors.DarkRed);
                return false;
            }

            DisplayRadioButton.Content = CustomContentTextBox.Text;

            DisplayRadioButtonErrorMessage.Text = "Okay";
            DisplayRadioButtonErrorMessage.Foreground = new SolidColorBrush(Colors.Green);
            return true;
        }
    }
}
