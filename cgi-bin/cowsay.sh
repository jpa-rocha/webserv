#!/bin/bash

echo '<html>'
echo '<head>'
echo '<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">'
echo '</head>'
echo '<body>'
echo '<pre>'
/usr/games/fortune | /usr/games/cowsay
echo '<form id="fortune" METHOD=GET ACTION="/cgi-bin/cowsay.sh">'
echo '<button type="submit">Get new fortune.</button>'
echo '</form>'
echo '</pre>'
echo '</body>'
echo '</html>'

exit 0
