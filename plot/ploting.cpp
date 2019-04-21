#include "ploting.h"
#include<time.h>
#include<cmath>
#include<algorithm>
#include "svgfile.h"

void plotPareto3D(std::vector<std::vector<float>> frontier,std::vector<std::vector<float>> rest){
    #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif
            std::sort(frontier.begin(),frontier.end());
            srand(time(nullptr));
            FILE* frtTxt = fopen("frt.txt","w");
            for(size_t i = 0; i < frontier.size(); i++){
                fprintf(frtTxt, "%3.2f %3.2f %3.2f\n", frontier[i][0],frontier[i][1], frontier[i][2]);           // data terminated with \n
            }
            FILE* rstTxt = fopen("rst.txt","w");
            for(size_t i = 0; i < rest.size(); i++){
                fprintf(rstTxt, "%3.2f %3.2f %3.2f\n", rest[i][0],rest[i][1], rest[i][2]);           // data terminated with \n
            }
            fclose(rstTxt);
            fclose(frtTxt);
        if (pipe != NULL)
        {         // set the terminal
            fprintf(pipe, "set term wx\n");
            fprintf(pipe, "set hidden3d\n");
            fprintf(pipe, "unset key\n");
            fprintf(pipe, "set title \"3D Plot of Pareto frontier\"\n");
            fprintf(pipe, "set xlabel \"Economic cost\"\n");
            fprintf(pipe, "set ylabel \"Shortest Path\"\n");
            fprintf(pipe, "set zlabel \"Max Flow\" offset 0,5\n");
            fprintf(pipe, "set palette defined ( 0 \"green\" ,1 \"red\" )\n");
            fprintf(pipe, "splot \"frt.txt\" with lp pt 7 ps 2 lc 'green' , \"rst.txt\" pt 7 ps 2 lc 'red'\n"); // plot type
                     // termination character
            //fprintf(pipe, "%s\n", "e");             // termination character
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

void printPareto3D(std::vector<std::vector<float>> frontier,std::vector<std::vector<float>> rest, Svgfile& svgout){
    #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif
            std::sort(frontier.begin(),frontier.end());
            srand(time(nullptr));
            FILE* frtTxt = fopen("frt.txt","w");
            for(size_t i = 0; i < frontier.size(); i++){
                fprintf(frtTxt, "%3.2f %3.2f %3.2f\n", frontier[i][0],frontier[i][1], frontier[i][2]);           // data terminated with \n
            }
            fclose(frtTxt);
            FILE* rstTxt = fopen("rst.txt","w");
            for(size_t i = 0; i < rest.size(); i++){
                fprintf(rstTxt, "%3.2f %3.2f %3.2f\n", rest[i][0],rest[i][1], rest[i][2]);           // data terminated with \n
            }
            fclose(rstTxt);
        if (pipe != NULL)
        {         // set the terminal
            fprintf(pipe, "set terminal gif animate delay 5 optimize\n");
            fprintf(pipe, "set output 'pareto3D.gif'\n");
            fprintf(pipe, "set hidden3d\n");
            fprintf(pipe, "unset key\n");
            fprintf(pipe, "set title \"3D Plot of Pareto frontier\"\n");
            fprintf(pipe, "set xlabel \"Economic cost\"\n");
            fprintf(pipe, "set ylabel \"Shortest Path\"\n");
            fprintf(pipe, "set zlabel \"Max Flow\" offset 0,5\n");
            fprintf(pipe, "splot \"frt.txt\" using 1:2:3: (sprintf(\"(%%3.2f, %%3.2f, %3.2f)\", $1, $2, $3)) with labels point  pt 7 ps 2 lc 'green' offset char 1,1 notitle, \"rst.txt\" pt 7 ps 2 lc 'red'\n"); // plot type
            fprintf(pipe, "n = 100\n");
            fprintf(pipe, "do for [i=1:n] {\n"); // plot type
            fprintf(pipe, " set view 60, i*360/n\n"); // plot typeas
            fprintf(pipe, "splot \"frt.txt\" using 1:2:3: (sprintf(\"(%%3.2f, %%3.2f, %3.2f)\", $1, $2, $3)) with labels point  pt 7 ps 2 lc 'green' offset char 1,1 notitle, \"rst.txt\" pt 7 ps 2 lc 'red'\n"); // plot type
            fprintf(pipe, "}\n");             // termination character
            //fprintf(pipe, "%s\n", "e");             // termination character
            fflush(pipe);                           // flush the pipe
            // wait for key press
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            #ifdef WIN32
                    _pclose(pipe);
            #else
                    pclose(pipe);
            #endif
            svgout.addImage("pareto3D.gif",800,800);
        }
        else
            std::cout << "Could not open pipe" << std::endl;
}

void printPareto2D(std::vector<std::vector<float>> frontier,std::vector<std::vector<float>> rest, Svgfile& svgout){
    #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif
            std::sort(frontier.begin(),frontier.end());
            srand(time(nullptr));
            FILE* frtTxt = fopen("frt.txt","w");
            for(size_t i = 0; i < frontier.size(); i++){
                fprintf(frtTxt, "%3.2f %3.2f\n", frontier[i][0],frontier[i][1]);           // data terminated with \n
            }
            fclose(frtTxt);
            FILE* rstTxt = fopen("rst.txt","w");
            for(size_t i = 0; i < rest.size(); i++){
                fprintf(rstTxt, "%3.2f %3.2f\n", rest[i][0],rest[i][1]);           // data terminated with \n
            }
            fclose(rstTxt);
        if (pipe != NULL)
        {         // set the terminal
            fprintf(pipe, "set terminal png\n");
            fprintf(pipe, "set output 'pareto2D.png'\n");
            fprintf(pipe, "unset key\n");
            fprintf(pipe, "set title \"2D Plot of Pareto frontier\"\n");
            fprintf(pipe, "set xlabel \"Economic cost\"\n");
            fprintf(pipe, "set ylabel \"Shortest Path\"\n");
            fprintf(pipe, "plot \"frt.txt\" using 1:2: (sprintf(\"(%%d, %%d)\", $1, $2)) every 2::0 with labels font \",9\" point  pt 7 ps 2 lc 'green' offset char 1,1 notitle ,    \"frt.txt\" using 1:2: (sprintf(\"(%%d, %%d)\", $1, $2)) every 2::1 with labels font \",9\" point  pt 7 ps 2 lc 'green' offset char 1,-1 notitle, \"rst.txt\" pt 7 ps 0.3 lc 'red'\n");
            //fprintf(pipe, "%s\n", "e");             // termination character
            fflush(pipe);                           // flush the pipe
            // wait for key press
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            #ifdef WIN32
                    _pclose(pipe);
            #else
                    pclose(pipe);
            #endif
            svgout.addImage("pareto2D.png",800,800);
        }
        else
            std::cout << "Could not open pipe" << std::endl;
}

void plotPareto2D(std::vector<std::vector<float>> frontier,std::vector<std::vector<float>> rest){
    #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif
        std::sort(frontier.begin(),frontier.end());
        FILE* frtTxt = fopen("frt.txt","w");
            for(size_t i = 0; i < frontier.size(); i++){
                fprintf(frtTxt, "%3.2f %3.2f\n", frontier[i][0],frontier[i][1]);           // data terminated with \n
            }
            fclose(frtTxt);
            FILE* rstTxt = fopen("rst.txt","w");
            for(size_t i = 0; i < rest.size(); i++){
                fprintf(rstTxt, "%3.2f %3.2f\n", rest[i][0],rest[i][1]);           // data terminated with \n
            }
            fclose(rstTxt);
        if (pipe != NULL)
        {
            fprintf(pipe, "set term wx\n");         // set the terminal
            fprintf(pipe, "unset key\n");         // set the terminal
            fprintf(pipe, "plot \"frt.txt\" with lp  pt 7 ps 2 lc 'green', \"rst.txt\" pt 7 ps 0.3 lc 'red'\n");
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


void plotPareto3Dtest(){
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
            for(size_t i = 0; i < 1000; i++){
                int x = rand()%100;
                int y = rand()%100;
                int z = rand()%100;
                fprintf(pipe, "%d %d %d\n", x,y, z);           // data terminated with \n
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

void plotPareto2Dtest(){
    #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif

        if (pipe != NULL)
        {
            srand(time(nullptr));
            fprintf(pipe, "set term wx\n");         // set the terminal
            fprintf(pipe, "plot '-' using 1:2:(sprintf(\"(%3.2f, %3.2f)\", $1, $2)) with labels notitle\n"); // plot type
            for(double i = 0; i < 20; i++){
                fprintf(pipe, "%f %f\n", i,exp(1/sqrt(i)));          // data terminated with \n
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
