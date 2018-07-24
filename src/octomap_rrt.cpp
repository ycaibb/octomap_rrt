//
//  octomap_rrt.cpp
//  OctomapRTT
//
//  Created by 王大鑫 on 2018/7/17.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//

#include "../include/octomap_rrt.hpp"
#include <chrono>

RRT3D::RRT3D(octomap::point3d start_position, octomap::point3d end_position, Map *map, int max_iter, short step_size)
{
    start_position_ = start_position;
    end_position_ = end_position;
    map_ = map;
    root_ = new Node;
    root_->parent = NULL;
    root_->position = start_position_;
    nodes_.push_back(root_);
    lastNode_ = root_;
    max_iter_ = max_iter;
    step_size_ = step_size;
    std::cout<<"step_size = "<<step_size_<<std::endl;
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
    octomap::point3d rand_point;
    short x_max = map_->getBBXMax().x()/ map_->getResolution();
    short y_max = map_->getBBXMax().y()/ map_->getResolution();
    short z_max = map_->getBBXMax().z()/ map_->getResolution();
    map_->getResolution();
    do{
        rand_point = octomap::point3d(rand()%x_max*map_->getResolution(),
                                     rand()%y_max*map_->getResolution(),
                                     rand()%z_max*map_->getResolution());

    }while(map_->isObstacle(rand_point));
    Node *rand_node = new Node;
    rand_node->position = rand_point;
    return rand_node;
}

Node* RRT3D::findNearestNode(octomap::point3d current_position)
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

Node* RRT3D::getNewNode(Node *q_rand, Node *q_nearest, octomap::point3d direction)
{
    octomap::point3d result;
    result.x()= q_nearest->position.x() + map_->getResolution()*step_size_*direction.x();
    result.y()= q_nearest->position.y() + map_->getResolution()*step_size_*direction.y();
    result.z()= q_nearest->position.z() + map_->getResolution()*step_size_*direction.z();
    Node *q_new = new Node;
    q_new->position = result;
    return q_new;
}

bool RRT3D::isNewNodeCollision(octomap::point3d q_new, octomap::point3d q_nearest,  octomap::point3d direction)
{
    int test_step = 4;
    bool ret = false;
    for(int i=1; i<=test_step; i++){
        float step = 1.0*i/test_step*step_size_;
        octomap::point3d test_point;
        test_point.x()= q_nearest.x() + map_->getResolution()*step*direction.x();
        test_point.y()= q_nearest.y() + map_->getResolution()*step*direction.y();
        test_point.z()= q_nearest.z() + map_->getResolution()*step*direction.z();
        if(map_->isObstacle(test_point)){
            ret = true;
            break;
        }
    }
    return ret;
}

void RRT3D::addNewNode(Node *q_nearest, Node *q_new)
{
    q_new->parent = q_nearest;
    q_nearest->children.push_back(q_new);
    nodes_.push_back(q_new);
    lastNode_ = q_new;
//    std::cout<<"Get a new Node"<<std::endl;
//    std::cout<<"Now nodes have:"<<nodes_.size()<<std::endl;
//    for(auto node:nodes_){
//        std::cout<<node->position<<"\n";
//    }
//    std::cout<<"\n";
}

bool RRT3D::isArrived()
{
//    if((lastNode_->position - end_position_).norm() < 2.2*map_->getResolution())
//        return true;
    if((lastNode_->position - end_position_).norm() < 5*map_->getResolution())
        return true;
    return false;
}

void RRT3D::run(bool debug)
{
    srand(static_cast<ushort>(time(NULL)));
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    for(int i=0; i<max_iter_; i++){
        if(debug)
            if(i%100==0)
                std::cout<<"i="<<i<<std::endl;
        Node *q_rand = getRandomNotObstacleNode();
//        std::cout<<"random_point: "<<q_rand->position<<std::endl;
        Node *q_nearest = findNearestNode(q_rand->position);
//        std::cout<<"nearest_node: "<<q_nearest->position<<std::endl;
        octomap::point3d direction = q_rand->position - q_nearest->position;
        if(direction.norm() > step_size_){
            direction = direction.normalize();
            Node *q_new = getNewNode(q_rand, q_nearest, direction);
            if(!isNewNodeCollision(q_new->position, q_nearest->position, direction))
                addNewNode(q_nearest, q_new);
        }
        if(isArrived()){
            break;
        }
    }
    Node *q;
    if(isArrived()){
        q = lastNode_;
        std::cout<<"Find a path\n";
    }
    else{
        q = findNearestNode(end_position_);
        std::cout<<"Can not find the path\n";
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout<<"calculate_time = "<<elapsed_seconds.count()<<"s\n";
    while(q!=NULL){
        path_.push_back(q);
        q = q->parent;
    }
}

void RRT3D::writeMap()
{
    //visualize
    for(auto node: path_){
        std::cout<<node->position<<std::endl;
        map_->mixPathMap(node->position, true);
        //        path_tree->setNodeColor(node->position.x(),node->position.y(),node->position.x(), 255, 0, 0);
    }
}
