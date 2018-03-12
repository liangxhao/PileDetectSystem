/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#include "stdafx.h"
//#include "precomp.hpp"
#include "SeamFinders.h"
#include "util_log.h"
#include <map>
#include "opencv2/stitching/detail/util.hpp"
#include "opencv2/stitching/detail/util_inl.hpp"
#include "opencv2/imgproc.hpp"

#include "D:\Library\OpenCV3.2.0\opencv\sources\modules\imgproc\src\gcgraph.hpp"


namespace st {
	//namespace detail {

	void PairwiseSeamFinder::find(const std::vector<std::string> &name_src, const std::vector<Point> &corners,
		std::vector<UMat> &masks)
	{
		LOGLN("Finding seams...");
		if (name_src.size() == 0)
			return;

#if ENABLE_LOG
		int64 t = getTickCount();
#endif

		names_images_ = name_src;
		

		corners_ = corners;
		masks_ = masks;
		run();

		LOGLN("Finding seams, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");
	}

	



	void PairwiseSeamFinder::run()
	{
		xfsr.open(tmp_path + dx_names_images_, FileStorage::READ);
		yfsr.open(tmp_path + dy_names_images_, FileStorage::READ);



		for (size_t i = 0; i < sizes_.size() - 1; ++i)
		{
			for (size_t j = i + 1; j < sizes_.size(); ++j)
			{
				Rect roi;
				if (detail::overlapRoi(corners_[i], corners_[j], sizes_[i], sizes_[j], roi))
					findInPair(i, j, roi);
			}
		}


		xfsr.release();
		yfsr.release();
	}

	

	namespace
	{

		template <typename T>
		float diffL2Square3(const Mat &image1, int y1, int x1, const Mat &image2, int y2, int x2)
		{
			const T *r1 = image1.ptr<T>(y1);
			const T *r2 = image2.ptr<T>(y2);
			return static_cast<float>(detail::sqr(r1[3 * x1] - r2[3 * x2]) + detail::sqr(r1[3 * x1 + 1] - r2[3 * x2 + 1]) +
				detail::sqr(r1[3 * x1 + 2] - r2[3 * x2 + 2]));
		}


		template <typename T>
		float diffL2Square4(const Mat &image1, int y1, int x1, const Mat &image2, int y2, int x2)
		{
			const T *r1 = image1.ptr<T>(y1);
			const T *r2 = image2.ptr<T>(y2);
			return static_cast<float>(detail::sqr(r1[4 * x1] - r2[4 * x2]) + detail::sqr(r1[4 * x1 + 1] - r2[4 * x2 + 1]) +
				detail::sqr(r1[4 * x1 + 2] - r2[4 * x2 + 2]));
		}

	} // namespace


	

	class GraphCutSeamFinder::Impl : public PairwiseSeamFinder
	{
	public:
		Impl(int cost_type, float terminal_cost, float bad_region_penalty)
			: cost_type_(cost_type), terminal_cost_(terminal_cost), bad_region_penalty_(bad_region_penalty) {}

		~Impl() {}

		void find(const std::vector<std::string> &name_src, const std::vector<Point> &corners, std::vector<UMat> &masks);
		void findInPair(size_t first, size_t second, Rect roi);

	private:
		void setGraphWeightsColor(const Mat &img1, const Mat &img2,
			const Mat &mask1, const Mat &mask2, GCGraph<float> &graph);
		void setGraphWeightsColorGrad(const Mat &img1, const Mat &img2, const Mat &dx1, const Mat &dx2,
			const Mat &dy1, const Mat &dy2, const Mat &mask1, const Mat &mask2,
			GCGraph<float> &graph);

		//std::vector<Mat> dx_, dy_;
		int cost_type_;
		float terminal_cost_;
		float bad_region_penalty_;
	};



	void GraphCutSeamFinder::Impl::find(const std::vector<std::string> &name_src, const std::vector<Point> &corners,
		std::vector<UMat> &masks)
	{
		tmp_path = name_src[0].substr(0, name_src[0].find_last_of('//')) + "//";
		// Compute gradients
	/*	dx_.resize(name_src.size());
		dy_.resize(name_src.size());*/
		sizes_.resize(name_src.size());


		FileStorage xfs(tmp_path + dx_names_images_, FileStorage::WRITE); 
		FileStorage yfs(tmp_path + dy_names_images_, FileStorage::WRITE);

		Mat dx, dy;
		Mat dx_, dy_;
		for (size_t i = 0; i < name_src.size(); ++i)
		{
			Mat img = cv::imread(name_src[i]);
			sizes_[i] = img.size();
			Mat img_f;
			img.convertTo(img_f, CV_32F);
			img.release();

			Sobel(img_f, dx, CV_32F, 1, 0);
			Sobel(img_f, dy, CV_32F, 0, 1);
			img_f.release();


			dx_.create(sizes_[i], CV_32F);
			dy_.create(sizes_[i], CV_32F);

			for (int y = 0; y < sizes_[i].height; ++y)
			{
				const Point3f* dx_row = dx.ptr<Point3f>(y);
				const Point3f* dy_row = dy.ptr<Point3f>(y);
				float* dx_row_ = dx_.ptr<float>(y);
				float* dy_row_ = dy_.ptr<float>(y);
				for (int x = 0; x < sizes_[i].width; ++x)
				{
					dx_row_[x] = detail::normL2(dx_row[x]);
					dy_row_[x] = detail::normL2(dy_row[x]);
				}
			}
			

			xfs << "x"+std::to_string(i) << dx_;
			yfs << "y"+std::to_string(i) << dy_;
			dx_.release();
			dy_.release();
		}

		xfs.release();
		yfs.release();


		PairwiseSeamFinder::find(name_src, corners, masks);
	}


	void GraphCutSeamFinder::Impl::setGraphWeightsColor(const Mat &img1, const Mat &img2,
		const Mat &mask1, const Mat &mask2, GCGraph<float> &graph)
	{
		const Size img_size = img1.size();

		// Set terminal weights
		for (int y = 0; y < img_size.height; ++y)
		{
			for (int x = 0; x < img_size.width; ++x)
			{
				int v = graph.addVtx();
				graph.addTermWeights(v, mask1.at<uchar>(y, x) ? terminal_cost_ : 0.f,
					mask2.at<uchar>(y, x) ? terminal_cost_ : 0.f);
			}
		}

		// Set regular edge weights
		const float weight_eps = 1.f;
		for (int y = 0; y < img_size.height; ++y)
		{
			for (int x = 0; x < img_size.width; ++x)
			{
				int v = y * img_size.width + x;
				if (x < img_size.width - 1)
				{
					float weight = detail::normL2(img1.at<Point3f>(y, x), img2.at<Point3f>(y, x)) +
						detail::normL2(img1.at<Point3f>(y, x + 1), img2.at<Point3f>(y, x + 1)) +
						weight_eps;
					if (!mask1.at<uchar>(y, x) || !mask1.at<uchar>(y, x + 1) ||
						!mask2.at<uchar>(y, x) || !mask2.at<uchar>(y, x + 1))
						weight += bad_region_penalty_;
					graph.addEdges(v, v + 1, weight, weight);
				}
				if (y < img_size.height - 1)
				{
					float weight = detail::normL2(img1.at<Point3f>(y, x), img2.at<Point3f>(y, x)) +
						detail::normL2(img1.at<Point3f>(y + 1, x), img2.at<Point3f>(y + 1, x)) +
						weight_eps;
					if (!mask1.at<uchar>(y, x) || !mask1.at<uchar>(y + 1, x) ||
						!mask2.at<uchar>(y, x) || !mask2.at<uchar>(y + 1, x))
						weight += bad_region_penalty_;
					graph.addEdges(v, v + img_size.width, weight, weight);
				}
			}
		}
	}


	void GraphCutSeamFinder::Impl::setGraphWeightsColorGrad(const Mat &img1, const Mat &img2, const Mat &dx1, const Mat &dx2,
		const Mat &dy1, const Mat &dy2, const Mat &mask1, const Mat &mask2,
		GCGraph<float> &graph)
	{
		const Size img_size = img1.size();

		// Set terminal weights
		for (int y = 0; y < img_size.height; ++y)
		{
			for (int x = 0; x < img_size.width; ++x)
			{
				int v = graph.addVtx();
				graph.addTermWeights(v, mask1.at<uchar>(y, x) ? terminal_cost_ : 0.f,
					mask2.at<uchar>(y, x) ? terminal_cost_ : 0.f);
			}
		}

		// Set regular edge weights
		const float weight_eps = 1.f;
		for (int y = 0; y < img_size.height; ++y)
		{
			for (int x = 0; x < img_size.width; ++x)
			{
				int v = y * img_size.width + x;
				if (x < img_size.width - 1)
				{
					float grad = dx1.at<float>(y, x) + dx1.at<float>(y, x + 1) +
						dx2.at<float>(y, x) + dx2.at<float>(y, x + 1) + weight_eps;
					float weight = (detail::normL2(img1.at<Point3f>(y, x), img2.at<Point3f>(y, x)) +
						detail::normL2(img1.at<Point3f>(y, x + 1), img2.at<Point3f>(y, x + 1))) / grad +
						weight_eps;
					if (!mask1.at<uchar>(y, x) || !mask1.at<uchar>(y, x + 1) ||
						!mask2.at<uchar>(y, x) || !mask2.at<uchar>(y, x + 1))
						weight += bad_region_penalty_;
					graph.addEdges(v, v + 1, weight, weight);
				}
				if (y < img_size.height - 1)
				{
					float grad = dy1.at<float>(y, x) + dy1.at<float>(y + 1, x) +
						dy2.at<float>(y, x) + dy2.at<float>(y + 1, x) + weight_eps;
					float weight = (detail::normL2(img1.at<Point3f>(y, x), img2.at<Point3f>(y, x)) +
						detail::normL2(img1.at<Point3f>(y + 1, x), img2.at<Point3f>(y + 1, x))) / grad +
						weight_eps;
					if (!mask1.at<uchar>(y, x) || !mask1.at<uchar>(y + 1, x) ||
						!mask2.at<uchar>(y, x) || !mask2.at<uchar>(y + 1, x))
						weight += bad_region_penalty_;
					graph.addEdges(v, v + img_size.width, weight, weight);
				}
			}
		}
	}


	void GraphCutSeamFinder::Impl::findInPair(size_t first, size_t second, Rect roi)
	{
		LOGLN("Seam Finder between #" << first+1 << " and #" << second+1)

		Mat img1, img2;
		Mat im1 = imread(names_images_[first]);
		im1.convertTo(img1, CV_32F);
		im1.release();
		Mat im2 = imread(names_images_[second]);
		im2.convertTo(img2, CV_32F);
		im2.release();
		

		
		//Mat dx1 = dx_[first], dx2 = dx_[second];
		//Mat dy1 = dy_[first], dy2 = dy_[second];
		Mat dx1, dx2, dy1, dy2;

		xfsr["x"+std::to_string(first)] >> dx1;
		xfsr["x" + std::to_string(second)] >> dx2;
		yfsr["y" + std::to_string(first)] >> dy1;
		yfsr["y" + std::to_string(second)] >> dy2;

		Mat mask1 = masks_[first].getMat(ACCESS_RW), mask2 = masks_[second].getMat(ACCESS_RW);
		Point tl1 = corners_[first], tl2 = corners_[second];

		const int gap = 10;
		Mat subimg1(roi.height + 2 * gap, roi.width + 2 * gap, CV_32FC3);
		Mat subimg2(roi.height + 2 * gap, roi.width + 2 * gap, CV_32FC3);
		Mat submask1(roi.height + 2 * gap, roi.width + 2 * gap, CV_8U);
		Mat submask2(roi.height + 2 * gap, roi.width + 2 * gap, CV_8U);
		Mat subdx1(roi.height + 2 * gap, roi.width + 2 * gap, CV_32F);
		Mat subdy1(roi.height + 2 * gap, roi.width + 2 * gap, CV_32F);
		Mat subdx2(roi.height + 2 * gap, roi.width + 2 * gap, CV_32F);
		Mat subdy2(roi.height + 2 * gap, roi.width + 2 * gap, CV_32F);

		// Cut subimages and submasks with some gap
		for (int y = -gap; y < roi.height + gap; ++y)
		{
			for (int x = -gap; x < roi.width + gap; ++x)
			{
				int y1 = roi.y - tl1.y + y;
				int x1 = roi.x - tl1.x + x;
				if (y1 >= 0 && x1 >= 0 && y1 < img1.rows && x1 < img1.cols)
				{
					subimg1.at<Point3f>(y + gap, x + gap) = img1.at<Point3f>(y1, x1);
					submask1.at<uchar>(y + gap, x + gap) = mask1.at<uchar>(y1, x1);
					subdx1.at<float>(y + gap, x + gap) = dx1.at<float>(y1, x1);
					subdy1.at<float>(y + gap, x + gap) = dy1.at<float>(y1, x1);
				}
				else
				{
					subimg1.at<Point3f>(y + gap, x + gap) = Point3f(0, 0, 0);
					submask1.at<uchar>(y + gap, x + gap) = 0;
					subdx1.at<float>(y + gap, x + gap) = 0.f;
					subdy1.at<float>(y + gap, x + gap) = 0.f;
				}

				int y2 = roi.y - tl2.y + y;
				int x2 = roi.x - tl2.x + x;
				if (y2 >= 0 && x2 >= 0 && y2 < img2.rows && x2 < img2.cols)
				{
					subimg2.at<Point3f>(y + gap, x + gap) = img2.at<Point3f>(y2, x2);
					submask2.at<uchar>(y + gap, x + gap) = mask2.at<uchar>(y2, x2);
					subdx2.at<float>(y + gap, x + gap) = dx2.at<float>(y2, x2);
					subdy2.at<float>(y + gap, x + gap) = dy2.at<float>(y2, x2);
				}
				else
				{
					subimg2.at<Point3f>(y + gap, x + gap) = Point3f(0, 0, 0);
					submask2.at<uchar>(y + gap, x + gap) = 0;
					subdx2.at<float>(y + gap, x + gap) = 0.f;
					subdy2.at<float>(y + gap, x + gap) = 0.f;
				}
			}
		}

		img1.release();
		img2.release();


		const int vertex_count = (roi.height + 2 * gap) * (roi.width + 2 * gap);
		const int edge_count = (roi.height - 1 + 2 * gap) * (roi.width + 2 * gap) +
			(roi.width - 1 + 2 * gap) * (roi.height + 2 * gap);
		GCGraph<float> graph(vertex_count, edge_count);

		switch (cost_type_)
		{
		case GraphCutSeamFinder::COST_COLOR:
			setGraphWeightsColor(subimg1, subimg2, submask1, submask2, graph);
			break;
		case GraphCutSeamFinder::COST_COLOR_GRAD:
			setGraphWeightsColorGrad(subimg1, subimg2, subdx1, subdx2, subdy1, subdy2,
				submask1, submask2, graph);
			break;
		default:
			CV_Error(Error::StsBadArg, "unsupported pixel similarity measure");
		}

		graph.maxFlow();

		for (int y = 0; y < roi.height; ++y)
		{
			for (int x = 0; x < roi.width; ++x)
			{
				if (graph.inSourceSegment((y + gap) * (roi.width + 2 * gap) + x + gap))
				{
					if (mask1.at<uchar>(roi.y - tl1.y + y, roi.x - tl1.x + x))
						mask2.at<uchar>(roi.y - tl2.y + y, roi.x - tl2.x + x) = 0;
				}
				else
				{
					if (mask2.at<uchar>(roi.y - tl2.y + y, roi.x - tl2.x + x))
						mask1.at<uchar>(roi.y - tl1.y + y, roi.x - tl1.x + x) = 0;
				}
			}
		}
	}


	GraphCutSeamFinder::GraphCutSeamFinder(int cost_type, float terminal_cost, float bad_region_penalty)
		: impl_(new Impl(cost_type, terminal_cost, bad_region_penalty)) {}

	GraphCutSeamFinder::~GraphCutSeamFinder() {}


	void GraphCutSeamFinder::find(const std::vector<std::string> &name_src, const std::vector<Point> &corners,
		std::vector<UMat> &masks)
	{


		
		impl_->find(name_src, corners, masks);
	}



} // namespace cv
