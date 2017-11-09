//
//  ViewController.swift
//  ObflowAudio
//
//  Created by Ivan Bella Lopez on 09/11/2017.
//  Copyright © 2017 Ivan Bella Lopez. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
  
  lazy var audioListener: AudioListener? = {
    [unowned self] in
    
    var audioListener = AudioListener.init (audioDataAvailableBlock: { (
      numberOfFramesAnalyzed,
      neutralScore,
      happinessScore,
      sadnessScore,
      angrinessScore,
      fearScore
      ) -> Void in
      
      let str = "descrips:\n- greenhouse\n- audiomood\ningests:\n" + "  neutrality: " + String(format:"%f\n", neutralScore)
     + "  happiness: " + String(format:"%f\n", happinessScore)
        + "  sadness: " + String(format:"%f\n", sadnessScore)
        + "  anger: " + String(format:"%f\n", angrinessScore)
        + "  fear: " + String(format:"%f\n", fearScore)
      
      /*
       descrips:
       - greenhouse
       - audiomood
       ingests:
         neutrality: 0.999126
         happiness: 0.000014
         sadness: 0.000700
         anger: 0.000091
         fear: 0.000069
      */
      
      print(str);
      self.saveData(audioData: str)
    })
    
    return audioListener
    }()
  
  override func viewDidLoad()
  {
    super.viewDidLoad()
  }
  
  override func viewWillAppear (_ animated: Bool)
  {
    super.viewWillAppear (animated);
    self.audioListener?.startListening()
  }
  
  override func viewWillDisappear(_ animated: Bool)
  {
    super.viewWillDisappear(animated);
    self.audioListener?.stopListening();
  }
  
  override func didReceiveMemoryWarning()
  {
    super.didReceiveMemoryWarning()
    // Dispose of any resources that can be recreated.
  }
  
  
  func saveData(audioData: String ) {
    let file = "protein"
    if let dir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first {
    
      let fileURL = dir.appendingPathComponent(file)
      do {
        try audioData.write(to: fileURL, atomically: false, encoding: .utf8)
      }
      catch { }
    }
  }
}

