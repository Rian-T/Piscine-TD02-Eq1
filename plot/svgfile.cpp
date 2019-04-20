#include "svgfile.h"
#include <iostream>
#include <sstream>

const std::string svgHeader =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" ";

const std::string svgEnding = "\n\n</svg>\n";

/// Effets "Boule en relief", voir données à la fin de ce fichier
extern const std::string svgBallGradients;

std::set<std::string> Svgfile::s_openfiles;

bool Svgfile::s_verbose = true;

Svgfile::Svgfile(std::string _filename, int _width, int _height) :
    m_filename{_filename}, m_width{_width}, m_height{_height}
{

    if (s_verbose)
        std::cout << "Opening SVG output file : "
                  << m_filename << std::endl;

    if ( s_openfiles.count(m_filename) )
        throw std::runtime_error( "File " + m_filename + " already open !" );

    m_ostrm.open(m_filename);
    s_openfiles.insert(m_filename);

    if (!m_ostrm)
    {
        std::cout << "Problem opening " << m_filename << std::endl;
        throw std::runtime_error("Could not open file " + m_filename );
    }

    if (s_verbose)
        std::cout << "OK" << std::endl;

    // Writing the header into the SVG file
    m_ostrm << svgHeader;
    m_ostrm << "width=\"" << m_width << "\" height=\"" << m_height << "\">\n\n";
}

Svgfile::~Svgfile()
{
    // Writing the ending into the SVG file
    m_ostrm << svgEnding;

    // Removing this file from the list of open files
    s_openfiles.erase(m_filename);

    // No need to explicitly close the ofstream object (automatic upon destruction)
}

// Helper templated function
template<typename T>
std::string attrib(std::string name, T val)
{
    std::ostringstream oss;
    oss << name << "=\"" << val << "\" ";
    return oss.str();
}

void Svgfile::addDisk(double x, double y, double r, std::string color)
{
    m_ostrm << "<circle "
            << attrib("cx", x)
            << attrib("cy", y)
            << attrib("r",  r)
            << attrib("fill", color )
            << "/>\n";
}

void Svgfile::addCircle(double x, double y, double r, double ep, std::string color)
{
    m_ostrm << "<circle "
            << attrib("cx", x)
            << attrib("cy", y)
            << attrib("r",  r)
            << attrib("fill", "none")
            << attrib("stroke", color )
            << attrib("stroke-width", ep )
            << "/>\n";
}

void Svgfile::addImage(std::string url, double h, double w, double x,double y)
{
    m_ostrm << "<image "
            << attrib("href", url)
            << attrib("height", h)
            << attrib("width", w)
            << attrib("x", x)
            << attrib("y", y)
            << "/>\n";
}



/// <polygon points="200,10 250,190 160,210" style="fill:lime;stroke:purple;stroke-width:1" />
void Svgfile::addTriangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, std::string colorFill,
                          double thickness, std::string colorStroke)
{

    m_ostrm << "<polygon points=\" "
            << x1 << "," << y1 << " "
            << x2 << "," << y2 << " "
            << x3 << "," << y3
            << "\" style=\"fill:" << colorFill
            << ";stroke:" << colorStroke
            << ";stroke-width:" << thickness
            << "\" />\n";
}


void Svgfile::addTriangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, std::string colorFill)
{
    m_ostrm << "<polygon points=\" "
            << x1 << "," << y1 << " "
            << x2 << "," << y2 << " "
            << x3 << "," << y3
            << "\" style=\"fill:" << colorFill
            << "\" />\n";
}

void Svgfile::addRectangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4, std::string colorFill, std::string opacity)
{
    m_ostrm << "<polygon points=\" "
            << x1 << "," << y1 << " "
            << x2 << "," << y2 << " "
            << x3 << "," << y3 << " "
            << x4 << "," << y4
            << "\" style=\"fill:" << colorFill << ";opacity:"<<opacity<< "\" "
            <<" />\n";
}

void Svgfile::addRectangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4, std::string colorFill)
{
    m_ostrm << "<polygon points=\" "
            << x1 << "," << y1 << " "
            << x2 << "," << y2 << " "
            << x3 << "," << y3 << " "
            << x4 << "," << y4
            << "\" style=\"fill:" << colorFill
            <<"\" />\n";
}

void Svgfile::addGradientRectangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4, std::string gradientId, std::string blur)
{
    std::string grad = "url(#";
    grad.append(gradientId);
    grad.append(")");

    std::string blurID = "url(#";
    blurID.append(blur);
    blurID.append(")");

    m_ostrm << "<polygon points=\" "
            << x1 << "," << y1 << " "
            << x2 << "," << y2 << " "
            << x3 << "," << y3 << " "
            << x4 << "," << y4 << "\" "
            << attrib("fill", grad) << ""
            << attrib("filter", blurID)
            << " />\n";
}

void Svgfile::addLine(double x1, double y1, double x2, double y2, std::string color)
{
    m_ostrm << "<line "
            << attrib("x1", x1)
            << attrib("y1", y1)
            << attrib("x2", x2)
            << attrib("y2", y2)
            << attrib("stroke", color)
            << "/>\n";
}

void Svgfile::addCross(double x, double y, double span, std::string color)
{
    addLine(x-span, y-span, x+span, y+span, color);
    addLine(x-span, y+span, x+span, y-span, color);
}

void Svgfile::addText(double x, double y, std::string text, std::string color)
{
    /// <text x="180" y="60">Un texte</text>
    m_ostrm << "<text "
            << attrib("x", x)
            << attrib("y", y)
            << attrib("fill", color)
            << ">" << text << "</text>\n";
}

void Svgfile::addText(double x, double y, double val, std::string color)
{
    std::ostringstream oss;
    oss << val;
    addText(x, y, oss.str(), color);
}

void Svgfile::addGrid(double span, bool numbering, std::string color)
{
    double debut_ligne_Horiz= 800;
    double y=0;
    while (y<=m_height)
    {
        addLine(0, debut_ligne_Horiz+y, m_width, debut_ligne_Horiz+y, color);
        if (numbering)
            addText(5, y-5, y, color);
        y+=span;
    }

    double x=0;
    while (x<=m_width)
    {
        addLine(x, 800, x, m_height, color);
        if (numbering)
            addText(x+5, 15, x, color);
        x+=span;
    }
}

void Svgfile::addGaussianBlur(std::string id, double deviation)
{
        m_ostrm << "<filter "
            << attrib("id", id) << ">"
            << "<feGaussianBlur "
            << attrib("in", "SourceGraphic")
            << attrib("stdDeviation", std::to_string(deviation)) << "/>"
            << "</filter>\n";
}

void Svgfile::addGradient(std::string id, std::string startColor, std::string stopColor, double startOpacity, double stopOpacity)
{
    std::string style1 = "stop-color:";
    style1.append(startColor);
    style1.append(";stop-opacity:");
    style1.append(std::to_string(stopOpacity));

    std::string style2 = "stop-color:";
    style2.append(stopColor);
    style2.append(";stop-opacity:");
    style2.append(std::to_string(stopOpacity));

    m_ostrm << "<defs><radialGradient "
            << attrib("id", id)
            << ">"
            << "<stop "
            << attrib("offset", "10%")
            << attrib("style", style1)
            << "/>"
            << "<stop "
            << attrib("offset", "95%")
            << attrib("style", style2)
            << "/>"
            << "</radialGradient></defs>\n";
}

void Svgfile::addLinearGradient(std::string id, std::string startColor, std::string stopColor, double startOpacity, double stopOpacity)
{
    std::string style1 = "stop-color:";
    style1.append(startColor);
    style1.append(";stop-opacity:");
    style1.append(std::to_string(startOpacity));

    std::string style2 = "stop-color:";
    style2.append(stopColor);
    style2.append(";stop-opacity:");
    style2.append(std::to_string(stopOpacity));

    m_ostrm << "<defs><linearGradient "
            << attrib("id", id) << " "
            << attrib("x1", "0%") << " "
            << attrib("y1", "0%") << " "
            << attrib("x2", "0%") << " "
            << attrib("y2", "100%")
            << ">"
            << "<stop "
            << attrib("offset", "0%")
            << attrib("style", style1)
            << "/>"
            << "<stop "
            << attrib("offset", "100%")
            << attrib("style", style2)
            << "/>"
            << "</linearGradient></defs>\n";
}

void Svgfile::addPath(std::string path, std::string color)
{
        m_ostrm << "<path "
            << attrib("d", path)
            << attrib("fill", color)
            << "/> \n";
}

int Svgfile::getWidth()
{
    return m_width;
}

int Svgfile::getHeight()
{
    return m_height;
}

std::string Svgfile::makeRGB(int r, int g, int b)
{
    std::ostringstream oss;
    oss << "rgb(" << r << "," << g << "," << b << ")";
    return oss.str();
}


//Kim
void Svgfile::addLinGrad(std::string grad)
{
    m_ostrm <<"<defs>"
            << "<linearGradient "
            << attrib("id",grad)
            << attrib("x1","0%")
            << attrib("y1","0%")
            << attrib("x2","0%")
            << attrib("y2","100%")
            << ">\n"
            << "<stop offset=\"0%\" style=\"stop-color:rgb(0,0,0);stop-opacity:1\"" << "/>\n"
            << "<stop offset=\"100%\" style=\"stop-color:rgb(1, 49, 180);stop-opacity:1\"" << "/>\n"
            <<"</linearGradient>\n"
            <<"</defs>\n";
}

void Svgfile::addRectRot(double x, double y, double width, double height, std::string color,std::string yo)
{
    m_ostrm << "<rect "
    << attrib("x", x)
    << attrib("y", y)
    << attrib("width", width)
    << attrib("height", height)
    << attrib("fill", color)
    <<"/>\n";
}



void Svgfile::addEndFusee()
{
    m_ostrm << "</g>\n";
}

void Svgfile::addRect(double x, double y, double width, double height, std::string grad)
    {
        m_ostrm << "<rect "
        << attrib("x", x)
        << attrib("y", y)
        << attrib("width", width)
        << attrib("height", height)
        << attrib("fill", "url(#grad)")
        <<"/>\n";
    }

    void Svgfile::addEllipse(std::string clipId,std::string gradId, double x, double y, double rx, double ry, std::string color)
{
    std::stringstream oss;
    oss << "url(#" << clipId << ")";
    std::stringstream pss;
    pss << "url(#" << gradId << ")";
    m_ostrm << "<ellipse "
            << attrib("cx", x)
            << attrib("cy", y)
            << attrib("rx",  rx)
            << attrib("ry",  ry)
            << attrib("fill", pss.str())
            << attrib("clip-path", oss.str())
            << "/>\n";
}

    void Svgfile::addClipPathEll(std::string clipId, double cx, double cy, double rx, double ry)
{
    m_ostrm <<"<defs>"
            << "<clipPath "
            << attrib("id",clipId) << ">\n"
            << "<ellipse "
            << attrib("cx", cx)
            << attrib("cy", cy)
            << attrib("rx", rx)
            << attrib("ry", ry)
            << "/>\n"
            <<"</clipPath>\n"
            <<"</defs>\n";
}

void Svgfile::addClipPathRect(std::string clipId, double x, double y, double width, double height)
{
    m_ostrm <<"<defs>"
            << "<clipPath "
            << attrib("id",clipId) << ">\n"
            << "<rect "
            << attrib("x", x)
            << attrib("y", y)
            << attrib("width", width)
            << attrib("height", height)
            << "/>\n"
            <<"</clipPath>\n"
            <<"</defs>\n";
}

void Svgfile::addPath(std::string fileName)
{
    std::ifstream ifs(fileName);
    do{
        std::string oss;
        ifs >> oss;
        m_ostrm << oss << " ";
    }while(!(ifs.eof()));
}

void Svgfile::addWave(int x, int y,std::string clipId)
{
    std::stringstream oss;
    oss << "url(#" << clipId << ")";
    m_ostrm <<"<rect "
            <<attrib("x",x)
            <<attrib("y",y)
            << "width=\"100%\" height=\"100%\" ng-attr-fill=\"url(#{{config.patid}})\" fill=\"url(#ldp-8aa6b23a37e128)\" "
            << attrib("clip-path", oss.str())
            << "></rect>\n";
}

void Svgfile::addPath(std::string path, std::string color, std::string rotation)
{
    std::stringstream oss;
    oss<<"rotate("<<rotation<<")";

        m_ostrm << "<path "
            << attrib("d", path)
            << attrib("fill", color)
            << attrib("transform", oss.str())
            << "/> \n";
}

void Svgfile::addRadGrad(std::string gradId)
{
    m_ostrm <<"<defs>"
            << "<radialGradient "
            << attrib("id",gradId)
            << attrib("cx","35%")
            << attrib("cy","-25%")
            << attrib("r","100%")
             << ">\n"
            << "<stop offset=\"0%\" style=\"stop-color:rgb(250,250,250);stop-opacity:1\"" << "/>\n"
            << "<stop offset=\"100%\" style=\"stop-color:rgb(0,0,255);stop-opacity:1\"" << "/>\n"
            <<"</radialGradient>\n"
            <<"</defs>\n";
}

void Svgfile::addRadGrad2(std::string gradId)
{
    m_ostrm <<"<defs>"
            << "<radialGradient "
            << attrib("id",gradId)
            << attrib("cx","35%")
            << attrib("cy","-25%")
            << attrib("r","100%")
             << ">\n"
            << "<stop offset=\"0%\" style=\"stop-color:rgb(250,250,250);stop-opacity:1\"" << "/>\n"
            << "<stop offset=\"100%\" style=\"stop-color:rgb(194, 190, 128);stop-opacity:1\"" << "/>\n"
            <<"</radialGradient>\n"
            <<"</defs>\n";
}

void Svgfile::addRadGrad4(std::string gradId)
{
    m_ostrm <<"<defs>"
            << "<radialGradient "
            << attrib("id",gradId)
            << attrib("cx","35%")
            << attrib("cy","-25%")
            << attrib("r","100%")
             << ">\n"
            << "<stop offset=\"0%\" style=\"stop-color:rgb(255,255,255);stop-opacity:1\"" << "/>\n"
            << "<stop offset=\"100%\" style=\"stop-color:rgb(255,250,250);stop-opacity:1\"" << "/>\n"
            <<"</radialGradient>\n"
            <<"</defs>\n";
}

void Svgfile::addRadGrad3(std::string gradId)
{
    m_ostrm <<"<defs>"
            << "<radialGradient "
            << attrib("id",gradId)
            << attrib("cx","35%")
            << attrib("cy","-25%")
            << attrib("r","100%")
             << ">\n"
            << "<stop offset=\"0%\" style=\"stop-color:rgb(255,255,255);stop-opacity:1\"" << "/>\n"
            << "<stop offset=\"100%\" style=\"stop-color:rgb(0,180,0);stop-opacity:1\"" << "/>\n"
            <<"</radialGradient>\n"
            <<"</defs>\n";
}


