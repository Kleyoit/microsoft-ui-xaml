﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Microsoft.UI.Private.Controls;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using System;
using System.Collections.Generic;
using Windows.UI.Xaml;

namespace MUXControlsTestApp
{
    public sealed partial class ScrollingViewKeyboardAndGamepadNavigationPage : TestPage
    {
        private Object asyncEventReportingLock = new Object();
        private List<string> lstAsyncEventMessage = new List<string>();

        public ScrollingViewKeyboardAndGamepadNavigationPage()
        {
            this.InitializeComponent();
        }

        private void ScrollingPresenter_ExtentChanged(ScrollingPresenter sender, object args)
        {
            AppendAsyncEventMessage("ScrollingPresenter.ExtentChanged ExtentWidth=" + sender.ExtentWidth + ", ExtentHeight=" + sender.ExtentHeight);
        }

        private void ScrollingPresenter_StateChanged(ScrollingPresenter sender, object args)
        {
            AppendAsyncEventMessage("ScrollingPresenter.StateChanged " + sender.State.ToString());
        }

        private void ScrollingPresenter_ViewChanged(ScrollingPresenter sender, object args)
        {
            AppendAsyncEventMessage("ScrollingPresenter.ViewChanged H=" + sender.HorizontalOffset.ToString() + ", V=" + sender.VerticalOffset + ", ZF=" + sender.ZoomFactor);
        }

        private void ScrollingPresenter_ScrollAnimationStarting(ScrollingPresenter sender, ScrollingScrollAnimationStartingEventArgs args)
        {
            AppendAsyncEventMessage("ScrollingPresenter.ScrollAnimationStarting OffsetsChangeId=" + args.ScrollInfo.OffsetsChangeId + " SP=(" + args.StartPosition.X + "," + args.StartPosition.Y + ") EP=(" + args.EndPosition.X + "," + args.EndPosition.Y + ")");
        }

        private void ScrollingPresenter_ZoomAnimationStarting(ScrollingPresenter sender, ScrollingZoomAnimationStartingEventArgs args)
        {
            AppendAsyncEventMessage("ScrollingPresenter.ZoomAnimationStarting ZoomFactorChangeId=" + args.ZoomInfo.ZoomFactorChangeId + ", CenterPoint=" + args.CenterPoint + ", SZF=" + args.StartZoomFactor + ", EZF=" + args.EndZoomFactor);
        }

        private void ScrollingView_GettingFocus(UIElement sender, Windows.UI.Xaml.Input.GettingFocusEventArgs args)
        {
            FrameworkElement oldFE = args.OldFocusedElement as FrameworkElement;
            string oldFEName = (oldFE == null) ? "null" : oldFE.Name;
            FrameworkElement newFE = args.NewFocusedElement as FrameworkElement;
            string newFEName = (newFE == null) ? "null" : newFE.Name;

            AppendAsyncEventMessage("ScrollingView.GettingFocus FocusState=" + args.FocusState + ", Direction=" + args.Direction + ", InputDevice=" + args.InputDevice + ", oldFE=" + oldFEName + ", newFE=" + newFEName);
        }

        private void ScrollingView_LostFocus(object sender, RoutedEventArgs e)
        {
            AppendAsyncEventMessage("ScrollingView.LostFocus");
        }

        private void ScrollingView_LosingFocus(UIElement sender, Windows.UI.Xaml.Input.LosingFocusEventArgs args)
        {
            FrameworkElement oldFE = args.OldFocusedElement as FrameworkElement;
            string oldFEName = (oldFE == null) ? "null" : oldFE.Name;
            FrameworkElement newFE = args.NewFocusedElement as FrameworkElement;
            string newFEName = (newFE == null) ? "null" : newFE.Name;

            AppendAsyncEventMessage("ScrollingView.LosingFocus FocusState=" + args.FocusState + ", Direction=" + args.Direction + ", InputDevice=" + args.InputDevice + ", oldFE=" + oldFEName + ", newFE=" + newFEName);
        }

        private void ScrollingView_GotFocus(object sender, RoutedEventArgs e)
        {
            AppendAsyncEventMessage("ScrollingView.GotFocus");
        }

        private void ScrollingView_ExtentChanged(ScrollingView sender, object args)
        {
            AppendAsyncEventMessage("ScrollingView.ExtentChanged ExtentWidth=" + sender.ExtentWidth + ", ExtentHeight=" + sender.ExtentHeight);
        }

        private void ScrollingView_StateChanged(ScrollingView sender, object args)
        {
            AppendAsyncEventMessage("ScrollingView.StateChanged " + sender.State.ToString());
        }

        private void ScrollingView_ViewChanged(ScrollingView sender, object args)
        {
            AppendAsyncEventMessage("ScrollingView.ViewChanged H=" + sender.HorizontalOffset.ToString() + ", V=" + sender.VerticalOffset + ", ZF=" + sender.ZoomFactor);
        }

        private void ScrollingView_ScrollAnimationStarting(ScrollingView sender, ScrollingScrollAnimationStartingEventArgs args)
        {
            AppendAsyncEventMessage("ScrollingView.ScrollAnimationStarting OffsetsChangeId=" + args.ScrollInfo.OffsetsChangeId);
        }

        private void ScrollingView_ZoomAnimationStarting(ScrollingView sender, ScrollingZoomAnimationStartingEventArgs args)
        {
            AppendAsyncEventMessage("ScrollingView.ZoomAnimationStarting ZoomFactorChangeId=" + args.ZoomInfo.ZoomFactorChangeId + ", CenterPoint=" + args.CenterPoint);
        }

        private void BtnClearLogs_Click(object sender, RoutedEventArgs e)
        {
            lstLogs.Items.Clear();
        }

        private void ChkLogScrollingPresenterEvents_Checked(object sender, RoutedEventArgs e)
        {
            if (muxScrollingView != null)
            {
                ScrollingPresenter scrollingPresenter = ScrollingViewTestHooks.GetScrollingPresenterPart(muxScrollingView);

                if (scrollingPresenter != null)
                {
                    scrollingPresenter.ExtentChanged += ScrollingPresenter_ExtentChanged;
                    scrollingPresenter.StateChanged += ScrollingPresenter_StateChanged;
                    scrollingPresenter.ViewChanged += ScrollingPresenter_ViewChanged;
                    scrollingPresenter.ScrollAnimationStarting += ScrollingPresenter_ScrollAnimationStarting;
                    scrollingPresenter.ZoomAnimationStarting += ScrollingPresenter_ZoomAnimationStarting;
                }
            }
        }

        private void ChkLogScrollingPresenterEvents_Unchecked(object sender, RoutedEventArgs e)
        {
            if (muxScrollingView != null)
            {
                ScrollingPresenter scrollingPresenter = ScrollingViewTestHooks.GetScrollingPresenterPart(muxScrollingView);

                if (scrollingPresenter != null)
                {
                    scrollingPresenter.ExtentChanged -= ScrollingPresenter_ExtentChanged;
                    scrollingPresenter.StateChanged -= ScrollingPresenter_StateChanged;
                    scrollingPresenter.ViewChanged -= ScrollingPresenter_ViewChanged;
                    scrollingPresenter.ScrollAnimationStarting -= ScrollingPresenter_ScrollAnimationStarting;
                    scrollingPresenter.ZoomAnimationStarting -= ScrollingPresenter_ZoomAnimationStarting;
                }
            }
        }

        private void ChkLogScrollingViewEvents_Checked(object sender, RoutedEventArgs e)
        {
            if (muxScrollingView != null)
            {
                muxScrollingView.GettingFocus += ScrollingView_GettingFocus;
                muxScrollingView.GotFocus += ScrollingView_GotFocus;
                muxScrollingView.LosingFocus += ScrollingView_LosingFocus;
                muxScrollingView.LostFocus += ScrollingView_LostFocus;
                muxScrollingView.ExtentChanged += ScrollingView_ExtentChanged;
                muxScrollingView.StateChanged += ScrollingView_StateChanged;
                muxScrollingView.ViewChanged += ScrollingView_ViewChanged;
                muxScrollingView.ScrollAnimationStarting += ScrollingView_ScrollAnimationStarting;
                muxScrollingView.ZoomAnimationStarting += ScrollingView_ZoomAnimationStarting;
            }
        }

        private void ChkLogScrollingViewEvents_Unchecked(object sender, RoutedEventArgs e)
        {
            if (muxScrollingView != null)
            {
                muxScrollingView.GettingFocus -= ScrollingView_GettingFocus;
                muxScrollingView.GotFocus -= ScrollingView_GotFocus;
                muxScrollingView.LosingFocus -= ScrollingView_LosingFocus;
                muxScrollingView.LostFocus -= ScrollingView_LostFocus;
                muxScrollingView.ExtentChanged -= ScrollingView_ExtentChanged;
                muxScrollingView.StateChanged -= ScrollingView_StateChanged;
                muxScrollingView.ViewChanged -= ScrollingView_ViewChanged;
                muxScrollingView.ScrollAnimationStarting -= ScrollingView_ScrollAnimationStarting;
                muxScrollingView.ZoomAnimationStarting -= ScrollingView_ZoomAnimationStarting;
            }
        }

        private void ChkLogScrollingPresenterMessages_Checked(object sender, RoutedEventArgs e)
        {
            MUXControlsTestHooks.SetLoggingLevelForType("ScrollingPresenter", isLoggingInfoLevel: true, isLoggingVerboseLevel: true);
            if (chkLogScrollingViewMessages.IsChecked == false)
                MUXControlsTestHooks.LoggingMessage += MUXControlsTestHooks_LoggingMessage;
        }

        private void ChkLogScrollingPresenterMessages_Unchecked(object sender, RoutedEventArgs e)
        {
            MUXControlsTestHooks.SetLoggingLevelForType("ScrollingPresenter", isLoggingInfoLevel: false, isLoggingVerboseLevel: false);
            if (chkLogScrollingViewMessages.IsChecked == false)
                MUXControlsTestHooks.LoggingMessage -= MUXControlsTestHooks_LoggingMessage;
        }

        private void ChkLogScrollingViewMessages_Checked(object sender, RoutedEventArgs e)
        {
            MUXControlsTestHooks.SetLoggingLevelForType("ScrollingView", isLoggingInfoLevel: true, isLoggingVerboseLevel: true);
            if (chkLogScrollingPresenterMessages.IsChecked == false)
                MUXControlsTestHooks.LoggingMessage += MUXControlsTestHooks_LoggingMessage;
        }

        private void ChkLogScrollingViewMessages_Unchecked(object sender, RoutedEventArgs e)
        {
            MUXControlsTestHooks.SetLoggingLevelForType("ScrollingView", isLoggingInfoLevel: false, isLoggingVerboseLevel: false);
            if (chkLogScrollingPresenterMessages.IsChecked == false)
                MUXControlsTestHooks.LoggingMessage -= MUXControlsTestHooks_LoggingMessage;
        }

        private void MUXControlsTestHooks_LoggingMessage(object sender, MUXControlsTestHooksLoggingMessageEventArgs args)
        {
            if ((chkLogScrollingPresenterMessages.IsChecked == false && sender is ScrollingPresenter) ||
                (chkLogScrollingViewMessages.IsChecked == false && sender is ScrollingView))
            {
                return;
            }

            // Cut off the terminating new line.
            string msg = args.Message.Substring(0, args.Message.Length - 1);
            string asyncEventMessage = string.Empty;
            string senderName = string.Empty;

            FrameworkElement fe = sender as FrameworkElement;

            if (fe != null)
            {
                senderName = "s:" + fe.Name + ", ";
            }

            if (args.IsVerboseLevel)
            {
                asyncEventMessage = "Verbose: " + senderName + "m:" + msg;
            }
            else
            {
                asyncEventMessage = "Info: " + senderName + "m:" + msg;
            }

            AppendAsyncEventMessage(asyncEventMessage);
        }

        private void AppendAsyncEventMessage(string asyncEventMessage)
        {
            lock (asyncEventReportingLock)
            {
                while (asyncEventMessage.Length > 0)
                {
                    string msgHead = asyncEventMessage;

                    if (asyncEventMessage.Length > 100)
                    {
                        int commaIndex = asyncEventMessage.IndexOf(',', 100);
                        if (commaIndex != -1)
                        {
                            msgHead = asyncEventMessage.Substring(0, commaIndex);
                            asyncEventMessage = asyncEventMessage.Substring(commaIndex + 1);
                        }
                        else
                        {
                            asyncEventMessage = string.Empty;
                        }
                    }
                    else
                    {
                        asyncEventMessage = string.Empty;
                    }

                    lstAsyncEventMessage.Add(msgHead);
                }

                var ignored = this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, AppendAsyncEventMessage);
            }
        }

        private void AppendAsyncEventMessage()
        {
            lock (asyncEventReportingLock)
            {
                foreach (string asyncEventMessage in lstAsyncEventMessage)
                {
                    lstLogs.Items.Add(asyncEventMessage);
                }
                lstAsyncEventMessage.Clear();
            }
        }
    }
}
