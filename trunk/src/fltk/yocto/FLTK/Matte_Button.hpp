#ifndef Fl_Matte_Button_H
#define Fl_Matte_Button_H

//
// Fl_Matte_Button.H -- A matte finished button widget
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

#include <FL/Fl.H>
#include <FL/Fl_Button.H>

#define FL_MATTE_BUTTON_HEIGHT 23		// buttons have fixed height -- just like Apple's..! ;)

#ifndef FL_ORANGE
#define FL_ORANGE Fl_Color(93)
#endif

namespace yocto 
{
    namespace FLTK
    {
        class  Matte_Button : public Fl_Button 
        {
        protected:
            void draw();
            void resize(int x,int y,int w,int) 
            {
                Fl_Button::resize(x,y,w,FL_MATTE_BUTTON_HEIGHT);	// no resizing of height
            }
        public:
            Matte_Button(int,int,int,int,const char * = 0);
            virtual ~Matte_Button() throw();
        private:
            Matte_Button( const Matte_Button & );
            Matte_Button &operator=( const Matte_Button & );
        };
        
    }
}
    
#endif 
