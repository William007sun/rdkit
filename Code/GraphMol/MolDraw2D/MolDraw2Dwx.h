//
//   @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
// 
// Author: Igor Filippov based on the work of David Cosgrove (AstraZeneca)
//
// This is a concrete class derived from MolDraw2D that uses RDKit to draw a
// molecule into a wxDC

#ifndef MOLDRAW2DWX_H
#define MOLDRAW2DWX_H

#include <GraphMol/MolDraw2D/MolDraw2D.h>
#include <wx/dc.h>
#include <wx/font.h>
#include <wx/pen.h>
#include <wx/colour.h>
#include <wx/brush.h>

// ****************************************************************************

namespace RDKit {
  
  class MolDraw2Dwx : public MolDraw2D {

  public :

  MolDraw2Dwx( int width , int height , wxDC &dc ) : MolDraw2D( width , height ), m_dc(dc) 
    {
      // m_dc.SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    }

    // set font size in molecule coordinate units. That's probably Angstrom for
    // RDKit. It will turned into drawing units using scale_, which might be
    // changed as a result, to make sure things still appear in the window.

    void setFontSize( float new_size )
    {
      MolDraw2D::setFontSize( new_size );
      float font_size_in_points = fontSize() * scale();
      wxFont font = m_dc.GetFont(); 
      //font.SetPointSize(font_size_in_points);
      font.SetPixelSize(wxSize(0,font_size_in_points));
      m_dc.SetFont(font);
    }

    void setColour( const DrawColour &col )
    {
      MolDraw2D::setColour( col );
      float r = col.get<0>();
      float g = col.get<1>();
      float b = col.get<2>();
      wxColour colour(r * 255, g * 255, b * 255);
      m_dc.SetTextForeground(colour);
      m_dc.SetPen(wxPen(colour));
      m_dc.SetBrush(wxBrush(colour));      
    }


  private :

    wxDC &m_dc;

    void drawLine( const std::pair<float,float> &cds1,  const std::pair<float,float> &cds2 )
    {
      std::pair<float,float> c1 = getDrawCoords( cds1 );
      std::pair<float,float> c2 = getDrawCoords( cds2 ); 
      m_dc.DrawLine(c1.first,c1.second,c2.first,c2.second);
    }

    void drawChar( char c , const std::pair<float,float> &cds )
    {
      m_dc.DrawText(wxString(c),cds.first,cds.second);
    }

    void drawTriangle( const std::pair<float,float> &cds1, const std::pair<float,float> &cds2, const std::pair<float,float> &cds3 )
    {
      std::pair<float,float> c1 = getDrawCoords( cds1 );
      std::pair<float,float> c2 = getDrawCoords( cds2 );
      std::pair<float,float> c3 = getDrawCoords( cds3 );
      wxPoint lines[] = {wxPoint(c1.first,c1.second), wxPoint(c2.first,c2.second), wxPoint(c3.first,c3.second)};
      m_dc.DrawPolygon(3,lines);  
    }

    void clearDrawing()
    {
      wxColour backgroundColour = m_dc.GetTextBackground();
      if (!backgroundColour.Ok())
        backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);    
      const wxBrush &brush = m_dc.GetBrush();
      const wxPen &pen = m_dc.GetPen(); 
      m_dc.SetBrush(wxBrush(backgroundColour));
      m_dc.SetPen(wxPen(backgroundColour, 1));
      m_dc.DrawRectangle(0,0, width(), height());
      m_dc.SetBrush(brush);
      m_dc.SetPen(pen);
    }

    // using the current scale, work out the size of the label in molecule coordinates
    void getStringSize( const std::string &label, float &label_width,  float &label_height ) const
    {
      if (m_dc.CanGetTextExtent())
	{
	  wxCoord width, height;
	  m_dc.GetTextExtent(wxString(label), &width, &height);
	  label_width = float(width) / scale();
	  label_height = float(height) / scale();
	}
    }

  };

}
#endif // MOLDRAW2DWX_H
