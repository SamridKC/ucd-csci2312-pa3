//
// Created by Samrid on 3/22/2016.
//

#include <iostream>

#include "KMeans.h"

using namespace std;

namespace Clustering{

    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {

    }

    KMeans::~KMeans() {

    }

    unsigned int KMeans::getMaxIter() {
        return __maxIter;
    }

    unsigned int KMeans::getNumIters() {
        return __numIter;
    }

    unsigned int KMeans::getNumNonemptyClusters() {
        return __numNonempty;
    }

    unsigned int KMeans::getNumMovesLastIter() {
        return __numMovesLastIter;
    }

    Cluster &KMeans::operator[](unsigned int u) {

    }

    const Cluster &KMeans::operator[](unsigned int u) const {

    }

    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans) {

    }

    void KMeans::run() {

    }
}
