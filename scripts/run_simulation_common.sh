#!/bin/bash

display_help() {
cat << EOF
Usage: run_simulation [-c CONFIGURATION_NAME] -u [USER_INTERFACE]
Runs simulation using given configuration.

    -h          display this help
    -c          configuration name (specified in local omnetpp.ini)
    -u          user interface mode (e.g. Cmdenv, Qtenv, Tkenv)
EOF
}

CONFIG_NAME=""
UI_MODE=""

while getopts hc:u: opt; do
    case $opt in
        h)
            display_help
            exit 0
            ;;
        c)  CONFIG_NAME=$OPTARG
            ;;
        u)  UI_MODE=$OPTARG
            ;;
        *)
            display_help >&2
            exit 1
            ;;
        ?)
            display_help >&2
            exit 1
            ;;
    esac
done

if [ "$CONFIG_NAME" == "" ]; then
    display_help >&2
    exit 1
fi

if [ "$UI_MODE" == "" ]; then
    display_help >&2
    exit 1
fi
