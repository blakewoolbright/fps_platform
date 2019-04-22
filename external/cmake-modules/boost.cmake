fps_add_external_component( 
    LABEL                "boost"
    DESCRIPTION          "Boost C++ Libraries v1.56.0"
    INCLUDE_DIRS         "/home/bwoolbright/dev/fps_toolchain/boost/versions/boost-1.56.0/include"
    REQUIRES             "libpthread.so"
    PROVIDES             "boost_system boost_program_options boost_filesystem boost_iostreams boost_regex boost_thread boost_date_time boost_serialization"
)
