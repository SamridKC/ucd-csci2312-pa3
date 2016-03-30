//
// Created by Samrid on 2/20/2016.
//

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>

#include "Cluster.h"
#include "Exceptions.h"

using namespace std;

namespace Clustering {

    const char POINT_CLUSTER_ID_DELIM = ':';

    unsigned int Cluster::__idGenerator = 0;


    LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n) { };

    void Cluster::__del() {
        if (__points != nullptr) {
            LNodePtr delPtr = __points;
            __points = __points->next;
            delete delPtr;
            __size--;
        }
    }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) :
            __c(c),
            __p(d)
    {
        __dimensions = d;
        __valid = false;
    }

    const Point Cluster::Centroid::get() const {
        return __p;
    }

    void Cluster::Centroid::set(const Point &p) {
        __valid = true;
        __p = p;
    }

    bool Cluster::Centroid::isValid() const {
        return __valid;
    }

    void Cluster::Centroid::setValid(bool valid) {
        __valid = valid;
    }

    void Cluster::Centroid::compute() {
        LNodePtr c = __c.__points;
        Point p(__dimensions);

        int i = 0;
        while(c){
            p += c->point / __c.getSize();
            c = c->next;
            i++;
        }

        if(!__c.__points) {
            toInfinity();
            __valid = true;
            return;
        }
        set(p);
    }

    bool Cluster::Centroid::equal(const Point &point) const {
        for (int i = 0; i < __dimensions; ++i) {
            if (__p[i] == point[i]) {
                return true;
            }
        }
        return false;
    }

    void Cluster::Centroid::toInfinity() {
        for (int i = 0; i < __dimensions; ++i) {
            __p[i] = numeric_limits<double>::max();
        }
    }

    Cluster::Cluster(unsigned int d):
            __dimensionality(d),
            __size(0),
            __points(nullptr),
            __id(__idGenerator++),
            centroid(d, *this)
    {};

    Cluster::Cluster(const Cluster &rtSide) :
            __dimensionality(rtSide.__dimensionality),
            __size(0),
            __points(nullptr),
            __id(rtSide.__id),
            centroid(__dimensionality, *this)
    {
        for (int i = 0; i < rtSide.__size; ++i){
            add(rtSide[i]);
        }
        __id = rtSide.__id;
        centroid.compute();
    }

    Cluster &Cluster::operator=(const Cluster &rtSide)  {
        __size = 0;
        __points = nullptr;

        if(this == &rtSide) {
            return *this;
        }
        else if(__points) {
            __del();
        }
        for(int count = 0; count < rtSide.__size; ++count) {
            add(rtSide[count]);
        }
        __id = rtSide.__id;
        centroid.compute();

        return *this;
    }

    Cluster::~Cluster() {
        __del();
    }

    unsigned int Cluster::getSize() const {
        return __size;
    }

    unsigned int Cluster::getDimensionality() const {
        return __dimensionality;
    }

    unsigned int Cluster::getId() const {
        return __id;
    }

    void Cluster::add(const Point &point) {
        if (point.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(__dimensionality, point.getDims());
        }
        LNodePtr a = new LNode(point, nullptr);
        if (__points == nullptr) {
            __points = a;
        }
        else if (__points->point > point) {
            a->next = __points;
            __points = a;
        }
        else {
            LNodePtr c = __points, n = __points->next;
            while (true) {
                if (n == nullptr || n->point > point) {
                    c->next = a;
                    a->next = n;
                    break;
                }
                else {
                    c = n;
                    n = n->next;
                }
            }
        }
        __size++;
        centroid.setValid(false);
    }

    const Point &Cluster::remove(const Point &point) {
        if (point.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(__dimensionality, point.getDims());
        }

        LNodePtr c = nullptr;
        LNodePtr n = __points;

        if (!__points) {
            return point;
        }

        if (__points->point == point) {
            __points = __points->next;
            delete c;
            __size--;
            centroid.setValid(false);
            return point;
        }
        else {
            c = __points;
            n = __points->next;
            while (n) {
                if(n->point == point){
                    c->next = n->next;
                    delete n;
                    __size--;
                    centroid.setValid(false);
                    return point;
                }
                c = n;
                n = n->next;
            }
        }
        return point;
    }

    bool Cluster::contains(const Point &point) const {
        if (point.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(__dimensionality, point.getDims());
        }
        LNodePtr targetNode = __points;

        while (targetNode != nullptr) {
            if (targetNode->point == point) {
                return true;
            }
            targetNode = targetNode->next;
        }
        return false;
    }

    void Cluster::pickCentroids(unsigned int k, Point **pointArray) {

    }

    const Point &Cluster::operator[](unsigned int index) const {
        if(__size == 0) {
            throw EmptyClusterEx();
        }
        if(index >= __size) {
            throw OutOfBoundsEx(__size, index);
        }
        LNodePtr temp = __points;
        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }
        return (temp->point);
    }

    Cluster &Cluster::operator+=(const Point &point) {
        add(point);
        return *this;
    }

    Cluster &Cluster::operator-=(const Point &point) {
        remove(point);
        return *this;
    }

    Cluster &Cluster::operator+=(const Cluster &cluster) {

        for (int i = 0; i < cluster.__size; i++) {
            if(!contains(cluster[i]))
                add(cluster[i]);
        }
    }

    Cluster &Cluster::operator-=(const Cluster &cluster) {

        for (int i = 0; i < cluster.__size; i++) {
            if(contains(cluster[i]))
                remove(cluster[i]);
        }
    }


    std::ostream &operator<<(std::ostream &os, const Cluster &cluster) {
/*        LNodePtr current;
        current = cluster.__points;

        for(int i = 0; i < cluster.__size; i++) {
            os << current->point << std::endl;
            current = current->next;
        }
        return os;
        */
    }

    std::istream &operator>>(std::istream &is, Cluster &cluster) {
/*        std::string line;
        while (getline(is,line)) {
            int d = (int) std::count(line.begin(),
                                     line.end(),
                                     ',');
            Point ptr(d + 1);
            std::stringstream lineStream(line);
            lineStream >> ptr;
           cluster.add(ptr);
        }
        return is;*/
    }

    bool operator==(const Cluster &ltSide, const Cluster &rtSide) {
        if(ltSide.__dimensionality != rtSide.__dimensionality){
            throw DimensionalityMismatchEx(ltSide.__dimensionality, rtSide.__dimensionality);
        }
        LNodePtr lt;
        LNodePtr rt;

        if (ltSide.__size != rtSide.__size) {
            return false;
        }
        else {
            lt = ltSide.__points;
            rt = rtSide.__points;

            while (lt && rt) {
                if (lt->point == rt->point) {
                    lt = lt->next;
                    rt = rt->next;
                }
                else {
                    return false;
                }
            }
            return true;
        }
    }

    bool operator!=(const Cluster &ltSide, const Cluster &rtSide) {
        return !(ltSide == rtSide);
    }

    const Cluster operator+(const Cluster &cluster, const Point &point) {
        Cluster c(cluster);
        c += point;
        return c;
    }

    const Cluster operator-(const Cluster &cluster, const Point &point) {
        Cluster c(cluster);
        c -= point;
        return c;
    }

    const Cluster operator+(const Cluster &ltCluster, const Cluster &rtCluster) {
        Cluster c(ltCluster);
        c += rtCluster;
        return c;
    }

    const Cluster operator-(const Cluster &ltCluster, const Cluster &rtCluster) {
        Cluster c(ltCluster);
        c -= rtCluster;
        return c;
    }

    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) :
    __p(p),
    __from(from),
    __to(to)
    {
        __to.add(__from.remove(__p));
    }

    void Cluster::Move::perform() {
        __to.centroid.setValid(false);
        __from.centroid.setValid(false);
    }

}