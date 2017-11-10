//
//  EmotionDot.h
//  alpha-hackathon
//
//  Created by miguel on 9/11/17.
//
//

#ifndef EmotionDot_h
#define EmotionDot_h

struct EmotionDot  :  public Points
{ int64 num;
  float64 radius;

  float64 maxArausal;
  float64 minArausal;
  float64 maxValence;
  float64 minValence;

  Color color;

  Str emotionStr;

  EmotionDot (float64 r) : Points ()
  {
    radius = r;

    LoadShaders ("shaders/hackathon-foggy.vert", "shaders/null.frag");

    SetVertexCount (10);

    SetVertexLoc (0, Loc ());
    SetPointSize(0, 50);

    SetVerticesReady ();

    TranslationAnimateChase (0.25);
    ScaleAnimateChase (0.75);
    ColorAnimateAsymp (0.75);
  }

  void AssignShaderInputs ()
  { SetShaderUniform ("fog_radius", 12);
    SetShaderUniform ("system_distance", Loc () . DistFrom (CameraLoc ()));
    SetShaderUniform ("camera_position", CameraLoc ());
    SetShaderUniform ("pointsize", radius);
    SetShaderUniform ("dotcolor", AdjColor ());
  }
};


#endif /* EmotionDot_h */
