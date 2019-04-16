#include "ploting.h"
#include<time.h>
#include<cmath>

void plotPareto3D(std::vector<std::vector<float>> frontier){
    #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif

        if (pipe != NULL)
        {
            srand(time(nullptr));
            fprintf(pipe, "set term wx\n");         // set the terminal
            fprintf(pipe, "set hidden3d\n");
            fprintf(pipe, "set dgrid3d 50,50 qnorm 2\n");
            fprintf(pipe, "splot '-' with lines\n"); // plot type
            for(size_t i = 0; i < frontier.size(); i++){
                fprintf(pipe, "%f %f %f\n", frontier[i][0],frontier[i][1], frontier[i][2]);           // data terminated with \n
            }
            fprintf(pipe, "%s\n", "e");             // termination character
            fflush(pipe);                           // flush the pipe
            // wait for key press
            std::cin.clear();
            std::cout<< "Ecrivez quelque chose pour fermer le programme : " << std::endl;
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cin.get();

            #ifdef WIN32
                    _pclose(pipe);
            #else
                    pclose(pipe);
            #endif
        }
        else
            std::cout << "Could not open pipe" << std::endl;
}

void plotPareto2D(std::vector<std::vector<float>> frontier){
    #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif

        if (pipe != NULL)
        {
            srand(time(nullptr));
            fprintf(pipe, "set term wx\n");         // set the terminal
            fprintf(pipe, "plot '-' with lp pt 4 lt 0\n"); // plot type
            for(size_t i = 0; i < frontier.size(); i++){
                fprintf(pipe, "%f %f\n", frontier[i][0], frontier[i][1] );           // data terminated with \n
            }
            fprintf(pipe, "%s\n", "e");             // termination character
            fflush(pipe);                           // flush the pipe

            // wait for key press
            std::cin.clear();
            std::cout<< "Ecrivez quelque chose pour fermer le programme : " << std::endl;
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cin.get();

            #ifdef WIN32
                    _pclose(pipe);
            #else
                    pclose(pipe);
            #endif
        }
        else
            std::cout << "Could not open pipe" << std::endl;
}
