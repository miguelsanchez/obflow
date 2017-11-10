//
//  EmotionDotImage.h
//  alpha-hackathon
//
//  Created by miguel on 9/11/17.
//
//

#ifndef EmotionDotImage_h
#define EmotionDotImage_h


struct EmotionDotImage  :  public Image
{ float64 radius;

  Color color;

  EmotionDotImage (float64 r) : Image ()
  {
    radius = r;

//    SetBackingColor (Color (1.0, 1.0, 1.0, 1.0));

    SetImage (FetchImageData ("images/circle-hack.png"));

    TranslationAnimateChase (0.25);
    ScaleAnimateChase (0.75);
//    ColorAnimateAsymp (0.75);
  }

  void setRadius (float64 newRadius)
  {
    radius = (radius + newRadius) / 2.0;
    IncTranslation(Vect(1.0, 1.0, 1.0));
    IncScale(newRadius);
  }

  void setNewColor (Color c)
  {
    color = c;
    SetBorderColor (c);
    SetAdjColorHard (c);
  }

  void DrawSelf ()
  {
    SetBackingColor(color);
  }
};


#endif /* EmotionDotImage_h */
