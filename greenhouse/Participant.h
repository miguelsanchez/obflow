//
//  Participant.h
//  alpha-hackathon
//
//  Created by miguel on 9/11/17.
//
//

#include "EmotionDot.h"

#ifndef Participant_h
#define Participant_h

struct Participant  :  public Thing
{
  Color color;

  float64 radius;

  float64 maxArausal;
  float64 minArausal;
  float64 maxValence;
  float64 minValence;

  Participant() : Thing ()
  {
    maxArausal = 1.0;
    minArausal = 1.0;
    maxValence = 1.0;
    minValence = 1.0;
  }

  void DrawSelf ()
  {
    glLineWidth (2.0);

    glColor4f (color.R(), color.G(), color.B(), 1.0);

    //    glBegin (GL_LINES);
    //    glVertex (Vect(0, maxArausal * radius, 0));
    //    glVertex (Vect(0.5 * radius, 0.5 * radius, 0));
    //    glVertex (Vect(0.5 * radius, 0.5 * radius, 0));
    //    glVertex (Vect(maxValence * radius, 0, 0));
    //    glVertex (Vect(maxValence * radius, 0, 0));
    //    glVertex (Vect(0.5 * radius, -0.5 * radius, 0));
    //    glVertex (Vect(0.5 * radius, -0.5 * radius, 0));
    //    glVertex (Vect(0, -minArausal * radius, 0));
    //    glVertex (Vect(0, -minArausal * radius, 0));
    //    glVertex (Vect(-0.5 * radius, -0.5 * radius, 0));
    //    glVertex (Vect(-0.5 * radius, -0.5 * radius, 0));
    //    glVertex (Vect(-minValence * radius, 0, 0));
    //    glVertex (Vect(-minValence * radius, 0, 0));
    //    glVertex (Vect(-0.5 * radius, 0.5 * radius, 0));
    //    glVertex (Vect(-0.5 * radius, 0.5 * radius, 0));
    //    glVertex (Vect(0, maxArausal * radius, 0));
    //    glEnd ();
  }
};


#endif /* Participant_h */
