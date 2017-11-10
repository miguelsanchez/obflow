//
//  Meeting.h
//  alpha-hackathon
//
//  Created by miguel on 9/11/17.
//
//

#include "EmotionDot.h"

#ifndef Meeting_h
#define Meeting_h

#define ANGER_COLOR      Color (255.0/255.0, 52.0/255.0, 0.0/255.0, 0.3)
#define FEAR_COLOR       Color (15.0/255.0, 15.0/255.0, 15.0/255.0, 0.3)
#define CONTEMPT_COLOR   Color (195.0/255.0, 3.0/255.0, 121.0/255.0, 0.3)
#define DISGUST_COLOR    Color (80.0/255.0, 0.0/255.0, 127.0/255.0, 0.3)
#define SADNESS_COLOR    Color (37.0/255.0, 116.0/255.0, 188.0/255.0, 0.3)
#define NEUTRAL_COLOR    Color (80.0/255.0, 71.0/255.0, 70.0/255.0, 0.3)
#define HAPPINESS_COLOR  Color (254.0/255.0, 98.0/255.0, 29.0/255.0, 0.3)
#define SURPRISE_COLOR   Color (255.0/255.0, 246.0/255.0, 82.0/255.0, 0.3)


struct Meeting  :  public Thing
{
  float64 radius;

  float64 anger;
  float64 contempt;
  float64 disgust;
  float64 fear;
  float64 happiness;
  float64 neutral;
  float64 sadness;
  float64 surprise;

  Trove<float64> emotions;

  Trove<EmotionDot *> angerDots;
  Trove<EmotionDot *> contemptDots;
  Trove<EmotionDot *> disgustDots;
  Trove<EmotionDot *> fearDots;
  Trove<EmotionDot *> happinessDots;
  Trove<EmotionDot *> neutralDots;
  Trove<EmotionDot *> sadnessDots;
  Trove<EmotionDot *> surpriseDots;


  int64 dataPointCount;

  EmotionDot *mainEmotion;
  Text *text;


  Meeting() : Thing ()
  {
    float64 slices = 8;
    float64 levels = 8;
    float64 angle;
    radius = 8;
    float64 levelStep = 8;


    for (int j = 0 ; j < levels ; j++)
    {
      angle = 360.0 / slices;
      for (int i = 0 ; i < slices ; i++ )
      {
        EmotionDot *emo = new EmotionDot (20);
        emo->radius = 20;

        float64 cs = cos(angle*i*PI/180);
        float64 sn = sin(angle*i*PI/180);
        float64 smid = sin(45*PI/180);
        float64 cmid = cos(45*PI/180);

        emo->SetTranslationHard(Loc ()
                                + Feld () -> Over () * (radius + levelStep * j) * cos(angle*i*PI/180)
                                + Feld () -> Up () * (radius + levelStep * j) * sin(angle*i*PI/180));

        if (cs >= 0 && cs <= cmid && sn <= 1 && sn >= smid)
        {
          surpriseDots.Append (emo);
          emo->SetAdjColor(SURPRISE_COLOR);
        }
        else if (cs > cmid && cs <= 1.0 && sn <= smid && sn >= 0.0)
        {
          happinessDots.Append (emo);
          emo->SetAdjColor(HAPPINESS_COLOR);
        }
        else if (cs > cmid && cs <= 1.0 && sn < 0.0 && sn > -smid)
        {
          neutralDots.Append (emo);
          emo->SetAdjColor(NEUTRAL_COLOR);
        }
        else if (cs > 0 && cs <= cmid && sn < -smid && sn >= -1.0)
        {
          sadnessDots.Append (emo);
          emo->SetAdjColor(SADNESS_COLOR);
        }
        else if (cs < 0 && cs >= -cmid && sn < -smid && sn >= -1.0)
        {
          disgustDots.Append (emo);
          emo->SetAdjColor(DISGUST_COLOR);
        }
        else if (cs < -cmid && cs >= -1.0 && sn < 0.0 && sn >= -smid)
        {
          contemptDots.Append (emo);
          emo->SetAdjColor(CONTEMPT_COLOR);
        }
        else if (cs < -cmid && cs >= -1.0 && sn < smid && sn >= 0.0)
        {
          fearDots.Append (emo);
          emo->SetAdjColor(FEAR_COLOR);
        }
        else if (cs <= 0 && cs >= -cmid && sn <= 1 && sn >= smid)
        {
          angerDots.Append (emo);
          emo->SetAdjColor(ANGER_COLOR);
        }

        AppendKid (emo);
      }
      slices += 10;
    }


    mainEmotion = new EmotionDot (2);
    mainEmotion->color = Color (1.0, 1.0, 1.0, 1.0);
    mainEmotion->radius = 80;
    AppendKid (mainEmotion);

    dataPointCount = 0;
  }

  void AddDataPoint(Dictionary<Str, float64> dict)
  {
    if (dataPointCount == 0)
    {
      for (int i = 0 ; i < 8 ; i++)
      {
        emotions.ReplaceNth (i, (dict.NthVal(i)));
      }
    }
    else
    {
      for (int i = 0 ; i < 8 ; i++)
      {
        emotions.Insert ((dict.NthVal(i) + emotions.Nth (i) * dataPointCount) / (dataPointCount + 1), i);
        OB_LOG_INFO ("%d, %f", i, emotions[i]);
      }
    }

    dataPointCount++;

    SetLargestColor();

    updateRadiusOn(angerDots, "anger", dict);
    updateRadiusOn(sadnessDots, "sadness", dict);
    updateRadiusOn(fearDots, "fear", dict);
    updateRadiusOn(happinessDots, "happiness", dict);
    updateRadiusOn(contemptDots, "contempt", dict);
    updateRadiusOn(neutralDots, "neutral", dict);
    updateRadiusOn(surpriseDots, "surprise", dict);
    updateRadiusOn(disgustDots, "disgust", dict);
  }

  void updateRadiusOn(Trove<EmotionDot *> dots, Str emotion, Dictionary<Str, float64>dict)
  {
    int convertFactor = 1000.0;
    int minRadius = 10.0;
    int maxRadius = 80.0;

    for (int i = 0 ; i < dots.Count () ; i++)
    {
      EmotionDot *dot = dots.Nth(i);
      dot->radius = Max(minRadius, Min(maxRadius, dict.ValFromKey(emotion) * convertFactor));
    }
  }

  void SetLargestColor ()
  {
    int64 largestIndex = 0;
    for (int i = 0 ; i < 8 ; i ++)
    {
      if (emotions.Nth (i) > emotions.Nth(largestIndex))
      {
        largestIndex = i;
      }
    }

    Color c = ColorForIndex (largestIndex);

    mainEmotion->SetAdjColor(c);
//    mainEmotion->setNewColor(c);
//    mainEmotion->radius = 2.0;
//    mainEmotion->IncTranslation(Vect(1.0, 1.0, 1.0));
//    IncTranslation (100.0 * Feld () -> Norm ());
  }

  Color ColorForIndex (int64 ix)
  {
    Color c;

    switch (ix)
    {
      case 0:
        c = ANGER_COLOR;
        break;
      case 1:
        c = CONTEMPT_COLOR;
        break;
      case 2:
        c = DISGUST_COLOR;
        break;
      case 3:
        c = FEAR_COLOR;
        break;
      case 4:
        c = HAPPINESS_COLOR;
        break;
      case 5:
        c = NEUTRAL_COLOR;
        break;
      case 6:
        c = SADNESS_COLOR;
        break;
      case 7:
        c = SURPRISE_COLOR;
        break;
    }

    return c;
  }


};

#endif /* Meeting_h */
