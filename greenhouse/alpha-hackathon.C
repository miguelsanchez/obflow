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

  bool settled;


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
    if (HasIngest (prt, "face-avg"))
    {
      Dictionary <Str,float64> dict = DictionaryFromIngest <Str,float64> (prt, "face-avg");
      meetingSummary->AddDataPoint (dict);

      lastmsEmotion = dict;
    }
  }

  void MetabolizeNoiseLevel(Protein prt)
  {
    if (HasIngest (prt, "noise"))
    {
      float64 noiseLevel = Ingest<float64>(prt, "noise");

      Dictionary<Str, float64> dict;
      dict.Put("anger", boostValue(lastmsEmotion.Get("anger"), noiseLevel, true));
      dict.Put("contempt", lastmsEmotion.Get("contempt"));
      dict.Put("disgust", boostValue(lastmsEmotion.Get("disgust"), noiseLevel, true));
      dict.Put("fear", lastmsEmotion.Get("fear"));
      dict.Put("happiness", lastmsEmotion.Get("happiness"));
      dict.Put("neutral", lastmsEmotion.Get("neutral"));
      dict.Put("sadness", boostValue(lastmsEmotion.Get("sadness"), noiseLevel, true));
      dict.Put("surprise", boostValue(lastmsEmotion.Get("surprise"), noiseLevel, true));

      meetingSummary->AddDataPoint (dict);
    }
  }

  float64 boostValue(float64 v, float64 boost, bool positiveCorrelation)
  {
    float64 sign = boost >= 1 ? 1 : -1;
    if (positiveCorrelation && sign)
    {
      return v*0.999 + 1.0*0.001;
    }
    else if (!positiveCorrelation && !sign)
    {
      return v*0.999 + 1.0*0.001;
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
  
  ///// Events
  
  void Blurt (BlurtEvent *e)
  {
    settled = false;
    ZeroTime();
    // 2D
    int64 translation_amount = 10;
    
    if (e -> Wordstamp () == "a") {
      IncTranslation (-translation_amount * Feld () -> Over ());
    } else if (e -> Wordstamp () == "d") {
      IncTranslation (translation_amount * Feld () -> Over ());
    } else if (e -> Wordstamp () == "w") {
      IncTranslation (translation_amount * 1 * Feld () -> Up ());
    } else if (e -> Wordstamp () == "s") {
      IncTranslation (-translation_amount * 1 * Feld () -> Up ());
    } else if (e -> Wordstamp () == "q") {
      CenterOnClosestKid();
    }
    
    //3D
    if (e -> Wordstamp () == "o") {
      IncTranslation (translation_amount * 2 * Feld () -> Norm ());
    } else if (e -> Wordstamp () == "l") {
      IncTranslation (-translation_amount * 2 * Feld () -> Norm ());
    }
  }
  
  void BlurtVanish (BlurtEvent *e)
  {
    ZeroTime();
  }
  
  void DisplacementAppear (DisplacementEvent *e)
  {
    if (IsHeedless ()) {
      Heed (e);
      INFORM ("Pose began at: " + ToStr (e -> EstabLoc ()));
    }
  }
  
  void DisplacementMove (DisplacementEvent *e)
  {
    if (IsHeedless ()) {
      Heed (e);
      INFORM ("Pose began at: " + ToStr (e -> EstabLoc ()));
    }
    
    if (e -> Wordstamp () == "v") {
      if (IsHeeding (e)) {
        IncTranslationHard (1.0 * e -> CurLinearDelta ());
        Vect total_disp = e -> CumuLinearOffset ();
        INFORM ("Total Displacement: " + ToStr (total_disp));
        ZeroTime();
        settled = false;
      }
    }
  }
  
  void DisplacementVanish (DisplacementEvent *e)
  {
    if (IsHeeding (e))
      StopHeeding ();
  }
  
  void CenterOnClosestKid ()
  {
    Thing *closestKid = NthKid <Thing> (0);
    if (! closestKid)
      return;
    
    Vect cent = Feld () -> Loc ();
    
    float64 closest_dist = cent . DistFrom (closestKid -> Loc ());
    for (int i = 1; i < KidCount(); i++)
    { Thing *kid = NthKid <Thing> (i);
      if (kid)
      { float64 dist = cent . DistFrom (kid -> Loc ());
        if (dist < closest_dist)
        { closest_dist = dist;
          closestKid = kid;
        }
      }
    }
    
    SetTranslation (Feld () -> Loc () - closestKid -> Translation ());
  }
  
  void Travail ()
  {
    if (CurTime() > 2.0 && ! settled) {
      CenterOnClosestKid ();
      settled = true;
    }
  }
};

void Setup ()
{
  RegisterDisplacement ("v", VictoryPose);
  
  Backgrounder *b = new Backgrounder();
  b -> SlapOnFeld ();
  b -> TranslationAnimateQuadratic(0.33);
}


