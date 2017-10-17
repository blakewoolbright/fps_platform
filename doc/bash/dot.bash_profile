#!/bin/bash

export EDITOR=/usr/bin/vim
export TBW_DEV="${HOME}/dev"
export TBW_FPS="${HOME}/dev/fps_platform"
export TBW_TCD="${HOME}/dev/fps_toolchain"
# export TBW_CXX="${TBW_TCD}/gcc/versions/gcc-4.9.3"

if [[ -d "${HOME}/dev/fps_platform" ]] ; then 
  export PATH="${HOME}/dev/fps_platform/scripts:${PATH}"
fi

alias fpsroot='cd ${TBW_FPS}'
alias fpslib='cd ${TBW_FPS}/cpp/lib'
alias gdbx='gdb -ex run '

# export FPS_TOOLCHAIN_DIR=${TBW_TCD}
export FPS_COMPILER_DIR=${TBW_CXX}

# Disable terminal flow control
stty -ixon

