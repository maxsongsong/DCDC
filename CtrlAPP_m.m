clc;
%dos('copySrc')
%dos('copyController_alg')
mex -I"Inc"...
    -I"Src"...
    -I"Inc/include"...
    -I"app"...
    -I"app/alg" ...
    -I"app/task" ...
    -I"bsp_m"...
    -I"lib"...
    -I"fsm"...
     CtrlAPP.cpp ...
     app/*.c ...
     bsp_m/*.c ...
     app/alg/*.c ...
     app/task/*.c ...
     lib/*.c...
     fsm/*.c...


