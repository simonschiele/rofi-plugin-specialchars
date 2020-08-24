#!/bin/bash
#
# fetch test-list of emojis from unicode.org and reformats as usable list for
# our rofi-plugin.
#

set -e

echo "Fetching emojis list"
wget -q \
    https://unicode.org/Public/emoji/13.0/emoji-test.txt

echo "Converting emojis lists"
grep -v -e '^$' -e '^#' emoji-test.txt \
    | sed "s/^\(.*\)\ *;.*#\ \(.*\)*\ [E0-9\.]*\ \(.*\)$/\2\t\t\3\t\t\1/g" \
    | sed "s/\ *$//g" > emojis.txt

grep -v -e '^$' -e '^#' emoji-test.txt \
    | grep "#\ .\ " \
    | sed "s/^\(.*\)\ *;.*#\ \(.*\)*\ [E0-9\.]*\ \(.*\)$/\2\t\t\3\t\t\1/g" \
    | sed "s/\ *$//g" > emojis-simple.txt

echo "Cleanup"
rm -f emoji-test.txt
