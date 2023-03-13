#!/usr/bin/python3

import cgi, os

form = cgi.FieldStorage()
form["filename"]
print(form)