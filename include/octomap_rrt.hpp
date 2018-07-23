//
//  octomap_rrt.hpp
//  OctomapRTT
//
//  Created by 王大鑫 on 2018/7/17.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//

#ifndef octomap_rrt_hpp
#define octomap_rrt_hpp

#include <vector>
#include <iostream>
#include <octomap/octomap.h>
#include "map.hpp"

using namespace std;
struct Node;
struct Node {
    vector<Node *> children;
    Node *parent;
    octomap::point3d position;
};

class RRT3D{
public:
    RRT3D(octomap::point3d start_position, octomap::point3d end_position, Map *map, int max_iter, short step_size);
    ~RRT3D();
    
    void deleteNodes(Node *root);
    Node* findNearestNode(octomap::point3d current_position);
    Node* getRandomNotObstacleNode();
    Node* getNewNode(Node *q, Node *q_nearest, octomap::point3d direction);
    void addNewNode(Node *q_nearest, Node *q_new);
    bool isNewNodeCollision(octomap::point3d q_new, octomap::point3d q_nearest, octomap::point3d direction);
    bool isArrived();
    
    void run();
    void writeMap();
private:
    vector<Node *> nodes_;
    vector<Node *> path_;
    Node *root_;
    Node *lastNode_;
    octomap::point3d start_position_;
    octomap::point3d end_position_;
    
    Map *map_;
    
    int max_iter_;
    //step_size: 1 unit based
    short step_size_;
};

#endif /* octomap_rrt_hpp */
