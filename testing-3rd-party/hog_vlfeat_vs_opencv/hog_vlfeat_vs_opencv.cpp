// hog_vlfeat_vs_opencv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <numeric>
#include <algorithm>

#include <opencv2\opencv.hpp>
#include <vl\hog.h>

//#include <dlib\array.h>
//#include <dlib\array2d.h>
#include <dlib\image_keypoint.h>
//#include <dlib\image_processing.h>
#include <dlib\opencv.h>

void test_vlfeat();

cv::Mat get_hogdescriptor_visual_image(const cv::Mat& origImg,
	std::vector<float>& descriptorValues,
	cv::Size winSize,
	cv::Size cellSize,
	int scaleFactor,
	double viz_factor);

int _tmain(int argc, _TCHAR* argv[])
{
	test_vlfeat();
	return 0;

	cv::Mat im, fim;
	im.create(128, 64, CV_8U);
	cv::theRNG().fill(im, cv::RNG::UNIFORM, 0, 255);

	im.convertTo(fim, CV_32F, 1.0);

	VlHog* hog = vl_hog_new(VlHogVariantDalalTriggs, 9, 0);
	//vl_hog_set_use_bilinear_orientation_assignments(hog, 1);
	vl_hog_put_image(hog, (float*)fim.data, 64, 128, 1, 8);

	auto hog_width = vl_hog_get_width(hog);
	auto hog_height = vl_hog_get_height(hog);
	auto hog_dim = vl_hog_get_dimension(hog);

	std::vector<int> perm(hog_dim);
	auto p = vl_hog_get_permutation(hog);
	for (int i = 0; i < hog_dim; i++) {
		perm[i] = p[i];
	}


	std::vector<float> hog_array(hog_width*hog_height*hog_dim);
	vl_hog_extract(hog, hog_array.data());

	vl_hog_delete(hog);

	cv::HOGDescriptor hog2;
	hog2.winSize = cv::Size(64, 128);
	hog2.cellSize = cv::Size(8, 8);
	hog2.blockSize = cv::Size(16, 16);
	hog2.blockStride = cv::Size(8, 8);
	hog2.nbins = 9;
	//hog2.gammaCorrection = false;

	auto hog_dim2 = hog2.getDescriptorSize();

	std::vector<float> hog_array2;
	hog2.compute(im, hog_array2);


	std::vector<float> cell1(36), cell2(36);

	for (size_t i = 0; i < 36; i++) {
		cell1[i] = hog_array[36 * 8 + i];
		cell2[i] = hog_array2[i];
	}


	auto dlib_im = dlib::cv_image<unsigned char>(im);
	dlib::hog_image<8, 2, 8, 9, dlib::hog_unsigned_gradient, dlib::hog_angle_interpolation> hog_;

	hog_.load(dlib_im);

	{
		auto sz = hog_.size();
		auto nr = hog_.nr();
		auto nc = hog_.nc();
		auto ndim = hog_.get_num_dimensions();


		auto r = hog_.get_block_rect(0, 0);

		auto xdas = hog_(0, 0);

		int gduohosd = 0;
	}
	return 0;
}

void test_vlfeat() {

	cv::Mat src = cv::imread(R"(d:\DATASET\INRIAPerson\INRIAPerson.1\INRIAPerson\70X134H96\Test\pos\crop_000001a.png)", cv::IMREAD_GRAYSCALE);


	cv::Mat im;
	src.convertTo(im, CV_32F, 1.0 / 255.0);
	auto im_width = im.cols;
	auto im_height = im.rows;


	VlHog* hog = vl_hog_new(VlHogVariantDalalTriggs, 9, 0);
	vl_hog_set_use_bilinear_orientation_assignments(hog, 1);
	vl_hog_put_image(hog, (float*)im.data, im_width, im_height, 1, 8);

	auto hog_im_width = vl_hog_get_width(hog);
	auto hog_im_height = vl_hog_get_height(hog);
	auto hog_im_dim = vl_hog_get_dimension(hog);
	auto hog_im_stride = hog_im_width*hog_im_height;
	size_t glyph_size = vl_hog_get_glyph_size(hog);

	std::vector<float> im_feats(hog_im_width*hog_im_height*hog_im_dim);
	vl_hog_extract(hog, im_feats.data());
	cv::Mat im_render = cv::Mat::zeros(hog_im_height * glyph_size, hog_im_width * glyph_size, CV_32F);
	vl_hog_render(hog, im_render.ptr<float>(), im_feats.data(), hog_im_width, hog_im_height);
	//cv::normalize(im_render, im_render, 255.0, 0.0, cv::NORM_MINMAX, CV_8U);
	cv::imshow("im_render", im_render);
	//cv::waitKey();

	cv::Mat roi = im({ 0, 0, 64, 128 }).clone();
	auto roi_width = roi.cols;
	auto roi_height = roi.rows;

	vl_hog_put_image(hog, roi.ptr<float>(), roi_width, roi_height, 1, 8);

	auto hog_roi_width = vl_hog_get_width(hog);
	auto hog_roi_height = vl_hog_get_height(hog);
	auto hog_roi_dim = vl_hog_get_dimension(hog);
	auto hog_roi_stride = hog_roi_width * hog_roi_height;

	std::vector<float> roi_feats(hog_roi_width*hog_roi_height*hog_roi_dim);
	vl_hog_extract(hog, roi_feats.data());


	std::vector<float> my_feats(8 * 16 * 4 * 9);

	for (int k = 0; k < 36; k++) {
		for (int y = 0; y < 16; y++) {
			for (int x = 0; x < 8; x++) {
				int i = x + hog_im_width * y + hog_im_stride * k;
				int j = x + hog_roi_width * y + hog_roi_stride * k;
				my_feats[j] = im_feats[i];
			}
		}
	}

	//size_t glyph_size = vl_hog_get_glyph_size(hog);

	cv::Mat roi_render = cv::Mat::zeros(16 * glyph_size, 8 * glyph_size, CV_32F);
	cv::Mat my_render = cv::Mat::zeros(16 * glyph_size, 8 * glyph_size, CV_32F);


	vl_hog_render(hog, roi_render.ptr<float>(), roi_feats.data(), 8, 16);
	vl_hog_render(hog, my_render.ptr<float>(), my_feats.data(), 8, 16);


	std::vector<float> diff_feat(my_feats.size());
	for (size_t i = 0; i < my_feats.size(); i++) {
		diff_feat[i] = fabs(my_feats[i] - roi_feats[i]);
	}

	float my_feats_sum = std::accumulate(my_feats.begin(), my_feats.end(), 0.0f);
	float roi_feats_sum = std::accumulate(roi_feats.begin(), roi_feats.end(), 0.0f);
	float diff_sum = std::accumulate(diff_feat.begin(), diff_feat.end(), 0.0f);

	cv::imshow("roi_render", roi_render);
	cv::imshow("my_render", my_render);
	cv::waitKey();

	cv::Mat roi1 = src({ 0, 0, 64, 128 }).clone();

	cv::HOGDescriptor hog2;
	hog2.winSize = cv::Size(64, 128);
	hog2.cellSize = cv::Size(8, 8);
	hog2.blockSize = cv::Size(16, 16);
	hog2.blockStride = cv::Size(8, 8);
	hog2.nbins = 9;
	//hog2.gammaCorrection = false;

	std::vector<float> hog_array2;
	hog2.compute(roi1, hog_array2);

	std::vector<float> my_feats2(7 * 15 * 4 * 9);

	size_t offset = 0;
	for (int x = 0; x < 8 - 1; x++) {
		for (int y = 0; y < 16 - 1; y++) {
			for (int k = 0; k < 36; k++) {
				int i = x + hog_roi_width * y + hog_roi_stride * k;
				int j = offset++;				
				my_feats2[j] = roi_feats[i];				
			}
		}
	}

	vl_hog_delete(hog);

	cv::Mat viz1 = get_hogdescriptor_visual_image(roi1, hog_array2, { 64, 128 }, { 8, 8 }, 3, 3);
	cv::Mat viz2 = get_hogdescriptor_visual_image(roi1, my_feats2, { 64, 128 }, { 8, 8 }, 3, 3);

	cv::imshow("viz1", viz1);
	cv::imshow("viz2", viz2);
	cv::waitKey();
}

cv::Mat get_hogdescriptor_visual_image(const cv::Mat& origImg,
	std::vector<float>& descriptorValues,
	cv::Size winSize,
	cv::Size cellSize,
	int scaleFactor,
	double viz_factor)
{
	using namespace cv;
	Mat visual_image;
	resize(origImg, visual_image, Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));

	int gradientBinSize = 9;
	// dividing 180° into 9 bins, how large (in rad) is one bin?
	float radRangeForOneBin = 3.14 / (float)gradientBinSize;

	// prepare data structure: 9 orientation / gradient strenghts for each cell
	int cells_in_x_dir = winSize.width / cellSize.width;
	int cells_in_y_dir = winSize.height / cellSize.height;
	int totalnrofcells = cells_in_x_dir * cells_in_y_dir;
	float*** gradientStrengths = new float**[cells_in_y_dir];
	int** cellUpdateCounter = new int*[cells_in_y_dir];
	for (int y = 0; y<cells_in_y_dir; y++)
	{
		gradientStrengths[y] = new float*[cells_in_x_dir];
		cellUpdateCounter[y] = new int[cells_in_x_dir];
		for (int x = 0; x<cells_in_x_dir; x++)
		{
			gradientStrengths[y][x] = new float[gradientBinSize];
			cellUpdateCounter[y][x] = 0;

			for (int bin = 0; bin<gradientBinSize; bin++)
				gradientStrengths[y][x][bin] = 0.0;
		}
	}

	// nr of blocks = nr of cells - 1
	// since there is a new block on each cell (overlapping blocks!) but the last one
	int blocks_in_x_dir = cells_in_x_dir - 1;
	int blocks_in_y_dir = cells_in_y_dir - 1;

	// compute gradient strengths per cell
	int descriptorDataIdx = 0;
	int cellx = 0;
	int celly = 0;

	for (int blockx = 0; blockx<blocks_in_x_dir; blockx++)
	{
		for (int blocky = 0; blocky<blocks_in_y_dir; blocky++)
		{
			// 4 cells per block ...
			for (int cellNr = 0; cellNr<4; cellNr++)
			{
				// compute corresponding cell nr
				int cellx = blockx;
				int celly = blocky;
				if (cellNr == 1) celly++;
				if (cellNr == 2) cellx++;
				if (cellNr == 3)
				{
					cellx++;
					celly++;
				}

				for (int bin = 0; bin<gradientBinSize; bin++)
				{
					float gradientStrength = descriptorValues[descriptorDataIdx];
					descriptorDataIdx++;

					gradientStrengths[celly][cellx][bin] += gradientStrength;

				} // for (all bins)


				// note: overlapping blocks lead to multiple updates of this sum!
				// we therefore keep track how often a cell was updated,
				// to compute average gradient strengths
				cellUpdateCounter[celly][cellx]++;

			} // for (all cells)


		} // for (all block x pos)
	} // for (all block y pos)


	// compute average gradient strengths
	for (int celly = 0; celly<cells_in_y_dir; celly++)
	{
		for (int cellx = 0; cellx<cells_in_x_dir; cellx++)
		{

			float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

			// compute average gradient strenghts for each gradient bin direction
			for (int bin = 0; bin<gradientBinSize; bin++)
			{
				gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
			}
		}
	}


	std::cout << "descriptorDataIdx = " << descriptorDataIdx << std::endl;

	// draw cells
	for (int celly = 0; celly<cells_in_y_dir; celly++)
	{
		for (int cellx = 0; cellx<cells_in_x_dir; cellx++)
		{
			int drawX = cellx * cellSize.width;
			int drawY = celly * cellSize.height;

			int mx = drawX + cellSize.width / 2;
			int my = drawY + cellSize.height / 2;

			rectangle(visual_image,
				Point(drawX*scaleFactor, drawY*scaleFactor),
				Point((drawX + cellSize.width)*scaleFactor,
				(drawY + cellSize.height)*scaleFactor),
				CV_RGB(100, 100, 100),
				1);

			// draw in each cell all 9 gradient strengths
			for (int bin = 0; bin<gradientBinSize; bin++)
			{
				float currentGradStrength = gradientStrengths[celly][cellx][bin];

				// no line to draw?
				if (currentGradStrength == 0)
					continue;

				float currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;

				float dirVecX = cos(currRad);
				float dirVecY = sin(currRad);
				float maxVecLen = cellSize.width / 2;
				float scale = viz_factor; // just a visual_imagealization scale,
				// to see the lines better

				// compute line coordinates
				float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
				float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
				float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
				float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

				// draw gradient visual_imagealization
				line(visual_image,
					Point(x1*scaleFactor, y1*scaleFactor),
					Point(x2*scaleFactor, y2*scaleFactor),
					CV_RGB(0, 0, 255),
					1);

			} // for (all bins)

		} // for (cellx)
	} // for (celly)


	// don't forget to free memory allocated by helper data structures!
	for (int y = 0; y<cells_in_y_dir; y++)
	{
		for (int x = 0; x<cells_in_x_dir; x++)
		{
			delete[] gradientStrengths[y][x];
		}
		delete[] gradientStrengths[y];
		delete[] cellUpdateCounter[y];
	}
	delete[] gradientStrengths;
	delete[] cellUpdateCounter;

	return visual_image;

}

