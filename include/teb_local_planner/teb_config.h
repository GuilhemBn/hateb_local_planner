/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2016,
 *  TU Dortmund - Institute of Control Theory and Systems Engineering.
 *  All rights reserved.
 *
 *  Copyright (c) 2016 LAAS/CNRS
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the institute nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Christoph Rösmann
 *          Harmish Khambhaita (harmish@laas.fr)
 *********************************************************************/

#ifndef TEB_CONFIG_H_
#define TEB_CONFIG_H_

#include <ros/console.h>
#include <ros/ros.h>
#include <Eigen/Core>
#include <Eigen/StdVector>

#include <teb_local_planner/TebLocalPlannerReconfigureConfig.h>

// Definitions
#define USE_ANALYTIC_JACOBI // if available for a specific edge, use analytic
                            // jacobi

namespace teb_local_planner {

/**
 * @class TebConfig
 * @brief Config class for the teb_local_planner and its components.
 */
class TebConfig {
public:
  std::string odom_topic; //!< Topic name of the odometry message, provided by
                          //! the robot driver or simulator
  std::string map_frame;  //!< Global planning frame

  int planning_mode;

  //! Trajectory related parameters
  struct Trajectory {
    double teb_autosize; //!< Enable automatic resizing of the trajectory w.r.t
                         //! to the temporal resolution (recommended)
    double dt_ref; //!< Desired temporal resolution of the trajectory (should be
                   //! in the magniture of the underlying control rate)
    double dt_hysteresis; //!< Hysteresis for automatic resizing depending on
                          //! the current temporal resolution (dt): usually 10%
    //! of dt_ref
    int min_samples; //!< Minimum number of samples (should be always greater
                     //! than 2)
    int human_min_samples;
    bool global_plan_overwrite_orientation; //!< Overwrite orientation of local
                                            //! subgoals provided by the global
    //! planner
    double global_plan_viapoint_sep; //!< Min. separation between each two
                                     //! consecutive via-points extracted from
    //! the global plan (if negative: disabled)
    bool via_points_ordered; //!< If true, the planner adheres to the order of
                             //! via-points in the storage container
    double max_global_plan_lookahead_dist; //!< Specify maximum length
                                           //!(cumulative Euclidean distances)
                                           //! of the subset of the global plan
    //! taken into account for
    //! optimization [if <=0: disabled;
    //! the length is also bounded by the
    //! local costmap size!]
    double force_reinit_new_goal_dist; //!< Reinitialize the trajectory if a
                                       //! previous goal is updated with a
    //! seperation of more than the specified
    //! value in meters (skip hot-starting)
    int feasibility_check_no_poses; //!< Specify up to which pose on the
                                    //! predicted plan the feasibility should be
    //! checked each sampling interval.
    bool publish_feedback; //!< Publish planner feedback containing the full
                           //! trajectory and a list of active obstacles (should
    //! be enabled only for evaluation or debugging
    //! purposes)
    bool shrink_horizon_backup; //!< Allows the planner to shrink the horizon
                                //! temporary (50%) in case of automatically
    //! detected issues.
    double horizon_reduction_amount;
    double teb_init_skip_dist;
  } trajectory; //!< Trajectory related parameters

  //! Robot related parameters
  struct Robot {
    double max_vel_x;           //!< Maximum translational velocity of the robot
    double min_vel_x;           //!< Minumum translational velocity of the robot
    double max_vel_x_backwards; //!< Maximum translational velocity of the robot
                                //! for driving backwards
    double min_vel_x_backwards; //!< Minumum translational velocity of the robot
                                //! for driving backwards
    double max_vel_theta;       //!< Maximum angular velocity of the robot
    double min_vel_theta;       //!< Minumum angular velocity of the robot
    double acc_lim_x;     //!< Maximum translational acceleration of the robot
    double acc_lim_theta; //!< Maximum angular acceleration of the robot
    double min_turning_radius; //!< Minimum turning radius of a carlike robot
                               //!(diff-drive robot: zero);
    double wheelbase; //!< The distance between the drive shaft and steering
                      //! axle (only required for a carlike robot with
    //!'cmd_angle_instead_rotvel' enabled); The value might be
    //! negative for back-wheeled robots!
    bool cmd_angle_instead_rotvel; //!< Substitute the rotational velocity in
                                   //! the commanded velocity message by the
    //! corresponding steering angle (check
    //!'axles_distance')
  } robot; //!< Robot related parameters

  struct Human {
    double radius;
    double min_human_robot_dist;
    double min_human_human_dist;
    double max_vel_x;
    double min_vel_x;
    double nominal_vel_x;
    double max_vel_x_backwards;
    double min_vel_x_backwards;
    double max_vel_theta;
    double min_vel_theta;
    double acc_lim_x;
    double acc_lim_theta;
    bool use_external_prediction;
    bool predict_human_behind_robot;
    double ttc_threshold;
    double ttcplus_threshold;                                        //michele
    double ttclosest_threshold;                                    //michele
    double ttcplus_timer;                                          //michele
    double dir_cost_threshold;
    double visibility_cost_threshold;
    double pose_prediction_reset_time;
    double fov;
  } human;

  //! Goal tolerance related parameters
  struct GoalTolerance {
    double yaw_goal_tolerance; //!< Allowed final orientation error
    double xy_goal_tolerance;  //!< Allowed final euclidean distance to the goal
                               //! position
    bool free_goal_vel; //!< Allow the robot's velocity to be nonzero (usally
                        //! max_vel) for planning purposes
  } goal_tolerance;     //!< Goal tolerance related parameters

  //! Obstacle related parameters
  struct Obstacles {
    double min_obstacle_dist; //!< Minimum desired separation from obstacles
    bool use_nonlinear_obstacle_penalty;
    double obstacle_cost_mult;
    bool include_costmap_obstacles; //!< Specify whether the obstacles in the
                                    //! costmap should be taken into account
    //! directly
    double costmap_obstacles_behind_robot_dist; //!< Limit the occupied local
                                                //! costmap obstacles taken into
    //! account for planning behind
    //! the robot (specify distance
    //! in meters)
    int obstacle_poses_affected; //!< The obstacle position is attached to the
                                 //! closest pose on the trajectory to reduce
    //! computational effort, but take a number of
    //! neighbors into account as well
    std::string costmap_converter_plugin; //!< Define a plugin name of the
                                          //! costmap_converter package (costmap
    //! cells are converted to
    //! points/lines/polygons)
    bool costmap_converter_spin_thread; //!< If \c true, the costmap converter
                                        //! invokes its callback queue in a
    //! different thread
    int costmap_converter_rate; //!< The rate that defines how often the
                                //! costmap_converter plugin processes the
    //! current costmap (the value should not be much
    //! higher than the costmap update rate)
  } obstacles; //!< Obstacle related parameters

  //! Optimization related parameters
  struct Optimization {
    int no_inner_iterations; //!< Number of solver iterations called in each
                             //! outerloop iteration
    int no_outer_iterations; //!< Each outerloop iteration automatically resizes
                             //! the trajectory and invokes the internal
    //! optimizer with no_inner_iterations

    bool optimization_activate; //!< Activate the optimization
    bool optimization_verbose;  //!< Print verbose information

    double penalty_epsilon; //!< Add a small safety margin to penalty functions
                            //! for hard-constraint approximations
    double time_penalty_epsilon;
    bool cap_optimaltime_penalty;

    double weight_max_vel_x; //!< Optimization weight for satisfying the maximum
                             //! allowed translational velocity
    double weight_max_human_vel_x; //!< Optimization weight for satisfying the
    //! maximum allowed translational velocity for
    //! humans
    double weight_nominal_human_vel_x;
    double weight_max_vel_theta; //!< Optimization weight for satisfying the
                                 //! maximum allowed angular velocity
    double weight_max_human_vel_theta; //!< Optimization weight for satisfying
                                       //! the maximum allowed angular velocity
    //! for humans
    double weight_acc_lim_x; //!< Optimization weight for satisfying the maximum
                             //! allowed translational acceleration
    double weight_human_acc_lim_x; //!< Optimization weight for satisfying the
    //! maximum allowed translational acceleration
    //! for humans
    double weight_acc_lim_theta; //!< Optimization weight for satisfying the
                                 //! maximum allowed angular acceleration
    double weight_human_acc_lim_theta; //!< Optimization weight for satisfying
                                       //! the maximum allowed angular
    //! acceleration for humans
    double weight_kinematics_nh; //!< Optimization weight for satisfying the
                                 //! non-holonomic kinematics
    double weight_kinematics_forward_drive; //!< Optimization weight for forcing
                                            //! the robot to choose only forward
    //! directions (positive transl.
    //! velocities, only diffdrive robot)
    double weight_kinematics_turning_radius; //!< Optimization weight for
                                             //! enforcing a minimum turning
    //! radius (carlike robots)
    double weight_optimaltime; //!< Optimization weight for contracting the
                               //! trajectory w.r.t transition time
    double weight_human_optimaltime; //!< Optimization weight for contracting the
                               //! trajectory w.r.t transition time
    double weight_obstacle;    //!< Optimization weight for satisfying a minimum
                               //! separation from obstacles
    double weight_dynamic_obstacle; //!< Optimization weight for satisfying a
    //! minimum separation from dynamic obstacles
    double weight_viapoint; //!< Optimization weight for minimizing the distance
                            //! to via-points
    double weight_human_viapoint; //!< Optimization weight for minimizing the
                                  //! distance from human to its via-points
    double weight_human_robot_safety; //!< Optimization weight for satisfying a minimum
                               //! seperation between human and robot
    double weight_human_human_safety;
    double weight_human_robot_ttc;
    double weight_human_robot_ttcplus;				//michele
    double weight_human_robot_ttclosest;                       //michele
    double weight_human_robot_dir;
    double weight_human_robot_visibility;
    double human_robot_ttc_scale_alpha;
    double human_robot_ttcplus_scale_alpha;			//michele
    bool use_human_robot_safety_c;
    bool use_human_human_safety_c;
    bool use_human_robot_ttc_c;
    bool use_human_robot_ttcplus_c;				//michele
    bool use_human_robot_ttclosest_c;                        //michele
    bool scale_human_robot_ttc_c;
    bool scale_human_robot_ttcplus_c;					//michele
    bool use_human_robot_dir_c;
    bool use_human_robot_visi_c;
    bool use_human_elastic_vel;
    bool disable_warm_start;
    bool disable_rapid_omega_chage;
    double omega_chage_time_seperation;
  } optim;                     //!< Optimization related parameters

  struct HomotopyClasses {
    bool enable_homotopy_class_planning; //!< Activate homotopy class planning
                                         //!(Requires much more resources that
                                         //! simple planning, since multiple
    //! trajectories are optimized at once).
    bool enable_multithreading; //!< Activate multiple threading for planning
                                //! multiple trajectories in parallel.
    bool simple_exploration; //!< If true, distinctive trajectories are explored
                             //! using a simple left-right approach (pass each
    //! obstacle on the left or right side) for path
    //! generation, otherwise sample possible roadmaps
    //! randomly in a specified region between start and
    //! goal.
    int max_number_classes; //!< Specify the maximum number of allowed
                            //! alternative homotopy classes (limits
    //! computational effort)
    double selection_cost_hysteresis; //!< Specify how much trajectory cost must
                                      //! a new candidate have w.r.t. a
    //! previously selected trajectory in order
    //! to be selected (selection if new_cost <
    //! old_cost*factor).
    double selection_obst_cost_scale; //!< Extra scaling of obstacle cost terms
                                      //! just for selecting the 'best'
    //! candidate.
    double selection_viapoint_cost_scale; //!< Extra scaling of via-point cost
    //! terms just for selecting the 'best'
    //! candidate.
    bool selection_alternative_time_cost; //!< If true, time cost is replaced by
                                          //! the total transition time.

    int roadmap_graph_no_samples; //! < Specify the number of samples generated
                                  //! for creating the roadmap graph, if
                                  //! simple_exploration is turend off.
    double roadmap_graph_area_width; //!< Random keypoints/waypoints are sampled
                                     //! in a rectangular region between start
    //! and goal. Specify the width of that
    //! region in meters.
    double h_signature_prescaler; //!< Scale number of obstacle value in order
                                  //! to allow huge number of obstacles. Do not
    //! choose it extremly low, otherwise obstacles
    //! cannot be distinguished from each other
    //!(0.2<H<=1).
    double h_signature_threshold; //!< Two h-signatures are assumed to be equal,
                                  //! if both the difference of real parts and
    //! complex parts are below the specified
    //! threshold.

    double obstacle_keypoint_offset; //!< If simple_exploration is turned on,
                                     //! this parameter determines the distance
    //! on the left and right side of the
    //! obstacle at which a new keypoint will be
    //! cretead (in addition to
    //! min_obstacle_dist).
    double obstacle_heading_threshold; //!< Specify the value of the normalized
                                       //! scalar product between obstacle
    //! heading and goal heading in order to
    //! take them (obstacles) into account for
    //! exploration [0,1]

    bool viapoints_all_candidates; //!< If true, all trajectories of different
    //! topologies are attached to the current set
    //! of via-points, otherwise only the
    //! trajectory sharing the same one as the
    //! initial/global plan.

    bool visualize_hc_graph; //!< Visualize the graph that is created for
                             //! exploring new homotopy classes.
  } hcp;

  struct Visualization {
    bool publish_robot_global_plan;
    bool publish_robot_local_plan;
    bool publish_robot_local_plan_poses;
    bool publish_robot_local_plan_fp_poses;
    bool publish_human_global_plans;
    bool publish_human_local_plans;
    bool publish_human_local_plan_poses;
    bool publish_human_local_plan_fp_poses;
    double pose_array_z_scale;
  } visualization;

  struct Approach {
    int approach_id;
    double approach_dist;
    double approach_angle;
    double approach_dist_tolerance;
    double approach_angle_tolerance;
  } approach;

  /**
   * @brief Construct the TebConfig using default values.
   * @warning If the \b rosparam server or/and \b dynamic_reconfigure
   *(rqt_reconfigure) node are used,
   *	     the default variables will be overwritten: \n
   *	     E.g. if \e base_local_planner is utilized as plugin for the
   *navigation stack, the initialize() method will register a
   * 	     dynamic_reconfigure server. A subset (not all but most) of the
   *parameters are considered for dynamic modifications.
   * 	     All parameters considered by the dynamic_reconfigure server (and
   *their \b default values) are
   * 	     set in \e PROJECT_SRC/cfg/TebLocalPlannerReconfigure.cfg. \n
   * 	     In addition the rosparam server can be queried to get parameters
   *e.g. defiend in a launch file.
   * 	     The plugin source (or a possible binary source) can call
   *loadRosParamFromNodeHandle() to update the parameters.
   * 	     In \e summary, default parameters are loaded in the following
   *order (the right one overrides the left ones): \n
   * 		<b>TebConfig Constructor defaults << dynamic_reconfigure
   *defaults
   *<< rosparam server defaults</b>
   */
  TebConfig() {

    odom_topic = "odom";
    map_frame = "odom";

    planning_mode = 1; // Human-Aware planning by default

    // Trajectory

    trajectory.teb_autosize = true;
    trajectory.dt_ref = 0.3;
    trajectory.dt_hysteresis = 0.1;
    trajectory.min_samples = 3;
    trajectory.human_min_samples = 3;
    trajectory.global_plan_overwrite_orientation = true;
    trajectory.global_plan_viapoint_sep = -1;
    trajectory.via_points_ordered = false;
    trajectory.max_global_plan_lookahead_dist = 1;
    trajectory.force_reinit_new_goal_dist = 1;
    trajectory.feasibility_check_no_poses = 5;
    trajectory.publish_feedback = false;
    trajectory.shrink_horizon_backup = true;
    trajectory.horizon_reduction_amount = 0.5;
    trajectory.teb_init_skip_dist = 0.4;

    // Robot

    robot.max_vel_x = 0.4;
    robot.min_vel_x = 0.0;
    robot.max_vel_x_backwards = 0.2;
    robot.min_vel_x_backwards = 0.0;
    robot.max_vel_theta = 0.3;
    robot.min_vel_theta = 0.0;
    robot.acc_lim_x = 0.5;
    robot.acc_lim_theta = 0.5;
    robot.min_turning_radius = 0;
    robot.wheelbase = 1.0;
    robot.cmd_angle_instead_rotvel = false;

    // Human
    human.radius = 0.2;
    human.min_human_robot_dist = 0.6;
    human.min_human_human_dist = 0.6;
    human.max_vel_x = 1.1;
    human.nominal_vel_x = 0.8;
    human.max_vel_x_backwards = 0.0;
    human.max_vel_theta = 1.1;
    human.acc_lim_x = 0.6;
    human.acc_lim_theta = 0.8;
    human.use_external_prediction = false;
    human.predict_human_behind_robot = false;
    human.ttc_threshold = 5.0;
    human.ttcplus_threshold = 5.0;
    human.ttclosest_threshold = 0.5;                           //michele
    human.ttcplus_timer = 5;
    human.pose_prediction_reset_time = 2.0;

    // GoalTolerance

    goal_tolerance.xy_goal_tolerance = 0.2;
    goal_tolerance.yaw_goal_tolerance = 0.2;
    goal_tolerance.free_goal_vel = false;

    // Obstacles

    obstacles.min_obstacle_dist = 0.5;
    obstacles.use_nonlinear_obstacle_penalty = true;
    obstacles.obstacle_cost_mult = 1.0;
    obstacles.include_costmap_obstacles = true;
    obstacles.costmap_obstacles_behind_robot_dist = 0.5;
    obstacles.obstacle_poses_affected = 25;
    obstacles.costmap_converter_plugin = "";
    obstacles.costmap_converter_spin_thread = true;
    obstacles.costmap_converter_rate = 5;

    // Optimization

    optim.no_inner_iterations = 8;
    optim.no_outer_iterations = 4;
    optim.optimization_activate = true;
    optim.optimization_verbose = false;
    optim.penalty_epsilon = 0.1;
    optim.time_penalty_epsilon = 0.1;
    optim.cap_optimaltime_penalty = true;
    optim.weight_max_vel_x = 1.0;
    optim.weight_max_human_vel_x = 2.0;
    optim.weight_nominal_human_vel_x = 2.0;
    optim.weight_max_vel_theta = 1.0;
    optim.weight_max_human_vel_theta = 2.0;
    optim.weight_acc_lim_x = 1;
    optim.weight_human_acc_lim_x = 1;
    optim.weight_acc_lim_theta = 1;
    optim.weight_human_acc_lim_theta = 1;
    optim.weight_kinematics_nh = 1000;
    optim.weight_kinematics_forward_drive = 1;
    optim.weight_kinematics_turning_radius = 1;
    optim.weight_optimaltime = 1;
    optim.weight_human_optimaltime = 1;
    optim.weight_obstacle = 10;
    optim.weight_dynamic_obstacle = 10;
    optim.weight_viapoint = 1;
    optim.weight_human_viapoint = 1;
    optim.weight_human_robot_safety = 20;
    optim.weight_human_human_safety = 20;
    optim.weight_human_robot_ttc = 20;
    optim.weight_human_robot_ttcplus = 20;				//michele
    optim.weight_human_robot_ttclosest = 10;                     //michele
    optim.weight_human_robot_dir = 20;
    optim.human_robot_ttcplus_scale_alpha = 1;			//michele
    optim.human_robot_ttc_scale_alpha = 1;
    optim.use_human_robot_safety_c = false;
    optim.use_human_human_safety_c = true;
    optim.use_human_robot_ttc_c = true ;
    optim.use_human_robot_ttcplus_c = false ;			//michele
    optim.use_human_robot_ttclosest_c = true;                   //michele
    optim.scale_human_robot_ttc_c = true;
    optim.scale_human_robot_ttcplus_c = true;
    optim.use_human_robot_dir_c = true;
    optim.use_human_elastic_vel = true;
    optim.disable_warm_start = false;
    optim.disable_rapid_omega_chage = true;
    optim.omega_chage_time_seperation = 1.0;

    // Homotopy Class Planner

    hcp.enable_homotopy_class_planning = true;
    hcp.enable_multithreading = true;
    hcp.simple_exploration = false;
    hcp.max_number_classes = 5;
    hcp.selection_cost_hysteresis = 1.0;
    hcp.selection_obst_cost_scale = 100.0;
    hcp.selection_viapoint_cost_scale = 1.0;
    hcp.selection_alternative_time_cost = false;

    hcp.obstacle_keypoint_offset = 0.1;
    hcp.obstacle_heading_threshold = 0.45;
    hcp.roadmap_graph_no_samples = 15;
    hcp.roadmap_graph_area_width = 6; // [m]
    hcp.h_signature_prescaler = 1;
    hcp.h_signature_threshold = 0.1;

    hcp.viapoints_all_candidates = true;

    hcp.visualize_hc_graph = false;

    // Visualization
    visualization.publish_robot_global_plan = true;
    visualization.publish_robot_local_plan = true;
    visualization.publish_robot_local_plan_poses = false;
    visualization.publish_robot_local_plan_fp_poses = false;
    visualization.publish_human_global_plans = false;
    visualization.publish_human_local_plans = true;
    visualization.publish_human_local_plan_poses = false;
    visualization.publish_human_local_plan_fp_poses = false;
    visualization.pose_array_z_scale = 1.0;

    // approach
    approach.approach_id = 1;
    approach.approach_dist = 0.5;
    approach.approach_angle = 3.14;
    approach.approach_dist_tolerance = 0.2;
    approach.approach_angle_tolerance = 0.3;
  }

  /**
   * @brief Load parmeters from the ros param server.
   * @param nh const reference to the local ros::NodeHandle
   */
  void loadRosParamFromNodeHandle(const ros::NodeHandle &nh);

  /**
   * @brief Reconfigure parameters from the dynamic_reconfigure config.
   * Change parameters dynamically (e.g. with <c>rosrun rqt_reconfigure
   * rqt_reconfigure</c>).
   * A reconfigure server needs to be instantiated that calls this method in
   * it's callback.
   * In case of the plugin \e teb_local_planner default values are defined
   * in \e PROJECT_SRC/cfg/TebLocalPlannerReconfigure.cfg.
   * @param cfg Config class autogenerated by dynamic_reconfigure according to
   * the cfg-file mentioned above.
   */
  void reconfigure(TebLocalPlannerReconfigureConfig &cfg);

  /**
   * @brief Check parameters and print warnings in case of discrepancies
   *
   * Call this method whenever parameters are changed using public interfaces to
   * inform the user
   * about some improper uses.
   */
  void checkParameters() const;

  /**
   * @brief Check if some deprecated parameters are found and print warnings
   * @param nh const reference to the local ros::NodeHandle
   */
  void checkDeprecated(const ros::NodeHandle &nh) const;

  /**
   * @brief Return the internal config mutex
   */
  boost::mutex &configMutex() { return config_mutex_; }

private:
  boost::mutex config_mutex_; //!< Mutex for config accesses and changes
};

} // namespace teb_local_planner

#endif
