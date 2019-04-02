/*=============================================================================
   Copyright (c) 2016-2019 Joel de Guzman

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#include <photon.hpp>
#include <photon/support/draw_utils.hpp>

using namespace cycfi::photon;

// Main window background color
auto constexpr bkd_color = rgba(35, 35, 37, 255);

struct background : element
{
   void draw(context const& ctx)
   {
      auto& cnv = ctx.canvas;
      cnv.fill_style(bkd_color);
      cnv.fill_rect(ctx.bounds);
   }
};

using slider_ptr = std::shared_ptr<basic_slider_base>;
slider_ptr hsliders[3];
slider_ptr vsliders[3];

using dial_ptr = std::shared_ptr<dial_base>;
dial_ptr dials[3];

template <bool is_vertical>
auto make_markers()
{
   auto track = basic_track<5, is_vertical>();
   return slider_labels<10>(
      slider_marks<35>(track),         // Track with marks
      0.8,                             // Label font size (relative size)
      "0", "1", "2", "3", "4",         // Labels
      "5", "6", "7", "8", "9", "10"
   );
}

auto make_hslider(int index)
{
   hsliders[index] = share(slider(
      basic_thumb<25>(),
      make_markers<false>(),
      (index + 1) * 0.25
   ));
   return align_middle(xside_margin({ 20, 20 }, link(hsliders[index])));
}

auto make_hsliders()
{
   return hmin_size(300,
      vtile(
         make_hslider(0),
         make_hslider(1),
         make_hslider(2)
      )
   );
}

auto make_vslider(int index)
{
   vsliders[index] = share(slider(
      basic_thumb<25>(),
      make_markers<true>(),
      (index + 1) * 0.25
   ));
   return align_center(yside_margin({ 20, 20 }, link(vsliders[index])));
}

auto make_vsliders()
{
   return hmin_size(300,
      htile(
         make_vslider(0),
         make_vslider(1),
         make_vslider(2)
      )
   );
}

auto make_dial(int index)
{
   dials[index] = share(
      dial(
         radial_marks<20>(basic_knob<50>()),
         (index + 1) * 0.25
      )
   );

   auto markers = radial_labels<15>(
      link(dials[index]),
      0.7,                                   // Label font size (relative size)
      "0", "1", "2", "3", "4",               // Labels
      "5", "6", "7", "8", "9", "10"
   );

   return align_center_middle(markers);
}

auto make_dials()
{
   return xside_margin(20,
      vtile(
         make_dial(0),
         make_dial(1),
         make_dial(2)
      )
   );
}

auto make_controls()
{
   return
      margin({ 20, 10, 20, 10 },
         vmin_size(400,
            htile(
               margin({ 20, 20, 20, 20 }, pane("Vertical Sliders", make_vsliders(), 0.8f)),
               margin({ 20, 20, 20, 20 }, pane("Horizontal Sliders", make_hsliders(), 0.8f)),
               hspan(0.5, margin({ 20, 20, 20, 20 }, pane("Knobs", make_dials(), 0.8f)))
            )
         )
      );
}

void link_control(int index, view& view_)
{
   vsliders[index]->on_change =
      [index, &view_](double val)
      {
         hsliders[index]->slider_base::value(val);
         dials[index]->dial_base::value(val);
         view_.refresh(*hsliders[index]);
         view_.refresh(*dials[index]);
      };

   hsliders[index]->on_change =
      [index, &view_](double val)
      {
         vsliders[index]->slider_base::value(val);
         dials[index]->dial_base::value(val);
         view_.refresh(*vsliders[index]);
         view_.refresh(*dials[index]);
      };

   dials[index]->on_change =
      [index, &view_](double val)
      {
         vsliders[index]->slider_base::value(val);
         hsliders[index]->slider_base::value(val);
         view_.refresh(*vsliders[index]);
         view_.refresh(*hsliders[index]);
      };
}

void link_controls(view& view_)
{
   link_control(0, view_);
   link_control(1, view_);
   link_control(2, view_);
}

int main(int argc, const char* argv[])
{
   app _app(argc, argv);
   window _win(_app.name());
   _win.on_close = [&_app]() { _app.stop(); };

   view view_(_win.host());

   view_.content(
      {
         share(background{}),
         share(make_controls())
      }
   );

   link_controls(view_);
   _app.run();
   return 0;
}
