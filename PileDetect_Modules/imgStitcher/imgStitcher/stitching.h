//#ifndef __OPENCV_STITCHING_STITCHER_HPP__
//#define __OPENCV_STITCHING_STITCHER_HPP__

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/stitching/warpers.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
//#include "opencv2/stitching/detail/motion_estimators.hpp"
//#include "opencv2/stitching/detail/exposure_compensate.hpp"
//#include "opencv2/stitching/detail/seam_finders.hpp"
#include "opencv2/stitching/detail/camera.hpp"
#include "opencv2/stitching/detail/util.hpp"

#include "SeamFinders.h"
#include "motion_estimators.h"
#include "ExposureCompensator.h"
#include "StringManage.h"
#include "Blenders.h"
#include <direct.h>
#include <io.h>
#include "opencv2/core/ocl.hpp"

#include <fstream>

#ifndef ENABLE_ES
#define ENABLE_ES 0
#endif

#ifndef ENABLE_SEAM
#define ENABLE_SEAM 0
#endif
using namespace cv;


/**
@defgroup stitching Images stitching

This figure illustrates the stitching module pipeline implemented in the Stitcher class. Using that
class it's possible to configure/remove some steps, i.e. adjust the stitching pipeline according to
the particular needs. All building blocks from the pipeline are available in the detail namespace,
one can combine and use them separately.

The implemented stitching pipeline is very similar to the one proposed in @cite BL07 .

![image](StitchingPipeline.jpg)

@{
    @defgroup stitching_match Features Finding and Images Matching
    @defgroup stitching_rotation Rotation Estimation
    @defgroup stitching_autocalib Autocalibration
    @defgroup stitching_warp Images Warping
    @defgroup stitching_seam Seam Estimation
    @defgroup stitching_exposure Exposure Compensation
    @defgroup stitching_blend Image Blenders
@}
  */



//! @addtogroup stitching
//! @{

/** @brief High level image stitcher.

It's possible to use this class without being aware of the entire stitching pipeline. However, to
be able to achieve higher stitching stability and quality of the final images at least being
familiar with the theory is recommended.

@note
   -   A basic example on image stitching can be found at
        opencv_source_code/samples/cpp/stitching.cpp
    -   A detailed example on image stitching can be found at
        opencv_source_code/samples/cpp/stitching_detailed.cpp
 */

namespace st {


	class CV_EXPORTS_W Stitcher
	{
	public:
		enum { ORIG_RESOL = -1 };
		enum Status
		{
			OK = 0,
			ERR_NEED_MORE_IMGS = 1,
			ERR_HOMOGRAPHY_EST_FAIL = 2,
			ERR_CAMERA_PARAMS_ADJUST_FAIL = 3,
			ERR_CREATE_FAIL=4,
			ERR_ExposureCompensator_FAIL=5,
			ERR_SAVEPARMS_FAIL=6
		};

		enum Stage
		{
			ALL =0,
			ESTIMATE_PARMS = 1,
			COMPOSE_PANORAMA=2
		};

		// Stitcher() {}
		 /** @brief Creates a stitcher with the default parameters.

		 @param try_use_gpu Flag indicating whether GPU should be used whenever it's possible.
		 @return Stitcher class instance.
		  */

		void setOutputFileName(const char *dst_name);
		static Stitcher createDefault();

		//CV_WRAP double registrationResol() const { return registr_resol_; }
		CV_WRAP void setRegistrationResol(double resol_mpx) { registr_resol_ = resol_mpx; }

		//CV_WRAP double seamEstimationResol() const { return seam_est_resol_; }
		CV_WRAP void setSeamEstimationResol(double resol_mpx) { seam_est_resol_ = resol_mpx; }

		CV_WRAP double compositingResol() const { return compose_resol_; }
		CV_WRAP void setCompositingResol(double resol_mpx) { compose_resol_ = resol_mpx; }

		CV_WRAP double panoConfidenceThresh() const { return conf_thresh_; }
		CV_WRAP void setPanoConfidenceThresh(double conf_thresh) { conf_thresh_ = conf_thresh; }

		CV_WRAP bool waveCorrection() const { return do_wave_correct_; }
		CV_WRAP void setWaveCorrection(bool flag) { do_wave_correct_ = flag; }

		st::WaveCorrectKind waveCorrectKind() const { return wave_correct_kind_; }
		void setWaveCorrectKind(st::WaveCorrectKind kind) { wave_correct_kind_ = kind; }

		Ptr<detail::FeaturesFinder> featuresFinder() { return features_finder_; }
		const Ptr<detail::FeaturesFinder> featuresFinder() const { return features_finder_; }
		void setFeaturesFinder(Ptr<detail::FeaturesFinder> features_finder)
		{
			features_finder_ = features_finder;
		}

		Ptr<detail::FeaturesMatcher> featuresMatcher() { return features_matcher_; }
		const Ptr<detail::FeaturesMatcher> featuresMatcher() const { return features_matcher_; }
		void setFeaturesMatcher(Ptr<detail::FeaturesMatcher> features_matcher)
		{
			features_matcher_ = features_matcher;
		}

		const cv::UMat& matchingMask() const { return matching_mask_; }
		void setMatchingMask(const cv::UMat &mask)
		{
			CV_Assert(mask.type() == CV_8U && mask.cols == mask.rows);
			matching_mask_ = mask.clone();
		}

		Ptr<st::BundleAdjusterBase> bundleAdjuster() { return bundle_adjuster_; }
		const Ptr<st::BundleAdjusterBase> bundleAdjuster() const { return bundle_adjuster_; }
		void setBundleAdjuster(Ptr<st::BundleAdjusterBase> bundle_adjuster)
		{
			bundle_adjuster_ = bundle_adjuster;
		}

		Ptr<WarperCreator> warper() { return warper_; }
		const Ptr<WarperCreator> warper() const { return warper_; }
		void setWarper(Ptr<WarperCreator> creator) { warper_ = creator; }

		Ptr<st::ExposureCompensator> exposureCompensator() { return exposure_comp_; }
		const Ptr<st::ExposureCompensator> exposureCompensator() const { return exposure_comp_; }
		void setExposureCompensator(Ptr<st::ExposureCompensator> exposure_comp)
		{
			exposure_comp_ = exposure_comp;
		}

		Ptr<st::SeamFinder> seamFinder() { return seam_finder_; }
		const Ptr<st::SeamFinder> seamFinder() const { return seam_finder_; }
		void setSeamFinder(Ptr<st::SeamFinder> seam_finder) { seam_finder_ = seam_finder; }

		Ptr<st::Blenders> blender() { return blender_; }
		const Ptr<st::Blenders> blender() const { return blender_; }
		void setBlender(Ptr<st::Blenders> b) { blender_ = b; }

		/** @overload */
		CV_WRAP Status estimateTransform();
		/** @brief These functions try to match the given images and to estimate rotations of each camera.

		@note Use the functions only if you're aware of the stitching pipeline, otherwise use
		Stitcher::stitch.
		@param images Input images.
		@return Status code.
		 */


		 /** @overload */

		/** @brief These functions try to compose the given images (or images stored internally from the other function
		calls) into the final pano under the assumption that the image transformations were estimated
		before.

		@note Use the functions only if you're aware of the stitching pipeline, otherwise use
		Stitcher::stitch.

		@param images Input images.
		@param pano Final pano.
		@return Status code.
		 */
		Status composePanorama();


		//CV_WRAP Status stitch(InputArrayOfArrays images, OutputArray pano);
		CV_WRAP Status stitch(std::vector<std::string>& name_imgs , Stage flag = ALL);
		/** @brief These functions try to stitch the given images.

		@param images Input images.
		@param pano Final pano.
		@return Status code.
		 */


		std::vector<int> component() const { return indices_; }
		std::vector<detail::CameraParams> cameras() const { return cameras_; }
		CV_WRAP double workScale() const { return work_scale_; }

	private:
		//Stitcher() {}

		Status matchImages();
		Status estimateCameraParams();

		double registr_resol_;
		double seam_est_resol_;
		double compose_resol_;
		double conf_thresh_;
		Ptr<detail::FeaturesFinder> features_finder_;
		Ptr<detail::FeaturesMatcher> features_matcher_;
		cv::UMat matching_mask_;
		Ptr<st::BundleAdjusterBase> bundle_adjuster_;
		bool do_wave_correct_;
		st::WaveCorrectKind wave_correct_kind_;
		Ptr<WarperCreator> warper_;
		Ptr<st::ExposureCompensator> exposure_comp_;
		Ptr<st::SeamFinder> seam_finder_;
		Ptr<st::Blenders> blender_;


		std::vector<std::vector<cv::Rect> > rois_;
		cv::Size origianl_img_size;

		std::vector<detail::ImageFeatures> features_;
		std::vector<detail::MatchesInfo> pairwise_matches_;

		std::vector<int> indices_;
		std::vector<detail::CameraParams> cameras_;
		std::vector<std::string> name_imgs_;
		std::vector<std::string> name_seam_est_imgs_;

		double work_scale_;
		double seam_scale_;
		double seam_work_aspect_;
		double warped_image_scale_;

		int num_imgs;
		std::string pathCache="./tmp/";//缓存目录
		std::string pathProject="./";//工程目录
		std::string exifFileName="";//exif文件名

		std::string parms_file = "parms";


	private:
		void releaseVectorUMat(std::vector<UMat> &mat);

		void composePanoramaSingle(int img_idx, double compose_scale, Ptr<detail::RotationWarper> &w,
			std::vector<UMat> &masks_warped, std::vector<Point> &corners);

		Stitcher::Status saveCameraParms();
		Stitcher::Status readCameraParms();
			
	public:
		
		void setPathProject(std::string path) { pathProject = path; pathCache = path + "/tmp/"; }
		void setExifFileName(std::string name) { exifFileName = name; }

	};



	//! @} stitching

}
//
//#endif // __OPENCV_STITCHING_STITCHER_HPP__
