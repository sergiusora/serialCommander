# serialCommander
library for arduino
compiles at under 6Kbytes.

you send a comand structured as : cod atr1 atr2\n
delimiter is SPACE;
cod is a 3 char combination defined by you in arduino project;
atr1 and atr2 can be any nr from -32767...32767, this atributes can be missing and the return is 0;

useful for setting and reading parameters (best in array) or simple bit set/get and function calls; 

