//
// Created by Samrid on 3/22/2016.
//

#include <iostream>
#include <sstream>
#include <algorithm>

#include "Point.h"
#include "Exceptions.h"

using namespace std;

namespace Clustering {

    unsigned int Point::__idGen = 0;

    const char Point::POINT_VALUE_DELIM = ',';

    void Point::rewindIdGen() {
        __idGen--;
    }

    Point::Point(unsigned int dimensions) {
        if (dimensions == 0){
            throw ZeroDimensionsEx();
        }
        __dim = dimensions;
        __id = ++__idGen;
        __values = new double[__dim];
        for (int i = 0; i < __dim; ++i) {
            __values[i] = 0.0;
        }
    }

    Point::Point(unsigned int dimensions, double *arr) {

        __dim = dimensions;
        __values = new double[__dim];

        for (int i = 0; i < __dim; i++) {
            __values[i] = arr[i];
        }
    }

    Point::Point(const Point &rtSide) {
        __dim = rtSide.__dim;
        __values = new double[__dim];
        for (int i = 0; i < __dim; ++i) {
            __values[i] = rtSide.__values[i];
        }
        __id = rtSide.__id;
    }

    Point &Point::operator=(const Point &rtSide) {
        if (__dim != rtSide.__dim){
            throw DimensionalityMismatchEx(__dim, rtSide.__dim);
        }

        if (this == &rtSide) {
            return *this;
        }
        else {
            __dim = rtSide.__dim;
            __values = new double[__dim];
            for (int i = 0; i < __dim; i++) {
                __values[i] = rtSide.__values[i];
            }
            return *this;
        }
    }

    Point::~Point() {
        delete[] __values;
    }

    int Point::getId() const {
        return __id;
    }

    unsigned int Point::getDims() const {
        return __dim;
    }

    void Point::setValue(unsigned int dimension, double val) {
        if(__dim <= dimension){
            throw OutOfBoundsEx(__dim, dimension);
        }
        __values[dimension] = val;
    }

    double Point::getValue(unsigned int dimension) const {
        if(__dim <= dimension){
            throw OutOfBoundsEx(__dim, dimension);
        }
        return __values[dimension];
    }

    double Point::distanceTo(const Point &point) const {
        if(__dim != point.__dim){
            throw DimensionalityMismatchEx(__dim, point.__dim);
        }
        if (point.__dim == __dim) {
            double distance = 0.0;

            for (int i = 0; i < __dim; i++)
                distance += pow((point.__values[i] - __values[i]), 2);
            return sqrt(distance);
        }
        else
            return 0;
    }

    Point &Point::operator*=(double multiply) {
        for (int i = 0; i < __dim; i++)
            this->__values[i] *= multiply;
        return *this;
    }

    Point &Point::operator/=(double divider) {
        if (__dim == 0) {
            return *this;
        }
        for (int i = 0; i < __dim; i++)
            this->__values[i] = this->__values[i] / divider;
        return *this;
    }

    const Point Point::operator*(double multiplier) const {
        Point multiply(__dim);
        for(int i = 0; i < __dim; i++)
            multiply.__values[i] = (multiplier * __values[i]);
        return multiply;
    }

    const Point Point::operator/(double divider) const {
        Point division(__dim);
        if (divider == 0) {
            return *this;
        }
        for (int i = 0; i < __dim; i++)
            division.__values[i] = (__values[i] / divider);
        return division;

    }

    double &Point::operator[](unsigned int index) {
        if (index >= __dim) {
            throw OutOfBoundsEx(__dim, index);
        }
        return __values[index];
    }

    const double &Point::operator[](unsigned int index) const {
        if (index >= __dim) {
            throw OutOfBoundsEx(__dim, index);
        }
        return __values[index];
    }

    Point &operator+=(Point &ltSide, const Point &rtSide) {
        if(ltSide.__dim != rtSide.__dim){
            throw DimensionalityMismatchEx(ltSide.__dim, rtSide.__dim);
        }
        for(int i = 0; i < ltSide.__dim; i++)
        {
            ltSide.__values[i] += rtSide.__values[i];
        }
        return ltSide;
    }

    Point &operator-=(Point &ltSide, const Point &rtSide) {
        if(ltSide.__dim != rtSide.__dim){
            throw DimensionalityMismatchEx(ltSide.__dim, rtSide.__dim);
        }
        for(int i = 0; i < ltSide.__dim; i++)
        {
            ltSide.__values[i] -= rtSide.__values[i];
        }
        return ltSide;
    }

    const Point operator+(const Point &ltSide, const Point &rtSide){
        Point p(ltSide);
        p += rtSide;
        return p;
    }

    const Point operator-(const Point &ltSide, const Point &rtSide){
        Point p(ltSide);
        p -= rtSide;
        return p;
    }

    bool operator==(const Point &ltSide, const Point &rtSide) {
        if (ltSide.__dim != rtSide.__dim){
            throw DimensionalityMismatchEx(ltSide.__dim, rtSide.__dim);
        }
        if(ltSide.__id != rtSide.__id) {
            return false;
        }
        else {
            for (int i = 0; i < ltSide.__dim; i++){
                if (ltSide.__values[i] != rtSide.__values[i])
                    return false;
            }
        }
        return true;
    }

    bool operator!=(const Point &ltSide, const Point &rtSide) {
        return !(ltSide == rtSide);
    }

    bool operator<(const Point &ltSide, const Point &rtSide) {
        if (ltSide.__dim != rtSide.__dim){
            throw DimensionalityMismatchEx(ltSide.__dim, rtSide.__dim);
        }
        for (int i = 0; i < ltSide.__dim; i++) {
            if (ltSide.__values[i] < rtSide.__values[i])
                return true;
            else if(ltSide.__values[i] > rtSide.__values[i])
                return false;
        }
        return false;
    }

    bool operator>(const Point &ltSide, const Point &rtSide) {
        if (ltSide.__dim != rtSide.__dim){
            throw DimensionalityMismatchEx(ltSide.__dim, rtSide.__dim);
        }
        for (int i = 0; i < ltSide.__dim; i++) {
            if (ltSide.__values[i] < rtSide.__values[i])
                return false;
            else if (ltSide.__values[i] > rtSide.__values[i])
                return true;
        }
        return false;
    }

    bool operator<=(const Point &ltSide, const Point &rtSide) {
        if (ltSide.__dim != rtSide.__dim){
            throw DimensionalityMismatchEx(ltSide.__dim, rtSide.__dim);
        }
        for (int i = 0; i < ltSide.__dim; i++) {
            if (ltSide.__values[i] <= rtSide.__values[i])
                return true;
            else if(ltSide.__values[i] > rtSide.__values[i])
                return false;
        }
        return true;
    }

    bool operator>=(const Point &ltSide, const Point &rtSide) {
        if (ltSide.__dim != rtSide.__dim){
            throw DimensionalityMismatchEx(ltSide.__dim, rtSide.__dim);
        }
        for (int i = 0; i < ltSide.__dim; i++) {
            if (ltSide.__values[i] >= rtSide.__values[i])
                return true;
            else if(ltSide.__values[i] < rtSide.__values[i])
                return false;
        }
        return true;
    }

    std::ostream &operator<<(std::ostream &os, const Point &point) {
        for (int i = 0; i < point.__dim; i++) {
            os << point.__values[i];
            os << " ";
        }
        return os;
    }

    std::istream &operator>>(std::istream &is, Point &point) {
        for(int i = 0; i < point.__dim; i++)
            is >> point.__values[i];
        return is;
    }
}