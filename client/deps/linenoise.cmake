add_library(pm3rrg_rdv4_linenoise STATIC
        linenoise-mob/utf8.c
        linenoise-mob/linenoise.c
)

target_compile_definitions(pm3rrg_rdv4_linenoise PRIVATE NDEBUG)
target_include_directories(pm3rrg_rdv4_linenoise INTERFACE linenoise-mob)
target_compile_options(pm3rrg_rdv4_linenoise PRIVATE -Wall -Werror -O3)
set_property(TARGET pm3rrg_rdv4_linenoise PROPERTY POSITION_INDEPENDENT_CODE ON)
