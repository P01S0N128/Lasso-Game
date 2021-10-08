CS101 Project
 -Made by Kushagra Mishra (200010042)

Link to the demo video: https://drive.google.com/file/d/10o54hhXU13PglUL-sCNwVP6GWuFAbmH6/view?usp=sharing
Controls:

  1) '-': Decrease the Lasso Tossing Speed
  2) '=': Increase the Lasso Tossing Speed
  3) '[': Decrease the Lasso Tossing Angle
  4) ']': Increase the Lasso Tossing Angle
  5) 'T': Toss the Lasso
  6) 'L': Loop the Lasso
  7) 'Y': Yank the Lasso 
  8) 'Q': Quit the game

Lasso Components:

  1) Lasso: White band attached to a Red Centre. It is Used to catch the coins.

  2) Coin:  Yellow colored circle. Main aim of the game is to catch this coin. Catching a coin increases the score by one.

  3) Bomb: Red colored circle. Avoid catching these buggers. Catching one reduces the score by one.

  4) Special Coin: Blue colored circle. The are just like normal coins but increase the score by two instead of one and give an additional time boost on
                   level 3.


Level 1 Rules:

  Level 1 is the easiest level of the game and is meant to familiarise the player with the controls of the game. There are no bombs or score penalties
  this level. The player has to catch 15 coins within 60 seconds to advance to the next level.

Level 2 Rules: 

  Level 2 is the intermediate level of the game. This level introduces 2 new features. One is the bomb, which reduces the score by one. The concept of 
  lives is introduced here as well. Your lives only decrease if your score goes below 0. go below a score of -3 and you lose the game. The player has
  to catch 15 coins within 60 seconds to advance to the next level.

Level 3 Rules:
  
  Level 3 is the final and the hardest level of the game. This level is the same as level 2 but introduces a penalty of 1 score on failure to catch a 
  coin. The special coin also gives a time boost of 2 seconds on this level. The player has to catch 20 coins within 70 seconds to win the game.

Modifications added:
   1) Made changes to the appearance of the game (Changed background color, Added Timer, Level indicator, the cowboy etc.).
 
   2) Added a bomb which subtracts the score if caught (Red in color).

   3) Added a special coin (Blue in color) which increases the score by 2 instead of 1 and additionally gives a time boost
      of 2 seconds for level 1 and 2 and 5 seconds for level 3.

   4) Made the coin and the bomb start from random positions and have varying trajectories (Trajectory will always be a parabola for the coin but a bit
      more generalized for the bomb).

   5) Added a timer to the game. If the timer runs out, the game ends.

   6) Added 3 separate levels, each getting progressively harder.
              
   7) A list of controls is displayed on the game screen while playing.




