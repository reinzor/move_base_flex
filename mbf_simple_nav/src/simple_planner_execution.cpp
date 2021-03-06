/*
 *  Copyright 2017, Magazino GmbH, Sebastian Pütz, Jorge Santos Simón
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  simple_planner_execution.cpp
 *
 *  authors:
 *    Sebastian Pütz <spuetz@uni-osnabrueck.de>
 *    Jorge Santos Simón <santos@magazino.eu>
 *
 */

#include "mbf_simple_nav/simple_planner_execution.h"

namespace mbf_simple_nav
{

SimplePlannerExecution::SimplePlannerExecution(boost::condition_variable &condition) :
    mbf_abstract_nav::AbstractPlannerExecution(condition)
{
}

SimplePlannerExecution::~SimplePlannerExecution()
{
}

mbf_abstract_core::AbstractPlanner::Ptr SimplePlannerExecution::loadPlannerPlugin(const std::string& planner_type)
{
  static pluginlib::ClassLoader<mbf_abstract_core::AbstractPlanner>
      class_loader("mbf_abstract_core", "mbf_abstract_core::AbstractPlanner");
  mbf_abstract_core::AbstractPlanner::Ptr planner_ptr;
  ROS_INFO("Load global planner plugin.");
  try
  {
    planner_ptr = class_loader.createInstance(planner_type);
  }
  catch (const pluginlib::PluginlibException &ex)
  {
    ROS_FATAL_STREAM("Failed to load the " << planner_type << " planner, are you sure it is properly registered"
                                           << " and that the containing library is built? Exception: " << ex.what());
  }
  ROS_INFO("Global planner plugin loaded.");

  return planner_ptr;
}

void SimplePlannerExecution::initPlugin()
{
}

} /* namespace mbf_simple_nav */
