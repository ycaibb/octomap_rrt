//
//  rrt.cpp
//  OctomapRTT
//
//  Created by Daxin Wang on 2018/7/16.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//

#include "rrt.hpp"
#include <cmath>
#include <algorithm>
RRT3D::RRT3D(Eigen::Vector3f start_position, Eigen::Vector3f end_position, Map map, int max_iter, int step_size)
{
    start_position_ = start_position;
    end_position_ = end_position;
    map_ = map;
    root_ = new Node;
    root_->parent = NULL;
    root_->position = start_position_;
    lastNode_ = root_;
    nodes_.push_back(root_);
    max_iter_ = max_iter;
    step_size_ = step_size;
}

RRT3D::~RRT3D()
{
    deleteNodes(root_);
}
void RRT3D::deleteNodes(Node *root)
{
    for(auto node:root->children) {
        deleteNodes(node);
    }
    delete root;
}

Node* RRT3D::getRandomNotObstacleNode()
{
//    int random_num = 0;
//    std::pair<Eigen::Vector3f, bool> random_pair;
    Eigen::Vector3f rand_point;
    srand48(time(NULL));
    do{
        rand_point = Eigen::Vector3f(round(drand48()*map_.x_max_),
                                     round(drand48()*map_.y_max_),
                                     round(drand48()*map_.z_max_));
//        random_num = static_cast<int>(drand48()*map_.cells_.size());
//        random_pair = map_.cells_[random_num];
        
    }while(map_.isObstacle(rand_point));
    Node *rand_node = new Node;
    rand_node->position = rand_point;
    return rand_node;
}

Node* RRT3D::findNearestNode(Eigen::Vector3f current_position)
{
    double min_distance = 1e5;
    Node* closest_node = NULL;
    for(auto node:nodes_){
        double distance = (current_position - node->position).norm();
        if(distance<min_distance){
            min_distance = distance;
            closest_node = node;
        }
    }
    return closest_node;
}

Node* RRT3D::getNewNode(Node *q_rand, Node *q_nearest)
{
    Eigen::Vector3f direction = q_rand->position - q_nearest->position;
    direction = direction / direction.norm();
    Eigen::Vector3f result = q_nearest->position + step_size_*direction;
    Node *q_new = new Node;
    q_new->position = result;
    return q_new;
}

void RRT3D::addNewNode(Node *q_nearest, Node *q_new)
{
    q_new->parent = q_nearest;
    q_nearest->children.push_back(q_new);
    nodes_.push_back(q_new);
    lastNode_ = q_new;
}

bool RRT3D::isArrived()
{
    if((lastNode_->position - end_position_).norm() < 1)
        return true;
    return false;
}

void RRT3D::run()
{
    for(int i=0; i<max_iter_; i++){
        Node *q_rand = getRandomNotObstacleNode();
        if(q_rand!=NULL) {
            Node *q_nearest = findNearestNode(q_rand->position);
            if((q_rand->position - q_nearest->position).norm() > step_size_){
                Node *q_new = getNewNode(q_rand, q_nearest);
                addNewNode(q_nearest, q_new);
            }
        }
        if(isArrived()){
            break;
        }
    }
    Node *q;
    if(isArrived()){
        q = lastNode_;
    }
    else{
        q = findNearestNode(end_position_);
        std::cout<<"Can not find the path\n";
    }
    while(q!=NULL){
        path_.push_back(q);
        q = q->parent;
    }
    for(auto node: path_){
        std::cout<<node->position<<std::endl;
    }
}

