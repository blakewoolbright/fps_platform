#!/bin/bash

SCRIPT_NAME=$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")
SCRIPT_DIR=$(dirname "$(readlink -e "${0}")")

[[ -z ${SCRIPT_DIR} ]] && SCRIPT_DIR=$(readlink -f $(which "${SCRIPT_NAME}"))

ARG_SUMMARY=`getopt -o     hvb: \
                    --long help,verbose,binary: \
                    -n     "${SCRIPT_NAME}" -- "${@}" \
            `

eval set -- "${ARG_SUMMARY}"

ARG_HELP=
ARG_VERBOSE=
ARG_BINARY=
ARG_EXTRA=

while true ; do 
  case "${1}" in 
    -h | --help       ) ARG_HELP=1      ; shift   ; break   ;;
    -v | --verbose    ) ARG_VERBOSE=1   ; shift   ;;
    -b | --binary     ) ARG_BINARY=${2} ; shift 2 ;;
    --                ) shift           ; break   ;; 
    *                 ) shift                     ;; 
  esac 
done

ARG_EXTRA="${@}"

if [[ ! -z ${ARG_HELP} ]] ; then 
  echo -e "${USAGE_TEXT}" 
  exit 0 
fi

if [[ -z ${ARG_BINARY} ]] ; then 
  echo "" 1>&2
  echo "[ ${SCRIPT_NAME} ] :: ERROR :: Required argument '-b/--binary' (the target binary) not found"
  echo "" 1>&2
  exit 1
fi

`fpsmake --env`

CMDLINE="${ARG_BINARY} ${ARG_EXTRA}"

if [[ ! -z ${ARG_VEROSE} ]] ; then 
  echo "${SCRIPT_NAME} :: Running unit test command"
  echo "${SCRIPT_NAME} :: '${CMDLINE}'"
fi

${CMDLINE}
RV=${?}

if [[ ${RV} -ne 0 ]] ; then 
  echo "" 1>&2
  echo "[ ${SCRIPT_NAME} ] :: ERROR :: Test failed - Exit code: ${RV}"
  echo "" 1>&2
  exit 1
fi






