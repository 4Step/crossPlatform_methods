// link.cpp
// Copyright (c) 2010-2012 by Troels K. All rights reserved.
// License: wxWindows Library Licence, Version 3.1 - see LICENSE.txt

#ifdef _MSC_VER

#include <wx/wx.h>

#if (wxVERSION_NUMBER >= 2900)
   #if wxUSE_UNICODE
      #ifdef _DEBUG
         #pragma comment(lib, "wxbase29ud.lib")
         #pragma comment(lib, "wxmsw29ud_core.lib")
         #pragma comment(lib, "wxmsw29ud_adv.lib")
         #pragma comment(lib, "wxmsw29ud_xrc.lib")
      #else
         #pragma comment(lib, "wxbase29u.lib")
         #pragma comment(lib, "wxbase29u_xml.lib")
         #pragma comment(lib, "wxmsw29u_core.lib")
         #pragma comment(lib, "wxmsw29u_adv.lib")
         #pragma comment(lib, "wxmsw29u_xrc.lib")
         #pragma comment(lib, "wxmsw29u_html.lib")
      #endif
   #else
      #ifdef _DEBUG
         #pragma comment(lib, "wxbase29d.lib")
         #pragma comment(lib, "wxmsw29d_core.lib")
         #pragma comment(lib, "wxmsw29d_adv.lib")
         #pragma comment(lib, "wxmsw29d_xrc.lib")
      #else
         #pragma comment(lib, "wxbase29.lib")
         #pragma comment(lib, "wxbase29_xml.lib")
         #pragma comment(lib, "wxmsw29_core.lib")
         #pragma comment(lib, "wxmsw29_adv.lib")
         #pragma comment(lib, "wxmsw29_xrc.lib")
         #pragma comment(lib, "wxmsw29_html.lib")
      #endif
   #endif
#elif (wxVERSION_NUMBER >= 2800)
   #if wxUSE_UNICODE
      #ifdef _DEBUG
         #pragma comment(lib, "wxbase28ud.lib")
         #pragma comment(lib, "wxmsw28ud_core.lib")
         #pragma comment(lib, "wxmsw28ud_adv.lib")
         #pragma comment(lib, "wxmsw28ud_xrc.lib")
      #else
         #pragma comment(lib, "wxbase28u.lib")
         #pragma comment(lib, "wxbase28u_xml.lib")
         #pragma comment(lib, "wxmsw28u_core.lib")
         #pragma comment(lib, "wxmsw28u_adv.lib")
         #pragma comment(lib, "wxmsw28u_xrc.lib")
         #pragma comment(lib, "wxmsw28u_html.lib")
      #endif
   #else
      #ifdef _DEBUG
         #pragma comment(lib, "wxbase28d.lib")
         #pragma comment(lib, "wxmsw28d_core.lib")
         #pragma comment(lib, "wxmsw28d_adv.lib")
         #pragma comment(lib, "wxmsw28d_xrc.lib")
      #else
         #pragma comment(lib, "wxbase28.lib")
         #pragma comment(lib, "wxbase28_xml.lib")
         #pragma comment(lib, "wxmsw28_core.lib")
         #pragma comment(lib, "wxmsw28_adv.lib")
         #pragma comment(lib, "wxmsw28_xrc.lib")
         #pragma comment(lib, "wxmsw28_html.lib")
      #endif
   #endif
#endif

#endif
