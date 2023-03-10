#!/bin/bash

echo '<html>'
echo '<head>'
echo '<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">'
echo '</head>'
echo '<body>'
echo '<pre>'
if (( RANDOM % 2 ))
then 
	/usr/games/fortune | /usr/games/cowsay
else 
	/usr/games/fortune | /usr/games/cowsay -f stegosaurus
fi
echo '<form METHOD=GET ACTION="/cgi-bin/cowsay.sh">'
echo '<button type="submit">Get new fortune.</button>'
echo '</form>'
echo '</pre>'
echo '</body>'
echo '</html>'

exit 0
