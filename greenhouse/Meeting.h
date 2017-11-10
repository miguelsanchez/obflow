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
#define FEAR_COLOR       Color (0.0/255.0, 0.0/255.0, 0.0/255.0, 0.3);
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

  Trove<float64> angerDots;
  Trove<float64> contemptDots;
  Trove<float64> disgustDots;
  Trove<float64> fearDots;
  Trove<float64> happinessDots;
  Trove<float64> neutralDots;
  Trove<float64> sadnessDots;
  Trove<float64> surpriseDots;


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
//        emo->color = SADNESS_COLOR;
        emo->radius = 20;
        emo->SetTranslationHard(Loc ()
                                + Feld () -> Over () * (radius + levelStep * j) * cos(angle*i*PI/180)
                                + Feld () -> Up () * (radius + levelStep * j) * sin(angle*i*PI/180));

        OB_LOG_INFO("%f, %f", cos(angle*i*PI/180),  sin(angle*i*PI/180));
        if (cos(angle*i*PI/180) > cos(0) && sin(angle*i*PI/180) > 0.0)
        {
          emo->SetAdjColor(SADNESS_COLOR);
        }
        else if (cos(angle*i*PI/180) > cos(0) && sin(angle*i*PI/180) < 0.0)
        {
          emo->SetAdjColor(SURPRISE_COLOR);
        }
        else if (cos(angle*i*PI/180) > cos(0) && sin(angle*i*PI/180) > 0.0)
        {
          emo->SetAdjColor(SADNESS_COLOR);
        }
        else if (cos(angle*i*PI/180) > cos(0) && sin(angle*i*PI/180) < 0.0)
        {
          emo->SetAdjColor(SURPRISE_COLOR);
        }

        AppendKid (emo);
      }
      slices += 10;
    }


    mainEmotion = new EmotionDot (2);
    mainEmotion->color = Color (1.0, 1.0, 1.0, 1.0);
    mainEmotion->radius = 80;
//    AppendKid (mainEmotion);

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
    mainEmotion->radius = 2.0;
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
