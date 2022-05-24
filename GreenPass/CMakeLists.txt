cmake_minimum_required(VERSION 3.22)
project(GreenPass C)

set(CMAKE_C_STANDARD 99)

add_executable(GreenPass ClientS.c Header/Client.h Header/include_functions.h ClientT.c Header/p_richiestaT.h Client.c ServerG.c Header/ServerG.h libreria/db_model.c libreria/db_model.h include_functions.c Header/ServerV.h ServerV.c CentroVaccinale.c)
