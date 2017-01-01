/*
    Craft Compiler v0.1.0 - The standard compiler for the Craft programming language.
    Copyright (C) 2016  Daniel McCarthy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   Tree.h
 * Author: Daniel McCarthy
 *
 * Created on 29 May 2016, 20:35
 */

#ifndef TREE_H
#define TREE_H

#include "Branch.h"

class STRUCTBranch;
class RootBranch;
class Tree {
public:
    Tree();
    virtual ~Tree();
    
    // Returns a structure based on the structure name
    std::shared_ptr<STRUCTBranch> getGlobalStructureByName(std::string name);
    std::shared_ptr<RootBranch> root;
private:

};

#endif /* TREE_H */

