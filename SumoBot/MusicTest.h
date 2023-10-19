/*
 * A plays the song.
 * B stops the song.
 * Maybe eventually I'll add a menu to select songs.
 */

// Variables.
byte songSelect = 0;
const char musicCharge[] PROGMEM = {
  "O4 T100 V15 L4 MS g12>c12>e12>G6>E12 ML>G2",
};
const char musicFightanthem[] PROGMEM = {
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
const char musicTwinkleTwinkle[] PROGMEM = {
  "V7 O4 c c g g a a g.. f f e e d d c.."
};

// Code to execute.
void musicTest(){
  if(aButtonPressed) buzzer.playFromProgramSpace(musicTwinkleTwinkle);
  if(bButtonPressed) buzzer.stopPlaying();
}
