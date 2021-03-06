/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#ifndef INCLUDED_VCL_BUTTON_HXX
#define INCLUDED_VCL_BUTTON_HXX

#include <tools/solar.h>
#include <vcl/dllapi.h>
#include <vcl/image.hxx>
#include <vcl/ctrl.hxx>
#include <vcl/vclenum.hxx>
#include <memory>
#include <vector>

namespace com::sun::star::frame { struct FeatureStateEvent; }
template <class T> class VclPtr;

class Color;
class ImplCommonButtonData;
enum class DrawButtonFlags;

class VCL_DLLPUBLIC Button : public Control
{
private:
    std::unique_ptr<ImplCommonButtonData> mpButtonData;
    Link<Button*,void> maClickHdl;

    /// Command URL (like .uno:Save) in case the button should handle it.
    OUString maCommand;

                                    Button (const Button &) = delete;
                                    Button & operator= (const Button &) = delete;
public:
    SAL_DLLPRIVATE DrawTextFlags    ImplGetTextStyle( WinBits nWinStyle, DrawFlags nDrawFlags );
    SAL_DLLPRIVATE void             ImplDrawAlignedImage(OutputDevice* pDev, Point& rPos, Size& rSize,
                                              sal_uLong nImageSep,
                                              DrawTextFlags nTextStyle, tools::Rectangle *pSymbolRect=nullptr, bool bAddImageSep = false );
    SAL_DLLPRIVATE void             ImplSetFocusRect( const tools::Rectangle &rFocusRect );
    SAL_DLLPRIVATE const tools::Rectangle& ImplGetFocusRect() const;
    SAL_DLLPRIVATE void             ImplSetSymbolAlign( SymbolAlign eAlign );
    /// The x-coordinate of the vertical separator line, use in MenuButton subclass only.
    SAL_DLLPRIVATE long             ImplGetSeparatorX() const;
    SAL_DLLPRIVATE void             ImplSetSeparatorX( long nX );

protected:
    explicit            Button( WindowType nType );

public:
    virtual            ~Button() override;
    virtual void        dispose() override;

    virtual void        Click();

    void                SetClickHdl( const Link<Button*,void>& rLink ) { maClickHdl = rLink; }
    const Link<Button*,void>& GetClickHdl() const { return maClickHdl; }

    /// Setup handler for UNO commands so that commands like .uno:Something are handled automagically by this button.
    void                SetCommandHandler(const OUString& aCommand);
    OUString const &    GetCommand() const { return maCommand; }

    void                SetModeImage( const Image& rImage );
    Image const &       GetModeImage( ) const;
    bool                HasImage() const;
    void                SetImageAlign( ImageAlign eAlign );
    ImageAlign          GetImageAlign() const;
    DrawButtonFlags     GetButtonState() const;
    DrawButtonFlags&    GetButtonState();


    bool                IsSmallSymbol() const;
    void                SetSmallSymbol();
    virtual bool        set_property(const OString &rKey, const OUString &rValue) override;

    /// Sets the button state according to the FeatureStateEvent emitted by a Uno state change.
    virtual void        statusChanged(const css::frame::FeatureStateEvent& rEvent);

    virtual FactoryFunction GetUITestFactory() const override;

    virtual void DumpAsPropertyTree(tools::JsonWriter&) override;

protected:

    /// Handler for click, in case we want the button to handle uno commands (.uno:Something).
    DECL_STATIC_LINK(Button, dispatchCommandHandler, Button*, void);
};

enum class PushButtonDropdownStyle
{
    NONE            = 0x0000,
    MenuButton      = 0x0002, //visual down arrow
    SplitMenuButton = 0x0003, //visual down arrow and separator line
};

class VCL_DLLPUBLIC PushButton : public Button
{
public:
    SAL_DLLPRIVATE void            ImplSetDefButton( bool bSet );
    SAL_DLLPRIVATE void            ImplDrawPushButtonFrame(vcl::RenderContext& rRenderContext, tools::Rectangle& rRect, DrawButtonFlags nStyle);
    SAL_DLLPRIVATE static bool     ImplHitTestPushButton(vcl::Window const * pDev, const Point& rPos);
    SAL_DLLPRIVATE bool            ImplIsDefButton() const;

    explicit        PushButton( vcl::Window* pParent, WinBits nStyle = 0 );

    virtual void    MouseButtonDown( const MouseEvent& rMEvt ) override;
    virtual void    Tracking( const TrackingEvent& rTEvt ) override;
    virtual void    KeyInput( const KeyEvent& rKEvt ) override;
    virtual void    KeyUp( const KeyEvent& rKEvt ) override;
    virtual void    Paint( vcl::RenderContext& rRenderContext, const tools::Rectangle& rRect ) override;
    virtual void    Draw( OutputDevice* pDev, const Point& rPos, DrawFlags nFlags ) override;
    virtual void    Resize() override;
    virtual void    GetFocus() override;
    virtual void    LoseFocus() override;
    virtual void    StateChanged( StateChangedType nType ) override;
    virtual void    DataChanged( const DataChangedEvent& rDCEvt ) override;
    virtual bool    PreNotify( NotifyEvent& rNEvt ) override;

    void            Toggle();

    void            SetSymbol( SymbolType eSymbol );
    SymbolType      GetSymbol() const { return meSymbol; }
    void            SetSymbolAlign( SymbolAlign eAlign );

    void            SetDropDown( PushButtonDropdownStyle nStyle );

    void            SetState( TriState eState );
    TriState        GetState() const { return meState; }
    virtual void    statusChanged(const css::frame::FeatureStateEvent& rEvent) override;

    void            Check( bool bCheck = true );
    bool            IsChecked() const;

    void            SetPressed( bool bPressed );
    bool            IsPressed() const { return mbPressed; }

    void            EndSelection();

    Size            CalcMinimumSize() const;
    virtual Size    GetOptimalSize() const override;

    virtual bool    set_property(const OString &rKey, const OUString &rValue) override;
    virtual void    ShowFocus(const tools::Rectangle& rRect) override;

    void setAction(bool bIsAction)
    {
        mbIsAction = bIsAction;
    }

    bool isAction() const
    {
        return mbIsAction;
    }

protected:
    PushButtonDropdownStyle mnDDStyle;
    bool            mbIsActive;

    SAL_DLLPRIVATE void            ImplInitPushButtonData();
    SAL_DLLPRIVATE static WinBits  ImplInitStyle( const vcl::Window* pPrevWindow, WinBits nStyle );
    SAL_DLLPRIVATE void            ImplInitSettings( bool bBackground );
    SAL_DLLPRIVATE void            ImplDrawPushButtonContent(OutputDevice* pDev, DrawFlags nDrawFlags,
                                                             const tools::Rectangle& rRect, bool bMenuBtnSep,
                                                             DrawButtonFlags nButtonFlags);
    SAL_DLLPRIVATE void            ImplDrawPushButton(vcl::RenderContext& rRenderContext);
    using Button::ImplGetTextStyle;
    SAL_DLLPRIVATE DrawTextFlags   ImplGetTextStyle( DrawFlags nDrawFlags ) const;
    SAL_DLLPRIVATE bool            IsSymbol() const { return ( (meSymbol != SymbolType::DONTKNOW) && (meSymbol != SymbolType::IMAGE) ); }

                                   PushButton( const PushButton & ) = delete;
                                   PushButton& operator=( const PushButton & )
                                       = delete;

    SAL_DLLPRIVATE void            ImplInit( vcl::Window* pParent, WinBits nStyle );

    using Control::ImplInitSettings;
    using Window::ImplInit;

    explicit        PushButton( WindowType nType );

    virtual void    FillLayoutData() const override;
    virtual const vcl::Font&
                    GetCanonicalFont( const StyleSettings& _rStyle ) const override;
    virtual const Color&
                    GetCanonicalTextColor( const StyleSettings& _rStyle ) const override;

private:
    SymbolType      meSymbol;
    TriState        meState;
    bool            mbPressed;
    bool            mbIsAction;
};

inline void PushButton::Check( bool bCheck )
{
    SetState( bCheck ? TRISTATE_TRUE : TRISTATE_FALSE );
}

inline bool PushButton::IsChecked() const
{
    return (GetState() == TRISTATE_TRUE);
}

class VCL_DLLPUBLIC RadioButton : public Button
{
private:
    friend class VclBuilder;

    std::shared_ptr< std::vector< VclPtr< RadioButton > > > m_xGroup;
    tools::Rectangle       maStateRect;
    tools::Rectangle       maMouseRect;
    Image           maImage;
    bool            mbChecked;
    bool            mbRadioCheck;
    bool            mbStateChanged;
    bool            mbUsesExplicitGroup;
    Link<RadioButton&,void> maToggleHdl;
    SAL_DLLPRIVATE void     ImplInitRadioButtonData();
    SAL_DLLPRIVATE WinBits  ImplInitStyle( const vcl::Window* pPrevWindow, WinBits nStyle );
    SAL_DLLPRIVATE void     ImplInitSettings( bool bBackground );
    SAL_DLLPRIVATE void     ImplDrawRadioButtonState(vcl::RenderContext& rRenderContext);
    SAL_DLLPRIVATE void     ImplDraw( OutputDevice* pDev, DrawFlags nDrawFlags,
                              const Point& rPos, const Size& rSize,
                              const Size& rImageSize, tools::Rectangle& rStateRect,
                              tools::Rectangle& rMouseRect );
    SAL_DLLPRIVATE void     ImplDrawRadioButton(vcl::RenderContext& rRenderContext );
    SAL_DLLPRIVATE void     ImplUncheckAllOther();
    SAL_DLLPRIVATE Size     ImplGetRadioImageSize() const;
    SAL_DLLPRIVATE long     ImplGetImageToTextDistance() const;

                            RadioButton(const RadioButton &) = delete;
                            RadioButton& operator= (const RadioButton &) = delete;

protected:
    using Control::ImplInitSettings;
    using Window::ImplInit;
    SAL_DLLPRIVATE void     ImplInit( vcl::Window* pParent, WinBits nStyle );

public:
    SAL_DLLPRIVATE void     ImplCallClick( bool bGrabFocus = false, GetFocusFlags nFocusFlags = GetFocusFlags::NONE );

protected:
    virtual void    FillLayoutData() const override;
    virtual const vcl::Font&
                    GetCanonicalFont( const StyleSettings& _rStyle ) const override;
    virtual const Color&
                    GetCanonicalTextColor( const StyleSettings& _rStyle ) const override;
    void            ImplAdjustNWFSizes() override;

public:
    /*
     bUsesExplicitGroup of true means that group() is used to set what radiobuttons are part of a group
     while false means that contiguous radiobuttons are considered part of a group where WB_GROUP designates
     the start of the group and all contiguous radiobuttons without WB_GROUP set form the rest of the group.

     true is fairly straightforward, false leads to trick situations and is the legacy case
    */
    explicit        RadioButton(vcl::Window* pParent, bool bUsesExplicitGroup = true, WinBits nWinStyle = 0);
    virtual         ~RadioButton() override;
    virtual void    dispose() override;

    virtual void    MouseButtonDown( const MouseEvent& rMEvt ) override;
    virtual void    Tracking( const TrackingEvent& rTEvt ) override;
    virtual void    KeyInput( const KeyEvent& rKEvt ) override;
    virtual void    KeyUp( const KeyEvent& rKEvt ) override;
    virtual void    Paint( vcl::RenderContext& rRenderContext, const tools::Rectangle& rRect ) override;
    virtual void    Draw( OutputDevice* pDev, const Point& rPos, DrawFlags nFlags ) override;
    virtual void    Resize() override;
    virtual void    GetFocus() override;
    virtual void    LoseFocus() override;
    virtual void    StateChanged( StateChangedType nType ) override;
    virtual void    DataChanged( const DataChangedEvent& rDCEvt ) override;
    virtual bool    PreNotify( NotifyEvent& rNEvt ) override;

    void            Toggle();

    bool            IsStateChanged() const { return mbStateChanged; }

    void            EnableRadioCheck( bool bRadioCheck ) { mbRadioCheck = bRadioCheck; }
    bool            IsRadioCheckEnabled() const { return mbRadioCheck; }

    void            SetModeRadioImage( const Image& rImage );

    void            SetState( bool bCheck );
    void            Check( bool bCheck = true );
    bool            IsChecked() const { return mbChecked; }

    static Image    GetRadioImage( const AllSettings& rSettings, DrawButtonFlags nFlags );

    Size            CalcMinimumSize( long nMaxWidth = 0 ) const;
    virtual Size    GetOptimalSize() const override;

    void            SetToggleHdl( const Link<RadioButton&,void>& rLink ) { maToggleHdl = rLink; }

    /** GetRadioButtonGroup returns a list of pointers to <code>RadioButton</code>s in the same group.

    The pointers in the returned list are valid at the time call returns. However rescheduling
    or giving up the SolarMutex may mean events get executed that lead to the pointers getting
    invalid.

    @param bIncludeThis
    defines whether <code>this</code> is contained in the returned list

    @return
    on return contains the <code>RadioButton</code>s
    in the same group as this <code>RadioButton</code>.
    */
    std::vector<VclPtr<RadioButton> > GetRadioButtonGroup(bool bIncludeThis = true) const;

    virtual bool set_property(const OString &rKey, const OUString &rValue) override;

    /*
     * Group this RadioButton with another
     */
    void group(RadioButton &rOther);
    virtual void ShowFocus(const tools::Rectangle& rRect) override;

    /// Button hes additional stuff that we need to dump too.
    void DumpAsPropertyTree(tools::JsonWriter&) override;

    virtual FactoryFunction GetUITestFactory() const override;
};

class VCL_DLLPUBLIC ImageButton final : public PushButton
{
protected:
    using PushButton::ImplInitStyle;

private:
    SAL_DLLPRIVATE void     ImplInitStyle();

                            ImageButton( const ImageButton & ) = delete;
                            ImageButton & operator= ( const ImageButton & ) = delete;

public:
                 ImageButton( vcl::Window* pParent, WinBits nStyle = 0 );
};

#endif // INCLUDED_VCL_BUTTON_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
