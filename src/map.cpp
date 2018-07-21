//
//  map.cpp
//  OctomapRTT
//
//  Created by Daxin Wang on 2018/7/16.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//

#include "map.hpp"
using namespace octomap;
Map::Map(float resolution)
{
    map_tree_ = new octomap::OcTree("./hello.bt");
//    std::vector<octomap::point3d> points = {point3d(0,0,0), point3d(1,1,1), point3d(0.5,0,0), point3d(1,0,0)};
//    for(auto p:points){
//        map_tree_->updateNode(p, true);
//    }
    bbx_max_ = point3d(40,40,40);
    bbx_min_ = point3d(0,0,0);
    map_tree_->setBBXMax(bbx_max_);
    map_tree_->setBBXMin(bbx_min_);
    
//    map_tree_->writeBinary("./map.bt");
}
Map::~Map()
{
    delete map_tree_;
}
bool Map::isObstacle(octomap::point3d point)
{
    //to do
    octomap::OcTreeNode* node = map_tree_->search(point);
    if(node!=NULL && node->getOccupancy()>0.5){
        return true;
    }
    return false;
}
