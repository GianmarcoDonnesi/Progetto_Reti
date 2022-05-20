cmake_minimum_required(VERSION 3.22)
project(GreenPass C)

set(CMAKE_C_STANDARD 99)

add_executable(GreenPass ClientS.c Header/p_auth.h Header/p_richiestaS.h Header/Client.h Header/include_functions.h ClientT.c Header/p_richiestaT.h Client.c ServerG.c Header/p_queryG.h Header/p_dbG.h Header/ServerG.h libreria/db_model.c libreria/db_model.h)
