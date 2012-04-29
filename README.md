# wadslip
*https://github.com/Slipyx/wadslip*

## About
wadslip is a Doom WAD info dumper and can also export a 2D map to SVG and PNG.
I started this a while ago in order to learn about the technical details of the
Doom WAD format and to also learn how to use C in a real project. It's been
sitting around untouched for some time now, so I decided to release it so maybe
someone else would find it and use it or even expand it. It's quite usable as
it is now, but some planned features are unfinished. The code is fairly well
commented so you shouldn't have too much trouble understanding it. I was also
just learning how to use C, so most of the code could be better written.

## License
Placed in the public domain. Not really sure if this is allowed. I know how
finicky these Doom project license issues can be. I hope no one will have
problems with this lack of license.

Uses the iniParser library by Nicolas Devillard which is under the MIT
license. See the included LICENSE.txt in src/iniparser for full information.

## Using
All you really need to do is specify the Doom WAD file to dump in config.ini
under the section 'Main'. Wadslip will dump the info to stdout unless piped to
a file. Specifying a map name will attempt to look for the map in the WAD file
and export a full 2D image of it to SVG and PNG. The settings under the
'MapDrawer' section are used to customize how the map is drawn. The drawThings
setting will draw a green square where the player 1 start is and colored
squares where all the keys are. The countThings setting will tally up all the
monsters and items that are on the map and dump the stats along with all the
other WAD info. So far, only monsters and powerups are counted.

## Dependencies
wadslip depends on the following libraries:

*   [cairo](http://www.cairographics.org/)
