#include "trianglesurface.h"
#include "vertex.h"
#include "math_constants.h"
#include <cmath>
#include <QDebug>

TriangleSurface::TriangleSurface() : VisualObject() {
    Vertex v{};
    v.set_xyz(0,0,0); v.set_rgb(1,0,0); v.set_uv(0,0);
    mVertices.push_back(v);
    v.set_xyz(0.5,0,0); v.set_rgb(0,1,0);  v.set_uv(1,0);
    mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(0,0,1); v.set_uv(1,1);
    mVertices.push_back(v);
    v.set_xyz(0,0,0); v.set_rgb(0,1,0); v.set_uv(0,0);
    mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(1,0,0); v.set_uv(1,1);
    mVertices.push_back(v);
    v.set_xyz(0,0.5,0); v.set_rgb(0,0,1); v.set_uv(0,1);
    mVertices.push_back(v);

    //constructplane();
    mMatrix.setToIdentity();
}

TriangleSurface::TriangleSurface(std::string filename) : VisualObject()
{
    readFile(filename);
    mMatrix.setToIdentity();
}

TriangleSurface::~TriangleSurface()
{
}

void TriangleSurface::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void TriangleSurface::readFile(std::string filename) {
    //mVertices.clear();
    std::ifstream inn;
    inn.open(filename.c_str());

    if (inn.is_open())
    {
        unsigned long n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (unsigned long i=0; i<n; i++)
        {
            inn >> vertex;
            mVertices.push_back(vertex);
        }
        inn.close();
    }
    else
    {
        qDebug() << "Error: " << filename.c_str() << " could not be opened!";
    }
}

void TriangleSurface::writeFile(std::string filename)
{
    std::ofstream ut;
    ut.open(filename.c_str());

    if (ut.is_open())
    {
        auto n = mVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            vertex = *it;
            ut << vertex << std::endl;
        }
        ut.close();
    }
    std::cout << "complete!";
}


void TriangleSurface::construct()
{
    float xmin=0.0f, xmax=1.0f, ymin=0.0f, ymax=1.0f, h=0.25f;
    for (auto x=xmin; x<xmax; x+=h)
    {
        for (auto y=ymin; y<ymax; y+=h)
        {
            float z = sin(gsl::PI*x)*sin(gsl::PI*y);
            mVertices.push_back(Vertex{x,y,z,x,y,z});
            z = sin(gsl::PI*(x+h))*sin(gsl::PI*y);
            mVertices.push_back(Vertex{x+h,y,z,x,y,z});
            z = sin(gsl::PI*x)*sin(gsl::PI*(y+h));
            mVertices.push_back(Vertex{x,y+h,z,x,y,z});
            mVertices.push_back(Vertex{x,y+h,z,x,y,z});
            z = sin(gsl::PI*(x+h))*sin(gsl::PI*y);
            mVertices.push_back(Vertex{x+h,y,z,x,y,z});
            z = sin(gsl::PI*(x+h))*sin(gsl::PI*(y+h));
            mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
        }
    }
}



void TriangleSurface::constructplane()
{

        float dx = 4.0;
        float dz = 4.0;
        mVertices.clear();
        for(float z=-2.0; z<2.0; z+=dz)
        {
            for(float x=-2.0; x<2.0; x+=dx)
            {
                float x0 = x;
                float z0 = z;
                float x1 = x0+dx;
                float z1 = z0 + dz;
                mVertices.push_back(Vertex{x0,1.0,z0,1,1,1});
                mVertices.push_back(Vertex{x1,0.0,z0,0,0,1});
                mVertices.push_back(Vertex{x0,0.0,z1,0,0,1});
                mVertices.push_back(Vertex{x0,0.0,z1,1,1,1});
                mVertices.push_back(Vertex{x1,0.0,z0,0,0,1});
                mVertices.push_back(Vertex{x1,1.5,z1,0,0,1});
            }
        }
        //writeFile("../OpenGL2019/SecondTriangle");
        //std::cout << "complete?";
        mMatrix.setToIdentity();
}
