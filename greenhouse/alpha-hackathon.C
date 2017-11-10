/* (c)  oblong industries */


#include "Greenhouse.h"
#include "EmotionDot.h"
#include "Participant.h"
#include "Meeting.h"


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


struct Backgrounder  :  public Sketch
{
  float64 radius;
  Trove<Participant *> participants;
  Trove<EmotionDot *> emotions;

  Meeting *meetingSummary;

  Backgrounder() : Sketch ()
  {
    ParticipateInPool ("hackathon");

    Dictionary<Str, float64> lastmsEmotion;

    radius = 50.0;

    Participant *p = new Participant();
    AppendKid (p);
    p->color = Color(0.0, 0.0, 1.0, 1.0);
    p->radius = radius;
    participants.Append (p);

//    EmotionDot *pp = new EmotionDot(radius);
//    pp->color = Color(1.0, 1.0, 1.0, 0.4);
//    AppendKid (pp);
//    emotions.Append (pp);

    meetingSummary = new Meeting();
    AppendKid (meetingSummary);
  }

  void Metabolize (const Protein &p)
  {
    if (HasDescrip (p, "ms-emotion"))
    {
      MetabolizeMSEmotion(p);
    }

    if (HasDescrip (p, "noise"))
    {
      MetabolizeNoiseLevel(p);
    }

    if (HasDescrip (p, "audiomood"))
    {
      MetabolizeNoiseEmotion(p);
    }
  }

  void MetabolizeMSEmotion(Protein prt)
  {
    OB_LOG_INFO("Hello");
    if (HasIngest (prt, "face-avg"))
    {
      Dictionary <Str,float64> dict = DictionaryFromIngest <Str,float64> (prt, "face-avg");

      float64 anger = dict.ValFromKey("anger");
      float64 contempt = dict.ValFromKey("contempt");
      float64 disgust = dict.ValFromKey("disgust");
      float64 fear = dict.ValFromKey("fear");
      float64 happiness = dict.ValFromKey("happiness");
      float64 neutral = dict.ValFromKey("neutral");
      float64 sadness = dict.ValFromKey("sadness");
      float64 surprise = dict.ValFromKey("surprise");

      Participant *p = participants.Nth(0);
      p->maxArausal = (anger + fear) / 2.0;
      p->minArausal = (disgust + sadness) / 2.0;
      p->maxValence = (fear + contempt + disgust) / 3.0;
      p->minValence = (surprise + happiness + neutral) / 3.0;

//      EmotionDot *pp = emotions.Nth(0);
//      pp->setRadius(10);
//      pp->maxArausal = (anger + fear) / 2.0;
//      pp->minArausal = (disgust + sadness) / 2.0;
//      pp->maxValence = (fear + contempt + disgust) / 3.0;
//      pp->minValence = (surprise + happiness + neutral) / 3.0;

      meetingSummary->AddDataPoint (dict);
    }
  }

  void MetabolizeNoiseLevel(Protein prt)
  {
    if (HasIngest (prt, "noise"))
    {
      float64 noiseLevel = Ingest<float64>(prt, "noise");
      noiseLevel = (noiseLevel / 6.0 * 2.0) - 1.0;

      Participant *p = participants.Nth(0);
      if (noiseLevel > 0)
        p->maxArausal = fabs(noiseLevel);
      else
        p->minArausal = fabs(noiseLevel);
    }
  }

  void MetabolizeNoiseEmotion(Protein prt)
  {
    float64 Neutrality = Ingest<float64>(prt, "neutrality");
    float64 Happiness = Ingest<float64>(prt, "happiness");
    float64 Sadness = Ingest<float64>(prt, "sadness");
    float64 Anger = Ingest<float64>(prt, "anger");
    float64 Fear = Ingest<float64>(prt, "fear");

    Participant *p = participants.Nth(0);
    p->maxArausal = (Anger + Neutrality) / 2.0;
    p->minArausal = (Sadness + Neutrality) / 2.0;
    p->maxValence = (Fear + Neutrality) / 2.0;
    p->minValence = (Happiness + Neutrality) / 2.0;

//    EmotionDot *pp = emotions.Nth(0);
//    pp->maxArausal = (Anger + Neutrality) / 2.0;
//    pp->minArausal = (Sadness + Neutrality) / 2.0;
//    pp->maxValence = (Fear + Neutrality) / 2.0;
//    pp->minValence = (Happiness + Neutrality) / 2.0;
  }

//  void DrawSelf ()
//  {
//    glLineWidth (2.0);
//
//    glColor4f (1.0, 0.0, 0.0, 1.0);
//
//    DrawLine (Vect (0, -radius, 0),
//              Vect (0, radius, 0));
//    DrawLine (Vect (-radius, 0, 0),
//              Vect (radius, 0, 0));
//    DrawLine (Vect (-radius * 2.0/3.0, -radius * 2.0/3.0, 0),
//              Vect (radius * 2.0/3.0, radius * 2.0/3.0, 0));
//    DrawLine (Vect (-radius * 2.0/3.0, radius * 2.0/3.0, 0),
//              Vect (radius * 2.0/3.0, -radius * 2.0/3.0, 0));
//  }

};

void Setup ()
{
  Backgrounder *b = new Backgrounder();
  b -> SlapOnFeld ();
}


