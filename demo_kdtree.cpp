//
// Demonstration of typical use cases of the kd-tree library
// Author:  Christoph Dalitz, 2018-02-04
// License: BSD style license (see the file LICENSE)
//

#include <time.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "kdtree.hpp"

using namespace std;

//
// helper function for printing points
//
void print_nodes(const Kdtree::KdNodeVector &nodes) {
  size_t i,j;
  for (i = 0; i < nodes.size(); ++i) {
    if (i > 0)
      cout << " ";
    cout << "(";
    for (j = 0; j < nodes[i].point.size(); j++) {
      if (j > 0)
        cout << ",";
      cout << nodes[i].point[j];
    }
    cout << ")";
  }
  cout << endl;
}

//
// main program demonstrating typical use cases
//
int main(int argc, char** argv) {

  //
  // functionality tests
  //
  cout << "Functionality tests" << endl;
  cout << "-------------------" << endl;
  
  // 1.1) construction of kd-tree
  Kdtree::KdNodeVector nodes;
  double points[10][2] = {{1, 1}, {2, 1}, {1, 2}, {2, 4}, {3, 4},
                          {7, 2}, {8, 3}, {8, 5}, {7, 3}, {7, 3}};
  for (int i = 0; i < 10; ++i) {
    std::vector<double> point(2);
    point[0] = points[i][0];
    point[1] = points[i][1];
    nodes.push_back(Kdtree::KdNode(point));
  }
  Kdtree::KdTree tree(&nodes);
  cout << "Points in kd-tree:\n  ";
  print_nodes(tree.allnodes);

  // 1.2) kNN search
  Kdtree::KdNodeVector result;
  std::vector<double> test_point(2);
  test_point[0] = 8;
  test_point[1] = 3;
  tree.k_nearest_neighbors(test_point, 3, &result);
  cout << "3NNs of (" << test_point[0] << "," << test_point[1] << "):\n  ";
  print_nodes(result);

  // 1.3) kNN search with predicate
  struct Predicate : Kdtree::KdNodePredicate {
    std::vector<double> point;
    Predicate(std::vector<double> p) {
      this->point = p;
    }
    // only search for points with smaller y-coordinate
    bool operator()(const Kdtree::KdNode& kn) const {
      return this->point[1] > kn.point[1];
    }
  };
  Predicate pred(test_point);
  tree.k_nearest_neighbors(test_point, 3, &result, &pred);
  cout << "3NNs of (" << test_point[0] << "," << test_point[1] << ") with smaller y-coordinate:\n  ";
  print_nodes(result);

  // 1.4) range query
  tree.range_nearest_neighbors(test_point, 2.0, &result);
  cout << "Neighbors of (" << test_point[0] << "," << test_point[1] << ") with distance <= 2.0:\n  ";
  print_nodes(result);

  //
  // speed tests
  //
  cout << "\nSpeed tests" << endl;
  cout << "-----------" << endl;

  double diff;
  size_t N = 500000;
  nodes.clear();
  for (size_t i = 0; i < N; ++i) {
    std::vector<double> point(2);
    point[0] = (double)rand() / RAND_MAX;
    point[1] = (double)rand() / RAND_MAX;
    nodes.push_back(Kdtree::KdNode(point));
  }
  
  // 2.1) build time
  clock_t begin = clock();
  Kdtree::KdTree tree2(&nodes);
  clock_t end = clock();
  diff = double(end - begin) / CLOCKS_PER_SEC;
  cout << "Creation time for " << N << " random points:\n  " << diff << "s" << endl;

  // 2.2) one kNN query
  test_point[0] = (double)rand() / RAND_MAX;
  test_point[1] = (double)rand() / RAND_MAX;
  begin = clock();
  tree.k_nearest_neighbors(test_point, 3, &result);
  end = clock();
  diff = double(end - begin) / CLOCKS_PER_SEC;
  cout << "3NN search time for one random point:\n  " << diff << "s" << endl;

  // 2.3) all NN query
  begin = clock();
  for (size_t i = 0; i < N; ++i) {
    std::vector<double> point(2);
    tree.k_nearest_neighbors(nodes[i].point, 2, &result);
  }
  end = clock();
  diff = double(end - begin) / CLOCKS_PER_SEC;
  cout << "Time for all nearest neighbor search:\n  " << diff << "s" << endl;

  // 2.4) range query
  double r = 0.3;
  begin = clock();
  tree.range_nearest_neighbors(test_point, r, &result);
  end = clock();
  diff = double(end - begin) / CLOCKS_PER_SEC;
  cout << "Range search time for one random point (r=" << r << "):\n  " << diff << endl;

  // 2.5) for comparison: loop version
  std::vector<Kdtree::CoordPoint> res;
  begin = clock();
  for (size_t i = 0; i < N; ++i) {
    if ((nodes[i].point[0] - test_point[0]) *
        (nodes[i].point[0] - test_point[0]) +
        (nodes[i].point[1] - test_point[1]) *
        (nodes[i].point[1] - test_point[1]) < r * r)
        res.push_back(nodes[i].point);
    }
    end = clock();
    diff = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Loop version of a single range search for comparison:\n  " << diff << "s" << endl;

  return 0;
}
