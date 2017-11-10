/* (c)  oblong industries */


#include "Greenhouse.h"
#include "EmotionDot.h"
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
  Trove<EmotionDot *> emotions;

  Meeting *meetingSummary;

  Dictionary<Str, float64> lastmsEmotion;


  Backgrounder() : Sketch ()
  {
    ParticipateInPool ("hackathon");

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
      meetingSummary->AddDataPoint (dict);
    }
  }

//  void MetabolizeNoiseLevel(Protein prt)
//  {
//    if (HasIngest (prt, "noise"))
//    {
//      float64 noiseLevel = Ingest<float64>(prt, "noise");
//      noiseLevel = (noiseLevel / 6.0 * 2.0) - 1.0;
//
//      Participant *p = participants.Nth(0);
//      if (noiseLevel > 0)
//        p->maxArausal = fabs(noiseLevel);
//      else
//        p->minArausal = fabs(noiseLevel);
//    }
//  }

  void MetabolizeNoiseLevel(Protein prt)
  {
    if (HasIngest (prt, "noise"))
    {
      float64 noiseLevel = Ingest<float64>(prt, "noise");
      noiseLevel = (noiseLevel / 6.0 * 2.0) - 1.0;

      Dictionary<Str, float64> dict;
      dict.Put("anger", boostValue(lastmsEmotion.Get("anger"), noiseLevel, true));
      dict.Put("contempt", boostValue(lastmsEmotion.Get("contempt"), noiseLevel, false));
      dict.Put("disgust", boostValue(lastmsEmotion.Get("disgust"), noiseLevel, false));
      dict.Put("fear", boostValue(lastmsEmotion.Get("fear"), noiseLevel, false));
      dict.Put("happiness", boostValue(lastmsEmotion.Get("happiness"), noiseLevel, true));
      dict.Put("neutral", lastmsEmotion.Get("neutral"));
      dict.Put("sadness", boostValue(lastmsEmotion.Get("sadness"), noiseLevel, false));
      dict.Put("surprise", boostValue(lastmsEmotion.Get("surprise"), noiseLevel, true));

//      meetingSummary->AddDataPoint (dict);
    }
  }

  float64 boostValue(float64 v, float64 boost, bool positiveCorrelation)
  {
//    float64 sign = boost >= 0 ? 1 : -1;
//    if (positiveCorrelation == boost > 0) {
//      return sign + 0,0001; //sign * sqrt(sign * v); // Make bigger
//    } else {
//      return sign - 0,0001; //* pow(v, 2); // Make smaller
//    }

    float64 sign = boost >= 0 ? 1 : -1;
    if (boost >= 0)
    {
      return positiveCorrelation ? (v*0.9 + 1.0*0.1) : v;
    }
    else
    {
      return positiveCorrelation ? (v*0.9 - 0.0*0.1) : v;
    }
  }

  void MetabolizeNoiseEmotion(Protein prt)
  {
    float64 Neutrality = Ingest<float64>(prt, "neutrality");
    float64 Happiness = Ingest<float64>(prt, "happiness");
    float64 Sadness = Ingest<float64>(prt, "sadness");
    float64 Anger = Ingest<float64>(prt, "anger");
    float64 Fear = Ingest<float64>(prt, "fear");


    Dictionary<Str, float64> dict;

    dict.Put ("anger", lastmsEmotion.ValFromKey("anger") * 0.8 + Anger * 0.2);
    dict.Put ("surprise", lastmsEmotion.ValFromKey("surprise") * 0.8 + Happiness * 0.2);
    dict.Put ("fear", lastmsEmotion.ValFromKey("fear") * 0.8 + Fear * 0.2);
    dict.Put ("sadness", lastmsEmotion.ValFromKey("sadness") * 0.8 + Sadness * 0.2);
    dict.Put ("contempt", lastmsEmotion.ValFromKey("contempt") * 0.9 + Neutrality * 0.1);
    dict.Put ("neutral", lastmsEmotion.ValFromKey("neutral") * 0.9 + Neutrality * 0.1);
    dict.Put ("disgust", lastmsEmotion.ValFromKey("disgust") * 0.9 + Neutrality * 0.1);
    dict.Put ("happiness", lastmsEmotion.ValFromKey("happiness") * 0.9 + Neutrality * 0.1);

    meetingSummary->AddDataPoint (dict);
  }
};

void Setup ()
{
  Backgrounder *b = new Backgrounder();
  b -> SlapOnFeld ();
}


