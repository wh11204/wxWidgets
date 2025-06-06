///////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/infobar.cpp
// Purpose:     generic wxInfoBar implementation
// Author:      Vadim Zeitlin
// Created:     2009-07-28
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"


#if wxUSE_INFOBAR

#include "wx/infobar.h"

#ifndef WX_PRECOMP
    #include "wx/bmpbuttn.h"
    #include "wx/button.h"
    #include "wx/dcmemory.h"
    #include "wx/settings.h"
    #include "wx/statbmp.h"
    #include "wx/stattext.h"
    #include "wx/sizer.h"
#endif // WX_PRECOMP

#include "wx/artprov.h"
#include "wx/scopeguard.h"
#include "wx/valgen.h"
#include "wx/checkbox.h"

#ifdef __WXGTK3__
    #include "wx/gtk/private/wrapgtk.h"

    #include "wx/gtk/private/gtk3-compat.h"
    #include "wx/gtk/private/stylecontext.h"
#endif

wxBEGIN_EVENT_TABLE(wxInfoBarGeneric, wxInfoBarBase)
    EVT_BUTTON(wxID_ANY, wxInfoBarGeneric::OnButton)
    EVT_BUTTON(wxID_CLOSE, wxInfoBarGeneric::OnButton)
wxEND_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

void wxInfoBarGeneric::Init()
{
    m_icon = nullptr;
    m_text = nullptr;
    m_button = nullptr;

    m_showEffect =
    m_hideEffect = wxSHOW_EFFECT_MAX;

    // use default effect duration
    m_effectDuration = 0;
}

bool wxInfoBarGeneric::Create(wxWindow *parent, wxWindowID winid, long style)
{
    // calling Hide() before Create() ensures that we're created initially
    // hidden
    Hide();
    SetWindowStyle(style);
    if ( !wxWindow::Create(parent, winid) )
        return false;

    // use special, easy to notice, colours
    wxColour colBg, colFg;
    if ( !m_hasBgCol && !m_hasFgCol )
    {
        // We want to use the native infobar colours for consistency with the
        // native implementation under GTK, but only do it for 3.24, as both
        // the CSS structure and the default colour values have changed in this
        // version compared to all the previous ones and it seems safer to keep
        // the old behaviour for the older GTK versions, see #25048.
#ifdef __WXGTK3__
        if ( wx_is_at_least_gtk3(24) )
        {
            wxGtkStyleContext sc;
            sc.Add(GTK_TYPE_INFO_BAR, "infobar", "info", nullptr);
            sc.Add("revealer");
            sc.Add("box");
            sc.Bg(colBg);
            sc.Fg(colFg);
        }
        else
#endif // GTK 3
        {
            colBg = wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK);
            colFg = wxSystemSettings::GetColour(wxSYS_COLOUR_INFOTEXT);
        }
    }

    if( !m_hasBgCol )
        SetBackgroundColour(colBg);

    // create the controls: icon, text and the button to dismiss the
    // message.

    // the icon is not shown unless it's assigned a valid bitmap
    m_icon = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap);

    m_text = new wxStaticText(this, wxID_ANY, wxString(),
                              wxDefaultPosition, wxDefaultSize,
                              wxST_ELLIPSIZE_MIDDLE);

    if(!m_hasFgCol)
        m_text->SetForegroundColour(colFg);

    m_button = wxBitmapButton::NewCloseButton(this, wxID_CLOSE);
    m_button->SetToolTip(_("Hide this notification message."));

    m_checkbox =
        new wxCheckBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, 0, wxGenericValidator(&m_checked));
    m_checkbox->SetForegroundColour(
        wxSystemSettings::GetColour(wxSYS_COLOUR_INFOTEXT));

    // center the text inside the sizer with an icon to the left of it and a
    // button at the very right
    //
    // NB: AddButton() relies on the button being the last control in the sizer
    //     and being preceded by a spacer
    wxSizer * const sizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer * const defaultControlSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* const firstRowSizer = new wxBoxSizer(wxHORIZONTAL);
    firstRowSizer->Add(m_icon, wxSizerFlags{}.Centre().Border());
    firstRowSizer->Add(m_text, wxSizerFlags{}.Proportion(1).Centre());
    firstRowSizer->AddSpacer(0); // This spacer only exists for compatibility.
    firstRowSizer->Add(m_button, wxSizerFlags{}.Centre().Border());

    wxBoxSizer* const secondRowSizer = new wxBoxSizer(wxHORIZONTAL);
    secondRowSizer->Add(m_checkbox, wxSizerFlags{}.CentreVertical().Border());

    defaultControlSizer->Add(firstRowSizer, wxSizerFlags{ 1 }.Expand());
    defaultControlSizer->Add(secondRowSizer);
    defaultControlSizer->Show(m_checkbox, !m_checkbox->GetLabel().empty(), true);

    sizer->Add(defaultControlSizer, wxSizerFlags{ 1 }.Expand());

    SetSizer(sizer);

    return true;
}

bool wxInfoBarGeneric::SetFont(const wxFont& font)
{
    if ( !wxInfoBarBase::SetFont(font) )
        return false;

    // check that we're not called before Create()
    if ( m_text )
        m_text->SetFont(font);

    return true;
}

bool wxInfoBarGeneric::SetForegroundColour(const wxColor& colour)
{
    if ( !wxInfoBarBase::SetForegroundColour(colour) )
        return false;

    if ( m_text )
        m_text->SetForegroundColour(colour);

    return true;
}

wxInfoBarGeneric::BarPlacement wxInfoBarGeneric::GetBarPlacement() const
{
    wxSizer * const sizer = GetContainingSizer();
    if ( !sizer )
        return BarPlacement_Unknown;

    const wxSizerItemList& siblings = sizer->GetChildren();
    if ( siblings.GetFirst()->GetData()->GetWindow() == this )
        return BarPlacement_Top;
    else if ( siblings.GetLast()->GetData()->GetWindow() == this )
        return BarPlacement_Bottom;
    else
        return BarPlacement_Unknown;
}

wxShowEffect wxInfoBarGeneric::GetShowEffect() const
{
    if ( m_showEffect != wxSHOW_EFFECT_MAX )
        return m_showEffect;

    switch ( GetBarPlacement() )
    {
        case BarPlacement_Top:
            return wxSHOW_EFFECT_SLIDE_TO_BOTTOM;

        case BarPlacement_Bottom:
            return wxSHOW_EFFECT_SLIDE_TO_TOP;

        default:
            wxFAIL_MSG( "unknown info bar placement" );
            wxFALLTHROUGH;

        case BarPlacement_Unknown:
            return wxSHOW_EFFECT_NONE;
    }
}

wxShowEffect wxInfoBarGeneric::GetHideEffect() const
{
    if ( m_hideEffect != wxSHOW_EFFECT_MAX )
        return m_hideEffect;

    switch ( GetBarPlacement() )
    {
        case BarPlacement_Top:
            return wxSHOW_EFFECT_SLIDE_TO_TOP;

        case BarPlacement_Bottom:
            return wxSHOW_EFFECT_SLIDE_TO_BOTTOM;

        default:
            wxFAIL_MSG( "unknown info bar placement" );
            wxFALLTHROUGH;

        case BarPlacement_Unknown:
            return wxSHOW_EFFECT_NONE;
    }
}

void wxInfoBarGeneric::UpdateParent()
{
    wxWindow * const parent = GetParent();
    parent->Layout();
}

void wxInfoBarGeneric::DoHide()
{
    TransferDataFromWindow();

    HideWithEffect(GetHideEffect(), GetEffectDuration());

    UpdateParent();
}

void wxInfoBarGeneric::DoShow()
{
    TransferDataToWindow();

    // re-layout the parent first so that the window expands into an already
    // unoccupied by the other controls area: for this we need to change our
    // internal visibility flag to force Layout() to take us into account (an
    // alternative solution to this hack would be to temporarily set
    // wxRESERVE_SPACE_EVEN_IF_HIDDEN flag but it's not really batter)

    // just change the internal flag indicating that the window is visible,
    // without really showing it
    wxWindowBase::Show();

    // adjust the parent layout to account for us
    UpdateParent();

    // reset the flag back before really showing the window or it wouldn't be
    // shown at all because it would believe itself already visible
    wxWindowBase::Show(false);


    // finally do really show the window.
    ShowWithEffect(GetShowEffect(), GetEffectDuration());
}

void wxInfoBarGeneric::ShowMessage(const wxString& msg, int flags)
{
    // first update the controls
    const int icon = flags & wxICON_MASK;
    if ( !icon || (icon == wxICON_NONE) )
    {
        m_icon->Hide();
    }
    else // do show an icon
    {
        m_icon->SetBitmap(wxArtProvider::GetBitmapBundle(
                            wxArtProvider::GetMessageBoxIconId(flags),
                          wxART_BUTTON));
        m_icon->Show();
    }

    // use SetLabelText() to ensure that "&" come through correctly
    m_text->SetLabelText(msg);

    // in case it doesn't fit in the window, show the full message as a tooltip
    m_text->SetToolTip(msg);

    // then show this entire window if not done yet
    if ( !IsShown() )
    {
        DoShow();
    }
    else // we're already shown
    {
        // just update the layout to correspond to the new message
        Layout();
    }
}

void wxInfoBarGeneric::Dismiss()
{
    DoHide();
}

void wxInfoBarGeneric::AddButton(wxWindowID btnid, const wxString& label)
{
    wxSizer * const sizer = GetSizer();
    wxCHECK_RET( sizer, "must be created first" );

    // user-added buttons replace the standard close button so remove it if we
    // hadn't done it yet
    if ( sizer->Detach(m_button) )
    {
        m_button->Hide();
    }

    wxButton * const button = new wxButton(this, btnid, label);

#ifdef __WXMAC__
    // smaller buttons look better in the (narrow) info bar under OS X
    button->SetWindowVariant(wxWINDOW_VARIANT_SMALL);
#endif // __WXMAC__

    sizer->Add(button, wxSizerFlags().Centre().DoubleBorder());
    if ( IsShown() )
        sizer->Layout();
}

size_t wxInfoBarGeneric::GetButtonCount() const
{
    size_t count = 0;
    wxSizer * const sizer = GetSizer();
    if ( !sizer )
        return 0;

    // iterate over the sizer items in reverse order
    const wxSizerItemList& items = sizer->GetChildren();
    for ( wxSizerItemList::compatibility_iterator node = items.GetLast();
          node != items.GetFirst();
          node = node->GetPrevious() )
    {
        const wxSizerItem * const item = node->GetData();

        // if we reached the spacer separating the buttons from the text
        // break the for-loop.
        if ( item->IsSpacer() )
            break;

        // if the standard button is shown, there must be no other ones
        if ( item->GetWindow() == m_button )
            return 0;

        ++count;
    }

    return count;
}

wxWindowID wxInfoBarGeneric::GetButtonId(size_t idx) const
{
    wxCHECK_MSG( idx < GetButtonCount(), wxID_NONE,
                 "Invalid infobar button position" );

    wxSizer * const sizer = GetSizer();
    if ( !sizer )
        return wxID_NONE;

    bool foundSpacer = false;

    size_t count = 0;
    const wxSizerItemList& items = sizer->GetChildren();
    for ( wxSizerItemList::compatibility_iterator node = items.GetLast();
          node != items.GetFirst() || node != items.GetLast();
          )
    {
        const wxSizerItem * const item = node->GetData();

        if ( item->IsSpacer() )
            foundSpacer = true;

        if ( foundSpacer )
        {
            if ( !item->IsSpacer() )
            {
                if ( count == idx )
                {
                  if ( item->GetWindow() != m_button )
                    return item->GetWindow()->GetId();
                }

                ++count;
            }

            node = node->GetNext();
        }
        else
        {
            node = node->GetPrevious();
        }
    }

    return wxID_NONE;
}

bool wxInfoBarGeneric::HasButtonId(wxWindowID btnid) const
{
    wxSizer * const sizer = GetSizer();
    if ( !sizer )
        return false;

    // iterate over the sizer items in reverse order to find the last added
    // button with this id
    const wxSizerItemList& items = sizer->GetChildren();
    for ( wxSizerItemList::compatibility_iterator node = items.GetLast();
          node != items.GetFirst();
          node = node->GetPrevious() )
    {
        const wxSizerItem * const item = node->GetData();

        // if we reached the spacer separating the buttons from the text
        // then the wanted ID is not inside.
        if ( item->IsSpacer() )
            return false;

        // check if we found our button
        if ( item->GetWindow()->GetId() == btnid )
            return true;
    }

    return false;
}

void wxInfoBarGeneric::RemoveButton(wxWindowID btnid)
{
    wxSizer * const sizer = GetSizer();
    wxCHECK_RET( sizer, "must be created first" );

    // iterate over the sizer items in reverse order to find the last added
    // button with this id (ids of all buttons should be unique anyhow but if
    // they are repeated removing the last added one probably makes more sense)
    const wxSizerItemList& items = sizer->GetChildren();
    for ( wxSizerItemList::compatibility_iterator node = items.GetLast();
          node != items.GetFirst();
          node = node->GetPrevious() )
    {
        const wxSizerItem * const item = node->GetData();

        // if we reached the spacer separating the buttons from the text
        // preceding them without finding our button, it must mean it's not
        // there at all
        if ( item->IsSpacer() )
        {
            wxFAIL_MSG( wxString::Format("button with id %d not found", btnid) );
            return;
        }

        // check if we found our button
        if ( item->GetWindow()->GetId() == btnid )
        {
            delete item->GetWindow();
            break;
        }
    }

    // check if there are any custom buttons left
    if ( sizer->GetChildren().GetLast()->GetData()->IsSpacer() )
    {
        // if the last item is the spacer, none are left so restore the
        // standard close button
        sizer->Add(m_button, wxSizerFlags().Centre().DoubleBorder());
        m_button->Show();
    }
}

void wxInfoBarGeneric::OnButton(wxCommandEvent& WXUNUSED(event))
{
    DoHide();
}

void wxInfoBarGeneric::ShowCheckBox(const wxString& checkBoxText, bool checked)
{
    wxASSERT_MSG( HasFlag(wxINFOBAR_CHECKBOX),
                  "wxINFOBAR_CHECKBOX style should be set if calling ShowCheckBox()!");
    m_checked = checked;
    if (m_checkbox != nullptr)
    {
        m_checkbox->SetLabel(checkBoxText);
        GetSizer()->Show(m_checkbox, !checkBoxText.empty(), true);
    }
}

#endif // wxUSE_INFOBAR
