#pragma once

/**\file localization.h
 * \brief Description...
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// std includes
#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <utility>

// ROS includes
#include <ros/ros.h>
#include <angles/angles.h>
#include <dynamic_reconfigure/server.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <nav_msgs/OccupancyGrid.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Quaternion.h>

// PCL includes
#include <pcl/pcl_macros.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/transforms.h>
#include <pcl/filters/filter.h>
#include <pcl/io/pcd_io.h>
#include <pcl/search/kdtree.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl_conversions/pcl_conversions.h>

// external libs includes
#include <Eigen/Core>

// project includes
#include <dynamic_robot_localization/common/configurable_object.h>
#include <dynamic_robot_localization/common/impl/math_utils.hpp>
#include <dynamic_robot_localization/common/pointcloud_conversions.h>
#include <dynamic_robot_localization/common/pointcloud_utils.h>
#include <dynamic_robot_localization/common/transformation_aligner.h>
#include <pose_to_tf_publisher/pose_to_tf_publisher.h>
#include <laserscan_to_pointcloud/tf_collector.h>
#include <laserscan_to_pointcloud/tf_rosmsg_eigen_conversions.h>

#include <dynamic_robot_localization/cloud_filters/cloud_filter.h>
#include <dynamic_robot_localization/cloud_filters/voxel_grid.h>
#include <dynamic_robot_localization/cloud_filters/approximate_voxel_grid.h>
#include <dynamic_robot_localization/cloud_filters/pass_through.h>
#include <dynamic_robot_localization/cloud_filters/radius_outlier_removal.h>
#include <dynamic_robot_localization/cloud_filters/crop_box.h>
#include <dynamic_robot_localization/cloud_filters/random_sample.h>
#include <dynamic_robot_localization/cloud_filters/statistical_outlier_removal.h>
#include <dynamic_robot_localization/cloud_filters/covariance_sampling.h>
#include <dynamic_robot_localization/cloud_filters/scale.h>
#include <dynamic_robot_localization/cloud_filters/plane_segmentation.h>
#include <dynamic_robot_localization/cloud_filters/euclidean_clustering.h>
#include <dynamic_robot_localization/cloud_filters/region_growing.h>
#include <dynamic_robot_localization/cloud_filters/hsv_segmentation.h>

#include <dynamic_robot_localization/curvature_estimators/curvature_estimator.h>
#include <dynamic_robot_localization/curvature_estimators/principal_curvatures_estimation.h>
#include <dynamic_robot_localization/normal_estimators/normal_estimator.h>
#include <dynamic_robot_localization/normal_estimators/normal_estimator_sac.h>
#include <dynamic_robot_localization/normal_estimators/normal_estimation_omp.h>
#include <dynamic_robot_localization/normal_estimators/moving_least_squares.h>

#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_detectors/keypoint_detector.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_detectors/intrinsic_shape_signature_3d.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_detectors/sift_3d.h>

#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_descriptors/keypoint_descriptor.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_descriptors/pfh.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_descriptors/fpfh.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_descriptors/shot.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_descriptors/shape_context_3d.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_descriptors/unique_shape_context.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/keypoint_descriptors/esf.h>

#include <dynamic_robot_localization/cloud_matchers/cloud_matcher.h>
#include <dynamic_robot_localization/cloud_matchers/point_matchers/iterative_closest_point.h>
#include <dynamic_robot_localization/cloud_matchers/point_matchers/iterative_closest_point_2d.h>
#include <dynamic_robot_localization/cloud_matchers/point_matchers/iterative_closest_point_non_linear.h>
#include <dynamic_robot_localization/cloud_matchers/point_matchers/iterative_closest_point_with_normals.h>
#include <dynamic_robot_localization/cloud_matchers/point_matchers/iterative_closest_point_generalized.h>
#include <dynamic_robot_localization/cloud_matchers/point_matchers/normal_distributions_transform_2d.h>
#include <dynamic_robot_localization/cloud_matchers/point_matchers/normal_distributions_transform_3d.h>
#include <dynamic_robot_localization/cloud_matchers/point_matchers/principal_component_analysis.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/sample_consensus_initial_alignment.h>
#include <dynamic_robot_localization/cloud_matchers/feature_matchers/sample_consensus_initial_alignment_prerejective.h>

#include <dynamic_robot_localization/transformation_validators/transformation_validator.h>
#include <dynamic_robot_localization/transformation_validators/euclidean_transformation_validator.h>

#include <dynamic_robot_localization/outlier_detectors/outlier_detector.h>
#include <dynamic_robot_localization/outlier_detectors/euclidean_outlier_detector.h>

#include <dynamic_robot_localization/cloud_analyzers/cloud_analyzer.h>
#include <dynamic_robot_localization/cloud_analyzers/angular_distribution_analyzer.h>

#include <dynamic_robot_localization/registration_covariance_estimators/registration_covariance_estimator.h>
#include <dynamic_robot_localization/registration_covariance_estimators/registration_covariance_point_to_point_3d.h>
#include <dynamic_robot_localization/registration_covariance_estimators/registration_covariance_point_to_plane_3d.h>
#include <dynamic_robot_localization/registration_covariance_estimators/registration_covariance_point_to_point_pm_3d.h>
#include <dynamic_robot_localization/registration_covariance_estimators/registration_covariance_point_to_plane_pm_3d.h>

#include <dynamic_robot_localization/common/circular_buffer_pointcloud.h>
#include <dynamic_robot_localization/common/performance_timer.h>

// project msgs
#include <dynamic_robot_localization/LocalizationDetailed.h>
#include <dynamic_robot_localization/LocalizationDiagnostics.h>
#include <dynamic_robot_localization/LocalizationTimes.h>
#include <dynamic_robot_localization/LocalizationConfiguration.h>
#include <dynamic_robot_localization/ReloadLocalizationConfiguration.h>
#include <dynamic_robot_localization/StartProcessingSensorData.h>
#include <dynamic_robot_localization/StopProcessingSensorData.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

namespace dynamic_robot_localization {
// ##############################################################################   localization   #############################################################################
/**
 * \brief Description...
 */
template <typename PointT = pcl::PointNormal>
class Localization : public ConfigurableObject {
	// ========================================================================   <public-section>   ===========================================================================
	public:
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <usings>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		using Ptr = std::shared_ptr< Localization<PointT> >;
		using ConstPtr = std::shared_ptr< const Localization<PointT> >;
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </usings>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <enums>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		enum MapUpdateMode {
			NoIntegration, 			// performs localization only (map can still be updated externally by OctoMap using the map update topic)
			FullIntegration, 		// the full registered cloud is integrated in the reference map
			InliersIntegration, 	// the inliers of the registered cloud are integrated in the reference map
			OutliersIntegration 	// the outliers of the registered cloud are integrated in the reference map
		};

		enum SensorDataProcessingStatus {
			ExceptionRaised,
			FailedInitialPoseEstimation,
			FailedNormalEstimation,
			FailedPoseEstimation,
			FailedTransformationAligner,
			FailedTFTransform,
			FillingCircularBufferWithMsgsFromAllTopics,
			FirstPointCloudInSlamMode,
			MinimumElapsedTimeSinceLastPointCloudNotReached,
			MissingReferencePointCloud,
			PointCloudAgeHigherThanMaximum,
			PointCloudDiscarded,
			ReachedLimitOfNumberOfPointCloudsToProcess,
			PointCloudFilteringFailed,
			PointCloudOlderThanLastPointCloudReceived,
			PointCloudSubscribersDisabled,
			PointCloudWithoutTheMinimumNumberOfRequiredPoints,
			PoseEstimationRejectedByTransformationValidators,
			SuccessfulPreprocessing,
			SuccessfulPoseEstimation,
			WaitingForSensorData
		};

		static std::string s_sensorDataProcessingStatusToStr(const SensorDataProcessingStatus& status) {
			switch (status) {
				case ExceptionRaised: return "ExceptionRaised";
				case FailedInitialPoseEstimation: return "FailedInitialPoseEstimation";
				case FailedNormalEstimation: return "FailedNormalEstimation";
				case FailedPoseEstimation: return "FailedPoseEstimation";
				case FailedTransformationAligner: return "FailedTransformationAligner";
				case FailedTFTransform: return "FailedTFTransform";
				case FillingCircularBufferWithMsgsFromAllTopics: return "FillingCircularBufferWithMsgsFromAllTopics";
				case FirstPointCloudInSlamMode: return "FirstPointCloudInSlamMode";
				case MinimumElapsedTimeSinceLastPointCloudNotReached: return "MinimumElapsedTimeSinceLastPointCloudNotReached";
				case MissingReferencePointCloud: return "MissingReferencePointCloud";
				case PointCloudAgeHigherThanMaximum: return "PointCloudAgeHigherThanMaximum";
				case PointCloudDiscarded: return "PointCloudDiscarded";
				case ReachedLimitOfNumberOfPointCloudsToProcess: return "ReachedLimitOfNumberOfPointCloudsToProcess";
				case PointCloudFilteringFailed: return "PointCloudFilteringFailed";
				case PointCloudOlderThanLastPointCloudReceived: return "PointCloudOlderThanLastPointCloudReceived";
				case PointCloudSubscribersDisabled: return "PointCloudSubscribersDisabled";
				case PointCloudWithoutTheMinimumNumberOfRequiredPoints: return "PointCloudWithoutTheMinimumNumberOfRequiredPoints";
				case PoseEstimationRejectedByTransformationValidators: return "PoseEstimationRejectedByTransformationValidators";
				case SuccessfulPreprocessing: return "SuccessfulPreprocessing";
				case SuccessfulPoseEstimation: return "SuccessfulPoseEstimation";
				case WaitingForSensorData: return "WaitingForSensorData";
			}
			return "";
		}
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </enums>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constants>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constants>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		Localization();
		virtual ~Localization();
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <Localization-functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		virtual void setupConfigurationFromParameterServer(ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle, const std::string& configuration_namespace);
		virtual bool reloadConfigurationFromParameterServerServiceCallback(dynamic_robot_localization::ReloadLocalizationConfiguration::Request& request, dynamic_robot_localization::ReloadLocalizationConfiguration::Response& response);
		virtual bool startProcessingSensorDataServiceCallback(dynamic_robot_localization::StartProcessingSensorData::Request& request, dynamic_robot_localization::StartProcessingSensorData::Response& response);
		virtual bool stopProcessingSensorDataServiceCallback(dynamic_robot_localization::StopProcessingSensorData::Request& request, dynamic_robot_localization::StopProcessingSensorData::Response& response);
		virtual bool reloadConfigurationFromParameterServer(const dynamic_robot_localization::LocalizationConfiguration& localization_configuration);
		virtual void setupGeneralConfigurationsFromParameterServer(const std::string& configuration_namespace);
		virtual void setupSubscribeTopicNamesFromParameterServer(const std::string &configuration_namespace);
		virtual void setupServiceServersNamesFromParameterServer(const std::string &configuration_namespace);
		virtual void setupPublishTopicNamesFromParameterServer(const std::string& configuration_namespace);
		virtual void setupFrameIdsFromParameterServer(const std::string& configuration_namespace);
		virtual void setupInitialPoseFromParameterServer(bool update_last_accepted_pose_time = false);
		virtual void setupInitialPoseFromParameterServer(const std::string& configuration_namespace, const ros::Time& time, bool use_latest_tf_time = false, bool update_last_accepted_pose_time = false);
		virtual void setupTFPublisherFromParameterServer(const std::string& configuration_namespace);
		virtual void setupMessageManagementFromParameterServer(const std::string& configuration_namespace);
		virtual void setupReferencePointCloudFromParameterServer(const std::string& configuration_namespace);

		virtual void setupCloudFiltersFromParameterServer(const std::string& configuration_namespace);
		virtual void setupCloudFiltersFromParameterServer(std::vector< typename CloudFilter<PointT>::Ptr >& filters_container, const std::string& configuration_namespace);
		static void s_setupCloudFiltersFromParameterServer(std::vector< typename CloudFilter<PointT>::Ptr >& filters_container, const std::string& configuration_namespace,
														   laserscan_to_pointcloud::TFCollector& tf_collector, ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void setupNormalEstimatorsFromParameterServer(const std::string& configuration_namespace);
		virtual void setupNormalEstimatorFromParameterServer(typename NormalEstimator<PointT>::Ptr& normal_estimator, const std::string& configuration_namespace);
		static void s_setupNormalEstimatorFromParameterServer(typename NormalEstimator<PointT>::Ptr& normal_estimator, const std::string& configuration_namespace,
															  ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void updateNormalsEstimatorsFlags();
		virtual void setupCurvatureEstimatorsFromParameterServer(const std::string& configuration_namespace);
		virtual void setupCurvatureEstimatorFromParameterServer(typename CurvatureEstimator<PointT>::Ptr& curvature_estimator, const std::string& configuration_namespace);
		static void s_setupCurvatureEstimatorFromParameterServer(typename CurvatureEstimator<PointT>::Ptr& curvature_estimator, const std::string& configuration_namespace,
																 ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void setupKeypointDetectorsFromParameterServer(const std::string& configuration_namespace);
		virtual void setupKeypointDetectorsFromParameterServer(std::vector<typename KeypointDetector<PointT>::Ptr >& keypoint_detectors, const std::string& configuration_namespace);
		static void s_setupKeypointDetectorsFromParameterServer(std::vector<typename KeypointDetector<PointT>::Ptr >& keypoint_detectors, const std::string& configuration_namespace,
																ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void setupCloudMatchersFromParameterServer(const std::string& configuration_namespace);
		virtual void setupInitialPoseEstimatorsFeatureMatchersFromParameterServer(const std::string& configuration_namespace);
		virtual void setupInitialPoseEstimatorsPointMatchersFromParameterServer(const std::string& configuration_namespace);
		virtual void setupTrackingMatchersFromParameterServer(const std::string& configuration_namespace);
		virtual void setupTrackingRecoveryMatchersFromParameterServer(const std::string& configuration_namespace);
		virtual void setupCloudMatchersFromParameterServer(std::vector< typename CloudMatcher<PointT>::Ptr >& pointcloud_matchers, const std::string& configuration_namespace);
		static void s_setupCloudMatchersFromParameterServer(std::vector< typename CloudMatcher<PointT>::Ptr >& pointcloud_matchers, const std::string& configuration_namespace,
															ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void setupFeatureCloudMatchersFromParameterServer(std::vector< typename CloudMatcher<PointT>::Ptr >& feature_cloud_matchers, const std::string& configuration_namespace);
		static void s_setupFeatureCloudMatchersFromParameterServer(std::vector< typename CloudMatcher<PointT>::Ptr >& feature_cloud_matchers, const std::string& configuration_namespace,
																   ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		template <typename DescriptorT>
		void setupKeypointMatcherFromParameterServer(std::vector< typename CloudMatcher<PointT>::Ptr >& feature_cloud_matchers, typename KeypointDescriptor<PointT, DescriptorT>::Ptr& keypoint_descriptor,
													 const std::string& keypoint_descriptor_configuration_namespace, const std::string& feature_matcher_configuration_namespace);
		template <typename DescriptorT>
		static void s_setupKeypointMatcherFromParameterServer(std::vector< typename CloudMatcher<PointT>::Ptr >& feature_cloud_matchers, typename KeypointDescriptor<PointT, DescriptorT>::Ptr& keypoint_descriptor,
															  const std::string& keypoint_descriptor_configuration_namespace, const std::string& feature_matcher_configuration_namespace,
															  ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void setupTransformationValidatorsForInitialAlignmentFromParameterServer(const std::string& configuration_namespace);
		virtual void setupTransformationValidatorsForTrackingFromParameterServer(const std::string& configuration_namespace);
		virtual void setupTransformationValidatorsForTrackingRecoveryFromParameterServer(const std::string& configuration_namespace);
		virtual void setupTransformationValidatorsFromParameterServer(std::vector< TransformationValidator::Ptr >& validators, const std::string& configuration_namespace);
		static void s_setupTransformationValidatorsFromParameterServer(std::vector< TransformationValidator::Ptr >& validators, const std::string& configuration_namespace,
																	   ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void setupOutlierDetectorsFromParameterServer(const std::string& configuration_namespace);
		virtual void setupOutlierDetectorsReferencePointCloudFromParameterServer(const std::string& configuration_namespace);
		virtual void setupOutlierDetectorsFromParameterServer(std::vector< typename OutlierDetector<PointT>::Ptr >& outlier_detectors, const std::string& configuration_namespace_detectors, const std::string& topics_configuration_prefix);
		static void s_setupOutlierDetectorsFromParameterServer(std::vector< typename OutlierDetector<PointT>::Ptr >& outlier_detectors, const std::string& configuration_namespace_detectors, const std::string& topics_configuration_prefix,
															   ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void setupCloudAnalyzersFromParameterServer(const std::string& configuration_namespace);
		static void s_setupCloudAnalyzersFromParameterServer(typename CloudAnalyzer<PointT>::Ptr& cloud_analyzer, const std::string& configuration_namespace,
															 ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void setupRegistrationCovarianceEstimatorsFromParameterServer(const std::string& configuration_namespace);
		static void s_setupRegistrationCovarianceEstimatorsFromParameterServer(typename RegistrationCovarianceEstimator<PointT>::Ptr& registration_covariance_estimator, const std::string& configuration_namespace,
																			   ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);
		virtual void setupTransformationAlignerFromParameterServer(const std::string &configuration_namespace);
		static void s_setupTransformationAlignerFromParameterServer(TransformationAligner::Ptr& transformation_aligner, const std::string &configuration_namespace,
																	ros::NodeHandlePtr& node_handle, ros::NodeHandlePtr& private_node_handle);

		bool clearReferencePointCloud();
		virtual bool loadReferencePointCloud();
		virtual bool loadReferencePointCloudFromFile(const std::string& reference_pointcloud_filename, const std::string& reference_pointclouds_database_folder_path = std::string(""));
		virtual void loadReferencePointCloudFromROSPointCloud(const sensor_msgs::PointCloud2ConstPtr& reference_pointcloud_msg);
		virtual void loadReferencePointCloudFromROSOccupancyGrid(const nav_msgs::OccupancyGridConstPtr& occupancy_grid_msg);
		virtual void publishReferencePointCloud(const ros::Time& time_stamp, bool update_msg = true);
		virtual bool updateLocalizationPipelineWithNewReferenceCloud(const ros::Time& time_stamp);
		virtual void updateMatchersReferenceCloud();

		virtual bool setInitialPose(const geometry_msgs::Pose& pose, const std::string& frame_id, const ros::Time& pose_time);
		virtual void setInitialPoseFromPose(const geometry_msgs::Pose& pose);
		virtual void setInitialPoseFromPose(const geometry_msgs::PoseConstPtr& pose);
		virtual void setInitialPoseFromPoseStamped(const geometry_msgs::PoseStampedConstPtr& pose);
		virtual void setInitialPoseFromPoseWithCovarianceStamped(const geometry_msgs::PoseWithCovarianceStampedConstPtr& pose);

		virtual void startPublishers();
		virtual void startReferenceCloudSubscribers();
		virtual void startSubscribers();
		virtual void startServiceServers();
		virtual void startLocalization(bool start_ros_spinner = true);
		virtual void startROSSpinner();
		virtual void stopProcessingSensorData();
		virtual void restartProcessingSensorData();
		virtual void resetNumberOfProcessedPointclouds();

		virtual bool transformCloudToTFFrame(typename pcl::PointCloud<PointT>::Ptr& ambient_pointcloud, const ros::Time& timestamp, const std::string& target_frame_id);
		virtual bool checkIfAmbientPointCloudShouldBeProcessed(const ros::Time& ambient_cloud_time, size_t number_of_points, bool check_if_pointcloud_subscribers_are_active = true, bool use_ros_console = true);
		virtual bool checkIfTrackingIsLost();
		virtual void processAmbientPointCloud(const sensor_msgs::PointCloud2ConstPtr& ambient_cloud_msg);
		virtual bool processAmbientPointCloud(typename pcl::PointCloud<PointT>::Ptr& ambient_pointcloud, bool check_if_pointcloud_should_be_processed = true, bool check_if_pointcloud_subscribers_are_active = true);
		virtual void resetPointCloudHeight(pcl::PointCloud<PointT>& pointcloud, float height = 0.0f);


		virtual bool applyCloudFilters(std::vector< typename CloudFilter<PointT>::Ptr >& cloud_filters, typename pcl::PointCloud<PointT>::Ptr& pointcloud);
		static bool s_applyCloudFilters(std::vector< typename CloudFilter<PointT>::Ptr >& cloud_filters, typename pcl::PointCloud<PointT>::Ptr& pointcloud, int minimum_number_of_points_in_ambient_pointcloud);

		virtual bool applyNormalEstimator(typename NormalEstimator<PointT>::Ptr& normal_estimator, typename CurvatureEstimator<PointT>::Ptr& curvature_estimator,
										  typename pcl::PointCloud<PointT>::Ptr& pointcloud,
										  typename pcl::PointCloud<PointT>::Ptr& surface,
										  typename pcl::search::KdTree<PointT>::Ptr& pointcloud_search_method, bool pointcloud_is_map = false);
		static bool s_applyNormalEstimator(typename NormalEstimator<PointT>::Ptr& normal_estimator, typename CurvatureEstimator<PointT>::Ptr& curvature_estimator,
										   typename pcl::PointCloud<PointT>::Ptr& pointcloud,
										   typename pcl::PointCloud<PointT>::Ptr& surface,
										   typename pcl::search::KdTree<PointT>::Ptr& pointcloud_search_method,
										   tf2::Transform& sensor_pose_tf_guess, int minimum_number_of_points_in_ambient_pointcloud);

		virtual bool applyKeypointDetectors(std::vector< typename KeypointDetector<PointT>::Ptr >& keypoint_detectors, typename pcl::PointCloud<PointT>::Ptr& pointcloud,
											typename pcl::search::KdTree<PointT>::Ptr& surface_search_method,
											typename pcl::PointCloud<PointT>::Ptr& keypoints);
		static bool s_applyKeypointDetectors(std::vector< typename KeypointDetector<PointT>::Ptr >& keypoint_detectors, typename pcl::PointCloud<PointT>::Ptr& pointcloud,
											 typename pcl::search::KdTree<PointT>::Ptr& surface_search_method,
											 typename pcl::PointCloud<PointT>::Ptr& keypoints);

		virtual bool applyCloudMatchers(std::vector< typename CloudMatcher<PointT>::Ptr >& matchers, typename pcl::PointCloud<PointT>::Ptr& ambient_pointcloud,
										typename pcl::search::KdTree<PointT>::Ptr& surface_search_method,
										typename pcl::PointCloud<PointT>::Ptr& pointcloud_keypoints,
										tf2::Transform& pointcloud_pose_in_out);
		static bool s_applyCloudMatchers(std::vector< typename CloudMatcher<PointT>::Ptr >& matchers, typename pcl::PointCloud<PointT>::Ptr& ambient_pointcloud,
										 typename pcl::search::KdTree<PointT>::Ptr& surface_search_method,
										 typename pcl::PointCloud<PointT>::Ptr& pointcloud_keypoints,
										 tf2::Transform& pointcloud_pose_in_out,
										 int minimum_number_of_points_in_ambient_pointcloud, std::vector< tf2::Transform >& accepted_pose_corrections, int& number_of_registration_iterations_for_all_matchers,
										 double& correspondence_estimation_time_for_all_matchers, double& transformation_estimation_time_for_all_matchers, double& transform_cloud_time_for_all_matchers, double& cloud_align_time_for_all_matchers,
										 std::string& last_matcher_convergence_state, double& root_mean_square_error_of_last_registration_correspondences, int& number_correspondences_last_registration_algorithm);

		virtual bool applyTransformationAligner(const tf2::Transform& pointcloud_pose_initial_guess, const tf2::Transform& pointcloud_pose_corrected, tf2::Transform& new_pose_corrections_out, const ros::Time& pointcloud_time);
		static bool s_applyTransformationAligner(const tf2::Transform& pointcloud_pose_initial_guess, const tf2::Transform& pointcloud_pose_corrected, tf2::Transform& new_pose_corrections_out, const ros::Time& pointcloud_time,
												 bool ignore_height_corrections, TransformationAligner::Ptr& transformation_aligner, const std::string& map_frame_id, const std::string& base_link_frame_id,
												 laserscan_to_pointcloud::TFCollector& tf_collector, const ros::Duration& tf_timeout,
												 SensorDataProcessingStatus& sensor_data_processing_status);

		virtual double applyOutlierDetectors(typename pcl::PointCloud<PointT>::Ptr& pointcloud, typename pcl::search::KdTree<PointT>::Ptr& reference_pointcloud_search_method,
											 std::vector< typename OutlierDetector<PointT>::Ptr >& detectors,
											 std::vector< typename pcl::PointCloud<PointT>::Ptr >& detected_outliers, std::vector< typename pcl::PointCloud<PointT>::Ptr >& detected_inliers,
											 double& root_mean_square_error_inliers, size_t& number_inliers);
		static double s_applyOutlierDetectors(typename pcl::PointCloud<PointT>::Ptr& pointcloud, typename pcl::search::KdTree<PointT>::Ptr& reference_pointcloud_search_method,
											  std::vector< typename OutlierDetector<PointT>::Ptr >& detectors,
											  std::vector< typename pcl::PointCloud<PointT>::Ptr >& detected_outliers, std::vector< typename pcl::PointCloud<PointT>::Ptr >& detected_inliers,
											  const std::string& map_frame_id, bool compute_outliers_angular_distribution, bool compute_inliers_angular_distribution,
											  double& root_mean_square_error_inliers, size_t& number_inliers);
		static bool s_applyPointCloudOutlierDetectors(typename pcl::PointCloud<PointT>::Ptr& ambient_pointcloud,
													  typename pcl::search::KdTree<PointT>::Ptr& reference_pointcloud_search_method, std::vector< typename OutlierDetector<PointT>::Ptr >& outlier_detectors,
													  std::vector< typename pcl::PointCloud<PointT>::Ptr >& detected_outliers, std::vector< typename pcl::PointCloud<PointT>::Ptr >& detected_inliers,
													  typename pcl::PointCloud<PointT>::Ptr& registered_inliers, typename pcl::PointCloud<PointT>::Ptr& registered_outliers,
													  const std::string& map_frame_id, bool compute_outliers_angular_distribution, bool compute_inliers_angular_distribution,
													  double& root_mean_square_error_inliers, size_t& number_inliers, double& outlier_percentage);
		virtual bool applyAmbientPointCloudOutlierDetectors(typename pcl::PointCloud<PointT>::Ptr& ambient_pointcloud);
		virtual bool applyReferencePointCloudOutlierDetectors(typename pcl::PointCloud<PointT>::Ptr& reference_pointcloud, typename pcl::search::KdTree<PointT>::Ptr& ambient_pointcloud_search_method);

		virtual void publishDetectedOutliers();
		virtual void publishDetectedInliers();

		virtual bool applyCloudAnalyzers(const tf2::Transform& estimated_pose);

		virtual bool applyTransformationValidator(std::vector< TransformationValidator::Ptr >& transformation_validators,
												  const tf2::Transform& pointcloud_pose_initial_guess, tf2::Transform& pointcloud_pose_corrected_in_out, double max_outlier_percentage, double max_outlier_percentage_reference_pointcloud);

		virtual void fillPoseCovariance(geometry_msgs::PoseWithCovarianceStamped& pose_corrected_msg, Eigen::MatrixXd& covariance_matrix);

		virtual bool updateLocalizationWithAmbientPointCloud(typename pcl::PointCloud<PointT>::Ptr& ambient_pointcloud, const ros::Time& pointcloud_time,
				const tf2::Transform& pointcloud_pose_initial_guess,
				tf2::Transform& pointcloud_pose_corrected_out, tf2::Transform& pose_corrections_out, typename pcl::PointCloud<PointT>::Ptr ambient_pointcloud_keypoints_out);
		virtual bool updateReferencePointCloudWithAmbientPointCloud(typename pcl::PointCloud<PointT>::Ptr& pointcloud, typename pcl::PointCloud<PointT>::Ptr pointcloud_keypoints);
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </Localization-functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		bool referencePointCloudLoaded() { return reference_pointcloud_loaded_; }
		bool referencePointCloudRequired() { return reference_pointcloud_required_; }
		SensorDataProcessingStatus getSensorDataProcessingStatus() { return sensor_data_processing_status_; }
		const std::vector< tf2::Transform >& getAcceptedPoseCorrections() { return accepted_pose_corrections_; }
		const tf2::Transform& getAcceptedEstimatedPose() { return pose_tf2_transform_corrected_; }
		const MapUpdateMode& getMapUpdateMode() { return map_update_mode_; }
		bool ambientPointcloudIntegrationActive() { return !ambient_pointcloud_integration_filters_.empty() || !ambient_pointcloud_integration_filters_map_frame_.empty(); }
		bool cloudMatchersActive() { return !initial_pose_estimators_feature_matchers_.empty() || !initial_pose_estimators_point_matchers_.empty() || !tracking_matchers_.empty() || !tracking_recovery_matchers_.empty(); }
		std::string getFilteredPointcloudSaveFilename() const { return filtered_pointcloud_save_filename_; }
		std::string getFilteredPointcloudSaveFrameId() const { return filtered_pointcloud_save_frame_id_; }
		ros::Time getLastAcceptedPoseTime() { return last_accepted_pose_time_; }
		std::string getMapFrameId() { return map_frame_id_; }
		std::string getOdomFrameId() { return odom_frame_id_; }
		std::string getBaseLinkFrameId() { return base_link_frame_id_; }
		std::string getSensorFrameId() { return sensor_frame_id_; }
		int getLimitOfPointcloudsToProcess() { return limit_of_pointclouds_to_process_; }
		size_t getNumberOfProcessedPointclouds() { return number_of_processed_pointclouds_; }
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		void setReferencePointCloudRequired(bool reference_pointcloud_required) { reference_pointcloud_required_ = reference_pointcloud_required; }
		void setAmbientPointcloudIntegrationFiltersPreprocessedPointcloudSaveFilename(const std::string& filename) { ambient_pointcloud_integration_filters_preprocessed_pointcloud_save_filename_ = filename; }
		void setFilteredPointcloudSaveFilename(const std::string& filtered_pointcloud_save_filename) { filtered_pointcloud_save_filename_ = filtered_pointcloud_save_filename; }
		void setFilteredPointcloudSaveFrameId(const std::string& filtered_pointcloud_save_frame_id) { filtered_pointcloud_save_frame_id_ = filtered_pointcloud_save_frame_id; }
		void setLimitOfPointcloudsToProcess(int limit_of_pointclouds_to_process) { limit_of_pointclouds_to_process_ = limit_of_pointclouds_to_process; }
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ========================================================================   </public-section>  ===========================================================================

	// ========================================================================   <protected-section>   ========================================================================
	protected:
		// subscription topic names
		std::string pose_topic_;
		std::string pose_stamped_topic_;
		std::string pose_with_covariance_stamped_topic_;
		std::string ambient_pointcloud_topics_;
		std::vector<std::string> ambient_pointcloud_topic_names_;
		bool ambient_pointcloud_topic_disabled_on_startup_;
		std::string reference_pointcloud_topic_;
		std::string reference_costmap_topic_;

		// service servers
		std::string reload_localization_configuration_service_server_name_;
		std::string start_processing_sensor_data_service_server_name_;
		std::string stop_processing_sensor_data_service_server_name_;

		// publish topic names
		std::string reference_pointcloud_publish_topic_;
		std::string reference_pointcloud_keypoints_publish_topic_;
		std::string filtered_pointcloud_publish_topic_;
		std::string aligned_pointcloud_publish_topic_;
		std::string aligned_pointcloud_global_outliers_publish_topic_;
		std::string aligned_pointcloud_global_inliers_publish_topic_;
		std::string reference_pointcloud_global_outliers_publish_topic_;
		std::string reference_pointcloud_global_inliers_publish_topic_;
		std::string pose_stamped_publish_topic_;
		std::string pose_array_publish_topic_;
		std::string pose_with_covariance_stamped_publish_topic_;
		std::string pose_with_covariance_stamped_tracking_reset_publish_topic_;
		std::string localization_detailed_publish_topic_;
		std::string localization_diagnostics_publish_topic_;
		std::string localization_times_publish_topic_;


		// configuration fields
		std::string reference_pointclouds_database_folder_path_;
		std::string reference_pointcloud_filename_;
		std::string reference_pointcloud_preprocessed_save_filename_;
		std::string reference_pointcloud_keypoints_filename_;
		std::string reference_pointcloud_keypoints_save_filename_;
		std::string ambient_pointcloud_integration_filters_preprocessed_pointcloud_save_filename_;
		bool ambient_pointcloud_integration_filters_preprocessed_pointcloud_save_original_pointcloud_;
		std::string filtered_pointcloud_save_filename_;
		std::string filtered_pointcloud_save_frame_id_;
		bool filtered_pointcloud_save_frame_id_with_cloud_time_;
		bool stop_processing_after_saving_filtered_pointcloud_;
		bool reference_pointcloud_normalize_normals_;
		bool ambient_pointcloud_normalize_normals_;
		bool flip_normals_using_occupancy_grid_analysis_;
		MapUpdateMode map_update_mode_;
		bool use_incremental_map_update_;
		std::string map_frame_id_;
		std::string map_frame_id_for_transforming_pointclouds_;
		std::string map_frame_id_for_publishing_pointclouds_;
		std::string odom_frame_id_;
		std::string base_link_frame_id_;
		std::string sensor_frame_id_;
		bool override_pointcloud_timestamp_to_current_time_;
		ros::Duration max_seconds_ambient_pointcloud_age_;
		ros::Duration max_seconds_ambient_pointcloud_offset_to_last_estimated_pose_;
		ros::Duration min_seconds_between_scan_registration_;
		ros::Duration min_seconds_between_reference_pointcloud_update_;
		ros::Duration tf_timeout_;
		ros::Duration pose_tracking_timeout_;
		ros::Duration pose_tracking_recovery_timeout_;
		ros::Duration initial_pose_estimation_timeout_;
		bool remove_points_in_sensor_origin_;
		int minimum_number_of_points_in_ambient_pointcloud_;
		int minimum_number_of_points_in_reference_pointcloud_;
		bool localization_detailed_use_millimeters_in_root_mean_square_error_inliers_;
		bool localization_detailed_use_millimeters_in_root_mean_square_error_of_last_registration_correspondences_;
		bool localization_detailed_use_millimeters_in_translation_corrections_;
		bool localization_detailed_use_degrees_in_rotation_corrections_;
		bool localization_detailed_compute_pose_corrections_from_initial_and_final_pose_tfs_;
		bool save_reference_pointclouds_in_binary_format_;
		bool republish_reference_pointcloud_after_successful_registration_;
		bool publish_tf_map_odom_;
		bool publish_tf_when_resetting_initial_pose_;
		bool add_odometry_displacement_;
		bool use_filtered_cloud_as_normal_estimation_surface_ambient_;
		bool use_filtered_cloud_as_normal_estimation_surface_reference_;
		bool compute_normals_when_tracking_pose_;
		bool compute_normals_when_recovering_pose_tracking_;
		bool compute_normals_when_estimating_initial_pose_;
		bool compute_keypoints_when_tracking_pose_;
		bool compute_keypoints_when_recovering_pose_tracking_;
		bool compute_keypoints_when_estimating_initial_pose_;
		bool compute_inliers_angular_distribution_;
		bool compute_outliers_angular_distribution_;
		double inliers_angular_distribution_;
		double outliers_angular_distribution_;
		double last_pose_weighted_mean_filter_;
		bool use_odom_when_transforming_cloud_to_map_frame_;
		bool use_last_accepted_pose_base_link_to_map_when_transforming_cloud_to_map_frame_;
		bool use_base_link_frame_when_publishing_registration_pose_;
		bool use_base_link_frame_when_publishing_initial_poses_array_;
		bool apply_cloud_registration_inverse_to_initial_poses_array_;
		bool invert_cloud_to_map_transform_;
		bool invert_registration_transformation_;
		bool invert_initial_poses_from_msgs_;
		bool initial_pose_msg_needs_to_be_in_map_frame_;
		bool reset_initial_pose_when_tracking_is_lost_;
		bool publish_global_inliers_and_outliers_pointclouds_only_if_there_is_subscribers_;

		// state fields
		ros::Time last_scan_time_;
		ros::Time last_map_received_time_;
		ros::Time last_accepted_pose_time_;
		ros::Time last_pointcloud_time_;
		bool robot_initial_pose_available_;
		int pose_tracking_minimum_number_of_failed_registrations_since_last_valid_pose_;
		int pose_tracking_maximum_number_of_failed_registrations_since_last_valid_pose_;
		int pose_tracking_recovery_minimum_number_of_failed_registrations_since_last_valid_pose_;
		int pose_tracking_recovery_maximum_number_of_failed_registrations_since_last_valid_pose_;
		int pose_tracking_number_of_failed_registrations_since_last_valid_pose_;
		bool reference_pointcloud_loaded_;
		bool reference_pointcloud_2d_;
		bool reference_pointcloud_available_;
		bool reference_pointcloud_required_;
		bool ignore_height_corrections_;
		bool last_accepted_pose_valid_;
		bool last_accepted_pose_performed_tracking_reset_;
		bool received_external_initial_pose_estimation_; // from rviz / other localization system / operator
		bool use_internal_tracking_;
		Eigen::MatrixXd last_accepted_pose_covariance_;
		tf2::Transform last_accepted_pose_base_link_to_map_;
		tf2::Transform last_accepted_pose_odom_to_map_;
		std::vector< tf2::Transform > accepted_pose_corrections_;
		tf2::Transform pose_tf2_transform_corrected_;
		SensorDataProcessingStatus sensor_data_processing_status_;
		size_t number_of_times_that_the_same_point_cloud_was_processed_;

		// ros communication fields
		pose_to_tf_publisher::PoseToTFPublisher::Ptr pose_to_tf_publisher_;
		ros::NodeHandlePtr node_handle_;
		ros::NodeHandlePtr private_node_handle_;
		std::string configuration_namespace_;
		ros::Subscriber pose_subscriber_;
		ros::Subscriber pose_stamped_subscriber_;
		ros::Subscriber pose_with_covariance_stamped_subscriber_;
		ros::ServiceServer reload_localization_configuration_service_server_;
		ros::ServiceServer start_processing_sensor_data_service_server_;
		ros::ServiceServer stop_processing_sensor_data_service_server_;
		std::vector< ros::Subscriber > ambient_pointcloud_subscribers_;
		bool ambient_pointcloud_subscribers_active_;
		int limit_of_pointclouds_to_process_;
		size_t number_of_processed_pointclouds_;
		ros::Subscriber costmap_subscriber_;
		ros::Subscriber reference_pointcloud_subscriber_;
		ros::Publisher reference_pointcloud_publisher_;
		ros::Publisher reference_pointcloud_keypoints_publisher_;
		ros::Publisher filtered_pointcloud_publisher_;
		ros::Publisher aligned_pointcloud_publisher_;
		ros::Publisher aligned_pointcloud_global_outliers_publisher_;
		ros::Publisher aligned_pointcloud_global_inliers_publisher_;
		ros::Publisher reference_pointcloud_global_outliers_publisher_;
		ros::Publisher reference_pointcloud_global_inliers_publisher_;
		ros::Publisher pose_with_covariance_stamped_publisher_;
		ros::Publisher pose_with_covariance_stamped_tracking_reset_publisher_;
		ros::Publisher pose_stamped_publisher_;
		ros::Publisher pose_array_publisher_;
		ros::Publisher localization_detailed_publisher_;
		ros::Publisher localization_diagnostics_publisher_;
		ros::Publisher localization_times_publisher_;

		// localization fields
		typename pcl::PointCloud<PointT>::Ptr reference_pointcloud_;
		sensor_msgs::PointCloud2Ptr reference_pointcloud_msg_;
		typename pcl::PointCloud<PointT>::Ptr reference_pointcloud_keypoints_;
		sensor_msgs::PointCloud2Ptr reference_pointcloud_keypoints_msg_;
		typename CircularBufferPointCloud<PointT>::Ptr ambient_pointcloud_with_circular_buffer_;
		bool circular_buffer_require_reception_of_pointcloud_msgs_from_all_topics_before_doing_registration_;
		bool circular_buffer_clear_inserted_points_if_registration_fails_;
		int minimum_number_points_ambient_pointcloud_circular_buffer_;
		size_t last_number_points_inserted_in_circular_buffer_;
		std::set<std::string> msg_frame_ids_with_data_in_circular_buffer_;
		typename pcl::search::KdTree<PointT>::Ptr reference_pointcloud_search_method_;
		std::vector< typename CloudFilter<PointT>::Ptr > reference_cloud_filters_;
		std::vector< typename CloudFilter<PointT>::Ptr > ambient_pointcloud_integration_filters_;
		std::vector< typename CloudFilter<PointT>::Ptr > ambient_pointcloud_integration_filters_map_frame_;
		std::vector< typename CloudFilter<PointT>::Ptr > ambient_pointcloud_filters_;
		std::vector< typename CloudFilter<PointT>::Ptr > ambient_pointcloud_filters_for_outlier_detection_;
		std::vector< typename CloudFilter<PointT>::Ptr > ambient_pointcloud_filters_custom_frame_;
		std::string ambient_pointcloud_filters_custom_frame_id_;
		std::vector< typename CloudFilter<PointT>::Ptr > ambient_pointcloud_filters_map_frame_;
		std::vector< typename CloudFilter<PointT>::Ptr > ambient_pointcloud_feature_registration_filters_;
		std::vector< typename CloudFilter<PointT>::Ptr > ambient_pointcloud_map_frame_feature_registration_filters_;
		std::vector< typename CloudFilter<PointT>::Ptr > ambient_pointcloud_filters_after_normal_estimation_;
		typename NormalEstimator<PointT>::Ptr reference_cloud_normal_estimator_;
		typename NormalEstimator<PointT>::Ptr ambient_cloud_normal_estimator_;
		typename CurvatureEstimator<PointT>::Ptr reference_cloud_curvature_estimator_;
		typename CurvatureEstimator<PointT>::Ptr ambient_cloud_curvature_estimator_;
		std::vector< typename KeypointDetector<PointT>::Ptr > reference_cloud_keypoint_detectors_;
		std::vector< typename KeypointDetector<PointT>::Ptr > ambient_cloud_keypoint_detectors_;
		std::vector< typename CloudMatcher<PointT>::Ptr > initial_pose_estimators_feature_matchers_;
		std::vector< typename CloudMatcher<PointT>::Ptr > initial_pose_estimators_point_matchers_;
		std::vector< typename CloudMatcher<PointT>::Ptr > tracking_matchers_;
		std::vector< typename CloudMatcher<PointT>::Ptr > tracking_recovery_matchers_;
		int number_of_registration_iterations_for_all_matchers_;
		double correspondence_estimation_time_for_all_matchers_;
		double transformation_estimation_time_for_all_matchers_;
		double transform_cloud_time_for_all_matchers_;
		double cloud_align_time_for_all_matchers_;
		std::string last_matcher_convergence_state_;
		double root_mean_square_error_of_last_registration_correspondences_;
		int number_correspondences_last_registration_algorithm_;
		std::vector< TransformationValidator::Ptr > transformation_validators_initial_alignment_;
		std::vector< TransformationValidator::Ptr > transformation_validators_;
		std::vector< TransformationValidator::Ptr > transformation_validators_tracking_recovery_;
		std::vector< typename OutlierDetector<PointT>::Ptr > outlier_detectors_;
		std::vector< typename OutlierDetector<PointT>::Ptr > outlier_detectors_reference_pointcloud_;
		typename CloudAnalyzer<PointT>::Ptr cloud_analyzer_;
		typename RegistrationCovarianceEstimator<PointT>::Ptr registration_covariance_estimator_;
		typename pcl::PointCloud<PointT>::Ptr registered_inliers_;
		typename pcl::PointCloud<PointT>::Ptr registered_outliers_;
		typename pcl::PointCloud<PointT>::Ptr registered_inliers_reference_pointcloud_;
		typename pcl::PointCloud<PointT>::Ptr registered_outliers_reference_pointcloud_;
		double outlier_percentage_;
		size_t number_inliers_;
		double root_mean_square_error_inliers_;
		double outlier_percentage_reference_pointcloud_;
		size_t number_inliers_reference_pointcloud_;
		double root_mean_square_error_inliers_reference_pointcloud_;
		std::vector< typename pcl::PointCloud<PointT>::Ptr > detected_outliers_;
		std::vector< typename pcl::PointCloud<PointT>::Ptr > detected_inliers_;
		std::vector< typename pcl::PointCloud<PointT>::Ptr > detected_outliers_reference_pointcloud_;
		std::vector< typename pcl::PointCloud<PointT>::Ptr > detected_inliers_reference_pointcloud_;
		LocalizationDiagnostics localization_diagnostics_msg_;
		LocalizationTimes localization_times_msg_;
		bool publish_filtered_pointcloud_only_if_there_is_subscribers_;
		bool publish_aligned_pointcloud_only_if_there_is_subscribers_;
		TransformationAligner::Ptr transformation_aligner_;
	// ========================================================================   </protected-section>  ========================================================================
};

} /* namespace dynamic_robot_localization */



#ifdef DRL_NO_PRECOMPILE
#include <dynamic_robot_localization/localization/impl/localization.hpp>
#endif
