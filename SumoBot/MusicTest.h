/*
 * A starts / stops the song.
 * B selects next song.
 */

// Variables.
byte songSelect = 0;
bool playSong = false;

// Saved Songs.
const char songCharge[] PROGMEM = {
  "! O4 T100 V15 L4 MS g12>c12>e12>G6>E12 ML>G2"
};
const char songFightanthem[] PROGMEM = {
  "! O5 L16 agafaea dac+adaea fa<aa<bac#a dac#adaea f"
  "O6 dcd<b-d<ad<g d<f+d<gd<ad<b- d<dd<ed<f+d<g d<f+d<gd<ad"
  "L8 MS <b-d<b-d MLe-<ge-<g MSc<ac<a ML d<fd<f O5 MS b-gb-g"
  "ML >c#e>c#e MS afaf ML gc#gc# MS fdfd ML e<b-e<b-"
  "O6 L16ragafaea dac#adaea fa<aa<bac#a dac#adaea faeadaca"
  "<b-acadg<b-g egdgcg<b-g <ag<b-gcf<af dfcf<b-f<af"
  "<gf<af<b-e<ge c#e<b-e<ae<ge <fe<ge<ad<fd"
  "O5 e>ee>ef>df>d b->c#b->c#a>df>d e>ee>ef>df>d"
  "e>d>c#>db>d>c#b >c#agaegfe f O6 dc#dfdc#<b c#4"
};
const char songTwinkleTwinkle[] PROGMEM = {
  "! V7 O4 c c g g a a g.. f f e e d d c.."
};

// Code to execute.
void musicTest(){

  // Playlist.
  switch(songSelect){
    case 0:{
      if(displayFlag) updateDisplay("Charge!");
      if(playSong){
        buzzer.playFromProgramSpace(songCharge);
        playSong = false;
      }
      break;
    }
    case 1:{
      if(displayFlag) updateDisplay("Fight"," Anthem");
      if(playSong){
        buzzer.playFromProgramSpace(songFightanthem);
        playSong = false;
      }
      break;
    }
    case 2:{
      if(displayFlag) updateDisplay("Twinkle"," Twinkle");
      if(playSong){
        buzzer.playFromProgramSpace(songTwinkleTwinkle);
        playSong = false;
      }
      break;
    }
    default:{
      songSelect = 0;
      break;
    }
  }

  // Controls.
  if(aButtonPressed){ // Play or stop song.
    if(buzzer.isPlaying()) {buzzer.stopPlaying();}
    else{playSong = true;}
  }
  if(bButtonPressed){songSelect++; displayFlag = true;} // Next song.
}
