//
// Created by gansen on 4/21/22.
//

#ifndef WEBRIDGE_DEFS_HPP
#define WEBRIDGE_DEFS_HPP

#include <map>

#ifdef ENABLE_WEBRIDGE
typedef std::map<std::string, std::set<std::string> > class_method_map_t;

#endif //ENABLE_WEBRIDGE

#endif //WEBRIDGE_DEFS_HPP
