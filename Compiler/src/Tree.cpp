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
 * File:   Tree.cpp
 * Author: Daniel McCarthy
 *
 * Created on 29 May 2016, 20:35
 * 
 * Description: Holds helper methods and also holds children of an AST(Abstract Syntax Tree)
 */

#include "Tree.h"
#include "STRUCTBranch.h"
#include "RootBranch.h"
#include "FuncDefBranch.h"

Tree::Tree()
{
    this->root = NULL;
}

Tree::~Tree()
{
}

std::shared_ptr<STRUCTBranch> Tree::getGlobalStructureByName(std::string name)
{
    return this->root->getDeclaredStructureByName(name);
}

bool Tree::isGlobalStructureDeclared(std::string name)
{
    return this->root->isStructureDeclared(name);
}

std::shared_ptr<FuncDefBranch> Tree::getGlobalFunctionDefinitionByName(std::string name)
{
    return this->root->getDeclaredFunctionDefinitionByName(name);
}

bool Tree::hasGlobalFunctionDefinition(std::string name)
{
    return this->root->isFunctionDefinitionDeclared(name);
}