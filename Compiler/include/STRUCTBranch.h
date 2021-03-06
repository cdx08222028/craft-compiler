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
 * File:   STRUCTBranch.h
 * Author: Daniel McCarthy
 *
 * Created on 13 October 2016, 05:47
 */

#ifndef STRUCTBRANCH_H
#define STRUCTBRANCH_H

#include "StructDescriptorBranch.h"
class BODYBranch;
class EXPORT STRUCTBranch : public STRUCTDescriptorBranch
{
public:
    STRUCTBranch(Compiler* compiler);
    virtual ~STRUCTBranch();
    void setStructBodyBranch(std::shared_ptr<BODYBranch> branch);
    std::shared_ptr<BODYBranch> getStructBodyBranch();

    virtual void imp_clone(std::shared_ptr<Branch> cloned_branch);
    virtual std::shared_ptr<Branch> create_clone();
private:

};

#endif /* STRUCTBRANCH_H */

