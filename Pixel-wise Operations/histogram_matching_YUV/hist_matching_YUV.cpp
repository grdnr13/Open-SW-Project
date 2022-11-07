#include "hist_func.h"

void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF);
void hist_match(Mat& input, Mat& output, G* trans_func);
void change_one_to_one(G* trans_func_eq2, G* trans_func_match);

int main() {

	Mat input = imread("input.jpg", CV_LOAD_IMAGE_COLOR);
	Mat reference = imread("reference_img2.jpg", CV_LOAD_IMAGE_COLOR);
	if (!reference.data) {
		std::cout << "reference image not found\n";
		return -1;
	}

	Mat equalized_YUV;
	Mat ref_equalized_YUV;
	Mat matched_YUV;

	cvtColor(input, equalized_YUV, CV_RGB2YUV);	// RGB -> YUV
	cvtColor(reference, ref_equalized_YUV, CV_RGB2YUV);	// RGB -> YUV
	cvtColor(input, matched_YUV, CV_RGB2YUV);	// RGB -> YUV

	// split each channel(Y, U, V)
	Mat channels[3];
	split(equalized_YUV, channels);
	Mat Y = channels[0];						// U = channels[1], V = channels[2]
	
	//reference -> split each channel(Y, U, V)
	Mat ref_channels[3];
	split(ref_equalized_YUV, ref_channels);
	Mat ref_Y = ref_channels[0];

	// split each channel(Y, U, V)
	Mat matched_channels[3];
	split(matched_YUV, matched_channels);
	Mat matched_Y = matched_channels[0];

	// PDF or transfer function txt files
	FILE* f_matched_PDF_RGB, * f_PDF_RGB;
	FILE* f_trans_func_match_Y;
	FILE* f_reference_PDF_RGB;

	FILE* f_matched_PDF_R, * f_matched_PDF_G, * f_matched_PDF_B;
	FILE* f_PDF_R, * f_PDF_G, * f_PDF_B;
	FILE* f_reference_PDF_R, * f_reference_PDF_G, * f_reference_PDF_B;


	float** PDF_RGB = cal_PDF_RGB(input);		// PDF of Input image(RGB) : [L][3]
	float* CDF_YUV = cal_CDF(Y);				// CDF of Y channel image

	float** ref_PDF_RGB = cal_PDF_RGB(reference);		// PDF of reference image(RGB) : [L][3]
	float* ref_CDF_YUV = cal_CDF(ref_Y);				// CDF of reference Y channel image


	fopen_s(&f_PDF_RGB, "PDF_RGB.txt", "w+");
	fopen_s(&f_reference_PDF_RGB, "PDF_reference_RGB.txt", "w+");
	fopen_s(&f_matched_PDF_RGB, "matched_PDF_RGB.txt", "w+");
	fopen_s(&f_trans_func_match_Y, "trans_func_match_Y.txt", "w+");

	fopen_s(&f_matched_PDF_R, "matched_PDF_R.txt", "w+");
	fopen_s(&f_matched_PDF_G, "matched_PDF_G.txt", "w+");
	fopen_s(&f_matched_PDF_B, "matched_PDF_B.txt", "w+");


	fopen_s(&f_PDF_R, "PDF_R.txt", "w+");
	fopen_s(&f_PDF_G, "PDF_G.txt", "w+");
	fopen_s(&f_PDF_B, "PDF_B.txt", "w+");

	fopen_s(&f_reference_PDF_R, "PDF_reference_R.txt", "w+");
	fopen_s(&f_reference_PDF_G, "PDF_reference_G.txt", "w+");
	fopen_s(&f_reference_PDF_B, "PDF_reference_B.txt", "w+");

	G trans_func_eq_YUV[L] = { 0 };			// transfer function
	G trans_func_eq2_YUV[L] = { 0 };		//G(z)
	G trans_func_match_YUV[L] = { 0 };		//matching function

	// histogram equalization on Y channel
	hist_eq(Y, channels[0], trans_func_eq_YUV, CDF_YUV);
	hist_eq(ref_Y, ref_channels[0], trans_func_eq2_YUV, ref_CDF_YUV);

	//일대일대응
	change_one_to_one(trans_func_eq2_YUV, trans_func_match_YUV);
	//match
	hist_match(Y, matched_Y, trans_func_match_YUV);

	// merge Y, U, V channels
	merge(matched_channels, 3, matched_YUV);

	// YUV -> RGB (use "CV_YUV2RGB" flag)
	cvtColor(matched_YUV, matched_YUV, CV_YUV2RGB);

	// matched PDF (YUV)
	float** matched_PDF_RGB = cal_PDF_RGB(matched_YUV);

	for (int i = 0; i < L; i++) {
		// write PDF
		fprintf(f_PDF_R, "%d\t%f\n", i, PDF_RGB[i][0]);
		fprintf(f_PDF_G, "%d\t%f\n", i, PDF_RGB[i][1]);
		fprintf(f_PDF_B, "%d\t%f\n", i, PDF_RGB[i][2]);

		fprintf(f_reference_PDF_R, "%d\t%f\n", i, ref_PDF_RGB[i][0]);
		fprintf(f_reference_PDF_G, "%d\t%f\n", i, ref_PDF_RGB[i][1]);
		fprintf(f_reference_PDF_B, "%d\t%f\n", i, ref_PDF_RGB[i][2]);

		fprintf(f_matched_PDF_R, "%d\t%f\n", i, matched_PDF_RGB[i][0]);
		fprintf(f_matched_PDF_G, "%d\t%f\n", i, matched_PDF_RGB[i][1]);
		fprintf(f_matched_PDF_B, "%d\t%f\n", i, matched_PDF_RGB[i][2]);


		for (int j = 0; j < 3; j++) {
			fprintf(f_PDF_RGB, "%d\t%f\n", i, PDF_RGB[i][j]);
			fprintf(f_reference_PDF_RGB, "%d\t%f\n", i, ref_PDF_RGB[i][j]);
			fprintf(f_matched_PDF_RGB, "%d\t%f\n", i, matched_PDF_RGB[i][j]);
		}
		// write transfer functions
		fprintf(f_trans_func_match_Y, "%d\t%d\n", i, trans_func_eq_YUV[i]);
	}

	// memory release
	free(PDF_RGB);
	free(CDF_YUV);
	fclose(f_matched_PDF_R); fclose(f_matched_PDF_G); fclose(f_matched_PDF_B);
	fclose(f_PDF_RGB); fclose(f_PDF_R); fclose(f_PDF_G); fclose(f_PDF_B);
	fclose(f_matched_PDF_RGB);
	fclose(f_trans_func_match_Y);
	fclose(f_reference_PDF_RGB);
	fclose(f_reference_PDF_R); fclose(f_reference_PDF_G); fclose(f_reference_PDF_B);

	////////////////////// Show each image ///////////////////////

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	//namedWindow("Reference", WINDOW_AUTOSIZE);
	//imshow("Reference", reference);

	namedWindow("Matched_YUV", WINDOW_AUTOSIZE);
	imshow("Matched_YUV", matched_YUV);

	//////////////////////////////////////////////////////////////

	waitKey(0);

	return 0;
}

// histogram equalization
void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF) {

	// compute transfer function
	for (int i = 0; i < L; i++)
		trans_func[i] = (G)((L - 1) * CDF[i]);

	// perform the transfer function
	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++)
			equalized.at<G>(i, j) = trans_func[input.at<G>(i, j)];
}

void change_one_to_one(G* trans_func_eq2, G* trans_func_match) {
	trans_func_match[trans_func_eq2[0]] = 0;
	for (int i = 1; i < L; i++) {
		if (trans_func_match[trans_func_eq2[i]] == 0)
			trans_func_match[trans_func_eq2[i]] = i;
		else {
			if (trans_func_eq2[i] + 1 < 256)
				trans_func_match[trans_func_eq2[i] + 1] = i;
		}
	}
	for (int i = 1; i < L; i++) {
		if (trans_func_match[i] == 0)
			trans_func_match[i] = min((trans_func_match[i - 1] + 1), 255);
	}

}

// histogram matching
void hist_match(Mat& input, Mat& output, G* trans_func) {

	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++)
			output.at<G>(i, j) = trans_func[input.at<G>(i, j)];
}