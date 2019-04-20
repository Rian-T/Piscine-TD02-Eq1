#ifndef SVGFILE_H_INCLUDED
#define SVGFILE_H_INCLUDED


#include <string>
#include <random>
#include <fstream>
#include <set>

constexpr char defcol[] = "black";

class Svgfile
{
    public:
        Svgfile(std::string _filename = "output.svg", int _width=1800, int _height=4000);
        ~Svgfile();

        void useClipPath(std::string shape, std::string clipId);
        void addClipPathRect(std::string clipId, double x, double y, double width, double height);
        void addRadGrad(std::string gradId);
        void addPath(std::string fileName);
        void addWave(int x, int y,std::string clipId);
        void addClipPathEll(std::string clipId, double cx, double cy, double rx, double ry);
        void addNewPath(double x1, double y1, double x2, double y2, std::string color);
        void addRadGrad2(std::string gradId);
        void addRadGrad3(std::string gradId);
        void addRadGrad4(std::string gradId);
        void addEllipse(std::string maskId,std::string gradId, double x, double y, double rx, double ry, std::string color);


        void addDisk(double x, double y, double r, std::string color=defcol);
        void addCircle(double x, double y, double r, double ep, std::string color=defcol);
        void addTriangle(double x1, double y1, double x2, double y2,
                         double x3, double y3, std::string colorFill,
                          double thickness, std::string colorStroke);
        void addTriangle(double x1, double y1, double x2, double y2,
                         double x3, double y3, std::string colorFill=defcol);
        void addLine(double x1, double y1, double x2, double y2, std::string color=defcol);
        void addCross(double x, double y, double span, std::string color=defcol);

        //custom
        void addRectangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4, std::string colorFill);

        void addRectangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4, std::string colorFill, std::string opacity);

        void addGradientRectangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4, std::string gradientId, std::string blur);

//        void addEllipse(double x, double y, double r1, double r2, std::string color);

        void addGaussianBlur(std::string id, double deviation);

        void addGradient(std::string id, std::string startColor, std::string stopColor, double startOpacity, double stopOpacity);

        void addLinearGradient(std::string id, std::string startColor, std::string stopColor, double startOpacity, double stopOpacity);

        void addPath(std::string path, std::string color);

        void addLinGrad(std::string grad);

        void addRectRot(double x, double y, double width, double height, std::string color,std::string yo);

        void addRect(double x, double y, double width, double height, std::string grad);
        //

        void addText(double x, double y, std::string text, std::string color=defcol);
        void addText(double x, double y, double val, std::string color=defcol);

        void addGrid(double span=50.0, bool numbering=true, std::string color="lightgrey");

        static std::string makeRGB(int r, int g, int b);
        void addEndFusee();
        void addBeginFusee(std::string yo,std::mt19937& seed);
        void addPath(std::string path, std::string color, std::string rotation);

        void addImage(std::string url, double h, double w, double x=500,double y= 0);


        /// Type non copiable
        Svgfile(const Svgfile&) = delete;
        Svgfile& operator=(const Svgfile&) = delete;

        static bool s_verbose;

        //Getters
        int getWidth();
        int getHeight();

        ///position pour affichages
        int getCptY() const {return cpt_y;}
        void incremCptY() {++cpt_y;}
        void initCptY() {cpt_y = 0; }

    private:
        std::string m_filename;
        std::ofstream m_ostrm;
        int m_width;
        int m_height;
        // Pour éviter les ouverture multiples
        static std::set<std::string> s_openfiles;

        ///valeurs affichage constant :
        int cpt_y;
};

#endif // SVGFILE_H_INCLUDED



