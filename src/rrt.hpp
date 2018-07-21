//
//  rrt.hpp
//  OctomapRTT
//
//  Created by Daxin Wang on 2018/7/16.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//

#ifndef rrt_hpp
#define rrt_hpp

#include <vector>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include "map.hpp"

using namespace std;

struct Node {
    vector<Node *> children;
    Node *parent;
    Eigen::Vector3f position;
};

class RRT3D{
public:
    RRT3D(Eigen::Vector3f start_position, Eigen::Vector3f end_position, Map map, int max_iter, int step_size);
    ~RRT3D();
    
    void deleteNodes(Node *root);
    Node* findNearestNode(Eigen::Vector3f current_position);
    Node* getRandomNotObstacleNode();
    Node* getNewNode(Node *q, Node *q_nearest);
    void addNewNode(Node *q_nearest, Node *q_new);
    bool isArrived();

    void run();
    
private:
    vector<Node *> nodes_;
    vector<Node *> path_;
    Node *root_;
    Node *lastNode_;
    Eigen::Vector3f start_position_;
    Eigen::Vector3f end_position_;
    
    Map map_;

    int max_iter_;
    int step_size_;
};
#endif /* rrt_hpp */
