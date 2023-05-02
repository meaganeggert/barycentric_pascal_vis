#include <vector>
#include <iostream>
#include "Bridges.h"
#include "Color.h"
#include "ColorGrid.h"
#include "/public/read.h"
#include <string>
#include <cmath>

using namespace std;
using namespace bridges;

struct Coolor {
    int r,g,b;
    friend istream& operator>> (istream& ins, Coolor &c) {
        return ins >> c.r >> c.g >> c.b;
    }
    friend ostream& operator<< (ostream& outs, const Coolor &c) {
        return outs << "Color: (" << c.r << ',' << c.g << ',' << c.g << ')';
    }
};

struct Point {
        float x= 0, y = 0; //Default position is (0,0)

        //Point steve = bob - cindy; //steve is {13,16}
        friend Point operator-(const Point &lhs, const Point &rhs) {
                return {lhs.x-rhs.x,lhs.y-rhs.y};
        }

        //Point steve = bob / 2; //steve is {1.5,2}
        friend Point operator/(const Point &lhs, float scale) {
                return {lhs.x/scale,lhs.y/scale};
        }

        //cout << length(bob) << endl; //Prints 5
        friend float length(const Point &foo) {
                return hypot(foo.x,foo.y);
        }

        //cout << bob.length() << endl; //Prints 5
        float length() {
                return hypot(x,y);
        }

        //cout << abs(cindy); //Prints (10,20)
        friend Point abs(const Point &foo) {
                return {std::abs(foo.x),std::abs(foo.y)};
        }

        //cout << bob.distance_to(6,0); //Prints 5
        float distance_to(int x, int y) const {
                return this->distance_to(Point{float(x),float(y)});
        }

        //cout << bob.distance_to(cindy); //Prints 20.6155 if I did the math right
        float distance_to(const Point &rhs) const {
                return hypot(rhs.x-this->x,rhs.y-this->y);
        }

        //cin >> steve; //Reads two doubles in
        friend std::istream& operator>>(std::istream& ins, Point &rhs) {
                return ins >> rhs.x >> rhs.y;
        }

        //cout << bob; //Prints "(3,4)"
        friend std::ostream& operator<<(std::ostream& outs, const Point &rhs) {
                return outs << '(' << rhs.x << ',' << rhs.y << ')';
        }

};

float crossProduct(Point A, Point B);
bool isItInside (Point testPoint, Point topVertex, Point leftVertex, Point rightVertex);
Coolor colorPicker (Point testPoint, Point topVertex, Point leftVertex, Point rightVertex, vector<vector<Coolor>> &colours);
long long int  pascal(int row, int col, int width,  vector<vector<long long int>> &cash);

int main() {

    //Bridges
    Bridges *bridges =  new Bridges(3333, "meaganeggert", "1483739998773");
    bridges->setTitle("Triangle Art - Mashup");
    bridges->setDescription("A visualization of a triangle using a combination of barycentric coordinates and Pascal's triangle mod 2 to create a gradient texture. If the value at the test point is odd, the program will color the point based on its relationship to the triangle's 3 vertices. If the point is even, it will color the space black.");

        int rows = read("How many rows?\n");
        int width = rows * 2 - 1;
        int height = rows;
        int triangle_area = 0.5 * width * height;
        long long int pascalVal;
        Coolor default_col;
        default_col.r = 255;
        default_col.g = 255;
        default_col.b = 255;
        Coolor temp_col;

        vector<vector<Coolor>> colours(rows, vector<Coolor>(width, default_col));
        vector<vector<long long int>> space(rows, vector<long long int>(width, 0));

        Point leftVert, topVert, rightVert;
        leftVert.x = 0;
        leftVert.y = height - 1;
        rightVert.x = width;
        rightVert.y = height - 1;
        topVert.x = width/2;
        topVert.y = 0;

        ColorGrid cg(height, width, Color("black"));

    for (int i = height - 1; i >= 0; i--) {
        for (int j = width / 2 - i % 2; j >= width / 2 - i; j -= 2) {
            pascalVal = pascal(i, j, width - 1, space);
//          cout << pascalVal << " ";
        }
        for (int j = width / 2 + i; j >= width / 2; j -= 2) {
            pascalVal = pascal(i, j, width - 1, space);
//          cout << pascalVal << " ";
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (space.at(i).at(j) % 2 == 1) {
                  Point test = Point(j, i);
                  temp_col = colorPicker(test, topVert, leftVert, rightVert, colours);
                  cg.set(i, j, Color(temp_col.r, temp_col.g, temp_col.b));
      //                              cout << space.at(i).at(j) << " ";
                  cout << "* " ;
            }
            else {
                  cg.set(i, j, Color("black"));
            }
        }
        cout << endl;
    }

    bridges->setDataStructure(&cg);

    bridges->visualize();

    delete bridges;

}

Coolor colorPicker (Point testPoint, Point topVertex, Point leftVertex, Point rightVertex, vector<vector<Coolor>> &colours) {

        Coolor currentColor;
        Point leftVector, rightVector, bottomVector, testVector;

        leftVector = Point(leftVertex.x - topVertex.x, leftVertex.y - topVertex.y);
        rightVector = Point(rightVertex.x - topVertex.x, rightVertex.y - topVertex.y);
        bottomVector = Point(leftVertex.x - rightVertex.x, leftVertex.y - rightVertex.y);
        testVector = Point(testPoint.x - topVertex.x, testPoint.y - topVertex.y);

        float totalArea, redArea, greenArea, blueArea;

        totalArea = abs(crossProduct(leftVector, rightVector));

        greenArea = abs(crossProduct(leftVector, testVector));
        redArea = abs(crossProduct(testVector, rightVector));

        testVector = Point(testPoint.x - rightVertex.x, testPoint.y - rightVertex.y); //Set the test vector to the displacement vector from the right vertex to the test point
        blueArea = abs(crossProduct(testVector, bottomVector));

        float redRatio, greenRatio, blueRatio;

        redRatio = redArea/totalArea;
        greenRatio = greenArea/totalArea;
        blueRatio = blueArea/totalArea;

        currentColor.r = redRatio*255;
        currentColor.g = greenRatio*255;
        currentColor.b = blueRatio*255;

        colours.at(testPoint.y).at(testPoint.x) = currentColor;

        return currentColor;

}

float crossProduct(Point A, Point B) {
        float result;
        result = B.x*A.y - A.x*B.y;
        return result;
}

long long int  pascal(int row, int col, int width,  vector<vector<long long int>> &cash) {

        if (cash.at(row).at(col) == 0) {
        //      if (row == 0) {
        //              cash.at(row).at(col) = 1;
        //      }
                if (col == width or col == width/2 - row or col == width/2 + row) {
                        cash.at(row).at(col) = 1;
                }
                else if (((col == row + 1) or (col == width - row + 1)) and (row != 0)) {
                        cash.at(row).at(col) = row;
                }
                else {
                        long long int temp_calc = (pascal(row-1, col-1, width, cash) + pascal(row - 1, col + 1, width, cash));
                        cash.at(row).at(col) = temp_calc;
                }
                return cash.at(row).at(col);
                }
        else {
                return cash.at(row).at(col);
        }

}

