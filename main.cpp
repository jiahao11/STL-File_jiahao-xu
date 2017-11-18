#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

class vector3d{
private:
    double x, y, z;
public:
    vector3d(){}
    vector3d(double x, double y, double z) : x(x), y(y), z(z) {}
    vector3d normals(const vector3d& v1, const vector3d& v2, const vector3d& v3) const{
        double a = (v2.y-v1.y * v3.z-v1.z) - (v2.z-v1.z * v3.y-v1.y);
        double b = (v2.z-v1.z * v3.x-v1.x) - (v2.x-v1.x * v3.z-v1.z);
        double c = (v2.x-v1.x * v3.y-v1.y) - (v2.y-v1.y * v3.x-v1.x);
        return vector3d(a,b,c);
    }
    double getX() const{
        return x;
    }
    double getY() const{
        return y;
    }
    double getZ() const{
        return z;
    }
};
class Shape {
private:
    double x, y, z;
public:
    Shape(double x=0, double y=0, double z=0) : x(x), y(y),z(z) {}
    double getX() const{
        return x;
    }
    double getY() const{
        return y;
    }
    double getZ() const{
        return z;
    }

    virtual double getR(){ return 0; };
    virtual double getH(){ return 0; };
    virtual int getFacets(){ return 0; };
    virtual double getSize(){ return 0; };

};

class Cylinder : public Shape{
private:
    double r, h;
    int facets;
public:
    Cylinder(double x, double y, double z, double r, double h, int facets) 
        : Shape(x,y,z), r(r), h(h), facets(facets) {}

    double getR(){
        return r;
    }
    double getH(){
        return h;
    }
    int getFacets(){
        return facets;
    }
};

class Cube: public Shape {
private:
    double size;
public:
    Cube(double x, double y, double z, double size) : Shape(x,y,z), size(size){}

    double getSize(){
        return size;
    }
};
class CAD {
private:
    vector<Shape*> shapes;
public:
    CAD(){}
    ~CAD(){}

    void add(Shape* s){
        shapes.push_back(s);
    }

    void write(const string& filename)
    {
        for (int i=0; i<shapes.size();i++) {
            double x = shapes[i]->getX();
            double y = shapes[i]->getY();
            double z = shapes[i]->getZ();

            if(shapes[i]->getR()>0){ //cylinder
                ofstream out("cylinder_"+filename);
                if(out.fail()){
                    cout<<"Open fail!";
                    exit(1);
                }
                out << "solid OpenSCAD_Model" <<'\n';
                double d = 360.0/shapes[i]->getFacets()/180.0*M_PI;
                double r = shapes[i]->getR();
                double h = shapes[i]->getH();
                vector3d temp;

                for(double j=0.0; j<=2.0*M_PI; j=j+d){
                    vector3d bottom1(x,y,z);
                    vector3d bottom2(x+r*cos(j+d),y+r*sin(j+d),z);
                    vector3d bottom3(x+r*cos(j),y+r*sin(j),z);
                    vector3d bottom = temp.normals(bottom1,bottom2,bottom3);
                    out << "  facet normal "<<bottom.getX()<<' '<<bottom.getY()<<' '<<bottom.getZ() <<'\n';
                    out << "    outer loop\n";
                    out << "      vertex " <<x<<" "<<y<<" "<<z<<'\n';
                    out << "      vertex " <<x+r*cos(j+d)<<" "<<y+r*sin(j+d)<<" "<<z<<'\n';
                    out << "      vertex " <<x+r*cos(j)<<" "<<y+r*sin(j)<<" "<<z<<'\n';
                    out << "    endloop\n";
                    out << "  endfacet\n";
                    vector3d top1(x,y,z+h);
                    vector3d top2(x+r*cos(j+d),y+r*sin(j+d),z+h);
                    vector3d top3(x+r*cos(j),y+r*sin(j),z+h);
                    vector3d top = temp.normals(top1,top2,top3);
                    out << "  facet normal "<<top.getX()<<" "<<top.getY()<<" "<<top.getZ() <<'\n';
                    out << "    outer loop\n";
                    out << "      vertex " <<x<<" "<<y<<" "<<z+h<<'\n';
                    out << "      vertex " <<x+r*cos(j)<<" "<<y+r*sin(j)<<" "<<z+h<<'\n';
                    out << "      vertex " <<x+r*cos(j+d)<<" "<<y+r*sin(j+d)<<" "<<z+h<<'\n';
                    out << "    endloop\n";
                    out << "  endfacet\n";
                    vector3d side11(x+r*cos(j),y+r*sin(j),z+h);
                    vector3d side12(x+r*cos(j),y+r*sin(j),z);
                    vector3d side13(x+r*cos(j+d),y+r*sin(j+d),z);
                    vector3d side1 = temp.normals(side11,side12,side13);
                    out << "  facet normal "<<side1.getX()<<" "<<side1.getY()<<" "<<side1.getZ() <<'\n';
                    out << "    outer loop\n";
                    out << "      vertex " <<x+r*cos(j)<<" "<<y+r*sin(j)<<" "<<z+h<<'\n';
                    out << "      vertex " <<x+r*cos(j)<<" "<<y+r*sin(j)<<" "<<z<<'\n';
                    out << "      vertex " <<x+r*cos(j+d)<<" "<<y+r*sin(j+d)<<" "<<z<<'\n';
                    out << "    endloop\n";
                    out << "  endfacet\n";
                    vector3d side21(x+r*cos(j+d),y+r*sin(j+d),z);
                    vector3d side22(x+r*cos(j+d),y+r*sin(j+d),z+h);
                    vector3d side23(x+r*cos(j),y+r*sin(j),z+h);
                    vector3d side2 = temp.normals(side21,side21,side21);
                    out << "  facet normal "<<side2.getX()<<" "<<side2.getY()<<" "<<side2.getZ() <<'\n';
                    out << "    outer loop\n";
                    out << "      vertex " <<x+r*cos(j+d)<<" "<<y+r*sin(j+d)<<" "<<z<<'\n';
                    out << "      vertex " <<x+r*cos(j+d)<<" "<<y+r*sin(j+d)<<" "<<z+h<<'\n';
                    out << "      vertex " <<x+r*cos(j)<<" "<<y+r*sin(j)<<" "<<z+h<<'\n';
                    out << "    endloop\n";
                    out << "  endfacet\n";
                }
                out << "endsolid OpenSCAD_Model" <<'\n';
                out.close();
            }
            else{ //cube
                double size = shapes[i]->getSize();
                ofstream out("cube_"+filename);
                if(out.fail()){
                    cout<<"Open fail!";
                    exit(1);
                }
                out << "solid OpenSCAD_Model" <<'\n';
                out << "  facet normal 0 1 0\n";
                out << "    outer loop\n";
                out << "      vertex "<<x<<" "<<y<<" "<<z<<"\n";
                out << "      vertex "<<x+size<<" "<<y<<" "<<z<<"\n";
                out << "      vertex "<<x<<" "<<y<<" "<<z+size<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal 0 1 0\n";
                out << "    outer loop\n";
                out << "      vertex "<<x+size<<" "<<y<<" "<<z<<"\n";
                out << "      vertex "<<x<<" "<<y<<" "<<z+size<<"\n";
                out << "      vertex "<<x+size<<" "<<y<<" "<<z+size<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal 0 0 1\n";
                out << "    outer loop\n";
                out << "      vertex "<<x<<" "<<y<<" "<<z<<"\n";
                out << "      vertex "<<x+size<<" "<<y<<" "<<z<<"\n";
                out << "      vertex "<<x<<" "<<y+size<<" "<<z<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal 0 0 1\n";
                out << "    outer loop\n";
                out << "      vertex "<<x+size<<" "<<y<<" "<<z<<"\n";
                out << "      vertex "<<x<<" "<<y+size<<" "<<z<<"\n";
                out << "      vertex "<<x+size<<" "<<y+size<<" "<<z<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal 1 0 0\n";
                out << "    outer loop\n";
                out << "      vertex "<<x<<" "<<y<<" "<<z<<"\n";
                out << "      vertex "<<x<<" "<<y+size<<" "<<z<<"\n";
                out << "      vertex "<<x<<" "<<y+size<<" "<<z+size<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal 1 0 0\n";
                out << "    outer loop\n";
                out << "      vertex "<<x<<" "<<y<<" "<<z<<"\n";
                out << "      vertex "<<x<<" "<<y<<" "<<z+size<<"\n";
                out << "      vertex "<<x<<" "<<y+size<<" "<<z+size<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal 0 0 -1\n";
                out << "    outer loop\n";
                out << "      vertex "<<x<<" "<<y<<" "<<z+size<<"\n";
                out << "      vertex "<<x<<" "<<y+size<<" "<<z+size<<"\n";
                out << "      vertex "<<x+size<<" "<<y+size<<" "<<z+size<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal 0 0 -1\n";
                out << "    outer loop\n";
                out << "      vertex "<<x+size<<" "<<y<<" "<<z+size<<"\n";
                out << "      vertex "<<x<<" "<<y<<" "<<z+size<<"\n";
                out << "      vertex "<<x+size<<" "<<y+size<<" "<<z+size<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal 0 -1 0\n";
                out << "    outer loop\n";
                out << "      vertex "<<x<<" "<<y+size<<" "<<z<<"\n";
                out << "      vertex "<<x<<" "<<y+size<<" "<<z+size<<"\n";
                out << "      vertex "<<x+size<<" "<<y+size<<" "<<z<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal 0 -1 0\n";
                out << "    outer loop\n";
                out << "      vertex "<<x+size<<" "<<y+size<<" "<<z<<"\n";
                out << "      vertex "<<x<<" "<<y+size<<" "<<z+size<<"\n";
                out << "      vertex "<<x+size<<" "<<y+size<<" "<<z+size<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal -1 0 0\n";
                out << "    outer loop\n";
                out << "      vertex "<<x+size<<" "<<y<<" "<<z<<"\n";
                out << "      vertex "<<x+size<<" "<<y<<" "<<z+size<<"\n";
                out << "      vertex "<<x+size<<" "<<y+size<<" "<<z<<"\n";
                out << "    endloop\n";
                out << "  endfacet\n";

                out << "  facet normal -1 0 0\n";
                out << "    outer loop\n";
                out << "      vertex "<<x+size<<" "<<y+size<<" "<<z<<"\n";
                out << "      vertex "<<x+size<<" "<<y<<" "<<z+size<<"\n";
                out << "      vertex "<<x+size<<" "<<y+size<<" "<<z+size<<"\n";
                out << "    endloop" <<'\n';
                out << "    endloop\n";
                out << "  endfacet\n";
                out.close();
            }
        }
    }
};

//https://www.stratasysdirect.com/resources/how-to-prepare-stl-files/
//https://www.viewstl.com/
int main() {
    CAD c;
    c.add(new Cube(0,0,0,5));
    c.add(new Cylinder(100,0,0,    3, 10, 10));
    c.write("test.stl");
}
