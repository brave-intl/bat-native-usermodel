// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>
#include <iterator>

#include "stmr.h"
#include "bag_of_words_extractor.h"

int main(int argc, char* argv[]) {
  printf("Testing bat-native-usermodel...\n");

  usermodel::BagOfWords bow;

  std::string str = std::string("The Comme Up: David Bullock (Founder of 907 Agency) #Mood: Most Hilarious Hip-Hop Memes And GIFs For The 'G' Challenge On The Green Carpet Rate the Bars With N.O.R.E. FWD: Ella Mai Is the Soulstress of the Future Hip-Hop Hates Me: The Complexity in Being a Woman ‘For the Culture’ Exclusive: Watch Halle Berry React to Trey Songz Sliding Into Her DMs ‘for the Culture’ Detroit: Anthony Mackie Jada Pinkett Smith and Queen Latifah Talk Reuniting on Film In Bed With Hennessy Almanzar Gabby Sidibe Test Drives Fenty Beauty to Prove It's For All Skin Tones I Got Non-Invasive Plastic Surgery to Cheat My Way to a Toned Tummy Serena Williams Is Sharing Her Pregnancy Story And Introduces The World To Her Daughter Alexis See The Fashion Line Danielle Brooks Is Designing For All The BBWs Around The World Solange Just Went Platinum Blonde Maxine Waters Reclaiming Her Time Just Became Everyone's New Summer '17 Mantra Serena Williams Laid Out the Blueprint on 'How Black Women Can Close the Pay Gap' in This Moving Essay Terrifying Viral Video Shows Ohio State Fair Ride Malfunction in Mid-Air, Killing One and Injuring Many Face Value Hip Hop Awards 17 50 Central FACE VALUE HIP HOP AWARDS 50 CENTRAL Check out the freestyling gurus featured in the cypher! See everything we show you about the awards. Hot weather. Hot bars. There's a lotta love and mad hip-hop. THE QUEEN OF LATE NIGHT IS HERE 0 DAYS 0 HRS 0 MIN 0 SEC days Follow us Subscribe to our Newsletter Music Janet Jackson’s ‘State of the World’ Tour Proves Iconic Doesn’t Equal Ignorance Hip-Hop Hates Me: Women & the Culture FWD: THEY. Will Spark A Change in Music The Cypher Takeover The Best And Funniest Moments From Hip-Hop Awards '17 See Who Blessed The Hip-Hop Awards Instabooth See Where Cardi B, 2 Chainz, Keyshia Cole And More Will Sit Subscribe to our Newsletter  Latest in music Follow us Subscribe to our Newsletter HIP HOP AWARDS 2017 TUES, OCT 10 8P/7C HOST DJ KHALED");
  bow.process(str);
  for (auto item : bow.frequencies ) {
    std::cout << item.first << " " << item.second << std::endl;
  }

  return 0;
}
