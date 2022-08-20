/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#if !defined(ELEMENTS_WINDOW_MARCH_6_2019)
#define ELEMENTS_WINDOW_MARCH_6_2019

#include <string>
#include <functional>

#if defined(ELEMENTS_HOST_UI_LIBRARY_WIN32)
# include <Windows.h>
#elif defined(ELEMENTS_HOST_UI_LIBRARY_COCOA) && defined(__OBJC__)
# import <Cocoa/Cocoa.h>
#elif defined(ELEMENTS_HOST_UI_LIBRARY_GTK)
# include <gtk/gtk.h>
#elif defined(ELEMENTS_HOST_UI_LIBRARY_X11)
# include <X11/Xlib.h>
#endif

#include <infra/support.hpp>
#include <elements/support/rect.hpp>
#include <elements/base_view.hpp>

namespace cycfi { namespace elements
{
   ////////////////////////////////////////////////////////////////////////////
   // Window class
   ////////////////////////////////////////////////////////////////////////////
   class window : non_copyable
   {
   public:
#if defined(ELEMENTS_HOST_UI_LIBRARY_WIN32)
      using native_window_handle = HWND;
      static constexpr native_window_handle no_native_window = NULL;
#elif defined(ELEMENTS_HOST_UI_LIBRARY_COCOA) && defined(__OBJC__)
      using native_window_handle = NSWindow*;
      static constexpr native_window_handle no_native_window = nullptr;
#elif defined(ELEMENTS_HOST_UI_LIBRARY_GTK)
      using native_window_handle = GtkWindow*;
      static constexpr native_window_handle no_native_window = nullptr;
#elif defined(ELEMENTS_HOST_UI_LIBRARY_X11)
      using native_window_handle = Window;
      static constexpr native_window_handle no_native_window = None;
#endif

      enum style
      {
         with_title     = 1
       , closable       = 2
       , miniaturizable = 4
       , resizable      = 8

       , bare           = 0
       , standard       = with_title | closable | miniaturizable | resizable
      };

#if !defined(ELEMENTS_HOST_UI_LIBRARY_COCOA) || defined(__OBJC__)
                           window(
                              std::string const& name
                            , int style_ = standard
                            , rect const& bounds = rect{ 20, 20, 660, 500 }
                            , native_window_handle native_window = no_native_window
                           );

                           window(
                              int style_ = standard
                            , rect const& bounds = rect{ 20, 20, 660, 500 }
                            , native_window_handle native_window = no_native_window
                           )
                            : window("", style_, bounds, native_window)
                           {}
#endif

                           ~window();

      point                size() const;
      void                 size(point const& p);
      void                 limits(view_limits limits_);
      point                position() const;
      void                 position(point const& p);
      host_window_handle   host() const { return _window; }

      /////////////////////////////////////////////////////////////////////////
      // Notifications
      using callback_fn = std::function<void()>;

      callback_fn          on_close;

   private:

      host_window_handle   _window;
   };
}}

#endif

