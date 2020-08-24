#!/bin/bash
#
# helper to insert or copy specialchars from rofi plugin
#
# copy/paste in Linux is a nightmare, so I simply rely on third-party tools
# here. To Copy to paste buffer...
# If notify-send is installed, a notification will be send when a char is
# pasted to the clipboard.
# To insert text directly, xdotool is used.
#
# Please install depends accordingly.
#

LANG=C
set -e

function help() {
    echo "$( basename "${0}" ) - helper to copy/insert from rofy-plugin-specialchars"
    echo
    echo "$( basename "${0}" ) [action] [value]"
    echo
    echo "Available actions:"
    echo " copy"
    echo " insert"
    echo
    echo "Example:"
    echo " > $( basename "${0}" ) copy 😀"
    echo " > $( basename "${0}" ) insert U1F917"
}

function send_notification() {
    local value="$1"
    if which notify-send >/dev/null ; then
        notify-send "${value} copied to clipboard"
    fi
}

function insert() {
    local value="$1"

    sleep 1

    if which xdotool >/dev/null ; then
        xdotool key "$value"
    else
        echo "xdotool not found." >&2
        exit 1
    fi
}

function copy() {
    local value="$1"

    if which xsel >/dev/null ; then
        printf "%s" "$value" | xsel --clipboard --input
        printf "%s" "$value" | xsel --primary --input
    elif which xclip >/dev/null ; then
        printf "%s" "$value" | xclip -selection clipboard -in
        printf "%s" "$value" | xclip -selection primary -in
    else
        echo "No tool to copy to clipboard found. Please install 'xsel' or 'xclip'" >&2
        exit 1
    fi

    send_notification "$1"
}

function copy_insert() {
    local action="$1"
    local value="$2"

    case "$action" in

        help|-help|--help|-h)
            help
            ;;

        insert)
            insert "$value"
            ;;

        copy)
            copy "$value"
            ;;

        *)
            help >&2
            echo -e "\nUnknown Action: $action" >&2
            exit 1
            ;;

    esac
}

copy_insert "$1" "$2"
