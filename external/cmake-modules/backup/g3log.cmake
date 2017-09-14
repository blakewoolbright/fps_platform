fps_add_external_component( 
    LABEL                "g3log"
    DESCRIPTION          "C++ logging library (https://github.com/KjellKod/g3log)"
    INCLUDE_DIRS         "/home/bwoolbright/dev/fps_toolchain/g3log/versions/g3log-20151229/include"
    REQUIRES             "libpthread.so"
    PROVIDES             "g3logger"
)
