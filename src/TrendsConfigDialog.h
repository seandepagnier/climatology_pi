/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Trends Plugin Friends
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#ifndef __TrendsConfigDIALOG_H__
#define __TrendsConfigDIALOG_H__

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "TrendsUI.h"

class TrendsConfigDialog : public TrendsConfigDialogBase {
public:

    TrendsConfigDialog(wxWindow *parent);
    ~TrendsConfigDialog();

private:

    void OnCycloneConfig();
    void OnCycloneConfig( wxCommandEvent& event ) { OnCycloneConfig(); }
    void OnCycloneConfig( wxDateEvent& event ) { OnCycloneConfig(); }
    void OnCycloneConfig( wxSpinEvent& event ) { OnCycloneConfig(); }

    wxWindow *pParent;
};

#endif

