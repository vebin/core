/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDED_VCL_WELDUTILS_HXX
#define INCLUDED_VCL_WELDUTILS_HXX

#include <com/sun/star/awt/XWindow.hpp>
#include <com/sun/star/uno/Reference.hxx>
#include <comphelper/interfacecontainer2.hxx>
#include <cppuhelper/compbase.hxx>
#include <tools/link.hxx>
#include <vcl/dllapi.h>
#include <vcl/formatter.hxx>
#include <vcl/weld.hxx>

namespace weld
{
typedef cppu::WeakComponentImplHelper<css::awt::XWindow> TransportAsXWindow_Base;

class VCL_DLLPUBLIC TransportAsXWindow : public TransportAsXWindow_Base
{
private:
    osl::Mutex m_aHelperMtx;
    weld::Widget* m_pWeldWidget;
    weld::Builder* m_pWeldWidgetBuilder;

    comphelper::OInterfaceContainerHelper2 m_aWindowListeners;
    comphelper::OInterfaceContainerHelper2 m_aKeyListeners;
    comphelper::OInterfaceContainerHelper2 m_aFocusListeners;
    comphelper::OInterfaceContainerHelper2 m_aMouseListeners;
    comphelper::OInterfaceContainerHelper2 m_aMotionListeners;
    comphelper::OInterfaceContainerHelper2 m_aPaintListeners;

public:
    TransportAsXWindow(weld::Widget* pWeldWidget, weld::Builder* pWeldWidgetBuilder = nullptr)
        : TransportAsXWindow_Base(m_aHelperMtx)
        , m_pWeldWidget(pWeldWidget)
        , m_pWeldWidgetBuilder(pWeldWidgetBuilder)
        , m_aWindowListeners(m_aHelperMtx)
        , m_aKeyListeners(m_aHelperMtx)
        , m_aFocusListeners(m_aHelperMtx)
        , m_aMouseListeners(m_aHelperMtx)
        , m_aMotionListeners(m_aHelperMtx)
        , m_aPaintListeners(m_aHelperMtx)
    {
    }

    weld::Widget* getWidget() const { return m_pWeldWidget; }

    weld::Builder* getBuilder() const { return m_pWeldWidgetBuilder; }

    virtual void clear()
    {
        m_pWeldWidget = nullptr;
        m_pWeldWidgetBuilder = nullptr;
    }

    // css::awt::XWindow
    void SAL_CALL setPosSize(sal_Int32, sal_Int32, sal_Int32, sal_Int32, sal_Int16) override
    {
        throw css::uno::RuntimeException("not implemented");
    }

    css::awt::Rectangle SAL_CALL getPosSize() override
    {
        throw css::uno::RuntimeException("not implemented");
    }

    void SAL_CALL setVisible(sal_Bool bVisible) override { m_pWeldWidget->set_visible(bVisible); }

    void SAL_CALL setEnable(sal_Bool bSensitive) override
    {
        m_pWeldWidget->set_sensitive(bSensitive);
    }

    void SAL_CALL setFocus() override { m_pWeldWidget->grab_focus(); }

    void SAL_CALL
    addWindowListener(const css::uno::Reference<css::awt::XWindowListener>& rListener) override
    {
        m_aWindowListeners.addInterface(rListener);
    }

    void SAL_CALL
    removeWindowListener(const css::uno::Reference<css::awt::XWindowListener>& rListener) override
    {
        m_aWindowListeners.removeInterface(rListener);
    }

    void SAL_CALL
    addFocusListener(const css::uno::Reference<css::awt::XFocusListener>& rListener) override
    {
        m_aFocusListeners.addInterface(rListener);
    }

    void SAL_CALL
    removeFocusListener(const css::uno::Reference<css::awt::XFocusListener>& rListener) override
    {
        m_aFocusListeners.removeInterface(rListener);
    }

    void SAL_CALL
    addKeyListener(const css::uno::Reference<css::awt::XKeyListener>& rListener) override
    {
        m_aKeyListeners.addInterface(rListener);
    }

    void SAL_CALL
    removeKeyListener(const css::uno::Reference<css::awt::XKeyListener>& rListener) override
    {
        m_aKeyListeners.removeInterface(rListener);
    }

    void SAL_CALL
    addMouseListener(const css::uno::Reference<css::awt::XMouseListener>& rListener) override
    {
        m_aMouseListeners.addInterface(rListener);
    }

    void SAL_CALL
    removeMouseListener(const css::uno::Reference<css::awt::XMouseListener>& rListener) override
    {
        m_aMouseListeners.removeInterface(rListener);
    }

    void SAL_CALL addMouseMotionListener(
        const css::uno::Reference<css::awt::XMouseMotionListener>& rListener) override
    {
        m_aMotionListeners.addInterface(rListener);
    }

    void SAL_CALL removeMouseMotionListener(
        const css::uno::Reference<css::awt::XMouseMotionListener>& rListener) override
    {
        m_aMotionListeners.removeInterface(rListener);
    }

    void SAL_CALL
    addPaintListener(const css::uno::Reference<css::awt::XPaintListener>& rListener) override
    {
        m_aPaintListeners.addInterface(rListener);
    }

    void SAL_CALL
    removePaintListener(const css::uno::Reference<css::awt::XPaintListener>& rListener) override
    {
        m_aPaintListeners.removeInterface(rListener);
    }
};

class VCL_DLLPUBLIC EntryFormatter : public Formatter
{
public:
    EntryFormatter(weld::Entry& rEntry);
    EntryFormatter(weld::FormattedSpinButton& rSpinButton);

    // EntryFormatter will set listeners to "changed" and "focus-out" of the
    // entry so users that want to add their own listeners to those must set
    // them through this formatter and not directly on the entry
    void connect_changed(const Link<weld::Entry&, void>& rLink) { m_aModifyHdl = rLink; }
    void connect_focus_out(const Link<weld::Widget&, void>& rLink) { m_aFocusOutHdl = rLink; }

    weld::Entry& get_widget() { return m_rEntry; }

    // public Formatter overrides, drives interactions with the Entry
    virtual Selection GetEntrySelection() const override;
    virtual OUString GetEntryText() const override;
    virtual void SetEntryText(const OUString& rText, const Selection& rSel) override;
    virtual void SetEntryTextColor(const Color* pColor) override;
    virtual SelectionOptions GetEntrySelectionOptions() const override;
    virtual void FieldModified() override;

    // public Formatter overrides, drives optional SpinButton settings
    virtual void ClearMinValue() override;
    virtual void SetMinValue(double dMin) override;
    virtual void ClearMaxValue() override;
    virtual void SetMaxValue(double dMin) override;

    virtual void SetSpinSize(double dStep) override;

    void SetEntrySelectionOptions(SelectionOptions eOptions) { m_eOptions = eOptions; }

    virtual ~EntryFormatter() override;

private:
    weld::Entry& m_rEntry;
    weld::FormattedSpinButton* m_pSpinButton;
    Link<weld::Entry&, void> m_aModifyHdl;
    Link<weld::Widget&, void> m_aFocusOutHdl;
    SelectionOptions m_eOptions;
    DECL_DLLPRIVATE_LINK(ModifyHdl, weld::Entry&, void);
    DECL_DLLPRIVATE_LINK(FocusOutHdl, weld::Widget&, void);
    void Init();

    // private Formatter overrides
    virtual void UpdateCurrentValue(double dCurrentValue) override;
};

class VCL_DLLPUBLIC DoubleNumericEntry final : public EntryFormatter
{
public:
    DoubleNumericEntry(weld::Entry& rEntry);
    DoubleNumericEntry(weld::FormattedSpinButton& rSpinButton);

    virtual ~DoubleNumericEntry() override;

private:
    virtual bool CheckText(const OUString& sText) const override;

    virtual void FormatChanged(FORMAT_CHANGE_TYPE nWhat) override;
    void ResetConformanceTester();

    std::unique_ptr<validation::NumberValidator> m_pNumberValidator;
};

// get the row the iterator is on
VCL_DLLPUBLIC size_t GetAbsPos(const weld::TreeView& rTreeView, const weld::TreeIter& rIter);

// an entry is visible if all parents are expanded
VCL_DLLPUBLIC bool IsEntryVisible(const weld::TreeView& rTreeView, const weld::TreeIter& rIter);

// A Parent's Children are turned into Children of the Parent which comes next in hierarchy
VCL_DLLPUBLIC void RemoveParentKeepChildren(weld::TreeView& rTreeView, weld::TreeIter& rParent);
}

#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
