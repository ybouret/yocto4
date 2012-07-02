#include "yocto/FLTK/Matte_Button.hpp"

#include <FL/fl_draw.H>
#include <FL/Fl_Pixmap.H>

//
// Fl_Matte_Button.cxx -- A matte finished button widget
//
// Copyright 2008 by Greg Ercolano.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//

// XPMS FOR Fl_Matte_Button                      //
//          _______________________              //
//         /                       \             //
//        |          Button         |            //
//         \_______________________/             //
//                                               //
//        |___| |______________| |__|            //
//          lt         ct         rt             //
//                                               //

namespace yocto  
{
    
    namespace FLTK
    {
        ////// RED //////
#include "xpms/rt_red_up.xpm"
#include "xpms/ct_red_up.xpm"
#include "xpms/lt_red_up.xpm"
#include "xpms/rt_red_dn.xpm"
#include "xpms/ct_red_dn.xpm"
#include "xpms/lt_red_dn.xpm"
        ////// ORANGE //////
#include "xpms/rt_orn_up.xpm"
#include "xpms/ct_orn_up.xpm"
#include "xpms/lt_orn_up.xpm"
#include "xpms/rt_orn_dn.xpm"
#include "xpms/ct_orn_dn.xpm"
#include "xpms/lt_orn_dn.xpm"
        ////// YELLOW //////
#include "xpms/rt_yel_up.xpm"
#include "xpms/ct_yel_up.xpm"
#include "xpms/lt_yel_up.xpm"
#include "xpms/rt_yel_dn.xpm"
#include "xpms/ct_yel_dn.xpm"
#include "xpms/lt_yel_dn.xpm"
        ////// GREEN //////
#include "xpms/rt_grn_up.xpm"
#include "xpms/ct_grn_up.xpm"
#include "xpms/lt_grn_up.xpm"
#include "xpms/rt_grn_dn.xpm"
#include "xpms/ct_grn_dn.xpm"
#include "xpms/lt_grn_dn.xpm"
        ////// CYAN //////
#include "xpms/rt_cyn_up.xpm"
#include "xpms/ct_cyn_up.xpm"
#include "xpms/lt_cyn_up.xpm"
#include "xpms/rt_cyn_dn.xpm"
#include "xpms/ct_cyn_dn.xpm"
#include "xpms/lt_cyn_dn.xpm"
        ////// GRAY //////
#include "xpms/rt_gry_up.xpm"
#include "xpms/ct_gry_up.xpm"
#include "xpms/lt_gry_up.xpm"
#include "xpms/rt_gry_dn.xpm"
#include "xpms/ct_gry_dn.xpm"
#include "xpms/lt_gry_dn.xpm"
        
        
        // Draw tab's box
        static void DrawBox(int isup, int x, int y, int w, int h, Fl_Color c) {
            // RED
            static Fl_Pixmap lt_red_up(lt_red_up_xpm); static Fl_Pixmap lt_red_dn(lt_red_dn_xpm);
            static Fl_Pixmap ct_red_up(ct_red_up_xpm); static Fl_Pixmap ct_red_dn(ct_red_dn_xpm);
            static Fl_Pixmap rt_red_up(rt_red_up_xpm); static Fl_Pixmap rt_red_dn(rt_red_dn_xpm);
            // ORANGE
            static Fl_Pixmap lt_orn_up(lt_orn_up_xpm); static Fl_Pixmap lt_orn_dn(lt_orn_dn_xpm);
            static Fl_Pixmap ct_orn_up(ct_orn_up_xpm); static Fl_Pixmap ct_orn_dn(ct_orn_dn_xpm);
            static Fl_Pixmap rt_orn_up(rt_orn_up_xpm); static Fl_Pixmap rt_orn_dn(rt_orn_dn_xpm);
            // YELLOW
            static Fl_Pixmap lt_yel_up(lt_yel_up_xpm); static Fl_Pixmap lt_yel_dn(lt_yel_dn_xpm);
            static Fl_Pixmap ct_yel_up(ct_yel_up_xpm); static Fl_Pixmap ct_yel_dn(ct_yel_dn_xpm);
            static Fl_Pixmap rt_yel_up(rt_yel_up_xpm); static Fl_Pixmap rt_yel_dn(rt_yel_dn_xpm);
            // GREEN
            static Fl_Pixmap lt_grn_up(lt_grn_up_xpm); static Fl_Pixmap lt_grn_dn(lt_grn_dn_xpm);
            static Fl_Pixmap ct_grn_up(ct_grn_up_xpm); static Fl_Pixmap ct_grn_dn(ct_grn_dn_xpm);
            static Fl_Pixmap rt_grn_up(rt_grn_up_xpm); static Fl_Pixmap rt_grn_dn(rt_grn_dn_xpm);
            // CYAN
            static Fl_Pixmap lt_cyn_up(lt_cyn_up_xpm); static Fl_Pixmap lt_cyn_dn(lt_cyn_dn_xpm);
            static Fl_Pixmap ct_cyn_up(ct_cyn_up_xpm); static Fl_Pixmap ct_cyn_dn(ct_cyn_dn_xpm);
            static Fl_Pixmap rt_cyn_up(rt_cyn_up_xpm); static Fl_Pixmap rt_cyn_dn(rt_cyn_dn_xpm);
            // GRAY
            static Fl_Pixmap lt_gry_up(lt_gry_up_xpm); static Fl_Pixmap lt_gry_dn(lt_gry_dn_xpm);
            static Fl_Pixmap ct_gry_up(ct_gry_up_xpm); static Fl_Pixmap ct_gry_dn(ct_gry_dn_xpm);
            static Fl_Pixmap rt_gry_up(rt_gry_up_xpm); static Fl_Pixmap rt_gry_dn(rt_gry_dn_xpm);
            
            // DETERMINE WHICH LEFT/CENTER/RIGHT PIXMAPS TO DRAW
            Fl_Pixmap *lt = 0;
            Fl_Pixmap *ct = 0;
            Fl_Pixmap *rt = 0;
            switch ( c ) {
                case FL_RED:
                    if ( isup ) { lt = &lt_red_up; ct = &ct_red_up; rt = &rt_red_up; }
                    else        { lt = &lt_red_dn; ct = &ct_red_dn; rt = &rt_red_dn; }
                    break;
                case FL_ORANGE:
                    if ( isup ) { lt = &lt_orn_up; ct = &ct_orn_up; rt = &rt_orn_up; }
                    else        { lt = &lt_orn_dn; ct = &ct_orn_dn; rt = &rt_orn_dn; }
                    break;
                case FL_YELLOW:
                    if ( isup ) { lt = &lt_yel_up; ct = &ct_yel_up; rt = &rt_yel_up; }
                    else        { lt = &lt_yel_dn; ct = &ct_yel_dn; rt = &rt_yel_dn; }
                    break;
                case FL_GREEN:
                    if ( isup ) { lt = &lt_grn_up; ct = &ct_grn_up; rt = &rt_grn_up; }
                    else        { lt = &lt_grn_dn; ct = &ct_grn_dn; rt = &rt_grn_dn; }
                    break;
                case FL_CYAN:
                    if ( isup ) { lt = &lt_cyn_up; ct = &ct_cyn_up; rt = &rt_cyn_up; }
                    else        { lt = &lt_cyn_dn; ct = &ct_cyn_dn; rt = &rt_cyn_dn; }
                    break;
                default:
                case FL_GRAY:
                    if ( isup ) { lt = &lt_gry_up; ct = &ct_gry_up; rt = &rt_gry_up; }
                    else        { lt = &lt_gry_dn; ct = &ct_gry_dn; rt = &rt_gry_dn; }
                    break;
            }
            
            int lt_w = lt->w();
            int ct_w = ct->w();
            int rt_w = rt->w();
            
            // DRAW LEFT SIDE OF BUTTON
            lt->draw(x,y);
            
            // DRAW CENTER OF BUTTON
            fl_push_clip(x,y,w-rt_w,h);
            for ( int xx=x+lt_w; xx<=x+w-rt_w; xx += ct_w ) ct->draw(xx,y);
            fl_pop_clip();
            
            // DRAW RIGHT SIDE OF BUTTON
            rt->draw(x+w-rt_w,y);
        }
        
        // Draw
        void Matte_Button::draw() 
        {
            if ( value() ) DrawBox(0, x(), y(), w(), h(), down_color());
            else           DrawBox(1, x(), y(), w(), h(), color());
            Fl_Button::draw_label();
            // DRAW FOCUS BOX
            if (Fl::focus() == this) draw_focus(FL_UP_BOX, x()+3, y()+1, w()-5, h()-1);
        }
        
        // CTOR
        Matte_Button:: Matte_Button(int X,int Y,int W, int , const char *l) : Fl_Button(X,Y,W,FL_MATTE_BUTTON_HEIGHT,l) {
        }
        
        Matte_Button:: ~Matte_Button() throw() {}
    }
    
}


