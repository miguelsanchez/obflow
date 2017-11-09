/* (c)  oblong industries */


#include "Greenhouse.h"

/**
  When using new resource files please be sure to do the following in order to
  ensure that your application can find those resources:

    - Place all resource files in:

        /opt/oblong/greenhouse/share

    - OR if you'd like to keep your resource files separated from the installed
         resources:

      Create a folder for your personal resource files and add the following
      line to your ~/.bash_profile (Mac) or ~/.bashrc (Linux) file:

        export OB_SHARE_PATH=[/path/to/new/share]:$OB_SHARE_PATH
**/


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
    radius = 50.0;
    maxArausal = 1.0;
    minArausal = 0.0;
    maxValence = 0.4;
    minValence = 0.6;

    color = Color(0.0, 0.0, 1.0, 1.0);
  }

  void DrawSelf ()
  {
    glLineWidth (2.0);

    glColor4f (color.R(), color.G(), color.B(), 1.0);

    glBegin (GL_LINES);
    glVertex (Vect(0, maxArausal * radius, 0));
    glVertex (Vect(0.5 * radius, 0.5 * radius, 0));
    glVertex (Vect(0.5 * radius, 0.5 * radius, 0));
    glVertex (Vect(maxValence * radius, 0, 0));
    glVertex (Vect(maxValence * radius, 0, 0));
    glVertex (Vect(0.5 * radius, -0.5 * radius, 0));
    glVertex (Vect(0.5 * radius, -0.5 * radius, 0));
    glVertex (Vect(0, -minArausal * radius, 0));
    glVertex (Vect(0, -minArausal * radius, 0));
    glVertex (Vect(-0.5 * radius, -0.5 * radius, 0));
    glVertex (Vect(-0.5 * radius, -0.5 * radius, 0));
    glVertex (Vect(-minValence * radius, 0, 0));
    glVertex (Vect(-minValence * radius, 0, 0));
    glVertex (Vect(-0.5 * radius, 0.5 * radius, 0));
    glVertex (Vect(-0.5 * radius, 0.5 * radius, 0));
    glVertex (Vect(0, maxArausal * radius, 0));
    glEnd ();
  }
};


struct Backgrounder  :  public Thing
{
  float64 radius;
  Trove<Participant *> participants;

  Backgrounder() : Thing ()
  {
    ParticipateInPool ("hackathon");

    radius = 50.0;

    Participant *p = new Participant();
    AppendKid (p);
    p -> color = Color(0.0, 0.0, 1.0, 1.0);
    participants.Append (p);
  }

  void Metabolize (const Protein &p)
  {
    if (HasDescrip (p, "background"))
    {
      if (HasIngest (p, "green"))
        SetFeldsColor (0.0, 1.0, 0.0);
      else if (HasIngest (p, "blue"))
        SetFeldsColor (0.0, 0.0, 1.0);
      else if (HasIngest (p, "red"))
        SetFeldsColor (1.0, 0.0, 0.0);
    }

    if (HasDescrip (p, "arousal"))
    {
      if (HasIngest (p, "value"))
      {
        float64 maxArausal = Ingest<float64>(p, "value");

        Participant *p = participants.Nth(0);
        p->maxArausal = maxArausal;
        p->minArausal = 1.0 - maxArausal;

      }
    }
  }

  void DrawSelf ()
  {
    glLineWidth (2.0);

      glColor4f (1.0, 0.0, 0.0, 1.0);

      glBegin (GL_LINES);

        glVertex (Vect (0, -radius, 0));
        glVertex (Vect (0, radius, 0));

      glEnd ();

      glBegin (GL_LINES);

        glVertex (Vect (-radius, 0, 0));
        glVertex (Vect (radius, 0, 0));

      glEnd ();

      glBegin (GL_LINES);

        glVertex (Vect (-radius/2.0, -radius/2.0, 0));
        glVertex (Vect (radius/2.0, radius/2.0, 0));

      glEnd ();

      glBegin (GL_LINES);

        glVertex (Vect (-radius/2.0, radius/2.0, 0));
        glVertex (Vect (radius/2.0, -radius/2.0, 0));

      glEnd ();
  }

};

void Setup ()
{
  Backgrounder *b = new Backgrounder();
  b -> SlapOnFeld ();
}


