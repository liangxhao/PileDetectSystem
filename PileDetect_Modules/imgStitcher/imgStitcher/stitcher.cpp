#include "stdafx.h"
#include "ExifReader.h"

#include "stitching.h"
#include "util_log.h"
#include "SelectMatcher.h"



namespace st {
	st::Stitcher Stitcher::createDefault()
	{
		Stitcher stitcher;
		stitcher.setRegistrationResol(0.6);
		stitcher.setSeamEstimationResol(0.1);
		stitcher.setCompositingResol(ORIG_RESOL);
		stitcher.setPanoConfidenceThresh(0.9);
		stitcher.setWaveCorrection(true);
		stitcher.setWaveCorrectKind(st::WAVE_CORRECT_HORIZ);
		stitcher.setFeaturesMatcher(makePtr<detail::BestOf2NearestMatcher>(false));

		//stitcher.setBundleAdjuster(makePtr<detail::BundleAdjusterReproj>());
		stitcher.setBundleAdjuster(makePtr<st::BundleAdjusterRay>());


		{
			//stitcher.setFeaturesFinder(makePtr<detail::SurfFeaturesFinder>());

			stitcher.setFeaturesFinder(makePtr<detail::OrbFeaturesFinder>());
			//球面投影
			//stitcher.setWarper(makePtr<cv::SphericalWarper>());
			stitcher.setWarper(makePtr<cv::PlaneWarper>());
			stitcher.setSeamFinder(makePtr<st::GraphCutSeamFinder>(st::GraphCutSeamFinderBase::COST_COLOR));
		}

		//stitcher.setExposureCompensator(makePtr<detail::BlocksGainCompensator>());
		stitcher.setExposureCompensator(makePtr<st::NoExposureCompensator>());

		stitcher.setBlender(makePtr<st::Blenders>());
		stitcher.work_scale_ = 1;
		stitcher.seam_scale_ = 1;
		stitcher.seam_work_aspect_ = 1;
		stitcher.warped_image_scale_ = 1;
		
		cv::ocl::setUseOpenCL(false);
		return stitcher;
	}

	void Stitcher::setOutputFileName(const char *dst_name)
	{
		blender_->setOutputFileName(dst_name);
		blender_->setChannels(3);
	}


	Stitcher::Status Stitcher::estimateTransform()
	{

		Status status;

		if ((status = matchImages()) != OK)
			return status;

		if ((status = estimateCameraParams()) != OK)
			return status;

		return OK;

	}

	void Stitcher::composePanoramaSingle(int img_idx,double compose_scale, Ptr<detail::RotationWarper> &w, 
		std::vector<UMat> &masks_warped, std::vector<Point> &corners)
	{

		UMat full_img, img;

		UMat img_warped, img_warped_s;
		UMat dilated_mask, seam_mask, mask, mask_warped;



		LOGLN("Compositing image #" << indices_[img_idx] + 1);
#if ENABLE_LOG
		int64 compositing_t = getTickCount();
#endif





		// Read image and resize it if necessary
		full_img = cv::imread(name_imgs_[img_idx]).getUMat(ACCESS_FAST).clone();
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (std::abs(compose_scale - 1) > 1e-1)
		{
			resize(full_img, img, Size(), compose_scale, compose_scale);

		}
		else
			img = full_img;
		full_img.release();

		Size img_size = img.size();

		Mat K;
		cameras_[img_idx].K().convertTo(K, CV_32F);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//#if ENABLE_LOG
//		int64 pt = getTickCount();
//#endif
		// Warp the current image
		w->warp(img, K, cameras_[img_idx].R, INTER_LINEAR, BORDER_CONSTANT, img_warped);

		img.release();

		//LOGLN(" warp the current image: " << ((getTickCount() - pt) / getTickFrequency()) << " sec");
		LOGLN("    warp the current image...");
//#if ENABLE_LOG
//		pt = getTickCount();
//#endif

		// Warp the current image mask
		mask.create(img_size, CV_8U);
		mask.setTo(Scalar::all(255));

		w->warp(mask, K, cameras_[img_idx].R, INTER_NEAREST, BORDER_CONSTANT, mask_warped);
		mask.release();

		//LOGLN(" warp the current image mask: " << ((getTickCount() - pt) / getTickFrequency()) << " sec");

		////////////////////////////////////////////////////////////////////////////////////////////////////////////


		

#if ENABLE_ES
		//#if ENABLE_LOG
		//pt = getTickCount();
		//#endif

		// Compensate exposure
		exposure_comp_->apply((int)img_idx, corners[img_idx], img_warped, mask_warped);
		//LOGLN(" compensate exposure: " << ((getTickCount() - pt) / getTickFrequency()) << " sec");
		LOGLN("    compensate exposure... ");

#endif
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////



//#if ENABLE_LOG
//		pt = getTickCount();
//#endif

		img_warped.convertTo(img_warped_s, CV_16S);
		img_warped.release();

#if ENABLE_SEAM
		// Make sure seam mask has proper size
		dilate(masks_warped[img_idx], dilated_mask, Mat());
		masks_warped[img_idx].release();

		resize(dilated_mask, seam_mask, mask_warped.size());
		dilated_mask.release();

		bitwise_and(seam_mask, mask_warped, mask_warped);
		seam_mask.release();

		//LOGLN(" seam mask: " << ((getTickCount() - pt) / getTickFrequency()) << " sec");
		LOGLN("    seam mask...");

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif


		LOGLN("    feed...");
//#if ENABLE_LOG
//		int64 feed_t = getTickCount();
//#endif
		// Blend the current image
		blender_->feed(img_warped_s, mask_warped, corners[img_idx]);

		img_warped_s.release();
		mask_warped.release();

		//LOGLN(" feed time: " << ((getTickCount() - feed_t) / getTickFrequency()) << " sec");
		LOGLN("Compositing #"<< indices_[img_idx] + 1<<", time: " << ((getTickCount() - compositing_t) / getTickFrequency()) << " sec");
	}





	Stitcher::Status Stitcher::composePanorama()
	{
		LOGLN("Warping images... ");


#if ENABLE_LOG
		int64 t = getTickCount();
#endif


		std::vector<Point> corners(num_imgs);
		std::vector<UMat> masks_warped(num_imgs);
		std::vector<UMat> images_warped(num_imgs);
		std::vector<Size> sizes(num_imgs);

		UMat masks;

		// Prepare image masks
		UMat seam_est_img;

		// Warp images and their masks
		Ptr<detail::RotationWarper> w;
#if ENABLE_SEAM
		w = warper_->create(float(warped_image_scale_ * seam_work_aspect_));
		for (size_t i = 0; i < num_imgs; ++i)
		{
			Mat_<float> K;
			cameras_[i].K().convertTo(K, CV_32F);
			K(0, 0) *= (float)seam_work_aspect_;
			K(0, 2) *= (float)seam_work_aspect_;
			K(1, 1) *= (float)seam_work_aspect_;
			K(1, 2) *= (float)seam_work_aspect_;

			seam_est_img = cv::imread(name_seam_est_imgs_[i]).getUMat(ACCESS_FAST).clone();

			corners[i] = w->warp(seam_est_img, K, cameras_[i].R, INTER_LINEAR, BORDER_CONSTANT, images_warped[i]);

			sizes[i] = images_warped[i].size();

			masks.create(seam_est_img.size(), CV_8U);
			seam_est_img.release();

			masks.setTo(Scalar::all(255));

			w->warp(masks, K, cameras_[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
			//释放内存
			masks.release();
		}
		
		LOGLN("Warping images, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

#endif




		/////////////////////////////////////////////////////////////////////////////////////////////////////
#if ENABLE_ES
		std::vector<string> name_images_warped(num_imgs);

		for (size_t i = 0; i < num_imgs; ++i)
		{
			std::string name = stringGetFileNameNotExt(name_imgs_[i]);
			name_images_warped[i] = pathCache + name + "_images_warped.jpg";

			imwrite(name_images_warped[i], images_warped[i]);
			images_warped[i].release();
		}



		try {
			exposure_comp_->feed(corners, name_images_warped, masks_warped);
		}
		
		catch (...) {
			LOGLN("Exposure Compensator Filed, Maybe images have some problems");
			return ERR_ExposureCompensator_FAIL;
		}
	
		seam_finder_->find(name_images_warped, corners, masks_warped);	
		name_images_warped.clear();
#endif
		/////////////////////////////////////////////////////////////////////////////////////////////////////////



		LOGLN("Compositing...");
#if ENABLE_LOG
		t = getTickCount();
#endif

		

		double compose_work_aspect = 1;
		//bool is_blender_prepared = false;s

		double compose_scale = 1;
		bool is_compose_scale_set = false;
		//bool is_compose_scale_set = true;



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (!is_compose_scale_set)
		{
			if (compose_resol_ > 0)
				compose_scale = std::min(1.0, std::sqrt(compose_resol_ * 1e6 / origianl_img_size.area()));
				//compose_scale = 1.0;
			is_compose_scale_set = true;

			// Compute relative scales
			//compose_seam_aspect = compose_scale / seam_scale_;
			compose_work_aspect = compose_scale / work_scale_;

			// Update warped image scale
			warped_image_scale_ *= static_cast<float>(compose_work_aspect);
			w = warper_->create((float)warped_image_scale_);

			// Update corners and sizes
			for (size_t i = 0; i < num_imgs; ++i)
			{
				// Update intrinsics
				cameras_[i].focal *= compose_work_aspect;
				cameras_[i].ppx *= compose_work_aspect;
				cameras_[i].ppy *= compose_work_aspect;

				// Update corner and size
				Size sz = origianl_img_size;

				if (std::abs(compose_scale - 1) > 1e-1)
				{
					sz.width = cvRound(origianl_img_size.width * compose_scale);
					sz.height = cvRound(origianl_img_size.height * compose_scale);
				}

				Mat K;
				cameras_[i].K().convertTo(K, CV_32F);
				Rect roi = w->warpRoi(sz, K, cameras_[i].R);
				corners[i] = roi.tl();
				sizes[i] = roi.size();
			}
		}

		blender_->prepare(corners, sizes);


		
		for (size_t img_idx = 0; img_idx < num_imgs; ++img_idx)
		{
			try {
				composePanoramaSingle((int)img_idx, compose_scale, w,
					masks_warped, corners);
			}
			catch (...)
			{
				LOGLN("Compositing this image failed" );
			}
			
		}



#if ENABLE_LOG
		int64 blend_t = getTickCount();
#endif


		blender_->blend();
		LOGLN("blend time: " << ((getTickCount() - blend_t) / getTickFrequency()) << " sec");

		LOGLN("Compositing, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");



		return OK;
	}


	Stitcher::Status Stitcher::stitch(std::vector<std::string>& name_imgs, Stage flag)
	{
		
		if (flag == 0)
		{
			this->name_imgs_ = name_imgs;
			num_imgs = (int)name_imgs.size();

			//pathCache = name_imgs[0].substr(0, name_imgs[0].find_last_of('\\')) + "//tmp\\";
			if (_access((char*)pathCache.data(), 0) == -1)
			{
				if (_mkdir((char*)pathCache.data()))
					return ERR_CREATE_FAIL;
			}
			Status status = estimateTransform();

			if (status != OK)
				return status;
			saveCameraParms();

			return composePanorama();
		}

		else if (flag == 1)
		{
			this->name_imgs_ = name_imgs;
			num_imgs = (int)name_imgs.size();

			//pathCache = name_imgs[0].substr(0, name_imgs[0].find_last_of('\\')) + "//tmp\\";
			if (_access((char*)pathCache.data(), 0) == -1)
			{
				if (_mkdir((char*)pathCache.data()))
					return ERR_CREATE_FAIL;
			}
			Status status = estimateTransform();

			if (status != OK)
				return status;

			return saveCameraParms();
		}
		

		else if (flag == 2)
		{
			//pathCache = name_imgs[0].substr(0, name_imgs[0].find_last_of('\\')) + "//tmp\\";
			readCameraParms();
	
			return composePanorama();
		}


		
	}





	Stitcher::Status Stitcher::matchImages()
	{
		if ((int)num_imgs < 2)
		{
			LOGLN("Need more images");
			return ERR_NEED_MORE_IMGS;
		}

		work_scale_ = 1;
		seam_work_aspect_ = 1;
		seam_scale_ = 1;
		bool is_work_scale_set = false;
		bool is_seam_scale_set = false;
		UMat full_img, img;
		features_.resize(num_imgs);
		name_seam_est_imgs_.resize(num_imgs);

		LOGLN("Finding features...");
#if ENABLE_LOG
		int64 t = getTickCount();
#endif


		for (size_t i = 0; i < num_imgs; ++i)
		{
			full_img = cv::imread(name_imgs_[i]).getUMat(ACCESS_FAST).clone();
		
			if (!is_work_scale_set)
			{
				origianl_img_size = full_img.size();
				work_scale_ = std::min(1.0, std::sqrt(registr_resol_ * 1e6 / full_img.size().area()));
				is_work_scale_set = true;
			}
			resize(full_img, img, Size(), work_scale_, work_scale_);

#if ENABLE_SEAM
			if (!is_seam_scale_set)
			{
				seam_scale_ = std::min(1.0, std::sqrt(seam_est_resol_ * 1e6 / full_img.size().area()));
				seam_work_aspect_ = seam_scale_ / work_scale_;
				is_seam_scale_set = true;
			}		

			resize(full_img, img, Size(), seam_scale_, seam_scale_);
			full_img.release();


			std::string name = stringGetFileNameNotExt(name_imgs_[i]);

			name_seam_est_imgs_[i] =pathCache+ name + "_seam_est.jpg";
			imwrite(name_seam_est_imgs_[i], img);
#endif

			(*features_finder_)(img, features_[i]);

			features_[i].img_idx = (int)i;
			LOGLN("Features in image #" << i + 1 << ": " << features_[i].keypoints.size());

			img.release();
		}

		// Do it to save memory
		features_finder_->collectGarbage();


		LOGLN("Finding features, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

		LOGLN("Pairwise matching...");
#if ENABLE_LOG
		t = getTickCount();
#endif


		//获取图像经纬度，组成匹配矩阵
		/////////////////////////////////////////////////////////
		std::vector<double> longitude;
		std::vector<double> latitude;
		ExifReader *pexifGPS = new ExifReader();


		if (this->exifFileName != "")
		{
			//从输入的txt中读取经纬度
			pexifGPS->readImageLonLatFromTxt(this->exifFileName, longitude, latitude);
		}
		else
		{
			//直接从图像中读取经纬度
			pexifGPS->readImageLonLat(this->name_imgs_, longitude, latitude);
		}
		
		if (NULL != pexifGPS)
			delete[] pexifGPS;

		Mat mask;
		SelectMatcher *seMa = new SelectMatcher();
		seMa->selectMatching(longitude, latitude, mask);

		if (NULL != seMa)
			delete[] seMa;

		longitude.clear();
		latitude.clear();


		matching_mask_ = mask.getUMat(ACCESS_FAST);

		///////////////////////////////////////////////////////////////
		(*features_matcher_)(features_, pairwise_matches_, matching_mask_);
		features_matcher_->collectGarbage();

		matching_mask_.release();



		LOGLN("Pairwise matching, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

		//去掉无法匹配的图像
		// Leave only images we are sure are from the same panorama
		indices_ = st::leaveBiggestComponent(features_, pairwise_matches_, (float)conf_thresh_);
		
		std::vector<std::string> name_seam_est_imgs_subset;
		std::vector<std::string> name_imgs_subset;

		for (size_t i = 0; i < indices_.size(); ++i)
		{
			name_imgs_subset.push_back(name_imgs_[indices_[i]]);
			name_seam_est_imgs_subset.push_back(name_seam_est_imgs_[indices_[i]]);

		}
		name_seam_est_imgs_ = name_seam_est_imgs_subset;
		name_imgs_ = name_imgs_subset;
		num_imgs =(int)name_imgs_.size();

		if ((int)num_imgs < 2)
		{
			LOGLN("Need more images");
			return ERR_NEED_MORE_IMGS;
		}

		return OK;
	}


	Stitcher::Status Stitcher::estimateCameraParams()
	{
#if ENABLE_LOG
		int64 t = getTickCount();
#endif
		LOGLN("Estimating Camera Params...");

		st::HomographyBasedEstimator estimator;
		if (!estimator(features_, pairwise_matches_, cameras_))
			return ERR_HOMOGRAPHY_EST_FAIL;

		for (size_t i = 0; i < cameras_.size(); ++i)
		{
			Mat R;
			cameras_[i].R.convertTo(R, CV_32F);
			cameras_[i].R = R;
			//LOGLN("Initial intrinsic parameters #" << indices_[i] + 1 << ":\n " << cameras_[i].K());
		}

		bundle_adjuster_->setConfThresh(conf_thresh_);
		if (!(*bundle_adjuster_)(features_, pairwise_matches_, cameras_))
			return ERR_CAMERA_PARAMS_ADJUST_FAIL;
		
		features_.clear();
		pairwise_matches_.clear();

		// Find median focal length and use it as final image scale
		std::vector<double> focals;
		for (size_t i = 0; i < cameras_.size(); ++i)
		{
			//LOGLN("Camera #" << indices_[i] + 1 << ":\n" << cameras_[i].K());
			focals.push_back(cameras_[i].focal);
		}

		std::sort(focals.begin(), focals.end());
		if (focals.size() % 2 == 1)
			warped_image_scale_ = static_cast<float>(focals[focals.size() / 2]);
		else
			warped_image_scale_ = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;

		if (do_wave_correct_)
		{
			std::vector<Mat> rmats;
			for (size_t i = 0; i < cameras_.size(); ++i)
				rmats.push_back(cameras_[i].R.clone());
			st::waveCorrect(rmats, wave_correct_kind_);
			for (size_t i = 0; i < cameras_.size(); ++i)
				cameras_[i].R = rmats[i];
		}
		LOGLN("Estimating Camera Params, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

		

		return OK;
	}


	Stitcher::Status Stitcher::saveCameraParms()
	{
		std::string pf = pathCache + parms_file;

		FileStorage fs(pf + ".xml", FileStorage::WRITE);

		if (!fs.isOpened())
			return ERR_SAVEPARMS_FAIL;

		fs << "registr_resol_" << registr_resol_;
		fs << "seam_est_resol_" << seam_est_resol_;
		fs << "compose_resol_" << compose_resol_;
		fs << "conf_thresh_" << conf_thresh_;
		fs << "do_wave_correct_" << do_wave_correct_;

		fs << "origianl_img_size" << origianl_img_size;
		fs << "indices_" << indices_;
		fs << "num_imgs" << num_imgs;

		for (int i = 0; i < num_imgs; i++) {
			fs <<  "cameras_focal_"+ to_string(i) << cameras_[i].focal;
			fs << "cameras_aspect_" + to_string(i) << cameras_[i].aspect;
			fs << "cameras_ppx_" + to_string(i) << cameras_[i].ppx;
			fs << "cameras_R_" + to_string(i) << cameras_[i].R;
			fs <<  "cameras_t_" + to_string(i) << cameras_[i].t;
		}

		fs << "work_scale_" << work_scale_;
		fs << "seam_scale_" << seam_scale_;
		fs << "seam_work_aspect_" << seam_work_aspect_;
		fs << "warped_image_scale_" << warped_image_scale_;		
	
		fs.release();


		ofstream file(pf+".txt");
		if (file)
		{
			for (int i = 0; i < num_imgs; i++)
			{
				file << name_imgs_[i] << endl;
			}

			for (int i = 0; i < num_imgs; i++)
			{
				file << name_seam_est_imgs_[i] << endl;
			}

			file.close();

		}
		else
			return ERR_SAVEPARMS_FAIL;

	}



	Stitcher::Status Stitcher::readCameraParms()
	{
		std::string pf = pathCache + parms_file;

		FileStorage fs(pf+".xml", FileStorage::READ);
		if (!fs.isOpened())
			return ERR_SAVEPARMS_FAIL;

		fs["registr_resol_"] >> registr_resol_;
		fs["seam_est_resol_"] >> seam_est_resol_;
		fs["compose_resol_"] >> compose_resol_;
		fs["conf_thresh_"] >> conf_thresh_;
		fs["do_wave_correct_"] >> do_wave_correct_;

		fs["origianl_img_size"] >>origianl_img_size;
		fs["indices_"] >> indices_;
		fs["num_imgs"] >> num_imgs;
		cameras_.resize(num_imgs);

		for (int i = 0; i < num_imgs; i++) {
			fs["cameras_focal_" + to_string(i)] >> cameras_[i].focal;
			fs["cameras_aspect_" + to_string(i)] >> cameras_[i].aspect;
			fs["cameras_ppx_" + to_string(i)] >> cameras_[i].ppx;
			fs["cameras_R_" + to_string(i)] >> cameras_[i].R;
			fs["cameras_t_" + to_string(i)] >> cameras_[i].t;
		}

		fs["work_scale_"] >> work_scale_;
		fs["seam_scale_"] >> seam_scale_;

		fs["seam_work_aspect_"] >> seam_work_aspect_;
		fs["warped_image_scale_"] >> warped_image_scale_;
		fs.release();
		name_imgs_.resize(num_imgs);
		name_seam_est_imgs_.resize(num_imgs);

		ifstream file(pf + ".txt");
		if (file)
		{
			for (int i = 0; i < num_imgs; i++)
			{
				file >> name_imgs_[i];
			}

			for (int i = 0; i < num_imgs; i++)
			{
				file >> name_seam_est_imgs_[i];
			}

			file.close();

		}
		else
			return ERR_SAVEPARMS_FAIL;
	}


	void Stitcher::releaseVectorUMat(std::vector<UMat> &mat)
	{
		for (size_t i = 0; i < mat.size(); ++i)
		{
			mat[i].release();
		}

		mat.clear();
	}
}
