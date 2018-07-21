//
//  map.hpp
//  OctomapRTT
//
//  Created by Daxin Wang on 2018/7/16.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//

#ifndef map_hpp
#define map_hpp

#include <vector>
#include <octomap/octomap.h>

class Map{
public:
    Map(float resolution=0.1);
    ~Map();
    bool isObstacle(octomap::point3d point);
    double getResolution(){ return map_tree_->getResolution();}
    octomap::point3d getBBXMax(){ return bbx_max_; }
    octomap::point3d getBBXMin(){ return bbx_min_; }
private:
    octomap::OcTree* map_tree_;
    float resolution_;
    octomap::point3d bbx_max_;
    octomap::point3d bbx_min_;
};
#endif /* map_hpp */
