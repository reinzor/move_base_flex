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
 *  move_base_navigation_server.h
 *
 *  authors:
 *    Sebastian Pütz <spuetz@uni-osnabrueck.de>
 *    Jorge Santos Simón <santos@magazino.eu>
 *
 */

#ifndef MBF_COSTMAP_NAV__COSTMAP_NAVIGATION_SERVER_H_
#define MBF_COSTMAP_NAV__COSTMAP_NAVIGATION_SERVER_H_

#include <std_srvs/Empty.h>

#include <mbf_msgs/CheckPose.h>
#include <mbf_msgs/MoveBaseAction.h>
#include <mbf_costmap_nav/MoveBaseFlexConfig.h>

#include "mbf_abstract_nav/abstract_navigation_server.h"

#include "costmap_planner_execution.h"
#include "costmap_controller_execution.h"
#include "costmap_recovery_execution.h"

namespace mbf_costmap_nav
{
/**
 * @defgroup move_base_server Move Base Server
 * @brief Classes belonging to the Move Base Server level.
 */


typedef boost::shared_ptr<dynamic_reconfigure::Server<mbf_costmap_nav::MoveBaseFlexConfig> > DynamicReconfigureServerCostmapNav;

/**
 * @brief The CostmapNavigationServer makes Move Base Flex backwards compatible to the old move_base. It combines the
 *        execution classes which use the nav_core/BaseLocalPlanner, nav_core/BaseCostmapPlanner and the
 *        nav_core/RecoveryBehavior base classes as plugin interfaces. These plugin interface are the same for the
 *        old move_base
 *
 * @ingroup navigation_server move_base_server
 */
class CostmapNavigationServer : public mbf_abstract_nav::AbstractNavigationServer
{
public:

  typedef boost::shared_ptr<costmap_2d::Costmap2DROS> CostmapPtr;

  /**
   * @brief Constructor
   * @param tf_listener_ptr Shared pointer to a common TransformListener
   */
  CostmapNavigationServer(const boost::shared_ptr<tf::TransformListener> &tf_listener_ptr);

  /**
   * @brief Destructor
   */
  virtual ~CostmapNavigationServer();

protected:

  /**
   * check whether the costmaps should be activated.
   */
  void checkActivateCostmaps();

  /**
   * @brief checks whether the costmaps should and could be deactivated
   */
  void checkDeactivateCostmaps();

  /**
   * @brief Callback method for the check_pose_cost service
   * @param request Request object, see the move_base_flex_msgs/CheckPose service definition file.
   * @param response Response object, see the move_base_flex_msgs/CheckPose service definition file.
   * @return true, if the service completed successfully, false otherwise
   */
  bool callServiceCheckPoseCost(mbf_msgs::CheckPose::Request &request,
                                mbf_msgs::CheckPose::Response &response);

  /**
   * @brief Callback method for the make_plan service
   * @param request Empty request object.
   * @param response Empty response object.
   * @return true, if the service completed successfully, false otherwise
   */
  bool callServiceClearCostmaps(std_srvs::Empty::Request &request, std_srvs::Empty::Response &response);

  /**
   * @brief GetPath action execution method. This method will be called if the action server receives a goal. It
   *        extends the base class method by calling the checkActivateCostmaps() and checkDeactivateCostmaps().
   * @param goal SimpleActionServer goal containing all necessary parameters for the action execution. See the action
   *        definitions in move_base_flex_msgs.
   */
  virtual void callActionGetPath(const mbf_msgs::GetPathGoalConstPtr &goal);

  /**
   * @brief ExePath action execution method. This method will be called if the action server receives a goal. It
   *        extends the base class method by calling the checkActivateCostmaps() and checkDeactivateCostmaps().
   * @param goal SimpleActionServer goal containing all necessary parameters for the action execution. See the action
   *        definitions in move_base_flex_msgs.
   */
  virtual void callActionExePath(const mbf_msgs::ExePathGoalConstPtr &goal);

  /**
   * @brief Recovery action execution method. This method will be called if the action server receives a goal. It
   *        extends the base class method by calling the checkActivateCostmaps() and checkDeactivateCostmaps().
   * @param goal SimpleActionServer goal containing all necessary parameters for the action execution. See the action
   *        definitions in move_base_flex_msgs.
   */
  virtual void callActionRecovery(const mbf_msgs::RecoveryGoalConstPtr &goal);

  /**
   *
   * @brief Reconfiguration method called by dynamic reconfigure.
   * @param config Configuration parameters. See the MoveBaseFlexConfig definition.
   * @param level bit mask, which parameters are set.
   */
  void reconfigure(mbf_costmap_nav::MoveBaseFlexConfig &config, uint32_t level);

  //! Dynamic reconfigure server for the mbf_costmap2d_specific part
  DynamicReconfigureServerCostmapNav dsrv_costmap_;

  //! last configuration save
  mbf_costmap_nav::MoveBaseFlexConfig last_config_;

  //! the default parameter configuration save
  mbf_costmap_nav::MoveBaseFlexConfig default_config_;

  //! true, if the dynamic reconfigure has been setup.
  bool setup_reconfigure_;

  //! Shared pointer to the common local costmap
  CostmapPtr costmap_controller_ptr_;

  //! Shared pointer to the common global costmap
  CostmapPtr costmap_planner_ptr_;

  //! true, if the local costmap is active
  bool local_costmap_active_;

  //! true, if the global costmap is active
  bool global_costmap_active_;

  typedef actionlib::SimpleActionServer<mbf_msgs::MoveBaseAction> ActionServerMoveBase;
  typedef boost::shared_ptr<ActionServerMoveBase> ActionServerMoveBasePtr;

  //! Shared pointer to the MoveBase action server
  ActionServerMoveBasePtr move_base_as_ptr_;

  //! Service Server for the check_pose_cost service
  ros::ServiceServer check_pose_cost_srv_;

  //! Service Server for the clear_costmap service
  ros::ServiceServer clear_costmaps_srv_;

  //! stop updating costmaps when not planning or controlling, if true
  bool shutdown_costmaps_;


  bool isQuaternionValid(const geometry_msgs::Quaternion &q);
  geometry_msgs::PoseStamped goalToGlobalFrame(const geometry_msgs::PoseStamped &goal_pose_msg);
  bool makePlan(const geometry_msgs::PoseStamped& goal, std::vector<geometry_msgs::PoseStamped> &plan);
  void wakePlanner(const ros::TimerEvent &event);
  void planThread();
  void callActionMoveBase(const mbf_msgs::MoveBaseGoalConstPtr &move_base_goal);
  double distance(const geometry_msgs::PoseStamped &p1, const geometry_msgs::PoseStamped &p2);
  bool executeCycle(geometry_msgs::PoseStamped &goal, std::vector<geometry_msgs::PoseStamped> &global_plan);
  void resetState();

  unsigned int recovery_index_ = 0;
  std::vector<std::string> recovery_behaviors_ = recovery_ptr_->listRecoveryBehaviors();

  tf::Stamped<tf::Pose> global_pose_;
  double planner_frequency_, controller_frequency_, inscribed_radius_, circumscribed_radius_;
  double planner_patience_, controller_patience_;
  int32_t max_planning_retries_;
  uint32_t planning_retries_;
  double conservative_reset_dist_, clearing_radius_;
  //ros::Publisher current_goal_pub_, vel_pub_, action_goal_pub_;
//  ros::Subscriber goal_sub_;
//  ros::ServiceServer make_plan_srv_, clear_costmaps_srv_;
  bool clearing_rotation_allowed_, recovery_behavior_enabled_;
  double oscillation_timeout_, oscillation_distance_;


  enum MoveBaseState {
    PLANNING,
    CONTROLLING,
    CLEARING
  };

  enum RecoveryTrigger
  {
    PLANNING_R,
    CONTROLLING_R,
    OSCILLATION_R
  };

  MoveBaseState state_;
  RecoveryTrigger recovery_trigger_;

  ros::Time last_valid_plan_, last_valid_control_, last_oscillation_reset_;
  geometry_msgs::PoseStamped oscillation_pose_;

  //set up plan triple buffer
  std::vector<geometry_msgs::PoseStamped>* planner_plan_;
  std::vector<geometry_msgs::PoseStamped>* latest_plan_;
  std::vector<geometry_msgs::PoseStamped>* controller_plan_;

  //set up the planner's thread
  bool runPlanner_;
  bool new_global_plan_;
  bool p_freq_change_, c_freq_change_;
  boost::recursive_mutex planner_mutex_;
  boost::condition_variable_any planner_cond_;
  geometry_msgs::PoseStamped planner_goal_;
  boost::thread* planner_thread_;

//  std::vector<std::string> recovery_behaviors_;
//  unsigned int recovery_index_;

};

} /* namespace mbf_costmap_nav */

#endif /* MBF_COSTMAP_NAV__COSTMAP_NAVIGATION_SERVER_H_ */
