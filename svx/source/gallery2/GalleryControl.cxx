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

#include "GalleryControl.hxx"

#include "gallery.hrc"
#include "svx/galmisc.hxx"
#include "svx/gallery1.hxx"
#include "galbrws1.hxx"
#include "galbrws2.hxx"
#include "GallerySplitter.hxx"
#include <vcl/svapp.hxx>
#include <vcl/settings.hxx>
#include <sfx2/sidebar/Theme.hxx>

#include <boost/bind.hpp>

namespace svx { namespace sidebar {

static const sal_Int32 gnInitialVerticalSplitPosition (150);

GalleryControl::GalleryControl (
    SfxBindings* /*pBindings*/,
    vcl::Window* pParentWindow)
    : Window(pParentWindow, WB_SIZEABLE|WB_MOVEABLE|WB_CLOSEABLE|WB_HIDE),
      mpGallery (Gallery::GetGalleryInstance()),
      mpSplitter(VclPtr<GallerySplitter>::Create(

              this,
              WB_HSCROLL,
              ::boost::bind(&GalleryControl::InitSettings, this))),
      mpBrowser1(VclPtr<GalleryBrowser1>::Create(

              this,
              mpGallery,
              ::boost::bind(&GalleryControl::GalleryKeyInput,this,_1,_2),
              ::boost::bind(&GalleryControl::ThemeSelectionHasChanged, this))),
      mpBrowser2(VclPtr<GalleryBrowser2>::Create(this, mpGallery)),
      maLastSize(GetOutputSizePixel()),
      mbIsInitialResize(true)
{
    mpBrowser1->SelectTheme(0);
    mpBrowser1->Show(true);

    mpBrowser2->Show(true);

    mpSplitter->SetHorizontal(false);
    mpSplitter->SetSplitHdl( LINK( this, GalleryControl, SplitHdl ) );
    mpSplitter->Show( true );

    InitSettings();
}

GalleryControl::~GalleryControl()
{
    disposeOnce();
}

void GalleryControl::dispose()
{
    mpSplitter.disposeAndClear();
    mpBrowser1.disposeAndClear();
    mpBrowser2.disposeAndClear();
    vcl::Window::dispose();
}

void GalleryControl::InitSettings()
{
    SetBackground( Wallpaper( GALLERY_DLG_COLOR ) );
    SetControlBackground( GALLERY_DLG_COLOR );
    SetControlForeground( GALLERY_DLG_COLOR );

    mpSplitter->SetBackground( Wallpaper( GALLERY_DLG_COLOR ) );
    mpSplitter->SetControlBackground( GALLERY_DLG_COLOR );
    mpSplitter->SetControlForeground( GALLERY_DLG_COLOR );

    mpBrowser1->SetBackground( Wallpaper( GALLERY_DLG_COLOR ) );
    mpBrowser1->SetControlBackground( GALLERY_DLG_COLOR );
    mpBrowser1->SetControlForeground( GALLERY_DLG_COLOR );

    mpBrowser2->SetBackground( Wallpaper( GALLERY_DLG_COLOR ) );
    mpBrowser2->SetControlBackground( GALLERY_DLG_COLOR );
    mpBrowser2->SetControlForeground( GALLERY_DLG_COLOR );

    const Wallpaper aBackground (sfx2::sidebar::Theme::GetWallpaper(sfx2::sidebar::Theme::Paint_PanelBackground));
    mpSplitter->SetBackground(aBackground);
    SetBackground(aBackground);
    mpBrowser2->SetBackground(aBackground);
}

void GalleryControl::Resize()
{
    // call parent
    Window::Resize();

    // update hor/ver
    const Size aNewSize( GetOutputSizePixel() );
    if (aNewSize.Width()<=0 || aNewSize.Height()<=0)
        return;

    const bool bNewLayoutHorizontal(aNewSize.Width() > aNewSize.Height());
    const bool bOldLayoutHorizontal(mpSplitter->IsHorizontal());
    long nSplitPos( bOldLayoutHorizontal ? mpSplitter->GetPosPixel().X() : mpSplitter->GetPosPixel().Y());
    const long nSplitSize( bOldLayoutHorizontal ? mpSplitter->GetOutputSizePixel().Width() : mpSplitter->GetOutputSizePixel().Height());

    if(bNewLayoutHorizontal != bOldLayoutHorizontal)
    {
        mpSplitter->SetHorizontal(bNewLayoutHorizontal);
    }
    else
    {
        if (mbIsInitialResize)
        {
            nSplitPos = gnInitialVerticalSplitPosition;
            if (nSplitPos > aNewSize.Height()/2)
                nSplitPos = aNewSize.Height()/2;
        }
    }
    mbIsInitialResize = false;

    const long nFrameLen = LogicToPixel( Size( 3, 0 ), MAP_APPFONT ).Width();
    const long nFrameLen2 = nFrameLen << 1;

    if(bNewLayoutHorizontal)
    {
        mpBrowser1->SetPosSizePixel(
            Point( nFrameLen, nFrameLen ),
            Size(nSplitPos - nFrameLen, aNewSize.Height() - nFrameLen2) );

        mpSplitter->SetPosSizePixel(
            Point( nSplitPos, 0),
            Size( nSplitSize, aNewSize.Height() ) );

        mpSplitter->SetDragRectPixel(
            Rectangle(
                Point( nFrameLen2, 0 ),
                Size( aNewSize.Width() - ( nFrameLen2 << 1 ) - nSplitSize, aNewSize.Height() ) ) );

        mpBrowser2->SetPosSizePixel(
            Point( nSplitPos + nSplitSize, nFrameLen ),
            Size( aNewSize.Width() - nSplitSize - nSplitPos - nFrameLen, aNewSize.Height() - nFrameLen2 ) );
    }
    else
    {
        mpBrowser1->SetPosSizePixel(
            Point( nFrameLen, nFrameLen ),
            Size(aNewSize.Width() - nFrameLen2, nSplitPos - nFrameLen));

        mpSplitter->SetPosSizePixel(
            Point( 0, nSplitPos),
            Size( aNewSize.Width(), nSplitSize ) );

        mpSplitter->SetDragRectPixel(
            Rectangle(
                Point( 0, nFrameLen2 ),
                Size( aNewSize.Width(), aNewSize.Height() - ( nFrameLen2 << 1 ) - nSplitSize ) ));

        mpBrowser2->SetPosSizePixel(
            Point( nFrameLen, nSplitPos + nSplitSize ),
            Size( aNewSize.Width() - nFrameLen2, aNewSize.Height() - nSplitSize - nSplitPos - nFrameLen ));
    }

    maLastSize = aNewSize;
}

bool GalleryControl::GalleryKeyInput( const KeyEvent& rKEvt, vcl::Window* )
{
    const sal_uInt16    nCode = rKEvt.GetKeyCode().GetCode();
    bool            bRet = ( !rKEvt.GetKeyCode().IsMod1() &&
                           ( ( KEY_TAB == nCode ) || ( KEY_F6 == nCode && rKEvt.GetKeyCode().IsMod2() ) ) );

    if( bRet )
    {
        if( !rKEvt.GetKeyCode().IsShift() )
        {
            if( mpBrowser1->mpThemes->HasChildPathFocus( true ) )
                mpBrowser2->GetViewWindow()->GrabFocus();
            else if( mpBrowser2->GetViewWindow()->HasFocus() )
                mpBrowser2->maViewBox->GrabFocus();
            else if( mpBrowser2->maViewBox->HasFocus() )
                mpBrowser1->maNewTheme->GrabFocus();
            else
                mpBrowser1->mpThemes->GrabFocus();
        }
        else
        {
            if( mpBrowser1->mpThemes->HasChildPathFocus( true ) )
                mpBrowser1->maNewTheme->GrabFocus();
            else if( mpBrowser1->maNewTheme->HasFocus() )
                mpBrowser2->maViewBox->GrabFocus();
            else if( mpBrowser2->maViewBox->HasFocus() )
                mpBrowser2->GetViewWindow()->GrabFocus();
            else
                mpBrowser1->mpThemes->GrabFocus();
        }
    }

    return bRet;
}

void GalleryControl::GetFocus()
{
    Window::GetFocus();
    mpBrowser1->GrabFocus();
}

void GalleryControl::ThemeSelectionHasChanged()
{
    mpBrowser2->SelectTheme(mpBrowser1->GetSelectedTheme());
}

IMPL_LINK_NOARG( GalleryControl, SplitHdl )
{
    if(mpSplitter->IsHorizontal())
    {
        mpSplitter->SetPosPixel( Point( mpSplitter->GetSplitPosPixel(), mpSplitter->GetPosPixel().Y() ) );
    }
    else
    {
        mpSplitter->SetPosPixel( Point( mpSplitter->GetPosPixel().X(), mpSplitter->GetSplitPosPixel() ) );
    }

    Resize();

    return 0L;
}


} } // end of namespace svx::sidebar

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
