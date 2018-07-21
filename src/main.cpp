//
//  main.cpp
//  OctomapRTT
//
//  Created by Daxin Wang on 2018/7/15.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//

#include "octomap_rrt.hpp"
#include "map.hpp"
#include <octomap/octomap.h>
#include <octomap/ColorOcTree.h>

using namespace std;
using namespace octomap;

int main(int argc, char** argv) {
    Map* map = new Map(0.1);
    octomap::point3d start_position(0,0,0);
    octomap::point3d end_position(39,20,30);
    RRT3D rrt(start_position, end_position, map, 60000, 1);
    rrt.run();
}
